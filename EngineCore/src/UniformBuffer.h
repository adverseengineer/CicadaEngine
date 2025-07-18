#pragma once

#include "Shader.h"
#include "Buffer.h"

namespace Cicada {

constexpr static size_t N = 4;

enum class Type : unsigned char {
	SCALAR = 0,
	VEC2,
	VEC3,
	VEC4,
	ARRAY,
	STRUCT,
	INVALID //TODO: add support for every type
};

template <Type type>
struct Element {

	Type type;
	unsigned int baseAlignment;
	unsigned int length;
	std::vector<Element> list;

	std::string TypeStr() {
		switch (T) {
			case Type::SCALAR: return "scalar";
			case Type::VEC2: return "vec2";
			case Type::VEC3: return "vec3";
			case Type::VEC4: return "vec4";
			case Type::ARRAY: return "array<" + list[0].TypeStr() + ">";
			case Type::STRUCT: return "struct";
			default: return "invalid";
		}
	}
};
typedef struct Element Element;

class UniformBuffer : public BufferObject {

private:
	Element root;
	unsigned int calculatedSize;
	GLuint bindingPoint;

public:
	UniformBuffer(GLuint bindingPoint) :
		BufferObject(GL_UNIFORM_BUFFER),
		//root(Element::NewStruct({})),
		calculatedSize(0),
		bindingPoint(bindingPoint) {}

	UniformBuffer(GLuint bindingPoint, const std::vector<Element>& elements) :
		BufferObject(GL_UNIFORM_BUFFER),
		//root(Element::NewStruct(elements)),
		calculatedSize(0),
		bindingPoint(bindingPoint) {}

	void Attach(const Shader& shader, std::string_view blockName) const {
		GLuint shaderProg = shader.GetShaderProg();
		GLuint blockIndex = glGetUniformBlockIndex(shaderProg, blockName.data());
		glUniformBlockBinding(shaderProg, blockIndex, bindingPoint);
	}

	unsigned int CalculateSize() const {
		return 0;//root.CalculatePaddedSize();
	}

	void AddElement(const Element& element) {
		root.list.push_back(element);
		//the base alignment of a UBO is the max of all its sub elements
		if (element.baseAlignment > root.baseAlignment)
			root.baseAlignment = element.baseAlignment;
		//TODO: why do we need this? just query the size of list
		root.length++;
	}

	//TODO: the rest of the shit
};

}