#include "Texture2DObject.h"

#include "Renderer.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "imgui/imgui.h"

Prototype::Texture2DObject::Texture2DObject() :
    m_View(glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f))),
    m_Proj(glm::ortho(0.0f, 960.0f, 0.0f, 540.0f, -1.0f, 1.0f)),
    m_ColorA(1.0f, 1.0f, 1.0f, 1.0f),
    m_ColorB(1.0f, 1.0f, 1.0f, 1.0f),
    m_TranslationA(200.0f, 200.0f, 0.0f),
    m_TranslationB(400.0f, 200.0f, 0.0f)
{
    // ���_���
    float vertices[] = {
        -50.0f, -50.0f, 0.0f, 0.0f,  // 0
         50.0f, -50.0f, 1.0f, 0.0f,  // 1
         50.0f,  50.0f, 1.0f, 1.0f, // 2
        -50.0f,  50.0f, 0.0f, 1.0f  // 3
    };

    // �C���f�b�N�X���
    unsigned int indices[] = {
        0,1,2,
        2,3,0
    };

    // �f�t�H���g�u�����h�ݒ�
    GLCall(glEnable(GL_BLEND))
    GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA))

    m_Shader = std::make_unique<Shader>("res/shaders/Basic.shader");
    m_VAO = std::make_unique<VertexArray>();
    // ���_�o�b�t�@����
    m_VBO = std::make_unique<VertexBuffer>(vertices, 4 * 4 * sizeof(float));
    // ���_���C�A�E�g����
    VertexBufferLayout layout;
    layout.Push<float>(2);
    layout.Push<float>(2);
    // ���_�z��ݒ�
    m_VAO->AddBuffer(*m_VBO, layout);
    m_IBO = std::make_unique<IndexBuffer>(indices, 6);

    // �V�F�[�_�o�C���h
    m_Shader->Bind();
    m_Shader->SetUniform4f("u_Color", 0.0f, 0.0f, 0.0f, 0.0f);

    // �e�N�X�`���g�p
    m_Texture = std::make_unique<Texture>("res/texture/awesomeface.png");
    // �V�F�[�_Uniform�g�p
    m_Shader->SetUniform1i("u_Texture", 0);
}

Prototype::Texture2DObject::~Texture2DObject()
{
}

void Prototype::Texture2DObject::onUpdate(float deltatime)
{
}

void Prototype::Texture2DObject::onRender()
{
    GLCall(glClearColor(0.0f, 0.0f, 0.0f, 1.0f))
    GLCall(glClear(GL_COLOR_BUFFER_BIT))

    Renderer renderer;

    // �e�N�X�`���o�C���h
    m_Texture->Bind();
    
    // MVP�}�g���b�N�X�v�Z
    {
        glm::mat4 model = glm::translate(glm::mat4(1.0f), m_TranslationA);
        glm::mat4 mvp = m_Proj * m_View * model;

        // �V�F�[�_�o�C���h
        m_Shader->Bind();
        m_Shader->SetUniformMat4f("u_MVP", mvp);
        m_Shader->SetUniform4f("u_Color", m_ColorA[0], m_ColorA[1], m_ColorA[2], m_ColorA[3]);

        // �����_���[�`��
        renderer.Draw(*m_VAO, *m_IBO, *m_Shader);
    }

    // MVP�}�g���b�N�X�v�Z
    {
        glm::mat4 model = glm::translate(glm::mat4(1.0f), m_TranslationB);
        glm::mat4 mvp = m_Proj * m_View * model;

        // �V�F�[�_�o�C���h
        m_Shader->Bind();
        m_Shader->SetUniformMat4f("u_MVP", mvp);
        m_Shader->SetUniform4f("u_Color", m_ColorB[0], m_ColorB[1], m_ColorB[2], m_ColorB[3]);
        // �����_���[�`��
        renderer.Draw(*m_VAO, *m_IBO, *m_Shader);
    }
}

void Prototype::Texture2DObject::onImGuiRender()
{
    ImGui::SliderFloat3("Translation A", &m_TranslationA.x, 0.0, 960.0f);
    ImGui::SliderFloat3("Translation B", &m_TranslationB.x, 0.0, 960.0f);
    ImGui::ColorEdit4("Color A", (float*)&m_ColorA);
    ImGui::ColorEdit4("Color B", (float*)&m_ColorB);
}
