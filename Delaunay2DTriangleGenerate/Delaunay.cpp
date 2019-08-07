
#include "Delaunay.h"

Delaunay::Delaunay(Point p1, Point p2, Point p3, Point p4) {

	myPts.resize(4);
	myPts[0] = p1;
	myPts[1] = p2;
	myPts[2] = p3;
	myPts[3] = p4; // ����ĸ���߿��

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

void Delaunay::readPointFromTxt(string inTxt) {
	openInputFile(inTxt);
	
	string line; // record each read line
	line = readLineFromInput();
	int nodeSize = parseStringToInteger(line);

	for (int i = 0; i < nodeSize; i++) {
		
		line = readLineFromInput();
		ptsList.push_back(parseLineToPointWithComma(line));
		/*readPoint(xx, yy, zz, line);
		myMesh.addPoint(xx, yy, zz);*/
	}
	
	closeInputFile();
}

Triangle* Delaunay::generateSuperTriangle() {
	// define x is in [-100, 100]
	// define y is in [-100, 100]
	
	double xmin = ptsList[0].x;
	double xmax = ptsList[0].x;
	double ymin = ptsList[0].y;
	double ymax = ptsList[0].y;

	for (vector <Point>::iterator it = ptsList.begin(); it != ptsList.end(); it++) {
		if (it->x < xmin) xmin = it->x;
		if (it->x > xmax) xmax = it->x;
		if (it->y < ymin) ymin = it->y;
		if (it->y > ymax) ymax = it->y;
	}

	double dx = xmax - xmin;
	double dy = ymax - ymin;
	
	double dmax = (dx > dy) ? dx : dy;
	double xmid = xmin + dx * 0.5;
	double ymid = ymin + dy * 0.5;
	
	Point p1 = { xmid - 20 * dmax, ymid - dmax };
	Point p2 = { xmid, ymid + 20 * dmax };
	Point p3 = { xmid + 20 * dmax, ymid - dmax };

	ptsList[0] = p1;
	ptsList[1] = p2;
	ptsList[2] = p3;

	Edge l1 = { 0, 1, -1 };
	Edge l2 = { 1, 2, -1 };
	Edge l3 = { 0, 2, -1 };

	edgsList[0] = l1;
	edgsList[1] = l2;
	edgsList[2] = l3;

	makeTriangle(0, 1, 2);
	makeTriangle(0, 2, 3);
	return[
		[],
			[],
			[]
	];
}

int Delaunay::inCircle(Point p, Triangle tris) {
	// judge if the point is in the circumcircle of the triangle
	// 0: point is on the circle
	// 1: point is in the circle
	// -1: point is out of the circle
	double dis = sqrt((tris.cir->x - p.x)*(tris.cir->x - p.x) + (tris.cir->y - p.y)*(tris.cir->y - p.y));
	if (fabs(dis - tris.cir->radius) < 1.0e-6) return 0;
	else if (dis - tris.cir->radius >= 1.0e-6) return -1;
	else return 1;
}

void Delaunay::makeTriangle(int n1, int n2, int n3) {
	/**
	 ����ָ�����������������Σ�
		1. ���������ε����ԲԲ����뾶
		2. ���������Σ������myTris�б�
		3. ����myEdges�б�
	 */
	double x_centre, y_centre, radius;
	calcCentre(x_centre, y_centre, radius, n1, n2, n3); // ��ö���Ϊn1,n2,n3�������ε����ԲԲ������Ͱ뾶  

	Triangle newTriangle = { 
		{n1,n2,n3}, 
		{{n1,n2,1}, {n2,n3,1} ,{n1,n3,1}},
		x_centre, y_centre, radius 
	}; // ����ָ����������  
		
	myTris.push_back(newTriangle); // ��myTris������¹����������  
	
	int edgeSize = (int)myEdges.size(); // ���Ŀǰ�ı���  
	int flag;
	for (int i = 0; i < 3; i++)	{
		flag = 1;
		for (int j = 0; j < edgeSize; j++) {
			// ͨ��ѭ���ж��¹���������εĸ����Ƿ��Ѿ�������myEdges��
			// ���������ֻ���Ӹñߵļ�������������±�  
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



bool Delaunay::addPoint(Point p) {

}

bool Delaunay::addPoint(double xx, double yy, double zz) {
	
	EdgeArray boundEdges; // boundEdges���ڴ洢��ɾ�������κ����µı߿����ڹ����µ�������  
	Point newPoint = { xx,yy,zz };
	
	myPts.push_back(newPoint); // ��myPts������µ�  
	intArray badTriangle; // badTriangle���ڴ洢�����Ͽ�Բ����������ε�������  
	
	int TriSize = (int)myTris.size(); // ���Ŀǰ����������  
	for (int i = 0; i < TriSize; i++) {
		// ͨ��ѭ���ҵ����в����Ͽ�Բ����������Σ������������Ŵ���badTriangle��  
		if (inCircle(xx, yy, myTris[i])) badTriangle.push_back(i);
	}
	
	for (int i = 0; i < (int)badTriangle.size(); i++) {
		// ͨ��ѭ��ɾ�����в����Ͽ�Բ����������Σ�ͬʱ�����߿�  
		delTriangle(badTriangle[i], boundEdges);
		for (int j = i + 1; j < (int)badTriangle.size(); j++) badTriangle[j] -= 1;
	}

	int ptSize = (int)myPts.size();//���Ŀǰ�ĵ���  
	for (int i = 0; i < (int)boundEdges.size(); i++) {
		//�����µ������� 
		if (ptSize - 1 < boundEdges[i].left) makeTriangle(ptSize - 1, boundEdges[i].left, boundEdges[i].right);
		else if (ptSize - 1 > boundEdges[i].left && ptSize - 1 < boundEdges[i].right) makeTriangle(boundEdges[i].left, ptSize - 1, boundEdges[i].right);
		else makeTriangle(boundEdges[i].left, boundEdges[i].right, ptSize - 1);		
	}
	return true;
}


void Delaunay::delTriangle(int n, EdgeArray &boundEdges) {
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < (int)myEdges.size(); j++) {
			if (myEdges[j].left == myTris[n].s[i].left && myEdges[j].right == myTris[n].s[i].right) {
				if (myEdges[j].count == 2) {
					//��Ҫɾ�������ε�һ�ߵļ���Ϊ2�����������1��������ѹ��BoundEdges������
					myEdges[j].count = 1;
					boundEdges.push_back(myEdges[j]);
				} else if (myEdges[j].count == -1) boundEdges.push_back(myEdges[j]);//�������߿���ֱ��ѹ��BoundEdges������  
				else if (myEdges[j].count == 1) {
					//���ɾ�������ε�һ�ߵļ���Ϊ1����ɾ���ñߣ�ͬʱ�鿴BoundEdges���Ƿ��д˱ߣ����У���ɾ��
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
	myTris.erase(myTris.begin() + n);//ɾ����������  
}

void Delaunay::output() {
	//��output.log"�ļ���д��ANSYS������ 
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
	//ɾ����߿�  
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
	//�ָ���ָ������ɵı߽�
	EdgeArray boundEdges;

	for (int i = 0; i < (int)myTris.size(); i++) {
		if (myTris[i].v[0] >= (fromPoint - 1) && myTris[i].v[2] <= (toPoint - 1)) {
			delTriangle(i, boundEdges);
			boundEdges.resize(0);
			i--;
		}
	}
}



