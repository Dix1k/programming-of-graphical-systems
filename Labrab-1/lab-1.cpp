#include <GL/freeglut.h>
#include <cmath>

void display()
{
    glClear(GL_COLOR_BUFFER_BIT);

    // Прямоугольник
    glColor3f(0.0f, 0.0f, 1.0f);
    glRectf(-0.6f, -0.4f, 0.6f, 0.4f);

    // Круг
    glColor3f(0.0f, 1.0f, 0.0f);
    glBegin(GL_POLYGON);
    for (int i = 0; i < 40; i++)
    {
        float t = 6.28f * i / 40;
        glVertex2f(0.3f * cos(t), 0.3f + 0.3f * sin(t));
    }
    glEnd();

    // Треугольник
    glColor3f(1.0f, 0.0f, 0.0f);
    glBegin(GL_TRIANGLES);
    glVertex2f(-0.5f, -0.6f);
    glVertex2f(0.5f, -0.6f);
    glVertex2f(0.0f, 0.3f);
    glEnd();

    glutSwapBuffers();
}

int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(600, 600);
    glutCreateWindow("Лабораторная №1");
    glClearColor(1, 1, 1, 1);
    glutDisplayFunc(display);
    glutMainLoop();
    return 0;
}