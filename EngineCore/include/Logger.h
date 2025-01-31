#pragma once

#include <ctime>
#include <fmt/format.h>
#include <string>
#include <unordered_map>
#include <vector>

namespace Cicada {

struct LogEntry {

	enum class Level {
		Info,
		Warning,
		Error
	};

	Level m_severity;
	std::time_t m_timeStamp;
	std::string m_content;
	
	LogEntry(Level level, const std::string& content);
};

class Logger {
private:
	static bool s_showLog;
	static bool s_autoScroll;
	static size_t s_maxEntries;
	static std::vector<LogEntry> s_log;
	static const std::unordered_map<LogEntry::Level, std::string> s_msgSeverityMap;
	static const std::unordered_map<unsigned int, std::string> s_glTypeNameMap;

	static void _Writefv(LogEntry::Level severity, fmt::string_view fmt, fmt::format_args args);
public:
	static void ToggleLog();
	static void RenderLog();

	static void Write(LogEntry::Level severity, const std::string& msg);
	static void Log(const std::string& msg);
	static void Warn(const std::string& msg);
	static void Error(const std::string& msg);

	template <typename... Args>
	inline static void Writef(LogEntry::Level severity, fmt::format_string<Args...> fmt, Args&&... args) {
		_Writefv(severity, fmt, fmt::make_format_args(args...));
	}
	template <typename... Args>
	inline static void Infof(fmt::format_string<Args...> fmt, Args&&... args) {
		_Writefv(LogEntry::Level::Info, fmt, fmt::make_format_args(args...));
	}
	template <typename... Args>
	inline static void Warnf(fmt::format_string<Args...> fmt, Args&&... args) {
		_Writefv(LogEntry::Level::Warning, fmt, fmt::make_format_args(args...));
	}
	template <typename... Args>
	inline static void Errorf(fmt::format_string<Args...> fmt, Args&&... args) {
		_Writefv(LogEntry::Level::Error, fmt, fmt::make_format_args(args...));
	}

	static std::string GLTypeToStr(unsigned int glType);
};

}