#ifndef __LodTerrain_H__
#define __LodTerrain_H__

#include "stdafx.h"
#include "Vector3.h"                 /**< ����������ͷ�ļ� */
#include "BMPLoader.h"
#include "Matrix.h"
#include "Camera.h"
#include "Lod.h"

class CTerrain
{
	friend class CLOD;
public:
	CTerrain(CCamera* cam);
	~CTerrain();

	void  Render(/*Camera* camera*/);
	float GetAveHeight(float x,float z);     //ͨ��˫���Բ�ֵ�õ��߶�(x,z)����������������߶�
    void  SetScale(float scale){m_fScale = scale;}
	void  SetDrawMode(int mode){ if(mode<3)m_iDrawMode = mode; }

	int   m_iDrawMode;
	int   m_iLodMode;
//private:
 	bool  LoadRawFile(const char* fileName); //��Raw�ļ��еĸ߶���Ϣ���뵽
	int   GetHeight(int x, int z);           //���(x,z)�����ŵĸ߶�y

 	CBMPLoader    m_texture[1];   //��������//������ϸ��
	int           m_nSize;        //LOD��ͼ��� 1025
	float         m_fScale;       //�趨һ���߶ȵ�λ��ʾ������
	CMatrix<BYTE> m_HMMatrix;     //???????????????????������
	CCamera*      m_pCamera;
	CLOD*         m_pLOD;         //�Ĳ�������Ҳ������ָ�룩
};

#endif //__LodTerrain_H__

	
