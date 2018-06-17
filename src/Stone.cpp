#include "Renderable.h"

using namespace std;

GLfloat Stone::material[4] = { 0.5f, 0.5f, 0.5f, 1.f };
GLfloat Stone::shininess = 80.f;


Stone::Stone() : Renderable()
{
	cout << "-- Generowanie kamienia\n";
	sx = sz = 2.f; 
	sy = 0.5f;
	build(dlist);
}


Stone::~Stone()
{
	cout << "++ Usuwanie kamienia\n";
}


void Stone::_draw(void)
{


	// kolor
	glColor3f(0.502f, 0.502f, 0.520f);

	glPushMatrix();
	glScalef(1.4f, 2.0f, 2.0f);
	gluSphere(quadric, Renderable::getRand(0.1, 0.2), 10, 10);
	glPopMatrix();




}


void Stone::_draw_dlist(void)
{
	glMaterialfv(GL_FRONT, GL_SPECULAR, material);
	glMaterialf(GL_FRONT, GL_SHININESS, shininess);
	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
	glEnable(GL_COLOR_MATERIAL);

	glCallList(this->dlist);

	glDisable(GL_COLOR_MATERIAL);
}







