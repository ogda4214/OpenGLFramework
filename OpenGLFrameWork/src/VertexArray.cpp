#include "VertexArray.h"
#include "Renderer.h"

VertexArray::VertexArray()
{
    // ���_�z��(VAO)����
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
        // ��`���ꂽ���_������K�p(�C���f�b�N�X)
        GLCall(glEnableVertexAttribArray(i));
        // ���_�����ݒ�(VAO�Ɗ֘A)(�C���f�b�N�X�A�ϐ��^�C�v�Ɛ��ʁA�I�t�Z�b�g..)
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
