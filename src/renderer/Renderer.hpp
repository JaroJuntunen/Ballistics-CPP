#pragma once

#include <SDL3/SDL.h>
#include <string>
#include <iostream>
#include <stdexcept>
#include "util/math/Vec2.hpp"
#include "../simulation/Environment.hpp"
#include "../simulation/Projectile.hpp"

class Renderer
{
public:
	Renderer(const std::string& title, int width, int height);
	~Renderer();

	// Non-copyable, non-movable (owns SDL resources)
	Renderer(const Renderer&)            = delete;
	Renderer& operator=(const Renderer&) = delete;

	

	void clear();
	void present();

	float getTerrainHeightAsScreenY(uint16_t pixelX);
	void  updateScale(int width, int height);

	SDL_Renderer* get()              const { return m_renderer; }
	Vec2          getLauncherPosition() const { return m_launcherPosition; }
	Vec2          screenToWorld(float sx, float sy) const {
		return {
			m_camPosition.x + (sx - m_width  * 0.5f) / m_scale,
			m_camPosition.y - (sy - m_height * 0.5f) / m_scale
		};
	}

	void renderTerrain();
	void renderLauncher();
	void renderProjectile(const Projectile& p);
	void renderFPS(float fps);
	void moveLauncher(float dx);
	void moveCamera(float dx, float dy);
	void setCameraToLauncher();
	void zoom(float delta);
	void update();



	private:
	SDL_Window*		m_window;
	SDL_Renderer*	m_renderer;

	Vec2			m_launcherPosition;
	Vec2			m_camPosition;

	float			m_scale;
	int				m_width;
	int				m_height;

	Envirnoment		m_envirnoment;
	bool			m_movingToLauncher      = false;
	bool			m_cameraLockedToLauncher = true;
	
};
