#include<iostream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

static unsigned int compileShader(unsigned int type, const std::string& source)
{
    unsigned int id = glCreateShader(type);
    const char* src = source.c_str();
    glShaderSource(id, 1, &src, nullptr);
    glCompileShader(id);

    int result;
    glGetShaderiv(id, GL_COMPILE_STATUS, &result);

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

    /* glfwウインドウ作成 */
    window = glfwCreateWindow(1024, 768, "Learn OpenGL", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* GLコンテキスト生成 */
    glfwMakeContextCurrent(window);

    /* glew初期化 */
    if (glewInit() != GLEW_OK)
    {
        glfwTerminate();
        return -1;
    }

    // glew動くか検証:OPENGLパーション出力
    std::cout << glGetString(GL_VERSION) << std::endl;

    // 頂点情報
    float vertices[6] = {
        -0.5f, -0.5f,
        0.0f, 0.5f,
        0.5f, -0.5f
    };

    // 頂点バッファ(VBO)生成
    unsigned int vertexBuffer;
    glGenBuffers(1, &vertexBuffer);
    // 頂点バッファ(VBO)バインド
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    // 頂点情報は頂点バッファへコピーとデータ種類設定
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // 頂点属性(インデックス、変数タイプと数量、オフセット..)
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0);
    // 定義された頂点属性を適用(インデックス)
    glEnableVertexAttribArray(0);

    std::string vs =
        "#version 330 core\n"
        "\n"
        "layout(location = 0) in vec4 position;\n"
        "\n"
        "void main()\n"
        "{\n"
        "   gl_Position = position;\n"
        "}\n";

    std::string fs =
        "#version 330 core\n"
        "\n"
        "layout(location = 0) out vec4 color;\n"
        "\n"
        "void main()\n"
        "{\n"
        "   color = vec4(1.0, 0.0, 0.0, 1.0);\n"
        "}\n";

    // シェーダコンパイル
    unsigned int shader = createShader(vs,fs);
    // シェーダプログラム使用
    glUseProgram(shader);

    /* ウインドウループ */
    while (!glfwWindowShouldClose(window))
    {
        /* バッファクリア */
        glClear(GL_COLOR_BUFFER_BIT);

        /* 頂点バッファ描画 */
        glDrawArrays(GL_TRIANGLES, 0, 3);

        /* フロントバッファとバックバッファ交換 */
        glfwSwapBuffers(window);

        /* glfwイベント処理 */
        glfwPollEvents();
    }

    // シェーダ解放
    glDeleteShader(shader);
    /* glfw終了 */
    glfwTerminate();

    return 0;
}