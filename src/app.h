#pragma once

#include <memory>
#include <iostream>

#include "SDL.h"
#undef main

#include "definitions.h"
#include "core/vec3.h"
#include "core/color.h"
#include "core/ray.h"
#include "core/hittable.h"
#include "core/primitives.h"
#include "camera.h"

class App {

public:
	App();
	void run();
	void update();
	void render();
	void handleInputs();
private: 
	std::unique_ptr<SDL_Window, decltype(&SDL_DestroyWindow)> m_Window;
	std::unique_ptr<SDL_Renderer, decltype(&SDL_DestroyRenderer)> m_Renderer;
	HittableList m_World;
	Camera m_Camera;
	bool m_Running;
};