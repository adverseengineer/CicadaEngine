#pragma once
#include <cstdarg>
#include <sstream>
#include <string>
#include <vector>

class Util {
private:
	static std::stringstream _log;
	static const unsigned int MAX_CHARS = 512;

public:
	inline static void Trim(std::string& str) {
		const std::string delimiters = " \f\n\r\t\v";
		str.erase(str.find_last_not_of(delimiters) + 1);
		str.erase(0, str.find_first_not_of(delimiters));
	}



	inline static const std::string GetLog() {
		return _log.str();
	}
	
	inline static void Log(const std::string& message) {
		_log << message << std::endl;
	}

	//inline static void Log(const char* message) {
	//	_log << message << std::endl;
	//}

	inline static void Log(const std::vector<char>& infoLog) {
		std::copy(infoLog.begin(), infoLog.end(), std::ostream_iterator<char>(_log, ""));
	}

	inline static void Log(const char* fmt, ...) {
		va_list args;
		va_start(args, fmt);
		char buf[MAX_CHARS];
		vsnprintf(buf, MAX_CHARS, fmt, args);
		va_end(args);
		_log << buf << std::endl;
	}
};
