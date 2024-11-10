#include "app.h"

App::App(int width, int height) : m_Refresh(true), m_Window(nullptr, SDL_DestroyWindow), m_Renderer(nullptr, SDL_DestroyRenderer), m_Running(true), m_Scene(std::make_unique<Scene>(width, height)) {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		std::cout << "Erro ao iniciar SDL" << std::endl;
		exit(1);
	}

	m_Window.reset(SDL_CreateWindow("Raytracer", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, 0));

	if (m_Window == nullptr) {
		std::cout << "Erro ao criar janela" << std::endl;
		exit(1);
	}

	m_Renderer.reset(SDL_CreateRenderer(m_Window.get(), -1, SDL_RENDERER_ACCELERATED));
	if (m_Renderer == nullptr) {
		std::cout << "Erro ao criar renderer" << std::endl;
		exit(1);
	}
    init();
}

void App::init() {

    int materialIndex = 0;

	for (int a = -11; a < 11; a++) {
		for (int b = -11; b < 11; b++) {
			double chosenMaterial = randomDouble();
			Point3 center(a + 0.9 * randomDouble(), 0.2, b + 0.9 * randomDouble());

			if ((center - Point3(4, 0.2, 0)).length() > 0.9) {
				Material sphereMaterial;

				if (chosenMaterial < 0.8) {
					sphereMaterial.albedo = Color::random() * Color::random();
                    sphereMaterial.type = MaterialType::Lambertian;
					m_Scene->addObject(Sphere(center, 0.2, materialIndex), sphereMaterial);
				}
				else if (chosenMaterial < 0.95) {
					// metal
					sphereMaterial.albedo = Color::random(0.5, 1);
                    sphereMaterial.fuzzyness = randomDouble(0, 0.5);;
                    sphereMaterial.type = MaterialType::Metal;
					m_Scene->addObject(Sphere(center, 0.2, materialIndex), sphereMaterial);
				}
				else {
					// glass
					sphereMaterial.albedo = Color(1.0, 1.0, 1.0);
                    sphereMaterial.refractionIndex = 1.5;
                    sphereMaterial.type = MaterialType::Dielectric;
					m_Scene->addObject(Sphere(center, 0.2, materialIndex), sphereMaterial);
				}
			    materialIndex++;
			}
		}
	}

	Material material1{Color(1.0, 1.0, 1.0), 0, 1.5, MaterialType::Dielectric};
    m_Scene->addObject(Sphere(Point3(0, 1, 0), 1.0, materialIndex), material1);

	Material material2{Color(0.4, 0.2, 0.1), 0, 0, MaterialType::Lambertian};
    m_Scene->addObject(Sphere(Point3(-4, 1, 0), 1.0, materialIndex+1), material2);

	Material material3{Color(0.7, 0.6, 0.5), 0.0, 0.0, MaterialType::Metal};
	m_Scene->addObject(Sphere(Point3(4, 1, 0), 1.0, materialIndex+2), material3);

    Material groundMaterial = Material{(Color(0.5, 0.5, 0.5)), 0, 0, Lambertian};
	m_Scene->addObject(Sphere(Point3(0, -1000, 0), 1000, materialIndex+3), groundMaterial);
}

void App::run() {
    while (m_Running) {
		if (m_Refresh) {
			SDL_SetRenderDrawColor(m_Renderer.get(), 0, 0, 0, 255);
			SDL_RenderClear(m_Renderer.get());
			render();
		}
        update();
    }
}

void App::render() {
    m_Scene->getPixels();
    for (const Pixel& pixel : m_Scene->pixelsToRender) {
        const Interval intensity{ 0.000, 0.999 };
		double r = 255.999 * intensity.Clamp(pixel.color.x);
		double g = 255.999 * intensity.Clamp(pixel.color.y);
		double b = 255.999 * intensity.Clamp(pixel.color.z);

        SDL_SetRenderDrawColor(m_Renderer.get(), r, g, b, 255);
        SDL_RenderDrawPoint(m_Renderer.get(), pixel.x, pixel.y);       
    }
    SDL_RenderPresent(m_Renderer.get());
}


void App::update() {
    m_Refresh = false;
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