/**
 * @file Camera.cpp
 * @author ayano
 * @date 2/7/24
 * @brief
 */

#include "Camera.h"
#include <string>
#include "Eigen/Core"
#include "Eigen/Geometry"
#include "ImageUtil.h"
#include "Material.h"

std::string Camera::Render(const IHittable &world, const std::string &name, const std::string &path) {
	auto now = std::chrono::system_clock::now();
	int worker_cnt;
	if (render_thread_count == 0) {
		worker_cnt = std::thread::hardware_concurrency() == 0 ? 12 : std::thread::hardware_concurrency();
	} else {
		worker_cnt = render_thread_count;
	}
	std::vector<std::vector<Color>> image;
	image.resize(height);
	for (auto &i: image) {
		i.resize(width);
	}
	int part = partition();
	auto m = KawaiiMQ::MessageQueueManager::Instance();
	auto result_queue = KawaiiMQ::makeQueue("result");
	auto result_topic = KawaiiMQ::Topic("renderResult");
	m->relate(result_topic, result_queue);
	auto th = std::vector<std::thread>();
	spdlog::info("rendering started!");
	spdlog::info("using {} threads to render {} blocks", worker_cnt, part);
	auto begin = std::chrono::system_clock::now();
	for (int i = 0; i < worker_cnt; i++) {
		th.emplace_back(&Camera::RenderWorker, this, std::ref(world));
	}
	for (auto &i: th) {
		i.join();
	}
	auto end = std::chrono::system_clock::now();
	auto time_elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end - begin);
	spdlog::info("render completed! taken {}s", static_cast<float>(time_elapsed.count()) / 1000.0);
	auto consumer = KawaiiMQ::Consumer("resultConsumer");
	consumer.subscribe(result_topic);
	while (!result_queue->empty()) {
		auto chunk = KawaiiMQ::getMessage<ImageChunk>(consumer.fetchSingleTopic(result_topic)[0]);
		for (int i = chunk.starty; i < chunk.starty + chunk.height; i++) {
			for (int j = chunk.startx; j < chunk.startx + chunk.width; j++) {
				image[i][j] = chunk.partial[i - chunk.starty][j - chunk.startx];
			}
		}
	}
	m->unrelate(result_topic, result_queue);
#ifndef ASCII_ART
	return makePPM(width, height, image, name, path);
#else
	return makeGrayscaleTxt(width, height, image, name);
#endif
}

void Camera::RenderWorker(const IHittable &world) {
	std::stringstream ss;
	ss << std::this_thread::get_id();
	auto m = KawaiiMQ::MessageQueueManager::Instance();
	auto result_topic = KawaiiMQ::Topic("renderResult");
	auto result_producer = KawaiiMQ::Producer("producer");
	result_producer.subscribe(result_topic);
	auto task_topic = KawaiiMQ::Topic("renderTask");
	auto task_fetcher = KawaiiMQ::Consumer({task_topic});
	auto task_queue = m->getAllRelatedQueue(task_topic)[0];
	auto chunk_message = std::shared_ptr<KawaiiMQ::MessageData>();
	spdlog::info("thread {} started", ss.str());
	while (task_queue->tryWait(chunk_message)) {
		auto chunk = KawaiiMQ::getMessage<ImageChunk>(chunk_message);
		spdlog::info("chunk {} (start from ({}, {}), dimension {} * {}) "
					 "started by thread {}",
					 chunk.chunk_idx, chunk.startx, chunk.starty, chunk.width, chunk.height, ss.str());

		for (int i = chunk.starty; i < chunk.starty + chunk.height; i++) {
			auto hori = std::vector<Color>();
			hori.reserve(chunk.width);
			for (int j = chunk.startx; j < chunk.startx + chunk.width; j++) {
				Color pixel_color = Color{0, 0, 0};
				for (int k = 0; k < sample_count; ++k) {
					auto ray = getRay(j, i);
					pixel_color += rayColor(ray, world, render_depth);
				}
				pixel_color /= sample_count;
				pixel_color =
						Color{gammaCorrect(pixel_color[0]), gammaCorrect(pixel_color[1]), gammaCorrect(pixel_color[2])};
				hori.emplace_back(pixel_color);
			}
			chunk.partial.emplace_back(hori);
		}
		auto message = KawaiiMQ::makeMessage(chunk);
		result_producer.broadcastMessage(message);
	}
	task_fetcher.unsubscribe(task_topic);
	result_producer.unsubscribe(result_topic);

	m->unrelate(task_topic, task_queue);
}

int Camera::partition() const {
	auto manager = KawaiiMQ::MessageQueueManager::Instance();
	auto queue = KawaiiMQ::makeQueue("renderTaskQueue");
	auto topic = KawaiiMQ::Topic("renderTask");
	manager->relate(topic, queue);
	KawaiiMQ::Producer prod("chunkPusher");
	prod.subscribe(topic);
	int upperleft_x = 0;
	int upperleft_y = 0;
	int idx = 0;
	while (upperleft_y < height) {
		while (upperleft_x < width) {
			ImageChunk chunk;
			chunk.chunk_idx = idx;
			++idx;
			chunk.startx = upperleft_x;
			chunk.starty = upperleft_y;
			if (upperleft_x + chunk_dimension > width) {
				chunk.width = width % chunk_dimension;
			} else {
				chunk.width = chunk_dimension;
			}
			if (upperleft_y + chunk_dimension > height) {
				chunk.height = height % chunk_dimension;
			} else {
				chunk.height = chunk_dimension;
			}
			auto message = KawaiiMQ::makeMessage(chunk);
			prod.broadcastMessage(message);
			upperleft_x += chunk_dimension;
		}
		upperleft_x = 0;
		upperleft_y += chunk_dimension;
	}
	prod.unsubscribe(topic);
	return idx;
}

void Camera::setRotation(const Eigen::Vector3d &rot) {
	rotation_ypr = rot;
	updateVectors();
}

Eigen::Vector3d Camera::getRotation() const { return rotation_ypr; }

Color Camera::getBackground() const { return background; }

void Camera::setBackground(const Color &background) { this->background = background; }

Camera::Camera(int width, float aspect_ratio, float fov, Point3 position, Eigen::Vector3d target, float dof_angle) :
	width(width), aspect_ratio(aspect_ratio), fov(fov), target(std::move(target)), position(std::move(position)),
	height(static_cast<int>(width / aspect_ratio)), dof_angle(dof_angle) {
	render_thread_count = std::thread::hardware_concurrency() == 0 ? 12 : std::thread::hardware_concurrency();
	auto q = Eigen::Quaterniond::FromTwoVectors(Eigen::Vector3d{0, 0, -1}, (target - position));
	rotation_matrix = q.toRotationMatrix();
	auto angle = rotation_matrix.canonicalEulerAngles(2, 1, 0);
	rotation_ypr = Eigen::Vector3d{angle(0, 0), angle(1, 0), angle(2, 0)};
	updateVectors();
}

void Camera::updateVectors() {
	auto theta = deg2Rad(fov);
	auto h = tan(theta / 2);
	focal_len = (position - target).norm();
	viewport_height = 2 * h * focal_len;
	viewport_width = viewport_height * (static_cast<float>(width) / height);
	w = rotation_matrix * Eigen::Vector3d::UnitZ();
	auto this_UP = UP;
	// flip vertically
	if (rotation_ypr[0] > PI / 2 && rotation_ypr[0] < 3 * PI / 2)
		this_UP = -UP;
	u = this_UP.cross(w).normalized();
	v = w.cross(u);
	hori_vec = viewport_width * u;
	vert_vec = viewport_height * -v;
	pix_delta_x = hori_vec / width;
	pix_delta_y = vert_vec / height;
	viewport_ul = position - focal_len * w - (vert_vec + hori_vec) / 2;
	pixel_00 = viewport_ul + (pix_delta_y + pix_delta_x) * 0.5;
	auto dof_radius = focal_len * tan(deg2Rad(dof_angle / 2));
	dof_disk_h = u * dof_radius;
	dof_disk_v = v * dof_radius;
}

int Camera::getWidth() const { return width; }

int Camera::getHeight() const { return height; }

float Camera::getAspectRatio() const { return aspect_ratio; }

float Camera::getViewportWidth() const { return viewport_width; }

float Camera::getViewportHeight() const { return viewport_height; }

float Camera::getFocalLen() const { return focal_len; }

const Point3 &Camera::getPosition() const { return position; }

const Eigen::Vector3d &Camera::getHoriVec() const { return hori_vec; }

const Eigen::Vector3d &Camera::getVertVec() const { return vert_vec; }

const Eigen::Vector3d &Camera::getPixDeltaX() const { return pix_delta_x; }

const Eigen::Vector3d &Camera::getPixDeltaY() const { return pix_delta_y; }

const Point3 &Camera::getViewportUl() const { return viewport_ul; }

const Point3 &Camera::getPixel00() const { return pixel_00; }

void Camera::setWidth(int width) {
	Camera::width = width;
	height = static_cast<int>(width / aspect_ratio);
	if (render_thread_count > width || render_thread_count > height) {
		render_thread_count = std::thread::hardware_concurrency() == 0 ? 12 : std::thread::hardware_concurrency();
	}
	updateVectors();
}

void Camera::setAspectRatio(float aspect_ratio) {
	Camera::aspect_ratio = aspect_ratio;
	height = static_cast<int>(width / aspect_ratio);
	updateVectors();
}

void Camera::setPosition(const Point3 &position) {
	Camera::position = position;
	updateVectors();
}

void Camera::setSampleCount(int sample_count) { Camera::sample_count = sample_count; }

Eigen::Vector3d Camera::dofDiskSample() const {
	auto p = randomVec3InUnitDisk();
	return position + (p[0] * dof_disk_h) + (p[1] * dof_disk_v);
}

Color Camera::rayColor(const Ray &ray, const IHittable &object, int depth) {
	HitRecord record;
	if (depth <= 0)
		return Color{0, 0, 0};

	if (object.hit(ray, Interval(EPS, INF), record)) {
		Ray scattered;
		Color attenuation;
		Color emission = record.material->emitted(record.u, record.v, record.p);
		if (record.material->scatter(ray, record, attenuation, scattered)) {
			return attenuation.cwiseProduct(rayColor(scattered, object, depth - 1)) + emission;
		}
		return emission;
	}
	return background;
}

int Camera::getSampleCount() const { return sample_count; }
Eigen::Vector3d Camera::randomDisplacement() const {
	auto delta_x = pix_delta_x * (randomFloat() - 0.5);
	auto delta_y = pix_delta_y * (randomFloat() - 0.5);
	return delta_x + delta_y;
}

int Camera::getRenderDepth() const { return render_depth; }
void Camera::setRenderDepth(int renderDepth) { render_depth = renderDepth; }
int Camera::getRenderThreadCount() const { return render_thread_count; }
void Camera::setRenderThreadCount(int renderThreadCount) {
	if (renderThreadCount == 0 || renderThreadCount > width || renderThreadCount > height) {
		render_thread_count = std::thread::hardware_concurrency() == 0 ? 12 : std::thread::hardware_concurrency();
	} else {
		render_thread_count = renderThreadCount;
	}
}
void Camera::setFov(float fov) { this->fov = fov; }
float Camera::getFov() const { return fov; }
const Point3 &Camera::getTarget() const { return target; }

void Camera::setTarget(const Point3 &target) {
	Camera::target = target;
	auto dir_vec = position - target;
	rotation_matrix =
			Eigen::Quaternion<double>::FromTwoVectors(Eigen::Vector3d::UnitZ(), position - target).toRotationMatrix();
	auto angle = rotation_matrix.canonicalEulerAngles(2, 1, 0);
	rotation_ypr = Eigen::Vector3d{angle(0, 0), angle(1, 0), angle(2, 0)};
	updateVectors();
}
float Camera::getDofAngle() const { return dof_angle; }
void Camera::setDofAngle(float dofAngle) {
	dof_angle = dofAngle;
	updateVectors();
}
void Camera::setFocalLen(float focalLen) {
	focal_len = focalLen;
	updateVectors();
}

Ray Camera::getRay(int x, int y) {
	auto pixel_vec = pixel_00 + pix_delta_x * x + pix_delta_y * y + randomDisplacement();
	auto origin = dof_angle <= 0 ? position : dofDiskSample();
	auto direction = pixel_vec - origin;
	auto time = randomFloat(0, shutter_speed);
	return Ray(origin, direction, time);
}
int Camera::getChunkDimension() const { return chunk_dimension; }

void Camera::setChunkDimension(int dimension) {
	chunk_dimension = (dimension > width || dimension > height) ? width / render_thread_count : dimension;
}

float Camera::getShutterSpeed() const { return shutter_speed; }

void Camera::setShutterSpeed(float shutterSpeed) { shutter_speed = shutterSpeed; }
