#pragma once

#include "Util.h"

class GraphicsObject;

//abstract class from which to derive all other animations
class Animation {
protected:
	std::shared_ptr<GraphicsObject> object;
public:
	Animation(void) : object(nullptr) {};
	virtual ~Animation(void) = default;
	inline void SetObject(const std::shared_ptr<GraphicsObject>& object) { this->object = object; }
	virtual void Update(double elapsedSeconds) = 0; //any class with at least one virtual method set to 0 is abstract
};

