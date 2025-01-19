#pragma once
#include <cstdarg>
#include <string>
#include <ctime>
#include <type_traits>
#include <unordered_map>
#include <vector>

#include <fstream>
#include <sstream>

#include <fmt/core.h>

struct LogEntry {

	enum class Severity {
		Info,
		Warning,
		Error
	};

	const std::string m_content;
	std::time_t m_timeStamp;
	Severity m_severity;

	LogEntry(const std::string& content, Severity severity):
		m_content(content), m_severity(severity), m_timeStamp(std::time(0)) {}
};

class Util {
private:
	static std::unordered_map<unsigned int, const std::string> s_typeNameMap;
	static std::vector<LogEntry> s_log;

public:
	inline static void TrimWhitespace(std::string& str) {
		const std::string delimiters = " \f\n\r\t\v";
		str.erase(str.find_last_not_of(delimiters) + 1);
		str.erase(0, str.find_first_not_of(delimiters));
	}

	inline static const std::vector<LogEntry>& GetLog() {
		return s_log;
	}

	inline static void Log(LogEntry::Severity severity, const std::string& msg) {
		s_log.emplace_back(msg, severity);
	}
	inline static void Log(LogEntry::Severity severity, const char* msg) {
		s_log.emplace_back(msg, severity);
	}

	inline static void Logf(LogEntry::Severity severity, const char* fmt, ...) {
		static const unsigned int CHAR_LIMIT = 256; 
		va_list args;
		va_start(args, fmt);
		char message[CHAR_LIMIT];
		vsnprintf(message, CHAR_LIMIT, fmt, args);
		va_end(args);
		s_log.emplace_back(message, severity);
	}

	inline static bool ReadFileToString(const char* path, std::string& content) {
		std::ifstream file(path);
		if (!file.is_open()) {
			Logf(LogEntry::Severity::Error, "Could not open file: %s", path);
			return false;
		}
		Logf(LogEntry::Severity::Error, "Successfully read file %s", path);
		content.assign(std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>());
		file.close();
		return true;
	}

	//linear interpolation from one numeric range to another, this takes any numeric type as input
	template <typename T>
	inline static constexpr T NumericMap(T input, T inputMin, T inputMax, T outputMin, T outputMax) {
		static_assert(std::is_arithmetic<T>::value, "type must be numeric");
		return outputMin + ((outputMax - outputMin) / (inputMax - inputMin)) * (input - inputMin);
	}

	static const std::string TypeStr(unsigned int glType);
};
