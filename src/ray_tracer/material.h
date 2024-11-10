#ifndef MATERIAL_H
#define MATERIAL_H

#include "object.h"

inline double reflectance(double cosine, double ri) {
    auto r0 = (1 - ri) / (1 + ri);
    r0 = r0 * r0;
    return r0 + (1 - r0) * std::pow((1 - cosine), 5);
}

inline bool scatterLambertian(const Ray& ray, const HitRecord& record, Ray& scatteredRay) {
    Vec3 direction = record.normal + randomUnitVector();
    if (direction.nearZero()) direction = record.normal;
    scatteredRay.origin = record.point;
    scatteredRay.direction = direction;
    return true;
}

inline bool scatterMetal(double fuzzy, const Ray& ray, const HitRecord& record, Ray& scatteredRay) {
    Vec3 reflected = reflect(ray.direction, record.normal);
    reflected = to_unit(reflected) + (fuzzy * randomUnitVector());
    scatteredRay.origin = record.point;
    scatteredRay.direction = reflected;
    return (dot(scatteredRay.direction, record.normal) > 0);
}

inline bool scatterDielectric(double refractionIndex, const Ray& ray, const HitRecord& record, Ray& scatteredRay) {
    Vec3 direction = to_unit(ray.direction);
    
    double ri = record.frontFace ? (1.0 / refractionIndex) : refractionIndex;

    double cosTheta = std::fmin(dot(-direction, record.normal), 1.0);
    double sinTheta = std::sqrt(1.0 - (cosTheta * cosTheta));
    
    bool cannotRefract = (ri * sinTheta) > 1.0;
    Vec3 dir;
    if (cannotRefract || reflectance(cosTheta, ri) > randomDouble()) {
        dir = reflect(direction, record.normal);
    } else {
        dir = refract(direction, record.normal, ri);
    }
    
    scatteredRay.origin = record.point;
    scatteredRay.direction = dir;
    return true;
}


enum MaterialType {
    Lambertian = 0,
    Metal,
    Dielectric
};

struct Material {
    Color albedo;
    double fuzzyness;
    double refractionIndex;
    MaterialType type;

    bool scatter(const Ray& ray, const HitRecord& record, Ray& scatteredRay) {
        switch(type) {
            case MaterialType::Lambertian:
                return scatterLambertian(ray, record, scatteredRay);
            case MaterialType::Metal:
                return scatterMetal(fuzzyness, ray, record, scatteredRay);
            default:
                return scatterDielectric(refractionIndex, ray, record, scatteredRay);
        }
    }
};

#endif