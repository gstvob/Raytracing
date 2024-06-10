#include "app.h"

App::App() : m_Window(nullptr, SDL_DestroyWindow), m_Renderer(nullptr, SDL_DestroyRenderer), m_Running(true) {
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		std::cout << "Erro ao iniciar SDL" << std::endl;
		exit(1);
	}

	m_Window.reset(SDL_CreateWindow("Raytracer", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, window::width, window::height, 0));

	if (m_Window == nullptr) {
		std::cout << "Erro ao criar janela" << std::endl;
		exit(1);
	}

	m_Renderer.reset(SDL_CreateRenderer(m_Window.get(), -1, SDL_RENDERER_ACCELERATED));
	if (m_Renderer == nullptr) {
		std::cout << "Erro ao criar renderer" << std::endl;
		exit(1);
	}
}


void App::run() {
	m_Running = true;
	while (m_Running) {
		SDL_SetRenderDrawColor(m_Renderer.get(), 0, 0, 0, 255);
		SDL_RenderClear(m_Renderer.get());
		update();
		render();
	}
}

void App::update() {
	handleInputs();
}

void App::render() {
	for (auto x = 0; x < window::width; x++) {
		for (auto y = 0; y < window::height; y++) {
			Color color(double(x) / (window::width - 1), double(y) / (window::height - 1), 0);
			renderColor(*m_Renderer.get(), color, {double(x), double(y), 0});
			/*SDL_SetRenderDrawColor(m_Renderer.get(), x, y, 153, 255);
			SDL_RenderDrawPoint(m_Renderer.get(), x, y);*/
		}
	}

	SDL_RenderPresent(m_Renderer.get());
}

void App::handleInputs() {
	SDL_Event event;

	while (SDL_PollEvent(&event)) {
		switch (event.type) {
			case SDL_QUIT:
				m_Running = false;
				break;
			default:
				break;
		}
	}
}
