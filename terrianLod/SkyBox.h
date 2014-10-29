
#ifndef __SKYBOX_H__
#define __SKYBOX_H__

#include "stdafx.h"
#include "BMPLoader.h"
#include "Vector3.h"
#include "Camera.h"

#define GL_CLAMP_TO_EDGE	0x812F

/** ��պ��� */
class CSkyBox
{
public:
	CSkyBox();
	~CSkyBox();

	bool init();
	void render(CCamera *cam);

	void SetDrawMode(int mode){m_iDrawMode = mode;}
private:
	CBMPLoader  m_texture[6];   /**< ��պ�����   */
	CVector3    m_CameraPos;    /**< ��ǰ�����λ�� */
	float       length;         /**< ���� */
	float       width;          /**< ��� */
	float       height;         /**< �߶� */
	float       yRot;           /**< ��Y����ת */

	int         m_iDrawMode;
		
};


#endif ///__SKYBOX_H__