#pragma once

#include <SDL3/SDL.h>

class InputHandler
{
public:
	void handleEvent(const SDL_Event& event);

	bool shouldQuit()       const { return m_quit; }
	bool wasResized()       const { return m_resized; }
	int  newWidth()         const { return m_newWidth; }
	int  newHeight()        const { return m_newHeight; }
	void clearResized()           { m_resized = false; }

	bool  isDragging()      const { return m_dragging; }
	float dragDeltaX()      const { return m_dragDeltaX; }
	float dragDeltaY()      const { return m_dragDeltaY; }
	void  clearDrag()             { m_dragDeltaX = 0.0f; m_dragDeltaY = 0.0f; }

	bool	shouldMoveCameraToLauncher()	const { return m_setCameraToLauncher; }
	void	clearMoveCameraToLauncher()		{ m_setCameraToLauncher = false; }

	float	zoomDelta()		const { return m_zoomDelta; }
	void	clearZoom()			  { m_zoomDelta = 0.0f; }

private:
	bool	m_quit		= false;
	bool	m_resized	= false;
	int		m_newWidth	= 0;
	int		m_newHeight	= 0;

	bool	m_dragging		= false;
	float	m_lastMouseX	= 0.0f;
	float	m_lastMouseY	= 0.0f;
	float	m_dragDeltaX	= 0.0f;
	float	m_dragDeltaY	= 0.0f;

	bool	m_setCameraToLauncher = false;

	float	m_zoomDelta = 0.0f;
};
