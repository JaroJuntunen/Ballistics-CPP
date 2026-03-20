#include <SDL3/SDL.h>
#include <iostream>
#include "renderer/Renderer.hpp"
#include "input/InputHandler.hpp"

static constexpr int WINDOW_WIDTH  = 1280;
static constexpr int WINDOW_HEIGHT = 720;

int main()
{
	try {
		Renderer renderer("Ballistics", WINDOW_WIDTH, WINDOW_HEIGHT);
		InputHandler input;

		SDL_Event event;

		static constexpr int    TARGET_FPS   = 60;
		const         Uint64    FREQ         = SDL_GetPerformanceFrequency();
		const         Uint64    FRAME_TICKS  = FREQ / TARGET_FPS;

		while (!input.shouldQuit()) {
			Uint64 frameStart = SDL_GetPerformanceCounter();

			while (SDL_PollEvent(&event))
				input.handleEvent(event);

			if (input.wasResized()) {
				renderer.updateScale(input.newWidth(), input.newHeight());
				input.clearResized();
			}

			if (input.isDragging()) {
				renderer.moveCamera(input.dragDeltaX(), input.dragDeltaY());
				input.clearDrag();
			}

			if (input.zoomDelta() != 0.0f) {
				renderer.zoom(input.zoomDelta());
				input.clearZoom();
			}

			if (input.shouldMoveCameraToLauncher()) {
				renderer.setCameraToLauncher();
				input.clearMoveCameraToLauncher();
			}

			renderer.update();
			renderer.clear();
			renderer.renderTerrain();
			renderer.present();

			Uint64 elapsed = SDL_GetPerformanceCounter() - frameStart;
			if (elapsed < FRAME_TICKS) {
				Uint32 msRemaining = (Uint32)((FRAME_TICKS - elapsed) * 1000 / FREQ);
				if (msRemaining > 0)
					SDL_Delay(msRemaining);
			}
		}
	}
	catch (const std::exception& e) {
		std::cerr << e.what() << '\n';
		return 1;
	}

	return 0;
}
