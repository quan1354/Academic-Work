#include <windows.h>
#include "object.h"
#include <stdio.h>
#include <gl/glut.h>

using namespace std;

GLfloat tNeedleAngle = 110.0f;
GLfloat tAngle = 110.0f;
GLfloat sNeedleAngle = 60.0f;
GLfloat sAngle = 60.0f;
GLfloat tAngle2;
GLfloat sAngle2;

int xNavigateComplete = 0;
GLfloat xNavigate = 0;
GLfloat xNavigate2 = 0;

Object speedometer = Object();
Object tachometer = Object();
Object infoColumn = Object();
Object gearInfo = Object();
Object signal = Object();
Object map = Object();
Object fuelLevel = Object(0, 200, 40, 105, 0.1, 0.1);

WT wt; // World transform
LT lt; // Local transform

Window window;
ViewFrustum viewer;

void windowSettings() {
	window.title = "Animation Car Dashboard";
	window.offsetX = 50;
	window.offsetY = 50;
	window.width = 1000;
	window.height = 700;
}

void spaceSettings() {
	viewer.eyeX = 0.0; viewer.eyeY = 20.0; viewer.eyeZ = 40.0;
	viewer.refX = 0.0; viewer.refY = 0.0; viewer.refZ = 0.0;
	viewer.upX = 0.0; viewer.upY = 1.0; viewer.upZ = 0.0;
	viewer.nearZ = 0.1; viewer.farZ = 500.0;
	viewer.fov = 60.0;
	viewer.ar = static_cast<GLdouble>(window.width / window.height);
}

void init() {
	fuelLevel.hasFilled = false;
	fuelLevel.enlarge = true;
	windowSettings();
	spaceSettings();

	wt.tX = 0.0; wt.tY = 0.0; wt.tZ = 0.0;
	wt.rotateX = 0.0; wt.rotateY = 0.0; wt.rotateZ = 0.0;
	wt.sX = 1.0; wt.sY = 1.0; wt.sZ = 1.0;
	lt.displaceAmt = 1.0; // the sensivity of input for translation
	lt.rotateAmt = 2.0; // the sensivity of input for rotation
}

void perspectiveView() {
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(viewer.fov, viewer.ar, viewer.nearZ, viewer.farZ);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(viewer.eyeX, viewer.eyeY, viewer.eyeZ,
		viewer.refX, viewer.refY, viewer.refZ,
		viewer.upX, viewer.upY, viewer.upZ);
}

void initAnimation() {
	windowSettings();
	spaceSettings();

	wt.tX = 0.0; wt.tY = 0.0; wt.tZ = 0.0;
	wt.rotateX = 0.0; wt.rotateY = 0.0; wt.rotateZ = 0.0;
	wt.sX = 1.0; wt.sY = 1.0; wt.sZ = 1.0;
	lt.displaceAmt = 1.0; // the sensivity of input for translation
	lt.rotateAmt = 2.0; // the sensivity of input for rotation
}

void mouseControl(GLint button, GLint state, GLint x, GLint y) {
	y = 1000 - y; // switch from window coordinates to canvas coordinate
	switch (button) {
	case GLUT_LEFT_BUTTON:
		if (state == GLUT_DOWN && !lt.leftMouseIsPressed) {
			lt.mouseX = x;
			lt.mouseY = y;
			lt.leftMouseIsPressed = true;
			lt.leftMouseIsPressed = true;
			map.click = 1;
			if (!fuelLevel.hasFilled) {
				fuelLevel.enlarge = false;
			}
		}
		if (state == GLUT_UP && lt.leftMouseIsPressed) {
			lt.leftMouseIsPressed = false;
		}
		break;
	case GLUT_RIGHT_BUTTON:
		if (state == GLUT_DOWN && !lt.rightMouseIsPressed) {
			lt.mouseX = x;
			lt.mouseY = y;
			lt.rightMouseIsPressed = true;
			lt.rightMouseIsPressed = true;
		}
		if (state == GLUT_UP && lt.rightMouseIsPressed) {
			lt.rightMouseIsPressed = false;
		}
		break;
	default:
		break;
	}
}

void mouseMoveControl(GLint x, GLint y) {
	y = 1000 - y;
	GLint dX = x - lt.mouseX;
	GLint dY = y - lt.mouseY;
	if (lt.leftMouseIsPressed) {
		wt.rotate(-dY, dX, 0.0f);
	}
	if (lt.rightMouseIsPressed) {
		wt.rotate(0.0f, 0.0f, dX);
	}
	lt.mouseX = x;
	lt.mouseY = y;
	glutPostRedisplay();
}

void keyboardControl(unsigned char key, GLint x, GLint y) {
	GLfloat displaceX, displaceY, displaceZ;
	displaceX = displaceY = displaceZ = 0.0;
	switch (key) {
	case 'w': case 'W': displaceZ = -lt.displaceAmt;
		break;
	case 's': case 'S': displaceZ = lt.displaceAmt;
		break;
	case 'a': case 'A': displaceX = -lt.displaceAmt;
		signal.signalType = 1;
		signal.signalCounter = 0;
		signal.largeArrow = true;
		break;
	case 'd': case 'D': displaceX = lt.displaceAmt;
		signal.signalType = 2;
		signal.signalCounter = 0;
		signal.largeArrow = true;
		break;
	case 27:initAnimation();
		signal.signalType = 0;
		signal.largeArrow = false;
		break;
	}
	wt.translate(displaceX, displaceY, displaceZ);
	glutPostRedisplay();
}


void keyboardSpecialControl(GLint key, GLint x, GLint y) {
	GLfloat displaceX, displaceY, displaceZ;
	displaceX = displaceY = displaceZ = 0.0;
	int count = 0;
	switch (key) {
	case GLUT_KEY_UP: displaceZ = -lt.displaceAmt;
		if (fuelLevel.height < 30 && xNavigateComplete == 5) {
			fuelLevel.height = 10;
			fuelLevel.enlarge = true;
		}

		if (fuelLevel.enlarge && fuelLevel.hasFilled == false && xNavigateComplete == 5) {
			fuelLevel.scale(1, 10.5);
			fuelLevel.hasFilled = true;
		}

		break;
	case GLUT_KEY_DOWN: lt.displaceAmt;
		break;
	case GLUT_KEY_LEFT: displaceX = -lt.displaceAmt;
		break;
	case GLUT_KEY_RIGHT: lt.displaceAmt;
		break;
	case GLUT_KEY_HOME:initAnimation();
		break;
	}
	wt.translate(displaceX, displaceY, displaceZ);
	glutPostRedisplay();
}


void gradientBackground() {
	glBegin(GL_QUADS);
	glColor3f(0.0f, 0.0f, 0.0f);//black
	glVertex2d(0, 0);

	glColor3f(0.0f, 0.0f, 0.6f);//dark blue
	glVertex2d(0, 700);

	glColor3f(0.0f, 0.0f, 0.6f);//dark blue
	glVertex2d(1000, 700);

	glColor3f(0.0f, 0.0f, 0.0f);//black
	glVertex2d(1000, 0);

	glEnd();
}

void drawText(const char* text, GLint length, GLint x, GLint y, void* font) {
	glMatrixMode(GL_PROJECTION);
	double* matrix = new double[16];
	glGetDoublev(GL_PROJECTION_MATRIX, matrix);
	glLoadIdentity();

	glOrtho(0, 900, 0, 700, -5, 5);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glPushMatrix();
	glLoadIdentity();
	glRasterPos2i(x, y);
	for (int i = 0; i < length; i++) {
		glutBitmapCharacter(font, (int)text[i]);
	}
	glPopMatrix();

	glMatrixMode(GL_PROJECTION);
	glLoadMatrixd(matrix);
	glMatrixMode(GL_MODELVIEW);
}

// ==========================Meter============================
void drawTachometerTicks(GLfloat radius, GLfloat cx, GLfloat cy) {
	float r = radius;
	//major ticks
	glLineWidth(4.0);
	glBegin(GL_LINES);
	int nTicks = 12;
	float angle = 360 / 14;

	for (int i = 0; i < nTicks; i++) {
		glVertex2f(cx + 80.0 * cos(i * angle * 3.14 / 180), cy + 80.0 * sin(i * angle * 3.14 / 180));
		glVertex2f(cx + r * cos(i * angle * 3.14 / 180), cy + r * sin(i * angle * 3.14 / 180));
	}
	glEnd();


	//minor ticks
	glLineWidth(2.0);
	glBegin(GL_LINES);
	for (int i = 0; i < ((nTicks) * 2 - 1); i++) {
		glVertex2f(cx + 90.0 * cos(i * (angle / 2) * 3.14 / 180), cy + 90.0 * sin(i * (angle / 2) * 3.14 / 180));
		glVertex2f(cx + r * cos(i * (angle / 2) * 3.14 / 180), cy + r * sin(i * (angle / 2) * 3.14 / 180));
	}
	glEnd();
}
void drawTachometerLabel(GLfloat radius, GLfloat cx, GLfloat cy) {
	int nTicks = 12;
	float tickLength = 20;
	radius = radius - 25;
	cx = cx - 5;
	cy = cy - 5;
	float angle = 360 / 14;
	int number = 12;
	char label[3];

	for (int i = 0; i < (nTicks); i++) {
		float x1 = cx + radius * cos(i * angle * 3.14 / 180);
		float y1 = cy + radius * sin(i * angle * 3.14 / 180);
		float x2 = cx + (radius - tickLength) * cos(i * angle * 3.14 / 180);
		float y2 = cy + (radius - tickLength) * sin(i * angle * 3.14 / 180);

		//number label
		number--;
		printf(label, "%d", number);
		glRasterPos2f(x2, y2);
		for (int j = 0; j < strlen(label); j++) {
			glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, label[j]);
		}
	}
}
void drawTachometer(GLfloat radius, GLfloat cx, GLfloat cy, GLfloat tx, GLfloat ty) {
	//tachometer dial
	glColor3f(1.0f, 1.0f, 1.0f); // Set white brush
	tachometer.drawArcCircle(cx, cy, radius + 5, 2);
	glColor3f(1.0f, 1.0f, 1.0f); // Set white brush
	tachometer.drawArcFilledCircle(cx, cy, radius + 2, 2);
	//tachometer dial
	glColor3f(0.0f, 0.0f, 0.0f); // Set black brush
	tachometer.drawArcFilledCircle(cx, cy, radius, 2);

	//unit label text
	glColor3f(1.0f, 1.0f, 1.0f);
	string text = "x1000r/min";
	drawText(text.data(), text.size(), 250, 145, GLUT_BITMAP_HELVETICA_10);

	//ticks and label
	glColor3f(1.0f, 1.0f, 1.0f);
	drawTachometerTicks(radius, cx, cy);
	//drawTachometerLabel(radius, cx, cy);

	////tachometer needle
	glColor3f(1.0f, 0.0f, 0.0f);

	if (tachometer.speedUp == 1) {
		tachometer.rotate(tAngle, cx, cy);
		tAngle2 = tAngle;
	}

	else if (tachometer.speedUp == 2) {
		tachometer.rotate(tAngle2, cx, cy);
	}

	else {
		tachometer.rotate(tNeedleAngle, cx, cy);
	}
	tachometer.drawTriangle(cx - 3, cy - 3, radius + 80, radius + 50, cx + 3, cy + 3);
	//tachometer centre
	glColor3f(1.0f, 1.0f, 1.0f); //set white brush
	tachometer.drawFilledCircle(cx, cy, 8);

	glLoadIdentity();

}

void tDecrementAngle(int) {
	if (tAngle > 0.0f) {
		tAngle -= 2.5f;
		glutPostRedisplay();
		glutTimerFunc(40, tDecrementAngle, 0);
	}
	else if (tAngle2 < 110.0f) {
		tAngle2 += 3.5f;
		glutPostRedisplay();
		glutTimerFunc(40, tDecrementAngle, 0);
	}
}
void drawSpeedometerTicks() {
	//major ticks
	glLineWidth(5.0);
	glBegin(GL_LINES);
	int nTicks = 11;
	float angle = 360 / 15;
	for (int i = 0; i < nTicks; i++) {
		glVertex2f(300 + 120.0 * cos(i * angle * 3.14 / 180), 180 + 120.0 * sin(i * angle * 3.14 / 180));
		glVertex2f(300 + 160.0 * cos(i * angle * 3.14 / 180), 180 + 160.0 * sin(i * angle * 3.14 / 180));
	}
	glEnd();

	//minor ticks
	glLineWidth(3.0);
	glBegin(GL_LINES);
	for (int i = 0; i < ((nTicks) * 2 - 1); i++) {
		glVertex2f(300 + 125.0 * cos(i * (angle / 2) * 3.14 / 180), 180 + 125.0 * sin(i * (angle / 2) * 3.14 / 180));
		glVertex2f(300 + 155.0 * cos(i * (angle / 2) * 3.14 / 180), 180 + 155.0 * sin(i * (angle / 2) * 3.14 / 180));
	}
	glEnd();

	//more minor ticks XD
	glLineWidth(2.0);
	glBegin(GL_LINES);
	for (int i = 0; i < ((nTicks) * 4 - 3); i++) {
		glVertex2f(300 + 130.0 * cos(i * (angle / 4) * 3.14 / 180), 180 + 130.0 * sin(i * (angle / 4) * 3.14 / 180));
		glVertex2f(300 + 150.0 * cos(i * (angle / 4) * 3.14 / 180), 180 + 150.0 * sin(i * (angle / 4) * 3.14 / 180));
	}
	glEnd();
}
void drawSpeedometerLabel() {
	int nTicks = 11;
	float tickLength = 20;
	float radius = 180;
	float centerX = 285;
	float centerY = 390;
	float angle = 360 / 15;
	int number = 220;
	char label[3];

	for (int i = 0; i < (nTicks); i++) {
		float x1 = centerX + radius * cos(i * angle * 3.14 / 180);
		float y1 = centerY + radius * sin(i * angle * 3.14 / 180);
		float x2 = centerX + (radius - tickLength) * cos(i * angle * 3.14 / 180);
		float y2 = centerY + (radius - tickLength) * sin(i * angle * 3.14 / 180);

		//number label
		number -= 20;
		printf(label, "%d", number);
		glRasterPos2f(x2, y2);
		for (int j = 0; j < strlen(label); j++) {
			glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, label[j]);
		}
	}
}
void drawSpeedometer(GLfloat radius, GLfloat cx, GLfloat cy, GLfloat tx, GLfloat ty) {
	glTranslatef(tx, ty, 0);
	//speedometer dial
	glColor3f(1.0f, 1.0f, 1.0f); // Set white brush
	speedometer.drawArcCircle(cx, cy, radius + 5, 1.35);
	glColor3f(1.0f, 1.0f, 1.0f); // Set white brush
	speedometer.drawArcFilledCircle(cx, cy, radius + 2, 1.35);
	//speedometer dial
	glColor3f(0.0f, 0.0f, 0.0f); // Set black brush
	speedometer.drawFilledCircle(cx, cy, radius);

	//speedometer ticks and label
	glColor3f(1.0f, 1.0f, 1.0f);
	drawSpeedometerTicks();
	//drawSpeedometerLabel();
	//speedometer needle
	glColor3f(1.0f, 0.0f, 0.0f);
	//===================================================//
	if (speedometer.speedUp == 1) {
		speedometer.rotate(sAngle, cx, cy);
		sAngle2 = sAngle;
	}

	else if (speedometer.speedUp == 2) {
		speedometer.rotate(sAngle2, cx, cy);
	}

	else {
		speedometer.rotate(sNeedleAngle, cx, cy);
	}

	speedometer.drawTriangle(cx - 6, cy - 6, radius - 20, radius + 20, cx + 6, cy + 6);
	////speedometer centre
	glColor3f(1.0f, 1.0f, 1.0f); //set white brush
	speedometer.drawFilledCircle(cx, cy, 10);

	////speedometer unit label text
	glColor3f(1.0f, 1.0f, 1.0f);
	string text = "km/h";
	drawText(text.data(), text.size(), 440, 200, GLUT_BITMAP_HELVETICA_18);
	glLoadIdentity();
}

void sDecrementAngle(int) {
	if (sAngle > 0.0f) {
		sAngle -= 1.5f;
		glutPostRedisplay();
		glutTimerFunc(50, sDecrementAngle, 0);
	}

	else if (sAngle2 < 60.0f) {
		sAngle2 += 2.5f;
		glutPostRedisplay();
		glutTimerFunc(40, sDecrementAngle, 0);
	}
}
// ==========================Status Table============================
void drawFuelTank() {
	if (fuelLevel.height >= 40 && fuelLevel.height < 80) {
		fuelLevel.setColor(255, 187, 0);
	}
	else if (fuelLevel.height >= 0 && fuelLevel.height < 50) {
		fuelLevel.setColor(255, 0, 0);
	}
	else {
		fuelLevel.setColor(69, 227, 48);
	}

	if (fuelLevel.height >= 30 && fuelLevel.enlarge == false) {
		fuelLevel.scale(1, 0.997);
	}

	fuelLevel.drawRect(890, 500, fuelLevel.width, fuelLevel.height);

	float x1 = 730 + 150;
	float y1 = 500;
	float x2 = 730 + 150;
	float y2 = 605;

	glColor3f(1.0f, 1.0f, 1.0f);
	infoColumn.drawLine(x1, y1, x2, y2, 2);

	//fuel tank label
	glColor3f(1.0f, 1.0f, 1.0f);
	string text = "E";
	drawText(text.data(), text.size(), 790, y1 - 15, GLUT_BITMAP_HELVETICA_12);
	glLoadIdentity();

	text = "F";
	drawText(text.data(), text.size(), 790, y2 + 10, GLUT_BITMAP_HELVETICA_12);
	glLoadIdentity();

	//Odometer
	text = "ODO";
	drawText(text.data(), text.size(), 670, 530, GLUT_BITMAP_HELVETICA_12);
	glLoadIdentity();

	text = "145227";
	drawText(text.data(), text.size(), 675, 505, GLUT_BITMAP_HELVETICA_18);
	glLoadIdentity();

	text = "21 MAR 2023";
	drawText(text.data(), text.size(), 675, 590, GLUT_BITMAP_HELVETICA_18);
	glLoadIdentity();

	text = "20:45";
	drawText(text.data(), text.size(), 675, 560, GLUT_BITMAP_HELVETICA_18);
	glLoadIdentity();
}
void drawInfoColumn(GLfloat w, GLfloat h) {
	glBegin(GL_QUADS);
	glColor3ub(51, 51, 51);
	glVertex2i(730, 480);
	glColor3ub(0, 0, 0);
	glVertex2i(w + 730, 480);
	glColor3ub(0, 0, 0);
	glVertex2i(w + 730, h + 480);
	glColor3ub(51, 51, 51);
	glVertex2i(730, h + 480);
	glEnd();
}
void drawInfoColumn(GLfloat w, GLfloat h, boolean color) {
	glBegin(GL_QUADS);
	glColor3ub(0, 0, 0);
	glVertex2i(730, 480);
	glVertex2i(w + 730, 480);
	glVertex2i(w + 730, h + 480);
	glVertex2i(730, h + 480);
	glEnd();
	drawFuelTank();
}

// ===========================Signals=========================
void renderArrow(GLfloat pos_x, GLfloat pos_y, GLfloat scalefac) {
	/*
				   g
				  |\
			  a---d \
			  |      \
			  o       f
			  |      /
			  b---c /
				  |/      o:center
				  e
	*/
	// Rect abcd
	GLfloat a[] = { pos_x + 0, pos_y + 2 };
	GLfloat b[] = { pos_x + 0, pos_y - 2 };
	GLfloat c[] = { pos_x + 3, pos_y - 2 };
	GLfloat d[] = { pos_x + 3, pos_y + 2 };
	// Triangle efg
	GLfloat e[] = { pos_x + 3, pos_y - 4 };
	GLfloat f[] = { pos_x + 6, pos_y + 0 };
	GLfloat g[] = { pos_x + 3, pos_y + 4 };

	signal.scale(scalefac, scalefac, scalefac, pos_x, pos_y, 0); // Scale up.
	signal.drawQuad(a[0], a[1], b[0], b[1], c[0], c[1], d[0], d[1]); // Draw rect abcd.
	signal.drawTriangle(e[0], e[1], f[0], f[1], g[0], g[1]); // Draw triangle efg.
}
void renderRightArrow(GLfloat pos_x, GLfloat pos_y, GLfloat scaleSmall, GLfloat scaleLarge) {
	const bool big = true;
	const int i = 0;

	glPushMatrix();
	if (signal.signalType == 2) {
		glColor3f(0.0f, 1.0f, 0.0f); // Set green brush.
		if (signal.largeArrow) {
			renderArrow(pos_x, pos_y, scaleLarge);
		}
		else {
			renderArrow(pos_x, pos_y, scaleSmall);
		}
	}
	else {
		glColor3f(1.0f, 1.0f, 0.6f); // Set yellow brush.	
		renderArrow(pos_x, pos_y, scaleSmall);
	}
	glPopMatrix();
}
void renderLeftArrow(GLfloat pos_x, GLfloat pos_y, GLfloat scaleSmall, GLfloat scaleLarge) {
	glPushMatrix();
	if (signal.signalType == 1) {
		glColor3f(0.0f, 1.0f, 0.0f); // Set green brush.
		if (signal.largeArrow) {
			signal.rotate(180, pos_x, pos_y);  // Rotate right arrow 180 degree.
			renderArrow(pos_x, pos_y, scaleLarge);
		}
		else {
			signal.rotate(180, pos_x, pos_y); // Rotate right arrow 180 degree.
			renderArrow(pos_x, pos_y, scaleSmall);
		}
	}
	else {
		glColor3f(1.0f, 1.0f, 0.6f); // Set yellow brush.
		signal.rotate(180, pos_x, pos_y); // Rotate right arrow 180 degree.
		renderArrow(pos_x, pos_y, scaleSmall);
	}
	glPopMatrix();
}
void drawArrows() {
	if (signal.signalType != 0) signal.signalCounter++;
	if (signal.signalCounter >= 10) {
		signal.signalCounter = 0;
		signal.largeArrow = !signal.largeArrow;
	}
	renderRightArrow(600, 530, 6, 8);
	renderLeftArrow(500, 530, 6, 8);
}


// =============================Map=========================

void xNavigatePosition(int) {
	if (xNavigate <= 135) {
		xNavigate += 1;
		glutPostRedisplay();
		glutTimerFunc(50, xNavigatePosition, 0);
		xNavigateComplete = 0;
	}
	else if (xNavigate >= 135 && xNavigate < 270) {
		xNavigate += 1;
		glutPostRedisplay();
		glutTimerFunc(50, xNavigatePosition, 0);
		xNavigateComplete = 1;
	}

	else if (xNavigate >= 269 && xNavigate < 366) {
		xNavigate += 1;
		glutPostRedisplay();
		glutTimerFunc(50, xNavigatePosition, 0);
		xNavigateComplete = 2;
	}

	else if (xNavigate >= 365 && xNavigate < 367) {
		xNavigate += 1;
		glutPostRedisplay();
		glutTimerFunc(50, xNavigatePosition, 0);
		xNavigateComplete = 3;
	}

	else if (xNavigate >= 366 && xNavigate2 <= 140) {
		xNavigateComplete = 4;
		xNavigate2 += 1;
		glutPostRedisplay();
		glutTimerFunc(50, xNavigatePosition, 0);
	}
	else if (xNavigate2 > 139) {
		xNavigateComplete = 5;
	}

}

void drawMaps() {
	map.setColor(194, 183, 155);
	map.drawRect(15, 350, 370, 340);

	// Map of Line
	map.setColor(235, 26, 120);
	map.drawLine(65, 390, 200, 390, 5);
	map.drawLine(200, 388, 200, 623, 5);
	map.drawLine(200, 620, 340, 620, 5);
	glLoadIdentity();

	// Map of Pointer start
	map.setColor(77, 76, 76);
	map.setColor(33, 33, 255);
	map.drawRegularPolygon(65, 390, 8, 360, 0, 1.0f, 1.0f);
	glLoadIdentity();

	// Map of Navigator
	map.setColor(18, 41, 252);
	if (map.click == 1 && xNavigateComplete == 0) {
		speedometer.speedUp = 1;
		tachometer.speedUp = 1;
		map.drawTriangle(50 + xNavigate, 370, 50 + xNavigate, 410, 85 + xNavigate, 390);
		glLoadIdentity();
	}
	else if (map.click == 1 && xNavigateComplete == 1) {
		map.rotate(90, 200, 390);
		map.drawTriangle(50 + xNavigate, 370, 50 + xNavigate, 410, 85 + xNavigate, 390);
		glLoadIdentity();
	}
	else if (map.click == 1 && xNavigateComplete == 2) {
		map.rotate(90, 200, 390);
		map.drawTriangle(50 + xNavigate, 370, 50 + xNavigate, 410, 85 + xNavigate, 390);
		glLoadIdentity();
	}
	else if (map.click == 1 && xNavigateComplete == 3) {
		map.drawTriangle(185, 600, 185, 640, 220, 620);
		glLoadIdentity();
	}

	else if (map.click == 1 && xNavigateComplete == 4) {
		map.drawTriangle(185 + xNavigate2, 600, 185 + xNavigate2, 640, 220 + xNavigate2, 620);
		glLoadIdentity();
	}

	else if (map.click == 1 && xNavigateComplete == 5) {
		speedometer.speedUp = 2;
		tachometer.speedUp = 2;
		map.drawTriangle(185 + xNavigate2, 600, 185 + xNavigate2, 640, 220 + xNavigate2, 620);
		//map.isFinish = true;
		glLoadIdentity();
	}

	else {
		map.drawTriangle(50, 370, 50, 410, 85, 390);
		glLoadIdentity();
	}
	map.translate(200, 600);
	glLoadIdentity();

	// Map of Pointer end
	map.setColor(77, 76, 76);
	map.drawLine(340, 618, 340, 650, 5);
	map.setColor(225, 33, 33);
	map.drawRegularPolygon(340, 655, 8, 340, 0, 1.0f, 1.0f);
	glLoadIdentity();


	// Map of fuel station
	map.setColor(242, 250, 20);
	map.drawRect(330, 560, 30, 40);
	map.setColor(255, 255, 255);
	map.drawRect(325, 560, 40, 6);
	map.setColor(255, 146, 3);
	map.drawRect(335, 583, 20, 10);
	glLoadIdentity();

	// Texts
	map.setColor(255, 255, 255);
	string text = "Fuel Station";
	drawText(text.data(), text.size(), 280, 545, GLUT_BITMAP_HELVETICA_12);
	glLoadIdentity();
}

// ==================Render and Main========================
void render() {
	glClearColor(0.0f, 1.0f, 1.0f, 1.0f); // white background
	// Canvas settings
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0, 1000, 0, 700); // Set canvas to 900—700 pixels.

	glClear(GL_COLOR_BUFFER_BIT); // Load frame buffer.

	// Render code here
	gradientBackground();
	//draw tachometer
	drawTachometer(110, 300, 130, -200, 100);
	//draw info column
	drawInfoColumn(220, 170);
	drawInfoColumn(200, 150, false);
	//draw speedometer
	drawSpeedometer(160, 300, 180, 200, 0);
	//draw signal arrows
	drawArrows();
	// draw map navigation
	drawMaps();

	glutSwapBuffers(); // Swap foreground and background frames
	glutPostRedisplay();// update canvas display
	glFlush();
	glFinish();
}

int main() {
	init();
	//glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(1000, 700);
	glutInitWindowPosition(0, 0);
	glutCreateWindow("Car dashboard");
	glutDisplayFunc(render); // Load render function.

	glutMouseFunc(mouseControl); // enable mouse button function
	glutMotionFunc(mouseMoveControl); // enable mouse move
	glutKeyboardFunc(keyboardControl); // enable escape keyboard
	glutSpecialFunc(keyboardSpecialControl);
	glutTimerFunc(0, tDecrementAngle, 0);
	glutTimerFunc(0, sDecrementAngle, 0);
	glutTimerFunc(0, xNavigatePosition, 0);
	perspectiveView(); // Set perspective view.
	glutMainLoop(); // Loop frame forever.

	system("PAUSE");
	return 0;
}
