
#ifndef DELAUNAY_H
#define DELAUNAY_H

#include <stdio.h>
#include <string.h>
#include <math.h>
#include <iostream>
#include <string>
#include <sstream>
#include <iomanip>
#include <fstream>
#include <vector>
using namespace std;

typedef struct {
	double x, y;
} Point;
typedef vector<Point> PointArray;

typedef struct {
	double x, y, radius;
} Circle; 

typedef struct {
	int left;
	int right;
	int count; // count the number of the same edges
} Edge;
typedef vector<Edge> EdgeArray;

typedef struct {
	int v[3]; // index of three vertexes of this triangle
	Edge s[3]; // three edges of this triangle
	Circle* cir;
} Triangle;
typedef vector<Triangle> TriangleArray;

typedef vector<int> intArray;

class Delaunay {
public:
	Delaunay();
	~Delaunay();

	void readPointFromTxt(string inTxt);
	Triangle* generateSuperTriangle(); // generate a super triangle automatically

	bool addPoint(double xx, double yy, double zz); // 向已有剖分图形中加点
	void deleteFrame(); // 删除外边框
	void boundaryRecover(int fromPoint, int toPoint); // 边界恢复  
	void output(); // 输出ANSYS命令流文件  


private:
	
	void generateTriangle(Point p); // generate 
	int inCircle(Point p, Triangle t); // judge if the point p in the circumcircle of triangle t

	bool inCircle(double xx, double yy, Triangle currentTris); // 判断点是否在圆内  
	void makeTriangle(int n1, int n2, int n3); // 生成指定顶点的三角形  
	void delTriangle(int n, EdgeArray &BoundEdges);//删除指定的三角形  
	
	int nodeSize;
	PointArray ptsList; // myPts存储所有点  
	EdgeArray edgsList;// myEdges存储所有边   
	TriangleArray trisList;//myTris用于存储所有三角形  
};

// void readPointFromTxt(double &xx, double &yy, double &zz, string line); // 解析从input文件中读取的每一行数据  

ofstream outputFile;
ifstream inputFile;

void openInputFile(string absolutePath) {
	// open ifstream path
	ifstream inputFile(absolutePath, ios::in);

//	if (!inFile) {
//		// judge if the file is opened correctly
//		cout << "ERROR!Unable to input nodes!\n";
//		exit(1);
//	}

}

string readLineFromInput() {
	// read every line from the input file

	string line;// 用来存放读取文件的每一行内容
	getline(inputFile, line);

	return line;
}

void closeInputFile() {
	// close ifstream path
	inputFile.close();
}

void openOutputFile(string absolutePath) {
	// open ofstream path
	outputFile = ofstream(absolutePath);
}

void closeOutputFile() {
	// close ofstream path
	outputFile.close();
}

void writeIntoOutput(string m) {
	// write into output file
	outputFile << m;
}


string trim(string s) {
	if (s.empty()) {
		return s;
	}
	s.erase(0, s.find_first_not_of(" "));
	s.erase(s.find_last_not_of(" ") + 1);
	return s;
}

string parseDoubleToString(double d) {
	stringstream ss;
	string result;

	ss << fixed << setprecision(6) << d;
	ss >> result;

	return result;
}

string parseIntegerToString(int i) {
	stringstream ss;
	string result;

	ss << i;
	ss >> result;

	return result;
}

double parseStringToDouble(string s) {
	s = trim(s);
	return atof(s.c_str());
}

int parseStringToInteger(string s) {
	s = trim(s);
	return atoi(s.c_str());
}

Point parseLineToPointWithComma(string line) {
	// Input format: 1, x, y

	int flag = 0;
	string tmp = "";
	double x, y;

	line = trim(line);

	// delete the index
	line = line.substr(line.find(",") + 1);
	line = trim(line);

	// obtain coordinate x
	tmp = trim(line.substr(0, line.find(",")));
	x = atof(tmp.c_str());
	line = line.substr(line.find(",") + 1);
	line = trim(line);

	// obtain coordinate y
	y = atof(line.c_str());

	if (fabs(x) < 1.0e-6) x = 0.0;
	if (fabs(y) < 1.0e-6) y = 0.0;

	Point p = { x,y };
	return p;

	//	char *cstr;
		//for (int i = (int)line.find(',') + 1; i < (int)line.size(); i++) {
		//	if (line[i] == ',') {
		//		cstr = new char[tmp.size() + 1];
		//		strcpy(cstr, tmp.c_str());
		//		if (flag == 0) {
		//			xx = atof(cstr);
		//			tmp.resize(0);
		//			flag++;
		//		}
		//		else if (flag == 1) {
		//			yy = atof(cstr);
		//			tmp.resize(0);
		//			flag++;
		//		}

		//		continue;
		//	}
		//	tmp = tmp + line[i];
		//}

}

Circle* calcCrcmCircle(Point* p1, Point* p2, Point* p3) {
	double x1, x2, x3, y1, y2, y3;
	x1 = p1->x;
	y1 = p1->y;
	x2 = p2->x;
	y2 = p2->y;
	x3 = p3->x;
	y3 = p3->y;

	double x_centre = ((y2 - y1)*(y3*y3 - y1 * y1 + x3 * x3 - x1 * x1) - (y3 - y1)*(y2*y2 - y1 * y1 + x2 * x2 - x1 * x1))
		/ (2 * (x3 - x1)*(y2 - y1) - 2 * ((x2 - x1)*(y3 - y1))); // calc coordinate x of CircumCircle
	double y_centre = ((x2 - x1)*(x3*x3 - x1 * x1 + y3 * y3 - y1 * y1) - (x3 - x1)*(x2*x2 - x1 * x1 + y2 * y2 - y1 * y1))
		/ (2 * (y3 - y1)*(x2 - x1) - 2 * ((y2 - y1)*(x3 - x1))); // calc coordinate y of CircumCircle
	double radius = sqrt((x1 - x_centre)*(x1 - x_centre) + (y1 - y_centre)*(y1 - y_centre)); // calc radius of CircumCircle

	Circle cl = { x_centre, y_centre, radius };

	return &cl;
}
#endif //DELAUNAY_H
