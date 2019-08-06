
#ifndef DELAUNAY_H
#define DELAUNAY_H

#include <stdio.h>
#include <string.h>
#include <math.h>
#include <iostream>
#include <string>
#include <fstream>
#include <vector>
using namespace std;

typedef struct {
	double x, y, z;
} Point; // 定义Point结构体
typedef vector<Point> PointArray; // 定义Point结构体的vector容器

typedef struct {
	int left;
	int right;
	int count; // 边的计数
} Edge; // 定义Edge结构体
typedef vector<Edge> EdgeArray; // 定义Edge结构体的vector容器

typedef struct {
	int v[3]; // Triangle的三个顶点
	Edge s[3]; // Triangle的三条边
	double xc, yc, r; // Triangle外接圆圆心的x、y坐标、半径
} Triangle; // 定义Triangle结构体
typedef vector<Triangle> TriangleArray; // 定义三角形类的vector容器

typedef struct {
	double x, y, radius;
} Circle;

typedef vector<int> intArray; // ?

class Delaunay {
public:
	Delaunay(Point p1, Point p2, Point p3, Point p4); // 构建外边框
	~Delaunay();

	bool addPoint(double xx, double yy, double zz); // 向已有剖分图形中加点
	void deleteFrame(); // 删除外边框
	void boundaryRecover(int fromPoint, int toPoint); // 边界恢复  
	void output(); // 输出ANSYS命令流文件  


private:
	void generateSuperTriangle(); // generate a super triangle automatically
	Circle calcCrcmCircle(Point p1, Point p2, Point p3); // calculate the circumcircle of 3 points
	void generateTriangle(Point p); // generate 
	int inCircle(Point p, Triangle t); // judge if the point p in the circumcircle of triangle t

	void calcCentre(double &x_centre, double &y_centre, double &radius, int n1, int n2, int n3); // 计算三角形的外接圆圆心坐标和半径  
	bool inCircle(double xx, double yy, Triangle currentTris); // 判断点是否在圆内  
	void makeTriangle(int n1, int n2, int n3); // 生成指定顶点的三角形  
	void delTriangle(int n, EdgeArray &BoundEdges);//删除指定的三角形  
	
	int nodeSize;
	PointArray ptsList; // myPts存储所有点  
	EdgeArray edgsList;// myEdges存储所有边   
	TriangleArray trisList;//myTris用于存储所有三角形  
};

void readPointFromTxt(double &xx, double &yy, double &zz, string line); // 解析从input文件中读取的每一行数据  
#endif //DELAUNAY_H
