//
//  BallDefinition.cpp
//  BallDefinition
//
//  Created by 廖芊芊 on 2020/6/8.
//  Copyright © 2020年 廖芊芊. All rights reserved.
//

#include "BallDefinition.hpp"
#include "CreatTexture.hpp"

Ball::Ball(Float Radius, Float Distance, Float Speed, Float SelfSpeed, Ball * Parent, int planet) {
    Float4(Color, 0.8f, 0.8f, 0.8f, 1.0f);
    this->Radius = Radius;
    this->SelfSpeed = SelfSpeed;
    if (Speed > 0)
        this->Speed = 360.0f / Speed;
    AlphaSelf = Alpha= 0;
    this->Distance = Distance;
    ParentBall = Parent;
    this->planet = planet;
}

#include <stdio.h>
#include <math.h>
#define PI 3.1415926535

// 对普通的球体进行移动和旋转
void Ball::DrawBall() {
    
    glEnable(GL_LINE_SMOOTH);
    glEnable(GL_BLEND);
    
    int n = 1440;
    
    glPushMatrix();
    
    // 公转
    if (ParentBall != 0 && ParentBall->Distance > 0) {
        glRotatef(ParentBall->Alpha, 0, 0, 1);
        glTranslatef(ParentBall->Distance, 0.0, 0.0);
        
        glBegin(GL_LINES);
        for(int i=0; i<n; ++i)
            glVertex2f(Distance * cos(2 * PI * i / n),
                       Distance * sin(2 * PI * i / n));
        glEnd();
    } else {
        glBegin(GL_LINES);
        for(int i=0; i<n; ++i)
            glVertex2f(Distance * cos(2 * PI * i / n),
                       Distance * sin(2 * PI * i / n));
        glEnd();
    }
    glRotatef(Alpha, 0, 0, 1);
    glTranslatef(Distance, 0.0, 0.0);
    
    
    // 自转
    glRotatef(AlphaSelf, 0, 0, 1);
    
    // 绘图
    glEnable(GL_TEXTURE_GEN_S);      // 自动生成s方向纹理坐标
    glEnable(GL_TEXTURE_GEN_T);      // 自动生成t方向纹理坐标

    switch (planet) {
        case 0:
            glBindTexture(GL_TEXTURE_2D,CreateTextureFromPng("/Users/liaoqianqian/Desktop/openGL_cpp/solarSystem/planet/sun.png"));
            break;
        case 1:
            glBindTexture(GL_TEXTURE_2D,CreateTextureFromPng("/Users/liaoqianqian/Desktop/openGL_cpp/solarSystem/planet/mercury.png"));
            break;
        case 2:
            glBindTexture(GL_TEXTURE_2D,CreateTextureFromPng("/Users/liaoqianqian/Desktop/openGL_cpp/solarSystem/planet/venes.png"));
            break;
        case 3:
            glBindTexture(GL_TEXTURE_2D,CreateTextureFromPng("/Users/liaoqianqian/Desktop/openGL_cpp/solarSystem/planet/earth.png"));
            break;
        case 4:
            glBindTexture(GL_TEXTURE_2D,CreateTextureFromPng("/Users/liaoqianqian/Desktop/openGL_cpp/solarSystem/planet/moon.png"));
            break;
        case 5:
            glBindTexture(GL_TEXTURE_2D,CreateTextureFromPng("/Users/liaoqianqian/Desktop/openGL_cpp/solarSystem/planet/mars.png"));
            break;
        case 6:
            glBindTexture(GL_TEXTURE_2D,CreateTextureFromPng("/Users/liaoqianqian/Desktop/openGL_cpp/solarSystem/planet/jupiter.png"));
            break;
        case 7:
            glBindTexture(GL_TEXTURE_2D,CreateTextureFromPng("/Users/liaoqianqian/Desktop/openGL_cpp/solarSystem/planet/saturn.png"));
            break;
        case 8:
            glBindTexture(GL_TEXTURE_2D,CreateTextureFromPng("/Users/liaoqianqian/Desktop/openGL_cpp/solarSystem/planet/uranus.png"));
            break;
        case 9:
            glBindTexture(GL_TEXTURE_2D,CreateTextureFromPng("/Users/liaoqianqian/Desktop/openGL_cpp/solarSystem/planet/neptune.png"));
            break;
        default:
            break;
    }

    //自动生成纹理坐标，这两句没有会变成一条一条或者一块一块
    glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);
    glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);

    GLUquadricObj *quadObj = gluNewQuadric();   //创建一个二次曲面物体
    gluQuadricTexture(quadObj,GL_TRUE);         //启用该二次曲面的纹理
    gluSphere(quadObj,Radius,40,32);            //画圆


    gluDeleteQuadric(quadObj);
    glDisable(GL_TEXTURE_2D);

    
    //不贴图效果
//    glColor3f(RFloat3(Color));
//    glutSolidSphere(Radius, 40, 32);
    

    glPopMatrix();
}

void Ball::Update(long TimeSpan) {
    Alpha += TimeSpan * Speed;
    AlphaSelf += SelfSpeed;
}

MatBall::MatBall(Float Radius, Float Distance, Float Speed, Float SelfSpeed,
                 Ball * Parent, Float3 color, int planet) : Ball(Radius, Distance, Speed, SelfSpeed, Parent, planet) {
    Float4(Color, color[0], color[1], color[2], 1.0f);
}

// 对材质进行设置
void MatBall::DrawMat() {
    GLfloat mat_ambient[]  = {0.0f, 0.0f, 0.5f, 1.0f}; //材质的环境颜色
    GLfloat mat_diffuse[]  = {0.0f, 0.0f, 0.5f, 1.0f}; //材质的散射颜色
    GLfloat mat_specular[] = {0.0f, 0.0f, 1.0f, 1.0f}; //材质的镜面反射颜色
    
    GLfloat mat_emission[] = {RFloat4(Color)};         //材质的发射光颜色
    GLfloat mat_shininess  = 90.0f;                    //镜面反射指数

    glMaterialfv(GL_FRONT, GL_AMBIENT,   mat_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE,   mat_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR,  mat_specular);
    glMaterialfv(GL_FRONT, GL_EMISSION,  mat_emission);
    glMaterialf (GL_FRONT, GL_SHININESS, mat_shininess);
}


LightBall::LightBall(Float Radius, Float Distance, Float Speed, Float SelfSpeed,
                     Ball * Parent, Float3 color, int planet)
    : MatBall(Radius, Distance, Speed, SelfSpeed, Parent, color, planet) {}

// 对光源进行设置
void LightBall::DrawLight() {
    GLfloat light_position[] = {0.0f, 0.0f, 0.0f, 1.0f};
    GLfloat light_ambient[]  = {1.0f, 0.0f, 0.0f, 1.0f};
    GLfloat light_diffuse[]  = {1.0f, 1.0f, 1.0f, 1.0f};
    GLfloat light_specular[] = {1.0f, 1.0f, 1.0f, 1.0f};

    glLightfv(GL_LIGHT0, GL_POSITION, light_position); //光源位置
    glLightfv(GL_LIGHT0, GL_AMBIENT,  light_ambient); //环境光成分（0）
    glLightfv(GL_LIGHT0, GL_DIFFUSE,  light_diffuse); //漫反射
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular); //镜面反射
}

