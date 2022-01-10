#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include<iostream>
#include<fstream>
#include<string>
#include<sstream>

#include "Renderer.h"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

#include "prototype/ClearColorObject.h"
#include "prototype/Texture2DObject.h"

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

        Renderer renderer;

        // プロトタイプ
        Prototype::BaseObject* obj = nullptr;
        Prototype::MenuObject* testmenu = new Prototype::MenuObject(obj);
        obj = testmenu;

        testmenu->RegisterObject<Prototype::ClearColorObject>("Clear Color");
        testmenu->RegisterObject<Prototype::Texture2DObject>("2D Texture");

        /* ウインドウループ */
        while (!glfwWindowShouldClose(window))
        {
            GLCall(glClearColor(0.0f,0.0f,0.0f,1.0f))
            // レンダラークリア
            renderer.Clear();

            // IMGUIフレーム開始
            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();

            if (obj)
            {
                obj->onUpdate(0.0f);
                obj->onRender();

                ImGui::Begin("Debug Menu");
                if (obj != testmenu && ImGui::Button("<-"))
                {
                    delete obj;
                    obj = testmenu;
                }
                obj->onImGuiRender();
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

        delete obj;
        if (obj != testmenu)
        {
            delete testmenu;
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