#pragma once

#include <memory>
#include <iostream>

#include "SDL.h"
#undef main

#include "utils.h"
#include "core/vec3.h"
#include "core/color.h"
// Classe vai cuidar da criação da janela, e renderização (não vou criar um renderer separado)
// 
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

	bool m_Running;
};