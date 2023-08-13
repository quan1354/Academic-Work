#include "object.h"

// Default constructor
Object::Object() {}

Object::Object(GLfloat x, GLfloat y) {
	this->x = x;
	this->y = y;
}

Object::Object(GLfloat x, GLfloat y, GLfloat width, GLfloat height) {
	this->x = x;
	this->y = y;
	this->width = width;
	this->height = height;
	this->scaleOnce = false;
}

Object::Object(GLfloat x, GLfloat y, GLfloat width, GLfloat height, GLfloat xVel, GLfloat yVel) {
	this->x = x;
	this->y = y;
	this->width = width;
	this->height = height;
	this->scaleOnce = false;
	this->xVel = xVel;
	this->yVel = yVel;
	this->enlarge = false;
}

// Destructor
Object::~Object() {}

void Object::setColor(int r, int g, int b) {
	this->r = r;
	this->g = g;
	this->b = b;

	GLfloat red = (r % 256) / 255.0f;
	GLfloat green = (g % 256) / 255.0f;
	GLfloat blue = (b % 256) / 255.0f;
	glColor3f(red, green, blue);
}

void Object::drawPoint(GLfloat x, GLfloat y, GLfloat size) {
	glPushMatrix();
	glPointSize(size);
	glBegin(GL_POINTS);
	glVertex2i(x, y);
	glEnd();
	glPopMatrix();
}

void Object::drawLine(GLfloat x1, GLfloat y1, GLfloat x2, GLfloat y2, GLfloat thickness) {
	glPushMatrix();
	glLineWidth(thickness);
	glBegin(GL_LINES);
	glVertex2i(x1, y1);
	glVertex2i(x2, y2);
	glEnd();
	glPopMatrix();
}

void Object::drawTriangle(GLfloat x1, GLfloat y1,
	GLfloat x2, GLfloat y2,
	GLfloat x3, GLfloat y3) {
	glPushMatrix();
	glBegin(GL_TRIANGLES);
	glVertex2i(x1, y1);
	glVertex2i(x2, y2);
	glVertex2i(x3, y3);
	glEnd();
	glPopMatrix();
}

void Object::drawQuad(GLfloat x1, GLfloat y1,
	GLfloat x2, GLfloat y2,
	GLfloat x3, GLfloat y3,
	GLfloat x4, GLfloat y4) {
	glPushMatrix();
	glBegin(GL_QUADS);
	glVertex2i(x1, y1);
	glVertex2i(x2, y2);
	glVertex2i(x3, y3);
	glVertex2i(x4, y4);
	glEnd();
	glPopMatrix();
}

void Object::drawRect(GLfloat x, GLfloat y, GLfloat width, GLfloat height) {
	for (this->x = x; this->x < x + width; this->x++) {
		// glColor3f((this->x - x)*1.0f/width, 0.0f, 0.0f); // Left=black(0,0,0), right=red(1,0,0).
		for (this->y = y; this->y < y + height; this->y++) {
			// glColor3f(1.0 - ((this->y - y)*1.0f/height),1,0); // Bottom=yellow(1,1,0), top=green(0,1,0).
			this->drawPoint(this->x, this->y, 1.0);
		}
	}
}

void Object::drawRegularPolygon(GLfloat cx, GLfloat cy, GLfloat radius, GLint side, GLfloat orientation, GLfloat width, GLfloat height) {
	if (side >= 3) {
		glPushMatrix();
		GLint xp, yp; // Interpolation points
		glBegin(GL_POLYGON);
		for (int i = 0; i < side; i++) {
			xp = (int)(cx + width * radius * cos(orientation + 2 * 3.14159265 / side * i));
			yp = (int)(cy + width * radius * sin(orientation + 2 * 3.14159265 / side * i));
			glVertex2i(xp, yp);
		}
		glEnd();
		glPopMatrix();
	}
	else {
		cerr << "Could not render polygon with less than three sides." << endl;
	}
}

void Object::drawSineCurve(GLfloat int0, GLfloat int1) {
	for (this->x = int0; this->x < int1; this->x++) {
		this->y += 2 * sin(this->x * 3.14159265 / 180);
		this->drawPoint(this->x, this->y, 5.0);
	}
}

void Object::drawHollowCircle(GLfloat x, GLfloat y, GLfloat radius) {
	int i;
	int lineAmount = 100; //# of triangles used to draw circle
	float PI = 3.14159265;

	//GLfloat radius = 0.8f; //radius
	GLfloat twicePi = 2.0f * PI;

	glBegin(GL_LINE_LOOP);
	for (i = 0; i <= lineAmount; i++) {
		glVertex2f(
			x + (radius * cos(i * twicePi / lineAmount)),
			y + (radius * sin(i * twicePi / lineAmount))
		);
	}
	glEnd();
}

void Object::drawFilledCircle(GLfloat x, GLfloat y, GLfloat radius) {
	int i;
	int triangleAmount = 40; //# of triangles used to draw circle
	float PI = 3.14159265;

	//GLfloat radius = 0.8f; //radius
	GLfloat twicePi = 2.0f * PI;

	glBegin(GL_TRIANGLE_FAN);
	glVertex2f(x, y); // center of circle
	for (i = 0; i <= triangleAmount; i++) {
		glVertex2f(
			x + (radius * cos(i * twicePi / triangleAmount)),
			y + (radius * sin(i * twicePi / triangleAmount))
		);
	}
	glEnd();
}

void Object::drawArcCircle(GLfloat x, GLfloat y, GLfloat radius, GLfloat percentage) {
	int i;
	int lineAmount = 100; //# of triangles used to draw circle
	float PI = 3.14159265;


	//GLfloat radius = 0.8f; //radius
	GLfloat twicePi = percentage * PI;

	glBegin(GL_LINE_LOOP);
	for (i = 0; i <= lineAmount; i++) {
		glVertex2f(
			x + (radius * cos(i * twicePi / lineAmount)),
			y + (radius * sin(i * twicePi / lineAmount))
		);
	}
	glEnd();
}

void Object::drawArcFilledCircle(GLfloat x, GLfloat y, GLfloat radius, GLfloat percentage) {
	int i;
	int triangleAmount = 40; //# of triangles used to draw circle
	float PI = 3.14159265;

	//GLfloat radius = 0.8f; //radius
	GLfloat twicePi = percentage * PI;

	glBegin(GL_TRIANGLE_FAN);
	glVertex2f(x, y); // center of circle
	for (i = 0; i <= triangleAmount; i++) {
		glVertex2f(
			x + (radius * cos(i * twicePi / triangleAmount)),
			y + (radius * sin(i * twicePi / triangleAmount))
		);
	}
	glEnd();
}

void Object::translate(GLfloat tX, GLfloat tY) {
	this->x = this->x + tX;
	this->y = this->y + tY;
}

void Object::orbit(GLfloat cx, GLfloat cy, GLfloat radius, GLfloat& angle, GLfloat torque) {
	this->x = cx + radius * cos(angle * 3.14159265 / 180);
	this->y = cy + radius * sin(angle * 3.14159265 / 180);
	if (angle < 360) {
		angle += torque; // Rotational movement
	}
	else {
		angle = 0.0;
	}
}

void Object::rotate(GLfloat t, GLfloat pX, GLfloat pY) {
	glTranslated(pX, pY, 0);
	glRotatef(t, 0.0f, 0.0f, 1.0f);
	glTranslated(-pX, -pY, 0);
}

void Object::scale(GLfloat sX, GLfloat sY) {
	this->width = this->width * sX;
	this->height = this->height * sY;
}

void Object::scale(GLfloat scaleFactorX, GLfloat scaleFactorY, GLfloat scaleFactorZ, GLfloat pivotX, GLfloat pivotY, GLfloat pivotZ) {
	glTranslatef(pivotX, pivotY, pivotZ);
	glScalef(scaleFactorX, scaleFactorY, scaleFactorZ);
	glTranslatef(-pivotX, -pivotY, -pivotZ);
}


