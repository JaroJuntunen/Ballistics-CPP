#include "Projectile.hpp"

static constexpr float GRAVITY = 9.81f;



Projectile::Projectile(Vec2 startPos, Vec2 dir)
{
	m_velocity = dir.normalized() * 300; // direction * speed in ms/s
	m_position = startPos;
}

Projectile::Projectile(Vec2 startPos, Vec2 dir, float speed)
{
	m_velocity = dir.normalized() * speed;
	m_position = startPos;
}

void Projectile::step(float dt)
{
	m_velocity.y -= GRAVITY * dt;
	m_position += m_velocity * dt;
	m_path.push_back(m_position);
}