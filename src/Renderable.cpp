#include "Renderable.h"

// statyczne czesci obiektow
unsigned int Renderable::textures[2];
GLUquadricObj *Renderable::quadric = gluNewQuadric();


/// domyslnie: pozycja = 0, obrot w.o. Y = losowy, dlist = wylaczone

Renderable::Renderable()
{
	this->x = 0.0f;
	this->y = 0.0f;
	this->z = 0.0f;
	
	this->rx = 0.0f;
	this->ry = getRand(0.0f, 360.0f);
	this->rz = 0.0f;

	this->sx = 1.0f;
	this->sy = 1.0f;
	this->sz = 1.0f;

	isList = false;
}


Renderable::~Renderable()
{
}


/// budowanie dlist
void Renderable::build(GLuint &dlist)
{
	dlist = glGenLists(1);
	if (!glIsList(dlist)) {
		isList = false;
		return;
	}
	isList = true;

	glPushMatrix();
	glNewList(dlist, GL_COMPILE);
		_draw();
	glEndList();
	glPopMatrix();
}


/// przesuwanie obiektu
void Renderable::move(GLfloat x, GLfloat y, GLfloat z)
{
	this->x = x;
	this->y = y;
	this->z = z;
}


/// obracanie obiektu
void Renderable::rotate(GLfloat x, GLfloat y, GLfloat z)
{
	this->rx = x;
	this->ry = y;
	this->rz = z;
}


/// skalowanie obiektu
void Renderable::scale(GLfloat x, GLfloat y, GLfloat z)
{
	this->sx = x;
	this->sy = y;
	this->sz = z;
}


/// przygotowywanie obiektu do rysowania (tworzenie dlist)
void Renderable::draw(void)
{
	glPushMatrix();

	glTranslatef(this->x, this->y, this->z);

	glRotatef(this->rx, 1.0f, 0.0f, 0.0f);
	glRotatef(this->ry, 0.0f, 1.0f, 0.0f);
	glRotatef(this->rz, 0.0f, 0.0f, 1.0f);

	glScalef(sx, sy, sz);

	if (this->isList)
		_draw_dlist();
	else
		_draw();

	glPopMatrix();
}


/// generowanie liczb
GLfloat Renderable::getRand(GLfloat minimum, GLfloat range)
{
	return (((GLfloat)rand() / (GLfloat)RAND_MAX) * range) + minimum;
}
