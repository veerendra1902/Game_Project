

#include<Math.h>
#include<iostream>
#include<stdio.h>
using namespace std;
#include "vector.h"
#include "Object.h"

const float pi=3.14;

vector::vector(){

}

	 vector::vector(float x1,float y1,float z1){


		x=x1;
		y=y1;
		z=z1;



	}

	float vector::DotProduct(vector v1, vector v2){
		float dot=(v1.x*v2.x)+(v1.y*v2.y)+(v1.z*v2.z);
		return dot;
	}

	vector vector::CrossProduct(vector v1, vector v2){



		return vector(v1.y*v2.z-v2.y*v1.z,v2.x*v1.z-v1.x*v2.z,v1.x*v2.y-v2.x*v1.y);
	}

	float vector::MagnitudeSqr(vector v){

		float result=((v.x)*(v.x)+(v.y)*(v.y)+(v.z)*(v.z));
		return result;
	}

	float vector::Magnitude(vector v){

			float result=sqrt((v.x)*(v.x)+(v.y)*(v.y)+(v.z)*(v.z));
			return result;
		}

	vector vector::Add(vector v1,vector v2){



		return vector(v1.x+v2.x,v1.y+v2.y,v1.z+v2.z);
	}

	vector vector::Subtract(vector v1,vector v2){



			return vector(v1.x-v2.x,v1.y-v2.y,v1.z-v2.z);


		}

	vector vector::Normalise(vector v){

		float r=sqrt((v.x)*(v.x)+(v.y)*(v.y)+(v.z)*(v.z));

vector n(v.x/r,v.y/r,v.z/r);
		return n;


	}

	void vector::PrintVector(vector v){

		cout<<"x coord:"<< v.x<<endl; ;
		cout<<"y coord:"<< v.y<<endl;;
		cout<< "z coord:"<<v.z<<endl;
	}

	float vector::Angle(vector v1,vector v2){
		float a=v1.Magnitude(v1);
		float b=v2.Magnitude(v2);
		float d=v1.DotProduct(v1,v2);
		return  d/(a*b);

	}


	float vector::Angx(vector v){
		float r=sqrt((v.x)*(v.x)+(v.y)*(v.y)+(v.z)*(v.z));
		float a=v.x/r;
		return a;
	}


	float vector::Angy(vector v){
			float r=sqrt((v.x)*(v.x)+(v.y)*(v.y)+(v.z)*(v.z));
			float a=v.y/r;
			return a;
		}

	float vector::Angz(vector v){
			float r=sqrt((v.x)*(v.x)+(v.y)*(v.y)+(v.z)*(v.z));
			float a=v.z/r;
			return a;
		}


	vector vector::ConstProduct(vector v,float t){

	//	vector n(v.x,v.y,v.z);

		return vector(v.x*t,v.y*t,v.z*t);
		//return n;
	}

	vector vector::RotateVectorLeft(vector v,vector n,float angle){
		vector i=v.Normalis
		float a= (pi/180)*angle;
		return v.Subtract(v.ConstProduct(i,cos(a)),v.CrossProduct(i,v.ConstProduct(n,sin(a))));

	}

	vector vector::RotateVectorRight(vector v,vector n,float angle){
			vector i=v.Normalise(v);
		//	v.PrintVector(i);
			float a= (pi/180)*angle;
			return v.Subtract(v.ConstProduct(i,cos(a)),v.CrossProduct(i,v.ConstProduct(n,-sin(a))));

		}




