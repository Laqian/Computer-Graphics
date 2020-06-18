//
//  main.cpp
//  teapotLighting
//
//  Created by 廖芊芊 on 2020/5/24.
//  Copyright © 2020年 廖芊芊. All rights reserved.
//

#define _CRT_SECURE_NO_WARNINGS
#include <GLUT/glut.h>
#include <iostream>

using namespace std;

static GLfloat theta[] = { 0.0,0.0,0.0 };//旋转
int mousepx, mousepy;//鼠标坐标
bool valid=0;
bool displaymode = 0;
int mode;
void myDisplay(void)
{
    glClearColor(0.0, 0.0, 0.0, 0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);  //清理颜色和深度缓存
    glMatrixMode(GL_PROJECTION);// 投影模式
    glLoadIdentity();
    gluPerspective(80.0f, 1.0f, 1.0f, 20.0f);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(0.0, 12.0, 10.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
    //光源设置
    GLfloat light_position[] = { 5.0f, 5.0f, 0.0f, 1.0f }; //位置
    GLfloat light_ambient[] = { 1.0f, 1.0f, 1.0f, 1.0f }; //环境光
    GLfloat light_diffuse[] = { 1.0f, 1.0f,1.0f, 1.0f }; //漫反射光
    GLfloat light_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };//镜面光
    //物体材料参数设置
    GLfloat object_ambient[] = { 0.2f, 0.2f, 0.2f, 0.2f };  //材质环境光
    GLfloat object_diffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };  //材质的漫反射光
    GLfloat object_specular[] = { 1.0f,1.0f, 1.0f, 1.0f };   //材质的镜面反射光
    GLfloat object_emission[] = { 0.0f, 0.0f, 0.0f, 1.0f };   //材质的辐射光
    GLfloat object_shininess = 100.0f;
    GLfloat mode0[] = { 0.0f, 0.0f, 0.0f, 1.0f };
    
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);//应用设置
    glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHTING);
    glEnable(GL_DEPTH_TEST);
    
    
    switch (mode) {
        case 1://泛光 仅考虑环境光，其余光为0
            glMaterialfv(GL_FRONT, GL_AMBIENT, object_ambient);
            glMaterialfv(GL_FRONT, GL_DIFFUSE, mode0);
            glMaterialfv(GL_FRONT, GL_SPECULAR, mode0);
            glMaterialfv(GL_FRONT, GL_EMISSION, mode0);
            glMaterialf(GL_FRONT, GL_SHININESS, 0.0f);
            break;
        case 2://Lambert漫反射 考虑环境光和漫反射光，无镜面光
            glMaterialfv(GL_FRONT, GL_AMBIENT, object_ambient);
            glMaterialfv(GL_FRONT, GL_DIFFUSE, object_diffuse);
            glMaterialfv(GL_FRONT, GL_SPECULAR, mode0);
            glMaterialfv(GL_FRONT, GL_EMISSION, mode0);
            glMaterialf(GL_FRONT, GL_SHININESS, 0.0f);
            break;
        case 3://Phong光照模型 考虑环境光、漫反射光、镜面光，镜面光系数100
            glMaterialfv(GL_FRONT, GL_AMBIENT, object_ambient);
            glMaterialfv(GL_FRONT, GL_DIFFUSE, object_diffuse);
            glMaterialfv(GL_FRONT, GL_SPECULAR, object_specular);
            glMaterialfv(GL_FRONT, GL_EMISSION, object_emission);
            glMaterialf(GL_FRONT, GL_SHININESS, object_shininess);
            break;
    }
    
    //鼠标旋转操作函数
    glRotatef(theta[0], 1.0f, 0.0f, 0.0f);
    glRotatef(theta[1], 0.0f, 1.0f, 0.0f);

    glutSolidTeapot(5);
    glutSwapBuffers();
}

//鼠标移动事件
void motion(int x, int y)
{
    int w, h;
    w = glutGet(GLUT_WINDOW_WIDTH);//坐标
    h = glutGet(GLUT_WINDOW_HEIGHT);
    if (0 <= x && x <= w && 0 <= y && y <= h)
    {
        theta[0] = (mousepy - y) / (GLfloat)h * 360;//相对位移赋值旋转角
        theta[1] = (mousepx - x) / (GLfloat)w * 360;
        glutPostRedisplay();
    }
}

//鼠标点击事件
void mousedown(int mouse, int state, int x, int y)
{
    if (state == GLUT_DOWN)
    {
        mousepx = x;//获取坐标到全局
        mousepy = y;
    }
}

int main(int argc, char* argv[])
{
    int k;
    while (!valid) {
        cout<<"请选择光照明演示模型"<<endl;
        cout<<" 1 泛光模型 "<<endl;
        cout<<" 2 Lambert漫反射模型 "<<endl;
        cout<<" 3 Phong镜面反射模型 "<<endl;
        cin>>k;
        mode = k;
        if (mode == 1 || mode == 2 || mode == 3)
            valid = 1;
        else
            cout<<"模型输入有误，请重试"<<endl;
    }
    
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);
    glutInitWindowPosition(200, 200);
    glutInitWindowSize(600, 600);
    glutCreateWindow("光照模型演示");
    //回调函数
    glutMotionFunc(motion);
    glutDisplayFunc(myDisplay);
    glutMouseFunc(mousedown);
    glutMainLoop();
    return 0;
}




