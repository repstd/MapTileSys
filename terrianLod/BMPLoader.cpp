//======================================================================
// @file      CBMPLoader.cpp
//======================================================================

#include"BMPLoader.h"              // ����ͷ�ļ� 

CBMPLoader::CBMPLoader()
{
   // ��ʼ����ԱֵΪ0 
	image = 0;
	imageWidth  = 0;
	imageHeight = 0;
}

CBMPLoader::~CBMPLoader()
{
   FreeImage(); // �ͷ�ͼ������ռ�ݵ��ڴ� 
}

// װ��һ��λͼ�ļ� 
bool CBMPLoader::LoadBitmap(const char *file)
{
	FILE *pFile = 0; 
	BITMAPINFOHEADER bitmapInfoHeader;
	BITMAPFILEHEADER header;
  	unsigned char textureColors = 0;// ���ڽ�ͼ����ɫ��BGR�任��RGB

	pFile = fopen(file, "rb"); 
	if(pFile == 0) return false;
	fread(&header, sizeof(BITMAPFILEHEADER), 1, pFile);	
	if(header.bfType != BITMAP_ID)
	   {
		   fclose(pFile);             // ������λͼ�ļ�,��ر��ļ������� 
		   return false;
	   }
	fread(&bitmapInfoHeader, sizeof(BITMAPINFOHEADER), 1, pFile);
	imageWidth  = bitmapInfoHeader.biWidth;
    imageHeight = bitmapInfoHeader.biHeight;
    if(bitmapInfoHeader.biSizeImage == 0)
      bitmapInfoHeader.biSizeImage = bitmapInfoHeader.biWidth *
      bitmapInfoHeader.biHeight * 3;
	fseek(pFile, header.bfOffBits, SEEK_SET);
	image = new unsigned char[bitmapInfoHeader.biSizeImage];
	if(!image)                        // �������ڴ�ʧ���򷵻� 
	   {
		   delete[] image;
		   fclose(pFile);
		   return false;
	   }
	// ��ȡͼ������ 
	fread(image, 1, bitmapInfoHeader.biSizeImage, pFile);
	// ��ͼ����ɫ���ݸ�ʽ���н���,��BGRת��ΪRGB 
	for(int index = 0; index < (int)bitmapInfoHeader.biSizeImage; index+=3)
	   {
		   textureColors = image[index];
		   image[index] = image[index + 2];
		   image[index + 2] = textureColors;
	   }
  
	fclose(pFile);       // �ر��ļ�  
	return true;         // �ɹ�����  
}

//����λͼ�ļ�������������
bool CBMPLoader::Load(const char* fileName)
{
	if(!LoadBitmap(fileName))
	{
		MessageBox(NULL,"����λͼ�ļ�ʧ��!","����",MB_OK);
		exit(0);
	}

	glGenTextures(1, &ID);   
    glBindTexture(GL_TEXTURE_2D, ID);
	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
   
   	gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGB, imageWidth,
	                  imageHeight, GL_RGB, GL_UNSIGNED_BYTE, image);
    return true;
}

void CBMPLoader::FreeImage()   // �ͷŷ�����ڴ� 
{
   if(image)
      {
         delete[] image;
         image = 0;
      }
}
