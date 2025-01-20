#include "Util.h"
#include <glad/glad.h>

std::unordered_map<unsigned int, const std::string> Util::s_typeNameMap = {
	{ GL_BYTE, "byte" },
	{ GL_UNSIGNED_BYTE, "ubyte" },
	{ GL_SHORT, "short"},
	{ GL_UNSIGNED_SHORT, "ushort" },
	{ GL_INT, "int" },
	{ GL_UNSIGNED_INT, "uint" },
	{ GL_FLOAT, "float" },
	{ GL_DOUBLE, "double" },
	{ GL_FLOAT_VEC2, "vec2f" },
	{ GL_FLOAT_VEC3, "vec3f" },
	{ GL_FLOAT_VEC4, "vec4f" },
	{ GL_FLOAT_MAT3, "mat3f" },
	{ GL_FLOAT_MAT4, "mat4f" },
	{ GL_SAMPLER_2D, "sampler2D"},
	{ GL_VERTEX_SHADER, "vertexShader" },
	{ GL_FRAGMENT_SHADER, "fragShader" }
};

const std::string Util::TypeStr(unsigned int glType) {
	if (s_typeNameMap.contains(glType))
		return s_typeNameMap.at(glType);
	else
		return std::string("invalid");
}