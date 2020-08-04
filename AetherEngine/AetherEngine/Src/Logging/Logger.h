#pragma once

#include <unordered_map>
#include <mutex>
#include <string>
#include <sstream>
#include <thread>
#include <list>

#undef ERROR

class Logger
{
public:
#pragma region Enums
	enum LoggerTags
	{
		OTHER = 0,
		INFO,
		WARNING,
		ERROR,
		EXCEPTION,
		DEBUG
	};

	enum LoggerColors
	{
		COLOR_BLUE = 1,
		COLOR_GREEN,
		COLOR_CYAN,
		COLOR_RED,
		COLOR_PURPLE,
		COLOR_YELLOW,
		COLOR_WHITE,
		COLOR_GRAY,
		COLOR_BRIGHT_BLUE,
		COLOR_BRIGHT_GREEN,
		COLOR_BRIGHT_CYAN,
		COLOR_BRIGHT_RED,
		COLOR_MAGENTA,
		COLOR_BRIGHT_YELLOW,
		COLOR_BRIGHT_WHITE
	};
#pragma endregion

	static Logger* instance;

	Logger();
	Logger(std::string threadName);
	~Logger();

	static bool RegisterNewTag(int tagId, std::string tagString, int tagColor = COLOR_WHITE);
	static void NameThread(std::string name);

	static void Log(std::string s, int tag = 0);


private:
	static std::unordered_map<int, std::string> tagStringMap;
	static std::unordered_map<int, int> tagColorMap;
	static std::unordered_map<std::thread::id, std::string> threadNameMap;

	static std::list<std::string> buffer;
	static std::chrono::steady_clock::time_point nextWriteTime;

	static std::mutex lock;

	static void HandleExit();

	static void WriteToBuffer(std::string s, int tag);
	static void WriteToLog();

	static void PrintColored(int color, std::string s);


#pragma region Header Implemented Functions
public:
	template<class ...Argst>
	static void Log(int tag, Argst ...args)
	{
#if not _DEBUG
			return;
#endif

		std::stringstream bufferStream;
		SubLog(bufferStream, args...);
		bufferStream.flush();
		Log(bufferStream.str(), tag);
	}

private:
	template<class A0, class ...Args>
	static void SubLog(std::stringstream& bufferStream, A0 arg0, Args ...args)
	{
		bufferStream << arg0;
		SubLog(bufferStream, args...);
	}

	// empty function required to cut off the iteration above. It's called once it runs out of args
	static void SubLog(std::stringstream& bufferStream) {}
#pragma endregion
};