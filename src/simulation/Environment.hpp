#pragma once
#include <cstdint>
#include "../util/math/Vec2.hpp"

class Envirnoment {
	public:
		Envirnoment();
		~Envirnoment() = default;

		float	getWorldHeight(float worldX);
		Vec2	getWindVelocity(float dt);

	private:
		Vec2	m_baseWindSpeed;
		float	m_windGustSeverity;
		float	m_gustFrequancy;
		float	m_time;
};