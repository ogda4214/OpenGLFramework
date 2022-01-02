#include "VertexBuffer.h"
#include "Renderer.h"

VertexBuffer::VertexBuffer(const void* data, unsigned int size)
{
    // 頂点バッファ(VBO)生成
    GLCall(glGenBuffers(1, &m_RendererID));
    // 頂点バッファ(VBO)バインド
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_RendererID));
    // 頂点情報は頂点バッファへコピーとデータ扱い設定
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
