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
	

	m_World.addHittable(std::make_shared<Sphere>(Point3(0.0, 0.0, -1), 0.5));
	m_World.addHittable(std::make_shared<Sphere>(Point3(0.0, -100.5, -1), 100));

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
	for (auto y = 0; y < window::height; y++) {
		for (auto x = 0; x < window::width; x++) {
			Vec3 pixelCenter = firstPixelLocation + (x * viewport::pixelDeltaU) + (y * viewport::pixelDeltaV);
			Vec3 rayDirection = pixelCenter - camera::center;
			Ray ray(camera::center, rayDirection);

			Color pixelColor(calculateColor(ray, m_World));
			renderColor(*m_Renderer.get(), pixelColor, {double(x), double(y), 0});
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
