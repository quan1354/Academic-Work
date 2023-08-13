#include "Object.h"

const double Object::PI = acos(-1);

Object::Object()
{
	this->setColor(0, 0, 0);
}

Object::Object(int r, int g, int b) {

	this->setColor(r, g, b);
}

void Object::setColor(int r, int g, int b) {
	this->r = r;
	this->g = g;
	this->b = b;

	GLfloat red = (r % 256) / 255.0f;
	GLfloat green = (g % 256) / 255.0f;
	GLfloat blue = (b % 256) / 255.0f;
	glColor3f(red, green, blue);
}

void Object::drawTriangle(Point p1, Point p2, Point p3) {
	glPushMatrix();

	glBegin(GL_TRIANGLES);
	glVertex2f(p1.getConvertX(), p1.getConvertY());
	glVertex2f(p2.getConvertX(), p2.getConvertY());
	glVertex2f(p3.getConvertX(), p3.getConvertY());
	glEnd();

	glPopMatrix();
}

void Object::drawRectangle(Point p1, Point p2, Point p3, Point p4) {
	glPushMatrix();

	glBegin(GL_QUADS);
	glVertex2f(p1.getConvertX(), p1.getConvertY());
	glVertex2f(p2.getConvertX(), p2.getConvertY());
	glVertex2f(p3.getConvertX(), p3.getConvertY());
	glVertex2f(p4.getConvertX(), p4.getConvertY());
	glEnd();

	glPopMatrix();
}

bool Object::drawPolygon(Point center, GLfloat radius, GLint sides, GLfloat rotation, GLfloat width, GLfloat height) {
	bool status = true;
	GLint xp, yp;
	if (sides > 2) {
		glPushMatrix();
		glBegin(GL_POLYGON);
		for (int i = 0; i < sides; ++i) {
			xp = center.getConvertX() + (width * radius * cos(rotation + 2 * PI / sides * i));
			yp = center.getConvertY() + (height * radius * sin(rotation + 2 * PI / sides * i));
			glVertex2f(xp, yp);
		}
		glEnd();
		glPopMatrix();
	}
	else
		status = false;

	return status;
}

void Object::drawPoint(GLfloat x, GLfloat y, GLfloat size) {
	glPushMatrix();
	glPointSize(size);
	glBegin(GL_POINTS);
	glVertex2i(x, y);
	glEnd();
	glPopMatrix();
}

void Object::drawLine(Point p1, Point p2, GLfloat thickness) {
	glPushMatrix();
	glLineWidth(thickness);
	glBegin(GL_LINES);
	glVertex2f(p1.getConvertX(), p1.getConvertY());
	glVertex2f(p2.getConvertX(), p2.getConvertY());
	glEnd();
	glPopMatrix();
}

void Object::rotate(Point center, GLfloat degree) {
	glTranslatef(center.getConvertX(), center.getConvertY(), 0.0f);
	glRotatef(degree, 0.0f, 0.0f, 1.0f);
	glTranslatef(-center.getConvertX(), -center.getConvertY(), 0.0f);
}

void Object::finishRotation() {
	glLoadIdentity();
}

void Object::drawText(const char* text, GLint length, Point p1, void* font) {
	glMatrixMode(GL_PROJECTION);
	double* matrix = new double[16];
	glGetDoublev(GL_PROJECTION_MATRIX, matrix);
	glLoadIdentity();
	glOrtho(0, 913, 0, 600, -5, 5);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glPushMatrix();
	glLoadIdentity();
	glRasterPos2i(p1.getConvertX(), p1.getConvertY());
	for (int i = 0; i < length; i++) {
		glutBitmapCharacter(font, (int)text[i]);
	}
	glPopMatrix();

	glMatrixMode(GL_PROJECTION);
	glLoadMatrixd(matrix);
	glMatrixMode(GL_MODELVIEW);
}