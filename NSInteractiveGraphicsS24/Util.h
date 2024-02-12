#pragma once

#include <string>

namespace Util {

	inline void Trim(std::string& str)
	{
		const std::string delimiters = " \f\n\r\t\v";
		str.erase(str.find_last_not_of(delimiters) + 1);
		str.erase(0, str.find_first_not_of(delimiters));
	}
}
