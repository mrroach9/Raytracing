///////////////////////class CMesh////////////////////////////////////
//		可以处理任意拓扑，任意多边形的二流型网格（2-manifode mesh)	//
//		可以读取一般的obj,smf,wrl等文件类型							//
//		根据点表和面表生成完整的连接关系							//
//																	//
//////////////////////////////////////////////////////////////////////
#ifndef _MESH_H_
#define _MESH_H_

#include "Geometry.h"
#include <list>
#include <fstream>
#include <string>
#include <sstream>

using namespace std;
typedef list<Vector3D>	_VECTORLIST;
typedef list<UINT>		_UINTLIST;

#define MAX_VERTEX_PER_FACE 20

class CVertex
{
public:
	Vector3D	m_vPosition;	//点的坐标
	UINT*		m_piEdge;		//从该点发出的halfedge,要根据点的度数动态创建
	_UINTLIST	m_lEdgeList;	//用来构造m_piEdge的临时链表
	short		m_nValence;		//点的度数
	Vector3D	m_vNormal;
	bool		m_bIsBoundary;

public:
	//constructions
	CVertex() { 
		m_piEdge=NULL; 
		m_nValence=0; 
		m_bIsBoundary = false; 
	}
	CVertex(double x,double y,double z) {
		m_vPosition = Vector3D(x,y,z);
		m_piEdge=NULL; 
		m_nValence=0; 
		m_bIsBoundary = false; 
	}
	CVertex(Vector3D v) {
		m_vPosition=v;
		m_piEdge=NULL;
		m_nValence=0; 
		m_bIsBoundary = false; 
	}
	virtual ~CVertex();

	//operations
	CVertex& operator = (CVertex& v);

};

class CEdge
{
public:
	UINT	m_iVertex[2];//边的两端点，Vertex0－>Vertex1

	UINT	m_iTwinEdge;//与该边方向相反的另一条边，如果为-1则该边为边界
	UINT	m_iNextEdge;//沿逆时针方向的下一条边
	UINT	m_iFace;//该边所属的面，应该在它的左边

public:
	//constructions
	CEdge() {
		m_iVertex[0] = m_iVertex[1] = -1;
		m_iTwinEdge = m_iNextEdge = m_iFace = -1; 
	}
	CEdge(UINT iV0, UINT iV1) { 
		m_iVertex[0] = iV0; 
		m_iVertex[1] = iV1;
		m_iTwinEdge = m_iNextEdge = m_iFace = -1;
	}
	virtual ~CEdge();

	//operations
	CEdge& operator = (const CEdge& e);
};

class CFace
{
public:
	short	m_nType;		//几边形
	UINT*	m_piVertex;		//所有点
	UINT*	m_piEdge;		//所有边
	Vector3D m_vNormal;		//Normal Direction
	Vector3D m_vMassPoint;	//Gravity Center
	double	m_dArea;		//面积

public:
	//constructions
	CFace() {
		m_nType=0;
		m_piVertex=m_piEdge=NULL;
		m_vNormal=Vector3D(0.0,0.0,1.0);
		m_dArea=0.0;
	}
	CFace(short s);
	virtual ~CFace();

	//operations
	void Create(short s);
	CFace& operator = (const CFace& f);
};

class CMesh
{
public:
	UINT		m_nVertex;				//点数
	CVertex*	m_pVertex;				//点表
	UINT		m_nEdge;				//边数
	CEdge*		m_pEdge; 				//边表
	UINT		m_nFace;	 			//面数
	CFace*		m_pFace;				//面表

	//temp
	_UINTLIST m_lFocusEdge;
	_UINTLIST m_lFocusVertex;
	_UINTLIST m_lFocusFace;

	bool	m_bClosed;
public:
	//constructions
	CMesh() {
		m_nVertex = m_nEdge = m_nFace = 0;
		m_pVertex = NULL;
		m_pEdge = NULL;
		m_pFace = NULL;
	}
	CMesh(CMesh* pMesh);
	virtual ~CMesh();

	//operations
	//输入输出
public:
	bool	load(string sFilename);		// load from file

	bool	construct();				// construct connectivity

public:
	void	clear();
	bool	reconstruct();				// construct connectivity from current mesh
	void	calcFaceNormal(UINT i);
	void	calcVertexNormal(UINT i);
};

#endif