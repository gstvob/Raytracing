#ifndef INTERVAL_H
#define INTERVAL_H

#include <cmath>
#include <cstdlib>
#include <memory>

const double infinity = std::numeric_limits<double>::infinity();
const double pi = 3.1415926535897932385;

const int samplesPerPixel = 500;
const double pixelSampleScale = 1.0 / double(samplesPerPixel);
const int maxDepth = 50;

inline double degreesToRadians(double degrees) {
	return degrees * pi / 180.0;
}

inline double linearToGamma(double linearComponent) {
	if (linearComponent > 0) {
		return std::sqrt(linearComponent);
	}
	return 0;
}

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


#endif