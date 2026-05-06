#include <GL/glut.h>
#include <stdio.h>
#include <math.h>

GLuint texture;

float legAngle = 0.0f;
float tailAngle = 0.0f;
float tailSwing = 0.0f;
float posX = -5.0f;
float grassTime = 0.0f;

// ================= TEXTURE =================
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

// ================= CUBE =================
void drawTexturedCube(float size)
{
    float s = size / 2.0f;

    glBegin(GL_QUADS);

    glNormal3f(0, 0, 1);
    glTexCoord2f(0, 0); glVertex3f(-s, -s, s);
    glTexCoord2f(1, 0); glVertex3f(s, -s, s);
    glTexCoord2f(1, 1); glVertex3f(s, s, s);
    glTexCoord2f(0, 1); glVertex3f(-s, s, s);

    glNormal3f(0, 0, -1);
    glTexCoord2f(0, 0); glVertex3f(-s, -s, -s);
    glTexCoord2f(1, 0); glVertex3f(s, -s, -s);
    glTexCoord2f(1, 1); glVertex3f(s, s, -s);
    glTexCoord2f(0, 1); glVertex3f(-s, s, -s);

    glNormal3f(0, 1, 0);
    glTexCoord2f(0, 0); glVertex3f(-s, s, -s);
    glTexCoord2f(1, 0); glVertex3f(s, s, -s);
    glTexCoord2f(1, 1); glVertex3f(s, s, s);
    glTexCoord2f(0, 1); glVertex3f(-s, s, s);

    glNormal3f(0, -1, 0);
    glTexCoord2f(0, 0); glVertex3f(-s, -s, -s);
    glTexCoord2f(1, 0); glVertex3f(s, -s, -s);
    glTexCoord2f(1, 1); glVertex3f(s, -s, s);
    glTexCoord2f(0, 1); glVertex3f(-s, -s, s);

    glNormal3f(1, 0, 0);
    glTexCoord2f(0, 0); glVertex3f(s, -s, -s);
    glTexCoord2f(1, 0); glVertex3f(s, -s, s);
    glTexCoord2f(1, 1); glVertex3f(s, s, s);
    glTexCoord2f(0, 1); glVertex3f(s, s, -s);

    glNormal3f(-1, 0, 0);
    glTexCoord2f(0, 0); glVertex3f(-s, -s, -s);
    glTexCoord2f(1, 0); glVertex3f(-s, -s, s);
    glTexCoord2f(1, 1); glVertex3f(-s, s, s);
    glTexCoord2f(0, 1); glVertex3f(-s, s, -s);

    glEnd();
}

// ================= GRASS =================
void drawGrass(float x, float z)
{
    glPushMatrix();
    glTranslatef(x, -1.0f, z); // ВАЖНО: трава строго на земле

    float sway = sin(grassTime + x * 0.3f + z * 0.3f) * 0.1f;

    glColor3f(0.1f, 0.8f, 0.2f);

    glBegin(GL_LINES);
    for (int i = 0; i < 5; i++)
    {
        float offset = (i - 2) * 0.02f;

        glVertex3f(offset, 0.0f, 0.0f);
        glVertex3f(offset + sway, 0.3f, 0.0f);
    }
    glEnd();

    glPopMatrix();
}

// ================= GROUND =================
void drawGround()
{
    glDisable(GL_TEXTURE_2D);

    glColor3f(0.2f, 0.6f, 0.2f);

    glBegin(GL_QUADS);
    glVertex3f(-50, -1, -50);
    glVertex3f(50, -1, -50);
    glVertex3f(50, -1, 50);
    glVertex3f(-50, -1, 50);
    glEnd();

    // трава рисуется ПОСЛЕ земли, но ДО собаки
    for (int i = -20; i <= 20; i += 2)
    {
        for (int j = -20; j <= 20; j += 2)
        {
            if ((i * j) % 11 == 0)
                drawGrass(i, j);
        }
    }

    glEnable(GL_TEXTURE_2D);
}

// ================= DOG =================
void drawDog()
{
    glBindTexture(GL_TEXTURE_2D, texture);
    glColor3f(1, 1, 1);

    glPushMatrix();
    glTranslatef(posX, 0, 0);

    glPushMatrix();
    glScalef(2, 1, 1);
    drawTexturedCube(1);
    glPopMatrix();

    GLUquadric* quad = gluNewQuadric();
    gluQuadricTexture(quad, GL_TRUE);

    glPushMatrix();
    glTranslatef(1.2f, 0.8f, 0);
    gluSphere(quad, 0.5, 20, 20);
    glPopMatrix();

    gluDeleteQuadric(quad);

    float legs[4][2] = {
        {0.7f, 0.4f}, {0.7f, -0.4f},
        {-0.7f, 0.4f}, {-0.7f, -0.4f}
    };

    float swingA = sin(legAngle) * 20;
    float swingB = sin(legAngle + 3.14f) * 20;

    for (int i = 0; i < 4; i++)
    {
        glPushMatrix();
        glTranslatef(legs[i][0], -0.8f, legs[i][1]);
        glRotatef((i < 2 ? swingA : swingB), 0, 0, 1);
        glScalef(0.2f, 1, 0.2f);
        drawTexturedCube(1);
        glPopMatrix();
    }

    glPushMatrix();
    glTranslatef(-1.1f, 0.5f, 0);
    glRotatef(-45 + tailSwing, 0, 0, 1);
    glScalef(0.5f, 0.15f, 0.15f);
    drawTexturedCube(1);
    glPopMatrix();

    glPopMatrix();
}

// ================= INIT =================
void init()
{
    glEnable(GL_DEPTH_TEST);   // 🔥 КЛЮЧЕВО: убирает "проход сквозь"
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_TEXTURE_2D);

    GLfloat light_pos[] = { 5, 5, 5, 1 };
    glLightfv(GL_LIGHT0, GL_POSITION, light_pos);

    texture = loadTexture("texture.bmp");

    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

    glClearColor(0.5f, 0.8f, 1.0f, 1.0f);
}

// ================= DISPLAY =================
void display()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    gluLookAt(10, 5, 10,
        0, 0, 0,
        0, 1, 0);

    drawGround(); // сначала земля + трава
    drawDog();    // потом собака (поверх)

    glutSwapBuffers();
}

// ================= RESHAPE =================
void reshape(int w, int h)
{
    glViewport(0, 0, w, h);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45, (double)w / h, 1, 50);

    glMatrixMode(GL_MODELVIEW);
}

// ================= TIMER =================
void timer(int)
{
    legAngle += 0.1f;
    tailAngle += 0.05f;
    tailSwing = sin(tailAngle) * 15;

    posX += 0.02f;
    if (posX > 5) posX = -5;

    grassTime += 0.05f;

    glutPostRedisplay();
    glutTimerFunc(16, timer, 0);
}

// ================= MAIN =================
int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800, 600);
    glutCreateWindow("Dog Run Scene");

    init();

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutTimerFunc(0, timer, 0);

    glutMainLoop();
    return 0;
}