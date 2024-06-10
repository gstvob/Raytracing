#pragma once

#include "vec3.h"
#include "SDL.h"

using Color = Vec3;

inline void renderColor(SDL_Renderer& renderer, const Color& color, const Vec3& position) {
	SDL_SetRenderDrawColor(&renderer, 255.999 * color.x(), 255.999 * color.y(), 255.999 * color.z(), 255);
	SDL_RenderDrawPoint(&renderer, position.x(), position.y());
}