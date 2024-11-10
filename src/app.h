#ifndef APP_H
#define APP_H

#include <memory>
#include <iostream>

#include "SDL.h"
#undef main

#include "ray_tracer/scene.h"

class App {

public:
	App(int width, int height);
	void init();
	void run();
	void update();
	void render();
private: 
	std::unique_ptr<SDL_Window, decltype(&SDL_DestroyWindow)> m_Window;
	std::unique_ptr<SDL_Renderer, decltype(&SDL_DestroyRenderer)> m_Renderer;
	std::unique_ptr<Scene> m_Scene;
	bool m_Running;
	bool m_Refresh;
};

#endif
