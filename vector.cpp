

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
		vector i=v.Normalise(v);
	//	v.PrintVector(i);
		float a= (pi/180)*angle;
	//	cout<<cos(a)<<endl;
	//	v.PrintVector(v.ConstProduct(i,cos(a)));
		return v.Subtract(v.ConstProduct(i,cos(a)),v.CrossProduct(i,v.ConstProduct(n,sin(a))));

	}

	vector vector::RotateVectorRight(vector v,vector n,float angle){
			vector i=v.Normalise(v);
		//	v.PrintVector(i);
			float a= (pi/180)*angle;
		//	cout<<cos(a)<<endl;
//			v.PrintVector(v.ConstProduct(i,cos(a)));
//			v.PrintVector(i);
//			v.PrintVector(v.CrossProduct(i,v.ConstProduct(n,-1)));
			return v.Subtract(v.ConstProduct(i,cos(a)),v.CrossProduct(i,v.ConstProduct(n,-sin(a))));

		}

//	bike::bike(){
//
//	}

//	bike::bike(float f1,float s1){
//		f=f1;
//		s=s1;
//	}
//
//vector	bike::front(vector p,vector v,bike b){
//	float r=sqrt((v.x)*(v.x)+(v.y)*(v.y)+(v.z)*(v.z));
//	vector n(b.f*v.x/r,b.f*v.y/r,b.f*v.z/r);
//	return p.Add(p,n);
//
//}
//
//vector	bike::back(vector p,vector v,bike b){
//	float r=sqrt((v.x)*(v.x)+(v.y)*(v.y)+(v.z)*(v.z));
//	vector n(b.f*v.x/r,b.f*v.y/r,b.f*v.z/r);
//	return p.Subtract(p,n);
//
//}
//







//int main(){
//
//vector v1(1.0,0.0,0.0);
//vector v2(0.00,0.0,1.0);
//bike b(1.5,1.0);
////v1.x=1.0;
////v1.y=1.0;
////v1.z=9.0;
//
//v1.PrintVector(v1);
//v2.PrintVector(v2);
////cout<<v1.Magnitude(v1)<<endl;
////v1.PrintVector(v1.RotateVectorLeft(v1,v2,45.0f));
//v1.PrintVector(b.back(v1,v2,b));
//}


