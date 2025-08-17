/**
 * @file GpuRenderer.h
 * @author Kagurazaka Ayano
 * @date 16/08/25
 * @brief A gpu renderer
 */

#ifndef RAYTRACING_GPURENDERER_H
#define RAYTRACING_GPURENDERER_H

#include <vector>
#include "Camera.h"
#include "GraphicObjects.h"
#include "IRenderer.h"
class GpuRenderer : public IRenderer {
public:
	RendererOutput render(const IHittable &world);
	IRenderer updateConf(const CommonOptions &options, const RendererOptions &renderer_options);
	GpuRenderer(const CommonOptions &options, const RendererOptions &renderer_options);
	virtual ~GpuRenderer() override;
};

#endif // RAYTRACING_GPURENDERER_H
