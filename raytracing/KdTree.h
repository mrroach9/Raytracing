#ifndef _KDTREE_H_
#define _KDTREE_H_

#include "Geometry.h"
#include "Mesh.h"
#include <vector>
#include <iostream>

using namespace std;

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

private:
	unsigned char*		face_count;
public:
	KdTree();
	KdTree(CMesh* mesh);
	~KdTree();
	
public:
	void constructFromMesh();
	void insert(UINT data, KdTreeNode* node);
	void searchVertexInBox(Vector3D lower, Vector3D upper, vector<UINT> &vid_vec);
	void searchFaceInBox(Vector3D lower, Vector3D upper, vector<UINT> &fid_vec);
	bool hasPoints(Vector3D lower, Vector3D upper, KdTreeNode* node);

	void searchLine(Vector3D origin, Vector3D dir, double s, double t, vector<UINT> &fid_vec);

private:
	void search(Vector3D lower, Vector3D upper, KdTreeNode* node, vector<UINT> &fid_vec);
	void searchLine_int(Vector3D v1, Vector3D v2, vector<UINT> &fid_vec);
	void getFaceList(vector<UINT> &vid_vec, vector<UINT> &fid_vec);
};

#endif //_KDTREE_H_