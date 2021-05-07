#include<iostream>

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

    /* ウインドウループ */
    while (!glfwWindowShouldClose(window))
    {
        /* バッファクリア */
        glClear(GL_COLOR_BUFFER_BIT);

        /* バッファ描画開始 */
        glBegin(GL_TRIANGLES);

        glVertex2f(-0.5f, -0.5f);
        glVertex2f(0.0f, 0.5f);
        glVertex2f(0.5f, -0.5f);

        /* バッファ描画終了 */
        glEnd();

        /* フロントバッファとバックバッファ交換 */
        glfwSwapBuffers(window);

        /* glfwイベント処理 */
        glfwPollEvents();
    }

    /* glfw終了 */
    glfwTerminate();

    return 0;
}