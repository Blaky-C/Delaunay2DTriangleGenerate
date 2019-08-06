
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
} Point; // ����Point�ṹ��
typedef vector<Point> PointArray; // ����Point�ṹ���vector����

typedef struct {
	int left;
	int right;
	int count; // �ߵļ���
} Edge; // ����Edge�ṹ��
typedef vector<Edge> EdgeArray; // ����Edge�ṹ���vector����

typedef struct {
	int v[3]; // Triangle����������
	Edge s[3]; // Triangle��������
	double xc, yc, r; // Triangle���ԲԲ�ĵ�x��y���ꡢ�뾶
} Triangle; // ����Triangle�ṹ��
typedef vector<Triangle> TriangleArray; // �������������vector����

typedef struct {
	double x, y, radius;
} Circle;

typedef vector<int> intArray; // ?

class Delaunay {
public:
	Delaunay(Point p1, Point p2, Point p3, Point p4); // ������߿�
	~Delaunay();

	bool addPoint(double xx, double yy, double zz); // �������ʷ�ͼ���мӵ�
	void deleteFrame(); // ɾ����߿�
	void boundaryRecover(int fromPoint, int toPoint); // �߽�ָ�  
	void output(); // ���ANSYS�������ļ�  


private:
	void generateSuperTriangle(); // generate a super triangle automatically
	Circle calcCrcmCircle(Point p1, Point p2, Point p3); // calculate the circumcircle of 3 points
	void generateTriangle(Point p); // generate 
	int inCircle(Point p, Triangle t); // judge if the point p in the circumcircle of triangle t

	void calcCentre(double &x_centre, double &y_centre, double &radius, int n1, int n2, int n3); // ���������ε����ԲԲ������Ͱ뾶  
	bool inCircle(double xx, double yy, Triangle currentTris); // �жϵ��Ƿ���Բ��  
	void makeTriangle(int n1, int n2, int n3); // ����ָ�������������  
	void delTriangle(int n, EdgeArray &BoundEdges);//ɾ��ָ����������  
	
	int nodeSize;
	PointArray ptsList; // myPts�洢���е�  
	EdgeArray edgsList;// myEdges�洢���б�   
	TriangleArray trisList;//myTris���ڴ洢����������  
};

void readPointFromTxt(double &xx, double &yy, double &zz, string line); // ������input�ļ��ж�ȡ��ÿһ������  
#endif //DELAUNAY_H
