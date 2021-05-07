#include<iostream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

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

    /* glfw終了 */
    glfwTerminate();

    return 0;
}