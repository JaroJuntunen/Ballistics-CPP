#pragma once
#include "../util/math/Vec2.hpp"
#include "../simulation/Environment.hpp"
#include <vector>

struct	ProjectileSettings {
	//	Ballistics
	float	speed			= 710.0f;	// m/s
	float	mass			= 8.0f;		// g
	float	dragCoefficient	= 0.3f;
	float	diameter		= 7.62f;	// mm

	//	Direction
	bool	directionLocked	= false;
	float	lockedAngle		= 45.0f;	// degrees, used when locked
	float	currentAngle	= 45.0f;	// degrees, updated each frame from mouse
};

class	Projectile {
	public:
		Projectile(Vec2 startPos, Vec2 dir, bool dragEnabled);
		Projectile(Vec2 startPos, Vec2 dir, bool dragEnabled,
					float speed, float mass, float dragCoefficient, float r);
		~Projectile() = default;

		//	Simulation
		void	step(float dt, Vec2 windVelocity);
		void	deactivate()	{ m_active = false; }

		//	Getters
		Vec2				getPosition()				const	{ return m_position; }
		bool				isActive()					const	{ return m_active; }
		bool				isDragEnabled()				const	{ return m_dragEnabled; }
		const std::vector<Vec2>		&getPathReference()	const	{ return m_path; }

	private:
		//	Physics state
		Vec2	m_position;
		Vec2	m_velocity;
		float	m_mass;
		float	m_dragCoefficient;
		float	m_crossSectionalArea;
		bool	m_dragEnabled;

		//	Path
		std::vector<Vec2>	m_path;
		float				m_noDragOptimizationDt	= 0; // Used for not storing all the step positions for non drag projectiles
		bool				m_active		= true;

		//	Environment
		Environment	m_env;
};
