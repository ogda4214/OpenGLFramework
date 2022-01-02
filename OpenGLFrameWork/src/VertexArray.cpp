#include "VertexArray.h"
#include "Renderer.h"

VertexArray::VertexArray()
{
    // 頂点配列(VAO)生成
    GLCall(glGenVertexArrays(1, &m_RendererID))
}

VertexArray::~VertexArray()
{
    GLCall(glDeleteVertexArrays(1, &m_RendererID));
}

void VertexArray::AddBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout)
{
    Bind();
	vb.Bind();
    const auto& elements = layout.getElemests();
    unsigned int offset = 0;
    for ( unsigned int i = 0; i < elements.size(); i++)
    {
        const auto& element = elements[i];
        // 定義された頂点属性を適用(インデックス)
        GLCall(glEnableVertexAttribArray(i));
        // 頂点属性設定(VAOと関連)(インデックス、変数タイプと数量、オフセット..)
        GLCall(glVertexAttribPointer(i, element.count, element.type, element.normalized, layout.getStride(), (const void*)offset));
        offset += element.count * VertexBufferElement::getSizeOfType(element.type);
    }
}

void VertexArray::Bind() const
{
    GLCall(glBindVertexArray(m_RendererID))
}

void VertexArray::Unbind() const
{
    GLCall(glBindVertexArray(0))
}
