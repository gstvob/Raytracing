#pragma once

#include <SDL.h>
#include "core/ray.h"
#include "core/color.h"
#include "core/hittable.h"
#include "core/material.h"

class Camera {
public:
	double focalLength;
	Vec3 center = { 0.0, 0.0, 0.0 };
	
	double vFov = 20;
	double theta;
	double h;
	double viewportHeight;
	double viewportWidth;

	Vec3 vU;
	Vec3 vV;
	Vec3 pixelDeltaU;
	Vec3 pixelDeltaV;
	Vec3 upperLeft;
	Vec3 insetHalfAPixel;
	Vec3 firstPixelLocation;
	
	double defocusAngle = 0.6;  // Variation angle of rays through each pixel
	double focusDistance = 10;
	Vec3 defocusDiskU;       // Defocus disk horizontal radius
	Vec3 defocusDiskV;

	Point3 lookFrom = Point3(13.0, 2.0, 3.0);
	Point3 lookAt = Point3(0.0, 0.0, 0.0);
	Vec3 vecUp = Vec3(0.0, 1.0, 0.0);
	
	Vec3 u, v, w;

	std::vector<Pixel> pixelsToRender;
	std::vector<int> windowHeightIterator;

public:
	Camera() {

		center = lookFrom;

		theta = degreesToRadians(vFov);
		h = theta / 2;
		viewportHeight = 2 * h * focusDistance;
		viewportWidth = viewportHeight * (double(window::width) / window::height);

		w = to_unit(lookFrom - lookAt);
		u = to_unit(cross(vecUp, w));
		v = cross(w, u);

		vU = viewportWidth * u;
		vV = viewportHeight * -v;

		pixelDeltaU = vU / window::width;
		pixelDeltaV = vV / window::height;

		upperLeft = center - (focusDistance * w) - (vU / 2) - (vV / 2);
		insetHalfAPixel = (0.5 * (pixelDeltaU + pixelDeltaV));
		firstPixelLocation = upperLeft + insetHalfAPixel;

		auto defocusRadius = focusDistance * std::tan(degreesToRadians(defocusAngle/ 2));
		defocusDiskU = u * defocusRadius;
		defocusDiskV = v * defocusRadius;

		windowHeightIterator.resize(window::height);

		for (int i = 0; i < window::height; i++) {
			windowHeightIterator.push_back(i);
		}
	}
	void GetRender(const HittableList& world) {
		for (auto y = 0; y < window::height; y++) {
			for (auto x = 0; x < window::width; x++) {

				Color pixelColor(0, 0, 0);

				for (int sample = 0; sample < samplesPerPixel; sample++) {
					Ray r = getRay(int(x), int(y));
					pixelColor = pixelColor + rayColor(r, maxDepth, world);
				}
				pixelsToRender.push_back(Pixel{ double(pixelSampleScale) * pixelColor , double(x), double(y) });
			}
		}
	}
	void Render(SDL_Renderer& renderer);

private:
	Color rayColor(const Ray& ray, int depth, const HittableList& world) const;
	Ray getRay(int i, int j) const;
	Vec3 sampleSquare() const {
		return Vec3(randomDouble() - 0.5, randomDouble() - 0.5, 0);
	}
	Point3 defocusDiskSample() const {
		// Returns a random point in the camera defocus disk.
		Vec3 p = randomUnitDisk();
		return center + (p.x() * defocusDiskU) + (p.y()* defocusDiskV);
	}
};