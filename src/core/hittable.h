#pragma once
#include "ray.h"
#include "../definitions.h"
#include <memory>
#include <vector>


struct HitRecord {
	Point3 point;
	Vec3 normal;
	double t;
	bool frontFace;

	void SetFaceNormal(const Ray& r, const Vec3& outwardNormal) {

		frontFace = dot(r.direction(), outwardNormal) < 0;
		normal = frontFace ? outwardNormal : -outwardNormal;
	}
};

class Hittable {
public:
	virtual ~Hittable() = default;
	virtual bool Hit(const Ray& r, Interval rayInterval, HitRecord& record) const = 0;
};


struct HittableList {
	std::vector<std::shared_ptr<Hittable>> hittableObjects;

	void clearHittables() {
		hittableObjects.clear();
	}

	void addHittable(std::shared_ptr<Hittable> object) {
		hittableObjects.push_back(object);
	}

	bool checkHits(const Ray& r, Interval rayInterval, HitRecord& record) const {
		HitRecord temporaryRecord;
		bool anyHits = false;
		double closestHit = rayInterval.max;

		for (const auto& object : hittableObjects) {
			if (object->Hit(r, Interval{ rayInterval.min, closestHit }, temporaryRecord)) {
				anyHits = true;
				closestHit = record.t;
				record = temporaryRecord;
			}
		}

		return anyHits;
	}
};

