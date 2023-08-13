#ifndef _OJBECT_H_
#define _OBJECT_H_
#include <iostream>
#include "math.h"
#include <GL/glut.h>

using namespace std;

class Object {
private:
	int r;
	int g;
	int b;
public:
	Object();
	Object(GLfloat x, GLfloat y);
	Object(GLfloat x, GLfloat y, GLfloat width, GLfloat height);
	Object(GLfloat x, GLfloat y, GLfloat width, GLfloat height, GLfloat xVel, GLfloat yVel);
	~Object();

	// 2D primitives
	void drawPoint(GLfloat x, GLfloat y, GLfloat size);
	void drawLine(GLfloat x1, GLfloat y1, GLfloat x2, GLfloat y2, GLfloat thickness);
	void drawTriangle(GLfloat x1, GLfloat y1,
		GLfloat x2, GLfloat y2,
		GLfloat x3, GLfloat y3);
	void drawQuad(GLfloat x1, GLfloat y1,
		GLfloat x2, GLfloat y2,
		GLfloat x3, GLfloat y3,
		GLfloat x4, GLfloat y4);
	void drawRect(GLfloat x, GLfloat y, GLfloat width, GLfloat height);
	void drawRegularPolygon(GLfloat cx, GLfloat cy, GLfloat radius, GLint side, GLfloat orientation, GLfloat width, GLfloat height);

	void drawSineCurve(GLfloat int0, GLfloat int1);
	void drawHollowCircle(GLfloat x, GLfloat y, GLfloat radius);
	void drawFilledCircle(GLfloat x, GLfloat y, GLfloat radius);
	void drawArcCircle(GLfloat x, GLfloat y, GLfloat radius, GLfloat percentage);
	void drawArcFilledCircle(GLfloat x, GLfloat y, GLfloat radius, GLfloat percentage);
	//bool drawfingerCircle(GLfloat cx, GLfloat cy, GLfloat radius, GLint sides, GLfloat rotation, GLfloat width, GLfloat height);

	// 2D transformation
	void translate(GLfloat tX, GLfloat tY);
	void rotate(GLfloat t, GLfloat pX, GLfloat pY);
	void scale(GLfloat sX, GLfloat sY);
	void scale(GLfloat scaleFactorX, GLfloat scaleFactorY, GLfloat scaleFactorZ, GLfloat pivotX, GLfloat pivotY, GLfloat pivotZ);
	void orbit(GLfloat cx, GLfloat cy, GLfloat radius, GLfloat& angle, GLfloat torque);

	// Set color
	void setColor(int r, int g, int b);
	//void setColor(int r, int g, int b, int opacity);

	//object declare 
	GLfloat x, y, width, height, xVel, yVel;
	GLboolean scaleOnce, enlarge;
	GLint signalType;
	GLint signalCounter;
	GLboolean largeArrow;
	GLboolean hasFilled;
	GLint speedUp;
	GLint click;
	GLboolean isFinish;
};

struct LT { // Local Transform
	GLdouble displaceAmt; // Translation  based on input
	GLdouble rotateAmt; // Rotation based on input
	GLint mouseX, mouseY;
	GLboolean leftMouseIsPressed, rightMouseIsPressed;
};

struct WT { // World Transform
	GLdouble tX, tY, tZ;
	GLdouble rotateX, rotateY, rotateZ;
	GLdouble sX, sY, sZ;
	void rotate(GLfloat dX, GLfloat dY, GLfloat dZ) { tX += dX; tY += dY; tZ += dZ; }
	void translate(GLfloat dX, GLfloat dY, GLfloat dZ) { tX += dX; tY += dY; tZ += dZ; }
};

struct Window {
	string title;
	GLint offsetX, offsetY;
	GLint width, height;
};

struct ViewFrustum {
	GLdouble eyeX, eyeY, eyeZ;
	GLdouble refX, refY, refZ;
	GLdouble upX, upY, upZ;
	GLdouble nearZ, farZ;
	GLdouble fov; // Angle in Y direction.
	GLdouble ar;
};

#endif
