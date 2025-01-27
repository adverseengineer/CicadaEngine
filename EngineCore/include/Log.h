#pragma once

#include <ctime>
#include <fmt/format.h>
#include <string>
#include <unordered_map>
#include <vector>

namespace Cicada {

struct LogEntry {

	enum class Severity {
		Info,
		Warning,
		Error
	};

	Severity m_severity;
	std::time_t m_timeStamp;
	std::string m_content;
	
	LogEntry(Severity severity, const std::string& content);
};

class Log {
private:
	static bool s_showLog;
	static bool s_autoScroll;
	static size_t s_maxEntries;
	static std::vector<LogEntry> s_log;
	static const std::unordered_map<LogEntry::Severity, std::string> s_msgSeverityMap;
	static const std::unordered_map<unsigned int, std::string> s_glTypeNameMap;

	static void _Writefv(LogEntry::Severity severity, fmt::string_view fmt, fmt::format_args args);
public:
	static void ToggleLog();
	static void RenderLog();

	static void Write(LogEntry::Severity severity, const std::string& msg);
	static void Info(const std::string& msg);
	static void Warn(const std::string& msg);
	static void Error(const std::string& msg);

	template <typename... Args>
	inline static void Writef(LogEntry::Severity severity, fmt::format_string<Args...> fmt, Args&&... args) {
		_Writefv(severity, fmt, fmt::make_format_args(args...));
	}
	template <typename... Args>
	inline static void Infof(fmt::format_string<Args...> fmt, Args&&... args) {
		_Writefv(LogEntry::Severity::Info, fmt, fmt::make_format_args(args...));
	}
	template <typename... Args>
	inline static void Warnf(fmt::format_string<Args...> fmt, Args&&... args) {
		_Writefv(LogEntry::Severity::Warning, fmt, fmt::make_format_args(args...));
	}
	template <typename... Args>
	inline static void Errorf(fmt::format_string<Args...> fmt, Args&&... args) {
		_Writefv(LogEntry::Severity::Error, fmt, fmt::make_format_args(args...));
	}

	static std::string GLTypeToStr(unsigned int glType);
};

}