#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include<iostream>
#include<fstream>
#include<string>
#include<sstream>

#include "Renderer.h"
#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "Shader.h"
#include "Texture.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

int main(void)
{
    GLFWwindow* window;

    /* glfw初期化 */
    if (!glfwInit())
        return -1;

    /* glfw_openglパーション指定 */
    const char* glsl_version = "#version 130";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    /* glfwウインドウ作成 */
    window = glfwCreateWindow(1024, 768, "Learn OpenGL", NULL, NULL);
    //window = glfwCreateWindow(800, 600, "Learn OpenGL", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* GLコンテキスト生成 */
    glfwMakeContextCurrent(window);

    /* スウップチェーン間隔設定 = (fps) */
    glfwSwapInterval(1);

    /* glew初期化 */
    if (glewInit() != GLEW_OK)
    {
        glfwTerminate();
        return -1;
    }

    // glew動くか検証:OPENGLパーション出力
    std::cout << glGetString(GL_VERSION) << std::endl;

    {
        // 頂点情報
        float vertices[] = {
            -0.5f, -0.5f, 0.0f, 0.0f,  // 0
             0.5f, -0.5f, 1.0f, 0.0f,  // 1
             0.5f,  0.5f, 1.0f, 1.0f, // 2
            -0.5f,  0.5f, 0.0f, 1.0f  // 3
        };

        // インデックス情報
        unsigned int indices[] = {
            0,1,2,
            2,3,0
        };

        // IMGUIコンテキスト生成
        ImGui::CreateContext();
        //ImGuiIO& io = ImGui::GetIO(); (void)io; io設定
        ImGui::StyleColorsDark();
        // プラントフォームとレンダラー設定(glfw+opengl3)
        ImGui_ImplGlfw_InitForOpenGL(window, true);
        ImGui_ImplOpenGL3_Init(glsl_version);

        // ブレンド設定
        GLCall(glEnable(GL_BLEND))
        GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA))

        // 頂点配列生成
        VertexArray va;
        // 頂点バッファ生成
        VertexBuffer vb(vertices, 4 * 4 * sizeof(float));
        // 頂点レイアウト生成
        VertexBufferLayout layout;
        layout.Push<float>(2);
        layout.Push<float>(2);
        // 頂点配列設定
        va.AddBuffer(vb, layout);

        // インデックスバッファ生成
        IndexBuffer ib(indices, 6);

        // モデル
        glm::vec3 translate(0.0f, 1.0f, 0.0f);
        // カメラ
        glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(-2.0f, -1.0f, 0.0f));
        // 正規投影マトリックス 4:3
        glm::mat4 proj = glm::ortho(-4.0f, 4.0f, -3.0f, 3.0f, -1.0f, 1.0f);


        // シェーダ生成
        Shader shader("res/shaders/Basic.shader");
        // シェーダ使用
        shader.Bind();
        // シェーダUniform使用
        ImVec4 color = ImVec4(0.2f, 0.3f, 0.8f, 1.0f);
        shader.SetUniform4f("u_Color", color.x, color.y, color.z, color.w);

        // テクスチャ使用
        Texture texture("res/texture/awesomeface.png");
        texture.Bind(0);
        // シェーダUniform使用
        shader.SetUniform1i("u_Texture", 0);

        /* デフォルトバインド */
        va.Unbind();
        vb.Unbind();
        ib.Unbind();
        shader.Unbind();
        Renderer renderer;

        /* ウインドウループ */
        while (!glfwWindowShouldClose(window))
        {
            // レンダラークリア
            renderer.Clear();

            // IMGUIフレーム開始
            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();

            // MVPマトリックス計算
            glm::mat4 model = glm::translate(glm::mat4(1.0f), translate);
            glm::mat4 mvp = proj * view * model;

            // シェーダバインド
            shader.Bind();
            shader.SetUniform4f("u_Color", color.x, color.y, color.z, color.w);
            shader.SetUniformMat4f("u_MVP", mvp);

            // 頂点配列バインド
            va.Bind();
            // インデックスバッファバインド
            ib.Bind();
            // レンダラー描画
            renderer.Draw(va, ib, shader);

            // IMGUI処理
            {
                ImGui::Begin("Debug Menu");
                ImGui::SliderFloat3("Translation", &translate.x, 4.0, -4.0f);
                ImGui::ColorEdit3("Color", (float*)&color);
                // FPS表示
                ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
                ImGui::End();
            }

            // IMGUIレンダリング
            ImGui::Render();
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

            /* フロントバッファとバックバッファ交換 */
            glfwSwapBuffers(window);

            /* glfwイベント処理 */
            glfwPollEvents();
        }
    }

    // IMGUIクリア
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    /* glfw終了 */
    glfwTerminate();

    return 0;
}