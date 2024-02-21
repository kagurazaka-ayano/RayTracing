/**
 * @file Camera.h
 * @author ayano
 * @date 2/7/24
 * @brief
 */

#ifndef ONEWEEKEND_CAMERA_H
#define ONEWEEKEND_CAMERA_H

#include "GraphicObjects.h"
#include "ImageUtil.h"
#include "KawaiiMQ/Topic.h"
#include "MathUtil.h"
#include "AppleMath/Vector.hpp"
#include "spdlog/fmt/bundled/core.h"
#include <future>
#include <thread>

class Camera {
public:
	Camera(int width, double aspect_ratio, double fov, Point3 position, AppleMath::Vector3 initial_facing, AppleMath::Vector3 initial_rotation, double dof_angle);

	int getWidth() const;

	int getHeight() const;

	double getAspectRatio() const;

	double getViewportWidth() const;

	double getViewportHeight() const;

	double getFocalLen() const;

	int getSampleCount() const;

	double getFov() const;

	const Point3 &getPosition() const;

	const AppleMath::Vector3 &getHoriVec() const;

	const AppleMath::Vector3 &getVertVec() const;

	const AppleMath::Vector3 &getPixDeltaX() const;

	const AppleMath::Vector3 &getPixDeltaY() const;

	const Point3 &getViewportUl() const;

	const Point3 &getPixel00() const;

	void setWidth(int width);

	void setAspectRatio(double aspect_ratio);

	void setPosition(const Point3 &position);

	void setSampleCount(int sample_count);

	void setFov(double fov);

	Ray getRay(int x, int y);

	std::string Render(const IHittable& world, const std::string& name, const std::string& path = IMG_OUTPUT_DIR);

	int getRenderDepth() const;

	void setRenderDepth(int renderDepth);

	int getRenderThreadCount() const;

	void setRenderThreadCount(int renderThreadCount);

	const Point3 &getTarget() const;

	void setTarget(const Point3 &target);

	double getDofAngle() const;

	void setDofAngle(double dofAngle);

	void setFocalLen(double focalLen);

	int getChunkDimension() const;

	void setChunkDimension(int dimension);

	double getShutterSpeed() const;

	void setShutterSpeed(double shutterSpeed);

	void setRotation(const AppleMath::Vector3& rot);

	AppleMath::Vector3 getRotation() const;
	
private:

	Color rayColor(const Ray &ray, const IHittable &object, int depth);

	void updateVectors();

	AppleMath::Vector3 randomDisplacement() const;

	Point3 dofDiskSample() const;

	void RenderWorker(const IHittable &world);

	int partition() const;

	int width;
	int height;
	double aspect_ratio;
	double viewport_width;
	double viewport_height;
	double focal_len;
	double fov = 45;
	int sample_count = 20;
	int render_depth = 50;
	int render_thread_count = std::thread::hardware_concurrency() == 0 ? 12 : std::thread::hardware_concurrency();
	int chunk_dimension = width / render_thread_count < 0 ? width : width / render_thread_count;
	double dof_angle = 0;
	double shutter_speed = 1;
	AppleMath::Vector3 u, v, w;
	Point3 position;
	AppleMath::Vector3 rotation_rad = {0, 0, 0};
	Point3 facing;
	AppleMath::Vector3 UP = AppleMath::Vector3{0, 1, 0};
	AppleMath::Vector3 hori_vec;
	AppleMath::Vector3 vert_vec;
	AppleMath::Vector3 pix_delta_x;
	AppleMath::Vector3 pix_delta_y;
	Point3 viewport_ul;
	Point3 pixel_00;
	AppleMath::Vector3 dof_disk_h;
	AppleMath::Vector3 dof_disk_v;
};

#endif // ONEWEEKEND_CAMERA_H
