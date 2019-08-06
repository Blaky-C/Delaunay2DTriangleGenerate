
#include "Delaunay.h"

Delaunay::Delaunay(Point p1, Point p2, Point p3, Point p4) {

	myPts.resize(4);
	myPts[0] = p1;
	myPts[1] = p2;
	myPts[2] = p3;
	myPts[3] = p4; // 添加四个外边框点

	myEdges.resize(4);
	Edge l1 = { 0, 1, -1 };
	Edge l2 = { 1,2,-1 };
	Edge l3 = { 0, 3, -1 };
	Edge l4 = { 2,3,-1 };
	myEdges[0] = l1;
	myEdges[1] = l2;
	myEdges[2] = l3;
	myEdges[3] = l4;
	makeTriangle(0, 1, 2);
	makeTriangle(0, 2, 3);

}

Delaunay::~Delaunay() {

	myPts.resize(0);
	myEdges.resize(0);
	myTris.resize(0);

}

void readPointFromTxt(string inTxt) {
	ifstream inFile(inTxt);
	
	if (!inFile) {
		// judge if the file is opened correctly
		cout << "ERROR!Unable to input nodes!\n";
		exit(1);
	}

	string line; // record each read line

	




}

void readPoint(double &xx, double &yy, double &zz, string line) {
	//从字符串line中解析出点的x,y,z坐标  
	int flag = 0;
	string tmp = "";
	char *cstr;
	for (int i = (int)line.find(',') + 1; i < (int)line.size(); i++) {
		if (line[i] == ',') {
			cstr = new char[tmp.size() + 1];
			strcpy(cstr, tmp.c_str());
			if (flag == 0) { 
				xx = atof(cstr); 
				tmp.resize(0); 
				flag++; 
			} else if (flag == 1) { 
				yy = atof(cstr); 
				tmp.resize(0); 
				flag++; 
			}
			
			continue;
		}
		tmp = tmp + line[i];
	}
	
	if (fabs(xx) < 1.0e-6) xx = 0.0;
	if (fabs(yy) < 1.0e-6) yy = 0.0;
	if (fabs(zz) < 1.0e-6) zz = 0.0;
}

void Delaunay::makeTriangle(int n1, int n2, int n3) {
	/**
	 生成指定坐标索引的三角形：
		1. 计算三角形的外接圆圆心与半径
		2. 生成三角形，添加至myTris列表
		3. 更新myEdges列表
	 */
	double x_centre, y_centre, radius;
	calcCentre(x_centre, y_centre, radius, n1, n2, n3); // 获得顶点为n1,n2,n3的三角形的外接圆圆心坐标和半径  

	Triangle newTriangle = { 
		{n1,n2,n3}, 
		{{n1,n2,1}, {n2,n3,1} ,{n1,n3,1}},
		x_centre, y_centre, radius 
	}; // 生成指定的三角形  
		
	myTris.push_back(newTriangle); // 向myTris中添加新构造的三角形  
	
	int edgeSize = (int)myEdges.size(); // 获得目前的边数  
	int flag;
	for (int i = 0; i < 3; i++)	{
		flag = 1;
		for (int j = 0; j < edgeSize; j++) {
			// 通过循环判断新构造的三角形的各边是否已经存在于myEdges中
			// 如果存在则只增加该边的计数，否则添加新边  
			if (newTriangle.s[i].left == myEdges[j].left
				&& newTriangle.s[i].right == myEdges[j].right
				&& myEdges[j].count != -1) {
				flag = 0;
				myEdges[j].count += 1;
				break;
			} else if (newTriangle.s[i].left == myEdges[j].left
				&& newTriangle.s[i].right == myEdges[j].right
				&& myEdges[j].count == -1) {
				flag = 0;
				break;
			}
		}

		if (flag == 1) myEdges.push_back(newTriangle.s[i]);
	}
}

void Delaunay::calcCentre(double &x_centre, double &y_centre, double &radius, int n1, int n2, int n3) {

	double x1, x2, x3, y1, y2, y3;
	x1 = myPts[n1].x;
	y1 = myPts[n1].y;
	x2 = myPts[n2].x;
	y2 = myPts[n2].y;
	x3 = myPts[n3].x;
	y3 = myPts[n3].y;
	x_centre = ((y2 - y1)*(y3*y3 - y1*y1 + x3*x3 - x1*x1) - (y3 - y1)*(y2*y2 - y1*y1 + x2*x2 - x1*x1)) 
		/ (2 * (x3 - x1)*(y2 - y1) - 2 * ((x2 - x1)*(y3 - y1)));//计算外接圆圆心的x坐标  
	y_centre = ((x2 - x1)*(x3*x3 - x1*x1 + y3*y3 - y1*y1) - (x3 - x1)*(x2*x2 - x1*x1 + y2*y2 - y1*y1)) 
		/ (2 * (y3 - y1)*(x2 - x1) - 2 * ((y2 - y1)*(x3 - x1)));//计算外接圆圆心的y坐标  
	radius = sqrt((x1 - x_centre)*(x1 - x_centre) + (y1 - y_centre)*(y1 - y_centre));//计算外接圆的半径  

}

bool Delaunay::addPoint(double xx, double yy, double zz) {
	
	EdgeArray boundEdges; // boundEdges用于存储在删除三角形后留下的边框，用于构造新的三角形  
	Point newPoint = { xx,yy,zz };
	
	myPts.push_back(newPoint); // 向myPts中添加新点  
	intArray badTriangle; // badTriangle用于存储不符合空圆规则的三角形的索引号  
	
	int TriSize = (int)myTris.size(); // 获得目前的三角形数  
	for (int i = 0; i < TriSize; i++) {
		// 通过循环找到所有不符合空圆规则的三角形，并将其索引号存在badTriangle中  
		if (inCircle(xx, yy, myTris[i])) badTriangle.push_back(i);
	}
	
	for (int i = 0; i < (int)badTriangle.size(); i++) {
		// 通过循环删除所有不符合空圆规则的三角形，同时保留边框  
		delTriangle(badTriangle[i], boundEdges);
		for (int j = i + 1; j < (int)badTriangle.size(); j++) badTriangle[j] -= 1;
	}

	int ptSize = (int)myPts.size();//获得目前的点数  
	for (int i = 0; i < (int)boundEdges.size(); i++) {
		//生成新的三角形 
		if (ptSize - 1 < boundEdges[i].left) makeTriangle(ptSize - 1, boundEdges[i].left, boundEdges[i].right);
		else if (ptSize - 1 > boundEdges[i].left && ptSize - 1 < boundEdges[i].right) makeTriangle(boundEdges[i].left, ptSize - 1, boundEdges[i].right);
		else makeTriangle(boundEdges[i].left, boundEdges[i].right, ptSize - 1);		
	}
	return true;
}

bool Delaunay::inCircle(double xx, double yy, Triangle currentTris) {
	// 判断点是否在三角形的外接圆内
	double dis = sqrt((currentTris.xc - xx)*(currentTris.xc - xx) + (currentTris.yc - yy)*(currentTris.yc - yy));
	if (dis > currentTris.r) return false;
	else return true;
}

void Delaunay::delTriangle(int n, EdgeArray &boundEdges) {
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < (int)myEdges.size(); j++) {
			if (myEdges[j].left == myTris[n].s[i].left && myEdges[j].right == myTris[n].s[i].right) {
				if (myEdges[j].count == 2) {
					//若要删除三角形的一边的计数为2，则将其计数减1，并将其压入BoundEdges容器中
					myEdges[j].count = 1;
					boundEdges.push_back(myEdges[j]);
				} else if (myEdges[j].count == -1) boundEdges.push_back(myEdges[j]);//如果是外边框，则直接压入BoundEdges容器中  
				else if (myEdges[j].count == 1) {
					//如果删除三角形的一边的计数为1，则删除该边，同时查看BoundEdges中是否有此边，若有，则删除
					for (int k = 0; k < (int)boundEdges.size(); k++) {
						if (boundEdges[k].left == myEdges[j].left&&boundEdges[k].right == myEdges[j].right) {
							boundEdges.erase(boundEdges.begin() + k);
							break;
						}
					}
					myEdges.erase(myEdges.begin() + j);
					j--;
				}
				break;
			}
		}
	}
	myTris.erase(myTris.begin() + n);//删除该三角形  
}

void Delaunay::output() {
	//向“output.log"文件中写入ANSYS命令流 
	ofstream outfile("output.log");
	
	if (!outfile) {
		cout << "Unable to output nodes!";
		exit(1);
	}
	
	outfile << "/PREP7" << endl;
	
	for (int i = 0; i < (int)myPts.size(); i++) {
		outfile << "K," << i + 1 << "," << myPts[i].x << "," << myPts[i].y << "," << myPts[i].z << endl;
	}

	for (int i = 0; i < (int)myEdges.size(); i++) {
		outfile << "L," << myEdges[i].left + 1 << "," << myEdges[i].right + 1 << endl;
	}
	
	outfile.close();
}



void Delaunay::deleteFrame() {
	//删除外边框  
	EdgeArray boundEdges;
	for (int i = 0; i < 4; i++) myPts.erase(myPts.begin());
	for (int i = 0; i < (int)myTris.size(); i++) {
		if (myTris[i].v[0] == 0 || myTris[i].v[0] == 1 || myTris[i].v[0] == 2 || myTris[i].v[0] == 3) {
			delTriangle(i, boundEdges);
			boundEdges.resize(0);
			i--;
		} else {
			for (int j = 0; j < 3; j++) {
				myTris[i].v[j] -= 4;
				myTris[i].s[j].left -= 4;
				myTris[i].s[j].right -= 4;
			}
		}
	}
	
	for (int i = 0; i < 4; i++) myEdges.erase(myEdges.begin());
	for (int i = 0; i < (int)myEdges.size(); i++) {
		myEdges[i].left -= 4;
		myEdges[i].right -= 4;
	}
}

void Delaunay::boundaryRecover(int fromPoint, int toPoint) {
	//恢复由指定点组成的边界
	EdgeArray boundEdges;

	for (int i = 0; i < (int)myTris.size(); i++) {
		if (myTris[i].v[0] >= (fromPoint - 1) && myTris[i].v[2] <= (toPoint - 1)) {
			delTriangle(i, boundEdges);
			boundEdges.resize(0);
			i--;
		}
	}
}

