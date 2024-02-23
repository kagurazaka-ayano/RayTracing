/**
 * @file scenes.cpp
 * @author ayano
 * @date 2/15/24
 * @brief
 */

#include "scenes.h"
#include <memory>
#include <string>
#include "AppleMath/Vector.hpp"
#include "Camera.h"
#include "GraphicObjects.h"
#include "Material.h"
#include "MathUtil.h"
#include "Texture.h"

void render(HittableList world, Camera camera, const std::string &name = "test.ppm",
			const std::string path = IMG_OUTPUT_DIR) {
#ifndef ASCII_ART
	auto file = camera.Render(world, name, path);
	// system(std::string("open " + file).c_str());
#else
	auto file = camera.Render(world, "test.txt");
	// system(std::string("open " + file).c_str());
#endif
}

void randomSpheres() {

<<<<<<< Updated upstream
	auto camera = Camera(400, 16.0 / 9.0, 30, {-13, 2, 3}, {0, 0, 0}, 0.6);
=======
	auto camera = Camera(200, 16.0 / 9.0, 30, {-13, 2, 3}, {0, 0, 0}, {0, 0, 0}, 0.6);
>>>>>>> Stashed changes

	camera.setSampleCount(100);
	camera.setShutterSpeed(1.0 / 24.0);
	camera.setRenderDepth(50);
	camera.setRenderThreadCount(12);
	camera.setChunkDimension(64);
	auto world = HittableList();
	auto checker = std::make_shared<CheckerTexture>(0.5, Color{0.1, 0.1, 0.1}, Color{0.9, 0.9, 0.9});
	auto ground_material = std::make_shared<Lambertian>(Lambertian(checker));
	auto left_ball_material = std::make_shared<Lambertian>(Color{0.357, 0.816, 0.98});
	auto center_ball_material = std::make_shared<Metal>(Metal(Color{0.965, 0.671, 0.729}, 0.4));
	auto right_ball_material = std::make_shared<Dielectric>(Dielectric(1.5, Color{0.8, 0.8, 0.8}));
	world.add(std::make_shared<Sphere>(Sphere(1000, AppleMath::Vector3{0, -1000, -1.0}, ground_material)));
	world.add(std::make_shared<Sphere>(Sphere(1, AppleMath::Vector3{0, 1, 0}, center_ball_material)));
	world.add(std::make_shared<Sphere>(Sphere(1, AppleMath::Vector3{4, 1, 0}, right_ball_material)));
	world.add(std::make_shared<Sphere>(Sphere(1, AppleMath::Vector3{-4, 1, 0}, left_ball_material)));
	int obj = 0;
	for (int i = -11; i < 11; i += 2) {
		for (int j = -11; j < 11; j += 2) {
			obj++;
			auto coord = AppleMath::Vector3{(i + randomDouble(-1, 1)), 0.2, (j + randomDouble(-1, 1))};
			auto displacement = AppleMath::Vector3{0, randomDouble(0, 0), 0};
			auto material = static_cast<int>(3.0 * randomDouble());
			if ((coord - AppleMath::Vector3{0, 1, 0}).length() > 0.9) {
				AppleMath::Vector3 color = randomVec3().componentProd(randomVec3());
				std::shared_ptr<IMaterial> sphere_mat;
				switch (material) {
					case 0:
						sphere_mat = std::make_shared<Lambertian>(color);
						world.add(std::make_shared<Sphere>(0.2, coord, coord + displacement, sphere_mat));
						break;
					case 1:
						sphere_mat = std::make_shared<Metal>(color, randomDouble(0.2, 0.5));
						world.add(std::make_shared<Sphere>(0.2, coord, coord + displacement, sphere_mat));
						break;
					case 2:
						color = randomVec3(0.7, 1);
						sphere_mat = std::make_shared<Dielectric>(randomDouble(1, 2), color);
						world.add(std::make_shared<Sphere>(0.2, coord, coord + displacement, sphere_mat));
						break;
					default:
						break;
				}
			}
		}
	}
	world = HittableList(std::make_shared<BVHNode>(world));
	render(world, camera, "randomSpheres.ppm");
}

void twoSpheres() {
<<<<<<< Updated upstream
	auto camera = Camera(400, 16.0 / 9.0, 30, {0, 0, 0}, {0, 0, -30}, 0.6);
=======
	auto camera = Camera(400, 16.0 / 9.0, 30,{0, 0, -30}, {0, 0, 0}, {PI / 4, 0, PI / 8}, 0.6);
>>>>>>> Stashed changes
	camera.setSampleCount(100);
	camera.setShutterSpeed(1.0 / 24.0);
	camera.setRenderDepth(50);
	camera.setRenderThreadCount(12);
	camera.setChunkDimension(64);

	auto world = HittableList();
	auto checker = std::make_shared<CheckerTexture>(2, Color{0.1, 0.1, 0.1}, Color{0.9, 0.9, 0.9});
	auto sphere_material = std::make_shared<Lambertian>(Lambertian(checker));
	world.add(std::make_shared<Sphere>(20.0, Point3{0, -20.0, -30}, sphere_material));
	world.add(std::make_shared<Sphere>(20.0, Point3{0, 20.0, -30}, sphere_material));
	world = HittableList(std::make_shared<BVHNode>(world));
	render(world, camera, "twoSpheres.ppm");
}

void huajiSphere() {
	auto camera = Camera(400, 16.0 / 9.0, 45, {30, 0, -30}, {0, 0, -30}, 0.1);
	camera.setSampleCount(100);
	camera.setShutterSpeed(1.0 / 24.0);
	camera.setRenderDepth(50);
	camera.setRenderThreadCount(12);
	camera.setChunkDimension(64);
	auto world = HittableList();
	auto huaji_texture = std::make_shared<ImageTexture>("huaji.jpeg");
	auto huaji_material = std::make_shared<Lambertian>(huaji_texture);
	world.add(std::make_shared<Sphere>(10.0, Point3{0, 0, -30}, huaji_material));
	world = HittableList(std::make_shared<BVHNode>(world));
	render(world, camera, "huajiSphere.ppm");
}

<<<<<<< Updated upstream
void perlinSpheres() {
	HittableList world;
	Camera camera(400, 16.0 / 9.0, 20, Point3{-13, 2, 3}, Point3{0, 0, 0}, 0);
=======
void perlinSpheres()
{
	HittableList world; 
	Camera camera(400, 16.0 / 9.0, 20, Point3{13, 2, 3}, Point3{0, 0, 0}, {0, 0, 0}, 0);
>>>>>>> Stashed changes
	camera.setSampleCount(100);
	camera.setShutterSpeed(1.0 / 24.0);
	camera.setRenderDepth(50);
	camera.setRenderThreadCount(12);
	camera.setChunkDimension(64);
	auto tex = std::make_shared<NoiseTexture>(1, 10, 0.5);
	world.add(std::make_shared<Sphere>(1000, Point3{0, -1000, 0}, std::make_shared<Lambertian>(tex)));
	world.add(std::make_shared<Sphere>(2, Point3{0, 2, 0}, std::make_shared<Lambertian>(tex)));

	render(world, camera, "perlinSpheres.ppm");
}

<<<<<<< Updated upstream
void terrain() {
	HittableList world;
	Camera camera(400, 16.0 / 9.0, 20, Point3{0, 0, -50}, Point3{0, 0, 0}, 0);
=======
void terrain()
{
	HittableList world; 
	Camera camera(400, 16.0 / 9.0, 30, {0, 0, 50}, {0, 0, 1}, {0, 0, 0}, 0);
>>>>>>> Stashed changes
	camera.setSampleCount(10);
	camera.setShutterSpeed(1.0 / 24.0);
	camera.setRenderDepth(4);
	camera.setRenderThreadCount(12);
	camera.setChunkDimension(64);
	auto tex = std::make_shared<TerrainTexture>(0.5, 10, 0.5);
	world.add(std::make_shared<Sphere>(10, Point3{0, 0, 0}, std::make_shared<Lambertian>(tex)));

	render(world, camera, "terrain.ppm");
}

void rotationTest() {
	HittableList world;
	Camera camera(400, 16.0 / 9.0, 30, {0, 0, 0}, {0, 0, 1}, 0);
	camera.setSampleCount(10);
	camera.setShutterSpeed(1.0 / 24.0);
	camera.setRenderDepth(4);
	camera.setRenderThreadCount(12);
	camera.setChunkDimension(64);
	auto tex = std::make_shared<ImageTexture>("huaji.jpeg");
	auto mat_posz = std::make_shared<Lambertian>(tex);
	auto mat_negz = std::make_shared<Lambertian>(Color{0, 0, 0});
	auto mat_posx = std::make_shared<Metal>(Color{1, 0, 0}, 0);
	auto mat_negx = std::make_shared<Lambertian>(Color{1, 0, 0});
	auto mat_posy = std::make_shared<Metal>(Color{0, 1, 0}, 0);
	auto mat_negy = std::make_shared<Lambertian>(Color{0, 1, 0});
	world.add(std::make_shared<Sphere>(1, Point3{0, 0, 5}, mat_posz));
	world.add(std::make_shared<Sphere>(1, Point3{0, 0, -5}, mat_negz));
	world.add(std::make_shared<Sphere>(1, Point3{0, 5, 0}, mat_posy));
	world.add(std::make_shared<Sphere>(1, Point3{0, -5, 0}, mat_negy));
	world.add(std::make_shared<Sphere>(1, Point3{5, 0, 0}, mat_posz));
	world.add(std::make_shared<Sphere>(1, Point3{-5, 0, 0}, mat_negz));

	world = HittableList(std::make_shared<BVHNode>(world));
	auto rot_init = AppleMath::Vector3{0, 2 * PI / 36, 0};
	for (int i = 0; i < 36; ++i) {
		camera.setRotation(rot_init * i);
		render(world, camera, "frame_" + std::to_string(i) + "_" + std::string(camera.getRotation()) + ".ppm",
			   std::string(IMG_OUTPUT_DIR) + "/theta");
	}
	camera.setRotation({0, 0, 0});
	rot_init = AppleMath::Vector3{2 * PI / 36, 0, 0};
	for (int i = 0; i < 36; ++i) {
		camera.setRotation(rot_init * i);
		render(world, camera, "frame_" + std::to_string(i) + "_" + std::string(camera.getRotation()) + ".ppm",
			   std::string(IMG_OUTPUT_DIR) + "/phi");
	}
	camera.setRotation({0, 0, 0});
	rot_init = AppleMath::Vector3{0, 0, 2 * PI / 36};
	for (int i = 0; i < 36; ++i) {
		camera.setRotation(rot_init * i);
<<<<<<< Updated upstream
		render(world, camera, "frame_" + std::to_string(i) + "_" + std::string(camera.getRotation()) + ".ppm",
			   std::string(IMG_OUTPUT_DIR) + "/psi");
=======
		render(world, camera, "frame_" + std::to_string(i) + "_" + std::string(camera.getRotation()) + ".ppm", std::string(IMG_OUTPUT_DIR) + "/psi");
>>>>>>> Stashed changes
	}
}
