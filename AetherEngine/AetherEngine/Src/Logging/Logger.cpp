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

char Logger::severityLevel;
bool Logger::addTags = true;

std::unordered_map<int, Logger::TagData*> Logger::tagDataMap;
std::unordered_map<std::thread::id, std::string> Logger::threadNameMap;

std::list<std::string> Logger::buffer;
std::chrono::steady_clock::time_point Logger::nextWriteTime;

std::mutex Logger::lock;
#pragma endregion

Logger::Logger(std::string threadName, char severity)
{
	if (instance)
		throw new std::runtime_error("Attempted to instantiate Logger class twice. This is not allowed as Logger is a singleton.");
	instance = this;
	/*std::atexit(memtrack::DumpUnfreed);
	memtrack::tracking = true;*/

	NameThread(threadName);
	severityLevel = severity;

	std::ofstream logfile("log.txt");
	logfile.close();

	RegisterNewTag(OTHER, "", COLOR_WHITE, 3);
	RegisterNewTag(INFO, "INFO", COLOR_WHITE, 3);
	RegisterNewTag(WARNING, "WARNING", COLOR_BRIGHT_YELLOW, 2);
	RegisterNewTag(ERROR, "ERROR", COLOR_BRIGHT_RED, 1);
	RegisterNewTag(EXCEPTION, "ERROR", COLOR_BRIGHT_RED, 1);
	RegisterNewTag(DEBUG, "DEBUG", COLOR_BRIGHT_CYAN, 4);


	nextWriteTime = std::chrono::steady_clock::now() + std::chrono::seconds(2);

	std::atexit(HandleExit);

	Logln("Logger Initialized", INFO);
}

Logger::~Logger()
{
	instance = NULL;

	std::lock_guard<std::mutex> lk(lock);

	for (const auto data : tagDataMap)
	{
		delete data.second;
	}
}

bool Logger::RegisterNewTag(int tagId, std::string tagString, Logger::LoggerColors tagColor, char tagSeverity)
{
	std::lock_guard<std::mutex> lk(lock);

	if (tagDataMap.find(tagId) != tagDataMap.end())
	{
		WriteToBuffer("Tag already exists: " + std::to_string(tagId) + "\n", Logger::ERROR);
		return false;
	}
	TagData* data = new TagData();
	data->name = tagString;
	data->color = tagColor;
	data->severity = tagSeverity;

	tagDataMap[tagId] = data;
	return true;
}

void Logger::NameThread(std::string name)
{
	std::lock_guard<std::mutex> lk(lock);

	threadNameMap[std::this_thread::get_id()] = name;
}

void Logger::HandleExit()
{
	nextWriteTime = std::chrono::steady_clock::now() - std::chrono::seconds(2);
	WriteToLog();
}

void Logger::Log(std::string s, int tag)
{
	std::lock_guard<std::mutex> lk(lock);

	if (tagDataMap.find(tag) == tagDataMap.end())
		tag = 0;

	WriteToBuffer(s, tag);
	addTags = false;

	if (tag == EXCEPTION)
	{
		HandleExit();
		throw new std::runtime_error(s);
	}
}

void Logger::Logln(std::string s, int tag)
{
	std::lock_guard<std::mutex> lk(lock);

	if (tagDataMap.find(tag) == tagDataMap.end())
		tag = 0;

	WriteToBuffer(s + '\n', tag);

	if (tag == EXCEPTION)
	{
		HandleExit();
		throw new std::runtime_error(s);
	}
}

void Logger::WriteToBuffer(std::string s, int tag)
{
	if (addTags)
	{
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

		std::string tagstr = tagDataMap[tag]->name;

		if (tagstr == "")
			s = "[" + timestr + "] <T:" + threadstr + "> " + s;
		else
			s = "[" + timestr + "] <T:" + threadstr + "> (" + tagstr + ") " + s;
	}
	else
		addTags = true;

	if (tagDataMap[tag]->severity <= severityLevel)
		PrintColored(tagDataMap[tag]->color, s);

	buffer.push_back(s);

	WriteToLog();
}

void Logger::WriteToLog()
{
	if (std::chrono::steady_clock::now() < nextWriteTime)
		return;

	std::ofstream logfile("log.txt", std::ios::ios_base::app);
	for (std::string str : buffer)
		logfile << str << std::endl;
	logfile.close();
	buffer.clear();
	std::cout.flush();
	nextWriteTime = std::chrono::steady_clock::now() + std::chrono::seconds(2);
}

void Logger::PrintColored(int color, std::string s)
{
#ifdef _WINDOWS
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hConsole, color);

	std::cout << s;

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
