
#include "scripting/MathBindings.h"

static void BindVec2(sol::state& lua) {

	auto vec2MultiplyOverloads = sol::overload(
		[](const glm::vec2& v1, const glm::vec2& v2) { return v1 * v2; },
		[](const glm::vec2& v1, float value) { return v1 * value; },
		[](float value, const glm::vec2& v1) { return v1 * value; }
	);

	auto vec2DivideOverloads = sol::overload(
		[](const glm::vec2& v1, const glm::vec2& v2) { return v1 / v2; },
		[](const glm::vec2& v1, float value) { return v1 / value; },
		[](float value, const glm::vec2& v1) { return v1 / value; }
	);

	auto vec2AddOverloads = sol::overload(
		[](const glm::vec2& v1, const glm::vec2& v2) { return v1 + v2; },
		[](const glm::vec2& v1, float value) { return v1 + value; },
		[](float value, const glm::vec2& v1) { return v1 + value; }
	);

	auto vec2SubtractOverloads = sol::overload(
		[](const glm::vec2& v1, const glm::vec2& v2) { return v1 - v2; },
		[](const glm::vec2& v1, float value) { return v1 - value; },
		[](float value, const glm::vec2& v1) { return v1 - value; }
	);

	auto vec2ToString = [](const glm::vec2& v1) {
		return std::format("vec2({}, {})", v1.x, v1.y);
	};

	lua.new_usertype<glm::vec2>("vec2",
		sol::call_constructor, sol::constructors<glm::vec2(float), glm::vec2(float, float)>(),
		"x", &glm::vec2::x,
		"y", &glm::vec2::y,
		sol::meta_function::multiplication, vec2MultiplyOverloads,
		sol::meta_function::division, vec2DivideOverloads,
		sol::meta_function::addition, vec2AddOverloads,
		sol::meta_function::subtraction, vec2SubtractOverloads,
		sol::meta_function::to_string, vec2ToString,
		"length", [](const glm::vec2& v) { return glm::length(v); },
		"normalize", [](const glm::vec2& v1) { return glm::normalize(v1); }
	);
}

static void BindVec3(sol::state& lua) {

	auto vec3MultiplyOverloads = sol::overload(
		[](const glm::vec3& v1, const glm::vec3& v2) { return v1 * v2; },
		[](const glm::vec3& v1, float value) { return v1 * value; },
		[](float value, const glm::vec3& v1) { return v1 * value; }
	);

	auto vec3DivideOverloads = sol::overload(
		[](const glm::vec3& v1, const glm::vec3& v2) { return v1 / v2; },
		[](const glm::vec3& v1, float value) { return v1 / value; },
		[](float value, const glm::vec3& v1) { return v1 / value; }
	);

	auto vec3AddOverloads = sol::overload(
		[](const glm::vec3& v1, const glm::vec3& v2) { return v1 + v2; },
		[](const glm::vec3& v1, float value) { return v1 + value; },
		[](float value, const glm::vec3& v1) { return v1 + value; }
	);

	auto vec3SubtractOverloads = sol::overload(
		[](const glm::vec3& v1, const glm::vec3& v2) { return v1 - v2; },
		[](const glm::vec3& v1, float value) { return v1 - value; },
		[](float value, const glm::vec3& v1) { return v1 - value; }
	);

	auto vec3ToString = [](const glm::vec3& v1) -> std::string {
		return std::format("vec3({}, {}, {})", v1.x, v1.y, v1.z);
	};

	lua.new_usertype<glm::vec3>("vec3",
		sol::call_constructor, sol::constructors<glm::vec3(float), glm::vec3(float, float, float)>(),
		"x", &glm::vec3::x,
		"y", &glm::vec3::y,
		"z", &glm::vec3::z,
		sol::meta_function::multiplication, vec3MultiplyOverloads,
		sol::meta_function::division, vec3DivideOverloads,
		sol::meta_function::addition, vec3AddOverloads,
		sol::meta_function::subtraction, vec3SubtractOverloads,
		sol::meta_function::to_string, vec3ToString,
		"length", [](const glm::vec3& v) { return glm::length(v); },
		"normalize", [](const glm::vec3& v1) { return glm::normalize(v1); }
	);
}

static void BindVec4(sol::state& lua) {
	
	auto vec4MultiplyOverloads = sol::overload(
		[](const glm::vec4& v1, const glm::vec4& v2) { return v1 * v2; },
		[](const glm::vec4& v1, float value) { return v1 * value; },
		[](float value, const glm::vec4& v1) { return v1 * value; }
	);

	auto vec4DivideOverloads = sol::overload(
		[](const glm::vec4& v1, const glm::vec4& v2) { return v1 / v2; },
		[](const glm::vec4& v1, float value) { return v1 / value; },
		[](float value, const glm::vec4& v1) { return v1 / value; }
	);

	auto vec4AddOverloads = sol::overload(
		[](const glm::vec4& v1, const glm::vec4& v2) { return v1 + v2; },
		[](const glm::vec4& v1, float value) { return v1 + value; },
		[](float value, const glm::vec4& v1) { return v1 + value; }
	);

	auto vec4SubtractOverloads = sol::overload(
		[](const glm::vec4& v1, const glm::vec4& v2) { return v1 - v2; },
		[](const glm::vec4& v1, float value) { return v1 - value; },
		[](float value, const glm::vec4& v1) { return v1 - value; }
	);

	auto vec4ToString = [](const glm::vec4& v1) -> std::string {
		return std::format("vec4({}, {}, {}, {})", v1.x, v1.y, v1.z, v1.w);
	};

	lua.new_usertype<glm::vec4>("vec4",
		sol::call_constructor, sol::constructors<glm::vec4(float), glm::vec4(float, float, float, float)>(),
		"x", &glm::vec4::x,
		"y", &glm::vec4::y,
		"z", &glm::vec4::z,
		"w", &glm::vec4::w,
		sol::meta_function::multiplication, vec4MultiplyOverloads,
		sol::meta_function::division, vec4DivideOverloads,
		sol::meta_function::addition, vec4AddOverloads,
		sol::meta_function::subtraction, vec4SubtractOverloads,
		sol::meta_function::to_string, vec4ToString,
		"length", [](const glm::vec4& v) { return glm::length(v); },
		"normalize", [](const glm::vec4& v1) { return glm::normalize(v1); }
	);
}

static void BindMat3(sol::state& lua) {
	//TODO:
}

static void BindMat4(sol::state& lua) {
	//TODO:
}

static void BindQuat(sol::state& lua) {
	//TODO: read more about quaternions before i commit to any decisions of what operations to implement
	//UPDATE: literally just copy amulet
	lua.new_usertype<glm::quat>("quat",
		sol::call_constructor, sol::constructors<glm::quat(float, float ,float, float)>(),
		"x", &glm::quat::x,
		"y", &glm::quat::y,
		"z", &glm::quat::z,
		"w", &glm::quat::w,
		"normalize", [](const glm::quat& q) { return glm::normalize(q); },
		"conjugate", [](const glm::quat& q) { return glm::conjugate(q); },
		"cross", [](const glm::quat& q, const glm::quat& q2) { return glm::cross(q, q2); },
		"dot", [](const glm::quat& q, const glm::quat& q2) { return glm::dot(q, q2); }
	);
}

//static void BindMathFunctions(sol::state& lua) {
//
//	auto math = lua["math"]; //get the math table to extend it, or create it if not present
//	if (!math.valid()) math = lua.create_table("math");
//	//sol::table math = lua.create_table("math2");
//
//	math.set_function("cross",
//		[](const glm::vec3& v1, const glm::vec3& v2) { return glm::cross(v1, v2); }
//	);
//
//	math.set_function("dot", sol::overload(
//		[](const glm::vec2& v1, const glm::vec2& v2) { return glm::dot(v1, v2); },
//		[](const glm::vec3& v1, const glm::vec3& v2) { return glm::dot(v1, v2); },
//		[](const glm::vec4& v1, const glm::vec4& v2) { return glm::dot(v1, v2); }
//	));
//
//	math.set_function("distance", sol::overload(
//		[](glm::vec2& v1, glm::vec2& v2) { return glm::distance(v1, v2); },
//		[](glm::vec3& v1, glm::vec3& v2) { return glm::distance(v1, v2); },
//		[](glm::vec4& v1, glm::vec4& v2) { return glm::distance(v1, v2); }
//	));
//
//	math.set_function("clamp", sol::overload(
//		[](int t, int min, int max) { return std::clamp(t, min, max); },
//		[](float t, float min, float max) { return std::clamp(t, min, max); },
//		[](double t, double min, double max) { return std::clamp(t, min, max); },
//		[](glm::vec2 t, glm::vec2 min, glm::vec2 max) {
//			return glm::vec2(
//				std::clamp(t.x, min.x, max.x),
//				std::clamp(t.y, min.y, max.y)
//			);
//		},
//		[](glm::vec3 t, glm::vec3 min, glm::vec3 max) {
//			return glm::vec3(
//				std::clamp(t.x, min.x, max.x),
//				std::clamp(t.y, min.y, max.y),
//				std::clamp(t.z, min.z, max.z)
//			);
//		},
//		[](glm::vec4 t, glm::vec4 min, glm::vec4 max) {
//			return glm::vec4(
//				std::clamp(t.x, min.x, max.x),
//				std::clamp(t.y, min.y, max.y),
//				std::clamp(t.z, min.z, max.z),
//				std::clamp(t.w, min.w, max.w)
//			);
//		}
//	));
//
//	math.set_function("lerp", sol::overload(
//		[](float to, float from, float t) { return std::lerp(to, from, t); },
//		[](double to, double from, double t) { return std::lerp(to, from, t); },
//		[](glm::vec2 to, glm::vec2 from, float t) {
//			return glm::vec2(
//				std::lerp(to.x, from.x, t),
//				std::lerp(to.y, from.y, t)
//			);
//		},
//		[](glm::vec3 to, glm::vec3 from, float t) {
//			return glm::vec3(
//				std::lerp(to.x, from.x, t),
//				std::lerp(to.y, from.y, t),
//				std::lerp(to.z, from.z, t)
//			);
//		},
//		[](glm::vec4 to, glm::vec4 from, float t) {
//			return glm::vec4(
//				std::lerp(to.x, from.x, t),
//				std::lerp(to.y, from.y, t),
//				std::lerp(to.z, from.z, t),
//				std::lerp(to.w, from.w, t)
//			);
//		}
//		//TODO: do i care enough to make duplicates of all these overloads where t is a double?
//	));
//
//	math.set_function("slerp", sol::overload(
//		[](const glm::quat& to, const glm::quat& from, float t) { glm::slerp(to, from, t); }
//	));
//
//	math.set_function("map", sol::overload(
//		[](float a, float b, float c, float d, float t) {
//			return c + (t - a) * (d - c) / (b - a);
//		},
//		[](double a, double b, double c, double d, double t) {
//			return c + (t - a) * (d - c) / (b - a);
//		}
//	));
//
//	//TODO: consider making this check approximate equal using an epsilon
//	math.set_function("sign", sol::overload(
//		[](int n) { if (n == 0) return 0; else return (n > 0) ? 1 : -1; },
//		[](float n) { if (n == 0) return 0; else return (n > 0) ? 1 : -1; },
//		[](double n) { if (n == 0) return 0; else return (n > 0) ? 1 : -1; }
//	));
//
//	//TODO: decide if amulet's `fract` is worth implementing. its basically just mod, but theres different kinds of mod
//	//math.set_function("fract" sol::overload(
//	//	...
//	//));
//}

//static void BindMathConstants(sol::state& lua) {
//	//sol::table math = lua["math"]; //get the math table to extend it, or create it if not present
//	//if (!math.valid()) math = lua.create_table("math");
//	sol::table math = lua.create_table("math_ext");
//
//	math.set("double_pi", 6.28318530717958648);
//	math.set("half_pi", 1.57079632679489662);
//	math.set("quarter_pi", 0.785398163397448310);
//	math.set("pi_squared", 9.86960440108935862);
//	math.set("phi", 1.61803398874989485);
//	math.set("e", 2.71828182845904524);
//	math.set("sqrt_2", 1.41421356237309505);
//	math.set("sqrt_3", 1.73205080756887729);
//	math.set("inv_sqrt_2", 0.707106781186547524);
//	math.set("inv_sqrt_3", 0.577350269189625765);
//}

void Cicada::Scripting::CreateMathBindings(sol::state& lua) {
	BindVec2(lua);
	BindVec3(lua);
	BindVec4(lua);
	BindMat3(lua);
	BindMat4(lua);
	BindQuat(lua);
	//BindMathFunctions(lua);
	//BindMathConstants(lua);
}
