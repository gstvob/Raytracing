#pragma once

#include "core/vec3.h"

#include <cmath>
#include <memory>

// The actual window(image) size
// Given that i have a desired aspect ratio and width, i can calculate the window height (i.e the image's height)
namespace window {
	constexpr int width = 800;
	constexpr double aspectRatio = 16.0 / 9.0;
	constexpr int height = int(window::width / aspectRatio);
}

namespace camera {
	constexpr double focalLength = 1.0;
	const Vec3 center = { 0.0, 0.0, 0.0 };
}

// The viewport is basically a imaginary rectangle in which im going to use for the calculation of the rays.
// I give it an arbitrary height, and i can calculate the width based on the window aspect ratio
// That way, the viewport will have the same aspect ratio as the window, and facilitates the calculations.

namespace viewport {
	constexpr double height = 2.0;
	constexpr double width = viewport::height * double(window::width) / window::height;
	const Vec3 vU = Vec3(viewport::width, 0.0, 0.0);
	const Vec3 vV = Vec3(0.0, -viewport::height, 0.0);
	const Vec3 pixelDeltaU = vU / window::width;
	const Vec3 pixelDeltaV = vV / window::height;
	const Vec3 upperLeft = camera::center - Vec3(0.0, 0.0, camera::focalLength) - (vU / 2) - (vV / 2);
}

// Obs: The reason as to why the actual aspect ratio is not directly used in the code is because the constant for the aspect ratio is the "desired" ratio, but the actual ratio might be a little bit different than the desired one.

const Vec3 insetHalfAPixel = (0.5 * (viewport::pixelDeltaU + viewport::pixelDeltaV));
const Vec3 firstPixelLocation = viewport::upperLeft + insetHalfAPixel;

const double infinity = std::numeric_limits<double>::infinity();
const double pi = 3.1415926535897932385;

inline double degrees_to_radians(double degrees) {
	return degrees * pi / 180.0;
}