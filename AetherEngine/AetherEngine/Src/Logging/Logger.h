#pragma once
#include "Src/Common/CrossPlatform.h"

#include <unordered_map>
#include <mutex>
#include <string>
#include <sstream>
#include <thread>
#include <condition_variable>
#include <atomic>
#include <unordered_set>
#include <fstream>
#include <sstream>

#ifdef AETH_WINDOWS
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#endif // AETH_WINDOWS

#include "Src/Utils/StlExtensions/SharedQueue.h"

#undef ERROR


namespace aeth::Logging
{
	/// <summary>
	/// Singleton logger class, inteded to replace print functions like std::cout and printf. Adds extra features ontop of regular console logging including logging to a log file.
	/// </summary>
	class Logger
	{
	public: // public types
		enum LoggerEndltag
		{
			endl // Used to end the stream operator by operating Logger::endl into it like so:   Logger::Lock() << "Thing to print" << Logger::endl;
		};

		enum LoggerTags
		{
			OTHER = 0,
			INFO,
			WARNING,
			ERROR,
			EXCEPTION,
			DEBUG
		};

#ifdef AETH_WINDOWS
		enum LoggerColors
		{
			COLOR_BLACK = 0,
			COLOR_BLUE = 1,
			COLOR_GREEN = 2,
			COLOR_CYAN = 3,
			COLOR_RED = 4,
			COLOR_MAGENTA = 5,
			COLOR_YELLOW = 6,
			COLOR_WHITE = 7,
			COLOR_GRAY = 8,
			COLOR_BRIGHT_BLUE = 9,
			COLOR_BRIGHT_GREEN = 10,
			COLOR_BRIGHT_CYAN = 11,
			COLOR_BRIGHT_RED = 12,
			COLOR_BRIGHT_MAGENTA = 13,
			COLOR_BRIGHT_YELLOW = 14,
			COLOR_BRIGHT_WHITE = 15
		};
#elif defined(AETH_UNIX)
		enum LoggerColors
		{
			COLOR_BLACK = 30,
			COLOR_BLUE = 34,
			COLOR_GREEN = 32,
			COLOR_CYAN = 36,
			COLOR_RED = 31,
			COLOR_MAGENTA = 35,
			COLOR_YELLOW = 33,
			COLOR_WHITE = 37,
			COLOR_GRAY = 90,
			COLOR_BRIGHT_BLUE = 94,
			COLOR_BRIGHT_GREEN = 92,
			COLOR_BRIGHT_CYAN = 96,
			COLOR_BRIGHT_RED = 91,
			COLOR_BRIGHT_MAGENTA = 95,
			COLOR_BRIGHT_YELLOW = 93,
			COLOR_BRIGHT_WHITE = 97
		};
#endif // AETH_WINDOWS || AETH_LINUX

	private: // private types
		struct TagData
		{
			std::string name = "";
			uint8_t severity = 0;
			uint8_t foreground = 0;
			uint8_t background = 0;
			std::string file = "";
		};

		struct MessageData
		{
			std::string message = "";
			uint32_t tag = 0;
			std::thread::id thread;
		};

		struct WriteData
		{
			std::string message = "";
			std::string file = "";
		};


	private: // Private members
		static Logger* instance;

		static std::thread loopThread;
		static std::atomic<bool> running;

		static std::string mainFile;
		static uint8_t severityLevel;

		static std::unordered_map<std::thread::id, std::string> threadNames;
		static std::unordered_map<uint32_t, TagData> tagData;

		static Utils::StlExtensions::SharedQueue<MessageData> messageQueue;
		static std::unordered_set<std::string> filesMade;
		static std::deque<WriteData> writeQueue;

		static std::mutex mapMutex;
		static std::mutex streamMutex;

		static std::stringstream inputStream;
		static uint32_t streamTag;


	public: // Construction & Destruction
		Logger(std::string _mainFile = "log.txt", std::string _threadName = "Main", uint8_t _severity = 0);
		~Logger();
		/// <summary>
		/// Call to destroy and clean up the logger.
		/// </summary>
		static void Destruct()
		{
			delete instance;
		}


	private: // Private function declerations
		static void MainLoop();
		static void ProcessMessages();
		static void WriteToLog();


	public: // Public inline function definitions
		/// <summary>
		/// Registers a new tag to use with the logger.
		/// </summary>
		/// <param name="_tagId">ID of the tag.</param>
		/// <param name="_tagName">Name of the tag (leave it as an empty string to give it no name).</param>
		/// <param name="_tagSeverity">Severity of the tag. Used to determine if it should be logged.</param>
		/// <param name="_tagForeground">Foreground color of the tag.</param>
		/// <param name="_tagBackground">Background color of the tag.</param>
		/// <param name="_file">File to log to. Leave it blank to use the default logger file specified in the constructor.</param>
		/// <returns>True if successful, false if the tag ID already exists.</returns>
		static bool RegisterNewTag(uint32_t _tagId, std::string _tagName= "", uint8_t _tagSeverity = 3, Logger::LoggerColors _tagForeground = COLOR_WHITE, Logger::LoggerColors _tagBackground = COLOR_BLACK, std::string _file = "")
		{
			std::lock_guard<std::mutex> lk(mapMutex);

			if (tagData.find(_tagId) != tagData.end())
			{
				messageQueue.push({ "Tag already exists: " + std::to_string(_tagId) + "\n", Logger::ERROR, std::this_thread::get_id() }); // bypass Log() as it re-aquires the mutex to check for a valid tag (which we know its valid)
				return false;
			}
			if (_file == "")
				_file = mainFile;
			if (filesMade.find(_file) == filesMade.end())
			{
				std::ofstream logfile(_file);
				logfile.close();
				filesMade.insert(_file);
			}

			tagData[_tagId] = {
				_tagName,
				_tagSeverity,
				(uint8_t)_tagForeground,
#ifdef AETH_WINDOWS
				(uint8_t)((uint32_t)_tagBackground << 4),
#elif defined(AETH_UNIX)
				(uint8_t)((uint32_t)_tagBackground + (uint32_t)10),
#endif // AETH_WINDOWS || AETH_UNIX
				_file
			};
			return true;
		}

		/// <summary>
		/// Names the thread that it is being called from.
		/// </summary>
		static void NameThread(std::string _name)
		{
			std::lock_guard<std::mutex> lock(mapMutex);
			threadNames[std::this_thread::get_id()] = _name;
		}

		/// <summary>
		/// Log text to the console.
		/// </summary>
		static void Log(std::string _s, uint32_t _tag = OTHER)
		{
			{
				mapMutex.lock();
				if (tagData.find(_tag) == tagData.end())
					_tag = 0;
				mapMutex.unlock();
			}

			messageQueue.push({ _s , _tag, std::this_thread::get_id() });

			if (_tag == EXCEPTION)
			{
				HandleExit();
				std::exit(-1);
			}
		}

		/// <summary>
		/// Lock the stream output and aquire a handle to use for stream output.
		/// </summary>
		/// <returns>Handle to stream output with.</returns>
		static Logger& Lock(uint32_t _tag = OTHER)
		{
			streamMutex.lock();
			streamTag = _tag;
			return *instance;
		}

		void operator <<(LoggerEndltag _end)
		{
			Logger::inputStream.flush();
			Log(Logger::inputStream.str(), streamTag);
			Logger::inputStream.str("");
#pragma warning(push)
#pragma warning(disable: 26110)
			streamMutex.unlock();
#pragma warning(pop)
		}

		template<class T>
		inline Logger& operator <<(T _val)
		{
			Logger::inputStream << _val;
			return *instance;
		}

	private: // Private inline function definitions
		static void HandleExit()
		{
			running = false;
			if (loopThread.joinable())
				loopThread.join(); // Ensure the thread is stopped
			while (!messageQueue.empty())
				ProcessMessages(); // Process left over messages
		}

		static void PrintToConsole(std::string _s, uint8_t foreground, uint8_t background)
		{
#ifdef AETH_WINDOWS
			HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
			SetConsoleTextAttribute(hConsole, foreground | background);

			printf(_s.data());

			SetConsoleTextAttribute(hConsole, 7);

			printf("\n");
#elif defined(AETH_UNIX)
			_s = "\033[" + std::to_string(foreground) + ";" + std::to_string(background) + "m" + _s + "\033[0m\n";
			printf(_s.data());
#endif // AETH_WINDOWS || AETH_UNIX
		}

		static std::string GetTimestamp()
		{
			char charBuffer[128];

#ifdef AETH_WINDOWS
			time_t rawTime;
			struct tm* timeInfo = new tm();
			time(&rawTime);
			localtime_s(timeInfo, &rawTime);
			strftime(charBuffer, sizeof(charBuffer), "%j_%H:%M:%S", timeInfo);
			delete timeInfo;
#elif defined(AETH_UNIX)
			time_t rawTime;
			time(&rawTime);
			struct tm* timeInfo = localtime(&rawTime);
			strftime(charBuffer, sizeof(charBuffer), "%j_%H:%M:%S", timeInfo);
#endif // AETH_WINDOWS || AETH_UNIX

			return charBuffer;
		}

		static void AddStringTags(std::string& _s, const uint32_t& _tag, std::thread::id _thread)
		{
			std::lock_guard<std::mutex> lock(mapMutex);

			std::string timestamp = GetTimestamp();

			std::string threadstamp;
			if (threadNames.find(_thread) != threadNames.end())
				threadstamp = threadNames[_thread];
			else
				threadstamp = "Unnamed";

			std::string tagstamp = tagData[_tag].name;

			if (tagstamp == "")
				_s = "[" + timestamp + "] <T:" + threadstamp + "> " + _s;
			else
				_s = "[" + timestamp + "] <T:" + threadstamp + "> (" + tagstamp + ") " + _s;
		}
	};
}
