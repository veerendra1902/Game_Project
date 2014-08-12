/*
 * Node.cpp
 *
 *  Created on: Sep 3, 2013
 *      Author: Veerendra
 */

#include<Math.h>
#include<iostream>
#include<stdio.h>
#include "Node.h"
using namespace std;



	node::node(){

	}
	float x,y,z,r;

	node::node(float x1,float y1,float z1,float r1){
		x=x1;
		y=y1;
		z=z1;
		r=r1;
	}

	void node::Print(){
		cout<<"x="<<x<<endl;
		cout<<"y="<<y<<endl;
		cout<<"z="<<z<<endl;
		cout<<"r="<<r<<endl;
	}



