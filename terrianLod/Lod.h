

#ifndef _LOD_H_
#define _LOD_H_

#include "stdafx.h"
#include "Terrain.h"
class CTerrain;

typedef unsigned char VS; //VS;
const VS VS_DISABLE =  0x00;
const VS VS_ACTIVE  =  0x01;
const VS VS_CULLED  =  0x02;
const VS VS_STOPED  =  0x04;
const VS VS_UNREACH =  0x08;

const int NV_L = 0;//ͬһ��������
const int NV_R = 1;
const int NV_U = 2;
const int NV_D = 3; 

const int SN_LU = 0;//��һ��������
const int SN_LD = 1;
const int SN_RD = 2;
const int SN_RU = 3;

class CLOD
{
	typedef struct _NODE
	{
		int _x,_y;
	}QNODE;
public :
// 	void AfterRender();
// 	void PreRender();
	void SetViewer(/*Camera* pCamera*/);

 	CLOD(CTerrain* t,CCamera* cam);
	~CLOD(){ }

	void DrawNode(int cx,int cy,int d);
	void DrawPrim(int cx,int cy);

	BOOL NodeCanDivid(int cx,int cy,int d);
	void DividNode(int cx,int cy,int d);
	void DisableNode(int cx,int cy,int d);
// 	bool IsViewerInRect(int cx,int cy,int d){return true;}//????????Ŀǰ��Ĭ��Ϊ�ɼ����Ժ�Ҫ�޸�

	void CheckNeighbor(int cx,int cy,int d);
    VS   CanActive(int x,int y,int d);
	void InitLODInfo();
    void RenderLOD();
	void Render();
	void CalculateDHMatrix();//??????????????????????????????????

	float m_fc;     // ȫ�ֲִڶ�����
	float m_fC;     // ȫ�־�������
	int   m_nSize;  // ��Ⱦ��ͼ�Ŀ��,��һ����Terrain�ĸ߶�ͼ�Ŀ��,����������һ��

private:
	void DrawPrim_FILL(int x, int y);
	void DrawPrim_TEXTURE(int x, int y);
	void DrawPrim_FRAME(int x, int y);
	void DrawNode_FILL(int x,int y, int d);
	void DrawNode_TEXTURE(int x, int y,int d);
	void DrawNode_FRAME(int x, int y,int d);

	int   m_delta[MAX_LOD_LEVEL];   //������õ�i��LOD�����εİ�߳�
//	int   m_maxlevel;               //ָ�������LOD�ȼ�
	float m_ViewX,m_ViewY,m_ViewZ;  //�ӵ�λ��, �������λ��

	int   m_neighbor[4]; //?????
	int   m_subnode[4];  //?????

	CTerrain*      m_pTerrain;
	CMatrix<VS>    m_LodMatrix;   //�����Ƿ�Ӧ������Ϊָ���������
	CMatrix<float> m_DHMatrix;

	CCamera*       m_pCamera;
};



#endif 