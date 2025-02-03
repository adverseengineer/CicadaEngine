#pragma once

#include <glm/glm.hpp>
#include <sol/sol.hpp>

#define GLM_ENABLE_EXPERIMENTAL 1
#include <glm/gtx/norm.hpp>

namespace Cicada::Scripting {

	inline static void CreateVec2Bindings(sol::state& lua) {

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

		auto vec2ToString = [](const glm::vec2& v1) -> std::string {
			return std::format("vec2({}, {})", v1.x, v1.y);
		};

		lua.new_usertype<glm::vec2>(
			"vec2",
			sol::call_constructor,
			sol::constructors<glm::vec2(float), glm::vec2(float, float)>(),
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

	inline static void CreateVec3Bindings(sol::state& lua) {
	
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

		lua.new_usertype<glm::vec3>(
			"vec3",
			sol::call_constructor,
			sol::constructors<glm::vec3(float), glm::vec3(float, float, float)>(),
			"x", &glm::vec3::x,
			"y", &glm::vec3::y,
			"z", &glm::vec3::z,
			sol::meta_function::multiplication, vec3MultiplyOverloads,
			sol::meta_function::division, vec3DivideOverloads,
			sol::meta_function::addition, vec3AddOverloads,
			sol::meta_function::subtraction, vec3SubtractOverloads,
			sol::meta_function::to_string, vec3ToString,
			"length", [](const glm::vec3& v) { return glm::length(v); },
			"normalize", [](const glm::vec3& v1) { return glm::normalize(v1); },
			"cross", [](const glm::vec3& v1, const glm::vec3& v2) { return glm::cross(v1, v2); }
		);
	}

	inline static void CreateVec4Bindings(sol::state& lua) {
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

		lua.new_usertype<glm::vec4>(
			"vec4",
			sol::call_constructor,
			sol::constructors<glm::vec4(float), glm::vec4(float, float, float, float)>(),
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

	inline static void CreateMathBindings(sol::state& lua) {

	}
}
