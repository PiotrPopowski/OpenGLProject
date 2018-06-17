

#ifndef __CAMERA_3201
#define __CAMERA_3201

#include <freeglut.h>
#include <iostream>
#include <math.h>

#define PI 3.14159

class Camera
{
private:
	GLfloat verticalTilt;
	GLfloat horizontalAngle;
	GLfloat distance;

public:
	Camera();	
	virtual ~Camera();	

	void reset(void);	
	void position(void);	
	void dec(void);	
	void inc(void);	
	void clockwise(void);	
	void anticlockwise(void);	
	void tiltup(void);	
	void tiltdown(void);	
};

#endif
