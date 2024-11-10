#pragma once

#include "vec3.h"
#include "SDL.h"
#include "Ray.h"
#include "hittable.h"
#include "../definitions.h"

using Color = Vec3;

inline Color calculateColor(const Ray& ray, HittableList& world) {
	HitRecord record;
	if (world.checkHits(ray, Interval{0, infinity}, record)) {
		return 0.5 * (record.normal + Color(1, 1, 1));
	}

	Vec3 unitDirection = to_unit(ray.direction());
	double factor = 0.5 * (unitDirection.y() + 1.0);
	Color pixelColor = (1.0 - factor) * Color(1.0, 1.0, 1.0) + (factor * Color(0.5, 0.7, 1.0));
	return pixelColor;
}

inline double linearToGamma(double linearComponent) {
	if (linearComponent > 0) {
		return std::sqrt(linearComponent);
	}
	return 0;
}


struct Pixel {
	Color pixelColor;
	double x;
	double y;
};

inline void renderColor(SDL_Renderer& renderer, const Color& color, const Vec3& position) {

	const Interval intensity{ 0.000, 0.999 };

	double r = linearToGamma(color.x());
	double g = linearToGamma(color.y());
	double b = linearToGamma(color.z());

	SDL_SetRenderDrawColor(&renderer, 255.999 * intensity.Clamp(r), 255.999 * intensity.Clamp(g), 255.999 * intensity.Clamp(b), 255);
	SDL_RenderDrawPoint(&renderer, position.x(), position.y());
}