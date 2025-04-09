#pragma once

class ShaderDataBuffer {
public:
    ShaderDataBuffer();
    ~ShaderDataBuffer();

    void createBuffer(GLsizeiptr size, const void* data, GLenum usage);
    void bindBuffer(GLuint bindingPoint);
    void updateBuffer(GLintptr offset, GLsizeiptr size, const void* data);
    void deleteBuffer();

private:
    GLuint bufferID;
};
