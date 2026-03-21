#include "Projectile.hpp"

static constexpr float GRAVITY = 9.81f;
static constexpr float AIRDENCITY = 1.225f;
static constexpr float PI = 3.14159265358979f;


Projectile::Projectile(Vec2 startPos, Vec2 dir, bool dragEnabled)
{
	// Default values are from 7.62mm bullet
	m_velocity = dir.normalized() * 710; // direction * speed in ms/s
	m_position = startPos;
	m_mass = 0.008f;
	m_dragCoeficiency = 0.3f;
	m_crossSectionalArea = PI * (0.00381f * 0.00381f);
	m_dragEnabled = dragEnabled;
}

Projectile::Projectile(Vec2 startPos, Vec2 dir, bool dragEnabled , float speed, float mass, float dragCoeficiency, float r)
{
	m_velocity = dir.normalized() * speed;
	m_position = startPos;
	m_mass = mass;
	m_dragCoeficiency = dragCoeficiency;
	m_crossSectionalArea = PI * (r * r);
	m_dragEnabled = dragEnabled;
}


void Projectile::step(float dt)
{
	if(m_dragEnabled){

		float velocity = std::sqrt((m_velocity.x * m_velocity.x) + (m_velocity.y * m_velocity.y));
		float dragForce = 0.5f * m_dragCoeficiency * AIRDENCITY * m_crossSectionalArea * (velocity * velocity);
		float deceleration = dragForce / m_mass; 
		
		m_velocity.x -= deceleration * (m_velocity.x / velocity) * dt;
		m_velocity.y -= (GRAVITY + (deceleration * (m_velocity.y / velocity))) * dt;
	}
	else {
		m_velocity.y -= GRAVITY * dt;
		m_noDragOptimizationDt += dt;
	}
	m_position += m_velocity * dt;
	
	if (m_dragEnabled) {
		m_path.push_back(m_position);
	}
	else if (m_noDragOptimizationDt > 0.5f) {
		m_noDragOptimizationDt = 0.0f;
		m_path.push_back(m_position);
	}
}

