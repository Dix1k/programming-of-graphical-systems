#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <cmath>

const char* vertexShader = "#version 330 core\n"
"layout(location=0)in vec2 p;\n"
"uniform float aspect;\n"
"void main(){\n"
"   vec2 pos = p;\n"
"   pos.x *= aspect;\n"
"   gl_Position=vec4(pos,0,1);\n"
"}";

const char* fragmentShader = "#version 330 core\n"
"out vec4 c;\n"
"uniform vec3 u;\n"
"void main(){c=vec4(u,1);}";

int main() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* w = glfwCreateWindow(1024, 768, "Лаб №1", NULL, NULL);
    glfwMakeContextCurrent(w);
    gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

    // Вычисляем аспектное соотношение
    int width, height;
    glfwGetFramebufferSize(w, &width, &height);
    float aspect = (float)height / (float)width;  // Для исправления искажения

    unsigned int vs = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vs, 1, &vertexShader, NULL);
    glCompileShader(vs);

    unsigned int fs = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fs, 1, &fragmentShader, NULL);
    glCompileShader(fs);

    unsigned int prog = glCreateProgram();
    glAttachShader(prog, vs);
    glAttachShader(prog, fs);
    glLinkProgram(prog);

    glDeleteShader(vs);
    glDeleteShader(fs);

    // Прямоугольник (по точкам)
    float rectVert[] = {
        -0.8f, -0.6f,    // левая нижняя
         0.8f, -0.6f,    // правая нижняя
         0.8f,  0.6f,    // правая верхняя
        -0.8f,  0.6f     // левая верхняя
    };
    unsigned int rectInd[] = { 0,1,2, 0,2,3 };

    // Круг
    const int SEG = 64;
    float circleVert[SEG * 2];
    unsigned int circleInd[SEG * 3];

    for (int i = 0; i < SEG; i++) {
        float a = i * (2.0f * 3.14159f) / SEG;
        // Рисуем круг
        circleVert[i * 2] = cos(a) * 0.5f;
        circleVert[i * 2 + 1] = sin(a) * 0.5f+0.5f;
    }

    for (int i = 0; i < SEG; i++) {
        circleInd[i * 3] = 0;
        circleInd[i * 3 + 1] = i;
        circleInd[i * 3 + 2] = (i + 1) % SEG;
    }

    // Треугольник (по точкам)
    float triVert[] = {
        -0.6f, -0.8f,    // левая нижняя
         0.6f, -0.8f,    // правая нижняя
         0.0f,  0.5f     // верхняя
    };
    unsigned int triInd[] = { 0,1,2 };

    unsigned int VAO[3], VBO[3], EBO[3];
    glGenVertexArrays(3, VAO);
    glGenBuffers(3, VBO);
    glGenBuffers(3, EBO);

    // Прямоугольник
    glBindVertexArray(VAO[0]);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(rectVert), rectVert, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO[0]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(rectInd), rectInd, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(0);

    // Круг
    glBindVertexArray(VAO[1]);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(circleVert), circleVert, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO[1]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(circleInd), circleInd, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(0);

    // Треугольник
    glBindVertexArray(VAO[2]);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[2]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(triVert), triVert, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO[2]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(triInd), triInd, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(0);

    glUseProgram(prog);
    int colorLoc = glGetUniformLocation(prog, "u");
    int aspectLoc = glGetUniformLocation(prog, "aspect");

    while (!glfwWindowShouldClose(w)) {
        glClearColor(0.2f, 0.2f, 0.2f, 1);
        glClear(GL_COLOR_BUFFER_BIT);

        // Передаем аспект в шейдер
        glUniform1f(aspectLoc, aspect);

        // Рисуем фигуры
        glUniform3f(colorLoc, 0, 0, 1); // синий
        glBindVertexArray(VAO[0]);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        glUniform3f(colorLoc, 0, 1, 0); // зеленый
        glBindVertexArray(VAO[1]);
        glDrawElements(GL_TRIANGLES, SEG * 3, GL_UNSIGNED_INT, 0);

        glUniform3f(colorLoc, 1, 0, 0); // красный
        glBindVertexArray(VAO[2]);
        glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);

        glfwSwapBuffers(w);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}