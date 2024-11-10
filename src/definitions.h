#pragma once

#include "core/vec3.h"

#include <cmath>
#include <cstdlib>
#include <memory>


const double infinity = std::numeric_limits<double>::infinity();
const double pi = 3.1415926535897932385;

inline double degreesToRadians(double degrees) {
	return degrees * pi / 180.0;
}

// The actual window(image) size
// Given that i have a desired aspect ratio and width, i can calculate the window height (i.e the image's height)
namespace window {
	constexpr int width = 1200;
	constexpr double aspectRatio = 16.0 / 9.0;
	constexpr int height = int(window::width / aspectRatio);
}

const int samplesPerPixel = 100;
const double pixelSampleScale = 1.0 / double(samplesPerPixel);
const int maxDepth = 50;


struct Interval {
	double min, max;

	double Size() const {
		return max - min;
	}

	bool Contains(double x) const {
		return min <= x && x <= max;
	}
	bool Surrounds(double x) const {
		return min < x && x < max;
	}
	
	double Clamp(double x) const {
		if (x < min) return min;
		if (x > max) return max;
		return x;
	}
};

const Interval empty{
	infinity,
	-infinity
};
const Interval universe{
	-infinity,
	infinity
};
