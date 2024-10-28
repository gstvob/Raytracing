#pragma once


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
	Vec3() : m_Vals{ 0.0, 0.0, 0.0 } {}
	Vec3(double v1, double v2, double v3) : m_Vals{ v1, v2 ,v3 } {}

	double x() const { return m_Vals[0]; }
	double y() const { return m_Vals[1]; }
	double z() const { return m_Vals[2]; }

	Vec3 operator-() const { return Vec3(-m_Vals[0], -m_Vals[1], -m_Vals[2]); }
	double operator[](int i) const { return m_Vals[i]; }
	double& operator[](int i) { return m_Vals[i]; }

	Vec3& operator+=(const Vec3& v) {
		m_Vals[0] += v[0];
		m_Vals[1] += v[1];
		m_Vals[2] += v[2];
		return *this;
	}
	Vec3& operator*= (const double t) {
		m_Vals[0] *= t;
		m_Vals[1] *= t;
		m_Vals[2] *= t;
		return *this;
	}
	Vec3& operator/=(double t) {
		return *this *= (1 / t);
	}

	double lenghtSquared () const {
		return m_Vals[0] * m_Vals[0] + m_Vals[1] * m_Vals[1] + m_Vals[2] * m_Vals[2];
	}

	double length() const {
		return sqrt(lenghtSquared());
	}

	static Vec3 Random() {
		return Vec3(randomDouble(), randomDouble(), randomDouble());
	}

	static Vec3 Random(double min, double max) {
		return Vec3(randomDouble(min, max), randomDouble(min, max), randomDouble(min, max));
	}

private:
	double m_Vals[3];
};

using Point3 = Vec3;

inline std::ostream& operator<<(std::ostream& out, const Vec3& v) {
	return out << v.x() << " " << v.y() << " " << v.z();
}
inline Vec3 operator+(const Vec3& a, const Vec3& b) {
	return Vec3(a.x() + b.x(), a.y() + b.y(), a.z() + b.z());
}
inline Vec3 operator-(const Vec3& a, const Vec3& b) {
	return Vec3(a.x() - b.x(), a.y() - b.y(), a.z() - b.z());
}
inline Vec3 operator*(double t, const Vec3& v) {
	return Vec3(t * v.x(), t * v.y(), t * v.z());
}
inline Vec3 operator*(const Vec3& v, double t) {
	return t * v;
}
inline Vec3 operator/(const Vec3& v, double t) {
	return (1 / t) * v;
}

inline double dot(const Vec3& a, const Vec3& b) {
	return a.x() * b.x() + a.y() * b.y() + a.z() * b.z();
}
inline Vec3 cross(const Vec3& a, const Vec3& b) {
	return Vec3(a.y() * b.z() - a.z() * b.y(),
		a.z() * b.x() - a.x() * b.z(),
		a.x() * b.y() - a.y() * b.x());
}

inline Vec3 to_unit(const Vec3& v) {
	return v / v.length();
}


inline Vec3 randomUnitVector() {
	while (true) {
		Vec3 p = Vec3::Random(-1, 1);
		double lensq = p.lenghtSquared();
		// Here theres a chance that a sqrt of a small enough value rounds to 0
		// thus making a division by 0, by being greater than 10^-160 i guarantee that 
		// it is never happening

		if (10e-160 < lensq && lensq <= 1) {
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