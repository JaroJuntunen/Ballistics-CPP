#pragma once

#include <SDL3/SDL.h>

class	InputHandler
{
public:
	void	handleEvent(const SDL_Event& event);

	//	Window
	bool	shouldQuit()	const	{ return m_quit; }
	bool	wasResized()	const	{ return m_resized; }
	int		newWidth()		const	{ return m_newWidth; }
	int		newHeight()		const	{ return m_newHeight; }
	void	clearResized()			{ m_resized = false; }

	//	Camera
	bool	isDragging()				const	{ return m_dragging; }
	float	dragDeltaX()				const	{ return m_dragDeltaX; }
	float	dragDeltaY()				const	{ return m_dragDeltaY; }
	void	clearDrag()						{ m_dragDeltaX = 0.0f; m_dragDeltaY = 0.0f; }
	bool	shouldMoveCameraToLauncher()	const	{ return m_setCameraToLauncher; }
	void	clearMoveCameraToLauncher()			{ m_setCameraToLauncher = false; }

	//	Zoom
	float	zoomDelta()	const	{ return m_zoomDelta; }
	void	clearZoom()			{ m_zoomDelta = 0.0f; }

	//	Launcher movement
	bool	isMovingLeft()	const	{ return m_moveLeft; }
	bool	isMovingRight()	const	{ return m_moveRight; }

	//	Shooting
	bool	shouldShootProjectiles()		const	{ return m_shootProjectile; }
	bool	shouldShootProjectilesNoDrag()	const	{ return m_shootProjectileNoDrag; }
	void	clearShootProjectiles()					{ m_shootProjectile = false; }
	void	clearShootProjectilesNoDrag()			{ m_shootProjectileNoDrag = false; }
	bool	shouldClearProjectile()					{ return m_clearProjectiles; }
	void	clearClearProjectiles()					{ m_clearProjectiles = false; }
	bool	shouldToggleLockDirection()				{ return m_toggleLockDirection; }
	void	clearToggleLockDirection()				{ m_toggleLockDirection = false; }

	//	Mouse
	float	mouseX()	const	{ return m_mouseX; }
	float	mouseY()	const	{ return m_mouseY; }

private:
	//	Window
	bool	m_quit		= false;
	bool	m_resized	= false;
	int		m_newWidth	= 0;
	int		m_newHeight	= 0;

	//	Camera
	bool	m_dragging				= false;
	float	m_lastMouseX			= 0.0f;
	float	m_lastMouseY			= 0.0f;
	float	m_dragDeltaX			= 0.0f;
	float	m_dragDeltaY			= 0.0f;
	bool	m_setCameraToLauncher	= false;

	//	Zoom
	float	m_zoomDelta	= 0.0f;

	//	Launcher movement
	bool	m_moveLeft	= false;
	bool	m_moveRight	= false;

	//	Shooting
	bool	m_shootProjectile		= false;
	bool	m_shootProjectileNoDrag	= false;
	bool	m_clearProjectiles		= false;
	bool	m_toggleLockDirection	= false;

	//	Mouse
	float	m_mouseX	= 0.0f;
	float	m_mouseY	= 0.0f;
};
