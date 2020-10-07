#include "Window.h"


namespace aeth::window
{
	WindowID mainWindow;

	std::unordered_map<uint32_t, Window*> Window::instanceMap;

	uint32_t Window::StartWindow(const char* windowTitle,
		uint32_t windowXPos,
		uint32_t windowYPos,
		uint32_t windowWidth,
		uint32_t windowHeight,
		uint32_t windowFlags,
		void(*closedCallback)())
	{
		Window* window = new Window(windowTitle, windowXPos, windowYPos, windowWidth, windowHeight, windowFlags, closedCallback);
		instanceMap[window->GetID()] = window;
		return window->GetID();
	}

	void Window::StopWindow(uint32_t _id)
	{
		if (!InstanceExists(_id))
		{
			logging::Logger::Log("Attempted to shutdown non-existant window (ID not found).", logging::Logger::WARNING);
			return;
		}
		delete instanceMap[_id];
		instanceMap.erase(_id);
	}


	Window::Window(const char* windowTitle,
		uint32_t windowXPos,
		uint32_t windowYPos,
		uint32_t windowWidth,
		uint32_t windowHeight,
		uint32_t windowFlags,
		void(*windowClosedCallback)())
	{
		title = windowTitle;
		width = windowWidth;
		height = windowHeight;
		closedCallback = windowClosedCallback;
		window = SDL_CreateWindow(title.data(), windowXPos, windowYPos, width, height, windowFlags);
	}

	Window::~Window()
	{
		SDL_DestroyWindow(window);
	}


	void Window::UpdateEvents()
	{
		SDL_PumpEvents();

		SDL_Event e;
		int currentEvents = 0;
		const int maxEvents = 50;
		while (SDL_PollEvent(&e) && currentEvents < maxEvents)//poll events
		{
			currentEvents++;

			if (e.type == SDL_WINDOWEVENT)
			{
				if (!InstanceExists(e.window.windowID))
					continue;
				Window* relevantWindow = GetInstance(e.window.windowID);
				switch (e.window.event)
				{
				case SDL_WINDOWEVENT_RESIZED:
					relevantWindow->width = e.window.data1;
					relevantWindow->height = e.window.data2;
					break;

				case SDL_WINDOWEVENT_MINIMIZED:
					relevantWindow->minimized = true;
					break;

				case SDL_WINDOWEVENT_RESTORED:
					relevantWindow->minimized = false;
					break;

				case SDL_WINDOWEVENT_CLOSE:
					relevantWindow->closedCallback();
					break;

				default:
					break;
				}
			}
		}
	}
}
