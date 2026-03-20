#include <SDL3/SDL.h>
#include <iostream>
#include <memory>
#include "renderer/Renderer.hpp"
#include "input/InputHandler.hpp"
#include "simulation/Projectile.hpp"

static constexpr int WINDOW_WIDTH  = 1280;
static constexpr int WINDOW_HEIGHT = 720;

int main()
{
	try {
		Renderer renderer("Ballistics", WINDOW_WIDTH, WINDOW_HEIGHT);
		InputHandler input;

		SDL_Event event;
		Envirnoment envirnoment;

		static constexpr int    TARGET_FPS   = 60;
		const         Uint64    FREQ         = SDL_GetPerformanceFrequency();
		const         Uint64    FRAME_TICKS  = FREQ / TARGET_FPS;

		float smoothFPS = (float)TARGET_FPS;
		std::unique_ptr<Projectile> projectile = nullptr;

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

			if (input.shouldShootProjectiles()) {
				Vec2 launchPos  = renderer.getLauncherPosition();
				Vec2 mouseWorld = renderer.screenToWorld(input.mouseX(), input.mouseY());
				Vec2 dir        = mouseWorld - launchPos;
				projectile = std::make_unique<Projectile>(launchPos, dir);
				input.clearShootProjectiles();
			}
			
			constexpr float LAUNCHER_SPEED = 0.75f; // m/frame
			if (input.isMovingLeft()) renderer.moveLauncher(-LAUNCHER_SPEED);
			if (input.isMovingRight()) renderer.moveLauncher( LAUNCHER_SPEED);
			
			constexpr float DT = 1.0f / TARGET_FPS;
			if (projectile && projectile->isActive()) {
				projectile->step(DT);
				Vec2 pos = projectile->getPosition();
				if (pos.y <= envirnoment.getWorldHeight(pos.x))
					projectile->deactivate();
			}
			renderer.update();
			renderer.clear();
			renderer.renderTerrain();
			renderer.renderLauncher();
			if (projectile)
				renderer.renderProjectile(*projectile);

			Uint64 elapsed = SDL_GetPerformanceCounter() - frameStart;
			if (elapsed < FRAME_TICKS) {
				Uint32 msRemaining = (Uint32)((FRAME_TICKS - elapsed) * 1000 / FREQ);
				if (msRemaining > 0)
					SDL_Delay(msRemaining);
			}

			Uint64 frameTime = SDL_GetPerformanceCounter() - frameStart;
			if (frameTime > 0)
				smoothFPS = smoothFPS * 0.95f + (float)FREQ / (float)frameTime * 0.05f;

			renderer.renderFPS(smoothFPS);
			renderer.present();
		}
	}
	catch (const std::exception& e) {
		std::cerr << e.what() << '\n';
		return 1;
	}

	return 0;
}
