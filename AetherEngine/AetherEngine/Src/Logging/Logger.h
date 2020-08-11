#pragma once

#include <unordered_map>
#include <mutex>
#include <string>
#include <sstream>
#include <thread>
#include <list>

#ifdef _VULKAN
#include <vulkan/vulkan.h>
#endif


/// <summary>
/// Singleton logger class, inteded to replace std::cout. Adds extra features ontop of regular console logging including logging to a log file.
/// </summary>
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

	/// <summary>
	/// singleton instance of this class
	/// </summary>
	static Logger* instance;

	/// <summary>
	/// Creates the first (and only) instance of the logger class.
	/// </summary>
	/// <param name="threadName">Name for the thread this logger is started on. Used in the information preceding log text.</param>
	/// <param name="severity">Severity level. If a log tag has a lower or equal severity, is it printed to the console. All messages are logged to a file regardless of severity.</param>
	/// <returns></returns>
	Logger(std::string threadName = "Main", char severity = 0);
	~Logger();

	/// <summary>
	/// Create and register a new tag for the logger to use.
	/// </summary>
	/// <param name="tagId">Numeric ID to reference this tag by. If a tag with the same ID is found, a warning will be logged and the function will return false.</param>
	/// <param name="tagString">The text that shows up in the preceding log information when this tag is used.</param>
	/// <param name="tagColor">The colour ID for the text using this tag.</param>
	/// <param name="tagSeverity">Severity number for this log. If it is at or below the global severity level, it will be printed to the console.</param>
	/// <returns>Returns true on success and false on failure. Only soft failure case is if the tagID clashes.</returns>
	static bool RegisterNewTag(int tagId, std::string tagString, LoggerColors tagColor = COLOR_WHITE, char tagSeverity = 3);

	/// <summary>
	/// Name the thread that this function is called from.
	/// </summary>
	/// <param name="name">Name of the thread</param>
	static void NameThread(std::string name);


	/// <summary>
	/// Log some text without a new line.
	/// </summary>
	/// <param name="s">Text to log.</param>
	/// <param name="tag">Tag to use, defaults to no tag.</param>
	static void Log(std::string s, int tag = 0);

	/// <summary>
	/// Log some text and append a new line.
	/// </summary>
	/// <param name="s">Text to log. A new line is appended automatically</param>
	/// <param name="tag">Tag to use, defaults to no tag.</param>
	static void Logln(std::string s, int tag = 0);


private:
	static char severityLevel; // 0 = nothing, 1 = error, 2 = error & warning, 3 = error & warning & info, 4 = error & warning & info & verbose
	static bool addTags;

	struct TagData
	{
		std::string name;
		int color = 0;
		char severity = 0;
	};

	static std::unordered_map<int, TagData*> tagDataMap;
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
	/// <summary>
	/// Log multiple variables and append a new line.
	/// !!WARNING!!: This multi-arg version of the log function is worse for performance. Where possible, use the other function.
	/// </summary>
	/// <typeparam name="...Argst"></typeparam>
	/// <param name="tag">Tag to use.</param>
	/// <param name="...args">Variable(s) to print</param>
	template<class ...Argst>
	static void LogArgln(int tag, Argst ...args)
	{
		std::stringstream bufferStream;
		SubLog(bufferStream, args...);
		bufferStream.flush();
		Logln(bufferStream.str(), tag);
	}

	/// <summary>
	/// Log multiple variables with no new line.
	/// !!WARNING!!: This multi-arg version of the log function is worse for performance. Where possible, use the other function.
	/// </summary>
	/// <typeparam name="...Argst"></typeparam>
	/// <param name="tag">Tag to use.</param>
	/// <param name="...args">Variable(s) to print</param>
	template<class ...Argst>
	static void LogArg(int tag, Argst ...args)
	{
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


#if defined(_DEBUG) && defined(_VULKAN)
static VKAPI_ATTR VkBool32 VKAPI_CALL VulkanDebugCallback(
	VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
	VkDebugUtilsMessageTypeFlagsEXT messageType,
	const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
	void* pUserData)
{
	std::string msg("Vulkan Callback: [" + std::string(pCallbackData->pMessageIdName) + "] code: " + std::to_string(pCallbackData->messageIdNumber) + " : " + std::string(pCallbackData->pMessage));

	if ((messageSeverity & VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT) == VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT)
	{
		Logger::Log(msg, Logger::DEBUG);
	}
	else if ((messageSeverity & VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT) == VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT)
	{
		Logger::Log(msg, Logger::INFO);
	}
	else if ((messageSeverity & VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT) == VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT)
	{
		Logger::Log(msg, Logger::WARNING);
	}
	else if ((messageSeverity & VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT) == VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT)
	{
		Logger::Log(msg, Logger::ERROR);
	}
	else
	{
		Logger::Log("Unknown vulkan message severity: " + std::to_string(messageSeverity), Logger::EXCEPTION);
	}

	return VK_FALSE;
}
#endif