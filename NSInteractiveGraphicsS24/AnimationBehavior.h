#pragma once
#include "Behavior.h"
#include "Util.h"

//abstract class from which to derive all other animations
class AnimationBehavior : public Behavior {
public:
	inline AnimationBehavior() : Behavior() {};
	inline virtual ~AnimationBehavior() = default;
};
