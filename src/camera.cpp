#include "camera.h"
#include <algorithm>
#include <execution>

void Camera::Render(SDL_Renderer& renderer) {

	for (const auto& pixel : pixelsToRender) {
		renderColor(renderer, pixel.pixelColor, { pixel.x, pixel.y, 0.0 });
	}
}

Color Camera::rayColor(const Ray& ray, int depth, const HittableList& world) const {

	if (depth <= 0) return Color(0, 0, 0);

	HitRecord record;
	if (world.checkHits(ray, Interval{ 0.001, infinity }, record)) {
		Ray scattered{};

		if (record.material.get()->Scatter(ray, record, scattered)) {
			return record.material.get()->GetAlbedo() * rayColor(scattered, depth - 1, world);
		}
		return Color(0, 0, 0);
	
	}

	Vec3 unitDirection = to_unit(ray.direction());
	double factor = 0.5 * (unitDirection.y() + 1.0);
	Color pixelColor = (1.0 - factor) * Color(1.0, 1.0, 1.0) + (factor * Color(0.5, 0.7, 1.0));
	return pixelColor;
}

Ray Camera::getRay(int i, int j) const {
	Vec3 offset = sampleSquare();
	Vec3 samplePixelLocation = firstPixelLocation +
		((i + offset.x()) * this->pixelDeltaU) + 
		((j + offset.y()) * this->pixelDeltaV);

	Vec3 rayOrigin = (defocusAngle <= 0) ? this->center : defocusDiskSample();
	Vec3 rayDirection = samplePixelLocation - rayOrigin;
	
	return Ray(rayOrigin, rayDirection);
}
