#include <GL/glew.h>
#include <GL/freeglut.h>
#include <stdio.h>
#include <math.h>

#include "util.h"
#include "cam.h"

GLuint vao;
GLuint vbo;
GLuint ibo;
GLuint projectionLoc, viewLoc, viewPosLoc, lightPosLoc, lightColorLoc;
GLuint modelLoc;
GLuint posAttr, colorAttr, normalAttr;

Cam cam;
float lastFrame;
float deltaTime;
int theW, theH;

void setUniformLocations() {
    modelLoc = glGetUniformLocation(prog, "model");
    viewLoc = glGetUniformLocation(prog, "view");
    projectionLoc = glGetUniformLocation(prog, "projection");
    viewPosLoc = glGetUniformLocation(prog, "viewPos");
    lightPosLoc = glGetUniformLocation(prog, "lightPos");
    lightColorLoc = glGetUniformLocation(prog, "lightColor");

    posAttr = glGetAttribLocation(prog, "pos");
    colorAttr = glGetAttribLocation(prog, "color");
    normalAttr = glGetAttribLocation(prog, "normal");
}

void createBuffer();

void initVao();

void init() {
    createBuffer();

    GLuint shaders[] = {
            createShader("../vshader.glsl", GL_VERTEX_SHADER),
            createShader("../fshader.glsl", GL_FRAGMENT_SHADER)
    };
    int len = sizeof(shaders) / sizeof(shaders[0]);
    createProg(shaders, len);
    int i = 0;
    for (; i < len; i++) {
        glDeleteShader(shaders[i]);
    }

    setUniformLocations();

    initVao();

    glEnable(GL_DEPTH_TEST);
    glDepthMask(GL_TRUE);
    glDepthFunc(GL_LEQUAL);
    glDepthRange(0.0f, 1.0f);

    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CW);
}

void createBuffer() {
    GLushort indices[] = {
            // front plane
            0, 1, 2,
            0, 2, 3,
            // top plane
            4, 5, 6,
            4, 6, 7,
            // bottom plane
            8, 9, 10,
            8, 10, 11,
            // back plane
            12, 13, 14,
            12, 14, 15,
            // left plane
            16, 17, 18,
            16, 18, 19,
            // right plane
            20, 21, 22,
            20, 22, 23
    };
    float vertices[] = {
            // 1. vertices
            // front plane:
            -0.5f, -0.5f, 0.5f,// left-bottom-front
            -0.5f, 0.5f, 0.5f,// left-top-front
            0.5f, 0.5f, 0.5f,// right-top-front
            0.5f, -0.5f, 0.5f,// right-bottom-front

            // top plane:
            -0.5f, 0.5f, 0.5f,// left-top-front
            -0.5f, 0.5f, -0.5f,// left-top-back
            0.5f, 0.5f, -0.5f,// right-top-back
            0.5f, 0.5f, 0.5f,// right-top-front888888999

            // bottom plane:
            -0.5f, -0.5f, -0.5f,// left-bottom-back
            -0.5f, -0.5f, 0.5f,// left-bottom-front
            0.5f, -0.5f, 0.5f,// right-bottom-front
            0.5f, -0.5f, -0.5f,// right-bottom-back

            // back plane:
            0.5f, -0.5f, -0.5f,// right-bottom-back
            0.5f, 0.5f, -0.5f,// right-top-back
            -0.5f, 0.5f, -0.5f,// left-top-back
            -0.5f, -0.5f, -0.5f,// left-bottom-back

            // left plane:
            -0.5f, -0.5f, -0.5f,// left-bottom-back
            -0.5f, 0.5f, -0.5f,// left-top-back
            -0.5f, 0.5f, 0.5f,// left-top-front
            -0.5f, -0.5f, 0.5f,// left-bottom-front

            // right plane:
            0.5f, -0.5f, 0.5f,// right-bottom-front
            0.5f, 0.5f, 0.5f,// right-top-front
            0.5f, 0.5f, -0.5f,// right-top-back
            0.5f, -0.5f, -0.5f,// right-bottom-back

            // 2. colors
            // front plane:
            1.0f, 0.0f, 0.0f, 1.0f,
            1.0f, 0.0f, 0.0f, 1.0f,
            1.0f, 0.0f, 0.0f, 1.0f,
            1.0f, 0.0f, 0.0f, 1.0f,

            // top plane:
            1.0f, 0.0f, 0.0f, 1.0f,
            1.0f, 0.0f, 0.0f, 1.0f,
            1.0f, 0.0f, 0.0f, 1.0f,
            1.0f, 0.0f, 0.0f, 1.0f,

            // bottom plane:
            1.0f, 0.0f, 0.0f, 1.0f,
            1.0f, 0.0f, 0.0f, 1.0f,
            1.0f, 0.0f, 0.0f, 1.0f,
            1.0f, 0.0f, 0.0f, 1.0f,

            // back plane:
            1.0f, 0.0f, 0.0f, 1.0f,
            1.0f, 0.0f, 0.0f, 1.0f,
            1.0f, 0.0f, 0.0f, 1.0f,
            1.0f, 0.0f, 0.0f, 1.0f,

            // left plane:
            1.0f, 0.0f, 0.0f, 1.0f,
            1.0f, 0.0f, 0.0f, 1.0f,
            1.0f, 0.0f, 0.0f, 1.0f,
            1.0f, 0.0f, 0.0f, 1.0f,

            // right plane:
            1.0f, 0.0f, 0.0f, 1.0f,
            1.0f, 0.0f, 0.0f, 1.0f,
            1.0f, 0.0f, 0.0f, 1.0f,
            1.0f, 0.0f, 0.0f, 1.0f,

            // 3. normals
            // front plane
            0.0f, 0.0f, 1.0f,
            0.0f, 0.0f, 1.0f,
            0.0f, 0.0f, 1.0f,
            0.0f, 0.0f, 1.0f,

            // top plane
            0.0f, 1.0f, 0.0f,
            0.0f, 1.0f, 0.0f,
            0.0f, 1.0f, 0.0f,
            0.0f, 1.0f, 0.0f,

            // bottom plane
            0.0f, -1.0f, 0.0f,
            0.0f, -1.0f, 0.0f,
            0.0f, -1.0f, 0.0f,
            0.0f, -1.0f, 0.0f,

            // back plane
            0.0f, 0.0f, -1.0f,
            0.0f, 0.0f, -1.0f,
            0.0f, 0.0f, -1.0f,
            0.0f, 0.0f, -1.0f,

            // left plane
            -1.0f, 0.0f, 0.0f,
            -1.0f, 0.0f, 0.0f,
            -1.0f, 0.0f, 0.0f,
            -1.0f, 0.0f, 0.0f,

            // right plane
            1.0f, 0.0f, 0.0f,
            1.0f, 0.0f, 0.0f,
            1.0f, 0.0f, 0.0f,
            1.0f, 0.0f, 0.0f,
    };

    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glGenBuffers(1, &ibo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void initVao() {
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glEnableVertexAttribArray(posAttr);
    glEnableVertexAttribArray(colorAttr);
    glEnableVertexAttribArray(normalAttr);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    glVertexAttribPointer(posAttr, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glVertexAttribPointer(colorAttr, 4, GL_FLOAT, GL_FALSE, 0, (void *) (6 * 4 * 3 * sizeof(float)));
    glVertexAttribPointer(normalAttr, 3, GL_FLOAT, GL_FALSE, 0, (void *) ((6 * 4 * 3 + 6 * 4 * 4) * sizeof(float)));

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);

    glBindVertexArray(0);

}

void setModel(float elapsed, mat4 dest) {
    float part = fmodf(elapsed, 20000.0f) / 25000.0f;
    float angle = M_PI * 2.0f * part;
    mRotateYMatr(angle, dest);
}

void display() {
    float elapsed = glutGet(GLUT_ELAPSED_TIME);
    deltaTime = elapsed - lastFrame;
    lastFrame = elapsed;
    camDoMovement(&cam, deltaTime, 0.005);
    glClearColor(0.33f, 0.06f, 0.68f, 0.0f);
    glClearDepth(1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glUseProgram(prog);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(vao);

    glUniform3fv(viewPosLoc, 1, cam.eye);
    glUniform3f(lightPosLoc, 5.0f, 5.0f, 0.0f);
    glUniform3f(lightColorLoc, 1.0f, 1.0f, 1.0f);

    mat4 view;
    camView(&cam, view);
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, (GLfloat *) view);

    mat4 model = MAT4_IDENTITY;
    setModel(elapsed, model);
    mat4 trans;
    mTranslateMatr(0, 0, 0, trans);
    mMat4Mul(trans, model, model);
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, (GLfloat *) model);

    glDrawElements(GL_TRIANGLES, 6 * 2 * 3, GL_UNSIGNED_SHORT, 0);

    glBindVertexArray(0);
    glUseProgram(0);
    glutSwapBuffers();
    glutPostRedisplay();
}

void reshape(int w, int h) {
    camReshape(&cam, w, h);
    theW = w;
    theH = h;
    float fovy = TO_RAD(45);
    float aspect = w / (float) h;
    float nearVal = 0.2f;
    float farVal = 1000.0f;

    mat4 projection;
    m_perspective(fovy, aspect, nearVal, farVal, projection);
    glUseProgram(prog);
    glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, (GLfloat *) projection);
    glUseProgram(0);
    glViewport(0, 0, (GLsizei) w, (GLsizei) h);
}

void keyboard(unsigned char key, int x, int y) {
    (void) x;
    (void) y;
    if (key == 27) {
        glutLeaveMainLoop();
    } else if (key == 'w') {
        cam.forwardActivated = 1;
    } else if (key == 's') {
        cam.backwardActivated = 1;
    } else if (key == 'a') {
        cam.leftActivated = 1;
    } else if (key == 'd') {
        cam.rightActivated = 1;
    }
    glutPostRedisplay();
}

void keyboardRelease(unsigned char key, int x, int y) {
    (void) x;
    (void) y;
    if (key == 27) {
        glutLeaveMainLoop();
    } else if (key == 'w') {
        cam.forwardActivated = 0;
    } else if (key == 's') {
        cam.backwardActivated = 0;
    } else if (key == 'a') {
        cam.leftActivated = 0;
    } else if (key == 'd') {
        cam.rightActivated = 0;
    }
}

void motion(int mx, int my) {
    camMotion(&cam, mx, my, theW, theH, 0.08f);
}

int main(int argc, char *argv[]) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_ALPHA | GLUT_DEPTH | GLUT_STENCIL);
    glutInitWindowSize(600, 600);
    glutInitWindowPosition(650, 200);
    glutCreateWindow("test");

    GLenum res = glewInit();
    if (res != GLEW_OK) {
        fprintf(stderr, "Error: %s\n", glewGetErrorString(res));
        return 1;
    }

    camInit(&cam);
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutSetCursor(GLUT_CURSOR_NONE);
    glutIgnoreKeyRepeat(1);
    glutKeyboardFunc(keyboard);
    glutKeyboardUpFunc(keyboardRelease);
    glutMotionFunc(motion);
    glutPassiveMotionFunc(motion);
    init();
    glBegin(GL_QUADS);
    glVertex3f(-3, -3, -3);
    glVertex3f(-3, 3, -3);
    glVertex3f(3, -3, -3);
    glVertex3f(3, 3, -3);
    glEnd();
    glutMainLoop();
    return 0;
}