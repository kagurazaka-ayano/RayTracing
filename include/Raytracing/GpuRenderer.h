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

struct Params {
	// camera metadata
	float px, py, pz, pad0;
	float ux, uy, uz, pad1;
	float vx, vy, vz, pad2;
	float wx, wy, wz, pad3;
	float pixel00x, pixel00y, pixel00z, pad4;
	float pixel_dx_x, pixel_dx_y, pixel_dx_z, pad5;
	float pixel_dy_x, pixel_dy_y, pixel_dy_z, pad6;

	// render metadata
	uint32_t width, height;
	uint32_t sample_per_pixel;
	uint32_t num_sphere;
	uint32_t num_triangle;
	uint32_t num_quad;
	uint32_t num_material;
	uint32_t seed_base;
	float gamma;
};

class GpuRenderer : public IRenderer {
public:
	RendererOutput render(const IHittable &world, const CameraParams &camera_params) override;
	RendererPtr updateConf(const RendererOptions &renderer_options) override;
	GpuRenderer(const RendererOptions &renderer_options);
	virtual ~GpuRenderer() override = default;
	RendererOptions opt;
	Params p;

private:
	void updateCameraParams(const CameraParams &cp);
	void flattenWorld(const IHittable &world);
};


#endif // RAYTRACING_GPURENDERER_H
