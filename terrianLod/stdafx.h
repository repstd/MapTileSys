/**  
*   @file   stdafx.h
*/

#ifndef __STDAFX_H__
#define __STDAFX_H__

/** �������õ�ͷ�ļ� */
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>                            
#define GLUT_DISABLE_ATEXIT_HACK
/** ����glͷ�ļ� */
#include <gl\gl.h>				 
#include <gl\glu.h>
#include <gl\glaux.h>
#include <gl\glut.h>

/** ����OpenGL���ӿ��ļ� */

/** ��ֹ��������������ת���ľ��� */
#pragma warning(disable: 4311)                                 
#pragma warning(disable: 4312)
#pragma warning(disable: 4244)
#pragma warning(disable: 4018)
#pragma warning(disable: 4267)

//------����������� MAP_WIDTH, CELL_WIDTH
#define TERRAIN_IMAGE_FILE "data/terrain.bmp" //����ͼƬ���ļ���
#define TERRAIN_RAW_FILE "data/terrain.raw"   //���θ߶�ͼ�ļ���
//------���Ʒ�ʽ
const int TEXTURE = 0;   //����
const int FRAME   = 1;   //�߿�
const int FILL    = 2;   //���
const int QUADTREE  = 0;
const int ROAM      = 1;

///�Ӿ�����Ϣ
const double FURSTUM_NEAR = 1.0;      //�Ӿ����near(ƽͷ����)
const double FURSTUM_FAR  = 2000.0;    //�Ӿ����far (ƽͷ����)
const double FURSTUM_FOVY = 80;       //??
//-----LOD�㷨�е�һЩ����
const unsigned int MAX_LOD_LEVEL=10;
const unsigned int LOD_MAP_SIZE = 1025 ;//���������������ƶ���Χ
const int EYES_HEIGHT = 10;  //��һ�˳�������۾��߶�
const float HEIGHT_SCALE = 1.0f;//���θ߶��������ӵ�Ĭ��ֵ
#endif