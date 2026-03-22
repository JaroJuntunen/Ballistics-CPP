#include "InputHandler.hpp"

void	InputHandler::handleEvent(const SDL_Event& event)
{
	if (event.type == SDL_EVENT_QUIT)
		m_quit = true;

	if (event.type == SDL_EVENT_KEY_DOWN && event.key.scancode == SDL_SCANCODE_ESCAPE)
		m_quit = true;

	if (event.type == SDL_EVENT_WINDOW_RESIZED) {
		m_resized	= true;
		m_newWidth	= event.window.data1;
		m_newHeight	= event.window.data2;
	}
	if (event.type == SDL_EVENT_MOUSE_BUTTON_DOWN) {
		if (event.button.button == SDL_BUTTON_RIGHT) {
			m_dragging		= true;
			m_lastMouseX	= event.button.x;
			m_lastMouseY	= event.button.y;
		}
		if (event.button.button == SDL_BUTTON_MIDDLE) {
			m_setCameraToLauncher = true;
		}
	}
	if (event.type == SDL_EVENT_MOUSE_BUTTON_UP && event.button.button == SDL_BUTTON_RIGHT)
		m_dragging = false;

	if (event.type == SDL_EVENT_MOUSE_MOTION) {
		m_mouseX = event.motion.x;
		m_mouseY = event.motion.y;
	}

	if (event.type == SDL_EVENT_MOUSE_MOTION && m_dragging) {
		m_dragDeltaX	+= event.motion.x - m_lastMouseX;
		m_dragDeltaY	+= event.motion.y - m_lastMouseY;
		m_lastMouseX	= event.motion.x;
		m_lastMouseY	= event.motion.y;
	}

	if (event.type == SDL_EVENT_MOUSE_WHEEL)
		m_zoomDelta += event.wheel.y;

	if (event.type == SDL_EVENT_KEY_DOWN) {
		if (event.key.scancode == SDL_SCANCODE_KP_PLUS)
			m_zoomDelta += 1.0f;
		if (event.key.scancode == SDL_SCANCODE_KP_MINUS)
			m_zoomDelta -= 1.0f;
		if (event.key.scancode == SDL_SCANCODE_KP_ENTER)
			m_setCameraToLauncher = true;

		if (event.key.scancode == SDL_SCANCODE_A)
			m_moveLeft = true;
		if (event.key.scancode == SDL_SCANCODE_D)
			m_moveRight = true;
		if (event.key.scancode == SDL_SCANCODE_SPACE && !(event.key.mod & SDL_KMOD_SHIFT))
			m_shootProjectile = true;
		if (event.key.scancode == SDL_SCANCODE_SPACE && (event.key.mod & SDL_KMOD_SHIFT))
			m_shootProjectileNoDrag = true;
		if (event.key.scancode == SDL_SCANCODE_SPACE && (event.key.mod & SDL_KMOD_CTRL)) {
			m_shootProjectile		= true;
			m_shootProjectileNoDrag	= true;
		}
		if (event.key.scancode == SDL_SCANCODE_C)
			m_clearProjectiles = true;
		if (event.key.scancode == SDL_SCANCODE_L)
			m_toggleLockDirection = true;
	}

	if (event.type == SDL_EVENT_KEY_UP) {
		if (event.key.scancode == SDL_SCANCODE_A)
			m_moveLeft = false;
		if (event.key.scancode == SDL_SCANCODE_D)
			m_moveRight = false;
	}
}
