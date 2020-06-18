//
//  main.cpp
//  Lighting
//
//  Created by 廖芊芊 on 2020/5/24.
//  Copyright © 2020年 廖芊芊. All rights reserved.
//

#define _CRT_SECURE_NO_WARNINGS
#include <stdlib.h>
#include<stdio.h>
#include <GLUT/glut.h>
#include <iostream>

using namespace std;

static GLfloat angle = 0.0f;
static GLfloat theta[] = { 0.0,0.0,0.0 };//旋转
GLfloat sf = 0;//缩放
GLfloat tx = 0;//平移
GLfloat ty = 0;
int mousepx, mousepy;//鼠标坐标
bool valid=0;

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
    GLfloat mode1 = 10.0f;
    //printf("%d", mode);
    
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
    //旋转等操作函数
    glRotatef(theta[0], 1.0f, 0.0f, 0.0f);
    glRotatef(theta[1], 0.0f, 1.0f, 0.0f);
    glTranslatef(0.0f + tx, 0.0f + ty, 0.0f);//平移
    glScalef(1 + sf, 1 + sf, 1 + sf); //缩放变换
    switch (displaymode) {
        case 1:glutSolidCube(4);break;//画正方体
        case 2:glutSolidCone(3,6,50,50);break;
        case 3:glutSolidSphere(3,100,50);break;//画球体
        case 4:glutSolidTeapot(5);break;//画茶壶
    }
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
//键盘回调函数
void myKeyboard(unsigned char key, int x, int y) {
    //缩放
    if (key == 'z' || key == 'Z') {
        sf += 0.1;
    }
    if (key == 'x' || key == 'X') {
        sf -= 0.1;
    }
    //退出
    if (key == 'c' || key == 'C')
        exit(0);
    glutPostRedisplay(); //重新调用绘制函数
}
//窗口缩放比例
void myReshape(int w, int h)
{
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    if (w <= h)
        glOrtho(-4.0, 4.0, -3.0 * (GLfloat)h / (GLfloat)w,
                5.0 * (GLfloat)h / (GLfloat)w, -10.0, 10.0);
    else
        glOrtho(-4.0 * (GLfloat)w / (GLfloat)h,
                4.0 * (GLfloat)w / (GLfloat)h, -3.0, 5.0, -10.0, 10.0);
    glMatrixMode(GL_MODELVIEW);
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

//特殊键盘事件 平移
void myspecialKeyboard(int key, int x, int y) {
    if (key == GLUT_KEY_UP)
        ty += 0.1;
    if (key == GLUT_KEY_DOWN) ty -= 0.1;
    if (key == GLUT_KEY_LEFT) tx -= 0.1;
    if (key == GLUT_KEY_RIGHT) tx += 0.1;
    glutPostRedisplay();
}

int main(int argc, char* argv[])
{
    int k;
    while (!valid) {
        printf("光照明模型演示程序\n\t1、泛光模型\n\t2、Lambert漫反射模型\n\t3、Phong镜面反射模型\n请选择：");
        scanf("%d", &k);
        mode = k;
        if (mode == 1 || mode == 2 || mode == 3) {
            printf("请选择立体形状\n\t1、六面体\n\t2、棱锥\n\t3、球体\n\t4、茶壶\n请选择：");
            scanf("%d", &displaymode);
            if (displaymode == 1 || displaymode == 2 || displaymode == 3 || displaymode == 4) {
                valid = 1;
            }
            else {
                printf("立体形状输入有误，请重试。\n");
            }
        }
        else printf("模型输入有误，请重试。\n");
    }
    mode = k;
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);
    glutInitWindowPosition(200, 200);
    glutInitWindowSize(600, 600);
    glutCreateWindow("OpenGL光照模型演示");
    //回调函数们
    glutMotionFunc(motion);
    glutDisplayFunc(myDisplay);
    glutMouseFunc(mousedown);
    glutSpecialFunc(myspecialKeyboard);
    glutReshapeFunc(myReshape);
    glutKeyboardFunc(myKeyboard);
    glutMainLoop();
    return 0;
}



