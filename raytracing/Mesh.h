///////////////////////class CMesh////////////////////////////////////
//		���Դ����������ˣ��������εĶ���������2-manifode mesh)	//
//		���Զ�ȡһ���obj,smf,wrl���ļ�����							//
//		���ݵ�������������������ӹ�ϵ							//
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
	Vector3D	m_vPosition;	//�������
	UINT*		m_piEdge;		//�Ӹõ㷢����halfedge,Ҫ���ݵ�Ķ�����̬����
	_UINTLIST	m_lEdgeList;	//��������m_piEdge����ʱ����
	short		m_nValence;		//��Ķ���
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
	UINT	m_iVertex[2];//�ߵ����˵㣬Vertex0��>Vertex1

	UINT	m_iTwinEdge;//��ñ߷����෴����һ���ߣ����Ϊ-1��ñ�Ϊ�߽�
	UINT	m_iNextEdge;//����ʱ�뷽�����һ����
	UINT	m_iFace;//�ñ��������棬Ӧ�����������

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
	short	m_nType;		//������
	UINT*	m_piVertex;		//���е�
	UINT*	m_piEdge;		//���б�
	Vector3D m_vNormal;		//Normal Direction
	Vector3D m_vMassPoint;	//Gravity Center
	double	m_dArea;		//���

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
	UINT		m_nVertex;				//����
	CVertex*	m_pVertex;				//���
	UINT		m_nEdge;				//����
	CEdge*		m_pEdge; 				//�߱�
	UINT		m_nFace;	 			//����
	CFace*		m_pFace;				//���

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
	//�������
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