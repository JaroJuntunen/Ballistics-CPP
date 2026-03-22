#define STB_PERLIN_IMPLEMENTATION
#include "stb_perlin.h"
#include "Environment.hpp"

static constexpr float PERLIN_NOISE_FREAGUANCY = 0.05f;
static constexpr float PERLIN_NOISE_SCALE = 15.0f;

Envirnoment::Envirnoment()
{
	m_baseWindSpeed.x = -30.0f;
	m_baseWindSpeed.y = 0.0f;
	m_windGustSeverity = 2.5f;
	m_gustFrequancy = 0.3f;
	m_time = 0.0f;
}

float Envirnoment::getWorldHeight(float worldX)
{
	return ((stb_perlin_noise3(worldX * PERLIN_NOISE_FREAGUANCY, 0.0, 0.0,0,0,0) * PERLIN_NOISE_SCALE));
}

Vec2 Envirnoment::getWindVelocity(float dt)
{
	m_time += dt;
	float windGustOfsetX = stb_perlin_noise3(m_time * m_gustFrequancy, 0.0,0.0,0.0,0.0,0.0);
	float windGustOfsetY = stb_perlin_noise3(m_time * m_gustFrequancy, 100.0,0.0,0.0,0.0,0.0);
	windGustOfsetX *= m_windGustSeverity;
	windGustOfsetY *= m_windGustSeverity;

	Vec2	windSpeed;
	windSpeed.x = m_baseWindSpeed.x + windGustOfsetX;
	windSpeed.y = m_baseWindSpeed.y + windGustOfsetY;
	return windSpeed;
}
