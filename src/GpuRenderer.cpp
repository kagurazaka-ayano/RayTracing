/**
 * @file GpuRenderer.cpp
 * @author ayano
 * @date 16/08/25
 * @brief
 */

#include "GpuRenderer.h"
#include <algorithm>
#include <memory>
#include <tuple>
#include <vector>
#include "GlobUtil.hpp"
#include "GpuGraphicalObjects.h"
#include "IRenderer.h"
#include "gpu.hpp"

RendererOutput GpuRenderer::render(const IHittable &world, const CameraParams &camera_params) {
	gpu::Context ctx;
	std::vector<SphereGpu> spheres;
	std::vector<> spheres;
	std::vector<MaterialGpu> materials;
	const int w = camera_params.width, h = camera_params.width * camera_params.aspect_ratio;
	gpu::Tensor output = gpu::createTensor(ctx, gpu::Shape{static_cast<std::size_t>(w * h * 4)}, gpu::kf32);
	gpu::Tensor sphere_buf = gpu::createTensor(ctx, gpu::Shape)
}
void GpuRenderer::updateCameraParams(const CameraParams &cp) {
	std::tie(p.px, p.py, p.pz) = unpackVectorToTuple(cp.pos);
	std::tie(p.ux, p.uy, p.uz) = unpackVectorToTuple(cp.u);
	std::tie(p.vx, p.vy, p.vz) = unpackVectorToTuple(cp.v);
	std::tie(p.wx, p.wy, p.wz) = unpackVectorToTuple(cp.w);
	std::tie(p.pixel00x, p.pixel00y, p.pixel00z) = unpackVectorToTuple(cp.pix_00);
	std::tie(p.pixel_dx_x, p.pixel_dx_y, p.pixel_dx_z) = unpackVectorToTuple(cp.pix_dx);
	std::tie(p.pixel_dy_x, p.pixel_dy_y, p.pixel_dy_z) = unpackVectorToTuple(cp.pix_dy);
	p.width = cp.width;
	p.height = cp.width * cp.aspect_ratio;
}

RendererPtr GpuRenderer::updateConf(const RendererOptions &renderer_options) {
	auto result = renderer_options;
	result.insert(opt.begin(), opt.end());
	opt = result;
	return std::make_shared<GpuRenderer>(this);
}
