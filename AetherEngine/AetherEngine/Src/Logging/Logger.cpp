#include "Logger.h"

#include <exception>
#include <iostream>

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#endif

#undef ERROR

namespace Aeth::Logging
{
#pragma region Static Decleration
	Logger* Logger::instance;

	std::thread Logger::loopThread;
	std::atomic<bool> Logger::running;

	std::string Logger::mainFile;
	uint8_t Logger::severityLevel;

	std::unordered_map<std::thread::id, std::string> Logger::threadNames;
	std::unordered_map<uint32_t, Logger::TagData> Logger::tagData;

	Utils::StlExtensions::SharedQueue<Logger::MessageData> Logger::messageQueue;
	std::unordered_set<std::string> Logger::filesMade;
	std::deque<Logger::WriteData> Logger::writeQueue;

	std::mutex Logger::mapMutex;
	std::mutex Logger::streamMutex;

	std::stringstream Logger::inputStream;
	uint32_t Logger::streamTag;
#pragma endregion

	Logger::Logger(std::string _mainFile, std::string _threadName, uint8_t _severity)
	{
		if (instance)
			throw new std::runtime_error("Attempted to instantiate Logger class twice. This is not allowed as Logger is a singleton.");
		instance = this;

		mainFile = _mainFile;
		NameThread(_threadName);
		severityLevel = _severity;

		std::ofstream logfile(_mainFile);
		logfile.flush();
		logfile.close();
		filesMade.insert(_mainFile);

		RegisterNewTag(OTHER, "", 3, COLOR_WHITE);
		RegisterNewTag(INFO, "INFO", 3, COLOR_WHITE);
		RegisterNewTag(WARNING, "WARNING", 2, COLOR_BLACK, COLOR_BRIGHT_YELLOW);
		RegisterNewTag(ERROR, "ERROR", 1, COLOR_BRIGHT_WHITE, COLOR_BRIGHT_RED);
		RegisterNewTag(EXCEPTION, "EXCEPTION", 1, COLOR_BRIGHT_WHITE, COLOR_BRIGHT_RED);
		RegisterNewTag(DEBUG, "DEBUG", 1, COLOR_BRIGHT_CYAN);

		std::atexit(HandleExit);

		running = true;
		loopThread = std::thread(Logger::MainLoop);

		Log("Logger Initialized", INFO);
	}

	Logger::~Logger()
	{
		instance = nullptr;

		running = false; // Signal thread to shutdown
		loopThread.join(); // Wait for thread to end

		while (!messageQueue.empty() || !writeQueue.empty())
			ProcessMessages();

		filesMade.clear();
		threadNames.clear();
		tagData.clear();
		writeQueue.clear();
		messageQueue.clear();
	}

	void Logger::MainLoop()
	{
		NameThread("Logger");
		while (running || !messageQueue.empty())
		{
			ProcessMessages();
		}
	}

	void Logger::ProcessMessages()
	{
		do
		{
			MessageData messageData;
			bool timedOut = messageQueue.front(std::chrono::milliseconds(16), messageData); // SharedQueue::front() holds the thread with a condition variable if it is empty

			if (timedOut)
				break;

			AddStringTags(messageData.message, messageData.tag, messageData.thread);

			uint8_t severity;
			uint8_t foreground;
			uint8_t background;
			std::string file;
			{
				std::lock_guard<std::mutex> lock(mapMutex);

				severity = tagData[messageData.tag].severity;
				foreground = tagData[messageData.tag].foreground;
				background = tagData[messageData.tag].background;
				file = tagData[messageData.tag].file;
			}

			if (severity <= severityLevel)
				PrintToConsole(messageData.message, foreground, background);

			writeQueue.push_back({ messageData.message, file });
			messageQueue.pop(std::chrono::milliseconds(50));

			size_t size = sizeof(MessageData) * messageQueue.size();
			if (size > 500000) // If the message queue is growing too big in memory, we should start trying to empty it
				break;
		} while (!messageQueue.empty());

		WriteToLog();

	}

	void Logger::WriteToLog()
	{
		constexpr size_t _maxLoops = 2000;
		constexpr uint8_t _maxFiles = 5;
		size_t loopCounter = 0;
		uint8_t fileCounter = 0;

		while (!writeQueue.empty())
		{
			if (loopCounter >= _maxLoops && fileCounter >= _maxFiles && running)
				break;

			std::vector<std::string> writeChunks;
			WriteData writeData = writeQueue.front();
			std::string file = writeData.file;
			while (writeData.file == file)
			{
				writeChunks.push_back(writeData.message + "\n"); // Will only push back if the file is the same
				writeQueue.pop_front();
				if (writeQueue.empty())
					break;
				writeData = writeQueue.front();
			}
			loopCounter += writeChunks.size();
			fileCounter++;

			if (file == "")
				file = mainFile;
			std::ofstream logfile(file, std::ios::ios_base::app | std::ios::ios_base::in);
			for (std::string& chunk : writeChunks)
				logfile << chunk << std::flush;
			logfile.close();	
		}
		
	}
}
