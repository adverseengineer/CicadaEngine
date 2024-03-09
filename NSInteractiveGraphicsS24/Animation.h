#pragma once
#include "Util.h"

//forward decl so that we can resolve a cyclical dependency
class GraphicsObject;

//abstract class from which to derive all other animations
class Animation {
protected:
	std::shared_ptr<GraphicsObject> object;

public:
	inline Animation() : object(nullptr) {};
	inline virtual ~Animation() = default;

	inline const auto& GetObject() const { return object; }
	inline void SetObject(const std::shared_ptr<GraphicsObject>& object) { this->object = object; }
	
	inline virtual void Update(double elapsedSeconds) = 0; //any class with at least one virtual method set to 0 is abstract
};
