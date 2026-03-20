#pragma once
#include "../util/math/Vec2.hpp"

class Projectile {
	public:
		Projectile(Vec2 startPos, Vec2 dir);
		Projectile(Vec2 startPos, Vec2 dir, float speed);
		~Projectile() = default;

		void	step(float dt);
		void	deactivate()			{ m_active = false; }
		Vec2	getPosition()	const	{ return m_position; }
		bool	isActive()		const	{ return m_active; }

	private:
		Vec2	m_position;
		Vec2	m_velocity;
		bool	m_active = true;
};