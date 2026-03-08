#include <glad/glad.h>
#include <GLFW/glfw3.h>

// Размеры окна
const unsigned int WINDOW_WIDTH = 800;
const unsigned int WINDOW_HEIGHT = 600;

int main() {
    // Инициализация GLFW
    glfwInit();

    // Создание окна
    GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT,
        "Прямоугольник", NULL, NULL);
    glfwMakeContextCurrent(window);

    // Инициализация GLAD
    gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

    // Настройка 2D проекции


    // Цвет фона - черный
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    // Главный цикл
    while (!glfwWindowShouldClose(window)) {
        // Очистка экрана
        glClear(GL_COLOR_BUFFER_BIT);

        // Рисуем прямоугольник
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}