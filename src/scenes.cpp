/**
 * @file scenes.cpp
 * @author ayano
 * @date 2/15/24
 * @brief
 */

#include "scenes.h"
#include "Camera.h"
#include "GlobUtil.hpp"
#include "GraphicObjects.h"
#include "ImageUtil.h"
#include "Material.h"
#include "MathUtil.h"
#include "Texture.h"
#include <memory>
#include <string>

void render(HittableList world, Camera camera,
            const std::string &name = "test.ppm",
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

    auto camera = Camera(1920, 16.0 / 9.0, 30, {-13, 2, 3}, {4, 1, 0}, 0.6);

    camera.setSampleCount(100);
    camera.setShutterSpeed(1.0 / 24.0);
    camera.setRenderDepth(50);
    camera.setRenderThreadCount(12);
    camera.setChunkDimension(120);
    camera.setBackground(Color{0.7, 0.8, 1});
    auto world = HittableList();
    auto checker = std::make_shared<CheckerTexture>(0.1, Color{0.05, 0.1, 0.1},
                                                    Color{0.9, 0.9, 0.9});
    auto ground_material = std::make_shared<Lambertian>(Lambertian(checker));
    auto left_ball_material =
        std::make_shared<Lambertian>(Color{0.357, 0.816, 0.98});
    auto center_ball_material =
        std::make_shared<Metal>(Metal(Color{0.965, 0.671, 0.729}, 0.4));
    auto right_ball_material =
        std::make_shared<Dielectric>(Dielectric(1.5, Color{0.8, 0.8, 0.8}));
    world.add(std::make_shared<Quad>(
        Eigen::Vector3d{-500, 0, -500}, Eigen::Vector3d{0, 0, 1000},
        Eigen::Vector3d{1000, 0, 0}, ground_material));
    world.add(std::make_shared<Sphere>(
        Sphere(1, Eigen::Vector3d{0, 1, 0}, center_ball_material)));
    world.add(std::make_shared<Sphere>(
        Sphere(1, Eigen::Vector3d{4, 1, 0}, right_ball_material)));
    world.add(std::make_shared<Sphere>(
        Sphere(1, Eigen::Vector3d{-4, 1, 0}, left_ball_material)));
    int obj = 0;
    for (int i = -22; i < 22; i += 2) {
        for (int j = -22; j < 22; j += 2) {
            obj++;
            auto coord = Eigen::Vector3d{(i + randomFloat(-1, 1)), 0.2,
                                         (j + randomFloat(-1, 1))};
            auto displacement = Eigen::Vector3d{0, randomFloat(0, 0), 0};
            auto material = static_cast<int>(3.0 * randomFloat());
            if ((coord - Eigen::Vector3d{0, 1, 0}).norm() > 0.9) {
                Eigen::Vector3d color = randomVec3().cwiseProduct(randomVec3());
                std::shared_ptr<IMaterial> sphere_mat;
                switch (material) {
                case 0:
                    sphere_mat = std::make_shared<Lambertian>(color);
                    world.add(std::make_shared<Sphere>(
                        0.2, coord, coord + displacement, sphere_mat));
                    break;
                case 1:
                    sphere_mat =
                        std::make_shared<Metal>(color, randomFloat(0.2, 0.5));
                    world.add(std::make_shared<Sphere>(
                        0.2, coord, coord + displacement, sphere_mat));
                    break;
                case 2:
                    color = randomVec3(0.7, 1);
                    sphere_mat =
                        std::make_shared<Dielectric>(randomFloat(1, 2), color);
                    world.add(std::make_shared<Sphere>(
                        0.2, coord, coord + displacement, sphere_mat));
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
    auto camera = Camera(400, 16.0 / 9.0, 30, {0, 0, 0}, {0, 0, -30}, 0.6);
    camera.setSampleCount(100);
    camera.setShutterSpeed(1.0 / 24.0);
    camera.setRenderDepth(50);
    camera.setRenderThreadCount(12);
    camera.setChunkDimension(64);
    camera.setBackground(Color{0.7, 0.8, 1});

    auto world = HittableList();
    auto checker = std::make_shared<CheckerTexture>(2, Color{0.1, 0.1, 0.1},
                                                    Color{0.9, 0.9, 0.9});
    auto sphere_material = std::make_shared<Lambertian>(Lambertian(checker));
    world.add(
        std::make_shared<Sphere>(20.0, Point3{0, -20.0, -30}, sphere_material));
    world.add(
        std::make_shared<Sphere>(20.0, Point3{0, 20.0, -30}, sphere_material));
    world = HittableList(std::make_shared<BVHNode>(world));
    render(world, camera, "twoSpheres.ppm");
}

void huajiSphere() {
    auto camera = Camera(400, 16.0 / 9.0, 45, {30, 0, -30}, {0, 0, 0}, 0.1);
    camera.setSampleCount(100);
    camera.setShutterSpeed(1.0 / 24.0);
    camera.setRenderDepth(50);
    camera.setRenderThreadCount(12);
    camera.setChunkDimension(64);
    camera.setBackground(Color{0.7, 0.8, 1});
    auto world = HittableList();
    auto huaji_texture = std::make_shared<ImageTexture>("huaji.jpeg");
    auto huaji_material = std::make_shared<Lambertian>(huaji_texture);
    world.add(
        std::make_shared<Sphere>(10.0, Point3{0, 0, -30}, huaji_material));
    world = HittableList(std::make_shared<BVHNode>(world));
    render(world, camera, "huajiSphere.ppm");
}

void perlinSpheres() {
    HittableList world;
    Camera camera(1920, 16.0 / 9.0, 20, Point3{-13, 2, 3}, Point3{0, 0, 0}, 0);
    camera.setSampleCount(100);
    camera.setShutterSpeed(1.0 / 24.0);
    camera.setRenderDepth(50);
    camera.setRenderThreadCount(12);
    camera.setChunkDimension(64);
    camera.setBackground(Color{0.7, 0.8, 1});
    auto tex = std::make_shared<NoiseTexture>(1, 10, 0.5);
    world.add(std::make_shared<Sphere>(1000, Point3{0, -1000, 0},
                                       std::make_shared<Lambertian>(tex)));
    world.add(std::make_shared<Sphere>(2, Point3{0, 2, 0},
                                       std::make_shared<Lambertian>(tex)));

    render(world, camera, "perlinSpheres.ppm");
}

void terrain() {
    HittableList world;
    Camera camera(400, 16.0 / 9.0, 20, Point3{0, 0, -50}, Point3{0, 0, 0}, 0);
    camera.setSampleCount(10);
    camera.setShutterSpeed(1.0 / 24.0);
    camera.setRenderDepth(4);
    camera.setRenderThreadCount(12);
    camera.setChunkDimension(64);
    camera.setBackground(Color{0.7, 0.8, 1});
    auto tex = std::make_shared<TerrainTexture>(0.5, 10, 0.5);
    world.add(std::make_shared<Sphere>(10, Point3{0, 0, 0},
                                       std::make_shared<Lambertian>(tex)));

    render(world, camera, "terrain.ppm");
}

void rotationTest() {
    HittableList world;
    Camera camera(400, 16.0 / 9.0, 70, {0, 0, 0}, {0, 0, 1}, 0);
    camera.setSampleCount(10);
    camera.setShutterSpeed(1.0 / 24.0);
    camera.setRenderDepth(4);
    camera.setRenderThreadCount(12);
    camera.setChunkDimension(34);
    camera.setBackground(Color{0.3, 0.4, 0.5});
    auto tex = std::make_shared<ImageTexture>("huaji.jpeg");
    auto mat_posz = std::make_shared<Lambertian>(Color{0, 0, 1});
    auto mat_negz = std::make_shared<Lambertian>(Color{0, 0, 0.2});
    auto mat_posx = std::make_shared<Lambertian>(Color{1, 0, 0});
    auto mat_negx = std::make_shared<Lambertian>(Color{0.2, 0, 0});
    auto mat_posy = std::make_shared<Lambertian>(Color{0, 1, 0});
    auto mat_negy = std::make_shared<Lambertian>(Color{0, 0.2, 0});
    world.add(std::make_shared<Sphere>(1, Point3{0, 0, 5}, mat_posz));
    world.add(std::make_shared<Sphere>(1, Point3{0, 0, -5}, mat_negz));
    world.add(std::make_shared<Sphere>(1, Point3{0, 5, 0}, mat_posy));
    world.add(std::make_shared<Sphere>(1, Point3{0, -5, 0}, mat_negy));
    world.add(std::make_shared<Sphere>(1, Point3{5, 0, 0}, mat_posx));
    world.add(std::make_shared<Sphere>(1, Point3{-5, 0, 0}, mat_negx));

    world = HittableList(std::make_shared<BVHNode>(world));
    auto rot_init = Eigen::Vector3d{0, 2 * PI / 36, 0};
    for (int i = 0; i < 36; ++i) {
        camera.setRotation(rot_init * i);
        render(world, camera,
               "frame_" + std::to_string(i) + "_" +
                   vecToStr(camera.getRotation()) + ".ppm",
               std::string(IMG_OUTPUT_DIR) + "/theta");
    }
    camera.setRotation({0, 0, 0});
    rot_init = Eigen::Vector3d{2 * PI / 36, 0, 0};
    for (int i = 0; i < 36; ++i) {
        camera.setRotation(rot_init * i);
        render(world, camera,
               "frame_" + std::to_string(i) + "_" +
                   vecToStr(camera.getRotation()) + ".ppm",
               std::string(IMG_OUTPUT_DIR) + "/phi");
    }
    camera.setRotation({0, 0, 0});
    rot_init = Eigen::Vector3d{0, 0, 2 * PI / 36};
    for (int i = 0; i < 36; ++i) {
        camera.setRotation(rot_init * i);
        render(world, camera,
               "frame_" + std::to_string(i) + "_" +
                   vecToStr(camera.getRotation()) + ".ppm",
               std::string(IMG_OUTPUT_DIR) + "/psi");
    }
}

void targetingTest() {
    auto camera = Camera(400, 16.0 / 9.0, 20, {0, 0, 0}, {0, 0, -1}, 0);
    camera.setBackground(Color{0.7, 0.8, 1});
    camera.setSampleCount(20);
    auto ball_cnt = 16;
    auto ball_dist = 8;
    auto ball_rad = 1;
    auto world = HittableList();
    std::vector<Eigen::Vector3d> pos1;
    std::vector<Eigen::Vector3d> pos2;
    for (double i = 0; i < 2 * PI; i += 2 * PI / ball_cnt) {
        double x = ball_dist * cos(i);
        double z = ball_dist * sin(i);
        pos1.emplace_back(x, 0, z);
        pos2.emplace_back(0, x, z);
        auto color = Color{i / (2 * PI), 0, 0};
        auto color2 = Color{0, i / (2 * PI), 0};
        auto mat = std::make_shared<Lambertian>(color);
        auto mat2 = std::make_shared<Lambertian>(color2);
        world.add(
            std::make_shared<Sphere>(ball_rad, Eigen::Vector3d{x, 0, z}, mat));
        world.add(
            std::make_shared<Sphere>(ball_rad, Eigen::Vector3d{0, x, z}, mat2));
    }

    Camera overview = camera;
    overview.setPosition({20, 40, 0});
    overview.setTarget({0, 0, 0});
    overview.setFov(150);
    overview.Render(world, "overview.ppm",
                    std::string(IMG_OUTPUT_DIR) + "/horizontalTargetTest");
    auto idx = 0;

    for (const auto i : pos1) {
        camera.setTarget(i);
        std::stringstream ss;
        auto rotation = camera.getRotation();
        ss << idx << "_looking(" << i.x() << "," << i.y() << "," << i.z() << ")"
           << "(yaw=" << rotation[0] << ",pitch=" << rotation[1]
           << ",roll=" << rotation[2] << ").ppm";
        idx++;
        camera.Render(world, ss.str(),
                      std::string(IMG_OUTPUT_DIR) + "/horizontalTargetTest");
    }

    for (const auto i : pos2) {
        camera.setTarget(i);
        std::stringstream ss;
        auto rotation = camera.getRotation();
        ss << idx << "_looking(" << i.x() << "," << i.y() << "," << i.z() << ")"
           << "(yaw=" << rotation[0] << ",pitch=" << rotation[1]
           << ",roll=" << rotation[2] << ").ppm";
        idx++;
        camera.Render(world, ss.str(),
                      std::string(IMG_OUTPUT_DIR) + "/verticalTargetTest");
    }
}

void quads() {
    HittableList world;

    auto blue = std::make_shared<Lambertian>(Color{0.36, 0.81, 0.98});
    auto pink = std::make_shared<Lambertian>(Color{0.96, 0.66, 0.72});
    auto white = std::make_shared<Lambertian>(Color{1, 1, 1});

    world.add(std::make_shared<Quad>(Point3{-3, -2, 5},
                                     Eigen::Vector3d{0, 0, -4},
                                     Eigen::Vector3d{0, 4, 0}, blue));
    world.add(std::make_shared<Quad>(Point3{-2, -2, 0},
                                     Eigen::Vector3d{4, 0, 0},
                                     Eigen::Vector3d{0, 4, 0}, white));
    world.add(std::make_shared<Quad>(Point3{3, -2, 1}, Eigen::Vector3d{0, 0, 4},
                                     Eigen::Vector3d{0, 4, 0}, pink));
    world.add(std::make_shared<Quad>(Point3{-2, 3, 1}, Eigen::Vector3d{4, 0, 0},
                                     Eigen::Vector3d{0, 0, 4}, blue));
    world.add(std::make_shared<Quad>(Point3{-2, -3, 5},
                                     Eigen::Vector3d{4, 0, 0},
                                     Eigen::Vector3d{0, 0, -4}, pink));
    Camera camera(1920, 9.0 / 9.0, 90, {0, 0, 9}, {0, 0, 0}, 0);
    camera.setSampleCount(100);
    camera.setShutterSpeed(1.0 / 24.0);
    camera.setRenderDepth(50);
    camera.setRenderThreadCount(12);
    camera.setChunkDimension(64);
    camera.setBackground(Color{0.7, 0.8, 1});
    render(world, camera, "quads.ppm");
}

void triangles() {
    HittableList world;

    auto blue = std::make_shared<Lambertian>(Color{0.36, 0.81, 0.98});
    auto pink = std::make_shared<Lambertian>(Color{0.96, 0.66, 0.72});
    auto white = std::make_shared<Lambertian>(Color{1, 1, 1});

    Point3 p1{1.5, 1.5, 0};
    Point3 p2{-1.5, -1.5, 0};
    Eigen::Vector3d vertical{3, 0, 0};
    Eigen::Vector3d horizontal{0, 3, 0};

    world.add(std::make_shared<Triangle>(p1, -vertical, -horizontal, blue));
    world.add(std::make_shared<Triangle>(p2, vertical, horizontal, pink));
    Camera camera(1920, 9.0 / 9.0, 90, {0, 0, 5}, {0, 0, 0}, 0);
    camera.setSampleCount(100);
    camera.setShutterSpeed(1.0 / 24.0);
    camera.setRenderDepth(50);
    camera.setRenderThreadCount(12);
    camera.setChunkDimension(64);
    camera.setBackground(Color{0.7, 0.8, 1});
    render(world, camera, "triangle.ppm");
}

void cornellBox() {
    auto red = std::make_shared<Lambertian>(Color{.65, .05, .05});
    auto white = std::make_shared<Lambertian>(Color{.73, .73, .73});
    auto green = std::make_shared<Lambertian>(Color{.12, .45, .15});
    auto light = std::make_shared<DiffuseLight>(Color{15, 15, 15});
    HittableList world;

    world.add(std::make_shared<Quad>(Point3{555, 0, 0},
                                     Eigen::Vector3d{0, 555, 0},
                                     Eigen::Vector3d{0, 0, 555}, green));
    world.add(std::make_shared<Quad>(Point3{0, 0, 0},
                                     Eigen::Vector3d{0, 555, 0},
                                     Eigen::Vector3d{0, 0, 555}, red));
    world.add(std::make_shared<Quad>(Point3{343, 554, 332},
                                     Eigen::Vector3d{-130, 0, 0},
                                     Eigen::Vector3d{0, 0, -105}, light));
    world.add(std::make_shared<Quad>(Point3{0, 0, 0},
                                     Eigen::Vector3d{555, 0, 0},
                                     Eigen::Vector3d{0, 0, 555}, white));
    world.add(std::make_shared<Quad>(Point3{555, 555, 555},
                                     Eigen::Vector3d{-555, 0, 0},
                                     Eigen::Vector3d{0, 0, -555}, white));
    world.add(std::make_shared<Quad>(Point3{0, 0, 555},
                                     Eigen::Vector3d{555, 0, 0},
                                     Eigen::Vector3d{0, 555, 0}, white));

    Camera camera(1920, 16.0 / 9.0, 40, {278, 278, -800}, {278, 278, 0}, 0);
    camera.setSampleCount(10000);
    camera.setShutterSpeed(1.0 / 24.0);
    camera.setRenderDepth(4);
    camera.setRenderThreadCount(12);
    camera.setChunkDimension(64);
    camera.setBackground(Color{0, 0, 0});

    world = HittableList(std::make_shared<BVHNode>(world));

    render(world, camera, "emptyCornell.ppm");
}

void cornellBoxWithObjects() {
    auto red = std::make_shared<Lambertian>(Color{.65, .05, .05});
    auto white = std::make_shared<Lambertian>(Color{.73, .73, .73});
    auto green = std::make_shared<Lambertian>(Color{.12, .45, .15});
    auto light = std::make_shared<DiffuseLight>(Color{15, 15, 15});
    HittableList world;

    world.add(std::make_shared<Quad>(Point3{555, 0, 0},
                                     Eigen::Vector3d{0, 555, 0},
                                     Eigen::Vector3d{0, 0, 555}, green));
    world.add(std::make_shared<Quad>(Point3{0, 0, 0},
                                     Eigen::Vector3d{0, 555, 0},
                                     Eigen::Vector3d{0, 0, 555}, red));
    world.add(std::make_shared<Quad>(Point3{343, 554, 332},
                                     Eigen::Vector3d{-130, 0, 0},
                                     Eigen::Vector3d{0, 0, -105}, light));
    world.add(std::make_shared<Quad>(Point3{0, 0, 0},
                                     Eigen::Vector3d{555, 0, 0},
                                     Eigen::Vector3d{0, 0, 555}, white));
    world.add(std::make_shared<Quad>(Point3{555, 555, 555},
                                     Eigen::Vector3d{-555, 0, 0},
                                     Eigen::Vector3d{0, 0, -555}, white));
    world.add(std::make_shared<Quad>(Point3{0, 0, 555},
                                     Eigen::Vector3d{555, 0, 0},
                                     Eigen::Vector3d{0, 555, 0}, white));

    Camera camera(800, 16.0 / 9.0, 40, {278, 278, -800}, {278, 278, 0}, 0);
    camera.setSampleCount(1000);
    camera.setShutterSpeed(1.0 / 24.0);
    camera.setRenderDepth(4);
    camera.setRenderThreadCount(12);
    camera.setChunkDimension(64);
    camera.setBackground(Color{0, 0, 0});

    std::shared_ptr<IHittable> box1 =
        box(Point3{0, 0, 0}, Point3{165, 330, 165}, white);

    box1 =
        std::make_shared<Rotation>(box1, 0, deg2Rad(-15), 0, Point3{0, 0, 0});
    box1 = std::make_shared<Translate>(box1, Eigen::Vector3d{265, 0, 295});

    std::shared_ptr<IHittable> box2 =
        box(Point3{0, 0, 0}, Point3{165, 165, 165}, white);

    box2 = std::make_shared<Rotation>(box2, 0, deg2Rad(18), 0, Point3{0, 0, 0});
    box2 = std::make_shared<Translate>(box2, Eigen::Vector3d{130, 0, 65});

    world.add(box1);
    world.add(box2);

    world = HittableList(std::make_shared<BVHNode>(world));

    render(world, camera, "cornell.ppm");
}

void entityRotationTest() {

    auto red = std::make_shared<Lambertian>(Color{.65, .05, .05});
    auto white = std::make_shared<Metal>(Color{.73, .73, .73}, 0.3);
    auto green = std::make_shared<Lambertian>(Color{.12, .45, .15});
    Camera camera(800, 16.0 / 9.0, 40, {278, 278, -800}, {278, 278, 0}, 0);
    camera.setSampleCount(200);
    camera.setShutterSpeed(1.0 / 24.0);
    camera.setRenderDepth(4);
    camera.setRenderThreadCount(12);
    camera.setChunkDimension(64);
    camera.setBackground(Color{0xad / 255.0, 0xd8 / 255.0, 0xe6 / 255.0});

    for (int i = 0; i <= 36; i += 1) {
        auto object = box({80, 80, 80}, {-80, -80, -80}, white);
        auto rot = std::make_shared<Rotation>(object, 0, deg2Rad(i * 10), 0,
                                              Point3{0, 0, 0});
        HittableList world;
        world.add(std::make_shared<Quad>(Point3{555, 0, 0},
                                         Eigen::Vector3d{0, 555, 0},
                                         Eigen::Vector3d{0, 0, 555}, red));
        world.add(std::make_shared<Quad>(Point3{0, 0, 0},
                                         Eigen::Vector3d{0, 555, 0},
                                         Eigen::Vector3d{0, 0, 555}, green));
        world.add(rot);
        render(world, camera, fmt::format("rotation_frame_{}.ppm", i),
               std::string(IMG_OUTPUT_DIR) + "/entityRotationTest");
    }
}
