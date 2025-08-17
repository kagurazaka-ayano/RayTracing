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
	struct GpuRendererConf {
		std::size_t workgroup_size_x, workgroup_size_y, sample_per_pixel, max_depth;
	};
	Image render();
};

#endif // RAYTRACING_GPURENDERER_H
