#pragma once
#include "GameObject.h"
#include "Ray.h"
#include "Util.h"

//forward decl so that we can resolve a cyclical dependency
class GameObject;

struct BehaviorParams {
	Ray* ray;
};
struct HighlightParams : BehaviorParams {
	//Ray* ray;
};
struct TranslationParams : BehaviorParams {
	glm::vec3 pointA;
	glm::vec3 pointB;
	float progress;
	int direction = 1;
	float speed;
};
struct RotationParams : BehaviorParams {
	glm::vec3 rotationAxis;
	float angularVelocity;
};

//abstract class from which to derive all other behaviors
class Behavior {
protected:
	std::shared_ptr<GameObject> object;
	static bool clicked;
public:
	inline const std::shared_ptr<GameObject>& GetObject() const { return object; }
	inline void SetObject(const std::shared_ptr<GameObject>& object) { this->object = object; }
	
	inline virtual void StoreDefaults() = 0; //any class with at least one virtual method set to 0 is abstract
	inline virtual void SetParameter(BehaviorParams& params) = 0;
	inline virtual void Update(double elapsedSeconds) = 0;

	static void SetClickState(bool clicked);
};


class HighlightBehavior : public Behavior {
protected:
	HighlightParams params;
	float ambientIntensity = 0.0f;
public:
	void StoreDefaults();
	void SetParameter(BehaviorParams& params);
	void Update(double elapsedSeconds);
};


class TranslateAnimation : public Behavior {
private:
	TranslationParams params;
	bool go = false;
public:
	void StoreDefaults();
	void SetParameter(BehaviorParams& params);
	void Update(double elapsedSeconds);
};

class RotateAnimation : public Behavior {
private:
	RotationParams params;
public:
	void StoreDefaults();
	void SetParameter(BehaviorParams& params);
	void Update(double elapsedSeconds);
};
