#include <math.h>
#include <iostream>
#include "CreatTexture.hpp"

void display()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    glEnable(GL_TEXTURE_GEN_S);      // 自动生成s方向纹理坐标
    glEnable(GL_TEXTURE_GEN_T);      // 自动生成t方向纹理坐标
    glBindTexture(GL_TEXTURE_2D,CreateTextureFromPng("/Users/liaoqianqian/Desktop/openGL_cpp/solarSystem/planet/sun.png"));
    
    glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);
    glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);
    
    GLUquadricObj *quadObj = gluNewQuadric();//创建一个二次曲面物体
    gluQuadricTexture(quadObj,GL_TRUE);        //启用该二次曲面的纹理
    gluSphere(quadObj,48.741,40,32); //画圆
    gluDeleteQuadric(quadObj);


    glutSwapBuffers();


}

int main(int ac,char** av)
{
    glutInit(&ac,av);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowPosition(100, 100);
    glutInitWindowSize(500, 500);
    
    glutCreateWindow("pngtest");
    
    glClearColor(1.0,1.0,1.0,1.0);
    glutDisplayFunc(display);
    glutReshapeFunc(Reshape);
    glutMainLoop();
    return 0;
}
