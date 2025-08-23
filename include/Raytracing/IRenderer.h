/**
 * @file IRenderer.h
 * @author Kagurazaka Ayano
 * @date 16/08/25
 * @brief Renderer interface
 */

#ifndef RAYTRACING_IRENDERER_H
#define RAYTRACING_IRENDERER_H

#include <memory>
#include <string_view>
#include <unordered_map>
#include <variant>
#include <vector>
#include "Eigen/Core"
#include "GraphicObjects.h"

class IRenderer;

using OptionValue = std::variant<int32_t, float, bool, std::string>;
using RendererOptions = std::unordered_map<std::string, OptionValue>;
using RendererPtr = std::shared_ptr<IRenderer>;

struct RendererOutput {
	uint32_t width, height;
	std::vector<float> data;
};
struct CameraParams {
	int width = 0;
	float aspect_ratio;
	Eigen::Vector3d pos, target, u, v, w, pix_00, pix_dx, pix_dy, dof_disk_h, dof_disk_v;

	float shutter_speed = 1;
	Eigen::Vector3d background;

	float fov = 45;
	float focal_len = 1;
};

class IRenderer {
public:
	virtual RendererOutput render(const IHittable &world, const CameraParams &camera_params) = 0;
	virtual RendererPtr updateConf(const RendererOptions &renderer_options) = 0;
	virtual ~IRenderer() = default;
};


#endif // RAYTRACING_IRENDERER_H
