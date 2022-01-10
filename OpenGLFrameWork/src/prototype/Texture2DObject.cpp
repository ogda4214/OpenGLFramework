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
    // 頂点情報
    float vertices[] = {
        -50.0f, -50.0f, 0.0f, 0.0f,  // 0
         50.0f, -50.0f, 1.0f, 0.0f,  // 1
         50.0f,  50.0f, 1.0f, 1.0f, // 2
        -50.0f,  50.0f, 0.0f, 1.0f  // 3
    };

    // インデックス情報
    unsigned int indices[] = {
        0,1,2,
        2,3,0
    };

    // デフォルトブレンド設定
    GLCall(glEnable(GL_BLEND))
    GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA))

    m_Shader = std::make_unique<Shader>("res/shaders/Basic.shader");
    m_VAO = std::make_unique<VertexArray>();
    // 頂点バッファ生成
    m_VBO = std::make_unique<VertexBuffer>(vertices, 4 * 4 * sizeof(float));
    // 頂点レイアウト生成
    VertexBufferLayout layout;
    layout.Push<float>(2);
    layout.Push<float>(2);
    // 頂点配列設定
    m_VAO->AddBuffer(*m_VBO, layout);
    m_IBO = std::make_unique<IndexBuffer>(indices, 6);

    // シェーダバインド
    m_Shader->Bind();
    m_Shader->SetUniform4f("u_Color", 0.0f, 0.0f, 0.0f, 0.0f);

    // テクスチャ使用
    m_Texture = std::make_unique<Texture>("res/texture/awesomeface.png");
    // シェーダUniform使用
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

    // テクスチャバインド
    m_Texture->Bind();
    
    // MVPマトリックス計算
    {
        glm::mat4 model = glm::translate(glm::mat4(1.0f), m_TranslationA);
        glm::mat4 mvp = m_Proj * m_View * model;

        // シェーダバインド
        m_Shader->Bind();
        m_Shader->SetUniformMat4f("u_MVP", mvp);
        m_Shader->SetUniform4f("u_Color", m_ColorA[0], m_ColorA[1], m_ColorA[2], m_ColorA[3]);

        // レンダラー描画
        renderer.Draw(*m_VAO, *m_IBO, *m_Shader);
    }

    // MVPマトリックス計算
    {
        glm::mat4 model = glm::translate(glm::mat4(1.0f), m_TranslationB);
        glm::mat4 mvp = m_Proj * m_View * model;

        // シェーダバインド
        m_Shader->Bind();
        m_Shader->SetUniformMat4f("u_MVP", mvp);
        m_Shader->SetUniform4f("u_Color", m_ColorB[0], m_ColorB[1], m_ColorB[2], m_ColorB[3]);
        // レンダラー描画
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
