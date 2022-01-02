﻿#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include<iostream>
#include<fstream>
#include<string>
#include<sstream>

#include "Renderer.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexArray.h"

struct ShaderProgramSource
{
    std::string VertexSource;
    std::string FragmengSource;
};

static ShaderProgramSource parseShader(const std::string& filepath)
{
    std::ifstream stream(filepath);

    enum class shaderType
    {
        NONE = -1,
        VERTEX,
        FRAGMENT
    };

    std::string line;
    std::stringstream ss[2];

    shaderType type = shaderType::NONE;
    // ファイル文字列解析
    while (getline(stream, line))
    {
        if (line.find("#shader") != std::string::npos)
        {
            if (line.find("vertex") != std::string::npos)
            {
                type = shaderType::VERTEX;
            }
            else  if (line.find("fragment") != std::string::npos)
            {
                type = shaderType::FRAGMENT;
            }
        }
        else
        {
            ss[static_cast<int>(type)] << line << "\n";
        }
    }

    return { ss[0].str(), ss[1].str() };
}

static unsigned int compileShader(unsigned int type, const std::string& source)
{
    unsigned int id = glCreateShader(type);
    const char* src = source.c_str();
    glShaderSource(id, 1, &src, nullptr);
    glCompileShader(id);

    int result;
    glGetShaderiv(id, GL_COMPILE_STATUS, &result);

    // エラーハンドリング
    if (result == GL_FALSE)
    {
        int length;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
        char* message = (char*)alloca(length * sizeof(char));
        glGetShaderInfoLog(id, length, &length, message);
        std::cout << "Failed to complie " << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << " shader!" << std::endl;
        std::cout << message << std::endl;
        glDeleteShader(id);
        return 0;
    }

    return id;
}

static unsigned int createShader(const std::string& vertexShader, const std::string& fragmentShader)
{
    unsigned int program = glCreateProgram();
    unsigned int vs = compileShader(GL_VERTEX_SHADER, vertexShader);
    unsigned int fs = compileShader(GL_FRAGMENT_SHADER, fragmentShader);

    // シェーダプログラムリング処理
    glAttachShader(program, vs);
    glAttachShader(program, fs);
    glLinkProgram(program);
    glValidateProgram(program);

    glDeleteShader(vs);
    glDeleteShader(fs);

    return program;
}

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
            -0.5f, -0.5f,   // 0
             0.5f, -0.5f,   // 1
             0.5f,  0.5f,   // 2
            -0.5f,  0.5f    // 3
        };

        // インデックス情報
        unsigned int indices[] = {
            0,1,2,
            2,3,0
        };

        // 頂点配列生成
        VertexArray va;
        // 頂点バッファ生成
        VertexBuffer vb(vertices, 4 * 2 * sizeof(float));
        // 頂点レイアウト生成
        VertexBufferLayout layout;
        layout.Push<float>(2);
        // 頂点配列設定
        va.AddBuffer(vb, layout);

        // インデックスバッファ生成
        IndexBuffer ib(indices, 6);

        // シェーダソースコート解析
        ShaderProgramSource source = parseShader("res/shaders/Basic.shader");
        // シェーダコンパイル
        unsigned int shader = createShader(source.VertexSource, source.FragmengSource);
        /* シェーダプログラム使用 */
        GLCall(glUseProgram(shader));

        // シェーダUniform使用
        GLCall(int location = glGetUniformLocation(shader, "u_Color"));
        ASSERT(location != -1);
        GLCall(glUniform4f(location, 0.2f, 0.3f, 0.8f, 1.0f));

        /* デフォルトバインド */
        va.Unbind();
        GLCall(glUseProgram(0));
        GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
        GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));

        float r = 0.0f;
        float increment = 0.05f;
        /* ウインドウループ */
        while (!glfwWindowShouldClose(window))
        {
            /* バッファクリア */
            glClear(GL_COLOR_BUFFER_BIT);

            GLCall(glUseProgram(shader));
            GLCall(glUniform4f(location, r, 0.3f, 0.8f, 1.0f));

            // 頂点配列バインド
            va.Bind();
            // インデックスバッファバインド
            ib.Bind();

            /* インデックスバッファ描画 */
            GLCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr));

            if (r > 1.0f)
            {
                increment = -0.05f;
            }
            else if (r < 0.0f)
            {
                increment = 0.05f;
            }

            r += increment;

            /* フロントバッファとバックバッファ交換 */
            glfwSwapBuffers(window);

            /* glfwイベント処理 */
            glfwPollEvents();
        }

        /* シェーダプログラム解放 */
        glDeleteProgram(shader);
    }

    /* glfw終了 */
    glfwTerminate();

    return 0;
}