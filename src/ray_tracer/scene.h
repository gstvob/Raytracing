#ifndef SCENE_H
#define SCENE_H

#include <vector>
#include <algorithm>
#include <execution>
#include "camera.h"
#include "interval.h"
#include "material.h"

class Scene {
public:
    Scene(int width, int height): m_Width(width), m_Height(height), m_Camera(width, height) {
        m_VerticalIterator.resize(height);

        for (int i = 0; i < height; i++) {
            m_VerticalIterator[i] = i;
        }
        pixelsToRender.resize(width * height);
    }
    ~Scene() = default;
    void getPixels() {
        std::for_each(std::execution::par, m_VerticalIterator.begin(), m_VerticalIterator.end(), [&](int y) {
            for (auto x = 0; x < m_Width; x++) {
                Color pixelColor(0, 0, 0);
                for (int sample = 0; sample < samplesPerPixel; sample++) {
                    Ray r = m_Camera.rayCast(int(x), int(y));
                    pixelColor = pixelColor + getPixelColor(r, maxDepth);
                }
                Pixel pixel = { pixelSampleScale * pixelColor , double(x), double(y) };
                pixelsToRender[x + y * m_Width] = pixel;
            }
        });
    }
    void addObject(const Sphere& object, const Material& material) {
        m_ObjectsList.push_back(object);
        m_MaterialList.push_back(material);
    }
    std::vector<Pixel> pixelsToRender;
private:
    std::vector<Sphere> m_ObjectsList; 
    std::vector<Material> m_MaterialList;
    std::vector<int> m_VerticalIterator;
    Camera m_Camera;
    int m_Width, m_Height;
private: 
    bool checkHits(Ray& r, Interval rayInterval, HitRecord& record) const {
		HitRecord temporaryRecord;
		bool anyHits = false;
		double closestHit = rayInterval.max;
		for (const Sphere& object : m_ObjectsList) {
			if (object.hit(r, Interval{ rayInterval.min, closestHit }, temporaryRecord)) {
				anyHits = true;
				closestHit = temporaryRecord.t;
				record = temporaryRecord;
			}
		}

		return anyHits;
	}

    Color getPixelColor(Ray& ray, int depth) {
        if (depth <= 0) return Color(0, 0, 0);

        HitRecord record;
        ray.directionLengthSquared = ray.direction.lenghtSquared();
        if (checkHits(ray, Interval{ 0.001, infinity }, record)) {
            Ray scattered{};

            if (m_MaterialList[record.materialIndex].scatter(ray, record, scattered)) {
                return m_MaterialList[record.materialIndex].albedo * getPixelColor(scattered, depth - 1);
            }
            return Color(0, 0, 0);  
        }

        Vec3 unitDirection = to_unit(ray.direction);
        double factor = 0.5 * (unitDirection.y + 1.0);
        Color pixelColor = (1.0 - factor) * Color(1.0, 1.0, 1.0) + (factor * Color(0.5, 0.7, 1.0));
        return pixelColor;
    }

};

#endif