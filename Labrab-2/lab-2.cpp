#include <GL/glut.h>

void init()
{
    glEnable(GL_DEPTH_TEST);      // Включаем тест глубины
    glEnable(GL_LIGHTING);        // Включаем освещение
    glEnable(GL_LIGHT0);          // Включаем источник света
    glEnable(GL_COLOR_MATERIAL);  // Разрешаем использование цвета материала

    // Задаём позицию источника света
    GLfloat light_pos[] = { 5.0, 5.0, 5.0, 1.0 };
    glLightfv(GL_LIGHT0, GL_POSITION, light_pos);

    glClearColor(0.2, 0.2, 0.2, 1.0); // Цвет фона окна
}

void drawDog()
{
    // Туловище
    glColor3f(0.6f, 0.2f, 0.4f);
    glPushMatrix();
    glScalef(2.0, 1.0, 1.0);
    glutSolidCube(1.0);
    glPopMatrix();

    //  Голова
    glColor3f(0.7f, 0.4f, 0.1f);
    glPushMatrix();
    glTranslatef(1.2, 0.8, 0.0);
    glutSolidSphere(0.5, 20, 20);
    glPopMatrix();

    //  Лапы
    glColor3f(0.8f, 0.6f, 0.4f);
    float legs[4][2] = { {0.7, 0.4}, {0.7, -0.4}, {-0.7, 0.4}, {-0.7, -0.4} };
    for (int i = 0; i < 4; i++)
    {
        glPushMatrix();
        glTranslatef(legs[i][0], -0.8, legs[i][1]);
        glScalef(0.2, 1.0, 0.2);
        glutSolidCube(1.0);
        glPopMatrix();
    }

    // Хвост
    glColor3f(0.4f, 0.2f, 0.0f);
    glPushMatrix();
    glTranslatef(-1.1, 0.5, 0.0);
    glRotatef(-45, 0, 0, 1);
    glScalef(0.5, 0.15, 0.15);
    glutSolidCube(1.0);
    glPopMatrix();
}

void display()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    gluLookAt(4.0, 3.0, 5.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

    drawDog();

    glutSwapBuffers();
}

void reshape(int w, int h)
{
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, (double)w / (double)h, 1.0, 20.0);
    glMatrixMode(GL_MODELVIEW);
}

int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800, 600);
    glutCreateWindow("Lab 2");

    init();
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutMainLoop();
    return 0;
}