#include "Renderer.hpp"
#include <algorithm>
#include <cmath>
#include <cstdio>


Renderer::Renderer(const std::string& title, int width, int height)
	: m_window(nullptr), m_renderer(nullptr)
{
	if (!SDL_Init(SDL_INIT_VIDEO))
		throw std::runtime_error(std::string("SDL_Init failed: ") + SDL_GetError());

	m_window = SDL_CreateWindow(title.c_str(), width, height, SDL_WINDOW_RESIZABLE);
	if (!m_window) {
		SDL_Quit();
		throw std::runtime_error(std::string("SDL_CreateWindow failed: ") + SDL_GetError());
	}

	m_renderer = SDL_CreateRenderer(m_window, nullptr);
	if (!m_renderer) {
		SDL_DestroyWindow(m_window);
		SDL_Quit();
		throw std::runtime_error(std::string("SDL_CreateRenderer failed: ") + SDL_GetError());
	}
	m_width  = width;
	m_height = height;
	m_scale = (float)width / 100.0f;
	m_launcherPosition = Vec2(0.0f, 0.0f);
	m_camPosition = Vec2(0.0f, 0.0f);
}

Renderer::~Renderer()
{
	SDL_DestroyRenderer(m_renderer);
	SDL_DestroyWindow(m_window);
	SDL_Quit();
}


void Renderer::clear()
{
	SDL_SetRenderDrawColor(m_renderer, 15, 15, 15, 255);
	SDL_RenderClear(m_renderer);
}

void Renderer::present()
{
	SDL_RenderPresent(m_renderer);
}

void Renderer::moveCamera(float dx, float dy)
{
	m_movingToLauncher       = false;
	m_cameraLockedToLauncher = false;
	m_camPosition.x -= dx / m_scale;
	m_camPosition.y += dy / m_scale;
}

void Renderer::moveLauncher(float dx)
{
	m_launcherPosition.x += dx;
	m_launcherPosition.y = m_envirnoment.getWorldHeight(m_launcherPosition.x);

	if (m_cameraLockedToLauncher) {
		m_camPosition = m_launcherPosition;
		return;
	}

	// when unlocked, only chase if launcher leaves the central 5/6 of the screen
	float sx = (m_launcherPosition.x - m_camPosition.x) * m_scale + m_width  * 0.5f;
	float sy =  m_height * 0.5f - (m_launcherPosition.y - m_camPosition.y) * m_scale;

	constexpr float kEdgeBuffer = 20.0f;
	if (sx < -kEdgeBuffer || sx > m_width  + kEdgeBuffer ||
	    sy < -kEdgeBuffer || sy > m_height + kEdgeBuffer)
		setCameraToLauncher();
}

void Renderer::renderLauncher()
{
	float screenX = (m_launcherPosition.x - m_camPosition.x) * m_scale + m_width  * 0.5f;
	float screenY =  m_height * 0.5f - (m_launcherPosition.y - m_camPosition.y) * m_scale;

	SDL_FRect rect {
		screenX - m_scale * 0.5f,
		screenY - m_scale * 0.5f,
		m_scale,
		m_scale
	};

	SDL_SetRenderDrawColor(m_renderer, 220, 60, 40, 255);
	SDL_RenderFillRect(m_renderer, &rect);
}

void Renderer::setCameraToLauncher()
{
	m_movingToLauncher = true;
}

void Renderer::update()
{
	if (m_movingToLauncher) {

		
		float dx   = m_launcherPosition.x - m_camPosition.x;
		float dy   = m_launcherPosition.y - m_camPosition.y;
		float dist = std::sqrt(dx * dx + dy * dy);
		
		constexpr float kFraction = 0.04f;
		constexpr float kMinSpeed = 0.3f;
		constexpr float kMaxSpeed = 40.0f;
		
		if (dist < kMinSpeed) {
			m_camPosition            = m_launcherPosition;
			m_movingToLauncher       = false;
			m_cameraLockedToLauncher = true;
			return;
		}
		
		float speed = std::clamp(dist * kFraction, kMinSpeed, kMaxSpeed);
		float step  = speed / dist;
		m_camPosition.x += dx * step;
		m_camPosition.y += dy * step;
	}
}

void Renderer::zoom(float delta)
{
	m_scale *= (1.0f + delta * 0.1f);
	if (m_scale < 0.1f) m_scale = 0.1f;
}

void Renderer::updateScale(int width, int height)
{
	m_width  = width;
	m_height = height;
	m_scale = (float)width / 100.0f;
}

float Renderer::getTerrainHeightAsScreenY(uint16_t pixelX)
{
	float xCameraOfset = (((float)pixelX - (float)m_width * 0.5)) / m_scale;
	float worldX = m_camPosition.x + xCameraOfset;
	float worldY = m_envirnoment.getWorldHeight(worldX);
	
	float yCameraOfset = (worldY - m_camPosition.y);
	float pixelY = (yCameraOfset * m_scale);

	return m_height*0.5 - pixelY;
}

void Renderer::renderProjectile(const Projectile& p)
{
	const std::vector<Vec2>& path = p.getPathReference();
	bool drag = p.isDragEnabled();

	// dot colour: yellow with drag, red without
	if (drag)
		SDL_SetRenderDrawColor(m_renderer, 255, 220, 50, 255);
	else
		SDL_SetRenderDrawColor(m_renderer, 220, 50, 50, 255);

	Vec2  pos     = p.getPosition();
	float screenX = (pos.x - m_camPosition.x) * m_scale + m_width  * 0.5f;
	float screenY =  m_height * 0.5f - (pos.y - m_camPosition.y) * m_scale;
	constexpr float RADIUS = 3.0f;
	SDL_FRect rect { screenX - RADIUS, screenY - RADIUS, RADIUS * 2.0f, RADIUS * 2.0f };
	SDL_RenderFillRect(m_renderer, &rect);

	// path colour: white for drag, yellow for no-drag
	if (drag)
		SDL_SetRenderDrawColor(m_renderer, 255, 255, 255, 255);
	else
		SDL_SetRenderDrawColor(m_renderer, 255, 220, 50, 255);

	for (size_t i = 1; i < path.size(); i++)
	{
		float x1 = (path[i-1].x - m_camPosition.x) * m_scale + m_width  * 0.5f;
		float y1 =  m_height * 0.5f - (path[i-1].y - m_camPosition.y) * m_scale;
		float x2 = (path[i].x   - m_camPosition.x) * m_scale + m_width  * 0.5f;
		float y2 =  m_height * 0.5f - (path[i].y   - m_camPosition.y) * m_scale;
		SDL_RenderLine(m_renderer, x1, y1, x2, y2);
	}

	// connect last path point to current projectile position
	if (!path.empty())
	{
		float x1 = (path.back().x - m_camPosition.x) * m_scale + m_width  * 0.5f;
		float y1 =  m_height * 0.5f - (path.back().y - m_camPosition.y) * m_scale;
		SDL_RenderLine(m_renderer, x1, y1, screenX, screenY);
	}
}

void Renderer::renderFPS(float fps)
{


	char buf[16];
	std::snprintf(buf, sizeof(buf), "FPS: %.0f", fps);
	SDL_SetRenderDrawColor(m_renderer, 220, 220, 220, 255);
	SDL_RenderDebugText(m_renderer, 8.0f, 8.0f, buf);
}

void Renderer::renderTerrain()
{
	for (uint16_t x = 0; x < m_width; x++)
	{
		int terrainY = (int)getTerrainHeightAsScreenY(x);

		constexpr int grassThick = 4;
		constexpr int dirtThick  = grassThick * 4;

		int dirtBottom = std::min((int)m_height, terrainY + grassThick + dirtThick);
		for (int y = terrainY; y < dirtBottom; y++)
		{
			if (y < terrainY + grassThick)
			{
				SDL_SetRenderDrawColor(m_renderer,
					5,
					120,
					30,
					255);
			}
			else
			{
				SDL_SetRenderDrawColor(m_renderer,
					100,
					60,
					20,
					255);
			}
			SDL_RenderPoint(m_renderer, (float)x, (float)y);
		}
	}
}