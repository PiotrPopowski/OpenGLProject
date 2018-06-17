#include "Renderable.h"

using namespace std;

GLfloat Plant::material1[4] = {0.1f, 0.3f, 0.15f, 1.f};
GLfloat Plant::material2[4] = {0.6f, 1.f, 0.8f, 1.f};
GLfloat Plant::shininess = 100.f;


Plant::Plant() : Renderable()
{
	cout << "-- Generowanie rosliny\n";
	build(dlist);
}


Plant::~Plant()
{
	cout << "++ Usuwanie rosliny\n";
}


void Plant::_draw(void)
{
	generate(0, Renderable::getRand(1, 6));
}


// generowanie rosliny. Roslina jest generowana z przynajmniej jedna galezia. Polecena rysowania znajduja sie w dlist
void Plant::generate(int level, int number)
{
	// max 5
	if (level == 5) return;

	// kazda kolejna galaz to 3/4 poprzedniej
	GLfloat height = 3.0f / (0.75 * (level + 1.f));	
	GLfloat bottom = 0.75f / pow(2, level);
	GLfloat top = 0.75f / pow(2, (GLfloat)level + 1.f);
	
	for (int i = 0; i < number; i++)
	{
		
		GLfloat horzAngle = Renderable::getRand(0, 180);
		GLfloat vertAngle = Renderable::getRand(0, 180);
		int numChildren = Renderable::getRand(0, 6);

		glPushMatrix();
		glRotatef(horzAngle, 0.0f, 1.0f, 0.0f);
		glRotatef(vertAngle, 1.0f, 0.0f, 0.0f);

		// srodek
		glFrontFace(GL_CW);
		gluDisk(quadric, 0.0f, bottom, 8 - level, 1);
		glFrontFace(GL_CCW);

		// rysowanie galezi
		gluCylinder(quadric, bottom, top, height, 8 - level, 1);
		
		// wierzcholek
		glTranslatef(0.0f, 0.0f, height);
		gluDisk(quadric, 0.0f, top, 8 - level, 1);

		// mozna wywolac wiecej razy, aby wygenerowac wiecej galezi, ale najpierw nalezy przywrocic pozycje 'normal'
		//w celu unikniecia galezi opadajacych w dol
		glRotatef(-vertAngle, 1.0f, 0.0f, 0.0f);
		generate(level + 1, numChildren);
		glPopMatrix();
	}
}


/// tworzy dlist (liste polecen rysowania)
void Plant::_draw_dlist(void)
{
	// konfiguruje wlasciwosc materialu (nalezy ustawic tylko front)
	glMaterialfv(GL_FRONT, GL_AMBIENT, material1);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, material1);
	glMaterialfv(GL_FRONT, GL_SPECULAR, material2);
	glMaterialf(GL_FRONT, GL_SHININESS, shininess);
	glColor4fv(material1);

	glCallList(this->dlist);
}