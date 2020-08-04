#include "Logger.h"

#include <exception>
#include <fstream>
#include <iostream>
#include <time.h>

#ifdef WIN32
#include <Windows.h>
#endif

#undef ERROR


#pragma region Static Decleration
Logger* Logger::instance;

std::unordered_map<int, std::string> Logger::tagStringMap;
std::unordered_map<int, int> Logger::tagColorMap;
std::unordered_map<std::thread::id, std::string> Logger::threadNameMap;

std::list<std::string> Logger::buffer;
std::chrono::steady_clock::time_point Logger::nextWriteTime;

std::mutex Logger::lock;
#pragma endregion

Logger::Logger()
{
	if (instance)
		throw new std::runtime_error("Attempted to instantiate Logger class twice. This is not allowed as Logger is a singleton.");
	instance = this;

	std::ofstream logfile("log.txt");
	logfile.close();

	RegisterNewTag(OTHER, "");
	RegisterNewTag(INFO, "INFO");
	RegisterNewTag(WARNING, "WARNING", COLOR_BRIGHT_YELLOW);
	RegisterNewTag(ERROR, "ERROR", COLOR_BRIGHT_RED);
	RegisterNewTag(EXCEPTION, "ERROR", COLOR_BRIGHT_RED);
	RegisterNewTag(DEBUG, "DEBUG", COLOR_BRIGHT_CYAN);


	nextWriteTime = std::chrono::steady_clock::now() + std::chrono::seconds(2);

	std::atexit(HandleExit);

	Log("Logger Initialized", INFO);
}

Logger::Logger(std::string threadName)
{
	if (instance)
		throw new std::runtime_error("Attempted to instantiate Logger class twice. This is not allowed as Logger is a singleton.");
	instance = this;

	NameThread(threadName);

	std::ofstream logfile("log.txt");
	logfile.close();

	RegisterNewTag(OTHER, "");
	RegisterNewTag(INFO, "INFO");
	RegisterNewTag(WARNING, "WARNING", COLOR_BRIGHT_YELLOW);
	RegisterNewTag(ERROR, "ERROR", COLOR_BRIGHT_RED);
	RegisterNewTag(EXCEPTION, "ERROR", COLOR_BRIGHT_RED);
	RegisterNewTag(DEBUG, "DEBUG", COLOR_BRIGHT_CYAN);


	nextWriteTime = std::chrono::steady_clock::now() + std::chrono::seconds(2);

	std::atexit(HandleExit);

	Log("Logger Initialized", INFO);
}

Logger::~Logger()
{
	instance = NULL;

	std::lock_guard<std::mutex> lk(lock);

	tagStringMap.clear();
	tagColorMap.clear();
	threadNameMap.clear();
}

bool Logger::RegisterNewTag(int tagId, std::string tagString, int tagColor)
{
	std::lock_guard<std::mutex> lk(lock);

	if (tagStringMap.find(tagId) != tagStringMap.end())
	{
		//TODO: Implement error output when tags clash with other tags. Perhaps only warning level, not error level.
		WriteToBuffer("Tag already exists: " + tagId, Logger::ERROR);
		return false;
	}
	tagStringMap[tagId] = tagString;
	tagColorMap[tagId] = tagColor;
	return true;
}

void Logger::NameThread(std::string name)
{
	std::lock_guard<std::mutex> lk(lock);

	threadNameMap[std::this_thread::get_id()] = name;
}

void Logger::HandleExit()
{
	WriteToLog();
}

void Logger::Log(std::string s, int tag)
{
#if not _DEBUG
	return;
#endif

	std::lock_guard<std::mutex> lk(lock);

	WriteToBuffer(s, tag);

	if (tag == EXCEPTION)
		throw new std::runtime_error(s);
}

void Logger::WriteToBuffer(std::string s, int tag)
{
	if (tagStringMap.find(tag) == tagStringMap.end())
		tag = 0;

	std::string timestr;
	{
#ifdef _WINDOWS
		time_t rawtime;
		struct tm* timeinfo = new tm();
		char charbuffer[128];
		time(&rawtime);
		localtime_s(timeinfo, &rawtime);
		strftime(charbuffer, sizeof(charbuffer), "%j_%H:%M:%S", timeinfo);
		delete timeinfo;
		timestr = std::string(charbuffer);
#elif _LINUX
		time_t rawtime;
		char charbuffer[128];
		time(&rawtime);
		struct tm* timeinfo = localtime(&rawtime);
		strftime(charbuffer, sizeof(charbuffer), "%j_%H:%M:%S", timeinfo);
		timestr = std::string(charbuffer);
#endif
	}

	std::string threadstr;
	if (threadNameMap.find(std::this_thread::get_id()) != threadNameMap.end())
		threadstr = threadNameMap[std::this_thread::get_id()];
	else
		threadstr = "Unnamed Thread";

	std::string tagstr = tagStringMap[tag];

	std::string out;
	if (tagstr == "")
		out = "[" + timestr + "] <T:" + threadstr + "> " + s;
	else
		out = "[" + timestr + "] <T:" + threadstr + "> (" + tagstr + ") " + s;

	PrintColored(tagColorMap[tag], out);

	buffer.push_back(out);

	if (std::chrono::steady_clock::now() >= nextWriteTime)
		WriteToLog();
}

void Logger::WriteToLog()
{
	std::ofstream logfile("log.txt", std::ios::ios_base::app);
	for (std::string str : buffer)
		logfile << str << std::endl;
	logfile.close();
	buffer.clear();
	nextWriteTime = std::chrono::steady_clock::now() + std::chrono::seconds(2);
}

void Logger::PrintColored(int color, std::string s)
{
#ifdef _WINDOWS
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hConsole, color);

	std::cout << s << std::endl;

	SetConsoleTextAttribute(hConsole, 7);
#elif _LINUX
	//I don't *think* there is a better way to do the ANSI colors
	//other than unnecessarily making a map or class to do it
	switch (color)
	{
	case LoggerColors::COLOR_RED:
		s = "\033[31m" + s + "\033[0m";
		break;
	case LoggerColors::COLOR_GREEN:
		s = "\033[32m" + s + "\033[0m";
		break;
	case LoggerColors::COLOR_YELLOW:
		s = "\033[33m" + s + "\033[0m";
		break;
	case LoggerColors::COLOR_BLUE:
		s = "\033[34m" + s + "\033[0m";
		break;
	case LoggerColors::COLOR_PURPLE:
		s = "\033[35m" + s + "\033[0m";
		break;
	case LoggerColors::COLOR_CYAN:
		s = "\033[36m" + s + "\033[0m";
		break;
	case LoggerColors::COLOR_WHITE:
		s = "\033[37m" + s + "\033[0m";
		break;
	case LoggerColors::COLOR_GRAY:
		s = "\033[90m" + s + "\033[0m";
		break;
	case LoggerColors::COLOR_BRIGHT_RED:
		s = "\033[91m" + s + "\033[0m";
		break;
	case LoggerColors::COLOR_BRIGHT_GREEN:
		s = "\033[92m" + s + "\033[0m";
		break;
	case LoggerColors::COLOR_BRIGHT_YELLOW:
		s = "\033[93m" + s + "\033[0m";
		break;
	case LoggerColors::COLOR_BRIGHT_BLUE:
		s = "\033[94m" + s + "\033[0m";
		break;
	case LoggerColors::COLOR_MAGENTA:
		s = "\033[95m" + s + "\033[0m";
		break;
	case LoggerColors::COLOR_BRIGHT_CYAN:
		s = "\033[96m" + s + "\033[0m";
		break;
	case LoggerColors::COLOR_BRIGHT_WHITE:
		s = "\033[97m" + s + "\033[0m";
		break;
	default:
		Log("Malformed color type when attempting to print.", LoggerTags::EXCEPTION);
	}

	std::cout << s << std::endl;
#endif
}
