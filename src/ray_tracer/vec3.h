#ifndef VEC3_H
#define VEC3_H

#include <math.h>
#include <iostream>

inline double randomDouble() {
	// Random number in [0, 1)
	return std::rand() / (RAND_MAX + 1.0);
}

inline double randomDouble(double min, double max) {
	// Random number in [min, max)
	return min + (max - min) * randomDouble();
}

class Vec3 {

public:
	double x, y, z;

	Vec3(): x( 0.0 ), y(0.0), z(0.0) {}
	Vec3(double v1, double v2, double v3) : x( v1), y(v2) ,z(v3)  {}

	Vec3 operator-() const { return Vec3(-x, -y, -z); }

	Vec3& operator+=(const Vec3& v) {
		x += v.x;
		y += v.y;
		z += v.z;
		return *this;
	}
	Vec3& operator*= (const double t) {
		x *= t;
		y *= t;
		z *= t;
		return *this;
	}
	Vec3& operator/=(double t) {
		return *this *= (1 / t);
	}

	double lenghtSquared () const {
		return x * x + y * y + z * z;
	}

	double length() const {
		return sqrt(lenghtSquared());
	}

	static Vec3 random() {
		return Vec3(randomDouble(), randomDouble(), randomDouble());
	}

	static Vec3 random(double min, double max) {
		return Vec3(randomDouble(min, max), randomDouble(min, max), randomDouble(min, max));
	}

	bool nearZero() const {
		auto s = 1e-8;
		return (std::fabs(x) < s) && (std::fabs(y) < s) && (std::fabs(z) < s);
	}
};

/*
* These aliases are just for ease of comprehension of the code, given that functionally they are the same thing, but semantically they differ
* i.e A color is defined by (R,G,B) tuple, a Point is a (X,Y,Z) point in space, and vector is a direction pointing to (X,Y,Z)
*/

using Point3 = Vec3;
using Color = Vec3;

inline std::ostream& operator<<(std::ostream& out, const Vec3& v) {
	return out << v.x << " " << v.y << " " << v.z;
}
inline Vec3 operator+(const Vec3& a, const Vec3& b) {
	return Vec3(a.x + b.x, a.y + b.y, a.z + b.z);
}
inline Vec3 operator-(const Vec3& a, const Vec3& b) {
	return Vec3(a.x - b.x, a.y - b.y, a.z - b.z);
}
inline Vec3 operator*(double t, const Vec3& v) {
	return Vec3(t * v.x, t * v.y, t * v.z);
}
inline Vec3 operator*(const Vec3& v, double t) {
	return t * v;
}
inline Vec3 operator*(const Vec3& u, const Vec3& v) {
	return Vec3(u.x * v.x, u.y * v.y, u.z * v.z);
}
inline Vec3 operator/(const Vec3& v, double t) {
	return (1 / t) * v;
}

inline double dot(const Vec3& a, const Vec3& b) {
	return a.x * b.x + a.y * b.y + a.z * b.z;
}
inline Vec3 cross(const Vec3& a, const Vec3& b) {
	return Vec3(a.y * b.z - a.z * b.y,
		a.z * b.x - a.x * b.z,
		a.x * b.y - a.y * b.x);
}

inline Vec3 to_unit(const Vec3& v) {
	return v / v.length();
}


inline Vec3 reflect(const Vec3& v, const Vec3& n) {
	return v - 2 * dot(v, n) * n;
}

inline Vec3 refract(const Vec3& uv, const Vec3& n, double refractionIndex) {

	auto cosTheta = std::fmin(dot(-uv, n), 1.0);
	Vec3 rPerpendicular = refractionIndex * (uv + cosTheta * n);
	Vec3 rParallel = -1.0 * std::sqrt(std::fabs(1 - rPerpendicular.lenghtSquared())) * n;

	return rPerpendicular + rParallel;
}

inline Vec3 randomUnitVector() {
	while (true) {
		Vec3 p = Vec3::random(-1, 1);
		double lensq = p.lenghtSquared();
		// Here theres a chance that a sqrt of a small enough value rounds to 0
		// thus making a division by 0, by being greater than 10^-160 i guarantee that 
		// it is never happening

		if (1e-160 < lensq && lensq <= 1) {
			return p / sqrt(lensq);
		}
	}
}

inline Vec3 randomOnHemisphere(const Vec3& normal) {
	Vec3 onUnitSphere = randomUnitVector();
	if (dot(onUnitSphere, normal) > 0.0) {
		return onUnitSphere;
	} else {
		return -onUnitSphere;
	}
}

inline Vec3 randomUnitDisk() {
	while (true) {
		auto p = Vec3(randomDouble(-1, 1), randomDouble(-1, 1), 0);
		if (p.lenghtSquared() < 1) {
			return p;
		}
	}
}

inline Vec3 sampleSquare() {
	return Vec3(randomDouble() - 0.5, randomDouble() - 0.5, 0);
}

#endif