//  main.cpp
//  Curve
//  Created by 廖芊芊 on 2020/5/20.
//  Copyright © 2020年 廖芊芊. All rights reserved.


//  程序使用说明
//  Step 1:在控制台选择用什么方法生成曲线，0为Bezier曲线生成算法，1为Chaikin曲线细分算法
//  Step 2:在窗口画五个点
//  Step 3:完成后动态生成曲线


#include <GLUT/glut.h>
#include <math.h>
#include <iostream>

#define N 5     //有五个顶点

class Point{
    public:
    float x, y;
    void setPoint(int a, int b) {
        x = a;
        y = b;
    }
};

using namespace std;

Point points[N];
Point ps[11];   //t为0.1，则需要1/0.1个
Point parray[N];
Point ps1[2*N]; //第一次细分的点
Point ps2[4*N]; //第二次细分的点
int cnt = 0;  //顶点计数器
int curve_s = 0; //选择方法画曲线


void display()
{
    glClear(GL_COLOR_BUFFER_BIT);
    glFlush();
}


void drawLine(Point p1, Point p2) {
    glBegin(GL_LINES);
    glVertex2f(p1.x, p1.y);
    glVertex2f(p2.x, p2.y);
    glEnd();
    glFlush();
}

void drawPoint(Point p) {
    
    glPointSize(10.0); //点大小10*10
    glBegin(GL_POINTS);
    glVertex2f(p.x, p.y);
    glEnd();
    glFlush();
    glColor3f(0.0, 0.0, 1.0);//蓝色连线
    if (cnt > 0) drawLine(points[cnt - 1], points[cnt]); //画出两点之间连接的直线
}

//Bezier曲线的递归函数
Point bezierRecursion(int n, float t, Point *parray, Point *parray2){
    int i;
    if (n == N) {
        return parray[0];
    }
    else{
        for (i = 0; i < N - n; i++) {
            parray[i].setPoint(parray2[i].x * (1 - t) + parray2[i + 1].x * t, parray2[i].y * (1 - t) + parray2[i + 1].y * t);
        }
        return bezierRecursion(++n, t, parray, parray);
    }
}

//用de Casteljau方法计算Bezier曲线
void drawBezierCurve(){
    int i = 0;
    float t;
    for (t = 0; t < 1.1; t += 0.1) {
        ps[i++] = bezierRecursion(1, t, parray, points);
    }
    glColor3f(0.0, 0.0, 0.0); //曲线为黑色
    for(i = 0; i < 10; i++){                //若要生成多边形则 i < 11，或起点和终点交于一点
        drawLine(ps[i], ps[i+1]);
    }
}



//Chaikin细分曲线
void drawChaikinCurve(int n, int num, Point *ps1, Point *points){
    int i, j;
    int n1;
    if (n > 2) {  //进行两次细分
        return;
    }
    else{
        n1 = 2 * num;
        //取线段1：3和3:1为顶点
        for (i = 0, j = 0; j < n1 ; i++) {
            ps1[j++].setPoint((points[(i + 1) % num].x - points[i].x)/4 + points[i].x, (points[(i + 1) % num].y - points[i].y)/4 + points[i].y);
            ps1[j++].setPoint(3 * (points[(i + 1) % num].x - points[i].x) / 4 + points[i].x, 3 * (points[(i + 1) % num].y - points[i].y) / 4 + points[i].y);
        }
    }
    glColor3f(n, 0, 0); //第一次曲线为黑色，第二次曲线为红色
    for(i = 0; i < n1; i++){
        drawLine(ps1[i], ps1[(i + 1) % n1]);
    }
    //递归调用
    drawChaikinCurve(++n, n1, ps2, ps1);
    
}


// 鼠标
void mymouseFunction(int button, int state, int x, int y) {
    if (state == GLUT_DOWN){
        points[cnt].setPoint(x, 480 - y); // 坐标
        //绘制点
        glColor3f(1.0, 0.0, 0.0);//点为红色
        drawPoint(points[cnt]);
        if (cnt == N - 1) {         //读取全部点
            if (curve_s){
                //绘制chaikin细分曲线
                glColor3f(0.0, 0.0, 1.0);//蓝色连线
                drawLine(points[cnt],points[0]);  //连接第一条和最后一条
                drawChaikinCurve(0, N, ps1, points);
            }
            else
            //绘制berzir曲线
            drawBezierCurve();
            cnt = 0;
        }
        else {
            cnt++;
        }
    }
}

int main(int argc, char* argv[])
{
    
    cout<<"选择曲线生成算法"<<endl;
    cout<<"0 Bezier "<<endl;
    cout<<"1 Chaikin "<<endl;
    cin>>curve_s;

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE);
    glutInitWindowSize(600, 480);
    glutInitWindowPosition(200, 200);
    if (curve_s)
        glutCreateWindow("Chaikin");
    else
        glutCreateWindow("Bezier");

    //设置显示窗口的背景为白色
    glClearColor(1.0, 1.0, 1.0, 0);
    
    //设置投影类型
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0.0, 600.0, 0.0, 480.0); //平行投影
    
    
    glutMouseFunc(mymouseFunction); // 鼠标
    glutDisplayFunc(display); // 显示
    glutMainLoop();
    
    return 0;
}
