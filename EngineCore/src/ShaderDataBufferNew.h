#pragma once

#include "Shader.h"

namespace Cicada {

constexpr static size_t N = 4;

typedef struct Element {

	enum class Type : unsigned char {
		SCALAR = 0,
		VEC2,
		VEC3,
		VEC4,
		ARRAY,
		STRUCT,
		INVALID //TODO: add support for every type
	};

	Type type;
	unsigned int baseAlignment;
	unsigned int length;
	std::vector<Element> list;

	std::string TypeStr() {
		switch (type) {
			case Type::SCALAR: return "scalar";
			case Type::VEC2: return "vec2";
			case Type::VEC3: return "vec3";
			case Type::VEC4: return "vec4";
			case Type::ARRAY: return "array<" + list[0].TypeStr() + ">";
			case Type::STRUCT: return "struct";
			default: return "invalid";
		}
	}

	static unsigned int RoundUpPow2(unsigned int val, unsigned char n) {

		unsigned int pow2n = 0b1 << n;
		unsigned int rem = val & (pow2n - 1);
		if (rem > 0)
			val += pow2n - rem;
		
		return val;
	}

	unsigned int AlignToPow2() const {
		switch (baseAlignment) {
			case 2: return 1;
			case 4: return 2;
			case 8: return 3;
			case 16: return 4;
			default: return 0;
		}
	}

	unsigned int CalculateSize() const {
		//structs and arrays are padded. all other types are not
		switch (type) {
			case Type::SCALAR:
				return N;
			case Type::VEC2:
				return N * 2;
			case Type::VEC3:
				return N * 3;
			case Type::VEC4:
				return N * 4;
			case Type::ARRAY:
			case Type::STRUCT:
				return CalculatePaddedSize();
			default:
				return 0;
		}
	}

	unsigned int CalculatePaddedSize() const {

		unsigned int offset = 0;

		switch (type) {
			case Type::ARRAY:
				return length * RoundUpPow2(list[0].CalculateSize(), AlignToPow2());
			case Type::STRUCT:
				for (const auto& e : list) {
					offset = RoundUpPow2(offset, e.AlignToPow2());
					offset += e.CalculateSize();
				}
				return offset;
			default:
				return CalculateSize();
		}
	}

	Element(Type type = Type::SCALAR) : type(type), length(0), list(0) {
		switch (type) {
			case Type::SCALAR:
				baseAlignment = N; break;
			case Type::VEC2:
				baseAlignment = N * 2; break;
			case Type::VEC3:
			case Type::VEC4:
				baseAlignment = 4 * N; break;
			default:
				baseAlignment = 0; break;
		}
	}

	static inline Element NewScalar() {
		return Element();
	}

	static inline Element NewVec(unsigned int elems) {
		switch (elems) {
			case 2:
				return Element(Type::VEC2);
			case 3:
				return Element(Type::VEC3);
			case 4:
			default:
				return Element(Type::VEC4);
		}
	}

	static inline Element NewArray(unsigned int length, Element arrayElement) {
		Element result(Type::ARRAY);
		result.length = length;
		result.list = { arrayElement };
		result.list.shrink_to_fit();

		result.baseAlignment = arrayElement.baseAlignment;
		if (arrayElement.type == Type::STRUCT)
			result.baseAlignment = RoundUpPow2(result.baseAlignment, 4);

		return result;
	}

	static inline Element NewColumnMatrix(unsigned int cols, unsigned int rows) {
		return NewArray(cols, NewVec(rows));
	}

	static inline Element NewColumnMatrixArray(unsigned int numMatrices, unsigned int cols, unsigned int rows) {
		return NewArray(numMatrices * cols, NewVec(rows));
	}

	static inline Element NewRowMatrix(unsigned int rows, unsigned int cols) {
		return NewArray(rows, NewVec(cols));
	}

	static inline Element NewRowMatrixArray(unsigned int numMatrices, unsigned int rows, unsigned int cols) {
		return NewArray(numMatrices * rows, NewVec(cols));
	}

	static inline Element NewStruct(const std::vector<Element>& subElements) {
		Element result(Type::STRUCT);
		result.list.insert(result.list.end(), subElements.begin(), subElements.end());
		result.length = result.list.size();

		//base alignment is the max of all sub-elements
		if (!subElements.empty()) {
			for (const auto& e : subElements) {
				if (e.baseAlignment > result.baseAlignment)
					result.baseAlignment = e.baseAlignment;
			}

			result.baseAlignment = RoundUpPow2(result.baseAlignment, 4);
		}

		return result;
	}
} Element;

class UniformBufferNew {

private:
	Element root;
	unsigned int calculatedSize;
	GLuint bindingPoint;

public:
	UniformBufferNew(GLuint bindingPoint) :
		//BufferObject (GL_UNIFORM_BUFFER),
		root(Element::NewStruct({})),
		calculatedSize(0),
		bindingPoint(bindingPoint) {}

	UniformBufferNew(GLuint bindingPoint, const std::vector<Element>& elements) :
		//BufferObject(GL_UNIFORM_BUFFER),
		root(Element::NewStruct(elements)),
		calculatedSize(0),
		bindingPoint(bindingPoint) {}

	void Attach(const Shader& shader, std::string_view blockName) const {
		GLuint shaderProg = shader.GetShaderProg();
		GLuint blockIndex = glGetUniformBlockIndex(shaderProg, blockName.data());
		glUniformBlockBinding(shaderProg, blockIndex, bindingPoint);
	}

	unsigned int CalculateSize() const {
		return root.CalculatePaddedSize();
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