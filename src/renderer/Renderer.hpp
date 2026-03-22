#pragma once

#include <SDL3/SDL.h>
#include <string>
#include <iostream>
#include <stdexcept>
#include "util/math/Vec2.hpp"
#include "../simulation/Environment.hpp"
#include "../simulation/Projectile.hpp"
#include "imgui.h"
#include "imgui_impl_sdl3.h"
#include "imgui_impl_sdlrenderer3.h"

class	Renderer
{
public:
	Renderer(const std::string& title, int width, int height);
	~Renderer();

	// Non-copyable, non-movable (owns SDL resources)
	Renderer(const Renderer&)				= delete;
	Renderer&	operator=(const Renderer&)	= delete;

	//	Core
	void	clear();
	void	present();
	void	zoom(float delta);
	void	update();
	void	updateScale(int width, int height);
	void	startFrame();
	void	capFrame();

	//	Getters
	SDL_Renderer*	get()					const	{ return m_renderer; }
	SDL_Window*	getWindow()					const	{ return m_window; }
	Vec2		getLauncherPosition()		const	{ return m_launcherPosition; }
	float		getTerrainHeightAsScreenY(uint16_t pixelX);
	Vec2		screenToWorld(float sx, float sy)	const {
		return {
			m_camPosition.x + (sx - m_width  * 0.5f) / m_scale,
			m_camPosition.y - (sy - m_height * 0.5f) / m_scale
		};
	}

	//	Renderers
	void	renderTerrain();
	void	renderLauncher();
	void	renderProjectile(const Projectile& p);
	void	renderFPS();
	void	renderUI(Environment& env, ProjectileSettings& proj);

	//	Movers
	void	moveLauncher(float dx);
	void	moveCamera(float dx, float dy);

	//	Setters
	void	setCameraToLauncher();

private:
	//	SDL handles
	SDL_Window*	m_window;
	SDL_Renderer*	m_renderer;

	//	Camera
	Vec2	m_launcherPosition;
	Vec2	m_camPosition;
	float	m_scale;
	int		m_width;
	int		m_height;

	//	State
	Environment	m_environment;
	bool		m_movingToLauncher			= false;
	bool		m_cameraLockedToLauncher	= true;

	//	Frame timing
	static constexpr int	TARGET_FPS	= 60;
	Uint64	m_freq;
	Uint64	m_frameTicks;
	Uint64	m_frameStart;
	float	m_smoothFPS;
};
