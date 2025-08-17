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
#include "ImageUtil.h"

using OptionValue = std::variant<int32_t, float, bool, std::string>;
using RendererOptions = std::unordered_map<std::string, OptionValue>;

struct CommonOptions {
	int32_t sample_count, render_depth;
};
struct RendererOutput {
	uint32_t width, height;
	std::vector<float> data;
};
struct CameraParams {
	int width = 0;
	int height = 0;
	Eigen::Vector3d pos, pix_00, pix_dx, pix_dy, dof_disk_h, dof_disk_v;

	float shutter_speed = 1;
	Color background;

	float fov = 45;
	float focal_len = 1;
};

class IRenderer {
	virtual RendererOutput render(const IHittable &world) = 0;
	virtual std::string render(const IHittable &world, const CameraParams &camera_params, std::string_view name,
							   std::string_view path) = 0;
	virtual IRenderer updateConf(const CommonOptions &options, const RendererOptions &renderer_options) = 0;
	virtual ~IRenderer() = default;
};

using RendererPtr = std::shared_ptr<IRenderer>();


#endif // RAYTRACING_IRENDERER_H
