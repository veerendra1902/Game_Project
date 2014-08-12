

#include "glm1.h"
//#include "phymain.h"
//#include "Node.h"
#include "text3d.h"


#include <iostream>
#include <fstream>
#include <stdlib.h>

#include <cstdlib>
#include <ctime>
#include <iostream>
#include <math.h>
#include <set>
#include <sstream>
#include <string>
#include <stdlib.h>
#ifdef __APPLE__
#include <OpenGL/OpenGL.h>
#include <GLUT/glut.h>
#else
#include <GL/freeglut.h>
#endif
#include <windows.h>
#include "imageloader.h"
#include "vector.h"
#include "Object.h"
#include "Node.h"
#include <Math.h>


bool pressed;
bool pressedl;
bool pressedr;
bool collide=false;
bool boost;
bool handbreak;
bool gameOver;
bool printScore=true;
bool Starcollide=false;
bool createArray=false;


const float pi=3.14;
double rotate_y=0;
double rotate_xl=0;
double rotate_xr=0;
double rotate_z=0;

float an=0.0;
float bs=0.0;
float speed;
float score;
float star;
float hb;


float _angle = 60.0f;


vector fv(2.5,0.0,0.0);
vector cv(0.0,0.0,0.0);
vector bv(-2.5,0.0,0.0);
Object bike(fv,cv,bv,2.5);
vector normalup(0.0,1.0,0.0);

GLuint DLid_tree;
GLuint DLid_House;
GLuint DLid_Rock;
GLuint DLid_Treetrunk;
GLuint DLid_boat;
GLuint DLid_star;

GLuint _textureId;
GLuint _textureId_bike;//The id of the texture
GLuint _textureId_house;
GLuint _textureId_treetrunk;
GLuint _textureId_boat;
GLuint _textureId_water;


GLMmodel* myModel;//////////////////////////////////////////classssssssshiiiiiiiiiing

GLMmodel* star1;
GLMmodel* myModel_tree;//////////////////////////////////////////classssssssshiiiiiiiiiing

GLMmodel* myHouse;
GLMmodel* myRock;
GLMmodel* treetrunk;
GLMmodel* boat;


vector v;
bool arr[14] = {false};

class Terrain {
    public:
        int w; //Width
        int l; //Length
        float** hs; //Heights
        vector** normals;
        bool computedNormals; //Whether normals is up-to-date
    public:
        Terrain(int w2, int l2);
        ~Terrain();
        int width();
        int length();

        //Sets the height at (x, z) to y
        void setHeight(int x, int z, float y);

        //Returns the height at (x, z)
        float getHeight(int x, int z);

        //Computes the normals, if they haven't been computed yet
        void computeNormals() ;

                    //Returns the normal at (x, z)
        vector getNormal(int x, int z);



};




Terrain::Terrain(int w2, int l2) {
            w = w2;
            l = l2;

            hs = new float*[l];
            for(int i = 0; i < l; i++) {
                hs[i] = new float[w];
            }

            normals = new vector*[l];
            for(int i = 0; i < l; i++) {
                normals[i] = new vector[w];
            }

            computedNormals = false;
        }

Terrain::~Terrain() {
            for(int i = 0; i < l; i++) {
                delete[] hs[i];
            }
            delete[] hs;

            for(int i = 0; i < l; i++) {
                delete[] normals[i];
            }
            delete[] normals;
        }

        int Terrain::width() {
            return w;
        }

        int Terrain::length() {
            return l;
        }

        //Sets the height at (x, z) to y
        void Terrain::setHeight(int x, int z, float y) {
            hs[z][x] = y;
            computedNormals = false;
        }

        //Returns the height at (x, z)
        float Terrain::getHeight(int x, int z) {
            return hs[z][x];
        }

        //Computes the normals, if they haven't been computed yet
        void Terrain::computeNormals() {
            if (computedNormals) {
                return;
            }

            //Compute the rough version of the normals
            vector** normals2 = new vector*[l];
            for(int i = 0; i < l; i++) {
                normals2[i] = new vector[w];
            }

            for(int z = 0; z < l; z++) {
                for(int x = 0; x < w; x++) {
                    vector sum(0.0f, 0.0f, 0.0f);

                    vector out;
                    if (z > 0) {
                        out = vector(0.0f, hs[z - 1][x] - hs[z][x], -1.0f);
                    }
                    vector in;
                    if (z < l - 1) {
                        in = vector(0.0f, hs[z + 1][x] - hs[z][x], 1.0f);
                    }
                    vector left;
                    if (x > 0) {
                        left = vector(-1.0f, hs[z][x - 1] - hs[z][x], 0.0f);
                    }
                    vector right;
                    if (x < w - 1) {
                        right = vector(1.0f, hs[z][x + 1] - hs[z][x], 0.0f);
                    }

                    if (x > 0 && z > 0) {
                        //sum += out.cross(left).normalize();
                         sum=sum.Add(sum,sum.Normalise(sum.CrossProduct(out,left)));
                    }
                    if (x > 0 && z < l - 1) {
                //		sum += left.cross(in).normalize();
                        sum=sum.Add(sum,sum.Normalise(sum.CrossProduct(left,in)));
                    }
                    if (x < w - 1 && z < l - 1) {
                    //	sum += in.cross(right).normalize();
                        sum=sum.Add(sum,sum.Normalise(sum.CrossProduct(in,right)));
                    }
                    if (x < w - 1 && z > 0) {
                    //	sum += right.cross(out).normalize();
                        sum=sum.Add(sum,sum.Normalise(sum.CrossProduct(right,out)));
                    }

                    normals2[z][x] = sum;
                }
            }

            //Smooth out the normals
            const float FALLOUT_RATIO = 0.5f;
            for(int z = 0; z < l; z++) {
                for(int x = 0; x < w; x++) {
                    vector sum = normals2[z][x];

                    if (x > 0) {
                    //	sum += normals2[z][x - 1] * FALLOUT_RATIO;
                        sum=sum.Add(sum,sum.ConstProduct(normals2[z][x - 1],FALLOUT_RATIO));
                    }
                    if (x < w - 1) {
                    //	sum += normals2[z][x + 1] * FALLOUT_RATIO;
                        sum=sum.Add(sum,sum.ConstProduct(normals2[z][x + 1],FALLOUT_RATIO));
                    }
                    if (z > 0) {
                        //sum += normals2[z - 1][x] * FALLOUT_RATIO;
                        sum=sum.Add(sum,sum.ConstProduct( normals2[z - 1][x],FALLOUT_RATIO));
                    }
                    if (z < l - 1) {
                    //	sum += normals2[z + 1][x] * FALLOUT_RATIO;
                        sum=sum.Add(sum,sum.ConstProduct(normals2[z + 1][x],FALLOUT_RATIO));
                    }

                    if (sum.Magnitude(sum) == 0) {
                        sum = vector(0.0f, 1.0f, 0.0f);
                    }
                    normals[z][x] = sum;
                }
            }

            for(int i = 0; i < l; i++) {
                delete[] normals2[i];
            }
            delete[] normals2;

            computedNormals = true;
        }

        //Returns the normal at (x, z)
        vector Terrain::getNormal(int x, int z) {
            if (!computedNormals) {
                computeNormals();
            }
            return normals[z][x];
        }






Terrain* loadTerrain(const char* filename, float height) {
    Image* image = loadBMP(filename);
    Terrain* t = new Terrain(image->width, image->height);
    for(int y = 0; y < image->height; y++) {
        for(int x = 0; x < image->width; x++) {

            unsigned char color =
                (unsigned char)image->pixels[3 * (y * image->width + x)];
            float h = height * ((color / 255.0f) - 0.5f);
            t->setHeight(x, y, h);


        }
    }

    delete image;
    t->computeNormals();
    return t;
}


GLuint loadTexture(Image* image) {
    GLuint textureId;
    glGenTextures(1, &textureId); //Make room for our texture
    glBindTexture(GL_TEXTURE_2D, textureId); //Tell OpenGL which texture to edit
    //Map the image to the texture
    glTexImage2D(GL_TEXTURE_2D,                //Always GL_TEXTURE_2D
                 0,                            //0 for now
                 GL_RGB,                       //Format OpenGL uses for image
                 image->width, image->height,  //Width and height
                 0,                            //The border of the image
                 GL_RGB, //GL_RGB, because pixels are stored in RGB format
                 GL_UNSIGNED_BYTE, //GL_UNSIGNED_BYTE, because pixels are stored
                                   //as unsigned numbers
                 image->pixels);               //The actual pixel data
    return textureId; //Returns the id of the texture
}


const float g= 0.005f;
const float accf=-0.01f;
const float acc=0.020f;
Terrain* _terrain;


float heightAt(Terrain* terrain, float x, float z) {
    //Make (x, z) lie within the bounds of the terrain
    if (x < 0) {
        x = 0;
    }
    else if (x > terrain->width() - 1) {
        x = terrain->width() - 1;
    }
    if (z < 0) {
        z = 0;
    }
    else if (z > terrain->length() - 1) {
        z = terrain->length() - 1;
    }

    //Compute the grid cell in which (x, z) lies and how close we are to the
    //left and outward edges
    int leftX = (int)x;
    if (leftX == terrain->width() - 1) {
        leftX--;
    }
    float fracX = x - leftX;

    int outZ = (int)z;
    if (outZ == terrain->width() - 1) {
        outZ--;
    }
    float fracZ = z - outZ;

    //Compute the four heights for the grid cell
    float h11 = terrain->getHeight(leftX, outZ);
    float h12 = terrain->getHeight(leftX, outZ + 1);
    float h21 = terrain->getHeight(leftX + 1, outZ);
    float h22 = terrain->getHeight(leftX + 1, outZ + 1);

    //Take a weighted average of the four heights
    return (1 - fracX) * ((1 - fracZ) * h11 + fracZ * h12) +
        fracX * ((1 - fracZ) * h21 + fracZ * h22);
}






void cleanup() {
    delete _terrain;
}


node* create(int m){

    node *ca=new node[23];
    node a(5.0,10,5.0,0.6);
    node b(175,10,150,0.6);
    node c(40,10,220,0.6);
    node d(220,10,50,0.6);
    node e(180,10,180,0.6);
    node f(60,10,70,0.6);
    node g(215,10,85,0.6);
    node h(194,10,20,0.6);
    node j(215,10,158,0.6);
    node k(90,10,215,0.6);
    node l(205,10,20,0.6);
    node m1(200,10,10,6);
    node n(10,10,170,6);
    node o(30,-11,30,3);
    node p(121,-11,20,3);
    node q(225,-11,211,3);
    node r(105,-11,201,3);
    node s(110,-11,110,3);
    node t(50,0,50,2);
    node u(140,0,135,2);
    node v(85,0,31,2);
    node w(112,0,171,2);
    node x(56,0,212,2);									

    ca[0]=a;
    ca[1]=b;
    ca[2]=c;
    ca[3]=d;
    ca[4]=e;
    ca[5]=f;
    ca[6]=g;
    ca[7]=h;
    ca[8]=j;
    ca[9]=k;
    ca[10]=l;
    ca[11]=m1;
    ca[12]=n;
    ca[13]=o;
    ca[14]=p;
    ca[15]=q;
    ca[16]=r;
    ca[17]=s;
    ca[18]=t;
    ca[19]=u;
    ca[20]=v;
    ca[21]=w;
    ca[22]=x;



return ca;


}



node* createStar(int m){

    node *ca=new node[14];
    node a(190.0f,_terrain->getHeight(190,40)+5, 40.0f,2.0);
    node b(130.0f, _terrain->getHeight(130, 78) + 5, 78.0f,0.6);
    node c(94.0f, _terrain->getHeight(94, 17) + 5, 17.0f
             ,0.6);
    node d(190.0f, _terrain->getHeight(190, 100) + 5, 100.0f
             ,0.6);
    node e(220.0f, _terrain->getHeight(220, 240) + 5, 240.0f
             ,0.6);
            node f(215.0f, _terrain->getHeight(215, 80) + 5, 80.0f
                     ,0.6);
            node g(150.0f, _terrain->getHeight(150, 180) + 5, 180.0f
                     ,0.6);
            node h(60.0f, _terrain->getHeight(60, 180) + 5, 200.0f
                     ,0.6);
            node j(45.0f, _terrain->getHeight(45, 204) + 5, 204.0f
                     ,0.6);
            node k(170.0f, _terrain->getHeight(170, 30) + 5, 30.0f
                     ,0.6);
            node l(40.0f, _terrain->getHeight(40, 40) + 5, 40.0f
                     ,0.6);
            node m1(110.0f, _terrain->getHeight(110, 190) + 5, 190.0f
                     ,6);
            node n(90.0f, _terrain->getHeight(90, 150) + 5, 150.0f
,6);
                        node o(210.0f, _terrain->getHeight(210, 220) + 5, 220.0f
                                 ,3);

    ca[0]=a;
    ca[1]=b;
    ca[2]=c;
    ca[3]=d;
    ca[4]=e;
    ca[5]=f;
    ca[6]=g;
    ca[7]=h;
    ca[8]=j;
    ca[9]=k;
    ca[10]=l;
    ca[11]=m1;
    ca[12]=n;
    ca[13]=o;

return ca;


}


void collision(node* a,Object o,int m){

    int i=0;
    while(i<m){
        float cx=a[i].x;
        float cz=a[i].z;
        float cy=a[i].y;
        float r=a[i].r;
        float bx=o.center.x;
        float bz=o.center.z;
        if((cx-bx)*(cx-bx)+(cz-bz)*(cz-bz)<=r*r && o.center.y<cy){
            collide =true;
            score-=5;
            break;
        }

        i++;
    }



}

void Starcollision(node* a,Object o,int m){

    int i=0;
    while(i<m){
        if(arr[i]==false){


        float cx=a[i].x;
        float cz=a[i].z;
        float cy=a[i].y;
        float r=a[i].r;
        float bx=o.center.x;
        float bz=o.center.z;
        if((cx-bx)*(cx-bx)+(cz-bz)*(cz-bz)<=r*r && o.center.y<cy){
            arr[i] =true;
            star+=1;
cout<<"i is :"<<i<<endl;
            break;
        }
        }
        else{

        }

        i++;
    }



}






float angleAt(vector f,vector b,vector v){
    float h=heightAt(_terrain,f.x,f.z)-heightAt(_terrain,b.x,b.z);

    float l=bike.fp;
    float tan=h/l;
    float a=atan(tan)*180/pi;
    return a;
}


void display_list(GLMmodel* myModel, GLuint _textureId, float angle, int x, int y,
        int z) {
    glEnable(GL_TEXTURE_2D);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    glBindTexture(GL_TEXTURE_2D, _textureId);
    //Bottom
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glmFacetNormals(myModel);
    glmVertexNormals(myModel, 90);
    glRotatef(angle, x, y, z);

    glScalef(2, 2, 2);
    glmDraw(myModel, GLM_SMOOTH | GLM_TEXTURE);
    glDisable(GL_TEXTURE_2D);

}

void star_cre(GLMmodel* myModel,float a,int x,int y,int z) {
    glColor3f(0.37, 0.37, 0.37);
    glmFacetNormals(myModel);
    glmVertexNormals(myModel, 90);
    glRotatef(a, x, y, z);
    glScalef(0.6, 0.6, 0.6);
    glmDraw(myModel, GLM_SMOOTH | GLM_TEXTURE);
}



void rock(GLMmodel* myModel) {
    glColor3f(0.37, 0.37, 0.37);
    glmFacetNormals(myModel);
    glmVertexNormals(myModel, 90);
    glScalef(0.2, 0.2, 0.2);
    glmDraw(myModel, GLM_SMOOTH | GLM_TEXTURE);
}


GLuint createDL_tree() {
    GLuint bike1;
    bike1 = glGenLists(1);
    glNewList(bike1, GL_COMPILE);
    display_list(myModel_tree, _textureId, 0, 0, 1, 0);
    glEndList();
    return (bike1);
}

GLuint createDL_rock() {
    GLuint bike1;
    bike1 = glGenLists(1);
    glNewList(bike1, GL_COMPILE);
    rock(myRock);
    glEndList();
    return (bike1);
}
GLuint createDL_House() {
    GLuint house;

    house = glGenLists(1);

    // start list
    glNewList(house, GL_COMPILE);

    // call the function that contains the rendering commands
    display_list(myHouse, _textureId_house, 0, 1, 0, 1);

    // endList
    glEndList();

    return (house);
}
GLuint createDL_treetrunk() {
    GLuint bike1;
    // Create the id for the list
    bike1 = glGenLists(1);
    // start list
    glNewList(bike1, GL_COMPILE);
    // call the function that contains the rendering commands
    display_list(treetrunk, _textureId_treetrunk, -60, 1, 0, 0);
    // endList
    glEndList();
    return (bike1);
}


GLuint createDL_star() {
    GLuint bike1;
    // Create the id for the list
    bike1 = glGenLists(1);
    // start list
    glNewList(bike1, GL_COMPILE);
    // call the function that contains the rendering commands
    star_cre(star1, 90, 0, 0, 1);
    // endList
    glEndList();
    return (bike1);
}







void bike_display(){
            glEnable(GL_TEXTURE_2D);
                glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
                glBindTexture(GL_TEXTURE_2D, _textureId_bike);
                //Bottom
                                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
                                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
                                glmFacetNormals(myModel);
                                glmVertexNormals(myModel,90);
                                glRotatef(90,0,1,0);
                                glScalef(0.09,0.09,0.09);
                                glmDraw(myModel ,GLM_SMOOTH | GLM_TEXTURE);
                                glDisable(GL_TEXTURE_2D) ;
}
GLuint DLid;
GLuint DLid_terrain;
GLuint createDL(void);
GLuint createDL() {
    GLuint snowManDL;
    // Create the id for the list
    snowManDL = glGenLists(1);
    // start list
    glNewList(snowManDL,GL_COMPILE);
    // call the function that contains the rendering commands
        bike_display();
    // endList
    glEndList();
    return(snowManDL);
}
void selectcolor(Terrain* _terrain, int x, int z) {
    float h = _terrain->getHeight(x, z);
    float red = 0.020 * (h + 35);
    float green = 0.016 * (h + 35) + 0.2;
    float blue = 0.014 * (h + 35);

    glColor3f(red, green, blue);

}
void terrain_ch(){
    //glScalef(5,5,5);
    for(int z = 0; z < _terrain->length() - 1; z++) {
        //glScalef(5.0,5.0,5.0);
            //Makes OpenGL draw a triangle at every three consecutive vertices
            glBegin(GL_TRIANGLE_STRIP);
            for(int x = 0; x < _terrain->width(); x++) {

                selectcolor(_terrain,x,z) ;


                vector normal = _terrain->getNormal(x, z);
                            glNormal3f(normal.x, normal.y, normal.z);
                glVertex3f(x, (_terrain->getHeight(x, z)), z);
                normal = _terrain->getNormal(x, z + 1);
                glNormal3f(normal.x, normal.y, normal.z);
                glVertex3f(x, (_terrain->getHeight(x, z + 1)),( z + 1));

            }
            glEnd();
        }

}
GLuint createDL_terrain() {
    GLuint snowManDL;
    // Create the id for the list
    snowManDL = glGenLists(1);
    // start list
    glNewList(snowManDL,GL_COMPILE);
    // call the function that contains the rendering commands
        terrain_ch();
    // endList
    glEndList();
    return(snowManDL);
}
void initRendering() {
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_NORMALIZE);
    glShadeModel(GL_SMOOTH);
    glMatrixMode(GL_MODELVIEW);
    t3dInit();
        glLoadIdentity();
        gluOrtho2D(-100.0,100.0,50.0,100.0);
        Image* image = loadBMP("E:/ab_to_pakka/heightmap.bmp");
                        _textureId_bike= loadTexture(image);
                        delete image;

                         image = loadBMP("E:/ab_to_pakka/shu1.bmp");
                            _textureId = loadTexture(image);

                            delete image;
                            image = loadBMP("E:/ab_to_pakka/House1.bmp");
                            _textureId_house = loadTexture(image);

                            delete image;
                            image = loadBMP("E:/ab_to_pakka/treestompcolmap.bmp");
                            _textureId_treetrunk = loadTexture(image);

                            delete image;

                            image = loadBMP("E:/ab_to_pakka/txt_001_diff.bmp");
                                                        _textureId_water = loadTexture(image);

                                                        delete image;

                            DLid_tree = createDL_tree();
                                DLid_House = createDL_House();
                                DLid_Rock = createDL_rock();
                                DLid_Treetrunk = createDL_treetrunk();
                        DLid = createDL();
                        DLid_terrain = createDL_terrain();
                        DLid_star = createDL_star();


                        }

void handleResize(int w, int h) {
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    //glOrtho(-100.0,100.0,0.0,100.0,1,2000);

    gluPerspective(45.0, (double)w / (double)h, 0.20, 2000.0);
}




void collisionEffect(){
    cout<<"collision effect"<<endl;
    float e=-0.7;
    bike.velocity.x=e*(bike.velocity.x);
    bike.velocity.z=e*(bike.velocity.z);
    bs=0.0;
    bike.center.x=bike.center.x+bike.velocity.x*5;
    bike.center.z=bike.center.z+bike.velocity.z*5;
bike.front=bike.velocity.Add(bike.head.ConstProduct(bike.head,bike.fp),bike.center);
    bike.back=bike.velocity.Add(bike.head.ConstProduct(bike.head,-bike.fp),bike.center);

}


void air(){


    bike.center.x=bike.center.x+bike.velocity.x;
    bike.center.z=bike.center.z+bike.velocity.z;
    //bike.velocity.y=bike.velocity.y+g;
    bike.yvelocity=bike.yvelocity+g;
    bike.center.y=bike.center.y+bike.yvelocity;


}

void accelerate(){
    if(printScore){

    score+=1;}
    vector a(bike.head.x,bike.head.y,bike.head.z);

    vector n=a.Normalise(a);
    vector t=a.ConstProduct(n,acc);
    if(bike.velocity.Magnitude(bike.velocity)>0.60&&bike.velocity.Magnitude(bike.velocity)<1.2){
        vector t=a.ConstProduct(n,acc);
                bike.velocity.x=(bike.velocity.x+t.x*0.04);
                bike.velocity.z=(bike.velocity.z+t.z*0.04);
                bike.center.x=bike.center.x+bike.velocity.x+bike.head.x*bs;
                bike.center.z=bike.center.z+bike.velocity.z+bike.head.z*bs;




    }
    else if(bike.velocity.Magnitude(bike.velocity)>01.20){
        bike.center.x=(bike.center.x+bike.velocity.x+bike.head.x*bs);
        bike.center.z=(bike.center.z+bike.velocity.z+bike.head.z*bs);
    }
    else{
        bike.velocity.x=(bike.velocity.x+t.x+t.x*bs);
        bike.velocity.z=(bike.velocity.z+t.z+t.z*bs);
        bike.center.x=bike.center.x+bike.velocity.x;
        bike.center.z=bike.center.z+bike.velocity.z;



    }



    bike.front=bike.velocity.Add(bike.head.ConstProduct(bike.head,bike.fp),bike.center);
    bike.back=bike.velocity.Add(bike.head.ConstProduct(bike.head,-bike.fp),bike.center);

                    float yf= heightAt(_terrain,bike.front.x,bike.front.z);
                    float yb= heightAt(_terrain,bike.back.x,bike.back.z);
                    float d=(yf-yb)/2;
                    float yc=yb+d;
                bike.center.y=yc;



}

bool ai;



void decelerate(){


    vector a(bike.velocity.x,bike.velocity.y,bike.velocity.z);
        vector n=a.Normalise(a);
        if(bike.velocity.Magnitude(bike.velocity)<0.005){


        }
        else{
            vector t=a.ConstProduct(n,acc);
            bike.velocity.x=bike.velocity.x-t.x+t.x*hb;
            bike.velocity.z=bike.velocity.z-t.z+t.z*hb;
            bike.center.x=bike.center.x+bike.velocity.x;
            bike.center.z=bike.center.z+bike.velocity.z;
            bike.center.y=heightAt(_terrain,bike.center.x,bike.center.z);


        }
        bike.front=bike.velocity.Add(bike.head.ConstProduct(bike.head,bike.fp),bike.center);
            bike.back=bike.velocity.Add(bike.head.ConstProduct(bike.head,-bike.fp),bike.center);





}


void hanf(){
    vector f(bike.velocity.x,bike.velocity.y,bike.velocity.z);
    f.Normalise(f);
    f.x=accf*f.x;
    f.y=accf*f.y;
    f.z=accf*f.z;

    bike.velocity.x=bike.velocity.x+f.x+f.x*hb;
    //bike.velocity.y=bike.velocity.y+f.y;
    bike.velocity.z=bike.velocity.z+f.z+f.z*hb;


    bike.center.x=bike.center.x+bike.velocity.x;
    //bike.center.y=bike.center.y+bike.velocity.y;
    bike.center.z=bike.center.z+bike.velocity.z;
    bike.center.y=heightAt(_terrain,bike.center.x,bike.center.z);


    bike.front=bike.velocity.Add(bike.head.ConstProduct(bike.head,bike.fp),bike.center);
    bike.back=bike.velocity.Add(bike.head.ConstProduct(bike.head,-bike.fp),bike.center);

    float yf= heightAt(_terrain,bike.front.x,bike.front.z);
                    float yb= heightAt(_terrain,bike.back.x,bike.back.z);
                    float d=(yf-yb)/2;
                    float yc=yb+d;
                bike.center.y=yc;

}



void left(){


    float vel=bike.velocity.Magnitude(bike.velocity);
    if(vel>0.005f){
        rotate_y+=1.0f;
        vector u=bike.velocity.ConstProduct(bike.velocity.RotateVectorLeft(bike.velocity,normalup,1.0f),vel);
        bike.velocity.x=u.x;
        //bike.velocity.y=u.y;
        bike.velocity.z=u.z;

        vector h=bike.velocity.RotateVectorLeft(bike.head,normalup,1.0f);

        //bike.head=bike.velocity.Normalise(bike.velocity);
        bike.head.x=h.x;
        bike.head.y=h.y;
        bike.head.z=h.z;
        bike.sideNormal=bike.velocity.Normalise(bike.velocity.CrossProduct(bike.head,normalup));
        bike.front=bike.velocity.Add(bike.head.ConstProduct(bike.head,bike.fp),bike.center);
            bike.back=bike.velocity.Add(bike.head.ConstProduct(bike.head,-bike.fp),bike.center);

    }
    else{
        rotate_y+=0.10f;
        vector h=bike.velocity.RotateVectorLeft(bike.head,normalup,0.10f);
                bike.head.x=h.x;
                bike.head.y=h.y;
                bike.head.z=h.z;

            bike.sideNormal=bike.velocity.Normalise(bike.velocity.CrossProduct(bike.head,normalup));
                    bike.front=bike.velocity.Add(bike.head.ConstProduct(bike.head,bike.fp),bike.center);
                        bike.back=bike.velocity.Add(bike.head.ConstProduct(bike.head,-bike.fp),bike.center);

    }



}

void right(){


    float vel=bike.velocity.Magnitude(bike.velocity);
    if(vel>0.005f){
        rotate_y-=1.0f;
        vector u=bike.velocity.ConstProduct(bike.velocity.RotateVectorRight(bike.velocity,normalup,1.0f),vel);
        bike.velocity.x=u.x;
    //	bike.velocity.y=u.y;
        bike.velocity.z=u.z;

        vector h=bike.velocity.RotateVectorRight(bike.head,normalup,1.0f);

        bike.head.x=h.x;
                bike.head.y=h.y;
                bike.head.z=h.z;
        bike.sideNormal=bike.velocity.Normalise(bike.velocity.CrossProduct(bike.head,normalup));
        bike.front=bike.velocity.Add(bike.head.ConstProduct(bike.head,bike.fp),bike.center);
        bike.back=bike.velocity.Add(bike.head.ConstProduct(bike.head,-bike.fp),bike.center);


    }
    else{
        rotate_y-=0.10f;

        vector h=bike.velocity.RotateVectorRight(bike.head,normalup,0.10f);

                        bike.head.x=h.x;
                        bike.head.y=h.y;
                        bike.head.z=h.z;


                bike.sideNormal=bike.velocity.Normalise(bike.velocity.CrossProduct(bike.head,normalup));
                bike.front=bike.velocity.Add(bike.head.ConstProduct(bike.head,bike.fp),bike.center);
                bike.back=bike.velocity.Add(bike.head.ConstProduct(bike.head,-bike.fp),bike.center);

    }
}


void handleKeypress(unsigned char key, int x, int y) {
    if(key==32){
        handbreak=true;
        }
}
void keyup1(unsigned char k, int x, int y ){

    if(k==32){
        handbreak=false;
        hb=0.0;
    }
}


void keyup(int k , int x, int y){
       if (k == GLUT_KEY_UP){

    pressed=false;
      }
       if (k == GLUT_KEY_LEFT)
       {
           rotate_xl=-2.5;
           pressedl=false;
       }
       if (k == GLUT_KEY_RIGHT)
           {
           rotate_xr=2.5;
               pressedr=false;
           }

        if(k==GLUT_KEY_F1){
          boost=false;
          bs=0.0;
        }

}

void specialKeys( int key, int x, int y ) {

  if (key == GLUT_KEY_RIGHT){

      pressedr=true;

  }


  else if (key == GLUT_KEY_LEFT){

      pressedl=true;


  }

  else if (key == GLUT_KEY_UP){

      pressed=true;
  }


  else if (key == GLUT_KEY_DOWN){
      if(handbreak){
          hb=-5.0;
      }
      decelerate();
  }

  else if(key==GLUT_KEY_F1){
      boost=true;
    }



  glutPostRedisplay();

}
float _scale;
const char* STRS[1] = {"Speed"};
float computeScale(const char* strs[1]) {
    float maxWidth = 0;
    for(int i = 0; i < 1; i++) {
        float width = t3dDrawWidth(strs[i]);
        if (width > maxWidth) {
            maxWidth = width;
        }
    }

    return 2.6f / maxWidth;
}


void cleanup1() {
    t3dCleanup();
}

void drawNumCollisions(int numCollisions,int y,int s,bool g) {
    ostringstream oss;
    ostringstream oss1;
    ostringstream oss2;
    ostringstream oss3;

    oss << "Score: " << numCollisions;




    string str = oss.str();
    oss1 << "Speed: " << y;

    string str1 = oss1.str();
    oss2 << " Star: " << s;
    string str2 = oss2.str();
    oss3  << "GAME OVER";
        string str3 = oss3.str();

    glDisable(GL_TEXTURE_2D);
    glDisable(GL_LIGHTING);
    glColor3f(1.0f, 1.0, 0.0f);
    glPushMatrix();
    glTranslatef(-2.50f, 0.90f, -5.0f);
    glScalef(0.1f, 0.1f, 0.1f);
    t3dDraw2D(str, 0, 0);
    glTranslatef(13.0f, 2.0f, -5.0f);


    t3dDraw2D(str1, 0, 0);
    glTranslatef(15.0f, 2.0f, -5.0f);

    t3dDraw2D(str2, 0, 0);
    if(g){
        glTranslatef(-15.0f, -5.0f, -5.0f);
        glScalef(4.0,4.0,4.0);
        //
    t3dDraw2D(str3, 0, 0);
    }

    glPopMatrix();


    glEnable(GL_LIGHTING);
}



void drawScene() {
    glClearColor (0.2, 0.6, 1.0, 0.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
glPushMatrix();
        glTranslatef(1.0,1.0,0);
        drawNumCollisions(score,speed,star,gameOver);
        glPopMatrix();

    float s= 2.0;
    gluLookAt((bike.center.x-bike.head.x*s),bike.center.y+0.50,(bike.center.z-bike.head.z*s),bike.center.x,bike.center.y,bike.center.z,0.0,1.0,0.0);
    GLfloat ambientColor[] = {0.4f, 0.4f, 0.4f, 1.0f};
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambientColor);

    GLfloat lightColor0[] = {0.6f, 0.6f, 0.6f, 1.0f};
    GLfloat lightPos0[] = {-0.5f, 0.8f, 0.1f, 0.0f};
    glLightfv(GL_LIGHT0, GL_DIFFUSE, lightColor0);
    glLightfv(GL_LIGHT0, GL_POSITION, lightPos0);

    glColor3f(0.3f, 0.9f, 0.0f);
    glPushMatrix();
    glCallList(DLid_terrain);
    glPopMatrix();
    glPushMatrix();
        glColor3f(1.0, 1.0, 1.0);
        glPushMatrix();
        glTranslatef(5.0, _terrain->getHeight(5.0, 5.0), 5.0);

        glCallList(DLid_tree);
        glPopMatrix();
        glPushMatrix();
        glTranslatef(175.0f, _terrain->getHeight(175.0, 150.0), 150.0f);
        glCallList(DLid_tree);
        glPopMatrix();

        glPushMatrix();
        glTranslatef(40.0f, _terrain->getHeight(40.0, 220.0), 220.0f);
        glCallList(DLid_tree);
        glPopMatrix();
        glPushMatrix();
        glTranslatef(220.0f, _terrain->getHeight(220.0, 50.0), 50.0f);
        glCallList(DLid_tree);
        glPopMatrix();
        glPushMatrix();
        glTranslatef(180.0f, _terrain->getHeight(180.0, 180.0), 180.0f);
        glCallList(DLid_tree);
        glPopMatrix();
        glPushMatrix();
        glTranslatef(60.0f, _terrain->getHeight(60.0, 70.0), 70.0f);
        glCallList(DLid_tree);
        glPopMatrix();
        glPushMatrix();
        glTranslatef(195.0f, _terrain->getHeight(215.0, 85.0), 85.0f);
        glCallList(DLid_tree);
        glPopMatrix();
        glPushMatrix();
        glTranslatef(194.0f, _terrain->getHeight(194.0, 20.0), 20.0f);
        glCallList(DLid_tree);
        glPopMatrix();
        glPushMatrix();
        glTranslatef(215.0f, _terrain->getHeight(215.0, 158.0), 158.0f);
        glCallList(DLid_tree);
        glPopMatrix();
        glPushMatrix();
        glTranslatef(90.0f, _terrain->getHeight(90.0, 215.0), 215.0f);
        glCallList(DLid_tree);
        glPopMatrix();
        glPushMatrix();
        glTranslatef(85.0f, _terrain->getHeight(205.0, 20.0), 20.0f);
        glCallList(DLid_tree);
        glPopMatrix();

        glPushMatrix();
        glTranslatef(200.0f, _terrain->getHeight(200.0, 10.0), 10.0f);
        glCallList(DLid_House);
        glPopMatrix();


        glPushMatrix();
        glTranslatef(10.0f, _terrain->getHeight(10.0, 170.0), 170.0f);
        glCallList(DLid_House);
        glPopMatrix();

        glPushMatrix();
        glTranslatef(30.0f, _terrain->getHeight(30.0, 30.0), 30.0f);
        glCallList(DLid_Rock);
        glPopMatrix();

        glPushMatrix();
        glTranslatef(121.0f, _terrain->getHeight(121.0, 20.0), 20.0f);
        glCallList(DLid_Rock);
        glPopMatrix();
        glPushMatrix();
        glTranslatef(225.0f, _terrain->getHeight(225.0, 211.0), 211.0f);
        glCallList(DLid_Rock);
        glPopMatrix();
        glPushMatrix();
        glTranslatef(105.0f, _terrain->getHeight(105.0, 201.0), 201.0f);
        glCallList(DLid_Rock);
        glPopMatrix();
        glPushMatrix();
        glTranslatef(110.0f, _terrain->getHeight(110.0, 110.0), 110.0f);
        glCallList(DLid_Rock);
        glPopMatrix();

        glPushMatrix();
        glTranslatef(50.0f, _terrain->getHeight(50.0, 50.0), 50.0f);
        glCallList(DLid_Treetrunk);
        glPopMatrix();

        glPushMatrix();
        glTranslatef(140.0f, _terrain->getHeight(140.0, 135.0), 135.0f);
        glCallList(DLid_Treetrunk);
        glPopMatrix();

        glPushMatrix();
        glTranslatef(85.0f, _terrain->getHeight(85.0, 31.0), 31.0f);
        glCallList(DLid_Treetrunk);
        glPopMatrix();

        glPushMatrix();
        glTranslatef(112.0f, _terrain->getHeight(112.0, 171.0), 171.0f);
        glCallList(DLid_Treetrunk);
        glPopMatrix();

        glPushMatrix();
        glTranslatef(56.0f, _terrain->getHeight(56.0, 212.0), 212.0f);
        glCallList(DLid_Treetrunk);
        glPopMatrix();
        glPopMatrix();
        glPushMatrix();
        glTranslatef(128,-23,128);
        glScalef(1000,1000,1000) ;
        glEnable(GL_TEXTURE_2D);
        glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
        glBindTexture(GL_TEXTURE_2D, _textureId_water);

                                        //Bottom
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glColor3f(0.105,0.905,0.964) ;
                glBegin(GL_QUADS) ;
                glTexCoord2f(1.0f, 1.0f);
                glVertex3f(1,0,1) ;
                glTexCoord2f(1.0f, 0.0f);
                glVertex3f(1,0,-1) ;
                glTexCoord2f(0.0f, 0.0f);
                glVertex3f(-1,0,-1) ;
                glTexCoord2f(0.0f, 1.0f);
                glVertex3f(-1,0,1) ;

    glEnd();
    glDisable(GL_TEXTURE_2D) ;
    glPopMatrix();

        glColor3f(1,1,0) ;
        glPushMatrix();
        if(arr[0]==false){
            glTranslatef(190.0f,_terrain->getHeight(190,40)+5, 40.0f);

        }
        else{
            glTranslatef(10000,_terrain->getHeight(190,40)+5, 40.0f);

        }
    //	glTranslatef(190.0f,_terrain->getHeight(190,40)+5, 40.0f);
                            glCallList(DLid_star);
                        //	glColor3f(1, 1, 1);
                            glPopMatrix();

                            glPushMatrix();
                            int StarHeight=0.0;
                            if(arr[1]==true){
                                glTranslatef(1300.0f, _terrain->getHeight(130, 78) +StarHeight , 78.0f);

                            }
                            else{
                            glTranslatef(130.0f, _terrain->getHeight(130, 78) +StarHeight , 78.0f);
                            }
                            glCallList(DLid_star);

                            glPopMatrix();
                            glPushMatrix();
                            if(arr[2]==true){
                                glTranslatef(9400.0f, _terrain->getHeight(94, 17) + StarHeight, 17.0f);

                            }
                            else{
                                glTranslatef(94.0f, _terrain->getHeight(94, 17) + StarHeight, 17.0f);

                            }
                            glCallList(DLid_star);

                            glPopMatrix();
                            glPushMatrix();
                            if(arr[3]==true){
                                glTranslatef(1900.0f, _terrain->getHeight(190, 100) + StarHeight, 100.0f);

                            }
                            else{
                            glTranslatef(190.0f, _terrain->getHeight(190, 100) + StarHeight, 100.0f);
                            }
                            glCallList(DLid_star);

                            glPopMatrix();
                            glPushMatrix();
                            if(arr[4]==true){
                                glTranslatef(2200.0f, _terrain->getHeight(220, 240) + StarHeight, 240.0f);

                            }
                            else{
                                glTranslatef(220.0f, _terrain->getHeight(220, 240) + StarHeight, 240.0f);

                            }
                            glCallList(DLid_star);

                            glPopMatrix();
                            glPushMatrix();
                            if(arr[5]==true){
                                glTranslatef(10000.0f, _terrain->getHeight(215, 80) + StarHeight, 80.0f);

                            }
                            else{
                            glTranslatef(215.0f, _terrain->getHeight(215, 80) + StarHeight, 80.0f);
                            }
                            glCallList(DLid_star);

                            glPopMatrix();
                            glPushMatrix();
                            if(arr[6]==true){
                                        glTranslatef(10000.0f, _terrain->getHeight(215, 80) + StarHeight, 80.0f);
                                    }
                            else{
                            glTranslatef(150.0f, _terrain->getHeight(150, 180) + StarHeight, 180.0f);
                            }
                            glCallList(DLid_star);

                            glPopMatrix();
                            glPushMatrix();
                            if(arr[7]==true){
                                                            glTranslatef(10000.0f, _terrain->getHeight(215, 80) + StarHeight, 80.0f);

                                                        }
                            else{
                            glTranslatef(60.0f, _terrain->getHeight(60, 180) + StarHeight, 200.0f);
                            }
                            glCallList(DLid_star);

                            glPopMatrix();
                            glPushMatrix();
                            if(arr[8]==true){
                                glTranslatef(10000.0f, _terrain->getHeight(215, 80) + StarHeight, 80.0f);

                                                        }
                            else{
                            glTranslatef(45.0f, _terrain->getHeight(45, 204) + StarHeight, 204.0f);
                            }
                            glCallList(DLid_star);

                            glPopMatrix();
                            glPushMatrix();
                            if(arr[9]==true){
                                            glTranslatef(10000.0f, _terrain->getHeight(215, 80) + StarHeight, 80.0f);

                                                        }
                            else{
                            glTranslatef(170.0f, _terrain->getHeight(170, 30) + StarHeight, 30.0f);
                            }
                            glCallList(DLid_star);

                            glPopMatrix();
                            glPushMatrix();
                            if(arr[10]==true){
                                glTranslatef(4000.0f, _terrain->getHeight(40, 40) + StarHeight, 40.0f);

                            }
                            else{
                                glTranslatef(40.0f, _terrain->getHeight(40, 40) + StarHeight, 40.0f);

                            }
                            glCallList(DLid_star);

                            glPopMatrix();
                            glPushMatrix();
                            if(arr[11]==true){
                                         glTranslatef(10000.0f, _terrain->getHeight(215, 80) + StarHeight, 80.0f);

                                                        }
                            else{
                                glTranslatef(110.0f, _terrain->getHeight(110, 190) + StarHeight, 190.0f);

                            }
                            glCallList(DLid_star);

                            glPopMatrix();
                            glPushMatrix();
                            if(arr[12]==true){
                                glTranslatef(10000.0f, _terrain->getHeight(215, 80) + StarHeight, 80.0f);

                                                                                    }

                            else{


                            glTranslatef(90.0f, _terrain->getHeight(90, 150) + StarHeight, 150.0f);
                            }
                            glCallList(DLid_star);

                            glPopMatrix();
                            glPushMatrix();
                            if(arr[13]==true){
                                    glTranslatef(10000.0f, _terrain->getHeight(215, 80) + StarHeight, 80.0f);

                                                                                    }
                            else{
                                glTranslatef(210.0f, _terrain->getHeight(210, 220) + StarHeight, 220.0f);
                            }
                            glCallList(DLid_star);
                        glColor3f(1,1,1) ;
                            glPopMatrix();


    glPushMatrix() ;


        if(ai){
            if((speed)<150){
                if(printScore){
                    score+=2;
                }

                    }
            else if(speed>150&&speed<500){
                if(printScore){
                                    score+=3;
                                }

                            }
                    else{

                    }

            bike.center.x=bike.center.x+bike.velocity.x*0.7+bike.head.x*bs;
                bike.center.z=bike.center.z+bike.velocity.z*0.7+bike.head.z*bs;
                //bike.velocity.y=bike.velocity.y+g;
                bike.yvelocity=bike.yvelocity-g;
                bike.center.y=bike.center.y+bike.yvelocity;
                bike.sideNormal=bike.velocity.Normalise(bike.velocity.CrossProduct(bike.head,normalup));
                bike.front=bike.velocity.Add(bike.head.ConstProduct(bike.head,bike.fp),bike.center);
                bike.back=bike.velocity.Add(bike.head.ConstProduct(bike.head,-bike.fp),bike.center);

        cout<<"ifffffyyyyyy"<<bike.center.y<<endl;
        v.PrintVector(bike.center);
                glTranslatef(bike.center.x, bike.center.y+0.2,bike.center.z);
        }
        else{
            //1.8
        glTranslatef(bike.center.x, bike.center.y+0.2,bike.center.z);
        }
        glRotatef(rotate_y, 0.0, 1.0, 0.0 );
        glRotatef(rotate_xl, 1.0, 0.0, 0.0 );
        glRotatef(rotate_xr, 1.0, 0.0, 0.0 );
        glRotatef(0.5*rotate_z, 0.0, 0.0, 1.0 );
    glColor3f(1.0,1.0,1.0) ;
    glCallList(DLid);
        glPopMatrix();


     glutSwapBuffers();
}



void update(int value) {

speed=floor((v.Magnitude(bike.velocity)+bs)*300);
    float a1=angleAt(bike.front,bike.back,bike.head);

    collision(create(23),bike,23);
    Starcollision(createStar(14),bike,14);
    if(collide){
            cout<<"entered"<<endl;
        collisionEffect();
        collide=false;

    }

if(ai==true ||bike.center.x>256||bike.center.z>256||bike.center.x<-1||bike.center.z<-1){

    if(bike.center.y-heightAt(_terrain,bike.center.x,bike.center.z)>0.150 ||bike.center.x>256||bike.center.z>256||bike.center.x<-1||bike.center.z<-1){

            if(bike.center.x>257||bike.center.z>257||bike.center.x<-2||bike.center.z<-2){
                                        
//                                       
                                            gameOver=true;
                                    }


            ai=true;
        }
else{
        ai=false;
    }
}
else{
    rotate_z=a1;

    if(heightAt(_terrain,bike.center.x+bike.velocity.x,bike.center.z+bike.velocity.z)-heightAt(_terrain,bike.center.x,bike.center.z)<-0.20){
        ai=true;
    }////0.09

    else{
        ai=false;
bike.yvelocity=0.00;


if(pressed){

if(boost){
    bs=0.5;
}




    if(pressedl){

        left();
        rotate_xl=-20;
    }

    if(pressedr){

        right();

            rotate_xr=20;

        }


    accelerate();
}
else{
    if(handbreak){
        hb=5;
    }
    if(pressedr){

        right();
        rotate_xr=20;
    }
    if(pressedl){
        left();
        rotate_xl=-20;
    }
    hanf();

}
    }
}


    glutKeyboardFunc(handleKeypress);
        glutKeyboardUpFunc(keyup1);
        glutSpecialFunc(specialKeys);
        glutSpecialUpFunc(keyup);
        glutPostRedisplay();
        glutTimerFunc(10, update, 0);
}








int start1(int i) {


    if(i==1){
        _terrain = loadTerrain("E:/ab_to_pakka/terrain1.bmp", 50);


    }
    else if(i==2){
        _terrain = loadTerrain("E:/ab_to_pakka/terrain2.bmp", 100);
    }
    else if(i==3){
            _terrain = loadTerrain("E:/ab_to_pakka/terrain3.bmp", 100);
        }
    else if(i==4){
            _terrain = loadTerrain("E:/ab_to_pakka/terrain4.bmp", 100);
        }
    else if(i==5){
            _terrain = loadTerrain("E:/ab_to_pakka/terrain5.bmp", 100);
        }
    else if(i==6){
            _terrain = loadTerrain("E:/ab_to_pakka/terrain6.bmp", 100);
        }
    else if(i==7){
            _terrain = loadTerrain("E:/ab_to_pakka/terrain7.bmp", 100);
        }
    else if(i==8){
            _terrain = loadTerrain("E:/ab_to_pakka/terrain8.bmp", 100);
        }
    else if(i==9){
            _terrain = loadTerrain("E:/ab_to_pakka/terrain9.bmp", 100);
        }
    else if(i==10){
            _terrain = loadTerrain("terrain10.bmp", 100);
        }

   // glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(1366,768);

    glutCreateWindow("Terrain - videotutorialsrock.com");
    //_terrain = loadTerrain("terrain256.bmp", 100);
    star1 = glmReadOBJ("E:/ab_to_pakka/star.obj");

    myModel = glmReadOBJ("E:/ab_to_pakka/quad.obj");
    myModel_tree = glmReadOBJ("E:/ab_to_pakka/lowpolytree5.obj");
        myHouse = glmReadOBJ("E:/ab_to_pakka/House1.obj");
        myRock = glmReadOBJ("E:/ab_to_pakka/rock1.obj");
        treetrunk = glmReadOBJ("E:/ab_to_pakka/treestompsculpture.obj");
    initRendering();
    glutDisplayFunc(drawScene);
    _scale = computeScale(STRS);
    glutKeyboardFunc(handleKeypress);
    glutKeyboardUpFunc(keyup1);
    glutReshapeFunc(handleResize);
    glutSpecialFunc(specialKeys);
    glutSpecialUpFunc(keyup);

    glutTimerFunc(2, update, 0);
        glutMainLoop();



    return 0;

}
















