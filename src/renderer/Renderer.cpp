#include "Renderer.hpp"
#include <algorithm>
#include <cmath>


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
	m_movingToLauncher = false;
	m_camPosition.x -= dx / m_scale;
	m_camPosition.y += dy / m_scale;
}

void Renderer::setCameraToLauncher()
{
	m_movingToLauncher = true;
}

void Renderer::update()
{
	if (!m_movingToLauncher)
		return;

	float dx   = m_launcherPosition.x - m_camPosition.x;
	float dy   = m_launcherPosition.y - m_camPosition.y;
	float dist = std::sqrt(dx * dx + dy * dy);

	if (dist < 0.5f) {
		m_camPosition      = m_launcherPosition;
		m_movingToLauncher = false;
		return;
	}

	constexpr float kFraction = 0.04f;   // fraction of distance per frame
	constexpr float kMaxSpeed = 40.0f;   // m/frame cap

	float speed = std::min(dist * kFraction, kMaxSpeed);
	float step  = speed / dist;
	m_camPosition.x += dx * step;
	m_camPosition.y += dy * step;
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

void Renderer::renderTerrain()
{
	for (uint16_t x = 0; x < m_width; x++)
	{
		int terrainY = (int)getTerrainHeightAsScreenY(x);

		float   worldX = m_camPosition.x + ((float)x - m_width * 0.5f) / m_scale;

		// grass thickness — two world-space octaves for more waviness
		auto sampleThick = [](float wx, float bs) {
			float   wn   = wx / bs;
			int32_t b0   = (int32_t)std::floor(wn);
			float   t    = wn - (float)b0;
			float   v0   = (float)(((uint32_t)b0       * 1234567891u) >> 29);
			float   v1   = (float)((((uint32_t)b0 + 1u) * 1234567891u) >> 29);
			return v0 + t * (v1 - v0);
		};
		float   grassThickWorld = 0.25f + sampleThick(worldX, 5.0f) * 0.07f
		                                + sampleThick(worldX, 1.2f) * 0.04f
		                                + sampleThick(worldX, 0.4f) * 0.02f;
		int     grassThick      = std::max(1, (int)(grassThickWorld * m_scale));

		// grass column colour — world-space hash
		uint8_t colNoise = ((uint32_t)(int32_t)std::floor(worldX) * 2654435761u) >> 28;

		for (int y = terrainY; y < m_height; y++)
		{
			if (y < terrainY + grassThick)
			{
				SDL_SetRenderDrawColor(m_renderer,
					50  + colNoise,
					120 + colNoise,
					30,
					255);
			}
			else
			{
				// dirt noise — world-space x and y
				int32_t wpy = (int32_t)std::floor(m_camPosition.y + (m_height * 0.5f - (float)y) / m_scale);
				int32_t wpx = (int32_t)std::floor(worldX);
				uint32_t dh = (uint32_t)wpx * 2246822519u ^ (uint32_t)wpy * 3266489917u;
				dh ^= dh >> 17;  dh *= 2246822519u;  dh ^= dh >> 13;
				uint8_t dirtNoise = dh >> 29;
				SDL_SetRenderDrawColor(m_renderer,
					100 + dirtNoise,
					60  + dirtNoise,
					20  + dirtNoise,
					255);
			}
			SDL_RenderPoint(m_renderer, (float)x, (float)y);
		}
	}
}