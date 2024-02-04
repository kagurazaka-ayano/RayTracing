/**
 * @file MathUtil.cpp
 * @author ayano
 * @date 1/16/24
 * @brief
*/

#include "MathUtil.h"

namespace MathUtil{

    Vec3::Vec3(double x, double y, double z): x(x), y(y), z(z) {

    }

    Vec3::Vec3(const Vec3 &other) {
        x = other.x;
        y = other.y;
        z = other.z;
    }

    Vec3::Vec3(Vec3 &&other) noexcept {
        x = other.x;
        y = other.y;
        z = other.z;
    }

    Vec3 &Vec3::operator=(const Vec3 &other) {
        if(this != &other) {
            x = other.x;
            y = other.y;
            z = other.z;
        }
        return *this;
    }

    double Vec3::operator[](int i) const {
        double arr[] = {x, y, z};
        return arr[i];
    }

    double Vec3::operator[](int i) {
        double arr[] = {x, y, z};
        return arr[i];
    }

    Vec3 Vec3::operator-(const Vec3 &other) const {
        return {x - other.x, y - other.y, z - other.z};
    }

    Vec3 &Vec3::operator+=(const Vec3 &other) {
        x += other.x;
        y += other.y;
        z += other.z;
        return *this;
    }

    Vec3 &Vec3::operator*=(double t) {
        x *= t;
        y *= t;
        z *= t;
        return *this;
    }

    Vec3 &Vec3::operator/=(double t) {
        x /= t;
        y /= t;
        z /= t;
        return *this;
    }

    double Vec3::lengthSq() const {
        return x * x + y * y + z * z;
    }

    double Vec3::length() const {
        return std::sqrt(lengthSq());
    }

    Vec3 Vec3::operator-() const {
        return {-x, -y, -z};
    }

    Vec3 Vec3::operator+(const Vec3 &other) const {
        return {x + other.x, y + other.y, z + other.z};
    }

    double Vec3::dot(const Vec3 &other) const {
        return x * other.x + y * other.y + z * other.z;
    }

    Vec3 Vec3::cross(const Vec3 &other) const {
        return {y * other.z - z * other.y, z * other.x - x * other.z, x * other.y - y * other.x};
    }

    Vec3 Vec3::unit() const {
        auto v = *this / this->length();
        return v;
    }

    Vec3 Vec3::operator/(double t) const {
        return {x / t, y / t, z / t};
    }

    Vec3 Vec3::operator*(double t) const {
        return {x * t, y * t, z * t};
    }

    std::string Vec3::makeColor() const {
        return fmt::format("{} {} {}\n", static_cast<int>(x * 255.999), static_cast<int>(y * 255.999), static_cast<int>(z * 255.999));
    }

    Vec3::operator std::string() const {
        return fmt::format("Vec3: {:.6f} {:.6f} {:.6f}", x, y, z);
    }

    Vec3::Vec3(): Vec3(0, 0, 0) {

    }
	Vec3 Vec3::random() {
		return {randomDouble(), randomDouble(), randomDouble()};
	}

	Vec3 Vec3::random(double min, double max) {
		return Vec3(randomDouble(min, max), randomDouble(min, max), randomDouble(min, max));
	}

	Vec3 Vec3::randomVec3InUnitSphere() {
		while (true) {
			auto p = Vec3::random(-1, 1);
			if (p.lengthSq() >= 1) continue;
			return p;
		}
	}

	Vec3 Vec3::randomUnitVec3() {
		return randomVec3InUnitSphere().unit();
	}

	Vec3 Vec3::randomUnitVec3InHemiSphere(const Vec3 &normal) {
		Vec3 in_unit_sphere = randomVec3InUnitSphere();
		if (in_unit_sphere.dot(normal) > 0.0) {
			return in_unit_sphere;
		}
		else {
			return -in_unit_sphere;
		}
	}


	std::ostream& operator<<(std::ostream &out, const Vec3 &other) {
        out << "Vec3: " << other.x << " " << other.y << " " << other.z;
        return out;
    }

    Ray::Ray(MathUtil::Vec3 pos, MathUtil::Vec3 dir) : position(std::move(pos)), direction(std::move(dir)) {

    }

    Point3 Ray::at(double t) const {
        return position + direction * t;
    }

    Vec3 Ray::dir() const {
        return direction;
    }

    Vec3 Ray::pos() const {
        return position;
    }
	Interval::Interval() : min(-INF), max(INF) {

	}
	Interval::Interval(double min, double max): min(min), max(max) {

	}
	bool Interval::within(double x) const {
		return (min <= x) && (x <= max);
	}
	bool Interval::surround(double x) const {
		return (min < x) && (x < max);
	}

}



