#include <windows.h>
#include <GL/glut.h>
#include <string>
#include <cstring>
#include <cmath>
#include <cstdio>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <mmsystem.h>

#define PI 3.14159265358979323846

// ============================================================
//  Utility: draw a string using GLUT bitmap font
// ============================================================
void drawText(float x, float y, const char* text,
              void* font = GLUT_BITMAP_HELVETICA_12)
{
    glRasterPos2f(x, y);
    for (const char* c = text; *c != '\0'; c++)
        glutBitmapCharacter(font, *c);
}

void drawCentered(float y, const char* text, void* font, float winW)
{
    int w = glutBitmapLength(font, (const unsigned char*)text);
    float x = (winW - w) / 2.0f;
    drawText(x, y, text, font);
}

void drawLine(float x1, float y1, float x2, float y2)
{
    glBegin(GL_LINES);
        glVertex2f(x1, y1);
        glVertex2f(x2, y2);
    glEnd();
}

void drawCircle(float cx, float cy, float r, bool filled = false)
{
    int segs = 64;
    glBegin(filled ? GL_POLYGON : GL_LINE_LOOP);
    for (int i = 0; i < segs; i++) {
        float a = 2.0f * 3.14159f * i / segs;
        glVertex2f(cx + r * cosf(a), cy + r * sinf(a));
    }
    glEnd();
}

void drawLogo(float cx, float cy)
{
    glColor3f(0.0f, 0.27f, 0.55f);
    glLineWidth(2.5f);
    drawCircle(cx, cy, 38);
    glLineWidth(1.5f);
    drawCircle(cx, cy, 32);
    glColor3f(1, 1, 1);
    drawCircle(cx, cy, 29, true);
    glColor3f(0.0f, 0.27f, 0.55f);
    glLineWidth(1.0f);
    drawCircle(cx, cy, 29);
    const char* logoText = "AIUB";
    int tw = glutBitmapLength(GLUT_BITMAP_HELVETICA_12, (const unsigned char*)logoText);
    drawText(cx - tw / 2.0f, cy + 5, logoText, GLUT_BITMAP_HELVETICA_12);
    const char* estText = "Est. 1994";
    int ew = glutBitmapLength(GLUT_BITMAP_HELVETICA_10, (const unsigned char*)estText);
    drawText(cx - ew / 2.0f, cy - 8, estText, GLUT_BITMAP_HELVETICA_10);
    glLineWidth(1.0f);
}

// ============================================================
//  Members table
// ============================================================
struct Member {
    const char* name;
    const char* id;
    const char* program;
};

Member members[] = {
    { "MD Golam Saqlain",    "22-47123-1", "CSE" },
    { "MD Yusuf",            "22-47461-2", "CSE" },
    { "MD Eftikhar Mahbub",  "22-47635-2", "CSE" },
    { "Abir Fazle Rabbi",    "22-48591-3", "CSE" },
    { "MD. Farhan Shahriar", "23-52390-3", "CSE" },
};
const int NUM_MEMBERS = 5;

void drawLabelValue(float& y, float cxPage, float winW,
                    const char* label, const char* value)
{
    char buf[256];
    snprintf(buf, sizeof(buf), "%s  %s", label, value);
    int totalW = glutBitmapLength(GLUT_BITMAP_HELVETICA_12, (const unsigned char*)buf);
    float x = cxPage - totalW / 2.0f;
    glRasterPos2f(x, y);
    for (const char* c = label; *c; c++) glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, *c);
    glRasterPos2f(x + 1, y);
    for (const char* c = label; *c; c++) glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, *c);
    int lw = glutBitmapLength(GLUT_BITMAP_HELVETICA_12, (const unsigned char*)label);
    drawText(x + lw + 6, y, value, GLUT_BITMAP_HELVETICA_12);
    y += 20.0f;
}

// ============================================================
//  Window handles
// ============================================================
int proposalWindow;
int villageWindow;
int cityWindow;
int parkWindow;
int abirWindow;
int saqlainWindow;

int screenState = 0;

// ============================================================
//  Member screens
// ============================================================
void drawMemberScreen(const char* name, const char* id,
                      const char* program, float r, float g, float b)
{
    int winW = glutGet(GLUT_WINDOW_WIDTH);
    int winH = glutGet(GLUT_WINDOW_HEIGHT);
    glClearColor(r * 0.15f, g * 0.15f, b * 0.15f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, winW, 0, winH);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslatef(0, winH, 0);
    glScalef(1, -1, 1);
    float margin = 60.0f;
    float cy = winH / 2.0f;
    glColor3f(r * 0.3f, g * 0.3f, b * 0.3f);
    glBegin(GL_QUADS);
        glVertex2f(margin,        cy - 90);
        glVertex2f(winW - margin, cy - 90);
        glVertex2f(winW - margin, cy + 110);
        glVertex2f(margin,        cy + 110);
    glEnd();
    glColor3f(r, g, b);
    glLineWidth(3.0f);
    glBegin(GL_LINE_LOOP);
        glVertex2f(margin,        cy - 90);
        glVertex2f(winW - margin, cy - 90);
        glVertex2f(winW - margin, cy + 110);
        glVertex2f(margin,        cy + 110);
    glEnd();
    glLineWidth(1.0f);
    glColor3f(r, g, b);
    drawCentered(cy - 68, "Group Member", GLUT_BITMAP_HELVETICA_18, winW);
    glColor3f(r * 0.7f, g * 0.7f, b * 0.7f);
    drawLine(margin + 20, cy - 48, winW - margin - 20, cy - 48);
    glColor3f(1.0f, 1.0f, 1.0f);
    drawCentered(cy - 20, name, GLUT_BITMAP_TIMES_ROMAN_24, winW);
    char idBuf[64];
    snprintf(idBuf, sizeof(idBuf), "Student ID :  %s", id);
    glColor3f(0.85f, 0.85f, 0.85f);
    drawCentered(cy + 20, idBuf, GLUT_BITMAP_HELVETICA_12, winW);
    char pgBuf[64];
    snprintf(pgBuf, sizeof(pgBuf), "Program    :  %s", program);
    drawCentered(cy + 44, pgBuf, GLUT_BITMAP_HELVETICA_12, winW);
    glColor3f(0.5f, 0.5f, 0.5f);
    drawLine(margin, cy + 78, winW - margin, cy + 78);
    glColor3f(0.7f, 0.9f, 0.7f);
    drawCentered(cy + 95,
        "ENTER=Proposal  G=Golam  Y=Park  E=CityView  A=AbirScene  F=Farhan  ",
        GLUT_BITMAP_HELVETICA_10, winW);
    glutSwapBuffers();
}

void displayMember0() { drawMemberScreen("MD Golam Saqlain",    "22-47123-1", "CSE", 1.0f, 0.6f, 0.2f); }
void displayMember3() { drawMemberScreen("Abir Fazle Rabbi",    "22-48591-3", "CSE", 0.2f, 0.9f, 0.5f); }
void displayMember4() { drawMemberScreen("MD. Farhan Shahriar", "23-52390-3", "CSE", 0.9f, 0.3f, 0.3f); }

// ============================================================
//  Keyboard Guide screen
// ============================================================
void displayKeyGuide()
{
    int winW = glutGet(GLUT_WINDOW_WIDTH);
    int winH = glutGet(GLUT_WINDOW_HEIGHT);
    glClearColor(0.05f, 0.05f, 0.15f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, winW, 0, winH);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslatef(0, winH, 0);
    glScalef(1, -1, 1);
    float margin = 60.0f;
    glColor3f(1.0f, 0.85f, 0.2f);
    drawCentered(winH * 0.10f, "City of Hours and village view", GLUT_BITMAP_TIMES_ROMAN_24, winW);
    glColor3f(0.8f, 0.8f, 0.8f);
    drawCentered(winH * 0.20f, "AIUB  |  Computer Graphics  |  Spring 2025-2026",
                 GLUT_BITMAP_HELVETICA_12, winW);
    glColor3f(0.3f, 0.3f, 0.5f);
    drawLine(margin, winH * 0.28f, winW - margin, winH * 0.28f);
    glColor3f(0.9f, 0.9f, 0.4f);
    drawCentered(winH * 0.34f, "Keyboard Controls", GLUT_BITMAP_HELVETICA_18, winW);
    struct KeyEntry { const char* line; float r, g, b; };
    KeyEntry keys[] = {
        { "G  -->  MD Golam Saqlain (Street Scene)",  1.0f, 0.6f, 0.2f },
        { "Y  -->  MD Yusuf (Park Scene)",             0.3f, 0.8f, 1.0f },
        { "E  -->  MD Eftikhar (City View)",            0.8f, 0.3f, 0.9f },
        { "A  -->  Abir Fazle Rabbi (City Scene)",      0.2f, 0.9f, 0.5f },
        { "F  -->  MD. Farhan Shahriar (Village Scene)", 0.9f, 0.3f, 0.3f },

    };
    float ky = winH * 0.44f;
    for (int i = 0; i < 5; i++) {
        glColor3f(keys[i].r, keys[i].g, keys[i].b);
        drawCentered(ky, keys[i].line, GLUT_BITMAP_HELVETICA_18, winW);
        ky += winH * 0.08f;
    }
    glutSwapBuffers();
}

// ============================================================
//  Proposal display
// ============================================================
void displayProposal()
{
    int winW = glutGet(GLUT_WINDOW_WIDTH);
    int winH = glutGet(GLUT_WINDOW_HEIGHT);
    glClearColor(1, 1, 1, 1);
    glClear(GL_COLOR_BUFFER_BIT);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, winW, 0, winH);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslatef(0, winH, 0);
    glScalef(1, -1, 1);
    float margin  = 60.0f;
    float cxPage  = winW / 2.0f;
    float y       = 30.0f;
    drawLogo(cxPage, y + 42);
    y += 100.0f;
    glColor3f(0, 0, 0);
    drawCentered(y, "American International University - Bangladesh (AIUB)",
                 GLUT_BITMAP_HELVETICA_18, winW);
    y += 28.0f;
    glColor3f(0, 0, 0);
    glLineWidth(1.5f);
    drawLine(margin, y, winW - margin, y);
    y += 16.0f;
    glColor3f(0.8f, 0.1f, 0.1f);
    drawCentered(y + 10, "Project Proposal", GLUT_BITMAP_TIMES_ROMAN_24, winW);
    y += 34.0f;
    glColor3f(0, 0, 0);
    drawLabelValue(y, cxPage, winW, "Course Title:",   "Computer Graphics");
    drawLabelValue(y, cxPage, winW, "Course Code:",    "CSC4118");
    drawLabelValue(y, cxPage, winW, "Course Teacher:", "Mahfujur Rahman");
    drawLabelValue(y, cxPage, winW, "Section:",        "D");
    drawLabelValue(y, cxPage, winW, "Semester:",       "Spring 2025-2026");
    drawLabelValue(y, cxPage, winW, "Project Title:",  "City of Hours");
    y += 10.0f;
    glColor3f(0, 0, 0);
    glLineWidth(1.0f);
    drawLine(margin, y, winW - margin, y);
    y += 20.0f;
    glColor3f(0, 0, 0);
    drawCentered(y, "Group Members", GLUT_BITMAP_HELVETICA_18, winW);
    y += 26.0f;
    float rowH   = 22.0f;
    float tableW = winW - margin * 2.0f;
    float col1   = margin;
    float col2   = margin + tableW * 0.55f;
    float col3   = margin + tableW * 0.80f;
    float tableTop = y;
    float tableH = rowH * (NUM_MEMBERS + 1);
    glColor3f(0.85f, 0.85f, 0.85f);
    glBegin(GL_QUADS);
        glVertex2f(col1,          tableTop);
        glVertex2f(col1 + tableW, tableTop);
        glVertex2f(col1 + tableW, tableTop + rowH);
        glVertex2f(col1,          tableTop + rowH);
    glEnd();
    glColor3f(0, 0, 0);
    glLineWidth(1.0f);
    glBegin(GL_LINE_LOOP);
        glVertex2f(col1,          tableTop);
        glVertex2f(col1 + tableW, tableTop);
        glVertex2f(col1 + tableW, tableTop + tableH);
        glVertex2f(col1,          tableTop + tableH);
    glEnd();
    for (float cx : {col2, col3}) {
        glBegin(GL_LINES);
            glVertex2f(cx, tableTop);
            glVertex2f(cx, tableTop + tableH);
        glEnd();
    }
    for (int r = 1; r <= NUM_MEMBERS; r++) {
        float ry = tableTop + r * rowH;
        glBegin(GL_LINES);
            glVertex2f(col1,          ry);
            glVertex2f(col1 + tableW, ry);
        glEnd();
    }
    float textBaseline = tableTop + rowH - 7.0f;
    drawText(col1 + 8, textBaseline, "Name",       GLUT_BITMAP_HELVETICA_12);
    drawText(col2 + 8, textBaseline, "Student ID", GLUT_BITMAP_HELVETICA_12);
    drawText(col3 + 8, textBaseline, "Program",    GLUT_BITMAP_HELVETICA_12);
    for (int i = 0; i < NUM_MEMBERS; i++) {
        float ry = tableTop + (i + 1) * rowH + rowH - 7.0f;
        drawText(col1 + 8, ry, members[i].name,    GLUT_BITMAP_HELVETICA_12);
        drawText(col2 + 8, ry, members[i].id,      GLUT_BITMAP_HELVETICA_12);
        drawText(col3 + 8, ry, members[i].program, GLUT_BITMAP_HELVETICA_12);
    }
    y = tableTop + tableH + 18.0f;
    glColor3f(0.3f, 0.3f, 0.3f);
    drawLine(margin, y, winW - margin, y);
    y += 14.0f;
    glColor3f(0.2f, 0.6f, 0.2f);
    drawCentered(y, "Press  ENTER  to see Keyboard Controls",
                 GLUT_BITMAP_HELVETICA_10, winW);
    glutSwapBuffers();
}

void reshapeProposal(int w, int h)
{
    glViewport(0, 0, w, h);
    glutPostRedisplay();
}

// ============================================================
//  Village Scene globals
// ============================================================
GLfloat birdPos  = 0.0f;
GLfloat cloudPos = 0.0f;
GLfloat boatX    = 4.0f;
int rainMode = 0;
float rainDropY[200];
float rainDropX[200];
int nightMode = 0;

void initRain() {
    for (int i = 0; i < 200; i++) {
        rainDropX[i] = rand() % 1000 / 100.0f;
        rainDropY[i] = rand() % 800 / 100.0f;
    }
}

void update(int value) {
    birdPos += 0.03f;
    if (birdPos > 10.00f) birdPos = -10.00f;
    cloudPos += 0.01f;
    if (cloudPos > 12.00f) cloudPos = -12.00f;
    static float dir = 1.00f;
    boatX += 0.02f * dir;
    if (boatX >= 5.40f) dir = -1.0f;
    if (boatX <= 3.50f) dir =  1.0f;
    glutSetWindow(villageWindow);
    glutPostRedisplay();
    glutTimerFunc(30, update, 0);
}

void keyboard(unsigned char key, int x, int y) {
    if (key == 'r' || key == 'R') {
        rainMode = !rainMode;
        if (rainMode) sndPlaySound("rain.wav", SND_ASYNC | SND_LOOP);
        else          sndPlaySound(NULL, 0);
    }
    if (key == 'n' || key == 'N') nightMode = !nightMode;


    if (key == 13) {
        rainMode  = 0;
        nightMode = 0;
        screenState = 0;
        sndPlaySound(NULL, 0);
        glutSetWindow(villageWindow);
        glutHideWindow();
        glutSetWindow(proposalWindow);
        glutShowWindow();
        glutDisplayFunc(displayProposal);
        glutPostRedisplay();
        return;
    }

    glutPostRedisplay();
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    if (nightMode) glColor3f(0.05f, 0.05f, 0.2f);
    else           glColor3f(0.5f, 0.85f, 0.95f);
    glBegin(GL_QUADS);
        glVertex2f(0.00f,  4.80f);
         glVertex2f(10.00f, 4.80f);
        glVertex2f(10.00f, 8.00f);
        glVertex2f(0.00f,  8.00f);
    glEnd();
    if (nightMode) glColor3f(0.95f, 0.95f, 0.8f);
    else           glColor3f(1.0f, 1.0f, 0.0f);
    glBegin(GL_TRIANGLE_FAN);
        glVertex2f(8.50f, 7.10f);
        for (int i = 0; i <= 100; i++) {
            float angle = 2 * 3.1416f * i / 100;
            glVertex2f(8.5f + 0.5f * cos(angle), 7.10f + 0.5f * sin(angle));
        }
    glEnd();
    if (nightMode) {
        glColor3f(1.0f, 1.0f, 1.0f);
        glPointSize(2.5f);
        glBegin(GL_POINTS);
            glVertex2f(1.00f, 7.50f); glVertex2f(2.50f, 7.80f);
            glVertex2f(4.00f, 7.20f); glVertex2f(5.50f, 7.70f);
            glVertex2f(3.00f, 6.80f); glVertex2f(6.50f, 7.50f);
            glVertex2f(0.50f, 6.50f); glVertex2f(7.00f, 6.90f);
        glEnd();
    }
    if (nightMode) glColor3f(0.1f, 0.35f, 0.15f);
    else           glColor3f(0.2f, 0.6f, 0.3f);
    glBegin(GL_QUADS);
        glVertex2f(0.00f,  0.00f); glVertex2f(10.00f, 0.00f);
        glVertex2f(10.00f, 4.80f); glVertex2f(0.00f,  4.80f);
    glEnd();
    if (rainMode) {
        glColor3f(0.7f, 0.7f, 1.0f);
        glLineWidth(1.5f);
        glBegin(GL_LINES);
        for (int i = 0; i < 200; i++) {
            rainDropY[i] -= 0.08f;
            if (rainDropY[i] < 0) rainDropY[i] = 8.0f;
            glVertex2f(rainDropX[i], rainDropY[i]);
            glVertex2f(rainDropX[i] + 0.05f, rainDropY[i] - 0.25f);
        }
        glEnd();
    }
    if (nightMode) glColor3f(0.05f, 0.25f, 0.1f);
    else           glColor3f(0.1f, 0.4f, 0.2f);
    glBegin(GL_QUADS);
        glVertex2f(0.00f,  0.00f); glVertex2f(10.00f, 0.00f);
        glVertex2f(10.00f, 0.80f); glVertex2f(0.00f,  0.80f);
    glEnd();
    glColor3f(0.65f, 0.70f, 0.75f);
    glBegin(GL_TRIANGLES);
        glVertex2f(1.80f, 4.80f); glVertex2f(3.00f, 5.80f); glVertex2f(4.30f, 4.80f);
    glEnd();
    glColor3f(0.65f, 0.70f, 0.75f);
    glBegin(GL_TRIANGLES);
        glVertex2f(6.00f, 4.80f); glVertex2f(7.00f, 5.80f); glVertex2f(8.20f, 4.80f);
    glEnd();
    glColor3f(0.4f, 0.4f, 0.5f);
    glBegin(GL_TRIANGLES);
        glVertex2f(3.00f, 4.80f); glVertex2f(5.00f, 7.20f); glVertex2f(7.00f, 4.80f);
    glEnd();
    glColor3f(0.5f, 0.5f, 0.6f);
    glBegin(GL_TRIANGLES);
        glVertex2f(1.00f, 4.80f); glVertex2f(2.00f, 6.00f); glVertex2f(3.00f, 4.80f);
    glEnd();
    glColor3f(0.5f, 0.5f, 0.6f);
    glBegin(GL_TRIANGLES);
        glVertex2f(7.00f, 4.80f); glVertex2f(8.00f, 6.00f); glVertex2f(9.00f, 4.80f);
    glEnd();
    glColor3f(0.1f, 0.45f, 0.9f);
    glBegin(GL_QUADS);
        glVertex2f(3.50f, 4.80f); glVertex2f(6.00f, 4.80f); glVertex2f(7.25f, 3.60f); glVertex2f(4.75f, 3.60f);
        glVertex2f(4.75f, 3.60f); glVertex2f(7.25f, 3.60f); glVertex2f(6.00f, 2.40f); glVertex2f(3.50f, 2.40f);
        glVertex2f(3.50f, 2.40f); glVertex2f(6.00f, 2.40f); glVertex2f(7.25f, 1.20f); glVertex2f(4.75f, 1.20f);
        glVertex2f(4.75f, 1.20f); glVertex2f(7.25f, 1.20f); glVertex2f(6.00f, 0.00f); glVertex2f(3.50f, 0.00f);
    glEnd();
    glColor3f(0.05f, 0.25f, 0.7f);
    glLineWidth(3.0f);
    glBegin(GL_LINE_STRIP);
        glVertex2f(6.00f, 4.80f); glVertex2f(7.25f, 3.60f); glVertex2f(6.00f, 2.40f);
        glVertex2f(7.25f, 1.20f); glVertex2f(6.00f, 0.00f);
    glEnd();
    glBegin(GL_LINE_STRIP);
        glVertex2f(3.50f, 4.80f); glVertex2f(4.75f, 3.60f); glVertex2f(3.50f, 2.40f);
        glVertex2f(4.75f, 1.20f); glVertex2f(3.50f, 0.00f);
    glEnd();
    glPushMatrix();
    glTranslatef(boatX - 5.0f, 0.0f, 0.0f);
    glColor3f(0.4f, 0.2f, 0.0f);
    glBegin(GL_POLYGON);
        glVertex2f(5.00f, 2.50f); glVertex2f(5.20f, 2.40f); glVertex2f(5.10f, 2.30f);
        glVertex2f(5.60f, 2.30f); glVertex2f(5.70f, 2.50f); glVertex2f(5.50f, 2.40f);
    glEnd();
    glColor3f(1.0f, 1.0f, 1.0f);
    glBegin(GL_TRIANGLES);
        glVertex2f(5.20f, 2.40f); glVertex2f(5.50f, 2.40f); glVertex2f(5.40f, 2.70f);
    glEnd();
    glPopMatrix();
    // Trees
    glColor3f(0.4f, 0.2f, 0.0f);
    glBegin(GL_QUADS);
        glVertex2f(0.40f, 4.80f); glVertex2f(0.60f, 4.80f); glVertex2f(0.60f, 5.50f); glVertex2f(0.40f, 5.50f);
    glEnd();
    glColor3f(0.0f, 0.5f, 0.0f);
    glBegin(GL_TRIANGLES);
        glVertex2f(0.00f, 5.30f); glVertex2f(1.00f, 5.30f); glVertex2f(0.50f, 6.40f);
        glVertex2f(0.00f, 5.80f); glVertex2f(1.00f, 5.80f); glVertex2f(0.50f, 6.80f);
        glVertex2f(0.00f, 6.40f); glVertex2f(1.00f, 6.40f); glVertex2f(0.50f, 7.40f);
    glEnd();
    glColor3f(0.4f, 0.2f, 0.0f);
    glBegin(GL_QUADS);
        glVertex2f(1.40f, 4.80f); glVertex2f(1.60f, 4.80f); glVertex2f(1.60f, 5.50f); glVertex2f(1.40f, 5.50f);
    glEnd();
    glColor3f(0.0f, 0.5f, 0.0f);
    glBegin(GL_TRIANGLES);
        glVertex2f(1.00f, 5.30f); glVertex2f(2.00f, 5.30f); glVertex2f(1.50f, 6.40f);
        glVertex2f(1.00f, 5.80f); glVertex2f(2.00f, 5.80f); glVertex2f(1.50f, 6.80f);
    glEnd();
    glColor3f(0.4f, 0.2f, 0.0f);
    glBegin(GL_QUADS);
        glVertex2f(8.40f, 4.80f); glVertex2f(8.60f, 4.80f); glVertex2f(8.60f, 5.50f); glVertex2f(8.40f, 5.50f);
    glEnd();
    glColor3f(0.0f, 0.5f, 0.0f);
    glBegin(GL_TRIANGLES);
        glVertex2f(8.00f, 5.35f); glVertex2f(9.00f, 5.35f); glVertex2f(8.50f, 6.40f);
    glEnd();
    glColor3f(0.4f, 0.2f, 0.0f);
    glBegin(GL_QUADS);
        glVertex2f(9.40f, 4.80f); glVertex2f(9.60f, 4.80f); glVertex2f(9.60f, 5.50f); glVertex2f(9.40f, 5.50f);
    glEnd();
    glColor3f(0.0f, 0.5f, 0.0f);
    glBegin(GL_TRIANGLES);
        glVertex2f(9.00f, 5.35f); glVertex2f(10.00f, 5.35f); glVertex2f(9.50f, 6.40f);
    glEnd();
    // Houses
    glColor3f(0.9f, 0.6f, 0.3f);
    glBegin(GL_QUADS);
        glVertex2f(0.20f, 2.80f); glVertex2f(1.20f, 2.80f); glVertex2f(1.20f, 3.50f); glVertex2f(0.20f, 3.50f);
    glEnd();
    glColor3f(0.6f, 0.15f, 0.1f);
    glBegin(GL_TRIANGLES);
        glVertex2f(0.00f, 3.50f); glVertex2f(1.50f, 3.50f); glVertex2f(0.70f, 4.10f);
    glEnd();
    glColor3f(0.4f, 0.2f, 0.0f);
    glBegin(GL_QUADS);
        glVertex2f(0.50f, 2.80f); glVertex2f(0.90f, 2.80f); glVertex2f(0.90f, 3.30f); glVertex2f(0.50f, 3.30f);
    glEnd();
    glColor3f(0.9f, 0.6f, 0.3f);
    glBegin(GL_QUADS);
        glVertex2f(2.00f, 2.80f); glVertex2f(3.00f, 2.80f); glVertex2f(3.00f, 3.50f); glVertex2f(2.00f, 3.50f);
    glEnd();
    glColor3f(0.6f, 0.15f, 0.1f);
    glBegin(GL_TRIANGLES);
        glVertex2f(1.70f, 3.50f); glVertex2f(3.20f, 3.50f); glVertex2f(2.50f, 4.10f);
    glEnd();
    glColor3f(0.4f, 0.2f, 0.0f);
    glBegin(GL_QUADS);
        glVertex2f(2.30f, 2.80f); glVertex2f(2.70f, 2.80f); glVertex2f(2.70f, 3.30f); glVertex2f(2.30f, 3.30f);
    glEnd();
    glColor3f(0.9f, 0.6f, 0.3f);
    glBegin(GL_QUADS);
        glVertex2f(7.70f, 2.80f); glVertex2f(8.70f, 2.80f); glVertex2f(8.70f, 3.50f); glVertex2f(7.70f, 3.50f);
    glEnd();
    glColor3f(0.6f, 0.15f, 0.1f);
    glBegin(GL_TRIANGLES);
        glVertex2f(7.50f, 3.50f); glVertex2f(9.00f, 3.50f); glVertex2f(8.20f, 4.10f);
    glEnd();
    glColor3f(0.4f, 0.2f, 0.0f);
    glBegin(GL_QUADS);
        glVertex2f(8.00f, 2.80f); glVertex2f(8.40f, 2.80f); glVertex2f(8.40f, 3.30f); glVertex2f(8.00f, 3.30f);
    glEnd();
    glColor3f(0.9f, 0.6f, 0.3f);
    glBegin(GL_QUADS);
        glVertex2f(8.70f, 2.80f); glVertex2f(9.70f, 2.80f); glVertex2f(9.70f, 3.50f); glVertex2f(8.70f, 3.50f);
    glEnd();
    glColor3f(0.6f, 0.15f, 0.1f);
    glBegin(GL_TRIANGLES);
        glVertex2f(8.50f, 3.50f); glVertex2f(10.00f, 3.50f); glVertex2f(9.20f, 4.10f);
    glEnd();
    glColor3f(0.4f, 0.2f, 0.0f);
    glBegin(GL_QUADS);
        glVertex2f(9.00f, 2.80f); glVertex2f(9.40f, 2.80f); glVertex2f(9.40f, 3.30f); glVertex2f(9.00f, 3.30f);
    glEnd();
    // Bushes
    glColor3f(0.0f, 0.3f, 0.15f);
    glBegin(GL_TRIANGLES);
        glVertex2f(0.00f, 0.20f); glVertex2f(0.50f, 0.20f); glVertex2f(0.25f, 0.76f);
        glVertex2f(0.50f, 0.20f); glVertex2f(1.00f, 0.20f); glVertex2f(0.75f, 0.92f);
        glVertex2f(1.00f, 0.20f); glVertex2f(1.50f, 0.20f); glVertex2f(1.25f, 1.00f);
        glVertex2f(1.50f, 0.20f); glVertex2f(2.00f, 0.20f); glVertex2f(1.75f, 0.92f);
        glVertex2f(2.00f, 0.20f); glVertex2f(2.50f, 0.20f); glVertex2f(2.25f, 0.76f);
        glVertex2f(7.50f, 0.20f); glVertex2f(8.00f, 0.20f); glVertex2f(7.75f, 0.76f);
        glVertex2f(8.00f, 0.20f); glVertex2f(8.50f, 0.20f); glVertex2f(8.25f, 0.92f);
        glVertex2f(8.50f, 0.20f); glVertex2f(9.00f, 0.20f); glVertex2f(8.75f, 1.00f);
        glVertex2f(9.00f, 0.20f); glVertex2f(9.50f, 0.20f); glVertex2f(9.25f, 0.92f);
        glVertex2f(9.50f,  0.20f); glVertex2f(10.00f, 0.20f); glVertex2f(9.75f,  0.76f);
    glEnd();
    // Fire
    glColor3f(0.4f, 0.2f, 0.0f);
    glBegin(GL_QUADS);
        glVertex2f(0.80f, 1.40f); glVertex2f(1.40f, 1.40f); glVertex2f(1.40f, 1.50f); glVertex2f(0.80f, 1.50f);
    glEnd();
    glColor3f(1.0f, 0.5f, 0.0f);
    glBegin(GL_TRIANGLES);
        glVertex2f(0.90f, 1.50f); glVertex2f(1.10f, 1.80f); glVertex2f(1.30f, 1.50f);
    glEnd();
    glColor3f(1.0f, 0.2f, 0.0f);
    glBegin(GL_TRIANGLES);
        glVertex2f(0.95f, 1.50f); glVertex2f(1.10f, 1.65f); glVertex2f(1.25f, 1.50f);
    glEnd();
    glColor3f(1.0f, 1.0f, 0.0f);
    glBegin(GL_TRIANGLES);
        glVertex2f(1.00f, 1.50f); glVertex2f(1.10f, 1.55f); glVertex2f(1.20f, 1.50f);
    glEnd();
    // Birds
    glColor3f(0.0f, 0.0f, 0.0f);
    glLineWidth(2.0f);
    glPushMatrix();
    glTranslatef(birdPos, 0.0f, 0.0f);
    glBegin(GL_LINES);
        glVertex2f(3.50f, 7.20f); glVertex2f(3.60f, 7.30f);
        glVertex2f(3.60f, 7.30f); glVertex2f(3.80f, 7.20f);
        glVertex2f(4.50f, 7.40f); glVertex2f(4.60f, 7.50f);
        glVertex2f(4.60f, 7.50f); glVertex2f(4.80f, 7.40f);
        glVertex2f(5.50f, 7.20f); glVertex2f(5.60f, 7.30f);
        glVertex2f(5.60f, 7.30f); glVertex2f(5.80f, 7.20f);
    glEnd();
    glPopMatrix();
    // Clouds
    glColor3f(1.0f, 1.0f, 1.0f);
    glPushMatrix();
    glTranslatef(cloudPos, 0.0f, 0.0f);
    glBegin(GL_TRIANGLE_FAN);
        glVertex2f(2.00f, 7.00f);
        for (int i = 0; i <= 100; i++) {
            float angle = 2 * 3.1416f * i / 100;
            glVertex2f(2.0f + 0.4f * cos(angle), 7.0f + 0.3f * sin(angle));
        }
    glEnd();
    glBegin(GL_TRIANGLE_FAN);
        glVertex2f(2.40f, 7.10f);
        for (int i = 0; i <= 100; i++) {
            float angle = 2 * 3.1416f * i / 100;
            glVertex2f(2.4f + 0.4f * cos(angle), 7.1f + 0.3f * sin(angle));
        }
    glEnd();
    glBegin(GL_TRIANGLE_FAN);
        glVertex2f(1.60f, 7.10f);
        for (int i = 0; i <= 100; i++) {
            float angle = 2 * 3.1416f * i / 100;
            glVertex2f(1.6f + 0.4f * cos(angle), 7.1f + 0.3f * sin(angle));
        }
    glEnd();
    glPopMatrix();
    glPushMatrix();
    glTranslatef(cloudPos, 0.0f, 0.0f);
    glBegin(GL_TRIANGLE_FAN);
        glVertex2f(8.00f, 7.00f);
        for (int i = 0; i <= 100; i++) {
            float angle = 2 * 3.1416f * i / 100;
            glVertex2f(8.00f + 0.4f * cos(angle), 7.00f + 0.3f * sin(angle));
        }
    glEnd();
    glBegin(GL_TRIANGLE_FAN);
        glVertex2f(8.40f, 7.10f);
        for (int i = 0; i <= 100; i++) {
            float angle = 2 * 3.1416f * i / 100;
            glVertex2f(8.40f + 0.4f * cos(angle), 7.10f + 0.3f * sin(angle));
        }
    glEnd();
    glBegin(GL_TRIANGLE_FAN);
        glVertex2f(7.60f, 7.10f);
        for (int i = 0; i <= 100; i++) {
            float angle = 2 * 3.1416f * i / 100;
            glVertex2f(7.60f + 0.4f * cos(angle), 7.10f + 0.3f * sin(angle));
        }
    glEnd();
    glPopMatrix();
    glFlush();
}

// ============================================================
//  CITY VIEW globals & functions
// ============================================================
const float PI2 = 3.14159265f;

GLfloat city_cloudPos   = -0.5f;
GLfloat city_cloudSpeed = 0.005f;
GLfloat city_skyFactor  = 1.0f;
int     city_elapsedTime = 0;
GLfloat city_windowFactor = 0.0f;
GLfloat city_sunFactor    = 1.0f;
GLfloat city_cloudFactor  = 0.0f;
GLfloat city_shipPos      = -0.3f;
GLfloat city_sunY         = 0.82f;

void city_drawCircle(float cx, float cy, float r) {
    int segments = 60;
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(cx, cy);
    for (int i = 0; i <= segments; i++) {
        float angle = 2.0f * PI2 * i / segments;
        glVertex2f(cx + r * cos(angle), cy + r * sin(angle));
    }
    glEnd();
}

void city_drawRoundCircle(float cx, float cy, float r) {
    int segments = 60;
    float aspectRatio = 500.0f / 900.0f;
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(cx, cy);
    for (int i = 0; i <= segments; i++) {
        float angle = 2.0f * PI2 * i / segments;
        glVertex2f(cx + r * cos(angle) * aspectRatio, cy + r * sin(angle));
    }
    glEnd();
}

void city_update(int value) {
    city_cloudPos += city_cloudSpeed;
    if (city_cloudPos > 1.9f) city_cloudPos = -1.9f;
    city_elapsedTime += 30;
    if (city_elapsedTime >= 10000 && city_elapsedTime <= 16000)
        city_skyFactor = 1.0f - ((city_elapsedTime - 10000) / 6000.0f);
    if (city_elapsedTime > 16000) city_skyFactor = 0.0f;
    city_shipPos += 0.0008f;
    if (city_shipPos > 1.5f) city_shipPos = -1.5f;
    if (city_elapsedTime >= 13000 && city_elapsedTime <= 22000)
        city_windowFactor = (city_elapsedTime - 13000) / 9000.0f;
    if (city_elapsedTime > 22000) city_windowFactor = 1.0f;
    if (city_elapsedTime >= 10000 && city_elapsedTime <= 16000)
        city_cloudFactor = (city_elapsedTime - 10000) / 6000.0f;
    if (city_elapsedTime > 16000) city_cloudFactor = 1.0f;
    if (city_elapsedTime >= 7000)
        city_sunY = 0.82f - ((city_elapsedTime - 7000) / 10000.0f) * 1.8f;
    glutSetWindow(cityWindow);
    glutPostRedisplay();
    glutTimerFunc(30, city_update, 0);
}

void city_display() {
    glClearColor(
        0.53f * city_skyFactor,
        0.81f * city_skyFactor,
        0.90f * city_skyFactor,
        1.0f
    );
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(city_sunFactor, city_sunFactor * 0.9f, 0.0f);
    city_drawRoundCircle(0.85f, city_sunY, 0.15f);
    float cloudR = 1.0f - 0.6f * city_cloudFactor;
    float cloudG = 1.0f - 0.6f * city_cloudFactor;
    float cloudB = 1.0f - 0.6f * city_cloudFactor;
    glColor3f(cloudR, cloudG, cloudB);
    glPushMatrix();
        glTranslatef(city_cloudPos, 0.0f, 0.0f);
        city_drawCircle(-0.75f, 0.77f, 0.08f); city_drawCircle(-0.63f, 0.80f, 0.10f);
        city_drawCircle(-0.50f, 0.78f, 0.09f); city_drawCircle(-0.68f, 0.85f, 0.08f);
        city_drawCircle(-0.55f, 0.84f, 0.07f);
    glPopMatrix();
    glPushMatrix();
        glTranslatef(city_cloudPos, 0.0f, 0.0f);
        city_drawCircle( 0.05f, 0.75f, 0.09f); city_drawCircle( 0.17f, 0.78f, 0.11f);
        city_drawCircle( 0.30f, 0.76f, 0.09f); city_drawCircle( 0.13f, 0.84f, 0.08f);
        city_drawCircle( 0.25f, 0.83f, 0.07f);
    glPopMatrix();
    // LEFT BUILDING
    glColor3f(0.96f, 0.93f, 0.75f);
    glBegin(GL_QUADS);
        glVertex2f(-1.0f, -1.0f); glVertex2f(-0.70f, -1.0f);
        glVertex2f(-0.70f,  0.8f); glVertex2f(-1.0f,  0.8f);
    glEnd();
    glColor3f(0.1f*(1-city_windowFactor)+1.0f*city_windowFactor,
              0.1f*(1-city_windowFactor)+1.0f*city_windowFactor,
              0.1f*(1-city_windowFactor));
    glBegin(GL_QUADS); glVertex2f(-0.93f,  0.55f); glVertex2f(-0.77f,  0.55f); glVertex2f(-0.77f,  0.70f); glVertex2f(-0.93f,  0.70f); glEnd();
    glBegin(GL_QUADS); glVertex2f(-0.93f,  0.25f); glVertex2f(-0.77f,  0.25f); glVertex2f(-0.77f,  0.40f); glVertex2f(-0.93f,  0.40f); glEnd();
    glBegin(GL_QUADS); glVertex2f(-0.93f, -0.05f); glVertex2f(-0.77f, -0.05f); glVertex2f(-0.77f,  0.10f); glVertex2f(-0.93f,  0.10f); glEnd();
    glBegin(GL_QUADS); glVertex2f(-0.93f, -0.35f); glVertex2f(-0.77f, -0.35f); glVertex2f(-0.77f, -0.20f); glVertex2f(-0.93f, -0.20f); glEnd();
    // BUILDING 2
    glColor3f(0.98f, 0.75f, 0.10f);
    glBegin(GL_QUADS);
        glVertex2f(-0.70f, -1.0f); glVertex2f( 0.30f, -1.0f);
        glVertex2f( 0.30f,  0.20f); glVertex2f(-0.70f,  0.20f);
    glEnd();
    glColor3f(0.1f*(1-city_windowFactor)+1.0f*city_windowFactor,
              0.1f*(1-city_windowFactor)+1.0f*city_windowFactor,
              0.1f*(1-city_windowFactor));
    glBegin(GL_QUADS); glVertex2f(-0.60f, -0.30f); glVertex2f(-0.38f, -0.30f); glVertex2f(-0.38f,  0.08f); glVertex2f(-0.60f,  0.08f); glEnd();
    glBegin(GL_QUADS); glVertex2f(-0.18f, -0.30f); glVertex2f( 0.04f, -0.30f); glVertex2f( 0.04f,  0.08f); glVertex2f(-0.18f,  0.08f); glEnd();
    glBegin(GL_QUADS); glVertex2f( 0.10f, -0.30f); glVertex2f( 0.25f, -0.30f); glVertex2f( 0.25f,  0.08f); glVertex2f( 0.10f,  0.08f); glEnd();
    // RIGHT BUILDING
    glColor3f(0.70f, 0.65f, 0.85f);
    glBegin(GL_QUADS);
        glVertex2f( 0.38f, -1.0f); glVertex2f( 0.72f, -1.0f);
        glVertex2f( 0.72f,  0.85f); glVertex2f( 0.38f,  0.85f);
    glEnd();
    float wr = 0.1f*(1-city_windowFactor)+1.0f*city_windowFactor;
    float wg = 0.1f*(1-city_windowFactor)+0.9f*city_windowFactor;
    float wb = 0.1f*(1-city_windowFactor)+0.0f*city_windowFactor;
    glColor3f(wr, wg, wb);
    glBegin(GL_QUADS); glVertex2f( 0.50f,  0.0f); glVertex2f( 0.58f,  0.0f); glVertex2f( 0.58f,  0.70f); glVertex2f( 0.50f,  0.70f); glEnd();
    glColor3f(0.55f, 0.05f, 0.05f);
    glBegin(GL_QUADS); glVertex2f( 0.50f, -1.0f); glVertex2f( 0.60f, -1.0f); glVertex2f( 0.60f, -0.55f); glVertex2f( 0.50f, -0.55f); glEnd();
    glColor3f(0.96f, 0.93f, 0.75f);
    glBegin(GL_QUADS);
        glVertex2f( 0.72f, -1.0f); glVertex2f( 0.85f, -1.0f);
        glVertex2f( 0.85f,  0.10f); glVertex2f( 0.72f,  0.10f);
    glEnd();
    glColor3f(0.1f*(1-city_windowFactor)+1.0f*city_windowFactor,
              0.1f*(1-city_windowFactor)+1.0f*city_windowFactor,
              0.1f*(1-city_windowFactor));
    glBegin(GL_QUADS); glVertex2f( 0.74f, -0.10f); glVertex2f( 0.83f, -0.10f); glVertex2f( 0.83f,  0.05f); glVertex2f( 0.74f,  0.05f); glEnd();
    glBegin(GL_QUADS); glVertex2f( 0.74f, -0.40f); glVertex2f( 0.83f, -0.40f); glVertex2f( 0.83f, -0.25f); glVertex2f( 0.74f, -0.25f); glEnd();
    glColor3f(0.96f, 0.93f, 0.75f);
    glBegin(GL_QUADS);
        glVertex2f( 0.85f, -1.0f); glVertex2f( 1.0f, -1.0f);
        glVertex2f( 1.0f,  0.65f); glVertex2f( 0.85f,  0.65f);
    glEnd();
    glColor3f(0.1f, 0.1f, 0.1f);
    glBegin(GL_QUADS); glVertex2f( 0.87f,  0.35f); glVertex2f( 0.98f,  0.35f); glVertex2f( 0.98f,  0.55f); glVertex2f( 0.87f,  0.55f); glEnd();
    glBegin(GL_QUADS); glVertex2f( 0.87f,  0.05f); glVertex2f( 0.98f,  0.05f); glVertex2f( 0.98f,  0.25f); glVertex2f( 0.87f,  0.25f); glEnd();
    glBegin(GL_QUADS); glVertex2f( 0.87f, -0.25f); glVertex2f( 0.98f, -0.25f); glVertex2f( 0.98f, -0.05f); glVertex2f( 0.87f, -0.05f); glEnd();
    // GRASS
    glColor3f(0.13f, 0.65f, 0.20f);
    glBegin(GL_QUADS);
        glVertex2f(-1.0f, -1.0f); glVertex2f( 1.0f, -1.0f);
        glVertex2f( 1.0f, -0.50f); glVertex2f(-1.0f, -0.50f);
    glEnd();
    // TREES
    glColor3f(0.40f, 0.25f, 0.10f);
    glBegin(GL_QUADS);
        glVertex2f(-0.85f-0.02f, -0.70f); glVertex2f(-0.85f+0.02f, -0.70f);
        glVertex2f(-0.85f+0.02f, -0.45f); glVertex2f(-0.85f-0.02f, -0.45f);
    glEnd();
    glColor3f(0.10f, 0.55f, 0.10f);
    city_drawCircle(-0.85f,         -0.40f, 0.08f);
    city_drawCircle(-0.85f - 0.06f, -0.45f, 0.07f);
    city_drawCircle(-0.85f + 0.06f, -0.45f, 0.07f);
    glColor3f(0.40f, 0.25f, 0.10f);
    glBegin(GL_QUADS);
        glVertex2f( 0.35f-0.02f, -0.70f); glVertex2f( 0.35f+0.02f, -0.70f);
        glVertex2f( 0.35f+0.02f, -0.45f); glVertex2f( 0.35f-0.02f, -0.45f);
    glEnd();
    glColor3f(0.10f, 0.55f, 0.10f);
    city_drawCircle( 0.35f,         -0.40f, 0.08f);
    city_drawCircle( 0.35f - 0.06f, -0.45f, 0.07f);
    city_drawCircle( 0.35f + 0.06f, -0.45f, 0.07f);
    // ROAD
    glColor3f(0.25f, 0.25f, 0.25f);
    glBegin(GL_QUADS);
        glVertex2f(-1.0f, -0.72f); glVertex2f( 1.0f, -0.72f);
        glVertex2f( 1.0f, -0.52f); glVertex2f(-1.0f, -0.52f);
    glEnd();
    glColor3f(1.0f, 0.85f, 0.0f);
    glBegin(GL_QUADS); glVertex2f(-1.00f,-0.617f); glVertex2f(-0.70f,-0.617f); glVertex2f(-0.70f,-0.603f); glVertex2f(-1.00f,-0.603f); glEnd();
    glBegin(GL_QUADS); glVertex2f(-0.50f,-0.617f); glVertex2f(-0.20f,-0.617f); glVertex2f(-0.20f,-0.603f); glVertex2f(-0.50f,-0.603f); glEnd();
    glBegin(GL_QUADS); glVertex2f( 0.00f,-0.617f); glVertex2f( 0.30f,-0.617f); glVertex2f( 0.30f,-0.603f); glVertex2f( 0.00f,-0.603f); glEnd();
    glBegin(GL_QUADS); glVertex2f( 0.50f,-0.617f); glVertex2f( 0.80f,-0.617f); glVertex2f( 0.80f,-0.603f); glVertex2f( 0.50f,-0.603f); glEnd();
    // CAR
    glColor3f(0.80f, 0.10f, 0.10f);
    glBegin(GL_QUADS);
        glVertex2f(-0.30f,-0.72f); glVertex2f( 0.30f,-0.72f);
        glVertex2f( 0.30f,-0.58f); glVertex2f(-0.30f,-0.58f);
    glEnd();
    glColor3f(0.70f, 0.08f, 0.08f);
    glBegin(GL_QUADS);
        glVertex2f(-0.18f,-0.58f); glVertex2f( 0.18f,-0.58f);
        glVertex2f( 0.15f,-0.50f); glVertex2f(-0.15f,-0.50f);
    glEnd();
    glColor3f(0.55f, 0.85f, 0.95f);
    glBegin(GL_QUADS); glVertex2f( 0.10f,-0.58f); glVertex2f( 0.17f,-0.58f); glVertex2f( 0.14f,-0.51f); glVertex2f( 0.08f,-0.51f); glEnd();
    glBegin(GL_QUADS); glVertex2f(-0.17f,-0.58f); glVertex2f(-0.10f,-0.58f); glVertex2f(-0.08f,-0.51f); glVertex2f(-0.14f,-0.51f); glEnd();
    glBegin(GL_QUADS); glVertex2f(-0.08f,-0.58f); glVertex2f( 0.08f,-0.58f); glVertex2f( 0.07f,-0.51f); glVertex2f(-0.07f,-0.51f); glEnd();
    glColor3f(1.0f, 1.0f, 0.80f);
    glBegin(GL_QUADS); glVertex2f( 0.23f,-0.65f); glVertex2f( 0.30f,-0.65f); glVertex2f( 0.30f,-0.60f); glVertex2f( 0.23f,-0.60f); glEnd();
    glColor3f(1.0f, 0.10f, 0.10f);
    glBegin(GL_QUADS); glVertex2f(-0.30f,-0.65f); glVertex2f(-0.23f,-0.65f); glVertex2f(-0.23f,-0.60f); glVertex2f(-0.30f,-0.60f); glEnd();
    glColor3f(0.15f, 0.15f, 0.15f); city_drawCircle(-0.20f,-0.72f,0.06f);
    glColor3f(0.50f, 0.50f, 0.50f); city_drawCircle(-0.20f,-0.72f,0.03f);
    glColor3f(0.15f, 0.15f, 0.15f); city_drawCircle( 0.20f,-0.72f,0.06f);
    glColor3f(0.50f, 0.50f, 0.50f); city_drawCircle( 0.20f,-0.72f,0.03f);
    // RIVER
    glColor3f(0.42f, 0.55f, 0.75f);
    glBegin(GL_QUADS);
        glVertex2f(-1.0f,-1.00f); glVertex2f( 1.0f,-1.00f);
        glVertex2f( 1.0f,-0.80f); glVertex2f(-1.0f,-0.80f);
    glEnd();
    glColor3f(0.55f, 0.70f, 0.88f);
    glBegin(GL_QUADS);
        glVertex2f(-1.0f,-0.87f); glVertex2f( 1.0f,-0.87f);
        glVertex2f( 1.0f,-0.84f); glVertex2f(-1.0f,-0.84f);
    glEnd();
    // SHIP HULL
    glColor3f(0.45f, 0.25f, 0.10f);
    glBegin(GL_QUADS);
        glVertex2f(city_shipPos-0.20f,-0.99f); glVertex2f(city_shipPos+0.20f,-0.99f);
        glVertex2f(city_shipPos+0.35f,-0.84f); glVertex2f(city_shipPos-0.35f,-0.84f);
    glEnd();
    glColor3f(0.95f, 0.92f, 0.85f);
    glBegin(GL_QUADS);
        glVertex2f(city_shipPos-0.20f,-0.84f); glVertex2f(city_shipPos+0.20f,-0.84f);
        glVertex2f(city_shipPos+0.20f,-0.76f); glVertex2f(city_shipPos-0.20f,-0.76f);
    glEnd();
    glColor3f(0.90f, 0.88f, 0.80f);
    glBegin(GL_QUADS);
        glVertex2f(city_shipPos-0.10f,-0.76f); glVertex2f(city_shipPos+0.10f,-0.76f);
        glVertex2f(city_shipPos+0.10f,-0.70f); glVertex2f(city_shipPos-0.10f,-0.70f);
    glEnd();
    glColor3f(0.30f, 0.30f, 0.30f);
    glBegin(GL_QUADS);
        glVertex2f(city_shipPos+0.02f,-0.70f); glVertex2f(city_shipPos+0.07f,-0.70f);
        glVertex2f(city_shipPos+0.07f,-0.63f); glVertex2f(city_shipPos+0.02f,-0.63f);
    glEnd();
    glColor3f(0.55f, 0.85f, 0.95f);
    glBegin(GL_QUADS); glVertex2f(city_shipPos-0.17f,-0.83f); glVertex2f(city_shipPos-0.10f,-0.83f); glVertex2f(city_shipPos-0.10f,-0.77f); glVertex2f(city_shipPos-0.17f,-0.77f); glEnd();
    glBegin(GL_QUADS); glVertex2f(city_shipPos-0.04f,-0.83f); glVertex2f(city_shipPos+0.04f,-0.83f); glVertex2f(city_shipPos+0.04f,-0.77f); glVertex2f(city_shipPos-0.04f,-0.77f); glEnd();
    glBegin(GL_QUADS); glVertex2f(city_shipPos+0.10f,-0.83f); glVertex2f(city_shipPos+0.17f,-0.83f); glVertex2f(city_shipPos+0.17f,-0.77f); glVertex2f(city_shipPos+0.10f,-0.77f); glEnd();
    // SMOKE
    glColor3f(0.80f, 0.80f, 0.80f);
    city_drawCircle(city_shipPos+0.045f,-0.61f,0.013f);
    city_drawCircle(city_shipPos+0.05f, -0.59f,0.016f);
    city_drawCircle(city_shipPos+0.06f, -0.57f,0.012f);
    glFlush();
}

void city_keyboard(unsigned char key, int x, int y) {
    if (key == 13) {
        screenState = 0;
        glutSetWindow(cityWindow);
        glutHideWindow();
        glutSetWindow(proposalWindow);
        glutShowWindow();
        glutDisplayFunc(displayProposal);
        glutPostRedisplay();
    }
}

// ============================================================
//  PARK SCENE (MD Yusuf) globals & functions
// ============================================================
GLfloat park_planeY = 0.7f;
GLfloat park_planeUpperLimit = 0.85f;
GLfloat park_planeLowerLimit = 0.62f;
GLfloat park_planePosition = 0.05f;
GLfloat park_planeSpeed = 0.004f;

GLfloat park_cloudPosition1 = -0.75f;
GLfloat park_cloudPosition2 = -0.35f;
GLfloat park_cloudPosition3 =  0.70f;
GLfloat park_cloudSpeed = 0.0005f;

int park_rainMode = 0;
float park_rainDropY[600];
float park_rainDropX[600];
int park_nightMode = 0;

void park_drawStars()
{
    glPointSize(2.0f);
    glBegin(GL_POINTS);
    glColor3f(1.0f, 1.0f, 1.0f);
    glVertex2f(-0.9f, 0.9f);
    glVertex2f(-0.7f, 0.8f);
    glVertex2f(-0.5f, 0.95f);
    glVertex2f(-0.2f, 0.75f);
    glVertex2f(0.0f, 0.92f);
    glVertex2f(0.2f, 0.82f);
    glVertex2f(0.5f, 0.90f);
    glVertex2f(0.8f, 0.78f);
    glVertex2f(0.65f, 0.96f);
    glEnd();
}

void park_initRain() {
    for (int i = 0; i < 600; i++) {
        park_rainDropX[i] = ((rand() % 2000) / 1000.0f) - 1.0f;
        park_rainDropY[i] = ((rand() % 2000) / 1000.0f) - 1.0f;
    }
}

void park_keyboard(unsigned char key, int x, int y) {
    if (key == 'r' || key == 'R') {
        park_rainMode = !park_rainMode;
         if (park_rainMode) sndPlaySound("rain.wav", SND_ASYNC | SND_LOOP);
        else          sndPlaySound(NULL, 0);
    }
    if (key == 'n' || key == 'N') {
        park_nightMode = !park_nightMode;
    }
    if (key == 13) {
        screenState = 0;
        park_rainMode  = 0;
        park_nightMode = 0;
        glutSetWindow(parkWindow);
        glutHideWindow();
        glutSetWindow(proposalWindow);
        glutShowWindow();
        glutDisplayFunc(displayProposal);
        glutPostRedisplay();
    }
}

void park_SpecialInput(int key, int x, int y)
{
    switch(key)
    {
        case GLUT_KEY_UP:
            park_planeY += 0.05f;
            if(park_planeY > park_planeUpperLimit)
                park_planeY = park_planeUpperLimit;
            break;
        case GLUT_KEY_DOWN:
            park_planeY -= 0.05f;
            if(park_planeY < park_planeLowerLimit)
                park_planeY = park_planeLowerLimit;
            break;
        case GLUT_KEY_LEFT:
            break;
        case GLUT_KEY_RIGHT:
            break;
    }
    glutPostRedisplay();
}

void park_drawEllipse(GLfloat x, GLfloat y, GLfloat radiusX, GLfloat radiusY, int triangleAmount) {
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(x, y);
    GLfloat twicePi = 2.0f * PI;
    for (int i = 0; i <= triangleAmount; i++) {
        glVertex2f(
            x + (radiusX * cos(i * twicePi / triangleAmount)),
            y + (radiusY * sin(i * twicePi / triangleAmount))
        );
    }
    glEnd();
}

void park_drawFilledCircle(GLfloat x, GLfloat y, GLfloat radius, int triangleAmount) {
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(x, y);
    GLfloat twicePi = 2.0f * PI;
    for(int i = 0; i <= triangleAmount; i++) {
        glVertex2f( x + (radius * cos(i * twicePi / triangleAmount)),
                    y + (radius * sin(i * twicePi / triangleAmount)) );
    }
    glEnd();
}

void park_drawQuad(GLfloat x, GLfloat y, GLfloat w, GLfloat h) {
    glBegin(GL_QUADS);
    glVertex2f(x, y);
    glVertex2f(x + w, y);
    glVertex2f(x + w, y + h);
    glVertex2f(x, y + h);
    glEnd();
}

void park_drawTriangle(GLfloat x, GLfloat y, GLfloat w, GLfloat h) {
    glBegin(GL_TRIANGLES);
    glVertex2f(x, y + h);
    glVertex2f(x - w, y);
    glVertex2f(x + w, y);
    glEnd();
}

void park_drawSun()
{
    if(park_nightMode)
    {
        glColor3f(0.9f, 0.9f, 0.95f);
        park_drawFilledCircle(0.45f, 0.75f, 0.12f, 100);
        glColor3f(0.05f, 0.05f, 0.12f);
        park_drawFilledCircle(0.49f, 0.78f, 0.10f, 100);
    }
    else if(park_rainMode)
    {
        glColor3f(0.25f, 0.25f, 0.32f);
        park_drawFilledCircle(0.45f, 0.75f, 0.15f, 100);
    }
    else
    {
        glColor3f(1.0f, 1.0f, 0.0f);
        park_drawFilledCircle(0.45f, 0.75f, 0.15f, 100);
    }
}

void park_updateCloud(int v)
{
    if(park_cloudPosition1 > 1.9 && !park_rainMode)
        park_cloudPosition1 = -1.9f;
    if(park_cloudPosition2 > 1.4 && !park_rainMode)
        park_cloudPosition2 = -1.4f;
    if(park_cloudPosition3 > 1.0 && !park_rainMode)
        park_cloudPosition3 = -1.0f;
    park_cloudPosition1 += park_cloudSpeed;
    park_cloudPosition2 += park_cloudSpeed;
    park_cloudPosition3 += park_cloudSpeed;
    glutSetWindow(parkWindow);
    glutPostRedisplay();
    glutTimerFunc(10, park_updateCloud, 0);
}

void park_drawCloud(GLfloat cx, GLfloat cy, GLfloat size) {
    glPushMatrix();
    glColor3f(1.0f, 1.0f, 1.0f);
    park_drawFilledCircle(cx, cy, size, 30);
    glTranslatef(+0.05f, 0.05f, 0.0f);
    park_drawFilledCircle(cx, cy, size, 30);
    glTranslatef(+0.07f, 0.01f, 0.0f);
    park_drawFilledCircle(cx, cy, size, 30);
    glTranslatef(+0.07f, -0.01f, 0.0f);
    park_drawFilledCircle(cx, cy, size, 30);
    glTranslatef(+0.03f, -0.05f, 0.0f);
    park_drawFilledCircle(cx, cy, size, 30);
    glTranslatef(-0.03f, -0.05f, 0.0f);
    park_drawFilledCircle(cx, cy, size, 30);
    glTranslatef(-0.06f, -0.02f, 0.0f);
    park_drawFilledCircle(cx, cy, size, 30);
    glTranslatef(-0.06f, -0.0f, 0.0f);
    park_drawFilledCircle(cx, cy, size, 30);
    glTranslatef(-0.06f, 0.02f, 0.0f);
    park_drawFilledCircle(cx, cy, size, 30);
    glTranslatef(0.09f, 0.05f, 0.0f);
    park_drawFilledCircle(cx, cy, size*1.15, 30);
    glPopMatrix();
}

void park_drawCityBuildings() {
    glPushMatrix();
    if(park_nightMode)
        glColor3f(0.20f, 0.20f, 0.25f);
    else
        glColor3f(0.55f, 0.60f, 0.68f);
    float x = -0.5f;
    park_drawQuad(x, 0.0f, 0.1f, 0.20f);
    x += 0.1f;
    glPushMatrix();
    glTranslatef(x + 0.5f, 0.0f, 0.0f);
    glScalef(1.0, 1.5, 1.0);
    park_drawQuad(-0.5f, 0.0f, 0.1f, 0.20f);
    glPopMatrix();
    x += 0.1f;
    glPushMatrix();
    glTranslatef(x + 0.5f, 0.0f, 0.0f);
    glScalef(1.0, 0.7, 1.0);
    park_drawQuad(-0.5f, 0.0f, 0.08f, 0.20f);
    glPopMatrix();
    x += 0.08f;
    glPushMatrix();
    glTranslatef(x + 0.5f, 0.0f, 0.0f);
    glScalef(1.0, 1.2, 1.0);
    park_drawQuad(-0.5f, 0.0f, 0.09f, 0.20f);
    glPopMatrix();
    x += 0.09f;
    glPushMatrix();
    glTranslatef(x + 0.5f, 0.0f, 0.0f);
    glScalef(1.0, 2.0, 1.0);
    park_drawQuad(-0.5f, 0.0f, 0.1f, 0.20f);
    glPopMatrix();
    x += 0.1f;
    glPushMatrix();
    glTranslatef(x + 0.5f, 0.0f, 0.0f);
    glScalef(1.0, 1.0, 1.0);
    park_drawQuad(-0.5f, 0.0f, 0.12f, 0.20f);
    glPopMatrix();
    x += 0.12f;
    glPushMatrix();
    glTranslatef(x + 0.5f, 0.0f, 0.0f);
    glScalef(1.0, 1.3, 1.0);
    park_drawQuad(-0.5f, 0.0f, 0.1f, 0.20f);
    glPopMatrix();
    x += 0.1f;
    glPushMatrix();
    glTranslatef(x + 0.5f, 0.0f, 0.0f);
    glScalef(1.0, 0.9, 1.0);
    park_drawQuad(-0.5f, 0.0f, 0.09f, 0.20f);
    glPopMatrix();
    x += 0.09f;
    glPushMatrix();
    glTranslatef(x + 0.5f, 0.0f, 0.0f);
    glScalef(1.0, 0.7, 1.0);
    park_drawQuad(-0.5f, 0.0f, 0.08f, 0.20f);
    glPopMatrix();
    x += 0.08f;
    glPushMatrix();
    glTranslatef(x + 0.5f, 0.0f, 0.0f);
    glScalef(1.0, 0.3, 1.0);
    park_drawQuad(-0.5f, 0.0f, 0.08f, 0.20f);
    glPopMatrix();
    glPopMatrix();
}

void park_drawGrass() {
    if(park_nightMode)
        glColor3f(0.10f, 0.35f, 0.10f);
    else
        glColor3f(0.30f, 0.70f, 0.20f);
    glBegin(GL_QUADS);
    glVertex2f(-1.0f, -1.0f);
    glVertex2f(1.0f, -1.0f);
    glVertex2f(1.0f, 0.04f);
    glVertex2f(-1.0f, 0.04f);
    glEnd();
    if(park_nightMode)
        glColor3f(0.10f, 0.35f, 0.10f);
    else
        glColor3f(0.4f, 0.8f, 0.22f);
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(-0.4f, 0.01f);
    glVertex2f(-1.0f, -0.1f);
    glVertex2f(-1.0f, 0.1f);
    glVertex2f(-0.5f, 0.08f);
    glVertex2f(0.0f, 0.04f);
    glEnd();
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(0.6f, 0.04f);
    glVertex2f(1.0f, 0.04f);
    glVertex2f(1.0f, 0.15f);
    glVertex2f(0.7f, 0.12f);
    glVertex2f(0.2f, 0.04f);
    glEnd();
}

void park_drawPineTree(GLfloat x, GLfloat y) {
    glPushMatrix();
    glTranslatef(x, y, 0.0f);
    glColor3f(0.45f, 0.28f, 0.10f);
    glPushMatrix();
    park_drawQuad(0.0f, 0.0f, 0.03f, 0.08f);
    glPopMatrix();
    glTranslatef(0.015f, 0.08f, 0.0f);
    glColor3f(0.10f, 0.55f, 0.15f);
    glPushMatrix();
    park_drawTriangle(0.0f, 0.0f, 0.12f, 0.14f);
    glPopMatrix();
    glTranslatef(0.0f, 0.09f, 0.0f);
    glPushMatrix();
    glScalef(0.8f, 1.0f, 0.0f);
    park_drawTriangle(0.0f, 0.0f, 0.12f, 0.13f);
    glPopMatrix();
    glTranslatef(0.0f, 0.09f, 0.0f);
    glPushMatrix();
    glScalef(0.6f, 1.0f, 0.0f);
    park_drawTriangle(0.0f, 0.0f, 0.12f, 0.12f);
    glPopMatrix();
    glPopMatrix();
}

void park_drawPath() {
    glColor3f(0.76f, 0.64f, 0.48f);
    glBegin(GL_QUADS);
    glVertex2f(-0.64f, -0.03f);
    glVertex2f(-0.55f, -0.04f);
    glVertex2f(-0.57f, -0.089f);
    glVertex2f(-0.72f, -0.055f);
    glEnd();
    glBegin(GL_QUADS);
    glVertex2f(-0.55f, -0.04f);
    glVertex2f(-0.57f, -0.089f);
    glVertex2f( 1.0f, -0.1f);
    glVertex2f(1.0f, -0.052f);
    glEnd();
    glBegin(GL_QUADS);
    glVertex2f( 0.1f, -0.05f);
    glVertex2f( 0.1f, -0.2f);
    glVertex2f( -0.1f, -0.20f);
    glVertex2f( -0.1f, -0.05f);
    glEnd();
    park_drawFilledCircle(-0.014,-0.38,0.24,30);
    glBegin(GL_QUADS);
    glVertex2f( -0.22f, -0.48f);
    glVertex2f( -0.42f, -0.60f);
    glVertex2f( -0.28f, -0.76f);
    glVertex2f( -0.008f, -0.61f);
    glEnd();
    glBegin(GL_TRIANGLES);
    glVertex2f( -0.42f, -0.60f);
    glVertex2f( -0.28f, -0.76f);
    glVertex2f(-0.5f, -0.74f);
    glEnd();
    glBegin(GL_QUADS);
    glVertex2f( -0.28f, -0.76f);
    glVertex2f(-0.5f, -0.74f);
    glVertex2f( -0.65f, -1.0f);
    glVertex2f( -0.4f, -1.0f);
    glEnd();
}

void park_drawPond() {
    glColor3f(0.25f, 0.55f, 0.80f);
    glBegin(GL_POLYGON);
    glVertex2f(-0.73f, -0.40f);
    glVertex2f(-0.67f, -0.54f);
    glVertex2f(-0.5f, -0.54f);
    glVertex2f(-0.33f, -0.36f);
    glVertex2f(-0.38f, -0.27f);
    glVertex2f(-0.28f, -0.11f);
    glVertex2f(-0.54f, -0.11f);
    glVertex2f(-0.62f, -0.21f);
    glVertex2f(-0.67f, -0.24f);
    glEnd();
    glColor3f(0.50f, 0.40f, 0.20f);
    glBegin(GL_QUADS);
    glVertex2f(-0.73f, -0.40f);
    glVertex2f(-0.67f, -0.24f);
    glVertex2f(-0.66f, -0.25f);
    glVertex2f(-0.71f, -0.40f);
    glEnd();
    glBegin(GL_QUADS);
    glVertex2f(-0.67f, -0.24f);
    glVertex2f(-0.66f, -0.25f);
    glVertex2f(-0.61f, -0.22f);
    glVertex2f(-0.62f, -0.21f);
    glEnd();
    glBegin(GL_QUADS);
    glVertex2f(-0.61f, -0.22f);
    glVertex2f(-0.62f, -0.21f);
    glVertex2f(-0.54f, -0.11f);
    glVertex2f(-0.52f, -0.11f);
    glEnd();
    glBegin(GL_QUADS);
    glVertex2f(-0.73f, -0.40f);
    glVertex2f(-0.71f, -0.40f);
    glVertex2f(-0.665f, -0.535f);
    glVertex2f(-0.67f, -0.54f);
    glEnd();
}

void park_drawRoundTree(GLfloat x, GLfloat y) {
    glPushMatrix();
    glTranslatef(x, y, 0.0f);
    glColor3f(0.45f, 0.28f, 0.10f);
    glBegin(GL_QUADS);
    glVertex2f(-0.85f, 0.027f);
    glVertex2f(-0.81f, 0.027f);
    glVertex2f(-0.79f, -0.32f);
    glVertex2f(-0.87f, -0.32f);
    glEnd();
    glBegin(GL_QUADS);
    glVertex2f(-0.83f, -0.09f);
    glVertex2f(-0.93f, -0.023f);
    glVertex2f(-0.91f, -0.0f);
    glVertex2f(-0.83f, -0.047f);
    glEnd();
    glBegin(GL_QUADS);
    glVertex2f(-0.82f, -0.06f);
    glVertex2f(-0.73f, 0.0f);
    glVertex2f(-0.72f, -0.02f);
    glVertex2f(-0.82f, -0.09f);
    glEnd();
    glColor3f(0.20f, 0.62f, 0.14f);
    park_drawFilledCircle(-0.83,0.18, 0.16, 30);
    park_drawFilledCircle(-0.66,0.072, 0.13, 30);
    park_drawFilledCircle(-1.0,0.062, 0.13, 30);
    glPopMatrix();
}

void park_drawBridge() {
    glColor3f(0.60f, 0.38f, 0.18f);
    glBegin(GL_QUADS);
    glVertex2f(-0.56, -0.14);
    glVertex2f(-0.30, -0.14);
    glVertex2f(-0.27, -0.093);
    glVertex2f(-0.525, -0.090);
    glEnd();
    glColor3f(0.50f, 0.50f, 0.55f);
    glLineWidth(5.0);
    glBegin(GL_LINES);
        glVertex2f(-0.56, -0.14);
        glVertex2f(-0.417, -0.0055);
    glEnd();
    glLineWidth(5.0);
    glBegin(GL_LINES);
        glVertex2f(-0.417, -0.0055);
        glVertex2f(-0.30, -0.14);
    glEnd();
    glLineWidth(5.0);
    glBegin(GL_LINES);
        glVertex2f(-0.525, -0.090);
        glVertex2f(-0.38, 0.055);
    glEnd();
    glLineWidth(5.0);
    glBegin(GL_LINES);
        glVertex2f(-0.38, 0.055);
        glVertex2f(-0.27, -0.093);
    glEnd();
    glLineWidth(2.0);
    glBegin(GL_LINES);
        glVertex2f(-0.34, -0.14);
        glVertex2f(-0.34, -0.095);
    glEnd();
    glLineWidth(2.0);
    glBegin(GL_LINES);
        glVertex2f(-0.38, -0.046);
        glVertex2f(-0.38, -0.14);
    glEnd();
    glLineWidth(2.0);
    glBegin(GL_LINES);
        glVertex2f(-0.425, -0.14);
        glVertex2f(-0.425, -0.0143);
    glEnd();
    glLineWidth(2.0);
    glBegin(GL_LINES);
        glVertex2f(-0.51, -0.095);
        glVertex2f(-0.46, -0.05);
    glEnd();
    glLineWidth(2.0);
    glBegin(GL_LINES);
        glVertex2f(-0.46, -0.053);
        glVertex2f(-0.46, -0.14);
    glEnd();
    glLineWidth(2.0);
    glBegin(GL_LINES);
        glVertex2f(-0.51, -0.14);
        glVertex2f(-0.51, -0.095);
    glEnd();
    glLineWidth(2.0);
    glBegin(GL_LINES);
        glVertex2f(-0.48, -0.09);
        glVertex2f(-0.48, -0.045);
    glEnd();
    glLineWidth(2.0);
    glBegin(GL_LINES);
        glVertex2f(-0.44, -0.0044);
        glVertex2f(-0.44, -0.09);
    glEnd();
    glLineWidth(2.0);
    glBegin(GL_LINES);
        glVertex2f(-0.41, 0.027);
        glVertex2f(-0.41, -0.090);
    glEnd();
    glLineWidth(2.0);
    glBegin(GL_LINES);
        glVertex2f(-0.37, 0.039);
        glVertex2f(-0.37, -0.091);
    glEnd();
    glLineWidth(2.0);
    glBegin(GL_LINES);
        glVertex2f(-0.33, -0.015);
        glVertex2f(-0.33, -0.091);
    glEnd();
    glLineWidth(2.0);
    glBegin(GL_LINES);
        glVertex2f(-0.29, -0.063);
        glVertex2f(-0.29, -0.091);
    glEnd();
}

void park_drawSwingSet() {
    glPushMatrix();
    glColor3f(0.44f, 0.27f, 0.10f);
    glLineWidth(3.5f);
    glBegin(GL_LINES);
    glVertex2f(0.44f,-0.008f);
    glVertex2f(0.38f,-0.25f);
    glVertex2f(0.44f,-0.008f);
    glVertex2f(0.48f,-0.25f);
    glEnd();
    glPushMatrix();
    glTranslatef(0.35f, 0.0f, 0.0f);
    glBegin(GL_LINES);
    glVertex2f(0.44f,-0.008f);
    glVertex2f(0.38f,-0.25f);
    glVertex2f(0.44f,-0.008f);
    glVertex2f(0.48f,-0.25f);
    glEnd();
    glPopMatrix();
    glLineWidth(3.5f);
    glBegin(GL_LINES);
    glVertex2f(0.44f,-0.008f);
    glVertex2f(0.79f,-0.008f);
    glEnd();
    glColor3f(0.86f, 0.79f, 0.56f);
    glBegin(GL_QUADS);
    glVertex2f(0.55f,-0.15f);
    glVertex2f(0.53f,-0.20f);
    glVertex2f(0.585f,-0.20f);
    glVertex2f(0.605f,-0.15f);
    glEnd();
    glColor3f(0.44f, 0.27f, 0.10f);
    glLineWidth(1.5f);
    glBegin(GL_LINES);
    glVertex2f(0.54f,-0.008f);
    glVertex2f(0.54f,-0.18f);
    glVertex2f(0.59f,-0.008f);
    glVertex2f(0.59f,-0.18f);
    glEnd();
    glColor3f(0.86f, 0.79f, 0.56f);
    glBegin(GL_QUADS);
    glVertex2f(0.66f,-0.15f);
    glVertex2f(0.64f,-0.20f);
    glVertex2f(0.695f,-0.20f);
    glVertex2f(0.715f,-0.15f);
    glEnd();
    glColor3f(0.44f, 0.27f, 0.10f);
    glLineWidth(1.5f);
    glBegin(GL_LINES);
    glVertex2f(0.65f,-0.008f);
    glVertex2f(0.65f,-0.18f);
    glVertex2f(0.70f,-0.008f);
    glVertex2f(0.70f,-0.18f);
    glEnd();
    glPopMatrix();
}

void park_update(int v)
{
    if(park_planePosition > 1.0 && !park_rainMode && !park_nightMode)
        park_planePosition = -1.0f;
    park_planePosition += park_planeSpeed;
    glutSetWindow(parkWindow);
    glutPostRedisplay();
    glutTimerFunc(10, park_update, 0);
}

void park_drawAirplane(float x, float y) {
    glColor3f(0.92f, 0.92f, 0.92f);
    park_drawEllipse(x, y, 0.1f, 0.04f, 50);
    glBegin(GL_TRIANGLES);
    glVertex2f(x + 0.098f, y-0.015);
    glVertex2f(x + 0.135f, y-0.005);
    glVertex2f(x + 0.095f, y + 0.015f);
    glEnd();
    glColor3f(0.85f, 0.85f, 0.88f);
    glBegin(GL_POLYGON);
    glVertex2f(x-0.03,        y-0.01);
    glVertex2f(x + 0.011f, y-0.01);
    glVertex2f(x + 0.001f, y - 0.074f);
    glVertex2f(x - 0.04f, y - 0.074f);
    glEnd();
    glBegin(GL_POLYGON);
    glVertex2f(x - 0.09f, y);
    glVertex2f(x - 0.06f, y);
    glVertex2f(x - 0.068f, y + 0.067f);
    glVertex2f(x - 0.09f, y + 0.060f);
    glEnd();
    glBegin(GL_POLYGON);
    glVertex2f(x - 0.095f, y + 0.035f);
    glVertex2f(x - 0.055f, y + 0.038f);
    glVertex2f(x - 0.055f, y + 0.073f);
    glVertex2f(x - 0.095f, y + 0.07f);
    glEnd();
    glColor3f(0.55f, 0.82f, 0.97f);
    for (int i = 0; i < 5; i++)
        park_drawFilledCircle(x - 0.025f + i * 0.022f, y + 0.007f, 0.010f, 30);
}

void park_drawBench(float x, float y) {
    glPushMatrix();
    glTranslatef(x, y, 0.0f);
    glColor3f(0.2f, 0.2f, 0.2f);
    glBegin(GL_QUADS);
    glVertex2f(0.032f, -0.02f);
    glVertex2f(0.052f, -0.02f);
    glVertex2f(0.052f, 0.06f);
    glVertex2f(0.032f, 0.06f);
    glVertex2f(0.15f, -0.02f);
    glVertex2f(0.13f, -0.02f);
    glVertex2f(0.13f,  0.06f);
    glVertex2f(0.15f, 0.06f);
    glEnd();
    glColor3f(0.55f, 0.27f, 0.07f);
    glBegin(GL_QUADS);
    glVertex2f(0.0f, 0.0f);
    glVertex2f(0.18f, 0.0f);
    glVertex2f(0.18f, -0.03f);
    glVertex2f(0.0f, -0.03f);
    glEnd();
    glBegin(GL_QUADS);
    glVertex2f(0.0f, 0.05f);
    glVertex2f(0.18f, 0.05f);
    glVertex2f(0.18f, 0.02f);
    glVertex2f(0.0f, 0.02f);
    glEnd();
    glColor3f(0.2f, 0.2f, 0.2f);
    glBegin(GL_QUADS);
    glVertex2f(0.01f, -0.03f);
    glVertex2f(0.03f, -0.03f);
    glVertex2f(0.03f, -0.08f);
    glVertex2f(0.01f, -0.08f);
    glVertex2f(0.15f, -0.03f);
    glVertex2f(0.17f, -0.03f);
    glVertex2f(0.17f, -0.08f);
    glVertex2f(0.15f, -0.08f);
    glEnd();
    glPopMatrix();
}

void park_drawParkLine(float x2, float y2) {
    float x1 = -0.018;
    float y1 = -0.11;
    glColor3f(0.6f, 0.3f, 0.1f);
    glLineWidth(4.0f);
    glBegin(GL_LINES);
    glVertex2f(x1, y1);
    glVertex2f(x2, y2);
    glEnd();
}

void park_drawSomethingAttractive(float x, float y) {
    glColor3f(0.75f, 0.75f, 0.75f);
    park_drawFilledCircle(-0.011,-0.37, 0.169f, 40);
    glColor3f(0.2f, 0.6f, 1.0f);
    park_drawFilledCircle(-0.011,-0.37, 0.148f, 40);
    park_drawParkLine(0.00,-0.54);
    park_drawParkLine(-0.13,-0.49);
    park_drawParkLine(-0.17,-0.39);
    park_drawParkLine(-0.14,-0.26);
    park_drawParkLine(-0.034,-0.20);
    park_drawParkLine(0.11,-0.26);
    park_drawParkLine(0.15,-0.37);
    park_drawParkLine(0.11,-0.48);
}

void park_display() {
    glClear(GL_COLOR_BUFFER_BIT);
    glLoadIdentity();
    if(park_nightMode)
        glClearColor(0.05f, 0.05f, 0.12f, 1.0f);
    else if(park_rainMode)
        glClearColor(0.25f, 0.25f, 0.32f, 1.0f);
    else
        glClearColor(0.53f, 0.81f, 0.92f, 1.0f);
    park_drawSun();
    if(park_nightMode)
        park_drawStars();
    park_drawCloud(park_cloudPosition1, 0.82f, 0.055f);
    park_drawCloud(park_cloudPosition2, 0.65f, 0.055f);
    park_drawCloud(park_cloudPosition3, 0.85f, 0.055f);
    park_drawCityBuildings();
    park_drawGrass();
    park_drawPath();
    park_drawPond();
    park_drawBridge();
    park_drawPineTree(-0.55f, 0.03f);
    park_drawPineTree( -0.10f, -0.02f);
    park_drawPineTree( 0.02f, 0.02f);
    park_drawPineTree( 0.70f, 0.0f);
    glPushMatrix();
    glScalef(1.20f, 1.40f, 0.0f);
    park_drawPineTree( 0.70f, 0.0f);
    glPopMatrix();
    park_drawRoundTree(0.0f, 0.0f);
    glPushMatrix();
    glTranslatef(0,-0.2f,0);
    park_drawSwingSet();
    glPopMatrix();
    park_drawAirplane(park_planePosition, park_planeY);
    park_drawBench(-0.3f, 0.05f);
    park_drawBench(0.3f, 0.05f);
    park_drawSomethingAttractive(-0.011f, -0.37f);
    if (park_rainMode) {
        glColor3f(0.75f, 0.75f, 1.0f);
        glLineWidth(1.2f);
        glBegin(GL_LINES);
        for (int i = 0; i < 600; i++) {
            park_rainDropY[i] -= 0.03f;
            if (park_rainDropY[i] < -1.0f) {
                park_rainDropY[i] = 1.0f;
                park_rainDropX[i] = ((rand() % 2000) / 1000.0f) - 1.0f;
            }
            glVertex2f(park_rainDropX[i], park_rainDropY[i]);
            glVertex2f(park_rainDropX[i] + 0.015f, park_rainDropY[i] - 0.05f);
        }
        glEnd();
    }
    glFlush();
}

// ============================================================
//  ABIR'S CITY SCENE globals & functions
// ============================================================
int    abir_ucounter = 25;
bool   abir_rain     = false;
bool   abir_carv     = false;
bool   abir_day      = true;
float  abir_move_cloud = 0.0f;
void*  abir_currentfont;

static float abir_car1Speed     = 0.5f;
static float abir_tempCar1Speed = 0.5f;
static float abir_car2Speed     = 0.5f;
static float abir_tempCar2Speed = 0.5f;

void abir_drawPixelInt(int x, int y) {
    glBegin(GL_POINTS);
    glVertex2i(x, y);
    glEnd();
}

void abir_setFont(void* font) {
    abir_currentfont = font;
}

void abir_drawstring(float x, float y, float z, const char* string) {
    glRasterPos3f(x, y, z);
    for (const char* c = string; *c != '\0'; c++)
        glutBitmapCharacter(abir_currentfont, *c);
}

void abir_twoIntVertS(int pT, int pB, int fixedP) {
    glBegin(GL_LINES);
    glVertex2i(fixedP, pT);
    glVertex2i(fixedP, pB);
    glEnd();
}

void abir_twoIntHorzS(int pT, int pB, int fixedP) {
    glBegin(GL_LINES);
    glVertex2i(pB, fixedP);
    glVertex2i(pT, fixedP);
    glEnd();
}

void abir_quadHorzInt(int pT, int pB, int fixedP, int fixedQ) {
    glBegin(GL_QUADS);
    glVertex2i(pB, fixedP);
    glVertex2i(pT, fixedP);
    glVertex2i(pT, fixedQ);
    glVertex2i(pB, fixedQ);
    glEnd();
}

void abir_quadHorzInt2(int v0,int v01,int v1,int v11,int v2,int v21,int v3,int v31) {
    glBegin(GL_QUADS);
    glVertex2i(v0, v01);
    glVertex2i(v1, v11);
    glVertex2i(v2, v21);
    glVertex2i(v3, v31);
    glEnd();
}

void abir_triangleNS(int v0,int v01,int v1,int v11,int v2,int v21) {
    glBegin(GL_TRIANGLES);
    glVertex2i(v0, v01);
    glVertex2i(v1, v11);
    glVertex2i(v2, v21);
    glEnd();
}

void abir_draw_circle(float x, float y, float radius) {
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();
    glTranslatef(x, y, 0.0f);
    static const int circle_points = 100;
    static const float angle = 2.0f * 3.1416f / circle_points;
    glBegin(GL_POLYGON);
    double angle1 = 0.0;
    glVertex2d(radius * cos(0.0), radius * sin(0.0));
    for (int i = 0; i < circle_points; i++) {
        glVertex2d(radius * cos(angle1), radius * sin(angle1));
        angle1 += angle;
    }
    glEnd();
    glPopMatrix();
}

void abir_otherCircle(GLfloat x, GLfloat y, GLfloat z, GLfloat radius, int r, int g, int b) {
    int triangleAmount = 20;
    GLfloat twicePi = 2.0f * 3.1416f;
    glColor3ub(r, g, b);
    glBegin(GL_TRIANGLE_FAN);
    glVertex3f(x, y, z);
    for (int i = 0; i <= triangleAmount; i++) {
        glVertex3f(
            x + (radius * cos(i * twicePi / triangleAmount)),
            y + (radius * sin(i * twicePi / triangleAmount)),
            z
        );
    }
    glEnd();
}

void abir_plane() {
    static float planeTFactX = -180.0f;
    static float planeTFactY = 0.0f;
    if (planeTFactX >= 900) {
        planeTFactX = -180.0f;
        planeTFactY = 0.0f;
    } else {
        planeTFactX += 0.1f;
        planeTFactY += 0.005f;
    }
    glColor3ub(115, 110, 109);
    glPushMatrix();
    glTranslatef(planeTFactX, planeTFactY, 0);
    abir_quadHorzInt2(30,700,100,700,100,715,35,715);
    abir_triangleNS(100,700,110,707,100,715);
    abir_quadHorzInt2(37,715,57,715,40,730,20,730);
    abir_quadHorzInt2(70,715,80,715,60,735,50,735);
    abir_quadHorzInt2(70,700,80,700,60,680,50,680);
    glColor3ub(255,255,255);
    int planeWindowX = 95;
    for (int i = 0; i < 8; i++) {
        abir_drawPixelInt(planeWindowX, 710);
        planeWindowX -= 5;
    }
    glPopMatrix();
    glutPostRedisplay();
}

void abir_roadBar(int heightLoc, int height) {
    int startingLoc = 0, endingLoc = 40;
    for (int i = 0; i < 20; i++) {
        if (i % 2 == 0) glColor3ub(1,1,1);
        else             glColor3ub(217,217,217);
        abir_quadHorzInt(startingLoc, endingLoc, heightLoc, heightLoc + height);
        startingLoc = endingLoc;
        endingLoc  += 40;
    }
}

void abir_DrawMainRoad() {
    glColor3f(0.2f,0.2f,0.2f);
    abir_quadHorzInt(0,800,110,330);
    glColor3ub(201,195,200);
    abir_quadHorzInt(0,800,315,420);
    glColor3b(0,0,0);
    abir_quadHorzInt(0,800,419,420);
    glColor3f(1.0f,1.0f,1.0f);
    int dividerLine = 0;
    for (int i = 0; i < 200; i++) {
        abir_twoIntHorzS(dividerLine+40, dividerLine+20, 210);
        dividerLine += 40;
    }
    abir_roadBar(305,10);
    abir_roadBar(100,10);
}

void abir_drawCloud(int r, int g, int b) {
    glColor3ub(r,g,b);
    abir_draw_circle(100 + abir_move_cloud, 730, 33);
    abir_draw_circle(55  + abir_move_cloud, 730, 23);
    abir_draw_circle(145 + abir_move_cloud, 730, 23);
    abir_draw_circle(400 + abir_move_cloud, 730, 33);
    abir_draw_circle(355 + abir_move_cloud, 730, 23);
    abir_draw_circle(445 + abir_move_cloud, 730, 23);
}

void abir_drawSun() {
    glColor3f(255,255,0);
    abir_draw_circle(300,760,30);
}

void abir_drawMoon() {
    glColor3f(255,255,255);
    abir_draw_circle(300,760,20);
}

void abir_DrawCity() {
    // Building two
    glColor3ub(153,51,51);
    abir_quadHorzInt(120,250,420,630);
    int buildingY1=590, buildingY2=593;
    for (int i = 0; i < 3; i++) {
        glColor3ub(153,153,102);
        abir_quadHorzInt(120,250,buildingY1,buildingY2);
        buildingY1 -= 55; buildingY2 -= 55;
    }
    int b2y1=495,b2y2=520,b2x1=130,b2x2=160;
    for (int j = 1; j < 7; j++) {
        glColor3ub(153,53,53);
        abir_quadHorzInt(b2x1,b2x2,b2y1,b2y2);
        b2x1+=85; b2x2+=85;
        if (j%2==0) { b2x1=130; b2x2=160; b2y1+=50; b2y2+=50; }
    }
    glColor3ub(201,195,200);
    abir_quadHorzInt(170,200,420,460);
    glColor3ub(0,10,0);
    abir_quadHorzInt(177,180,420,460);
    abir_quadHorzInt(190,193,420,460);
    // Building one
    glColor3ub(236,238,58);
    abir_triangleNS(5,580,80,625,155,580);
    glColor3ub(201,195,200);
    abir_quadHorzInt(10,150,420,580);
    glColor3ub(88,86,85);
    abir_quadHorzInt(62,92,420,468);
    glColor3ub(0,10,0);
    abir_quadHorzInt(65,67,420,468);
    abir_quadHorzInt(88,90,420,468);
    abir_quadHorzInt(62,92,440,443);
    abir_quadHorzInt(62,92,445,448);
    int b1y1=470,b1y2=500,b1x1=15,b1x2=52;
    for (int i = 1; i < 5; i++) {
        glColor3ub(242,242,242);
        abir_quadHorzInt(b1x1,b1x2,b1y1,b1y2);
        b1x1+=85; b1x2+=85;
        if (i%2==0) { b1x1=15; b1x2=52; b1y1+=65; b1y2+=65; }
    }
    // Building three
    glColor3ub(102,51,0);
    abir_quadHorzInt(210,350,420,560);
    glColor3ub(0,106,77);
    abir_quadHorzInt(315,365,570,610);
    glColor3f(255,0,0);
    abir_draw_circle(340,590,10);
    glColor3ub(0,0,128);
    abir_quadHorzInt(310,315,560,615);
    glColor3ub(88,86,85);
    abir_quadHorzInt(250,310,420,460);
    glColor3ub(0,10,0);
    abir_quadHorzInt(250,310,440,443);
    abir_quadHorzInt(250,310,430,433);
    abir_quadHorzInt(290,293,420,460);
    int b3y1=470,b3y2=500,b3x1=220,b3x2=255;
    for (int j = 1; j < 5; j++) {
        glColor3ub(242,242,242);
        abir_quadHorzInt(b3x1,b3x2,b3y1,b3y2);
        b3x1+=85; b3x2+=85;
        if (j%2==0) { b3x1=220; b3x2=255; b3y1+=45; b3y2+=45; }
    }
}

void abir_rainfunc() {
    int x=0, y=778;
    static float a = -760.0f;
    if (a <= -768) a = -760.0f;
    else           a -= 0.5f;
    glColor3ub(255,255,255);
    glPushMatrix();
    glTranslatef(0.0f,a,0.0f);
    glBegin(GL_LINES);
    for (int i = 500; i >= 0; i--) {
        for (int j = 0; j <= i; j++) {
            glVertex3i(x, y, 0);
            glVertex3i(x+3, y+10, 0);
            x += rand() % 1050;
        }
        y += rand() % 15;
        x = 0;
    }
    glEnd();
    glPopMatrix();
    glutPostRedisplay();
}

void abir_DrawBank() {
    glColor3ub(153,53,53);
    abir_quadHorzInt(580,730,420,580);
    glColor3ub(34,10,60);
    abir_quadHorzInt(550,580,420,600);
    abir_quadHorzInt(730,760,420,600);
    glColor3ub(255,255,255);
    abir_twoIntHorzS(550,760,580);
    abir_twoIntHorzS(550,760,579);
    abir_twoIntHorzS(550,760,578);
    glColor3ub(153,226,57);
    abir_draw_circle(655,580,30);
    glColor3ub(255,255,255);
    int winStart=590,winEnd=630,winH1=440,winH2=480;
    for (int i = 0; i < 6; i++) {
        abir_quadHorzInt(winStart,winEnd,winH1,winH2);
        winStart+=45; winEnd+=45;
        if (i==2) { winStart=590; winEnd=630; winH1+=80; winH2+=80; }
    }
    glColor3ub(0,0,0);
    abir_setFont(GLUT_BITMAP_HELVETICA_12);
    abir_drawstring(637.0f,590.0f,0.0f,"BANK BD");
}

void abir_carBase(int r, int g, int b) {
    glColor3ub(r,g,b);
    abir_quadHorzInt(20,220,140,180);
    abir_quadHorzInt2(40,180,200,180,160,210,80,210);
    glColor3ub(0,0,0);
    abir_quadHorzInt2(115,180,190,180,155,205,115,205);
    abir_quadHorzInt2(45,180,110,180,110,205,80,205);
    abir_quadHorzInt(115,135,170,175);
    abir_quadHorzInt(45,65,170,175);
    abir_otherCircle(30.0f,160.0f,0.0f,19.0f,r,g,b);
    abir_otherCircle(210.0f,160.0f,0.0f,19.0f,r,g,b);
    glColor3ub(250,250,0);
    abir_quadHorzInt(225,230,155,165);
    if (!abir_day) {
        glColor3ub(200,200,200);
        glBegin(GL_QUADS);
        glVertex3i(230,155,0); glVertex3i(280,155,0);
        glVertex3i(280,195,0); glVertex3i(230,165,0);
        glEnd();
    }
    abir_otherCircle(60.0f,140.0f,0.0f,18.0f,0,0,0);
    abir_otherCircle(60.0f,140.0f,0.0f,10.0f,137,137,137);
    abir_otherCircle(170.0f,140.0f,0.0f,18.0f,0,0,0);
    abir_otherCircle(170.0f,140.0f,0.0f,10.0f,137,137,137);
}

void abir_cars() {
    static float car1Position = -400.0f;
    static float car2Position = -1200.0f;
    if (car1Position >= 1324) car1Position = -400.0f;
    else car1Position += abir_car1Speed;
    if (car2Position >= 1324) car2Position = -1200.0f;
    else car2Position += abir_car2Speed;
    glPushMatrix();
    glTranslatef(car1Position,0,0);
    abir_carBase(255,0,0);
    glPopMatrix();
    glPushMatrix();
    glTranslatef(car2Position,0,0);
    abir_carBase(0,0,255);
    glPopMatrix();
    glutPostRedisplay();
}

void abir_commonStuff() {
    abir_DrawCity();
    abir_DrawMainRoad();
    abir_DrawBank();
    abir_plane();
    if (abir_carv) abir_cars();
    if (abir_rain) abir_rainfunc();
}

void abir_daymode() {
    abir_drawSun();
    abir_drawCloud(255,255,255);
    glClearColor(0.0f,0.7f,1.5f,0.0f);
    abir_commonStuff();
}

void abir_nightmode() {
    abir_drawMoon();
    abir_drawCloud(232,225,224);
    glClearColor(0.0f,0.0f,0.0f,0.0f);
    abir_commonStuff();
}

void abir_display() {
    glClear(GL_COLOR_BUFFER_BIT);
    if (abir_day) abir_daymode();
    else          abir_nightmode();
    glFlush();
    glutSwapBuffers();
}

void abir_update(int value) {
    abir_move_cloud += 0.2f;
    glutSetWindow(abirWindow);
    glutPostRedisplay();
    glutTimerFunc(abir_ucounter, abir_update, 0);
}

void abir_keyboard(unsigned char key, int x, int y) {
    if (key == 13) {
        screenState   = 0;
        abir_rain     = false;
        abir_carv     = false;
        abir_day      = true;
        abir_car1Speed = 0.5f;
        abir_car2Speed = 0.5f;
        sndPlaySound(NULL, SND_ASYNC);
        glutSetWindow(abirWindow);
        glutHideWindow();
        glutSetWindow(proposalWindow);
        glutShowWindow();
        glutDisplayFunc(displayProposal);
        glutPostRedisplay();
        return;
    }
    if (key == 'r') abir_rain = true;
    if (key == 's') abir_rain = false;
    if (key == 'd') abir_day  = true;
    if (key == 'n') abir_day  = false;
    if (key == 'c') {
        abir_carv = true;
        sndPlaySound("TrafficSound.wav", SND_ASYNC | SND_LOOP);
    }
    if (key == 'v') {
        abir_carv = false;
        sndPlaySound(NULL, SND_ASYNC | SND_LOOP);
    }
    if (key == '.') {
        sndPlaySound(NULL, SND_ASYNC | SND_LOOP);
        if (abir_car2Speed != 0.0f) {
            abir_tempCar1Speed = abir_car1Speed;
            abir_tempCar2Speed = abir_car2Speed;
        }
        abir_car1Speed = 0.0f;
        abir_car2Speed = 0.0f;
    }
    if (key == ',') {
        abir_car1Speed = abir_tempCar1Speed;
        abir_car2Speed = abir_tempCar2Speed;
        sndPlaySound("TrafficSound.wav", SND_ASYNC | SND_LOOP);
    }
    glutSetWindow(abirWindow);
    glutPostRedisplay();
}

void abir_mouse(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON) {
        if (state == GLUT_DOWN) abir_day = false;
    } else {
        if (state == GLUT_DOWN) abir_day = true;
    }
    glutSetWindow(abirWindow);
    glutPostRedisplay();
}

// ============================================================
//  SAQLAIN'S STREET SCENE globals & functions
//  (All symbols prefixed with saqlain_ to avoid name conflicts)
// ============================================================
int   saqlain_isDay   = 1;
int   saqlain_rainOn  = 0;

GLfloat saqlain_carX       = -1.2f;
GLfloat saqlain_carSpeed   = 0.008f;
GLfloat saqlain_wheelAngle = 0.0f;

GLfloat saqlain_cloud1X    = -0.8f;
GLfloat saqlain_cloud2X    =  0.3f;
GLfloat saqlain_cloudSpeed = 0.003f;

GLfloat saqlain_batX       = -1.0f;
GLfloat saqlain_batSpeed   = 0.007f;
GLfloat saqlain_batWing    = 0.0f;
int     saqlain_batWingDir = 1;

#define SAQLAIN_NUM_DROPS 120
GLfloat saqlain_rainX[SAQLAIN_NUM_DROPS];
GLfloat saqlain_rainY[SAQLAIN_NUM_DROPS];

void saqlain_drawFilledCircle(GLfloat cx, GLfloat cy, GLfloat r, int segs) {
    GLfloat twoPi = 2.0f * (float)PI;
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(cx, cy);
    for (int i = 0; i <= segs; i++)
        glVertex2f(cx + r * cos(i * twoPi / segs),
                   cy + r * sin(i * twoPi / segs));
    glEnd();
}

void saqlain_drawCircleOutline(GLfloat cx, GLfloat cy, GLfloat r, int segs) {
    GLfloat twoPi = 2.0f * (float)PI;
    glBegin(GL_LINE_LOOP);
    for (int i = 0; i <= segs; i++)
        glVertex2f(cx + r * cos(i * twoPi / segs),
                   cy + r * sin(i * twoPi / segs));
    glEnd();
}

// Commented out per request - audio kept but disabled
// void saqlain_playAudio() {
//     PlaySound(TEXT("music.wav"), NULL, SND_ASYNC | SND_FILENAME | SND_LOOP);
// }

void saqlain_drawSun() {
    glColor3f(1.0f, 0.95f, 0.3f);
    glLineWidth(2.0f);
    glBegin(GL_LINES);
    float cx = 0.75f, cy = 0.82f;
    for (int i = 0; i < 12; i++) {
        float angle = i * (float)PI / 6.0f;
        glVertex2f(cx + 0.09f * cos(angle), cy + 0.09f * sin(angle));
        glVertex2f(cx + 0.14f * cos(angle), cy + 0.14f * sin(angle));
    }
    glEnd();
    glColor3f(1.0f, 0.9f, 0.0f);
    saqlain_drawFilledCircle(cx, cy, 0.085f, 60);
}

void saqlain_drawMoon() {
    glColor3f(1.0f, 1.0f, 0.4f);
    saqlain_drawFilledCircle(-0.72f, 0.82f, 0.09f, 60);
    glColor3f(0.05f, 0.05f, 0.18f);
    saqlain_drawFilledCircle(-0.65f, 0.86f, 0.08f, 60);
}

void saqlain_drawStars() {
    glColor3f(1.0f, 1.0f, 1.0f);
    glPointSize(2.5f);
    float stars[][2] = {
        {-0.9f,0.95f},{-0.5f,0.93f},{-0.2f,0.97f},{0.1f,0.9f},{0.4f,0.94f},
        {0.7f,0.92f},{0.9f,0.88f},{-0.7f,0.87f},{0.0f,0.85f},{0.55f,0.78f},
        {-0.4f,0.80f},{0.85f,0.75f},{-0.85f,0.72f},{0.25f,0.73f},{-0.1f,0.68f},
        {0.6f,0.65f},{-0.6f,0.65f},{0.95f,0.96f},{-0.95f,0.60f},{0.45f,0.60f}
    };
    glBegin(GL_POINTS);
    for (auto& s : stars)
        glVertex2f(s[0], s[1]);
    glEnd();
    glColor3f(1.0f, 1.0f, 0.5f);
    float bigStars[][2] = {{-0.55f,0.93f},{0.15f,0.90f},{0.80f,0.94f},{-0.20f,0.70f}};
    for (auto& s : bigStars) {
        glBegin(GL_LINES);
        glVertex2f(s[0]-0.015f, s[1]); glVertex2f(s[0]+0.015f, s[1]);
        glVertex2f(s[0], s[1]-0.015f); glVertex2f(s[0], s[1]+0.015f);
        glEnd();
    }
}

void saqlain_drawCloud(GLfloat ox, GLfloat oy) {
    glColor3f(1.0f, 1.0f, 1.0f);
    saqlain_drawFilledCircle(ox + 0.0f,  oy,        0.07f, 30);
    saqlain_drawFilledCircle(ox + 0.07f, oy + 0.03f,0.08f, 30);
    saqlain_drawFilledCircle(ox + 0.15f, oy,        0.065f,30);
    saqlain_drawFilledCircle(ox + 0.21f, oy + 0.02f,0.055f,30);
}

void saqlain_drawBat() {
    GLfloat bx = saqlain_batX, by = 0.72f;
    GLfloat w = 0.04f + saqlain_batWing * 0.03f;
    glColor3f(0.2f, 0.0f, 0.3f);
    saqlain_drawFilledCircle(bx, by, 0.025f, 20);
    glColor3f(0.25f, 0.0f, 0.35f);
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(bx, by);
    glVertex2f(bx - 0.13f, by + w);
    glVertex2f(bx - 0.09f, by - 0.01f);
    glVertex2f(bx - 0.05f, by + w * 0.5f);
    glVertex2f(bx, by);
    glEnd();
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(bx, by);
    glVertex2f(bx + 0.13f, by + w);
    glVertex2f(bx + 0.09f, by - 0.01f);
    glVertex2f(bx + 0.05f, by + w * 0.5f);
    glVertex2f(bx, by);
    glEnd();
    glColor3f(0.2f, 0.0f, 0.3f);
    glBegin(GL_TRIANGLES);
    glVertex2f(bx - 0.015f, by + 0.025f);
    glVertex2f(bx - 0.025f, by + 0.05f);
    glVertex2f(bx - 0.005f, by + 0.025f);
    glVertex2f(bx + 0.005f, by + 0.025f);
    glVertex2f(bx + 0.025f, by + 0.05f);
    glVertex2f(bx + 0.015f, by + 0.025f);
    glEnd();
}

void saqlain_drawRain() {
    glColor3f(0.5f, 0.7f, 1.0f);
    glLineWidth(1.0f);
    glBegin(GL_LINES);
    for (int i = 0; i < SAQLAIN_NUM_DROPS; i++) {
        glVertex2f(saqlain_rainX[i], saqlain_rainY[i]);
        glVertex2f(saqlain_rainX[i] + 0.008f, saqlain_rainY[i] - 0.04f);
    }
    glEnd();
}

void saqlain_drawTree(GLfloat tx, GLfloat ty) {
    glColor3f(0.4f, 0.25f, 0.05f);
    glBegin(GL_QUADS);
    glVertex2f(tx - 0.018f, ty - 0.10f);
    glVertex2f(tx + 0.018f, ty - 0.10f);
    glVertex2f(tx + 0.018f, ty + 0.02f);
    glVertex2f(tx - 0.018f, ty + 0.02f);
    glEnd();
    glColor3f(0.1f, 0.5f, 0.1f);
    saqlain_drawFilledCircle(tx, ty + 0.08f, 0.09f, 30);
}

void saqlain_drawLamp(GLfloat lx) {
    glColor3f(0.55f, 0.55f, 0.6f);
    glLineWidth(3.0f);
    glBegin(GL_LINES);
    glVertex2f(lx, -0.10f);
    glVertex2f(lx, 0.23f);
    glEnd();
    glBegin(GL_LINES);
    glVertex2f(lx, 0.23f);
    glVertex2f(lx + 0.04f, 0.26f);
    glEnd();
    glColor3f(1.0f, 0.95f, 0.5f);
    saqlain_drawFilledCircle(lx + 0.04f, 0.265f, 0.025f, 20);
    if (!saqlain_isDay) {
        glColor4f(1.0f, 0.95f, 0.5f, 0.15f);
        saqlain_drawFilledCircle(lx + 0.04f, 0.20f, 0.12f, 30);
    }
}

void saqlain_drawGrassTufts(GLfloat baseX, GLfloat baseY) {
    struct TuftEntry { GLfloat ox, h; };
    TuftEntry tufts[] = {
        {-0.15f, 0.040f}, {-0.09f, 0.032f}, {-0.03f, 0.044f},
        { 0.03f, 0.036f}, { 0.09f, 0.042f}, { 0.15f, 0.030f}
    };
    for (int t = 0; t < 6; t++) {
        GLfloat tx = baseX + tufts[t].ox;
        GLfloat ty = baseY;
        GLfloat h  = tufts[t].h;
        glLineWidth(1.6f);
        glColor3f(0.12f, 0.52f, 0.08f);
        glBegin(GL_LINES);
        glVertex2f(tx - 0.008f, ty);
        glVertex2f(tx - 0.022f, ty + h);
        glEnd();
        glColor3f(0.18f, 0.60f, 0.12f);
        glBegin(GL_LINES);
        glVertex2f(tx, ty);
        glVertex2f(tx, ty + h + 0.008f);
        glEnd();
        glColor3f(0.12f, 0.52f, 0.08f);
        glBegin(GL_LINES);
        glVertex2f(tx + 0.008f, ty);
        glVertex2f(tx + 0.022f, ty + h);
        glEnd();
    }
    glLineWidth(1.0f);
}

void saqlain_drawHouse(GLfloat hx, GLfloat hy,
                       GLfloat wr, GLfloat wg, GLfloat wb,
                       GLfloat rr, GLfloat rg, GLfloat rb) {
    GLfloat w = 0.18f, h = 0.18f;
    glColor3f(wr, wg, wb);
    glBegin(GL_QUADS);
    glVertex2f(hx - w, hy);
    glVertex2f(hx + w, hy);
    glVertex2f(hx + w, hy + h);
    glVertex2f(hx - w, hy + h);
    glEnd();
    glColor3f(rr, rg, rb);
    glBegin(GL_TRIANGLES);
    glVertex2f(hx - w - 0.02f, hy + h);
    glVertex2f(hx + w + 0.02f, hy + h);
    glVertex2f(hx,              hy + h + 0.12f);
    glEnd();
    glColor3f(0.35f, 0.18f, 0.05f);
    glBegin(GL_QUADS);
    glVertex2f(hx - 0.04f, hy);
    glVertex2f(hx + 0.04f, hy);
    glVertex2f(hx + 0.04f, hy + 0.08f);
    glVertex2f(hx - 0.04f, hy + 0.08f);
    glEnd();
    glColor3f(1.0f, 1.0f, 0.5f);
    glBegin(GL_QUADS);
    glVertex2f(hx + 0.07f, hy + 0.06f);
    glVertex2f(hx + 0.15f, hy + 0.06f);
    glVertex2f(hx + 0.15f, hy + 0.14f);
    glVertex2f(hx + 0.07f, hy + 0.14f);
    glEnd();
    glColor3f(0.5f, 0.4f, 0.2f);
    glLineWidth(1.5f);
    glBegin(GL_LINES);
    glVertex2f(hx + 0.11f, hy + 0.06f);
    glVertex2f(hx + 0.11f, hy + 0.14f);
    glVertex2f(hx + 0.07f, hy + 0.10f);
    glVertex2f(hx + 0.15f, hy + 0.10f);
    glEnd();
}

void saqlain_drawCar(GLfloat cx) {
    GLfloat cy = -0.52f;
    glColor3f(0.8f, 0.1f, 0.1f);
    glBegin(GL_QUADS);
    glVertex2f(cx - 0.18f, cy);
    glVertex2f(cx + 0.18f, cy);
    glVertex2f(cx + 0.18f, cy + 0.08f);
    glVertex2f(cx - 0.18f, cy + 0.08f);
    glEnd();
    glColor3f(0.6f, 0.08f, 0.08f);
    glBegin(GL_QUADS);
    glVertex2f(cx - 0.10f, cy + 0.08f);
    glVertex2f(cx + 0.10f, cy + 0.08f);
    glVertex2f(cx + 0.08f, cy + 0.14f);
    glVertex2f(cx - 0.08f, cy + 0.14f);
    glEnd();
    glColor3f(0.6f, 0.85f, 1.0f);
    glBegin(GL_QUADS);
    glVertex2f(cx - 0.08f, cy + 0.085f);
    glVertex2f(cx - 0.01f, cy + 0.085f);
    glVertex2f(cx - 0.02f, cy + 0.13f);
    glVertex2f(cx - 0.07f, cy + 0.13f);
    glEnd();
    glBegin(GL_QUADS);
    glVertex2f(cx + 0.01f, cy + 0.085f);
    glVertex2f(cx + 0.08f, cy + 0.085f);
    glVertex2f(cx + 0.07f, cy + 0.13f);
    glVertex2f(cx + 0.02f, cy + 0.13f);
    glEnd();
    GLfloat wheelCenters[2][2] = {{cx - 0.10f, cy}, {cx + 0.10f, cy}};
    for (int w = 0; w < 2; w++) {
        GLfloat wx = wheelCenters[w][0];
        GLfloat wy = wheelCenters[w][1];
        GLfloat outerR = 0.052f;
        GLfloat innerR = 0.028f;
        GLfloat hubR   = 0.010f;
        glColor3f(0.12f, 0.12f, 0.12f);
        saqlain_drawFilledCircle(wx, wy, outerR, 28);
        glColor3f(0.25f, 0.25f, 0.25f);
        glLineWidth(2.0f);
        saqlain_drawCircleOutline(wx, wy, outerR - 0.005f, 28);
        glColor3f(0.70f, 0.70f, 0.72f);
        saqlain_drawFilledCircle(wx, wy, innerR, 24);
        glColor3f(0.50f, 0.50f, 0.55f);
        glLineWidth(1.8f);
        int numSpokes = 6;
        for (int s = 0; s < numSpokes; s++) {
            float angle = (saqlain_wheelAngle + s * 360.0f / numSpokes) * (float)PI / 180.0f;
            glBegin(GL_LINES);
            glVertex2f(wx + hubR * cos(angle),   wy + hubR * sin(angle));
            glVertex2f(wx + innerR * cos(angle), wy + innerR * sin(angle));
            glEnd();
        }
        glLineWidth(1.0f);
        glColor3f(0.85f, 0.85f, 0.88f);
        saqlain_drawFilledCircle(wx, wy, hubR, 14);
        glColor3f(0.40f, 0.40f, 0.42f);
        saqlain_drawFilledCircle(wx, wy, 0.004f, 8);
    }
    glColor3f(1.0f, 1.0f, 0.7f);
    saqlain_drawFilledCircle(cx + 0.18f, cy + 0.03f, 0.018f, 15);
    glColor3f(1.0f, 0.1f, 0.1f);
    saqlain_drawFilledCircle(cx - 0.18f, cy + 0.03f, 0.015f, 15);
}

void saqlain_drawButtons() {
    if (saqlain_isDay)
        glColor3f(0.9f, 0.8f, 0.1f);
    else
        glColor3f(0.5f, 0.45f, 0.1f);
    glBegin(GL_QUADS);
    glVertex2f(-1.0f, -0.96f); glVertex2f(-0.68f,-0.96f);
    glVertex2f(-0.68f,-0.82f); glVertex2f(-1.0f, -0.82f);
    glEnd();
    glColor3f(0.0f,0.0f,0.0f);
    glRasterPos2f(-0.97f, -0.91f);
    const char* d = "DAY [D]";
    for (int i = 0; d[i]; i++) glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, d[i]);

    if (!saqlain_isDay)
        glColor3f(0.3f, 0.3f, 0.8f);
    else
        glColor3f(0.15f, 0.15f, 0.45f);
    glBegin(GL_QUADS);
    glVertex2f(-0.63f,-0.96f); glVertex2f(-0.28f,-0.96f);
    glVertex2f(-0.28f,-0.82f); glVertex2f(-0.63f,-0.82f);
    glEnd();
    glColor3f(1.0f,1.0f,1.0f);
    glRasterPos2f(-0.61f, -0.91f);
    const char* n = "NIGHT [N]";
    for (int i = 0; n[i]; i++) glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, n[i]);

    if (saqlain_rainOn)
        glColor3f(0.2f, 0.5f, 0.9f);
    else
        glColor3f(0.2f, 0.3f, 0.55f);
    glBegin(GL_QUADS);
    glVertex2f(-0.23f,-0.96f); glVertex2f( 0.12f,-0.96f);
    glVertex2f( 0.12f,-0.82f); glVertex2f(-0.23f,-0.82f);
    glEnd();
    glColor3f(1.0f,1.0f,1.0f);
    glRasterPos2f(-0.21f, -0.91f);
    const char* r = saqlain_rainOn ? "RAIN ON [R]" : "RAIN [R]";
    for (int i = 0; r[i]; i++) glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, r[i]);

    glColor3f(0.7f, 0.7f, 0.7f);
    glRasterPos2f(0.2f, -0.91f);
    const char* sp = "CAR SPEED: UP/DOWN arrows";
    for (int i = 0; sp[i]; i++) glutBitmapCharacter(GLUT_BITMAP_HELVETICA_10, sp[i]);
}

void saqlain_display() {
    glClear(GL_COLOR_BUFFER_BIT);
    glLoadIdentity();

    if (saqlain_isDay)
        glColor3f(0.45f, 0.75f, 1.0f);
    else
        glColor3f(0.05f, 0.05f, 0.18f);
    glBegin(GL_QUADS);
    glVertex2f(-1.0f, -0.12f); glVertex2f( 1.0f, -0.12f);
    glVertex2f( 1.0f,  1.0f);  glVertex2f(-1.0f,  1.0f);
    glEnd();

    if (saqlain_isDay) {
        saqlain_drawSun();
        saqlain_drawCloud(saqlain_cloud1X, 0.82f);
        saqlain_drawCloud(saqlain_cloud2X, 0.72f);
    } else {
        saqlain_drawStars();
        saqlain_drawMoon();
        saqlain_drawBat();
    }

    if (saqlain_isDay)
        glColor3f(0.15f, 0.55f, 0.1f);
    else
        glColor3f(0.05f, 0.22f, 0.05f);
    glBegin(GL_QUADS);
    glVertex2f(-1.0f, -0.12f); glVertex2f( 1.0f, -0.12f);
    glVertex2f( 1.0f, -0.42f); glVertex2f(-1.0f, -0.42f);
    glEnd();

    glColor3f(0.20f, 0.20f, 0.20f);
    glBegin(GL_QUADS);
    glVertex2f(-1.0f, -0.42f); glVertex2f( 1.0f, -0.42f);
    glVertex2f( 1.0f, -0.75f); glVertex2f(-1.0f, -0.75f);
    glEnd();
    glColor3f(0.9f, 0.9f, 0.9f);
    glLineWidth(2.0f);
    glBegin(GL_LINES);
    glVertex2f(-1.0f, -0.42f); glVertex2f(1.0f, -0.42f);
    glVertex2f(-1.0f, -0.75f); glVertex2f(1.0f, -0.75f);
    glEnd();
    glColor3f(0.85f, 0.85f, 0.85f);
    glLineWidth(2.5f);
    float dashX = -1.0f;
    while (dashX < 1.0f) {
        glBegin(GL_LINES);
        glVertex2f(dashX, -0.585f);
        glVertex2f(dashX + 0.1f, -0.585f);
        glEnd();
        dashX += 0.18f;
    }

    glColor3f(0.55f, 0.55f, 0.55f);
    glBegin(GL_QUADS);
    glVertex2f(-1.0f, -0.75f); glVertex2f( 1.0f, -0.75f);
    glVertex2f( 1.0f, -0.82f); glVertex2f(-1.0f, -0.82f);
    glEnd();

    saqlain_drawHouse(-0.75f, -0.12f,  0.85f,0.65f,0.1f,  0.75f,0.1f,0.1f);
    saqlain_drawGrassTufts(-0.75f, -0.12f);
    saqlain_drawHouse(-0.28f, -0.12f,  0.85f,0.80f,0.65f,  0.42f,0.25f,0.08f);
    saqlain_drawGrassTufts(-0.28f, -0.12f);
    saqlain_drawHouse( 0.22f, -0.12f,  0.28f,0.48f,0.78f,  0.75f,0.1f,0.1f);
    saqlain_drawGrassTufts( 0.22f, -0.12f);
    saqlain_drawHouse( 0.70f, -0.12f,  0.95f,0.68f,0.72f,  0.85f,0.55f,0.1f);
    saqlain_drawGrassTufts( 0.70f, -0.12f);

    saqlain_drawTree(-0.55f, -0.03f);
    saqlain_drawTree(-0.05f, -0.03f);
    saqlain_drawTree( 0.45f, -0.03f);
    saqlain_drawTree( 0.93f, -0.03f);

    saqlain_drawLamp(-0.44f);
    saqlain_drawLamp( 0.55f);

    saqlain_drawCar(saqlain_carX);

    if (saqlain_rainOn)
        saqlain_drawRain();

    saqlain_drawButtons();

    glutSwapBuffers();
}

void saqlain_update(int value) {
    saqlain_carX += saqlain_carSpeed;
    if (saqlain_carX > 1.3f) saqlain_carX = -1.3f;
    saqlain_wheelAngle -= saqlain_carSpeed * (180.0f / ((float)PI * 0.052f));
    if (saqlain_wheelAngle < -360.0f) saqlain_wheelAngle += 360.0f;

    saqlain_cloud1X -= saqlain_cloudSpeed;
    if (saqlain_cloud1X < -1.4f) saqlain_cloud1X = 1.2f;
    saqlain_cloud2X -= saqlain_cloudSpeed * 0.7f;
    if (saqlain_cloud2X < -1.4f) saqlain_cloud2X = 1.2f;

    saqlain_batX += saqlain_batSpeed;
    if (saqlain_batX > 1.2f) saqlain_batX = -1.2f;
    saqlain_batWing += 0.15f * saqlain_batWingDir;
    if (saqlain_batWing > 1.0f) saqlain_batWingDir = -1;
    if (saqlain_batWing < 0.0f) saqlain_batWingDir =  1;

    if (saqlain_rainOn) {
        for (int i = 0; i < SAQLAIN_NUM_DROPS; i++) {
            saqlain_rainY[i] -= 0.025f;
            if (saqlain_rainY[i] < -0.82f) {
                saqlain_rainY[i] = 0.95f;
                saqlain_rainX[i] = -1.0f + (float)rand() / RAND_MAX * 2.0f;
            }
        }
    }

    glutSetWindow(saqlainWindow);
    glutPostRedisplay();
    glutTimerFunc(30, saqlain_update, 0);
}

void saqlain_keyboard(unsigned char key, int x, int y) {
    // ENTER — return to proposal
    if (key == 13) {
        screenState = 0;
        saqlain_isDay  = 1;
        saqlain_rainOn = 0;
        saqlain_carX   = -1.2f;
        saqlain_carSpeed = 0.008f;
        glutSetWindow(saqlainWindow);
        glutHideWindow();
        glutSetWindow(proposalWindow);
        glutShowWindow();
        glutDisplayFunc(displayProposal);
        glutPostRedisplay();
        return;
    }
    if (key == 'd' || key == 'D') { saqlain_isDay = 1;  }
    if (key == 'n' || key == 'N') { saqlain_isDay = 0;  }
    if (key == 'r' || key == 'R') saqlain_rainOn = !saqlain_rainOn;
    glutPostRedisplay();
}

void saqlain_specialKeys(int key, int x, int y) {
    if (key == GLUT_KEY_UP)   saqlain_carSpeed += 0.002f;
    if (key == GLUT_KEY_DOWN) {
        saqlain_carSpeed -= 0.002f;
        if (saqlain_carSpeed < 0.001f) saqlain_carSpeed = 0.001f;
    }
    glutPostRedisplay();
}

void saqlain_mouse(int button, int state, int mx, int my) {
    if (button != GLUT_LEFT_BUTTON || state != GLUT_DOWN) return;
    int W = glutGet(GLUT_WINDOW_WIDTH);
    int H = glutGet(GLUT_WINDOW_HEIGHT);
    float gx = (float)mx / W * 2.0f - 1.0f;
    float gy = 1.0f - (float)my / H * 2.0f;
    if (gx > -1.0f && gx < -0.68f && gy > -0.96f && gy < -0.82f) { saqlain_isDay = 1; }
    if (gx > -0.63f && gx < -0.28f && gy > -0.96f && gy < -0.82f) { saqlain_isDay = 0; }
    if (gx > -0.23f && gx < 0.12f && gy > -0.96f && gy < -0.82f) saqlain_rainOn = !saqlain_rainOn;
    glutPostRedisplay();
}

void saqlain_initRain() {
    for (int i = 0; i < SAQLAIN_NUM_DROPS; i++) {
        saqlain_rainX[i] = -1.0f + (float)rand() / RAND_MAX * 2.0f;
        saqlain_rainY[i] = -1.0f + (float)rand() / RAND_MAX * 2.0f;
    }
}

// ============================================================
//  Proposal keyboard handler
// ============================================================
void keyboardProposal(unsigned char key, int x, int y)
{
    glutSetWindow(proposalWindow);
    if (screenState == 0 && key == 13) {
        screenState = 1;
        glutDisplayFunc(displayKeyGuide);
        glutPostRedisplay();
        return;
    }
    if (screenState == 1 || screenState >= 10) {
        if (key == 'f' || key == 'F') {
            screenState = 2;
            glutHideWindow();
            glutSetWindow(villageWindow);
            glutShowWindow();
            glutPostRedisplay();
            return;
        }
        if (key == 'e' || key == 'E') {
            screenState = 12;
            city_elapsedTime  = 0;
            city_skyFactor    = 1.0f;
            city_windowFactor = 0.0f;
            city_cloudFactor  = 0.0f;
            city_sunY         = 0.82f;
            city_shipPos      = -0.3f;
            glutHideWindow();
            glutSetWindow(cityWindow);
            glutShowWindow();
            glutPostRedisplay();
            return;
        }
        if (key == 'y' || key == 'Y') {
            screenState         = 15;
            park_rainMode       = 0;
            park_nightMode      = 0;
            park_planePosition  = 0.05f;
            park_planeY         = 0.7f;
            park_cloudPosition1 = -0.75f;
            park_cloudPosition2 = -0.35f;
            park_cloudPosition3 =  0.70f;
            glutHideWindow();
            glutSetWindow(parkWindow);
            glutShowWindow();
            glutPostRedisplay();
            return;
        }
        if (key == 'a' || key == 'A') {
            screenState    = 16;
            abir_rain      = false;
            abir_carv      = false;
            abir_day       = true;
            abir_car1Speed = 0.5f;
            abir_car2Speed = 0.5f;
            glutHideWindow();
            glutSetWindow(abirWindow);
            glutShowWindow();
            glutPostRedisplay();
            return;
        }
        // ── G key: open Saqlain's Street Scene ──
        if (key == 'g' || key == 'G') {
            screenState      = 17;
            saqlain_isDay    = 1;
            saqlain_rainOn   = 0;
            saqlain_carX     = -1.2f;
            saqlain_carSpeed = 0.008f;
            glutHideWindow();
            glutSetWindow(saqlainWindow);
            glutShowWindow();
            glutPostRedisplay();
            return;
        }

    }

}

void mouseProposal(int button, int state, int x, int y) {}

// ============================================================
//  main
int main(int argc, char** argv)
{
    srand(time(NULL));
    glutInit(&argc, argv);

    // ── Village window (Farhan) ──
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(900, 600);
    glutInitWindowPosition(0, 0);
    villageWindow = glutCreateWindow("Village Scene - MD Farhan Shahriar");
    glutFullScreen();
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    gluOrtho2D(0, 10, 0, 8);
    glutDisplayFunc(display);
    glutTimerFunc(30, update, 0);
    glutKeyboardFunc(keyboard);
    glutHideWindow();
    initRain();

    // ── City View window (Eftikhar) ──
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(900, 600);
    glutInitWindowPosition(0, 0);
    cityWindow = glutCreateWindow("City View - MD Eftikhar Mahbub");
    glutFullScreen();
    gluOrtho2D(-1.0, 1.0, -1.0, 1.0);
    glutDisplayFunc(city_display);
    glutTimerFunc(30, city_update, 0);
    glutKeyboardFunc(city_keyboard);
    glutHideWindow();

    // ── Park Scene window (Yusuf) ──
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(900, 600);
    glutInitWindowPosition(0, 0);
    parkWindow = glutCreateWindow("Park Scene - MD Yusuf");
    glutFullScreen();
    glClearColor(0.53f, 0.81f, 0.92f, 1.0f);
    gluOrtho2D(-1.0, 1.0, -1.0, 1.0);
    glutDisplayFunc(park_display);
    glutTimerFunc(10, park_update, 0);
    glutTimerFunc(10, park_updateCloud, 0);
    glutKeyboardFunc(park_keyboard);
    glutSpecialFunc(park_SpecialInput);
    glutHideWindow();
    park_initRain();

    // ── Abir's City Scene window ──
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(900, 600);
    glutInitWindowPosition(0, 0);
    abirWindow = glutCreateWindow("City Scene - Abir Fazle Rabbi");
    glutFullScreen();
    glClearColor(0.0f, 0.7f, 1.5f, 0.0f);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0.0, 800.0, 100.0, 800.0);
    glMatrixMode(GL_MODELVIEW);
    glutDisplayFunc(abir_display);
    glutKeyboardFunc(abir_keyboard);
    glutMouseFunc(abir_mouse);
    glutTimerFunc(abir_ucounter, abir_update, 0);
    glutHideWindow();

    // ── Saqlain's Street Scene window ──
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(900, 600);
    glutInitWindowPosition(0, 0);
    saqlainWindow = glutCreateWindow("Street Scene - MD Golam Saqlain");
    glutFullScreen();
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(-1.0, 1.0, -1.0, 1.0);
    glMatrixMode(GL_MODELVIEW);
    glutDisplayFunc(saqlain_display);
    glutKeyboardFunc(saqlain_keyboard);
    glutSpecialFunc(saqlain_specialKeys);
    glutMouseFunc(saqlain_mouse);
    glutTimerFunc(30, saqlain_update, 0);
    glutHideWindow();
    saqlain_initRain();

    // ── Proposal window (shown first) ──
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(700, 720);
    glutInitWindowPosition(100, 50);
    proposalWindow = glutCreateWindow("City of Hours - Project Proposal");
    glutDisplayFunc(displayProposal);
    glutReshapeFunc(reshapeProposal);
    glutKeyboardFunc(keyboardProposal);
    glutMouseFunc(mouseProposal);

    glutMainLoop();
    return 0;
}
