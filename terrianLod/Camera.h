#ifndef __CAMERA_H__
#define __CAMERA_H__

#include "Vector3.h"

//��һ�˳������

class CCamera {
public:
	CCamera();
	// ��������Camera��Ϣ
	CVector3 Position()	{ return m_vPosition;	}
	CVector3 View()		{ return m_vView;		}
	CVector3 UpVector()	{ return m_vUpVector;	}
	CVector3 Strafe()	{ return m_vStrafe;		}
	float    Speed()    { return m_fSpeed;      }

	void SetPosition(const CVector3& pos)	{   m_vPosition = pos;	}
	void SetView(const CVector3& view)		{   m_vView = view;		}
	void SetUpVector(const CVector3& up)	{   m_vUpVector = up;	}
	void SetSpeed(float speed)	            {   m_fSpeed = speed;	}
	//�ڲ��ı����߷��������£��ı������λ����Ϣ
	void SetPosX(float x);
	void SetPosY(float y);//����SetHeight
	void SetPosZ(float z);
	void SetHeight(float h);

	// ��ʼ��Camera����
	void PositionCamera(float posX,  float posY,  float posZ,
			 		    float viewX, float viewY, float viewZ,
						float upX,   float upY,   float upZ);
	void PositionCamera(const CVector3 &pos, const CVector3 &view, const CVector3 &up);
	
	// ʹ��gluLookAt()�ڳ����аڷ�Camera
	void Look();

private:
	// ͨ���ƶ�����ƶ�Camera�ķ���(��һ�˳�)
	void SetViewByMouse(void); 

	// �Ƹ���������תCamera�ķ���
	void RotateView(float angle, const CVector3 &vAxis);

	// �����ƶ�Camera(�ٶ�:speed)
	void StrafeCamera(float speed);

	// ǰ���ƶ�Camera(�ٶ�:speed)
	void MoveCamera(float speed);
	
	// ȷ�ϼ��̿��Ƶ��ƶ�
	void CheckForMovement();
	
	// ����Camere�ķ����������Ϣ
	void Update();
	
	CVector3 m_vPosition;	// Camera ��λ��
	CVector3 m_vView;		// Camera ���ӵ�
	CVector3 m_vUpVector;	// Camera ���ϵ�����
	CVector3 m_vStrafe;		// Camera ˮƽ������
	float    m_fSpeed;      // Camera �ٶ�
};

#endif //__CAMERA_H__
