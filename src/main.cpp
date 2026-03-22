#include <SDL3/SDL.h>
#include <iostream>
#include <memory>
#include <cmath>
#include "renderer/Renderer.hpp"
#include "input/InputHandler.hpp"
#include "simulation/Projectile.hpp"

static constexpr int	WINDOW_WIDTH  = 1280;
static constexpr int	WINDOW_HEIGHT = 720;
static constexpr float	DT            = 1.0f / 60.0f;
static constexpr float	PI            = 3.14159265358979f;

//	Input handlers

static void	onResize(InputHandler& input, Renderer& renderer)
{
	renderer.updateScale(input.newWidth(), input.newHeight());
	input.clearResized();
}

static void	onCameraDrag(InputHandler& input, Renderer& renderer)
{
	renderer.moveCamera(input.dragDeltaX(), input.dragDeltaY());
	input.clearDrag();
}

static void	onZoom(InputHandler& input, Renderer& renderer)
{
	renderer.zoom(input.zoomDelta());
	input.clearZoom();
}

static void	onCameraFocus(InputHandler& input, Renderer& renderer)
{
	renderer.setCameraToLauncher();
	input.clearMoveCameraToLauncher();
}

static Vec2	updateMouseAim(InputHandler& input, Renderer& renderer,
							ProjectileSettings& projSettings)
{
	Vec2	launchPos  = renderer.getLauncherPosition();
	Vec2	mouseWorld = renderer.screenToWorld(input.mouseX(), input.mouseY());
	Vec2	mouseDir   = mouseWorld - launchPos;
	projSettings.currentAngle = std::atan2(mouseDir.y, mouseDir.x) * 180.0f / PI;
	return mouseDir;
}

static void	onToggleLockDirection(InputHandler& input, ProjectileSettings& projSettings)
{
	projSettings.directionLocked = !projSettings.directionLocked;
	if (projSettings.directionLocked)
		projSettings.lockedAngle = projSettings.currentAngle;
	input.clearToggleLockDirection();
}

static void	onShoot(InputHandler& input, Renderer& renderer,
					ProjectileSettings& projSettings,
					std::vector<std::unique_ptr<Projectile>>& projectiles,
					Vec2 mouseDir)
{
	Vec2	fireDir;
	if (projSettings.directionLocked) {
		float	rad = projSettings.lockedAngle * PI / 180.0f;
		fireDir = { std::cos(rad), std::sin(rad) };
	} else {
		fireDir = mouseDir;
	}
	Vec2	launchPos = renderer.getLauncherPosition();
	if (!input.shouldShootProjectilesNoDrag()) {
		projectiles.push_back(std::make_unique<Projectile>(launchPos, fireDir, true,  projSettings.speed, projSettings.mass / 1000.0f, projSettings.dragCoefficient, (projSettings.diameter / 1000.0f) * 0.5f));
		input.clearShootProjectiles();
	} else {
		projectiles.push_back(std::make_unique<Projectile>(launchPos, fireDir, false, projSettings.speed, projSettings.mass / 1000.0f, projSettings.dragCoefficient, (projSettings.diameter / 1000.0f) * 0.5f));
		input.clearShootProjectilesNoDrag();
	}
}

static void	onClearProjectile(InputHandler& input,
								std::vector<std::unique_ptr<Projectile>>& projectiles)
{
	if (projectiles.size() > 0)
		projectiles.erase(projectiles.begin());
	input.clearClearProjectiles();
}

static void	onLauncherMove(InputHandler& input, Renderer& renderer)
{
	constexpr float	LAUNCHER_SPEED = 0.75f;
	if (input.isMovingLeft())  renderer.moveLauncher(-LAUNCHER_SPEED);
	if (input.isMovingRight()) renderer.moveLauncher( LAUNCHER_SPEED);
}

//	Main input dispatch

static void	handleInput(InputHandler& input, Renderer& renderer,
						ProjectileSettings& projSettings,
						std::vector<std::unique_ptr<Projectile>>& projectiles)
{
	SDL_Event	event;
	while (SDL_PollEvent(&event)) {
		ImGui_ImplSDL3_ProcessEvent(&event);
		input.handleEvent(event);
	}

	if (input.wasResized())
		onResize(input, renderer);												// window resized, update render dimensions
	if (input.isDragging())
		onCameraDrag(input, renderer);											// right-click drag, pan the camera
	if (input.zoomDelta() != 0.0f)
		onZoom(input, renderer);												// scroll wheel, zoom in/out
	if (input.shouldMoveCameraToLauncher())
		onCameraFocus(input, renderer);											// middle-click/numpad enter, snap camera to launcher

	Vec2	mouseDir = updateMouseAim(input, renderer, projSettings);			// update current aim angle from mouse position every frame

	if (input.shouldToggleLockDirection())
		onToggleLockDirection(input, projSettings);								// L key, lock/unlock firing direction
	if (input.shouldShootProjectiles() || input.shouldShootProjectilesNoDrag())
		onShoot(input, renderer, projSettings, projectiles, mouseDir);			// left/right click, fire projectile
	if (input.shouldClearProjectile()) 
		onClearProjectile(input, projectiles);									// backspace, remove oldest projectile
	onLauncherMove(input, renderer);											// A/D keys, move launcher left or right
}

//	Simulation

static void	stepSimulation(Environment& env,
							std::vector<std::unique_ptr<Projectile>>& projectiles)
{
	Vec2	windVelocity = env.getWindVelocity(DT);
	for (std::unique_ptr<Projectile>& projectile : projectiles)
	{
		if (projectile->isActive())
			projectile->step(DT, windVelocity);
	}
}

//	Rendering

static void	renderFrame(Renderer& renderer, Environment& env,
						ProjectileSettings& projSettings,
						std::vector<std::unique_ptr<Projectile>>& projectiles)
{
	renderer.clear();
	renderer.renderTerrain();
	renderer.renderLauncher();
	for (std::unique_ptr<Projectile>& projectile : projectiles)
	{
		if (projectile)
			renderer.renderProjectile(*projectile);
	}
	renderer.renderFPS();
	bool	prevLocked = projSettings.directionLocked;
	renderer.renderUI(env, projSettings);
	if (projSettings.directionLocked && !prevLocked)
		projSettings.lockedAngle = projSettings.currentAngle;
	renderer.present();
}

int	main()
{
	try {
		Renderer		renderer("Ballistics", WINDOW_WIDTH, WINDOW_HEIGHT);
		InputHandler		input;
		Environment		env;
		ProjectileSettings	projSettings;

		std::vector<std::unique_ptr<Projectile>>	projectiles;

		while (!input.shouldQuit()) {
			renderer.startFrame();

			handleInput(input, renderer, projSettings, projectiles);
			stepSimulation(env, projectiles);
			renderer.update();
			renderer.capFrame();

			renderFrame(renderer, env, projSettings, projectiles);
		}
	}
	catch (const std::exception& e) {
		std::cerr << e.what() << '\n';
		return 1;
	}

	return 0;
}
