/**
 * @file IRenderer.h
 * @author Kagurazaka Ayano
 * @date 16/08/25
 * @brief Renderer interface
 */

#ifndef RAYTRACING_IRENDERER_H
#define RAYTRACING_IRENDERER_H

#include <unordered_map>
#include <variant>
#include <vector>
#include "GraphicObjects.h"
#include "ImageUtil.h"

using OptionValue = std::variant<int32_t, float, bool, std::string>;
using RendererOptions = std::unordered_map<std::string, OptionValue>;

struct CommonOptions {
	int32_t sample_count, render_depth, chunk_dimension;
};
struct RendererOutput {
	uint32_t width, height;
	std::vector<float> data;
};

class IRenderer {
	virtual RendererOutput render(const IHittable &world) = 0;
	virtual IRenderer initialize(const CommonOptions &options, const RendererOptions &renderer_options) = 0;
	virtual ~IRenderer() = default;
};


#endif // RAYTRACING_IRENDERER_H
