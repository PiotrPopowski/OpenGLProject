#include "Camera.h"

using namespace std;

Camera::Camera()
{
	cout << "-- Creating camera\n";
	reset();
}


Camera::~Camera()
{
	cout << "++ Destructing camera\n";
}


/// resetuje kamere
void Camera::reset(void)
{
	this->distance = -40.0f;
	this->verticalTilt = -5.0f;
	this->horizontalAngle = 90.0f;
}


/// pozycjonowanie kamery
void Camera::position(void)
{
	glTranslatef(0.0f, 0.0f, this->distance);
	glRotatef(this->verticalTilt, 1.0f, 0.0f, 0.0f);
	glRotatef(this->horizontalAngle, 0.0f, 1.0f, 0.0f); 
}


/// przyblizanie kamery
void Camera::dec(void)
{
		this->distance--;
}


/// oddalanie kamery
void Camera::inc(void)
{
	this->distance++;
}


/// obrot zgodnie z ruchem wskazowek zegara
void Camera::clockwise(void)
{
	this->horizontalAngle++;
}


/// obrot przeciwnie do ruchu wskazowek zegara
void Camera::anticlockwise(void)
{
	this->horizontalAngle--;
}


/// obrot nachylenia (w gore)
void Camera::tiltup(void)
{
	if (this->verticalTilt < 0)
		this->verticalTilt++;
}


/// obrot nachylenia (w dol)
void Camera::tiltdown(void)
{
	if (this->verticalTilt > -90)
		this->verticalTilt--;
}
