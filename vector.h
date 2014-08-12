/*
 * vector.h
 *
 *  Created on: Aug 8, 2013
 *      Author: Veerendra
 */

#ifndef VECTOR_H_
#define VECTOR_H_



class vector{
public :
	float x,y,z;
	vector();
	vector(float x,float y,float z);
   void PrintVector(vector v);
	vector Add(vector v1,vector v2);
	vector Subtract(vector v1,vector v2);
	float DotProduct(vector v1,vector v2);
	float Angle(vector v1,vector v2);
	vector CrossProduct(vector v1,vector v2);
	vector ConstProduct(vector v1,float t);
	float MagnitudeSqr(vector v);
	float Magnitude(vector v);
	vector Normalise(vector v);
	float Angx(vector v);//gives the cos(angle) with x axis
	float Angy(vector v);
	float Angz(vector v);
	vector RotateVectorLeft(vector v,vector n,float angle);
	vector RotateVectorRight(vector v,vector n,float angle);///  it will return a unit vector

};

//class bike{
//public:
//	float f,s;
//	bike();
//	bike(float f,float s);
//	vector front(vector p,vector v,bike b);
//	vector back(vector p,vector v,bike b);
//	vector left(vector p,vector v,bike b);
//	vector right(vector p,vector v,bike b);
//
//};



#endif /* VECTOR_H_ */

