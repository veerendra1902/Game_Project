/*
 * Object.cpp
 *
 *  Created on: Aug 14, 2013
 *      Author: Veerendra
 */

#include <iostream>
#include <stdlib.h>

#ifdef __APPLE__
#include <OpenGL/OpenGL.h>
#include <GLUT/glut.h>
#else
#include <GL/freeglut.h>
#endif
#include <Math.h>
#include "vector.h"
#include "Object.h"



using namespace std;

 Object::Object(){

}

  Object::Object(vector f, vector c, vector b,float l){
mass=1.0;

	center=c;
yvelocity=0.050;
	velocity=vector(0.0,0.0,0.0);
	vector n(0.0,1.0,0.0);
	vector p=(f.Subtract(f,c));

	fp =l;
	head= f.Normalise(p);
	front=f.Add(head.ConstProduct(head,l),center);
	back=f.Add(head.ConstProduct(head,-l),center);
	sideNormal=f.Normalise(f.CrossProduct(head,n));

}









