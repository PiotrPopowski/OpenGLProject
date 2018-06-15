/*
* Crab class. Draws a pinky colour crab.
*
* Igor Kromin 40125374
*/

#include "Renderable.h"

using namespace std;

// setup the static variables
GLfloat Stone::material[4] = {0.5f, 0.5f, 0.5f, 1.f};
GLfloat Stone::shininess = 50.f;


/// Default Constructor. Builds the display list for the crab.
Stone::Stone() : Renderable()
{
	cout << "-- Creating crab\n";
	sy = sx = sz = 2.f; // make crab twice as big
	build(dlist);
}


/// Default destructor.
Stone::~Stone()
{
	cout << "++ Destructing crab\n";
}


/// Draws the crab
void Stone::_draw(void)
{
	/*
	* The materials are set in _draw_dlist() since that function
	* is called eacht time to draw the display list version of the
	* crab, this functin only gets used to build the display list
	*/

	// get our pinky colour
	glColor3f(0.502f, 0.502f, 0.502f);

	// draw crab body (squashed along Y axis
	glPushMatrix();
	glScalef(.0f, 0.5f, 1.0f);
	gluSphere(quadric, 0.3f, 16, 16);
	glPopMatrix();




}


/// Draws the display list for the crab object
void Stone::_draw_dlist(void)
{
	// set up the material properties (only front needs to be set)
	glMaterialfv(GL_FRONT, GL_SPECULAR, material);
	glMaterialf(GL_FRONT, GL_SHININESS, shininess);
	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
	glEnable(GL_COLOR_MATERIAL);

	glCallList(this->dlist);

	// turn of colour material tracking
	glDisable(GL_COLOR_MATERIAL);
}







	