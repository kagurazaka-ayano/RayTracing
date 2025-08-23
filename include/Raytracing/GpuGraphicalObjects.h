/**
 * @file GpuGraphicalObjects.h
 * @author Kagurazaka Ayano
 * @date 17/08/25
 * @brief
 */

#ifndef RAYTRACING_GPUGRAPHICALOBJECTS_H
#define RAYTRACING_GPUGRAPHICALOBJECTS_H


#include <cstdint>
struct SphereGpu {
	float center[3];
	float bbox_begin[3];
	float bbox_end[3];
	uint32_t material_idx;
};

struct QuadGpu {
	float q[3], u[3], v[3];
	// plane equation: Ax + By + Cz = D, and D = n (plane normal) dot v
	float wx, wy, wz, pad4;
	float bbox_x_begin, bbox_y_begin, bbox_z_begin, pad5;
	float bbox_x_end, bbox_y_end, bbox_z_end, pad6;
	uint32_t material_idx, padli2[3];
};

struct TriangleGpu {
	float qx, qy, qz, pad1;
	float ux, uy, uz, pad2;
	float vx, vy, vz, pad3;
	float nx, ny, nz, pad4;
	float wx, wy, wz, pad5

}

struct MaterialGpu {
	uint32_t type;
	float r, g, b;
	float fuzz;
	float idx_of_refraction;
	float r_emm, g_emm, b_emm;
};


#endif // RAYTRACING_GPUGRAPHICALOBJECTS_H
