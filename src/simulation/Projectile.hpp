#pragma once
#include "../util/math/Vec2.hpp"
#include <vector>

class Projectile {
	public:
		Projectile(Vec2 startPos, Vec2 dir, bool dragEnabled);
		Projectile(Vec2 startPos, Vec2 dir, bool dragEnabled , float speed, float mass, float dragCoeficiency, float r);
		~Projectile() = default;

		void	step(float dt);
		void	stepNoDrag(float dt);
		void	deactivate()					{ m_active = false; }
		Vec2	getPosition()		const	{ return m_position; }
		bool	isActive()			const	{ return m_active; }
		bool	isDragEnabled()		const	{ return m_dragEnabled; }
		const std::vector<Vec2> &getPathReference()	const { return m_path;};

	private:
		Vec2				m_position;
		Vec2				m_velocity;
		float				m_mass;
		float				m_dragCoeficiency;
		float				m_crossSectionalArea;
		bool				m_active = true;
		bool				m_dragEnabled;
		std::vector<Vec2>	m_path;
		float				m_noDragOptimizationDt = 0; // Used for not storing all the step positions for non drag projectiles
};