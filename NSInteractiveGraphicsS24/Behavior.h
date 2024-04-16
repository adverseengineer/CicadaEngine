#pragma once
#include "GraphicsStructures.h"
#include "Util.h"

//forward decl so that we can resolve a cyclical dependency
class GraphicsObject;

//abstract class from which to derive all other animations
class Behavior {
protected:
	std::shared_ptr<GraphicsObject> object;

public:
	inline Behavior() : object(nullptr) {};
	inline virtual ~Behavior() = default;

	inline const std::shared_ptr<GraphicsObject>& GetObject() const { return object; }
	inline void SetObject(const std::shared_ptr<GraphicsObject>& object) { this->object = object; }

	virtual void StoreDefaults() = 0; //any class with at least one virtual method set to 0 is abstract
	virtual void SetParameter(BehaviorParams& params) = 0;
	virtual void Update(double elapsedSeconds) = 0;
};
