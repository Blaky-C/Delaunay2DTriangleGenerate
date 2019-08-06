
#include "Delaunay.h"
#include <iostream>
using namespace std;

void testDelaunay() {

	ifstream infile("input.txt"); // 打开"input.txt"文件  
	if (!infile) {
		// 判断文件是否正常打开  
		cout << "Unable to input nodes!";
		exit(1);
	}

	string line;
	PointArray p;
	double xx, yy, zz;
	int nodeSize;
	for (int i = 0; i < 4; i++) {
		// 读入4外边框点  
		getline(infile, line);
		readPoint(xx, yy, zz, line);
		Point tmp = { xx,yy,zz };
		p.push_back(tmp);
	}
	
	Delaunay myMesh(p[0], p[1], p[2], p[3]); // 实例化Delaunay类  
	getline(infile, line); // 读入节点数，用于后面循环   
	
	char *cstr;
	cstr = new char[line.size() + 1];
	strcpy(cstr, line.c_str());
	
	nodeSize = atoi(cstr);
	
	for (int i = 0; i < nodeSize; i++) {
		//读入每个节点的坐标
		getline(infile, line);
		readPoint(xx, yy, zz, line);
		myMesh.addPoint(xx, yy, zz);
	}
	
	infile.close();
	myMesh.deleteFrame();//删除外边框   
	myMesh.boundaryRecover(203, 466);
	myMesh.boundaryRecover(467, 487);
	myMesh.boundaryRecover(488, 511);
	myMesh.boundaryRecover(512, 537);//以上都是恢复指定边界  
	myMesh.output();//将相应ANSYS命令流输出  
}


int main() {
	testDelaunay();
	return 0;
}