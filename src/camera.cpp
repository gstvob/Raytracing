#include "camera.h"

void Camera::Render(const HittableList& world, SDL_Renderer& renderer) {
	for (auto y = 0; y < window::height; y++) {
		for (auto x = 0; x < window::width; x++) {
			
			Color pixelColor(0, 0, 0);

			for (int sample = 0; sample < samplesPerPixel; sample++) {
				Ray r = getRay(int(x), int(y));
				pixelColor = pixelColor + rayColor(r, maxDepth, world);
			}
			
			/*Vec3 pixelCenter = firstPixelLocation + (x * viewport::pixelDeltaU) + (y * viewport::pixelDeltaV);
			Vec3 rayDirection = pixelCenter - camera::center;
			Ray ray(camera::center, rayDirection);*/

			//Color pixelColor(rayColor(ray, world));
			renderColor(renderer, double(pixelSampleScale) * pixelColor, { double(x), double(y), 0 });
		}
	}
}

Color Camera::rayColor(const Ray& ray, int depth, const HittableList& world) const {

	if (depth <= 0) return Color(0, 0, 0);

	HitRecord record;
	if (world.checkHits(ray, Interval{ 0.001, infinity }, record)) {
		//Vec3 direction = randomOnHemisphere(record.normal);
		Vec3 direction = record.normal + randomUnitVector();
		return 0.5 * rayColor(Ray(record.point, direction), depth - 1, world);
	}

	Vec3 unitDirection = to_unit(ray.direction());
	double factor = 0.5 * (unitDirection.y() + 1.0);
	Color pixelColor = (1.0 - factor) * Color(1.0, 1.0, 1.0) + (factor * Color(0.5, 0.7, 1.0));
	return pixelColor;
}

Ray Camera::getRay(int i, int j) const {
	Vec3 offset = sampleSquare();
	Vec3 samplePixelLocation = firstPixelLocation +
		((i + offset.x()) * viewport::pixelDeltaU) + 
		((j + offset.y()) * viewport::pixelDeltaV);

	Vec3 rayOrigin = camera::center;
	Vec3 rayDirection = samplePixelLocation - rayOrigin;
	
	return Ray(rayOrigin, rayDirection);
}
