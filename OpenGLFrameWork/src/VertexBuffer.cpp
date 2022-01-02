#include "VertexBuffer.h"
#include "Renderer.h"

VertexBuffer::VertexBuffer(const void* data, unsigned int size)
{
    // ���_�o�b�t�@(VBO)����
    GLCall(glGenBuffers(1, &m_RendererID));
    // ���_�o�b�t�@(VBO)�o�C���h
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_RendererID));
    // ���_���͒��_�o�b�t�@�փR�s�[�ƃf�[�^�����ݒ�
    GLCall(glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW));
}

VertexBuffer::~VertexBuffer()
{
    GLCall(glDeleteBuffers(1, &m_RendererID));
}

void VertexBuffer::Bind() const
{
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_RendererID));
}

void VertexBuffer::Unbind() const
{
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
}
