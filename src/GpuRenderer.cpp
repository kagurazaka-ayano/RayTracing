/**
 * @file GpuRenderer.cpp
 * @author ayano
 * @date 16/08/25
 * @brief
 */

#include "GpuRenderer.h"
#include <vector>
#include "GraphicObjects.h"

GpuRenderer::GpuImage GpuRenderer::render(const IHittable &world, const Camera &cam, const GpuRendererConf &conf) {}

struct SphereGpu {
	float cx, cy, cz, radius;
	uint32_t material_idx;
	// padding is used to better utilizing cache line
	uint32_t pad[3];
};

struct MaterialGpu {
	uint32_t type;
	float r, g, b;
	float fuzz;
	float idx_of_refraction;
	float r_emm, g_emm, b_emm;
};

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
	uint32_t num_material;
	uint32_t seed_base;
	float gamma;
};

static void flattenScene(const IHittable &world, std::vector<SphereGpu> &sphere, std::vector<MaterialGpu> &materials) {}
