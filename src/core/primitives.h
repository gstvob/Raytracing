#pragma once


#include "hittable.h"
#include "vec3.h"

class Sphere : public Hittable {
public:
	Sphere(const Point3 center, double radius) : m_Center(center), m_Radius(std::fmax(0, radius)){}

	bool Hit(const Ray& r, Interval rayInterval, HitRecord& record) const override {
		Vec3 rayToCenter = m_Center - r.origin();
		double a = r.direction().lenghtSquared();
		double h = dot(r.direction(), rayToCenter);
		double c = rayToCenter.lenghtSquared() - m_Radius * m_Radius;

		// b2 - 4ac
		double discriminant = h * h - a * c;
		if (discriminant < 0) {
			return false;
		}

		double root = (h - sqrt(discriminant)) / a;
		if (!rayInterval.Surrounds(root)) {
			root = (h + sqrt(discriminant)) / a;
			if (!rayInterval.Surrounds(root)) {
				return false;
			}
		}
		record.t = root;
		record.point = r.at(root);

		Vec3 outwardNormal = (record.point - m_Center) / m_Radius;
		record.SetFaceNormal(r, outwardNormal);
	
		return true;
	}

private:
	Point3 m_Center;
	double m_Radius;

};