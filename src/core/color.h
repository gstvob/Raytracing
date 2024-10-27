#pragma once

#include "vec3.h"
#include "SDL.h"
#include "Ray.h"
#include "hittable.h"
#include "../definitions.h"

using Color = Vec3;

inline Color calculateColor(const Ray& ray, HittableList& world) {
	HitRecord record;
	if (world.checkHits(ray, 0, infinity, record)) {
		return 0.5 * (record.normal + Color(1, 1, 1));
	}

	Vec3 unitDirection = to_unit(ray.direction());
	double factor = 0.5 * (unitDirection.y() + 1.0);
	Color pixelColor = (1.0 - factor) * Color(1.0, 1.0, 1.0) + (factor * Color(0.5, 0.7, 1.0));
	return pixelColor;
}

inline void renderColor(SDL_Renderer& renderer, const Color& color, const Vec3& position) {
	SDL_SetRenderDrawColor(&renderer, 255.999 * color.x(), 255.999 * color.y(), 255.999 * color.z(), 255);
	SDL_RenderDrawPoint(&renderer, position.x(), position.y());
}