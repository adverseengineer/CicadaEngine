#pragma once
#include "Behavior.h"
#include "Util.h"

//abstract class from which to derive all other animations
class Animation : public Behavior {
public:
	inline Animation() : Behavior() {};
	inline virtual ~Animation() = default;
};
