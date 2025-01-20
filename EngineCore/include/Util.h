#pragma once
#include <string>
#include <type_traits>
#include <unordered_map>

#include <fstream>
#include <sstream>

#include "Log.h"

class Util {
private:
	static std::unordered_map<unsigned int, const std::string> s_typeNameMap;

public:
	inline static void TrimWhitespace(std::string& str) {
		const std::string delimiters = " \f\n\r\t\v";
		str.erase(str.find_last_not_of(delimiters) + 1);
		str.erase(0, str.find_first_not_of(delimiters));
	}

	inline static bool ReadFileToString(const char* path, std::string& content) {
		std::ifstream file(path);
		if (!file.is_open()) {
			Log::Writef(LogEntry::Severity::Error, "Could not open file: %s", path);
			return false;
		}
		Log::Writef(LogEntry::Severity::Error, "Successfully read file %s", path);
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
