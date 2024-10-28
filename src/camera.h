#pragma once

#include <SDL.h>
#include "core/ray.h"
#include "core/color.h"
#include "core/hittable.h"

class Camera {
public:
	void Render(const HittableList& world, SDL_Renderer& renderer);
private:
	Color rayColor(const Ray& ray, int depth, const HittableList& world) const;
	Ray getRay(int i, int j) const;
	Vec3 sampleSquare() const {
		return Vec3(randomDouble() - 0.5, randomDouble() - 0.5, 0);
	}
};