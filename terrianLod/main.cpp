

#include <time.h>

#include "stdafx.h"
#include "Vector3.h"
#include "Camera.h"
#include "Terrain.h"
#include "SkyBox.h"
#pragma comment(lib, "opengl32.lib")		
#pragma comment(lib, "glu32.lib")							
#pragma comment(lib, "glaux.lib")	
#pragma comment(lib, "glut32.lib")	
//����λ���Ѿ��ߴ���Ϣ
const unsigned int WINDOW_WIDTH = 640;
const unsigned int WINDOW_HEIGTH = 480; 
const unsigned int WINDOW_POS_X = 200; 
const unsigned int WINDOW_POS_Y = 200; 
//const double FURSTUM_ASPECT       = 4.0/3.0;

CCamera g_Camera;
CTerrain* g_pTerrain;
CSkyBox * g_pSkybox;
static bool bFullScreen = false;
static int iDrawMode    = TEXTURE;

void init();
void display(void);
void keyboard(unsigned char key, int x, int y);
void special(int key, int x, int y);
void reshape(int w, int h);
void idle();

void main(int argc, char **argv) 
{
	glutInit(&argc, argv);                           // GLUT Initializtion
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);     // Display Mode (Rgb And Double Buffered)	
	glutInitWindowPosition(WINDOW_POS_X,WINDOW_POS_Y);
	glutInitWindowSize(WINDOW_WIDTH,WINDOW_HEIGTH);               // Window Size If We Start In Windowed Mode
	glutCreateWindow("OpenGL Terrain");              // Window Title 
    init();                                          // Our Initialization
	glutDisplayFunc(display);                        // Register The Display Function
	glutReshapeFunc(reshape);                        // Register The Reshape Handler
	glutKeyboardFunc(keyboard);
	glutIdleFunc(idle);
	glutMainLoop();                                  // Go To GLUT Main Loop
}

void init()
{
	glShadeModel(GL_SMOOTH);							// Enable Smooth Shading
    glClearColor(0.0f, 0.0f, 0.0f, 0.5f);				// Black Background
    glClearDepth(1.0f);									// Depth Buffer Setup
    glEnable(GL_DEPTH_TEST);							// Enables Depth Testing
    glDepthFunc(GL_LEQUAL);								// The Type Of Depth Testing To Do
    glEnable(GL_COLOR_MATERIAL);                        // Enable Color Tracking (NEW)
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
	
	glEnable(GL_TEXTURE_2D);
	g_pTerrain = new CTerrain(&g_Camera);
	g_pSkybox = new CSkyBox();
}

void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();	

	g_Camera.Look();
	g_pTerrain->Render();/////??????
	g_pSkybox->render(&g_Camera);
	
	glutSwapBuffers();
}

void keyboard(unsigned char key, int x, int y)
{
	switch (key)
	{
	case  VK_ESCAPE: 
		  delete g_pTerrain;
		  exit(0);                   
		  break;
	case VK_SPACE://ȫ��/�����л�
		if (!bFullScreen)
			glutFullScreen();
		else 
		{
			glutPositionWindow(WINDOW_POS_X, WINDOW_POS_Y);
			glutReshapeWindow(WINDOW_WIDTH, WINDOW_HEIGTH); 
		}
		bFullScreen = !bFullScreen;
		break;	
	case 'q'://�л���ͼģʽ����ͼ�����գ���䣬�߿�
		iDrawMode++;
		if (iDrawMode > 2) iDrawMode = 0;
		g_pTerrain->SetDrawMode(iDrawMode);
		g_pSkybox->SetDrawMode(iDrawMode);
		break;
	case '2'://�л��۲�ģʽ����һ�˳ƣ������˳�
		break;
	case '3'://�ӽ��Զ���ת����/�ر�
		break;
	case '=': //���Ӵֲڶ�����
		g_pTerrain->m_pLOD->m_fc++;
		break;
	case '-': //���ʹֲڶ�����
		g_pTerrain->m_pLOD->m_fc--;
		break;
	case ']': //���Ӿ��������
		g_pTerrain->m_pLOD->m_fC++;
		break;
	case '[': //���;�������
		g_pTerrain->m_pLOD->m_fC--; 
		break;
	}
}

void special(int key, int x, int y)
{
//  camera.setViewByMouse();////////
// 	switch (key)
// 	{
// 	case GLUT_KEY_UP:
// 		break;
// 	}
}
void reshape(int w, int h)
{
	glViewport(0,0,w,h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(FURSTUM_FOVY, (float)w/(float)h, FURSTUM_NEAR, FURSTUM_FAR);
	glMatrixMode(GL_MODELVIEW);
}

void idle()
{
	static int t=0;
	static clock_t st = clock(), ed;
	glutPostRedisplay();
	t++;
	ed = clock();
	if (ed - st == 1000)
	{
		printf("fps: %d\n",  t);
		t = 0;
		st = clock();
	}
}
