#pragma once

#include "vec3.h"

class Ray {
public:
	Ray() = default;
	Ray(const Vec3& origin, const Vec3& direction) : m_Origin(origin), m_Direction(direction) {}

	const Vec3& origin() const { return m_Origin; }
	const Vec3& direction() const { return m_Direction; }

	Vec3 at(double t) const {
		return m_Origin + t * m_Direction;
	}
private:
	Vec3 m_Origin;
	Vec3 m_Direction;
};



inline double hitSphere(const Vec3& sphereCenter, double sphereRadius, const Ray& ray) {
	Vec3 rayToCenter = sphereCenter - ray.origin();
	double a = dot(ray.direction(), ray.direction());
	double h = dot(ray.direction(), rayToCenter);
	double c = dot(rayToCenter, rayToCenter) - sphereRadius * sphereRadius;

	// b2 - 4ac
	double discriminant = h * h - a * c;
	//print(discriminant);
	if (discriminant < 0) {
		return -1;
	}
	return (h - sqrt(discriminant)) / a;
}
