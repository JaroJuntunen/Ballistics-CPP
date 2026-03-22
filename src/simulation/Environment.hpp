#pragma once
#include <cstdint>
#include "../util/math/Vec2.hpp"

class	Environment {
	public:
		Environment();
		~Environment() = default;

		//	World
		float	getWorldHeight(float worldX);
		Vec2	getWindVelocity(float dt);

		//	Getters
		Vec2	getBaseWindSpeed()			const	{ return m_baseWindSpeed; }
		float	getWindGustSeverity()		const	{ return m_windGustSeverity; }
		float	getGustFrequency()			const	{ return m_gustFrequency; }

		//	Setters
		void	setBaseWindSpeed(Vec2 wind)				{ m_baseWindSpeed = wind; }
		void	setWindGustSeverity(float severity)		{ m_windGustSeverity = severity; }
		void	setGustFrequency(float frequency)		{ m_gustFrequency = frequency; }

	private:
		//	Wind
		Vec2	m_baseWindSpeed;
		float	m_windGustSeverity;
		float	m_gustFrequency;

		//	State
		float	m_time;
};
