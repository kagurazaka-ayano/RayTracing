/**
 * @file CpuRenderer.h
 * @author Kagurazaka Ayano
 * @date 16/08/25
 * @brief
 */

#ifndef RAYTRACING_CPURENDERER_H
#define RAYTRACING_CPURENDERER_H
#include "IRenderer.h"
class CpuRenderer : public IRenderer {
public:
	CpuRenderer();

	IRenderer initialize(const CommonOptions &options, const RendererOptions &renderer_options);

	virtual ~CpuRenderer();

	CommonOptions common_opt;
	RendererOptions cpu_opt;
};

#endif // RAYTRACING_CPURENDERER_H
