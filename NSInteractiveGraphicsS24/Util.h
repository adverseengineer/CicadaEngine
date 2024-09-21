#pragma once
#include <cstdarg>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

class Util {
private:
	static std::stringstream s_log;
	static const unsigned int MAX_CHARS = 512;

public:
	inline static void TrimWhitespace(std::string& str) {
		const std::string delimiters = " \f\n\r\t\v";
		str.erase(str.find_last_not_of(delimiters) + 1);
		str.erase(0, str.find_first_not_of(delimiters));
	}

	inline static const std::string GetLog() {
		return s_log.str();
	}

	inline static void Log(const std::string& message) {
		s_log << message << std::endl;
	}

	//inline static void Log(const char* message) {
	//	_log << message << std::endl;
	//}

	inline static void Log(const std::vector<char>& infoLog) {
		std::copy(infoLog.begin(), infoLog.end(), std::ostream_iterator<char>(s_log, ""));
	}

	inline static void Log(const char* fmt, ...) {
		va_list args;
		va_start(args, fmt);
		char buf[MAX_CHARS];
		vsnprintf(buf, MAX_CHARS, fmt, args);
		va_end(args);
		s_log << buf << std::endl;
	}

	inline static bool ReadFileToString(const char* path, std::string& content) {
		std::ifstream file(path);
		if (!file.is_open()) {
			Log("Could not open file: %s", path);
			return false;
		}
		Log("Successfully read file %s", path);
		content.assign(std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>());
		file.close();
		return true;
	}
};
