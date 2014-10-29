
#include "iostream"
using namespace std;
#include <vector>

#include "stdafx.h"
#include "LOD.h"

CLOD::CLOD(CTerrain* t, CCamera* cam)
{
    m_pTerrain = t;
	m_pCamera = cam;
	m_nSize = t->m_nSize;
    m_fC = 5;
	m_fc = 1.5;
	m_LodMatrix.Reset(t->m_nSize,t->m_nSize);
	m_DHMatrix.Reset(t->m_nSize, t->m_nSize); 
	for (int i=0;i<m_nSize;i++)
		for (int j=0;j<m_nSize;j++)
			m_LodMatrix(i,j)=VS_UNREACH;
	CalculateDHMatrix();
	InitLODInfo(); 
}

void CLOD::CalculateDHMatrix()
{
	int iEdgeLength = 3;
	while(iEdgeLength <= m_nSize)
	{
		int iEdgeOffset = ( iEdgeLength-1 )>>1;	    //��߳�
		int iChildOffset = ( iEdgeLength-1 )>>2;	//�����ӵİ�߳�		
		for( int z = iEdgeOffset; z < m_nSize; z +=( iEdgeLength-1 ) )
		{
			for(int x = iEdgeOffset; x < m_nSize; x +=( iEdgeLength-1 ) )
			{
				if(iEdgeLength == 3)
				{
					//��С������,������ߵ�DH��Ȼ�������ֵ��Ϊ�������ε�//DH��
					int iDH[6];
					//BOTTOM
					iDH[0] = (int)ceil((float)abs(((m_pTerrain->GetHeight(x-iEdgeOffset, z+iEdgeOffset)+m_pTerrain->GetHeight(x+iEdgeOffset, z+iEdgeOffset))>>1)-m_pTerrain->GetHeight(x, z+iEdgeOffset)));
					//RIGHT
					iDH[1] = (int)ceil((float)abs(((m_pTerrain->GetHeight(x+iEdgeOffset, z+iEdgeOffset)+m_pTerrain->GetHeight(x+iEdgeOffset, z-iEdgeOffset))>>1)-m_pTerrain->GetHeight(x+iEdgeOffset, z)));
					//TOP
					iDH[2] = (int)ceil((float)abs(((m_pTerrain->GetHeight(x-iEdgeOffset, z-iEdgeOffset)+m_pTerrain->GetHeight(x+iEdgeOffset, z-iEdgeOffset))>>1)-m_pTerrain->GetHeight(x, z-iEdgeOffset)));
					//LEFT
					iDH[3] = (int)ceil((float)abs(((m_pTerrain->GetHeight(x-iEdgeOffset, z+iEdgeOffset)+m_pTerrain->GetHeight(x-iEdgeOffset, z-iEdgeOffset))>>1)-m_pTerrain->GetHeight(x-iEdgeOffset, z)));
					//LEFT-TOP TO RIGHT-BOTTOM
					iDH[4] = (int)ceil((float)abs(((m_pTerrain->GetHeight(x-iEdgeOffset, z-iEdgeOffset)+m_pTerrain->GetHeight(x+iEdgeOffset, z+iEdgeOffset))>>1)-m_pTerrain->GetHeight(x, z)));
					//RIGHT-TOP TO LEFT-BOTTOM
					iDH[5] = (int)ceil((float)abs(((m_pTerrain->GetHeight(x+iEdgeOffset, z-iEdgeOffset)+m_pTerrain->GetHeight(x-iEdgeOffset, z+iEdgeOffset))>>1)-m_pTerrain->GetHeight(x, z)));
					//�����DH
					int iDHMAX = iDH[0];
					for( int i = 1; i < 6; i++)
						if(iDHMAX < iDH[i])
							iDHMAX = iDH[i];
					m_DHMatrix(x,z) = iDHMAX;
				}
				else
				{
					//���������С�����Σ�Ҫ���������ٵĵȼ�һ�������εĶ���//�ڵ㣬���DH
					int iDH[14];
					int iNumDH = 0;
					float K = 1.0f * m_fC / ( 2.0f * ( m_fC - 1.0f ) );
					
					//LEFT TWO CHILD
					int iNeighborX;
					int iNeighborZ;
					iNeighborX = x - (iEdgeLength - 1);
					iNeighborZ = z;
					if(iNeighborX > 0)
					{
						iDH[iNumDH] = m_DHMatrix(iNeighborX + iChildOffset, iNeighborZ - iChildOffset);
						iNumDH++;
						iDH[iNumDH] = m_DHMatrix(iNeighborX + iChildOffset, iNeighborZ + iChildOffset);
						iNumDH++;
					}
					//TOP TWO CHILD
					iNeighborX = x ;
					iNeighborZ = z - (iEdgeLength - 1);			
					if(iNeighborZ > 0)
					{
						iDH[iNumDH] = m_DHMatrix(iNeighborX - iChildOffset, iNeighborZ + iChildOffset);
						iNumDH++;
						iDH[iNumDH] = m_DHMatrix(iNeighborX + iChildOffset, iNeighborZ + iChildOffset);
						iNumDH++;
					}
					//RIGHT TWO CHILD
					iNeighborX = x + (iEdgeLength - 1);
					iNeighborZ = z ;					
					if(iNeighborX < m_nSize)
					{
						iDH[iNumDH] = m_DHMatrix(iNeighborX - iChildOffset, iNeighborZ - iChildOffset);
						iNumDH++;
						iDH[iNumDH] = m_DHMatrix(iNeighborX - iChildOffset, iNeighborZ + iChildOffset);
						iNumDH++;
					}
					//BOTTOM TWO CHILD
					iNeighborX = x ;
					iNeighborZ = z + (iEdgeLength - 1);					
					if(iNeighborZ < m_nSize)
					{
						iDH[iNumDH] = m_DHMatrix(iNeighborX - iChildOffset, iNeighborZ - iChildOffset);
						iNumDH++;
						iDH[iNumDH] = m_DHMatrix(iNeighborX + iChildOffset, iNeighborZ - iChildOffset);
						iNumDH++;
					}
					//Ȼ���������DHi
					//BOTTOM T
					iDH[iNumDH]	= (int)ceil((float)abs(((m_pTerrain->GetHeight(x-iEdgeOffset, z+iEdgeOffset)+m_pTerrain->GetHeight(x+iEdgeOffset, z+iEdgeOffset))>>1)-m_pTerrain->GetHeight(x,z+iEdgeOffset)));
					iNumDH++;
					//RIGHT
					iDH[iNumDH] = (int)ceil((float)abs(((m_pTerrain->GetHeight(x+iEdgeOffset, z+iEdgeOffset)+m_pTerrain->GetHeight(x+iEdgeOffset, z-iEdgeOffset))>>1)-m_pTerrain->GetHeight(x+iEdgeOffset,z)));
					iNumDH++;
					//TOP
					iDH[iNumDH]	= (int)ceil((float)abs(((m_pTerrain->GetHeight(x-iEdgeOffset, z-iEdgeOffset)+m_pTerrain->GetHeight(x+iEdgeOffset, z-iEdgeOffset))>>1)-m_pTerrain->GetHeight(x,z-iEdgeOffset)));
					iNumDH++;
					//LEFT
					iDH[iNumDH] = (int)ceil((float)abs(((m_pTerrain->GetHeight(x-iEdgeOffset, z+iEdgeOffset)+m_pTerrain->GetHeight(x-iEdgeOffset, z-iEdgeOffset))>>1)-m_pTerrain->GetHeight(x-iEdgeOffset,z)));
					iNumDH++;
					//LEFT-TOP TO RIGHT-BOTTOM
					iDH[iNumDH] = (int)ceil((float)abs(((m_pTerrain->GetHeight(x-iEdgeOffset, z-iEdgeOffset)+m_pTerrain->GetHeight(x+iEdgeOffset, z+iEdgeOffset))>>1)-m_pTerrain->GetHeight(x,z)));
					iNumDH++;
					//RIGHT-TOP TO LEFT-BOTTOM
					iDH[iNumDH]	= (int)ceil((float)abs(((m_pTerrain->GetHeight(x+iEdgeOffset, z-iEdgeOffset)+m_pTerrain->GetHeight(x-iEdgeOffset, z+iEdgeOffset))>>1)-m_pTerrain->GetHeight(x, z)));
					iNumDH++;
										
					int iDHMAX = iDH[0];
					for( int i = 1 ; i < iNumDH ; i++)
					{
						if(iDHMAX < iDH[i])
							iDHMAX = iDH[i];
					}
					m_DHMatrix(x,z) = (int)ceil(K * iDHMAX);	
				}
			}//end for
		}//end for
			iEdgeLength = ( iEdgeLength<<1 ) - 1;
	}//end while	
}

VS CLOD::CanActive(int x,int z,int size)
{
	if (size==2)                
		return VS_DISABLE;
	//int size = 2*d;

	float dx = x - m_ViewX;
	float dz = z - m_ViewZ;
	float y  = m_pTerrain->GetHeight(x,z);
	float dy = y - m_ViewY;
	//////////////////////////////////////////////////////////////////////////
	float f;
	float fViewDistance= (float)( fabs(dx)+fabs(dy)+fabs(dz) );
	f = fViewDistance/( size*m_fC*max(m_fc*m_DHMatrix(x,z), 1.0f) );	
	if(f < 1.0f)
		return VS_ACTIVE;		
	else
		return VS_DISABLE;				
}

void CLOD::InitLODInfo()
{
	//m_delta�����ﱣ����ÿһ����Ӧ��level��Rect��Ƭ�Ĵ�С��
	//�� LOD��Ĺ����ʱ��Ҫ������������ʼ���á�
	//������m_delta[0]=size/2,Ҳ���ǵ�ͼ�İ�߳�
	int size = m_nSize - 1; 
//	m_maxlevel = -2;
	for(int i=0; size; i++)
	{
		this->m_delta[i] = size/2;
		size = size/2;
//		m_maxlevel++;
	}
}
/*#define GetDelta(x) m_delta[(x)]*/
//==================================================================
//�ú���Ϊ LOD ��Ƭ���º�������ڡ��������Ȱ��������θ����
//����һ��һ�� ���(level)�Ĳ���ÿ���ڵ㣬�����Ҫ�������ĵ��ӽڵ�Ļ�
//�ͰѰ������ӽڵ��Ƶ�һ�������У��Ա�����һ����β��Ե�ʱ���á�
//���һ���ڵ�û���ӽڵ�ҪҪ������Ǿ� Disable�������ĸ��ӽڵ㣬�Է�ֹ�����ѷ�
//ͬʱ��������ڵ㡣
//���������ȫ�ֱ��ʵ�ʱ������Ҳ�򵥵Ļ�������
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
void  CLOD::RenderLOD()
{
   	vector<QNODE>  v1;
	vector<QNODE>  v2;
	vector<QNODE>* cur_Queue;
	vector<QNODE>* next_Queue, *temp_v;
	cur_Queue =&v1; next_Queue = &v2;

	//�Ƚ����ڵ����cur_Queue;
	int level = 0,i=0;
	int d = m_delta[0];
 	int cx = (m_nSize - 1)/2;
 	int cy = (m_nSize - 1)/2;

    QNODE node ={cx,cy};
	cur_Queue->push_back(node);
	while( d > 0)
	{
		int vlen = cur_Queue->size();
		for(i = 0 ; i < vlen;i++)
		{
			node = (*cur_Queue)[i];
			cx = node._x;
			cy = node._y;

//���˴��ɷ��òü����롿-------
            //ȡ�����ڽڵ���Ϣ
			CheckNeighbor(cx,cy,d);
			if( NodeCanDivid(cx,cy,d) && CanActive(cx,cy,2*d) )
			{
				if( d == 1)
				{
					//m_LodMatrix(cx,cy) = VS_ACTIVE;
					DrawPrim(cx,cy);
				}
				else
				{
					DividNode(cx,cy,d);
					//Push the left_up sub node to the next level Queue
					node._x = cx - d/2;
					node._y = cy + d/2;
					next_Queue->push_back(node);

					//Push the left_down sub node to the next level queue
					node._x = cx - d/2;
					node._y = cy - d/2;
					next_Queue->push_back(node);

					//Push the right_up sub node to the next level queue
					node._x = cx + d/2;
					node._y = cy + d/2;
					next_Queue->push_back(node);

					//Push the right_down sub node to the next level queue
					node._x = cx + d/2;
					node._y = cy - d/2;
					next_Queue->push_back(node);
				}
			}
			else
			{
				DisableNode(cx,cy,d);
				DrawNode(cx,cy,d);
			}			
		}//end for
		cur_Queue->clear();

		//Swap the two queue
		temp_v = cur_Queue;
		cur_Queue = next_Queue;
		next_Queue = temp_v;
		level ++ ;
		d = m_delta[level];
	}//end while
}

void CLOD::Render()
{
	SetViewer();

//	PreRender();
	RenderLOD();
//  AfterRender(); 
}

void CLOD::DisableNode(int cx, int cy, int d)
{
	int d2 = d/2;
	m_LodMatrix(cx,cy) = VS_ACTIVE;

	if(d == 1)
		return;
	m_LodMatrix( cx - d2, cy - d2) = VS_DISABLE;
	m_LodMatrix( cx - d2, cy + d2) = VS_DISABLE;
	m_LodMatrix( cx + d2, cy - d2) = VS_DISABLE;
	m_LodMatrix( cx + d2, cy + d2) = VS_DISABLE;
}

void CLOD::DividNode(int cx, int cy, int d)
{
	int d2 = d/2;
	m_LodMatrix(cx,cy)=VS_ACTIVE;

	m_LodMatrix( cx - d2, cy - d2 ) = VS_ACTIVE ;
	m_LodMatrix( cx - d2, cy + d2 ) = VS_ACTIVE ;
	m_LodMatrix( cx + d2, cy - d2 ) = VS_ACTIVE ;
	m_LodMatrix( cx + d2, cy + d2 ) = VS_ACTIVE ;
}

BOOL CLOD::NodeCanDivid(int cx, int cy, int d)
{	
    if(m_neighbor[0] == VS_DISABLE)
	   return FALSE;
	if(m_neighbor[1] == VS_DISABLE)
	   return FALSE;
	if(m_neighbor[2] == VS_DISABLE)
	   return FALSE;
	if(m_neighbor[3] == VS_DISABLE)
	   return FALSE;
	return TRUE;
}

void CLOD::CheckNeighbor(int cx, int cy, int d)
{
	int nx,ny;
	int d2= 2*d;

	m_neighbor[NV_L] = VS_ACTIVE;
	m_neighbor[NV_U] = VS_ACTIVE;
	m_neighbor[NV_D] = VS_ACTIVE;
	m_neighbor[NV_R] = VS_ACTIVE;
	//Test the four neighbor if they are all enabled/stoped
	nx = cx;ny = cy - d2;
	if(ny >0)
	{
		if(m_LodMatrix(nx,ny) == VS_DISABLE)
			m_neighbor[NV_D] = VS_DISABLE;
	}

	nx = cx;ny = cy + d2;
	if(ny<m_nSize)
	{
		if(m_LodMatrix(nx,ny) == VS_DISABLE)
			m_neighbor[NV_U] = VS_DISABLE;
	}

	nx = cx + d2;ny = cy;
	if(nx <m_nSize )
	{
		if(m_LodMatrix(nx,ny) == VS_DISABLE)
			m_neighbor[NV_R] = VS_DISABLE;
	}

	nx = cx - d2;ny = cy;
	if(nx > 0 )
	{
		if(m_LodMatrix(nx,ny) == VS_DISABLE)
			m_neighbor[NV_L] = VS_DISABLE;
	}
}

void CLOD::SetViewer()
{
	CVector3 m = m_pCamera->Position(); 
	m_ViewX = m.x;
	m_ViewY = m.y;
	m_ViewZ = m.z;
}

#define GLVERTEX(x,z)  glVertex3f(x, m_pTerrain->GetHeight(x,z), z)

void CLOD::DrawNode(int x, int z, int d)
{
	switch(m_pTerrain->m_iDrawMode)
	{
	case TEXTURE:
		DrawNode_TEXTURE(x,z,d);
		break;
	case FILL:
		DrawNode_FILL(x, z, d);
		break;
	case FRAME:
		DrawNode_FRAME(x,z,d);
		break;
	default:
		break;
	}
}
void CLOD::DrawPrim(int x,int z)
{
	switch(m_pTerrain->m_iDrawMode)
	{
	case TEXTURE:
		DrawPrim_TEXTURE(x,z);
		break;
	case FILL:
		DrawPrim_FILL(x, z);
		break;
	case FRAME:
		DrawPrim_FRAME(x,z);
		break;
	default:
		break;
	}
}

 void CLOD::DrawNode_FILL(int x, int z, int d)
 {
	 glBegin(GL_TRIANGLE_FAN);
	 //Center
	 glColor3f(1,0,0);
	 GLVERTEX(x, z);

	 glColor3f(1,1,1);
	 //Left_up
	 GLVERTEX(x-d, z+d);
	 //Left
	 if(m_neighbor[NV_L] == VS_ACTIVE)
		 GLVERTEX(x-d, z);
	 //Left down
	 GLVERTEX(x-d, z-d);
	 //down
	 if(m_neighbor[NV_D] == VS_ACTIVE)
		 GLVERTEX(x, z-d);
	 //right down
	 GLVERTEX(x+d, z-d);
	 //right
	 if(m_neighbor[NV_R] == VS_ACTIVE)
		GLVERTEX(x+d, z);
	 //Right up
	 GLVERTEX(x+d, z+d);
	 //up
	 if(m_neighbor[NV_U] == VS_ACTIVE)
		 GLVERTEX(x, z+d);	 
	 //Left_up
	 GLVERTEX(x-d, z+d);
	 glEnd();
}
void CLOD::DrawNode_TEXTURE(int x, int z, int d)
{
	float tx = float(x)/m_nSize;
	float tz = float(z)/m_nSize;
	float td = float(d)/m_nSize;
	
	glBegin(GL_TRIANGLE_FAN);
	//Center
	glTexCoord2f(tx,tz); 
	GLVERTEX(x, z);	
	//Left_up
	glTexCoord2f(tx - td, tz + td); 
	GLVERTEX(x-d, z+d);
	//Left
	if(m_neighbor[NV_L] == VS_ACTIVE)
	{
		glTexCoord2f(tx - td, tz); 
		GLVERTEX(x-d, z);
	}
	//Left down
	glTexCoord2f(tx - td, tz - td);
	GLVERTEX(x-d, z-d);
	//down
	if(m_neighbor[NV_D] == VS_ACTIVE)
	{
		glTexCoord2f(tx , tz - td);
		GLVERTEX(x, z-d);
	}
	//right down
	glTexCoord2f(tx + td, tz - td);
	GLVERTEX(x+d, z-d);
	
	//right
	if(m_neighbor[NV_R] == VS_ACTIVE)
	{
		// 			
		// #ifdef _COUNT_TRIAN_COUNT
		// 			m_trian_count++;
		// #endif
		glTexCoord2f(tx + td, tz);
		GLVERTEX(x+d, z);
	}
	//Right up
	glTexCoord2f(tx + td, tz + td);
	GLVERTEX(x+d, z+d);
	//up
	if(m_neighbor[NV_U] == VS_ACTIVE)
	{		
		// #ifdef _COUNT_TRIAN_COUNT
		// 			m_trian_count++;
		// #endif
		glTexCoord2f(tx , tz + td);
		GLVERTEX(x, z+d);
	}	
	//Left_up
	glTexCoord2f(tx - td, tz + td);
	GLVERTEX(x-d, z+d);

	glEnd();
}

void CLOD::DrawNode_FRAME(int x, int z, int d)//�����ס�����
{
	glPushAttrib(GL_COLOR);
	glColor3f(1,1,1);
	glBegin(GL_LINE_STRIP);		
		GLVERTEX(x+d, z-d);              //right down	
		GLVERTEX(x, z);                  //Center		
		GLVERTEX(x-d, z+d);              //Left_up		
		if(m_neighbor[NV_L]==VS_ACTIVE)  //Left
			GLVERTEX(x-d, z);	
		GLVERTEX(x-d, z-d);	             //Left down		
		if(m_neighbor[NV_D]==VS_ACTIVE)  //down
			GLVERTEX(x, z-d);	
		GLVERTEX(x+d, z-d);              //right down
		if(m_neighbor[NV_R] == VS_ACTIVE)//right
			GLVERTEX(x+d, z);
		GLVERTEX(x+d, z+d);              //Right up
		if(m_neighbor[NV_U] == VS_ACTIVE)//up
			GLVERTEX(x, z+d);	 		
		GLVERTEX(x-d, z+d);              //Left_up
	glEnd();	
	glBegin(GL_LINE_STRIP);
		GLVERTEX(x-d, z-d);	//Left down		
		GLVERTEX(x, z);     //Center	
		GLVERTEX(x+d, z+d); //Right up
	glEnd();
	glBegin(GL_LINE_STRIP);		
		if(m_neighbor[NV_D] == VS_ACTIVE)//down
			GLVERTEX(x, z-d);		
		GLVERTEX(x, z);                  //Center	
		if(m_neighbor[NV_U] == VS_ACTIVE)//up
			GLVERTEX(x, z+d);	 
	glEnd();		
	glBegin(GL_LINE_STRIP);		
		if(m_neighbor[NV_L] == VS_ACTIVE)//Left
			GLVERTEX(x-d, z);	
		GLVERTEX(x, z);                  //Center
		if(m_neighbor[NV_R] == VS_ACTIVE)//right
			GLVERTEX(x+d, z);
	glEnd();
	glPushAttrib(GL_COLOR);
}

void CLOD::DrawPrim_TEXTURE(int x, int z)
{
	 DrawNode_TEXTURE(x, z, 1);
}

void CLOD::DrawPrim_FILL(int x, int z)
{
	 DrawNode_FILL(x,z,1);
}


void CLOD::DrawPrim_FRAME(int x, int z)
{
	DrawNode_FRAME(x,z,1);
}
