#ifndef CSCIx229
#define CSCIx229

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <math.h>

#ifdef USEGLEW
#include <GL/glew.h>
#endif
#define GL_GLEXT_PROTOTYPES
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#define Cos(th) cos(3.1415926/180*(th))
#define Sin(th) sin(3.1415926/180*(th))

#ifdef __cplusplus
extern "C" {
#endif

void Print(const char* format , ...);
void Fatal(const char* format , ...);
unsigned int LoadTexBMP(const char* file);
void Project(double fov,double asp,double dim);
void ErrCheck(const char* where);
int  LoadOBJ(const char* file);
void special(int key,int x,int y);
void key(unsigned char ch,int x,int y);
void idle();
void reshape(int width,int height);

// Global Variables

extern int th;
extern int ph;
extern int mode;

extern double dim;
extern int fov;
extern double asp;
extern char* views[];
extern double EX;
extern double EY;
extern double EZ;
extern double AX;
extern double AY;
extern double AZ;

extern int light;
extern int ambient;
extern int diffuse;
extern int emission;
extern int specular;
extern int light_distance;
extern int light_angle;
extern int light_height;
extern int light_move;


#ifdef __cplusplus
}
#endif

#endif
