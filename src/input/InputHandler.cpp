#include "InputHandler.hpp"

void InputHandler::handleEvent(const SDL_Event& event)
{
	if (event.type == SDL_EVENT_QUIT)
		m_quit = true;

	if (event.type == SDL_EVENT_KEY_DOWN && event.key.scancode == SDL_SCANCODE_ESCAPE)
		m_quit = true;

	if (event.type == SDL_EVENT_WINDOW_RESIZED) {
		m_resized    = true;
		m_newWidth   = event.window.data1;
		m_newHeight  = event.window.data2;
	}
	if (event.type == SDL_EVENT_MOUSE_BUTTON_DOWN && event.button.button == SDL_BUTTON_LEFT) {
		m_dragging   = true;
		m_lastMouseX = event.button.x;
		m_lastMouseY = event.button.y;
	}
	if (event.type == SDL_EVENT_MOUSE_BUTTON_DOWN && event.button.button == SDL_BUTTON_RIGHT) {
		m_setCameraToLauncher = true;
	}

	if (event.type == SDL_EVENT_MOUSE_BUTTON_UP && event.button.button == SDL_BUTTON_LEFT)
		m_dragging = false;

	if (event.type == SDL_EVENT_MOUSE_MOTION && m_dragging) {
		m_dragDeltaX += event.motion.x - m_lastMouseX;
		m_dragDeltaY += event.motion.y - m_lastMouseY;
		m_lastMouseX  = event.motion.x;
		m_lastMouseY  = event.motion.y;
	}

	if (event.type == SDL_EVENT_MOUSE_WHEEL)
		m_zoomDelta += event.wheel.y;

	if (event.type == SDL_EVENT_KEY_DOWN) {
		if (event.key.scancode == SDL_SCANCODE_KP_PLUS)
			m_zoomDelta += 1.0f;
		if (event.key.scancode == SDL_SCANCODE_KP_MINUS)
			m_zoomDelta -= 1.0f;
	}
}
