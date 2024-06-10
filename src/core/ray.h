#pragma once

#include "vec3.h"

class Ray {
public:
	Ray() = default;
	Ray(const Point3& origin, const Vec3& direction) : m_Origin(origin), m_Direction(direction) {}

	const Point3& origin() const { return m_Origin; }
	const Vec3& direction() const { return m_Direction; }

	Point3 at(double t) {
		return m_Origin + t * m_Direction;
	}
private:
	// Lembrando que Point3 é simplesmente um alias pro Vec3, é feito assim pra ajudar a interpretação do código (ler como Vec3 algumas funções é esquisito)
	Point3 m_Origin;
	Vec3 m_Direction;
};