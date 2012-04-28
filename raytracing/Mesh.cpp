#include "Mesh.h"

#define ITERATE_TIMES 1
#define	ALPHA_STRETCH 1.0

//////////////////////////////////////////////////////
//						CVertex						//
//////////////////////////////////////////////////////
CVertex::~CVertex()
{
	if(m_piEdge!=NULL)
		delete[] m_piEdge;
	m_lEdgeList.clear();
}

CVertex& CVertex::operator = (CVertex& v)
{
	m_vPosition = v.m_vPosition;		//点的坐标
	m_nValence = v.m_nValence;			//点的度数
	m_piEdge = new UINT[m_nValence];	//从该点发出的halfedge,要根据点的度数动态创建
	for(short i=0;i<m_nValence;i++)
		m_piEdge[i] = v.m_piEdge[i];
	
	m_bIsBoundary = v.m_bIsBoundary;
	m_vNormal = v.m_vNormal;

	return *this;
}


//////////////////////////////////////////////////////
//						CEdge						//
//////////////////////////////////////////////////////

CEdge::~CEdge()
{
}

CEdge& CEdge::operator = (const CEdge& e)
{
	m_iVertex[0] = e.m_iVertex[0];	//边的两端点，Vertex0－>Vertex1
	m_iVertex[1] = e.m_iVertex[1];
	m_iTwinEdge = e.m_iTwinEdge;	//与该边方向相反的另一条边，如果为-1则该边为边界
	m_iNextEdge = e.m_iNextEdge;	//沿逆时针方向的下一条边
	m_iFace = e.m_iFace;			//该边所属的面，应该在它的左边

	return *this;
}

//////////////////////////////////////////////////////
//						CFace						//
//////////////////////////////////////////////////////
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

//////////////////////////////////////////////////////
//						CMesh						//
//////////////////////////////////////////////////////
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
	m_nVertex = pMesh->m_nVertex;				//点数
	m_pVertex = new CVertex[m_nVertex];			//点表
	UINT i;
	for(i=0;i<m_nVertex;i++)
		m_pVertex[i] = pMesh->m_pVertex[i];

	m_nEdge = pMesh->m_nEdge;					//边数
	m_pEdge = new CEdge[m_nEdge]; 				//边表
	for(i=0;i<m_nEdge;i++)
		m_pEdge[i] = pMesh->m_pEdge[i];
	m_nFace = pMesh->m_nFace;	 				//面数
	m_pFace = new CFace[m_nFace];				//面表
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

	_VECTORLIST VertexList;//临时存储所有点的链表
	_UINTLIST FaceList;//临时存储所有面包含的点的信息
	UINT l[3];
	Vector3D vector;
	Vector3D vMin(1e9,1e9,1e9);
	Vector3D vMax(-1e9,-1e9,-1e9);


	string strLine; 
	while (getline(fin, strLine)) {//将点和面的信息临时存在VertexList和FaceList中
		if (strLine.length() == 0) continue;
		if (strLine[0] == 'v') { //A line starting with a 'v' designating a vertex position information
			istringstream sin(strLine);
			char tmp;
			sin >> tmp >> vector.x >> vector.y >> vector.z;
			VertexList.push_back(Vector3D(vector));
			//计算所有点的包围盒
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
	//根据包围盒将物体中心移至原点，同时进行归一化
	vector=vMax-vMin;
	double d=__max(vector.x,vector.y);
	d=0.5*__max(d,vector.z);
	vector=0.5*(vMax+vMin);//偏移量

	m_nVertex=(UINT) VertexList.size();
	m_nFace=(UINT) FaceList.size()/3;
	m_nEdge=(UINT) 3*m_nFace;

	//read vertices and faces
	m_pVertex = new CVertex[m_nVertex];
	if (m_pVertex==NULL) {clear(); return false;}//out of memory
	m_pFace = new CFace[m_nFace];
	if (m_pFace == NULL) {clear(); return false;}//out of memory

	_VECTORLIST::iterator iVertex = VertexList.begin();
	_UINTLIST::iterator iFace = FaceList.begin();

	for(int i = 0; i < (int)m_nVertex; i++){
		m_pVertex[i].m_vPosition=(*(iVertex++)-vector)/d;
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
	_VECTORLIST VertexList;//临时存储所有点的链表
	_UINTLIST FaceList;//临时存储所有面包含的点的信息

	Vector3D vector;
	Vector3D vMin;
	Vector3D vMax;
	bool bFirst=true;

	UINT i;
	UINT* old2new = new UINT[m_nVertex];
	UINT iNewIndex=0;
	for(i=0;i<m_nVertex;i++)
	{
		VertexList.push_back(m_pVertex[i].m_vPosition);
		old2new[i]=iNewIndex++;
	}

	for(i=0;i<m_nFace;i++)
		for(short j=0;j<3;j++)
			FaceList.push_back(old2new[m_pFace[i].m_piVertex[j]]);

	clear();

	m_nVertex=(UINT) VertexList.size();
	m_nFace=(UINT) FaceList.size()/3;
	m_nEdge=(UINT) 3*m_nFace;
	//read vertices and faces
	m_pVertex = new CVertex[m_nVertex];
	if (m_pVertex==NULL) {clear(); return false;}//out of memory
	m_pFace = new CFace[m_nFace];
	if (m_pFace == NULL) {clear(); return false;}//out of memory

	_VECTORLIST::iterator iVertex = VertexList.begin();
	_UINTLIST::iterator iFace = FaceList.begin();

	for(i=0;i<m_nVertex;i++)
		m_pVertex[i].m_vPosition=*iVertex++;
	for(i=0;i<m_nFace;i++)
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
	if ((m_pVertex==NULL)||(m_pFace==NULL)) return false;//empty

	if(m_pEdge!=NULL){delete[] m_pEdge;m_pEdge=NULL;}//delete old edgelist

	m_bClosed = true;

	UINT i;
	if(m_nEdge==0)		//在load过程中应该已经计算过边数，如果仍未计算则重新扫描所有面，得到总边数
		for(i=0;i<m_nFace;i++)
			m_nEdge+=m_pFace[i].m_nType;

	m_pEdge = new CEdge[m_nEdge];
	
	short j,nType;

	UINT iEdge=0;
	UINT iVertex;

	for(i=0;i<m_nFace;i++)//扫描所有面
	{
		calcFaceNormal(i);//顺便计算该面的法向和面积

		nType=m_pFace[i].m_nType;
		for(j=0;j<nType;j++)//对每个面建立相关的点、边、面的连接信息
		{
			//面的连接信息
			m_pFace[i].m_piEdge[j]=iEdge;//按顺序给每个边分配编号
			//点的连接信息
			iVertex=m_pFace[i].m_piVertex[j];
			m_pVertex[iVertex].m_nValence++;//度数加1
			m_pVertex[iVertex].m_lEdgeList.push_back(iEdge);//先将该边加入点的临时边表
			//边的连接信息
			m_pEdge[iEdge].m_iFace=i;
			m_pEdge[iEdge].m_iVertex[0]=iVertex;//源点
			m_pEdge[iEdge].m_iVertex[1]=m_pFace[i].m_piVertex[(j+1)%nType];//目标点
			m_pEdge[iEdge].m_iNextEdge=iEdge+1;
			iEdge++;

		}
		m_pEdge[iEdge-1].m_iNextEdge-=nType;

	}

	_UINTLIST::iterator iv;
	UINT iSrcVertex,iDesVertex;
	bool bFlag;
	for(i=0;i<m_nEdge;i++)//扫描所有边，生成所有边的m_iTwinEdge的连接关系
	{
		if(m_pEdge[i].m_iTwinEdge!=-1)//已经计算过
			continue;
		iSrcVertex=m_pEdge[i].m_iVertex[0];//源点
		iDesVertex=m_pEdge[i].m_iVertex[1];//目标点
		//遍历从目标点发出的所有边，看其中有没有回到源点的边
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
		//如果没有，说明源点是边界点
		if(bFlag)
		{
			m_pVertex[iSrcVertex].m_bIsBoundary = true;
			m_bClosed = false;
		}

	}
	short nValence;
	UINT iTwinEdge;
	
	//初始化Wedge的大小为2倍的m_nVertex，以便以后进行扩展

	for(i=0;i<m_nVertex;i++)//扫描所有点，根据每个点的m_lEdgelist,按顺时针方向生成m_piEdge,
	{
		nValence=m_pVertex[i].m_nValence;
		if(nValence<2)//某点的度数小于2
			m_pVertex[i].m_bIsBoundary=true;

		if(nValence==0)//网格中包含孤立的点，应该禁止
			continue;

		if(m_pVertex[i].m_piEdge!=NULL)
			delete[] m_pVertex[i].m_piEdge;
		m_pVertex[i].m_piEdge=new UINT[nValence];


//		if(m_pVertex[i].m_piEdge[0]==-1)//temp
//			return false;

		if(m_pVertex[i].m_bIsBoundary)//边界点
		{
			for (iv = m_pVertex[i].m_lEdgeList.begin(); iv != m_pVertex[i].m_lEdgeList.end(); iv++)
			{
				bFlag=true;
				m_pVertex[i].m_piEdge[0]=*iv;
				for(j=1;j<nValence;j++)
				{
					iTwinEdge=m_pEdge[m_pVertex[i].m_piEdge[j-1]].m_iTwinEdge;
					if(iTwinEdge==-1)//边界Edge
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
		else//内部点
		{
			iv = m_pVertex[i].m_lEdgeList.begin();
			m_pVertex[i].m_piEdge[0]=*iv;
			for(j=1;j<nValence;j++)//将该点发出的half edge顺时针存入m_piEdge
			{
				iTwinEdge=m_pEdge[m_pVertex[i].m_piEdge[j-1]].m_iTwinEdge;
				m_pVertex[i].m_piEdge[j]=m_pEdge[iTwinEdge].m_iNextEdge;
			}
		}
		m_pVertex[i].m_lEdgeList.clear();//清除临时链表
		for(j=0;j<nValence;j++)
			m_pVertex[i].m_lEdgeList.push_back(m_pVertex[i].m_piEdge[j]);

		calcVertexNormal(i);//根据点的邻面信息计算该点的平均法向
	}

	return true;
}


void CMesh::calcFaceNormal(UINT i)
{
	Vector3D v[2];

	//get the vector
	v[0] = m_pVertex[m_pFace[i].m_piVertex[2]].m_vPosition-m_pVertex[m_pFace[i].m_piVertex[0]].m_vPosition;

	if(m_pFace[i].m_nType==3)
		v[1] = m_pVertex[m_pFace[i].m_piVertex[2]].m_vPosition-m_pVertex[m_pFace[i].m_piVertex[1]].m_vPosition;
	else
		v[1] = m_pVertex[m_pFace[i].m_piVertex[3]].m_vPosition-m_pVertex[m_pFace[i].m_piVertex[1]].m_vPosition;
	m_pFace[i].m_vNormal=v[0]^v[1];

	if(m_pFace[i].m_nType == 3)
		m_pFace[i].m_dArea = 0.5*m_pFace[i].m_vNormal.normalize();//将法向单位化同时计算了该三角面的面积
	else if(m_pFace[i].m_nType == 4)
		m_pFace[i].m_dArea = m_pFace[i].m_vNormal.normalize();//将法向单位化同时计算了该四边形面的面积
	else
		m_pFace[i].m_vNormal.normalize();

	Vector3D vMassPoint;

	for(short j = 0; j < m_pFace[i].m_nType; j++){
		vMassPoint += m_pVertex[m_pFace[i].m_piVertex[j]].m_vPosition;
	}
	vMassPoint /= (double)(m_pFace[i].m_nType);
	m_pFace[i].m_vMassPoint = vMassPoint;
}

void CMesh::calcVertexNormal(UINT i)
{
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
