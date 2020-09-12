#pragma once

#include <string>
#include <cstdint>
#include <unordered_map>

#include <SDL2/SDL.h>

#include "Src/Logging/Logger.h"


namespace Aeth::Window
{
	typedef uint32_t WindowID;

	extern WindowID mainWindow; // Main window that is created internally rather than by the user


	class Window
	{
	private:
		static std::unordered_map<WindowID, Window*> instanceMap;


	public:
		/// <summary>
		/// Get a window instance via its window ID. If the ID is invalid, raises an exception.
		/// </summary>
		/// <param name="_id">Window ID of the desired instance.</param>
		/// <returns>Instance of the given ID.</returns>
		static Window* GetInstance(WindowID _id) {
			if (!InstanceExists(_id))
				Logging::Logger::Log("Invalid Window ID.", Logging::Logger::EXCEPTION);
			return instanceMap[_id];
		}

		/// <summary>
		/// Check if a window instance exists.
		/// </summary>
		/// <returns>True if the instance exists, False otherwise.</returns>
		static bool InstanceExists(WindowID _id)
		{
			if (instanceMap.find(_id) == instanceMap.end())
				return false;
			return true;
		}

		/// <summary>
		/// Update the events. This should be called in a loop.
		/// </summary>
		static void UpdateEvents();

		/// <summary>
		/// Starts a new window with the given configuration.
		/// </summary>
		/// <param name="closedCallback">Optional callback to be called when the window is closed</param>
		/// <returns>ID of the new window</returns>
		static uint32_t StartWindow(const char* windowTitle = "",
			uint32_t windowXPos = 0,
			uint32_t windowYPos = 0,
			uint32_t windowWidth = 0,
			uint32_t windowHeight = 0,
			uint32_t windowFlags = 0,
			void(*closedCallback)() = {});

		/// <summary>
		/// Stops a window with a given ID. Raises a warning if there is no window with the ID.
		/// </summary>
		/// <param name="_id">ID of the window to stop.</param>
		static void StopWindow(uint32_t _id);

		/// <summary>
		/// Starts SDL. Needs to be called before creating any windows.
		/// </summary>
		static void StartSDL()
		{
			if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS) != 0)
				Logging::Logger::Log("SDL Error: " + std::string(SDL_GetError()), Logging::Logger::EXCEPTION);
		}

		/// <summary>
		/// Stops SDL. All windows should be destroyed before calling this.
		/// </summary>
		static void StopSDL()
		{
			SDL_Quit();
		}


	private:
		SDL_Window* window = nullptr;
		std::string title = "";
		uint32_t width = 0;
		uint32_t height = 0;
		bool minimized = false;

		void(*closedCallback)();


		Window(const char* windowTitle = "",
			uint32_t windowXPos = 0,
			uint32_t windowYPos = 0,
			uint32_t windowWidth = 0,
			uint32_t windowHeight = 0,
			uint32_t windowFlags = 0,
			void(*windowClosedCallback)() = {});
		~Window();


	public:
		SDL_Window* getWindow() { return window; }
		std::string getTitle() { return title; }
		unsigned int getWidth() { return width; }
		unsigned int getHeight() { return height; }

		operator SDL_Window*() const { return window; }

		void setTitle(std::string _title) { title = _title; SDL_SetWindowTitle(window, title.data()); }
		void setWidth(unsigned int _width) { width = _width; SDL_SetWindowSize(window, width, height); }
		void setHeight(unsigned int _height) { height = _height; SDL_SetWindowSize(window, width, height); }
		void setWidthAndHeight(unsigned int _width, unsigned int _height) { width = _width; height = _height; SDL_SetWindowSize(window, width, height); }

		WindowID GetID() { return SDL_GetWindowID(window); }
		bool IsMinimized() { return minimized; }
	};
}
