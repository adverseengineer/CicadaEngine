#pragma once

#include "Log.h"
#include <fstream>
#include <sstream>
#include <string>
#include <type_traits>
#include <unordered_map>

namespace Cicada {

class Util {
public:
	inline static void TrimWhitespace(std::string& str) {
		const std::string delimiters = " \f\n\r\t\v";
		str.erase(str.find_last_not_of(delimiters) + 1);
		str.erase(0, str.find_first_not_of(delimiters));
	}

	//linear interpolation from one numeric range to another, this takes any numeric type as input
	template <typename T>
	inline static constexpr T NumericMap(T input, T inputMin, T inputMax, T outputMin, T outputMax) {
		static_assert(std::is_arithmetic<T>::value, "type must be numeric");
		return outputMin + ((outputMax - outputMin) / (inputMax - inputMin)) * (input - inputMin);
	}
};

}
