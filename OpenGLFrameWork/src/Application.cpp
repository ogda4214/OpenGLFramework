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

int main(void)
{
    GLFWwindow* window;

    /* glfw初期化 */
    if (!glfwInit())
        return -1;

    /* glfw_openglパーション指定 */
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    /* glfwウインドウ作成 */
    window = glfwCreateWindow(1024, 768, "Learn OpenGL", NULL, NULL);
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

        // シェーダ生成
        Shader shader("res/shaders/Basic.shader");
        // シェーダ使用
        shader.Bind();
        // シェーダUniform使用
        shader.SetUniform4f("u_Color", 0.2f, 0.3f, 0.8f, 1.0f);

        // テクスチャ使用
        Texture texture("res/texture/bg.png");
        texture.Bind(0);
        // シェーダUniform使用
        shader.SetUniform1i("u_Texture", 0);

        /* デフォルトバインド */
        va.Unbind();
        vb.Unbind();
        ib.Unbind();
        shader.Unbind();
        Renderer renderer;


        float r = 0.0f;
        float increment = 0.05f;
        /* ウインドウループ */
        while (!glfwWindowShouldClose(window))
        {
            // レンダラークリア
            renderer.Clear();
            // シェーダバインド
            shader.Bind();
            shader.SetUniform4f("u_Color", r, 0.3f, 0.8f, 1.0f);

            // 頂点配列バインド
            va.Bind();
            // インデックスバッファバインド
            ib.Bind();
            // レンダラー描画
            renderer.Draw(va, ib, shader);

            if (r > 1.0f)
            {
                increment = -0.025f;
            }
            else if (r < 0.0f)
            {
                increment = 0.025f;
            }

            r += increment;

            /* フロントバッファとバックバッファ交換 */
            glfwSwapBuffers(window);

            /* glfwイベント処理 */
            glfwPollEvents();
        }
    }

    /* glfw終了 */
    glfwTerminate();

    return 0;
}