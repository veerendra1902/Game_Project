/*
 * Object.h
 *
 *  Created on: Aug 14, 2013
 *      Author: Veerendra
 */

#ifndef OBJECT_H_
#define OBJECT_H_


#include <iostream>
#include <stdlib.h>

#ifdef __APPLE__
#include <OpenGL/OpenGL.h>
#include <GLUT/glut.h>
#else
#include <GL/freeglut.h>
#endif
#include <Math.h>




using namespace std;


class Object{
public:
	vector sideNormal;
	vector head;
	vector front;
	vector center;
	vector back;
	vector velocity;
	float fp;
	float side;
	float yvelocity;
	float mass;
	 Object();
	 Object(vector f,vector c,vector b,float l);


};



#endif /* OBJECT_H_ */

