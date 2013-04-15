#ifndef _KDTREE_H_
#define _KDTREE_H_

#include "Geometry.h"
#include "Mesh.h"
#include <vector>
#include <iostream>
#include <map>
#include <time.h>
#include <omp.h>

using namespace std;

#define MAX_THREAD	16

//static int searchTime = 0;
//static int faceTime = 0;
//static int callTime = 0;

class KdTreeNode{
public:
	KdTreeNode* left;
	KdTreeNode* right;
	KdTreeNode* parent;
	UINT		depth;
	Vector3D	lowerBound;
	Vector3D	upperBound;
	UINT		vid;
public:
	KdTreeNode();
	~KdTreeNode();
	KdTreeNode(KdTreeNode* parent, UINT depth, Vector3D lb,
			   Vector3D ub);
	bool enclose(Vector3D point);
};

class KdTree{
public:
	KdTreeNode*	root;
	CMesh*		mesh;
	double		radius;

public:
	KdTree();
	KdTree(CMesh* mesh);
	~KdTree();
private:
	unsigned char*		face_count[MAX_THREAD];
public:
	void constructFromMesh();
	void insert(UINT data, KdTreeNode* node);
	void searchVertexInBox(Vector3D lower, Vector3D upper, vector<UINT> &vid_vec);
	void searchFaceInBox(Vector3D lower, Vector3D upper, vector<UINT> &fid_vec);
	bool hasPoints(Vector3D lower, Vector3D upper, KdTreeNode* node);

	void searchLine(Vector3D origin, Vector3D dir, double s, double t, vector<UINT> &fid_vec);

private:
	void search(Vector3D lower, Vector3D upper, KdTreeNode* node, vector<UINT> &fid_vec);
	void searchLine_int(Vector3D v1, Vector3D v2, vector<UINT> &vid_vec);
	inline void searchLine_fast_int(Vector3D o, Vector3D d, double s, double t, KdTreeNode* node, vector<UINT> &vid_vec);
	void getFaceList(vector<UINT> &vid_vec, vector<UINT> &fid_vec);
};

#endif //_KDTREE_H_