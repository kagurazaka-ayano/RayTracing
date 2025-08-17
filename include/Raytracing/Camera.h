/**
 * @file Camera.h
 * @author ayano
 * @date 2/7/24
 * @brief
 */

#ifndef ONEWEEKEND_CAMERA_H
#define ONEWEEKEND_CAMERA_H

#include <thread>
#include "Eigen/Dense"
#include "GraphicObjects.h"
#include "ImageUtil.h"
#include "MathUtil.h"

class Camera {
public:
	Camera(int width, float aspect_ratio, float fov, Point3 position, Eigen::Vector3d target, float dof_angle);

	int getWidth() const;

	int getHeight() const;

	float getAspectRatio() const;

	float getViewportWidth() const;

	float getViewportHeight() const;

	float getFocalLen() const;

	int getSampleCount() const;

	float getFov() const;

	const Point3 &getPosition() const;

	const Eigen::Vector3d &getHoriVec() const;

	const Eigen::Vector3d &getVertVec() const;

	const Eigen::Vector3d &getPixDeltaX() const;

	const Eigen::Vector3d &getPixDeltaY() const;

	const Point3 &getViewportUl() const;

	const Point3 &getPixel00() const;

	void setWidth(int width);

	void setAspectRatio(float aspect_ratio);

	void setPosition(const Point3 &position);

	void setSampleCount(int sample_count);

	void setFov(float fov);

	Ray getRay(int x, int y);

	std::string Render(const IHittable &world, const std::string &name, const std::string &path);

	int getRenderDepth() const;

	void setRenderDepth(int renderDepth);

	int getRenderThreadCount() const;

	void setRenderThreadCount(int renderThreadCount);

	const Point3 &getTarget() const;

	void setTarget(const Point3 &target);

	float getDofAngle() const;

	void setDofAngle(float dofAngle);

	void setFocalLen(float focalLen);

	int getChunkDimension() const;

	void setChunkDimension(int dimension);

	float getShutterSpeed() const;

	void setShutterSpeed(float shutterSpeed);

	void setRotation(const Eigen::Vector3d &rot);

	Eigen::Vector3d getRotation() const;

	Color getBackground() const;

	void setBackground(const Color &background);

private:
	Color rayColor(const Ray &ray, const IHittable &object, int depth);

	void updateVectors();

	Eigen::Vector3d randomDisplacement() const;

	Point3 dofDiskSample() const;

	void RenderWorker(const IHittable &world);

	int partition() const;

	int width;
	int height;
	float aspect_ratio;
	float viewport_width;
	float viewport_height;
	float focal_len;
	float fov = 45;
	int sample_count = 20;
	int render_depth = 50;
	int render_thread_count = std::thread::hardware_concurrency() == 0 ? 12 : std::thread::hardware_concurrency();
	int chunk_dimension = width / render_thread_count < 0 ? width : width / render_thread_count;
	float dof_angle = 0;
	float shutter_speed = 1;
	Eigen::Vector3d u, v, w;
	Point3 position;
	Eigen::Vector3d rotation_ypr = {0, 0, 0};
	Eigen::Matrix3d rotation_matrix = Eigen::Matrix3d::Identity();
	Point3 target;
	Eigen::Vector3d UP = Eigen::Vector3d::UnitY();
	Eigen::Vector3d hori_vec;
	Eigen::Vector3d vert_vec;
	Eigen::Vector3d pix_delta_x;
	Eigen::Vector3d pix_delta_y;
	Point3 viewport_ul;
	Point3 pixel_00;
	Eigen::Vector3d dof_disk_h;
	Eigen::Vector3d dof_disk_v;
	Color background;
};

#endif // ONEWEEKEND_CAMERA_H
