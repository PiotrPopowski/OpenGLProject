#include "Renderable.h"

using namespace std;

GLfloat Quad::material[4] = {1.f, 1.f, 1.f, 1.f};
GLfloat Quad::shininess = 120.f;


Quad::Quad() : Renderable()
{
	cout << "-- Tworzenie podloza\n";
}


Quad::~Quad()
{
	cout << "++ Usuwanie podloza\n";
}


void Quad::_draw(void)
{
	// materialy
	glMaterialfv(GL_FRONT, GL_AMBIENT, material);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, material);
	glMaterialfv(GL_FRONT, GL_SPECULAR, material);
	glMaterialf(GL_FRONT, GL_SHININESS, shininess);

	// teksturowanie
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, FLOOR_TEXTURE);

	// wlasciwosci tekstury
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	glColor3f(0.9f, 0.6f, 0.0f);
	glFrontFace(GL_CCW);
	glBegin(GL_QUADS);
		glNormal3f(0.f, 0.f, 1.f);
		glTexCoord2f(0.0f, 0.0f);	glVertex2f(-0.5f, -0.5f);
		glTexCoord2f(1.0f, 0.0f);	glVertex2f(0.5f,  -0.5f);
		glTexCoord2f(1.0f, 1.0f);	glVertex2f(0.5f, 0.5f);
		glTexCoord2f(0.0f, 1.0f);	glVertex2f(-0.5f, 0.5f);
	glEnd();

	glDisable(GL_TEXTURE_2D);
}
