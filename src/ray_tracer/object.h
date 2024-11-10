#ifndef OBJECT_H
#define OBJECT_H

#include "vec3.h"
#include "interval.h"


struct Pixel {
    Color color;
    double x;
    double y;  
};

struct Ray {
    Vec3 origin;
    Vec3 direction;
    
    // Optmization
    double directionLengthSquared;

    Vec3 at(double t) const {
        return origin + t * direction;
    }
};

struct HitRecord {
	Point3 point;
	Vec3 normal;
	double t;
	bool frontFace;
	int materialIndex;

	void setFaceNormal(const Ray& r, const Vec3& outwardNormal) {
		frontFace = dot(r.direction, outwardNormal) < 0;
		normal = frontFace ? outwardNormal : -outwardNormal;
	}
};


class Sphere { 

public:
    Sphere(const Point3 center, double radius, int materialIndex) : m_Center(center), m_Radius(std::fmax(0, radius)), m_MaterialIndex(materialIndex) {}
    
    bool hit(const Ray& r, Interval rayInterval, HitRecord& record) const {
		Vec3 rayToCenter = m_Center - r.origin;
		double h = dot(r.direction, rayToCenter);
		double c = rayToCenter.lenghtSquared() - m_Radius * m_Radius;

		// b2 - 4ac
		double discriminant = h * h - r.directionLengthSquared * c;
		if (discriminant < 0) {
			return false;
		}

		double root = (h - sqrt(discriminant)) / r.directionLengthSquared;
		if (!rayInterval.Surrounds(root)) {
			root = (h + sqrt(discriminant)) / r.directionLengthSquared;
			if (!rayInterval.Surrounds(root)) {
				return false;
			}
		}
		record.t = root;
		record.point = r.at(root);

		Vec3 outwardNormal = (record.point - m_Center) / m_Radius;
		record.setFaceNormal(r, outwardNormal);
		record.materialIndex = m_MaterialIndex;
		return true;
    }
private:
    Vec3 m_Center;
    int m_MaterialIndex;
    double m_Radius;
};

#endif