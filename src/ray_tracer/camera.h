#ifndef CAMERA_H
#define CAMERA_H

#include "vec3.h"
#include "interval.h"
#include "object.h"

class Camera {
public:
    Camera(int width, int height) {
        m_Width = width;
        m_Height = height;
        m_Vfov = 20;

        m_DefocusAngle = 0.6;  // Variation angle of rays through each pixel
        m_FocusDistance = 10;

		double theta = degreesToRadians(m_Vfov);
		double h = theta / 2;
		double viewportHeight = 2 * h * m_FocusDistance;
		double viewportWidth = viewportHeight * (double(m_Width) / double(m_Height));

	    Vec3 w = to_unit(m_LookFrom - m_LookAt);
		Vec3 u = to_unit(cross(m_VecUp, w));
		Vec3 v = cross(w, u);

	    Vec3 vU = viewportWidth * u;
	    Vec3 vV = viewportHeight * -1 * v;

		m_PixelDeltaU = vU / m_Width;
		m_PixelDeltaV = vV / m_Height;

		Vec3 upperLeft = m_LookFrom - (m_FocusDistance * w) - (vU / 2) - (vV / 2);
		Vec3 insetHalfAPixel = (0.5 * (m_PixelDeltaU + m_PixelDeltaV));
		m_FirstPixelLocation = upperLeft + insetHalfAPixel;

		auto defocusRadius = m_FocusDistance * std::tan(degreesToRadians(m_DefocusAngle/ 2));
		m_DefocusDiskU = u * defocusRadius;
		m_DefocusDiskV = v * defocusRadius;
    }

    
    Ray rayCast(int i, int j) {
        Vec3 offset = sampleSquare();
        Vec3 samplePixelLocation = m_FirstPixelLocation +
            ((i + offset.x) * m_PixelDeltaU) + 
            ((j + offset.y) * m_PixelDeltaV);

        Vec3 rayOrigin = (m_DefocusAngle <= 0) ? m_LookFrom : defocusDiskSample();
        Vec3 rayDirection = samplePixelLocation - rayOrigin;
        
        return Ray{rayOrigin, rayDirection};
    }
private:
    int m_Width, m_Height;
    double m_Vfov;

    Vec3 m_PixelDeltaU, m_PixelDeltaV;
    Vec3 m_FirstPixelLocation;

    double m_DefocusAngle, m_FocusDistance;
    Vec3 m_DefocusDiskU, m_DefocusDiskV;

    Point3 m_LookFrom = Point3(13.0, 2.0, 3.0);
	Point3 m_LookAt = Point3(0.0, 0.0, 0.0);
	Vec3 m_VecUp = Vec3(0.0, 1.0, 0.0);
private:

    Point3 defocusDiskSample() const {
	    // Returns a random point in the camera defocus disk.
	    Vec3 p = randomUnitDisk();
	    return m_LookFrom + (p.x * m_DefocusDiskU) + (p.y * m_DefocusDiskV);
    }

};

#endif