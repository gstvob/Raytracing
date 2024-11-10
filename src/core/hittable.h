#pragma once
#include "ray.h"
#include "../definitions.h"
#include <memory>
#include <vector>

#include "material.h"

class Material;


struct HitRecord {
	Point3 point;
	Vec3 normal;
	double t;
	bool frontFace;
	std::shared_ptr<Material> material;

	void SetFaceNormal(const Ray& r, const Vec3& outwardNormal) {
		frontFace = dot(r.direction(), outwardNormal) < 0;
		normal = frontFace ? outwardNormal : -outwardNormal;
	}
};

class Material {
public:
	Vec3 albedo;
public:
	Material(const Vec3& a) : albedo(a) {}

	virtual bool Scatter(const Ray& ray, const HitRecord& record, Ray& scatteredRay) const { return false; }

	Vec3 GetAlbedo() const { return albedo; }
};

class Lambertian : public Material {
public:
	Lambertian(const Vec3& a) : Material(a) {}
	bool Scatter(const Ray& r, const HitRecord& rec, Ray& scatteredRay) const override {
		Vec3 direction = rec.normal + randomUnitVector();
		if (direction.nearZero()) direction = rec.normal;
		scatteredRay.SetOrigin(rec.point);
		scatteredRay.SetDirection(direction);
		return true;
	}
};

class Metal : public Material {
public:
	Metal(const Vec3& a, double fuzz) : Material(a), m_Fuzz(fuzz < 1 ? fuzz : 1) {}
	bool Scatter(const Ray& r, const HitRecord& rec, Ray& scatteredRay) const override {
		Vec3 reflected = reflect(r.direction(), rec.normal);
		reflected = to_unit(reflected) + (m_Fuzz * randomUnitVector());
		scatteredRay.SetOrigin(rec.point);
		scatteredRay.SetDirection(reflected);
		return (dot(scatteredRay.direction(), rec.normal) > 0);
	}
private:
	double m_Fuzz;
};


class Dielectric : public Material {
public:
	Dielectric(double refractionIndex) : Material(Vec3(1.0, 1.0, 1.0)), m_RefractionIndex(refractionIndex) {}
	bool Scatter(const Ray& r, const HitRecord& rec, Ray& scatteredRay) const override {
		Vec3 direction = to_unit(r.direction());
		
		double ri = rec.frontFace ? (1.0 / m_RefractionIndex) : m_RefractionIndex;

		double cosTheta = std::fmin(dot(-direction, rec.normal), 1.0);
		double sinTheta = std::sqrt(1.0 - (cosTheta * cosTheta));
		
		bool cannotRefract = (ri * sinTheta) > 1.0;
		Vec3 dir;
		if (cannotRefract || reflectance(cosTheta, ri) > randomDouble()) {
			dir = reflect(direction, rec.normal);
		} else {
			dir = refract(direction, rec.normal, ri);
		}
		
		scatteredRay.SetOrigin(rec.point);
		scatteredRay.SetDirection(dir);
		return true;
	}

private: 
	double m_RefractionIndex;

private:
	static double reflectance(double cosine, double ri) {
		auto r0 = (1 - ri) / (1 + ri);
		r0 = r0 * r0;
		return r0 + (1 - r0) * std::pow((1 - cosine), 5);
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
				closestHit = temporaryRecord.t;
				record = temporaryRecord;
			}
		}

		return anyHits;
	}
};

