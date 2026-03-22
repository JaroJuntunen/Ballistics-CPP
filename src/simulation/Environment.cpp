#define STB_PERLIN_IMPLEMENTATION
#include "stb_perlin.h"
#include "Environment.hpp"

static constexpr float	PERLIN_NOISE_FREQUENCY	= 0.05f;
static constexpr float	PERLIN_NOISE_SCALE	= 15.0f;

Environment::Environment()
{
	m_baseWindSpeed.x	= -30.0f;
	m_baseWindSpeed.y	= 0.0f;
	m_windGustSeverity	= 2.5f;
	m_gustFrequency		= 0.3f;
	m_time			= 0.0f;
}

//	World

float	Environment::getWorldHeight(float worldX)
{
	return ((stb_perlin_noise3(worldX * PERLIN_NOISE_FREQUENCY, 0.0, 0.0, 0, 0, 0) * PERLIN_NOISE_SCALE));
}

Vec2	Environment::getWindVelocity(float dt)
{
	m_time += dt;
	float	windGustOffsetX	= stb_perlin_noise3(m_time * m_gustFrequency,   0.0, 0.0, 0.0, 0.0, 0.0);
	float	windGustOffsetY	= stb_perlin_noise3(m_time * m_gustFrequency, 100.0, 0.0, 0.0, 0.0, 0.0);
	windGustOffsetX *= m_windGustSeverity;
	windGustOffsetY *= m_windGustSeverity;

	Vec2	windSpeed;
	windSpeed.x	= m_baseWindSpeed.x + windGustOffsetX;
	windSpeed.y	= m_baseWindSpeed.y + windGustOffsetY;
	return windSpeed;
}
