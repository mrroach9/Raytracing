#include "Mesh.h"

#define ITERATE_TIMES 1
#define	ALPHA_STRETCH 1.0

CVertex::~CVertex()
{
	if(m_piEdge!=NULL)
		delete[] m_piEdge;
	m_lEdgeList.clear();
}

CVertex& CVertex::operator = (CVertex& v)
{
	m_vPosition = v.m_vPosition;
	m_nValence = v.m_nValence;	
	m_piEdge = new UINT[m_nValence];
	for(short i=0;i<m_nValence;i++)
		m_piEdge[i] = v.m_piEdge[i];
	
	m_bIsBoundary = v.m_bIsBoundary;
	m_vNormal = v.m_vNormal;

	return *this;
}


CEdge::~CEdge()
{
}

CEdge& CEdge::operator = (const CEdge& e)
{
	m_iVertex[0] = e.m_iVertex[0];
	m_iVertex[1] = e.m_iVertex[1];
	m_iTwinEdge = e.m_iTwinEdge;
	m_iNextEdge = e.m_iNextEdge;
	m_iFace = e.m_iFace;

	return *this;
}

CFace::CFace(short s)
{
	m_nType=s;
	m_vNormal=Vector3D(0.0,0.0,1.0);
	m_dArea=0.0;
	m_piEdge=new UINT[s];
	m_piVertex=new UINT[s];
}

CFace::~CFace()
{
	if(m_piEdge!=NULL)
		delete[] m_piEdge;
	if(m_piVertex!=NULL)
		delete[] m_piVertex;
}

void CFace::Create(short s)
{
	m_nType=s;
	m_vNormal=Vector3D(0.0,0.0,1.0);
	m_dArea=0.0;
	m_piEdge=new UINT[s];
	m_piVertex=new UINT[s];
}

CFace& CFace::operator =(const CFace& f)
{
	m_nType = f.m_nType;
	m_piVertex = new UINT[m_nType];
	m_piEdge = new UINT[m_nType];
	for(short i=0;i<m_nType;i++)
	{
		m_piVertex[i] = f.m_piVertex[i];
		m_piEdge[i] = f.m_piEdge[i];
	}
	
	m_vNormal = f.m_vNormal;
	m_vMassPoint = f.m_vMassPoint;
	m_dArea = f.m_dArea;
	
	return (*this);
}


void CMesh::clear()
{
	if(m_pVertex!=NULL)	{delete[] m_pVertex;m_pVertex=NULL;}
	if(m_pEdge!=NULL)	{delete[] m_pEdge;m_pEdge=NULL;}
	if(m_pFace!=NULL)	{delete[] m_pFace;m_pFace=NULL;}
	m_nVertex=m_nEdge=m_nFace=0;
	m_lFocusEdge.clear();
	m_lFocusVertex.clear();
}

CMesh::CMesh(CMesh* pMesh)
{
	m_nVertex = pMesh->m_nVertex;	
	m_pVertex = new CVertex[m_nVertex];
	UINT i;
	for(i=0;i<m_nVertex;i++)
		m_pVertex[i] = pMesh->m_pVertex[i];

	m_nEdge = pMesh->m_nEdge;		
	m_pEdge = new CEdge[m_nEdge]; 	
	for(i=0;i<m_nEdge;i++)
		m_pEdge[i] = pMesh->m_pEdge[i];
	m_nFace = pMesh->m_nFace;	 
	m_pFace = new CFace[m_nFace];	
	for(i=0;i<m_nFace;i++)
		m_pFace[i] = pMesh->m_pFace[i];

	m_lFocusEdge = pMesh->m_lFocusEdge;
}

CMesh::~CMesh()
{
	clear();	
}

bool CMesh::load(string sFilename)
{
	clear();
	string sExt = sFilename.substr(sFilename.length() - 4, 4);
	if (sExt != ".smf" && sExt != ".obj" && sExt != ".dat") {
		return false;
	}

	ifstream fin(sFilename.c_str());

	_VECTORLIST VertexList;
	_UINTLIST FaceList;
	UINT l[3];
	Vector3D vector;
	Vector3D vMin(1e9,1e9,1e9);
	Vector3D vMax(-1e9,-1e9,-1e9);


	string strLine; 
	while (getline(fin, strLine)) {
		if (strLine.length() == 0) continue;
		if (strLine[0] == 'v') { 
			istringstream sin(strLine);
			char tmp;
			sin >> tmp >> vector.x >> vector.y >> vector.z;
			VertexList.push_back(Vector3D(vector));
		
			if(vector.x<vMin.x){
				vMin.x=vector.x;
			}else if(vector.x>vMax.x){
				vMax.x=vector.x;
			}
			if(vector.y<vMin.y){
				vMin.y=vector.y;
			}else if(vector.y>vMax.y){
				vMax.y=vector.y;
			}
			if(vector.z<vMin.z){
				vMin.z=vector.z;
			}else if(vector.z>vMax.z){
				vMax.z=vector.z;
			}
		} else if (strLine[0] == 'f') {
			istringstream sin(strLine);
			char tmp;
			sin >> tmp >> l[0] >> l[1] >> l[2];
			for(int j = 0; j < 3; j++){
				FaceList.push_back(l[j]-1);
			}
		}
	}
	fin.close();
	vector = vMax - vMin;
	double d = max(vector.x, vector.y);
	d = 0.5 * max(d, vector.z);
	vector=0.5 * (vMax + vMin);

	m_nVertex=(UINT) VertexList.size();
	m_nFace=(UINT) FaceList.size()/3;
	m_nEdge=(UINT) 3*m_nFace;

	//read vertices and faces
	m_pVertex = new CVertex[m_nVertex];
	if (m_pVertex==NULL) {
		clear(); 
		return false;
	}//out of memory
	m_pFace = new CFace[m_nFace];
	if (m_pFace == NULL) {
		clear(); 
		return false;
	}//out of memory

	_VECTORLIST::iterator iVertex = VertexList.begin();
	_UINTLIST::iterator iFace = FaceList.begin();

	for(int i = 0; i < (int)m_nVertex; i++){
		m_pVertex[i].m_vPosition = (*(iVertex++)-vector)/d;
	}
	for(int i = 0; i < (int)m_nFace; i++)
	{
		m_pFace[i].Create(3);
		for(int j = 0; j < 3; j++)
			m_pFace[i].m_piVertex[j]=*iFace++;
	}

	VertexList.clear();
	FaceList.clear();
	return construct();
}

bool CMesh::reconstruct()
{
	_VECTORLIST VertexList;
	_UINTLIST FaceList;

	Vector3D vector;
	Vector3D vMin;
	Vector3D vMax;
	bool bFirst = true;

	UINT* old2new = new UINT[m_nVertex];
	UINT iNewIndex=0;
	for(UINT i = 0; i < m_nVertex; i++)
	{
		VertexList.push_back(m_pVertex[i].m_vPosition);
		old2new[i]=iNewIndex++;
	}

	for(UINT i = 0; i < m_nFace; i++) {
		for(short j=0;j<3;j++) {
			FaceList.push_back(old2new[m_pFace[i].m_piVertex[j]]);
		}
	}

	clear();

	m_nVertex=(UINT) VertexList.size();
	m_nFace=(UINT) FaceList.size()/3;
	m_nEdge=(UINT) 3*m_nFace;
	//read vertices and faces
	m_pVertex = new CVertex[m_nVertex];
	if (m_pVertex == NULL) {
		clear(); 
		return false;
	}//out of memory
	m_pFace = new CFace[m_nFace];
	if (m_pFace == NULL) {
		clear(); 
		return false;
	}//out of memory

	_VECTORLIST::iterator iVertex = VertexList.begin();
	_UINTLIST::iterator iFace = FaceList.begin();

	for(UINT i = 0; i < m_nVertex; i++) {
		m_pVertex[i].m_vPosition=*iVertex++;
	}
	for(UINT i = 0; i < m_nFace; i++) 
	{
		m_pFace[i].Create(3);
		for(short j=0;j<3;j++)
			m_pFace[i].m_piVertex[j]=*iFace++;
	}

	delete[] old2new;
	VertexList.clear();
	FaceList.clear();
	return construct();
}

bool CMesh::construct()
{
	if (m_pVertex == NULL || m_pFace==NULL) {
		return false;//empty
	}

	if(m_pEdge!=NULL){
		delete[] m_pEdge;
		m_pEdge=NULL;
	}//delete old edgelist

	m_bClosed = true;

	if(m_nEdge == 0) {
		for(UINT i = 0; i < m_nFace; i++) {
			m_nEdge+=m_pFace[i].m_nType;
		}
	}

	m_pEdge = new CEdge[m_nEdge];
	
	short j,nType;

	UINT iEdge=0;
	UINT iVertex;

	for(UINT i = 0; i < m_nFace; i++) {
		calcFaceNormal(i);

		nType=m_pFace[i].m_nType;
		for(j=0;j<nType;j++)
		{
			m_pFace[i].m_piEdge[j]=iEdge;
			iVertex=m_pFace[i].m_piVertex[j];
			m_pVertex[iVertex].m_nValence++;
			m_pVertex[iVertex].m_lEdgeList.push_back(iEdge);
			m_pEdge[iEdge].m_iFace=i;
			m_pEdge[iEdge].m_iVertex[0]=iVertex;
			m_pEdge[iEdge].m_iVertex[1]=m_pFace[i].m_piVertex[(j+1)%nType];
			m_pEdge[iEdge].m_iNextEdge=iEdge+1;
			iEdge++;

		}
		m_pEdge[iEdge-1].m_iNextEdge-=nType;

	}

	_UINTLIST::iterator iv;
	UINT iSrcVertex,iDesVertex;
	bool bFlag;
	for (UINT i = 0; i < m_nEdge; i++) {
		if(m_pEdge[i].m_iTwinEdge!=-1)
			continue;
		iSrcVertex=m_pEdge[i].m_iVertex[0];
		iDesVertex=m_pEdge[i].m_iVertex[1];
		bFlag=true;
		for (iv = m_pVertex[iDesVertex].m_lEdgeList.begin(); iv != m_pVertex[iDesVertex].m_lEdgeList.end(); iv++)
		{
			iEdge=*iv;
			if(m_pEdge[iEdge].m_iVertex[1]==iSrcVertex)
			{
				m_pEdge[i].m_iTwinEdge=iEdge;
				m_pEdge[iEdge].m_iTwinEdge=i;
				bFlag=false;
				break;
			}
		}
		if(bFlag)
		{
			m_pVertex[iSrcVertex].m_bIsBoundary = true;
			m_bClosed = false;
		}

	}
	short nValence;
	UINT iTwinEdge;
	

	for(UINT i = 0; i < m_nVertex; i++)
	{
		nValence=m_pVertex[i].m_nValence;
		if(nValence<2)
			m_pVertex[i].m_bIsBoundary=true;

		if(nValence==0)
			continue;

		if(m_pVertex[i].m_piEdge!=NULL)
			delete[] m_pVertex[i].m_piEdge;
		m_pVertex[i].m_piEdge=new UINT[nValence];

		if(m_pVertex[i].m_bIsBoundary)
		{
			for (iv = m_pVertex[i].m_lEdgeList.begin(); iv != m_pVertex[i].m_lEdgeList.end(); iv++)
			{
				bFlag=true;
				m_pVertex[i].m_piEdge[0]=*iv;
				for(j=1;j<nValence;j++)
				{
					iTwinEdge=m_pEdge[m_pVertex[i].m_piEdge[j-1]].m_iTwinEdge;
					if(iTwinEdge==-1)
					{
						bFlag=false;
						break;
					}
					m_pVertex[i].m_piEdge[j]=m_pEdge[iTwinEdge].m_iNextEdge;
				}
				if(bFlag)//success
					break;
			}
		
		}
		else
		{
			iv = m_pVertex[i].m_lEdgeList.begin();
			m_pVertex[i].m_piEdge[0]=*iv;
			for(j=1;j<nValence;j++)
			{
				iTwinEdge=m_pEdge[m_pVertex[i].m_piEdge[j-1]].m_iTwinEdge;
				m_pVertex[i].m_piEdge[j]=m_pEdge[iTwinEdge].m_iNextEdge;
			}
		}
		m_pVertex[i].m_lEdgeList.clear();
		for(j=0;j<nValence;j++)
			m_pVertex[i].m_lEdgeList.push_back(m_pVertex[i].m_piEdge[j]);

		calcVertexNormal(i);
	}

	return true;
}


void CMesh::calcFaceNormal(UINT i)
{
	Vector3D v[2];

	//get the vector
	v[0] = m_pVertex[m_pFace[i].m_piVertex[2]].m_vPosition-m_pVertex[m_pFace[i].m_piVertex[0]].m_vPosition;

	if(m_pFace[i].m_nType==3) {
		v[1] = m_pVertex[m_pFace[i].m_piVertex[2]].m_vPosition-m_pVertex[m_pFace[i].m_piVertex[1]].m_vPosition;
	} else {
		v[1] = m_pVertex[m_pFace[i].m_piVertex[3]].m_vPosition-m_pVertex[m_pFace[i].m_piVertex[1]].m_vPosition;
	}
	m_pFace[i].m_vNormal=v[0]^v[1];

	if(m_pFace[i].m_nType == 3) {
		m_pFace[i].m_dArea = 0.5*m_pFace[i].m_vNormal.normalize();
	} else if(m_pFace[i].m_nType == 4) {
		m_pFace[i].m_dArea = m_pFace[i].m_vNormal.normalize();
	} else {
		m_pFace[i].m_vNormal.normalize();
	}

	Vector3D vMassPoint;

	for(short j = 0; j < m_pFace[i].m_nType; j++){
		vMassPoint += m_pVertex[m_pFace[i].m_piVertex[j]].m_vPosition;
	}
	vMassPoint /= (double)(m_pFace[i].m_nType);
	m_pFace[i].m_vMassPoint = vMassPoint;
}

void CMesh::calcVertexNormal(UINT i) {
	Vector3D v;
	UINT iFace;
	short valence=m_pVertex[i].m_nValence;
	if(valence < 1)
		return;
	for(short j=0;j<valence;j++)
	{
		iFace=m_pEdge[m_pVertex[i].m_piEdge[j]].m_iFace;
		v+=m_pFace[iFace].m_vNormal;
	}
	v/=(double)valence;
	m_pVertex[i].m_vNormal=v;

}
