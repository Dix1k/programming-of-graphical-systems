#include <GL/glut.h>
#include <stdio.h>
#include <math.h>

GLuint texture;
float legAngle = 0.0f;    // угол для движения лап
float tailAngle = 0.0f;   // угол для хвоста (для синуса)
float tailSwing = 0.0f;   // текущий угол хвоста
float posX = -5.0f;       // позиция собаки по X

GLuint loadTexture(const char* filename)
{
    FILE* file;
    if (fopen_s(&file, filename, "rb") != 0)
    {
        printf("ERROR: texture not found\n");
        return 0;
    }

    unsigned char header[54];
    fread(header, 1, 54, file);

    unsigned int width = *(int*)&header[18];
    unsigned int height = *(int*)&header[22];
    unsigned int imageSize = width * height * 3;

    unsigned char* data = new unsigned char[imageSize];
    fread(data, 1, imageSize, file);
    fclose(file);

    GLuint tex;
    glGenTextures(1, &tex);
    glBindTexture(GL_TEXTURE_2D, tex);

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB,
        width, height, 0,
        GL_BGR_EXT, GL_UNSIGNED_BYTE, data);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    delete[] data;
    return tex;
}

// КУБ
void drawTexturedCube(float size)
{
    float s = size / 2.0f;

    glBegin(GL_QUADS);

    // Перед
    glNormal3f(0, 0, 1);
    glTexCoord2f(0, 0); glVertex3f(-s, -s, s);
    glTexCoord2f(1, 0); glVertex3f(s, -s, s);
    glTexCoord2f(1, 1); glVertex3f(s, s, s);
    glTexCoord2f(0, 1); glVertex3f(-s, s, s);

    // Зад
    glNormal3f(0, 0, -1);
    glTexCoord2f(0, 0); glVertex3f(-s, -s, -s);
    glTexCoord2f(1, 0); glVertex3f(s, -s, -s);
    glTexCoord2f(1, 1); glVertex3f(s, s, -s);
    glTexCoord2f(0, 1); glVertex3f(-s, s, -s);

    // Верх
    glNormal3f(0, 1, 0);
    glTexCoord2f(0, 0); glVertex3f(-s, s, -s);
    glTexCoord2f(1, 0); glVertex3f(s, s, -s);
    glTexCoord2f(1, 1); glVertex3f(s, s, s);
    glTexCoord2f(0, 1); glVertex3f(-s, s, s);

    // Низ
    glNormal3f(0, -1, 0);
    glTexCoord2f(0, 0); glVertex3f(-s, -s, -s);
    glTexCoord2f(1, 0); glVertex3f(s, -s, -s);
    glTexCoord2f(1, 1); glVertex3f(s, -s, s);
    glTexCoord2f(0, 1); glVertex3f(-s, -s, s);

    // Правая
    glNormal3f(1, 0, 0);
    glTexCoord2f(0, 0); glVertex3f(s, -s, -s);
    glTexCoord2f(1, 0); glVertex3f(s, -s, s);
    glTexCoord2f(1, 1); glVertex3f(s, s, s);
    glTexCoord2f(0, 1); glVertex3f(s, s, -s);

    // Левая
    glNormal3f(-1, 0, 0);
    glTexCoord2f(0, 0); glVertex3f(-s, -s, -s);
    glTexCoord2f(1, 0); glVertex3f(-s, -s, s);
    glTexCoord2f(1, 1); glVertex3f(-s, s, s);
    glTexCoord2f(0, 1); glVertex3f(-s, s, -s);

    glEnd();
}

void init()
{
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_TEXTURE_2D);

    GLfloat light_pos[] = { 5.0f, 5.0f, 5.0f, 1.0f };
    glLightfv(GL_LIGHT0, GL_POSITION, light_pos);

    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

    texture = loadTexture("texture.bmp");

    glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
}

// СОБАКА
void drawDog()
{
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texture);
    glColor3f(1.0f, 1.0f, 1.0f);

    glPushMatrix();
    glTranslatef(posX, 0.0f, 0.0f); // движение собаки вперед

    // туловище
    glPushMatrix();
    glScalef(2.0f, 1.0f, 1.0f);
    drawTexturedCube(1.0f);
    glPopMatrix();

    // голова
    GLUquadric* quad = gluNewQuadric();
    gluQuadricTexture(quad, GL_TRUE);
    gluQuadricNormals(quad, GLU_SMOOTH);

    glPushMatrix();
    glTranslatef(1.2f, 0.8f, 0.0f);
    gluSphere(quad, 0.5, 20, 20);
    glPopMatrix();

    gluDeleteQuadric(quad);

    // лапы
    float legs[4][2] = {
        {0.7f, 0.4f}, {0.7f, -0.4f},
        {-0.7f, 0.4f}, {-0.7f, -0.4f}
    };

    float legSwingFront = sin(legAngle) * 20.0f;
    float legSwingBack = sin(legAngle + 3.1415f) * 20.0f;

    for (int i = 0; i < 4; i++)
    {
        glPushMatrix();
        glTranslatef(legs[i][0], -0.8f, legs[i][1]);
        float angle = (i < 2) ? legSwingFront : legSwingBack;
        glRotatef(angle, 0, 0, 1);
        glScalef(0.2f, 1.0f, 0.2f);
        drawTexturedCube(1.0f);
        glPopMatrix();
    }

    // хвост
    glPushMatrix();
    glTranslatef(-1.1f, 0.5f, 0.0f);
    glRotatef(-45.0f + tailSwing, 0.0f, 0.0f, 1.0f);
    glScalef(0.5f, 0.15f, 0.15f);
    drawTexturedCube(1.0f);
    glPopMatrix();

    glPopMatrix();
}

void display()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    gluLookAt(10.0, 5.0, 10.0,
        0.0, 0.0, 0.0,
        0.0, 1.0, 0.0);

    drawDog();

    glutSwapBuffers();
}

void reshape(int w, int h)
{
    glViewport(0, 0, w, h);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, (double)w / (double)h, 1.0, 50.0);

    glMatrixMode(GL_MODELVIEW);
}

void timer(int)
{
    legAngle += 0.1f;
    tailAngle += 0.05f;

    tailSwing = sin(tailAngle) * 15.0f;

    posX += 0.02f;
    if (posX > 5.0f) posX = -5.0f;

    glutPostRedisplay();
    glutTimerFunc(16, timer, 0);
}

int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800, 600);
    glutCreateWindow("Лабораторная №4");

    init();
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutTimerFunc(0, timer, 0);

    glutMainLoop();
    return 0;
}