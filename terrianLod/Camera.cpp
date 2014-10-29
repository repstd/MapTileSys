
#include "camera.h"
#include <windows.h>
#include <gl\glu.h>
#include <math.h>

// Ĭ�ϳ�ʼ��Camera����
CCamera::CCamera()
{
	// ��ʼ��CameraΪOpenGL��Ĭ�Ϸ���

	m_vPosition = CVector3(0.0, 0.0, 0.0);  
	m_vView     = CVector3(0.0, 0.0,-1.0);
	m_vUpVector = CVector3(0.0, 1.0, 0.0);
    m_fSpeed    = 2.0f;

 	ShowCursor(FALSE);
}

// ����Cameraλ�÷���
void CCamera::PositionCamera( float posX,  float posY,  float posZ,
							  float viewX, float viewY, float viewZ,
							  float upX,   float upY,   float upZ   )
{
	// ��ʼ��Camera
	m_vPosition	= CVector3(posX,  posY,  posZ);
	m_vView	    = CVector3(viewX, viewY, viewZ);
	m_vUpVector	= CVector3(upX,   upY,   upZ);

	// ���ӵ�m_vView��������ڵ�m_vPosition��������λ��
	m_vView = m_vPosition + (m_vView-m_vPosition).GetNormalized();
}

// ����Cameraλ��
void CCamera::PositionCamera(const CVector3 &pos, const CVector3 &view, const CVector3 &up)
{
	// ��ʼ��Camera
	m_vPosition = pos;
	m_vView     = view;
	m_vUpVector = up;

	// ��m_vView��m_vPosition��ʸ����λ��
	m_vView = m_vPosition + (m_vView-m_vPosition).GetNormalized();
}

// ͨ���ƶ�����ƶ�Camera�ķ���(��һ�˳�)
void CCamera::SetViewByMouse(void)
{
	POINT mousePos;						// �洢���λ�õĽṹ��
	int middleX = GetSystemMetrics(SM_CXSCREEN) >> 1;	// ���ڿ�ȵ�һ��
	int middleY = GetSystemMetrics(SM_CYSCREEN) >> 1;	// ���ڸ߶ȵ�һ��
	float angleY = 0.0f;				// �洢���Ͽ������¿��ĽǶ�
	float angleZ = 0.0f;				// �洢���󿴡����ҿ��ĽǶ�
	static float currentRotX = 0.0f;	// �洢�ܵ����ϡ����µ���ת�Ƕ�
	
	// ��õ�ǰ���λ��
	GetCursorPos(&mousePos);						
	
	// ��������Ȼ�����м䣬�����³���
	if ((mousePos.x == middleX) && (mousePos.y == middleY)) return;

	// ������û���Ļ������
	SetCursorPos(middleX, middleY);

	// �������ƶ��ķ���
	angleY = (float)((middleX - mousePos.x)) / 1000.0f;
	angleZ = (float)((middleY - mousePos.y)) / 1000.0f;

	// ����һ����ǰ���ϻ�������ת���ĽǶȣ���������Camera������360����ת
	currentRotX -= angleZ;  

	// �����ǰ����ת���ȴ���1.3������Camera����������ת
	if (currentRotX > 1.3f)
	{
		currentRotX = 1.3f;
	}
	// �����ǰ����ת����С��1.3������Camera����������ת
	else if (currentRotX < -1.3f)
	{
		currentRotX = -1.3f;
	}
	// ����������λ����ת����
	else
	{
		// ��Camera��ˮƽ������תCamera�����£�
		RotateView(angleZ, m_vStrafe);
	}

	// ��Camera������������תCamera�����ң�
	RotateView(angleY, CVector3(0,1,0));
}

// �Ƹ���������תCamera�ķ���
void CCamera::RotateView(float angle, const CVector3 &vAxis)
{
	CVector3 vNewView;
	CVector3 vView = m_vView - m_vPosition;	
	
	vNewView = vView.GetRotatedAxis(angle, vAxis);//????????????????????????????????????? ����

	m_vView = m_vPosition + vNewView;
}

// ȷ�ϼ��̿��Ƶ��ƶ�
void CCamera::CheckForMovement()
{
	// �ж��Ƿ������ϼ���'W'��
	if (GetKeyState(VK_UP) & 0x80 || GetKeyState('W') & 0x80)
	{
		MoveCamera(m_fSpeed);		// ��ǰ�ƶ�Camera
	}

	// �ж��Ƿ������¼���'S'��
	if (GetKeyState(VK_DOWN) & 0x80 || GetKeyState('S') & 0x80)
	{
		MoveCamera(-m_fSpeed);		// ����ƶ�Camera
	}

	// �ж��Ƿ����������'A'��
	if (GetKeyState(VK_LEFT) & 0x80 || GetKeyState('A') & 0x80)
	{
		StrafeCamera(-m_fSpeed);	// �����ƶ�Camera
	}

	// �ж��Ƿ������Ҽ���'D'��
	if (GetKeyState(VK_RIGHT) & 0x80 || GetKeyState('D') & 0x80)
	{
		StrafeCamera(m_fSpeed);	    // �����ƶ�Camera
	}

	// �������
	if (GetKeyState('E') & 0x80)
	{
		if (m_fSpeed < 10)
			m_fSpeed += 0.5f;
	}
	// �������
	if (GetKeyState('R') & 0x80)
	{
		if (m_fSpeed > 0.5f)
			m_fSpeed -= 0.5f;
	}
}

// �����ƶ�Camera
void CCamera::StrafeCamera(float speed)
{
	m_vPosition.x += m_vStrafe.x * speed;
	m_vPosition.z += m_vStrafe.z * speed;
	m_vView.x += m_vStrafe.x * speed;
	m_vView.z += m_vStrafe.z * speed;
}

// ǰ���ƶ�Camera
void CCamera::MoveCamera(float speed)
{
	CVector3 vView = m_vView - m_vPosition;
	m_vPosition.x += vView.x * speed;
	m_vPosition.z += vView.z * speed;
	m_vView.x += vView.x * speed;
	m_vView.z += vView.z * speed;
}

// ����Cameraλ�úͷ���
void CCamera::Update(void)
{
	// ����Cameraˮƽ����
	m_vStrafe = ((m_vView - m_vPosition).CrossProduct(m_vUpVector)).GetNormalized();

	// ����ƶ�Camera
	SetViewByMouse();

	// �����ƶ�Camera
	CheckForMovement();
}

// �ڳ����з���Camera
void CCamera::Look(void)
{
	// ����Camera��Ϣ
	Update();

	// ����Camera
	gluLookAt(m_vPosition.x, m_vPosition.y, m_vPosition.z,
			  m_vView.x,	 m_vView.y,     m_vView.z,
			  m_vUpVector.x, m_vUpVector.y, m_vUpVector.z);
}

//�����ֱ����h
void CCamera::SetHeight(float h)
{
	CVector3 vView = m_vView - m_vPosition;	
	m_vPosition.y = h;
	m_vView = vView + m_vPosition;
}

void CCamera::SetPosX(float x)
{
	CVector3 vView = m_vView - m_vPosition;	
	m_vPosition.x = x;
	m_vView = vView + m_vPosition;
}
void CCamera::SetPosY(float y)
{
	SetHeight(y);
}
void CCamera::SetPosZ(float z)
{
	CVector3 vView = m_vView - m_vPosition;	
	m_vPosition.z = z;
	m_vView = vView + m_vPosition;
}