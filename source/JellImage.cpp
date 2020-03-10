#include "StdAfx.h"
#include "JellImage.h"




/********************************************************
*������: CJellImage
*
*��������:���캯��
*
*����:  ��
*
*����ֵ:��
********************************************************/
CJellImage::CJellImage()
{
	ClearMemory();
}

/********************************************************
*������: ~CJellImage
*
*��������:��������
*
*����:��
*
*����ֵ:��
********************************************************/
CJellImage::~CJellImage()
{
	ClearMemory();
}


/********************************************************
*������: SetParentHwnd
*
*��������:�õ���ǰ�����ڵľ�� 
*
*����:
hWnd ��ʾ��ǰ���ڵľ��
hint ��ʾ��������߳�
*
*����ֵ:��
********************************************************/
void CJellImage::SetParent(HWND hWnd, HINSTANCE hint)
{
	m_hWndParent = hWnd;

	m_hinstance = hint; 
}






/********************************************************
*������: LoadFile
*
*��������:��ȡһ���ļ�
*
*����:��
*
*����ֵ:�ɹ�����true, ʧ�ܷ���false
********************************************************/
bool CJellImage::LoadFile()
{
	if (!CloseFile())
	{
		return false;
	}

	OPENFILENAME   fileStruct;     
	
	ZeroMemory(&fileStruct,   sizeof(OPENFILENAME));     
	
	fileStruct.lStructSize   =   sizeof(OPENFILENAME);   
	
	fileStruct.hwndOwner   =   NULL;     
	
	fileStruct.lpstrFilter   =  "����ͼƬ�ļ�\0*.gif;*.bmp;*.jpg;*.jpeg;*.png\0BMP�ļ�(*.bmp)\0*.bmp\0JPG�ļ�(*.jpg)\0*.jpg\0JPEG�ļ�(*.jpeg)\0*.jpeg\0GIF�ļ�(*.gif)\0*.gif\0PNG�ļ�(*.png)\0*.png\0";
	
	fileStruct.nMaxFile   =   MAX_PATH;
	
	TCHAR   szFile[MAX_PATH]="";
	
	fileStruct.lpstrFile = szFile;
	
	fileStruct.lpstrTitle="";
	
	fileStruct.lpstrDefExt="";   
	
	fileStruct.nMaxFile   = MAX_PATH;   
	
	fileStruct.hInstance   =   m_hinstance;
	
	if(GetOpenFileName(&fileStruct) == FALSE)
	{
		m_strLastError = "�û�ȡ���˴��ļ��Ĳ���";

		return false;
	}
	else
	{
		if (!OpenFileToHDC(fileStruct.lpstrFile))
		{
			//MessageBox(m_hWndParent, "�ļ�: " + m_strFileName + " �򿪴���", "����", MB_OK | MB_ICONERROR);

			m_strLastError = "���ļ�ʱ��������";
		
			return false;
		}
	}
	return  true;
}


/********************************************************
*������: IsVaild
*
*��������:��ǰ�Ƿ����
*
*����:��
*
*����ֵ:�����Ƿ����
********************************************************/
bool CJellImage::IsVaild()
{
	return m_bIsVaild;
}


/********************************************************
*������: SaveFile
*
*��������:���浱ǰ�ļ�
*
*����:��
*
*����ֵ:�ɹ�����true, ʧ�ܷ���false
********************************************************/
bool CJellImage::SaveFile()
{
	if (m_bIsVaild)
	{
		if (SaveHDCToFile(m_strFileName.GetBuffer(0)))
		{
			m_strFileName.ReleaseBuffer();
			if (OpenFileToHDC(m_strFileName.GetBuffer(0)))
			{
				m_strFileName.ReleaseBuffer();
				
				return true;
			}
			else
			{
				m_strLastError = "���ļ������,���¶�ȡʱ��������";

				return false;
			}
		}
		else
		{
			m_strLastError = "�ļ��޷�����";

			return false;
		}
	}
	else
	{

		m_strLastError = "��ǰû�д��ļ�";

		return false;
	}
}


/********************************************************
*������: SaveAsFile
*
*��������:�ļ����Ϊ
*
*����:
lpszFileName ��ʾҪ������ļ���
*
*����ֵ:�ɹ�����true, ʧ�ܷ���false
********************************************************/
bool CJellImage::SaveAsFile()
{
	if (m_bIsVaild)
	{
		OPENFILENAME   fileStruct;     
		
		ZeroMemory(&fileStruct,   sizeof(OPENFILENAME));     
		
		fileStruct.lStructSize   =   sizeof(OPENFILENAME);   
		
		fileStruct.hwndOwner   =   NULL;     
		
		fileStruct.lpstrFilter   =  "BMP�ļ�(*.bmp)\0*.bmp\0JPG�ļ�(*.jpg)\0*.jpg\0JPEG�ļ�(*.jpeg)\0*.jpeg\0GIF�ļ�(*.gif)\0*.gif\0PNG�ļ�(*.png)\0*.png\0";
		
		fileStruct.nMaxFile   =   MAX_PATH;
		
		TCHAR   szFile[MAX_PATH]="";
		
		fileStruct.lpstrFile = szFile;
		
		fileStruct.lpstrTitle="";
		
		fileStruct.lpstrDefExt="";   
		
		fileStruct.nMaxFile   = MAX_PATH;   
		
		fileStruct.hInstance   =   m_hinstance;
		
		if(GetSaveFileName(&fileStruct) == FALSE)
		{
			m_strLastError = "�û�ȡ�����ļ����Ϊ";

			return false;
		}
		
		if (SaveHDCToFile(fileStruct.lpstrFile))
		{
			if (OpenFileToHDC(fileStruct.lpstrFile))
			{
				m_strFileName = fileStruct.lpstrFile;
				
				m_bHaveChanged = false;

				return true;
			}
			else
			{
				m_strLastError = "���ļ����Ϊ��,���¶�ȡʱ��������";

				return false;
			}
		}
		else
		{
			m_strLastError = "�ļ��޷����Ϊ";

			return false;
		}
	}
	else
	{
		m_strLastError = "��ǰû�д��ļ�";

		return false;
	}
}


/********************************************************
*������: CloseFile
*
*��������:�ر��ļ�
*
*����:��
*
*����ֵ:�ɹ�����true, ʧ�ܷ���false
********************************************************/
bool CJellImage::CloseFile()
{
	if (m_bIsVaild && m_bHaveChanged)
	{
		int nID = MessageBox(m_hWndParent, "�Ƿ�Ҫ���浱ǰ�ļ�: " + m_strFileName, "ѯ��", MB_YESNOCANCEL | MB_ICONQUESTION);

		switch(nID)
		{
		case IDYES:
			{
				if (!SaveHDCToFile(m_strFileName.GetBuffer(0)))
				{
					m_strLastError = "�ļ����治�ɹ�";

					//MessageBox(m_hWndParent, "�ļ�: " + m_strFileName + " �������", "����", MB_OK | MB_ICONERROR);
				}
				m_strFileName.ReleaseBuffer();

				break;
			}
		case IDNO:
			{
				ClearMemory();

				break;
			}
		case IDCANCEL:
			{

				m_strLastError = "�û�ȡ���˹ر��ļ��Ĳ���";

				return false;

				break;
			}
		}
	}
	else
	{
		ClearMemory();
	}

	return true;
}


/********************************************************
*������: Draw
*
*��������:��ͼƬ����һ���豸��
*
*����:

  DestDC Ŀ���豸
  DestX  Ŀ��X������
  DestY  Ŀ��Y������
  DestWidth Ŀ���
  DestHeight Ŀ���
  
		  SrcX   ԴͼX������
		  SrcY   ԴͼY������
		  SrcWidth Դͼ��
		  SrcHeight Դͼ��
		  
			DrawMode ��ͼģʽ
			
			  *
			  *����ֵ:�ɹ�����true, ʧ�ܷ���false
********************************************************/
bool CJellImage::Draw(HDC DestDC, long DestX, long DestY, long DestWidth, long DestHeight, long SrcX, long SrcY, long SrcWidth, long SrcHeight, DWORD  DrawMode)
{

	if (!m_bIsVaild)
	{
		m_strLastError = "��ǰͼƬ������";

		return false;
	}
	else
	{
		if (!StretchBlt(DestDC, DestX, DestY, DestWidth, DestHeight, m_stack_current.top()->hdc, SrcX, SrcY, SrcWidth, SrcHeight, DrawMode))
		{
			m_strLastError = "��ͼʱ��������";

			return false;
		}
	}
	return true;
}



/********************************************************
*������: GetHeight
*
*��������:�õ�ͼƬ�ĸ�
*
*����:��
*
*����ֵ:����ͼƬ�ĸ�
********************************************************/
long CJellImage::GetHeight()
{
	if (m_bIsVaild)
	{
		return m_lHeight;
	}
	else
	{
		return 0;
	}
}



/********************************************************
*������: GetWidth
*
*��������:�õ�ͼƬ�Ŀ�
*
*����:��
*
*����ֵ:����ͼƬ�Ŀ�
********************************************************/
long CJellImage::GetWidth()
{
	if (m_bIsVaild)
	{
		return m_lWidth;
	}
	else
	{
		return 0;
	}

}



/********************************************************
*������: GetLastError
*
*��������:��������������Ϣ
*
*����:��
*
*����ֵ:��������������Ϣ
********************************************************/
CString CJellImage::GetLastError()
{
	return m_strLastError;
}




/********************************************************
*������: GetFileName
*
*��������:�õ���ǰ���ļ���
*
*����:��
*
*����ֵ:���ص�ǰ���ļ���
********************************************************/
CString CJellImage::GetFileName()
{
	if (m_bIsVaild)
	{
		return m_strFileName;
	}
	else
	{
		return "";
	}
}




/********************************************************
*������: GetFileType
*
*��������:�õ���ǰ���ļ�����
*
*����:��
*
*����ֵ:����ǰ���ļ�����  "gif", "jpeg", "bmp", "jpg", "png"
********************************************************/
CString CJellImage::GetFileType()
{
	if (!m_bIsVaild)
	{
		return "Unknow";
	}
	
	CString strFileType;

	switch(m_FileType)
	{
	case GIF:
		{
			strFileType = "gif";

			break;
		}
	case JPEG:
		{
			strFileType = "jpeg";

			break;
		}
	case JPG:
		{
			strFileType = "jpg";

			break;
		}
	case BMP:
		{
			strFileType = "bmp";

			break;
		}
	case PNG:
		{
			strFileType = "png";

			break;
		}

	case UNKNOW:
		{
			strFileType = "Unknow";

			break;
		}
	}

	return strFileType;
}



/********************************************************
*������: Back
*
*��������:���˵�ǰһ��״̬
*
*����:��
*
*����ֵ:�ɹ�����true, ʧ�ܷ���false
********************************************************/
bool CJellImage::Back()
{
	if (!m_bIsVaild)
	{
		m_strLastError = "ͼƬ������";

		return false;
	}
	else
	{
		if (m_stack_current.size() <= 1)
		{
			m_strLastError = "�Ѿ���������һ��״̬��";

			return false;
		}
		else
		{
			m_stack_history.push(m_stack_current.top());

			m_stack_current.pop();

			//m_Shape.Init();
		}
	}

	return true;
}



/********************************************************
*������: Forward
*
*��������:ǰ������һ��״̬
*
*����:��
*
*����ֵ:�ɹ�����true, ʧ�ܷ���false
********************************************************/
bool CJellImage::Forward()
{
	if (!m_bIsVaild)
	{
		m_strLastError = "ͼƬ������";

		return false;
	}
	else
	{
		if (m_stack_history.size() <= 0)
		{
			m_strLastError = "�Ѿ��ظ������һ��״̬��";

			return false;
		}
		else
		{
			m_stack_current.push(m_stack_history.top());

			m_stack_history.pop();

			//m_Shape.Init();
		}
	}

	return true;
}


/********************************************************
*������: GetBackCount
*
*��������:�õ�����״̬�ĸ���
*
*����:��
*
*����ֵ:���ظ���
********************************************************/
int CJellImage::GetBackCount()
{
	if (m_bIsVaild)
	{
		return m_stack_current.size();
	}
	else
	{
		return 0;
	}
	
}




/********************************************************
*������: GetForwardCount
*
*��������:�õ���ǰ״̬�ĸ���
*
*����:��
*
*����ֵ:���ظ���
********************************************************/
int CJellImage::GetForwardCount()
{
	if (m_bIsVaild)
	{
		return m_stack_history.size();
	}
	else
	{
		return 0;
	}
	
}



/********************************************************
*������: ClearMemory
*
*��������:��յ�ǰ��������
*
*����:��
*
*����ֵ:��
********************************************************/
void CJellImage::ClearMemory()
{
	m_FileType = UNKNOW;

	while (!m_stack_current.empty())
	{
		m_stack_current.top()->ClearMemory();

		delete m_stack_current.top();

		m_stack_current.pop();
	}


	while (!m_stack_history.empty())
	{
		m_stack_history.top()->ClearMemory();

		delete m_stack_history.top();

		m_stack_history.pop();
	}


	//m_strFileName = "";//��ǰ���ļ���

	m_bIsVaild = false;//��ǰ���ļ��Ƿ����

	m_lWidth = 0;//λͼ�Ŀ�

	m_lHeight = 0;//λͼ�ĸ�

	m_strLastError = "";//���һ�����������

	m_bHaveChanged = false;//ͼƬ�Ƿ��Ѿ��ı����
}


/********************************************************
*������: TransFormFileTypeToCXImageDword
*
*��������:�����Լ����ļ�����ת����CXImage�ĸ�ʽ����
*
*����˵��:
*       FILETYPE��ʾ��ǰ���ļ�����
*
*��������˵��: CXimage�ĸ�ʽ����
*******************************************************/
DWORD CJellImage::TransFormFileTypeToCXImageDword(FILETYPE FileType)
{
	switch(FileType)
	{
	case BMP:
		{
			return CXIMAGE_FORMAT_BMP; 
			break;
		}
	case JPEG:
	case JPG:
		{
			return CXIMAGE_FORMAT_JPG;
			break;
		}

	case GIF:
		{
			return CXIMAGE_FORMAT_GIF;
			break;
		}

	case PNG:
		{
			return CXIMAGE_FORMAT_PNG;
			break;
		}
	}

	return CXIMAGE_FORMAT_BMP;
}



/********************************************************
*������: SaveHDCToFile
*
*��������:�ѵ�ǰ�����ݱ��浽�ļ���
*
*����:
lpszFileName  Ҫ������ļ���
*
*����ֵ:�ɹ�����true, ʧ�ܷ���false
********************************************************/
bool CJellImage::SaveHDCToFile(char *lpszFileName)
{
	bool bHaveSave = false;
	
	CxImage image;
	
	image.CreateFromHBITMAP(m_stack_current.top()->bitmap);
	
	FILETYPE FileType = GetFileExt(lpszFileName);
	
	switch(FileType) 
	{
	case BMP:
		{
			bHaveSave = image.Save(lpszFileName, CXIMAGE_FORMAT_BMP);
			
			break;
		}
	case JPEG:
	case JPG:
		{
			if(!image.IsGrayScale()) 
			{
				image.IncreaseBpp(24);
			}
			image.SetJpegQuality(99);
			
			bHaveSave = image.Save(lpszFileName, CXIMAGE_FORMAT_JPG);
			
			break;
		}
	case GIF:
		{

			image.DecreaseBpp(8, true);   
			
			bHaveSave = image.Save(lpszFileName,CXIMAGE_FORMAT_GIF); 
		}
		
	case PNG:
		{
			bHaveSave = image.Save(lpszFileName, CXIMAGE_FORMAT_PNG);
		}
	}
	
	m_bHaveChanged = false;
	
	return bHaveSave;
}



/********************************************************
*������: OpenFileToHDC
*
*��������:���ļ�
*
*����:
lpszFileName  Ҫ������ļ���
*
*����ֵ:�ɹ�����true, ʧ�ܷ���false
********************************************************/
bool CJellImage::OpenFileToHDC(char *lpszFileName)
{
	ClearMemory();
	
	CxImage image;
	
	image.Load(lpszFileName, TransFormFileTypeToCXImageDword(GetFileExt(lpszFileName)));
	
	if (!image.IsValid())
	{
		m_strLastError = "�޷����ļ�";

		return false;
	}

	m_lWidth = image.GetWidth();

	m_lHeight = image.GetHeight();
	
	HDC hdcScreen = CreateDC("DISPLAY", NULL, NULL, NULL); 

	BitmapStruct *pBitmap = new BitmapStruct;

	pBitmap->hdc = CreateCompatibleDC(hdcScreen);
	
	pBitmap->bitmap = CreateCompatibleBitmap(hdcScreen, m_lWidth, m_lHeight);
	
	SelectObject(pBitmap->hdc, pBitmap->bitmap);

	PatBlt(pBitmap->hdc, 0, 0, m_lWidth, m_lHeight, WHITENESS);

	image.Draw(pBitmap->hdc, 0, 0, m_lWidth, m_lHeight);

	m_stack_current.push(pBitmap);

	m_strFileName = lpszFileName;

	m_FileType = GetFileExt(lpszFileName);

	m_bIsVaild = true;

	m_bHaveChanged = false;

	DeleteDC(hdcScreen);
	
	return true;
}
//-----------------------------------------------------------------------------------------


/********************************************************
*������: GrayDeal
*
*��������:���캯��
*
*����:  
nGrade ��ʾ�ҶȺ���������, -255 ��255֮��
*
*����ֵ:�ɹ�����true;ʧ�ܷ���false;
********************************************************/
bool CJellImage::GrayDeal(int nGrade)
{
	if (!m_bIsVaild)
	{
		m_strLastError = "ͼƬ������,���ܽ��д���";

		return false;
	}
	else
	{
		BitmapStruct *pBitmap = new BitmapStruct;

		HDC hdcScreen = CreateDC("DISPLAY", NULL, NULL, NULL);
		
		pBitmap->hdc = CreateCompatibleDC(hdcScreen);
		
		pBitmap->bitmap = CreateCompatibleBitmap(hdcScreen, m_lWidth, m_lHeight);
		
		SelectObject(pBitmap->hdc, pBitmap->bitmap);
		
		BitBlt(pBitmap->hdc, 0, 0, m_lWidth, m_lHeight, m_stack_current.top()->hdc, 0, 0, SRCCOPY);////

		HDC mdc1 = CreateCompatibleDC(hdcScreen);

		HDC mdc2 = CreateCompatibleDC(hdcScreen);

		HDC mdc3 = CreateCompatibleDC(hdcScreen);

		HBITMAP bitmap1 = CreateCompatibleBitmap(hdcScreen, m_lWidth, m_lHeight);

		HBITMAP bitmap2 = CreateCompatibleBitmap(hdcScreen, m_lWidth, m_lHeight);

		HBITMAP bitmap3 = CreateCompatibleBitmap(hdcScreen, m_lWidth, m_lHeight);

		SelectObject(mdc1, bitmap1);

		SelectObject(mdc2, bitmap2);

		SelectObject(mdc3, bitmap3);

		BitBlt(mdc1, 0, 0, m_lWidth, m_lHeight, pBitmap->hdc, 0, 0, SRCCOPY);

		SetBlackAndWhite(mdc2, mdc3, m_lWidth, m_lHeight);

		//------------------------------------------------------------------------
		
		BITMAP bm;

		GetObject(bitmap1, sizeof(BITMAP), &bm);

		BYTE *pData = new BYTE[bm.bmWidthBytes * bm.bmHeight];

		GetBitmapBits(bitmap1,  bm.bmWidthBytes * bm.bmHeight, pData);

		int bits = bm.bmWidthBytes / bm.bmWidth;
		

		for (int i = m_Shape.lMinY; i < m_Shape.lMaxY; i++)
		{
			
			for (int k = m_Shape.lMinX * bits; k < m_Shape.lMaxX * bits; k += bits)
			{
				BYTE Gray = pData[i * bm.bmWidthBytes + k] * 0.3 + pData[i * bm.bmWidthBytes + k + 1] * 0.59 + pData[i * bm.bmWidthBytes + k + 2] * 0.11;

				

				if (Gray + nGrade > 255)
				{
					Gray = 255;
				}
				else
				{
					if (Gray + nGrade < 0)
					{
						Gray = 0;
					}
					else
					{
						Gray += nGrade;
					}
				}
				

				pData[i * bm.bmWidthBytes + k]  = Gray;
				pData[i * bm.bmWidthBytes + k + 1]  = Gray;
				pData[i * bm.bmWidthBytes + k + 2]  = Gray;
			}
		}

		SetBitmapBits(bitmap1,  bm.bmWidthBytes * bm.bmHeight, pData);



		delete []pData;

		BitBlt(mdc1, 0, 0, m_lWidth, m_lHeight, mdc3, 0, 0, SRCAND);

		BitBlt(pBitmap->hdc, 0, 0, m_lWidth, m_lHeight, mdc2, 0, 0, SRCAND);

		BitBlt(pBitmap->hdc, 0, 0, m_lWidth, m_lHeight, mdc1, 0, 0, SRCPAINT);

  
		//---------------------------------------------------------------------------
		
		m_stack_current.push(pBitmap);
		
		m_bHaveChanged = true;

		DeleteDC(hdcScreen);

		DeleteDC(mdc1);

		DeleteDC(mdc2);

		DeleteDC(mdc3);

		DeleteObject(bitmap1);

		DeleteObject(bitmap2);

		DeleteObject(bitmap3);
	}

	return true;
}



/********************************************************
*������: DeleteTop
*
*��������:ɾ����ǰ�������״̬
*
*����:��
*
*����ֵ:�ɹ�����true, ʧ�ܷ���false
********************************************************/
bool CJellImage::DeleteTop()
{
	if (!IsVaild())
	{
		m_strLastError = "ͼƬ������";

		return false;
	}
	else
	{
		if (m_stack_current.size() <= 1)
		{
			m_strLastError = "�Ѿ������״̬��,�޷�ɾ��";

			return false;
		}
		else
		{
			m_stack_current.top()->ClearMemory();

			delete m_stack_current.top();

			m_stack_current.pop();
		}
	}

	return true;
}




/********************************************************
*������: SetBlackAndWhite
*
*��������:�����ڰ�ģ��
*
*����:
BlackDC  ѡ��������ɫ���豸
WhiteDC  ѡ��������ɫ���豸
lWidth   ͼƬ�Ŀ�
lHeight  ͼƬ�ĸ�
*
*����ֵ:��
********************************************************/
void CJellImage::SetBlackAndWhite(HDC BlackDC, HDC WhiteDC, long lWidth, long lHeight)
{
	PatBlt(BlackDC, 0, 0, m_lWidth, m_lHeight, WHITENESS);

	PatBlt(WhiteDC, 0, 0, m_lWidth, m_lHeight, BLACKNESS);

	switch(m_Shape.m_SelectType)
	{
	case ALL:
		{
			PatBlt(BlackDC, 0, 0, m_lWidth, m_lHeight, BLACKNESS);

			PatBlt(WhiteDC, 0, 0, m_lWidth, m_lHeight, WHITENESS);

			m_Shape.lMinX = 0;

			m_Shape.lMinY = 0;

			m_Shape.lMaxX = lWidth;

			m_Shape.lMaxY = lHeight;

			break;
		}

	case BOX:
		{
			HPEN pen, oldPen;

			HBRUSH brush, oldBrush;

			brush = CreateSolidBrush(RGB(0, 0, 0));

			pen = CreatePen(PS_SOLID, 1, RGB(0, 0, 0));

			oldBrush = (HBRUSH)SelectObject(BlackDC, brush);

			oldPen = (HPEN)SelectObject(BlackDC, pen);

			Rectangle(BlackDC, m_Shape.m_vecP.at(0).x, m_Shape.m_vecP.at(0).y, m_Shape.m_vecP.at(1).x, m_Shape.m_vecP.at(1).y);

			SelectObject(BlackDC, oldPen);

			SelectObject(BlackDC, oldBrush);

			DeleteObject(brush);

			DeleteObject(oldBrush);

			DeleteObject(pen);

			DeleteObject(oldPen);




			brush = CreateSolidBrush(RGB(255, 255, 255));

			pen = CreatePen(PS_SOLID, 1, RGB(255, 255, 255));

			oldBrush = (HBRUSH)SelectObject(WhiteDC, brush);

			oldPen = (HPEN)SelectObject(WhiteDC, pen);

			Rectangle(WhiteDC, m_Shape.m_vecP.at(0).x, m_Shape.m_vecP.at(0).y, m_Shape.m_vecP.at(1).x, m_Shape.m_vecP.at(1).y);

			SelectObject(WhiteDC, oldPen);

			SelectObject(WhiteDC, oldBrush);

			DeleteObject(brush);

			DeleteObject(oldBrush);

			DeleteObject(pen);

			DeleteObject(oldPen);


			break;
		}

	case ELLIPSE:
		{
			HPEN pen, oldPen;

			HBRUSH brush, oldBrush;

			brush = CreateSolidBrush(RGB(0, 0, 0));

			pen = CreatePen(PS_SOLID, 1, RGB(0, 0, 0));

			oldBrush = (HBRUSH)SelectObject(BlackDC, brush);

			oldPen = (HPEN)SelectObject(BlackDC, pen);

			Ellipse(BlackDC, m_Shape.m_vecP.at(0).x, m_Shape.m_vecP.at(0).y, m_Shape.m_vecP.at(1).x, m_Shape.m_vecP.at(1).y);

			SelectObject(BlackDC, oldPen);

			SelectObject(BlackDC, oldBrush);

			DeleteObject(brush);

			DeleteObject(oldBrush);

			DeleteObject(pen);

			DeleteObject(oldPen);




			brush = CreateSolidBrush(RGB(255, 255, 255));

			pen = CreatePen(PS_SOLID, 1, RGB(255, 255, 255));

			oldBrush = (HBRUSH)SelectObject(WhiteDC, brush);

			oldPen = (HPEN)SelectObject(WhiteDC, pen);

			Ellipse(WhiteDC, m_Shape.m_vecP.at(0).x, m_Shape.m_vecP.at(0).y, m_Shape.m_vecP.at(1).x, m_Shape.m_vecP.at(1).y);

			SelectObject(WhiteDC, oldPen);

			SelectObject(WhiteDC, oldBrush);

			DeleteObject(brush);

			DeleteObject(oldBrush);

			DeleteObject(pen);

			DeleteObject(oldPen);

			break;
		}

	case POLYGON:
		{
			int n = m_Shape.m_vecP.size();

			POINT *p = new POINT[n];

			for (int i = 0; i < n; i++)
			{
				p[i] = m_Shape.m_vecP.at(i);
			}

			HPEN pen, oldPen;

			HBRUSH brush, oldBrush;

			brush = CreateSolidBrush(RGB(0, 0, 0));

			pen = CreatePen(PS_SOLID, 1, RGB(0, 0, 0));

			oldBrush = (HBRUSH)SelectObject(BlackDC, brush);

			oldPen = (HPEN)SelectObject(BlackDC, pen);

			Polygon(BlackDC, p, n);

			SelectObject(BlackDC, oldPen);

			SelectObject(BlackDC, oldBrush);

			DeleteObject(brush);

			DeleteObject(oldBrush);

			DeleteObject(pen);

			DeleteObject(oldPen);


			brush = CreateSolidBrush(RGB(255, 255, 255));

			pen = CreatePen(PS_SOLID, 1, RGB(255, 255, 255));

			oldBrush = (HBRUSH)SelectObject(WhiteDC, brush);

			oldPen = (HPEN)SelectObject(WhiteDC, pen);

			Polygon(WhiteDC, p, n);

			SelectObject(WhiteDC, oldPen);

			SelectObject(WhiteDC, oldBrush);

			DeleteObject(brush);

			DeleteObject(oldBrush);

			DeleteObject(pen);

			DeleteObject(oldPen);

			delete []p;

			break;
		}

	case LINE:
		{
			int n = m_Shape.m_vecP.size();

			POINT *p = new POINT[n];

			for (int i = 0; i < n; i++)
			{
				p[i] = m_Shape.m_vecP.at(i);
			}

			HPEN pen, oldPen;

			HBRUSH brush, oldBrush;

			brush = CreateSolidBrush(RGB(0, 0, 0));

			pen = CreatePen(PS_SOLID, 1, RGB(0, 0, 0));

			oldBrush = (HBRUSH)SelectObject(BlackDC, brush);

			oldPen = (HPEN)SelectObject(BlackDC, pen);

			Polygon(BlackDC, p, n);

			SelectObject(BlackDC, oldPen);

			SelectObject(BlackDC, oldBrush);

			DeleteObject(brush);

			DeleteObject(oldBrush);

			DeleteObject(pen);

			DeleteObject(oldPen);


			brush = CreateSolidBrush(RGB(255, 255, 255));

			pen = CreatePen(PS_SOLID, 1, RGB(255, 255, 255));

			oldBrush = (HBRUSH)SelectObject(WhiteDC, brush);

			oldPen = (HPEN)SelectObject(WhiteDC, pen);

			Polygon(WhiteDC, p, n);

			SelectObject(WhiteDC, oldPen);

			SelectObject(WhiteDC, oldBrush);

			DeleteObject(brush);

			DeleteObject(oldBrush);

			DeleteObject(pen);

			DeleteObject(oldPen);

			delete []p;

			break;
		}
	}
}




/********************************************************
*������: ReverseColorDeal
*
*��������:��ɫ����
*
*����:  ��
*
*����ֵ:�ɹ�����true;ʧ�ܷ���false;
********************************************************/
bool CJellImage::ReverseColorDeal()
{
	if (!m_bIsVaild)
	{
		m_strLastError = "ͼƬ������,���ܽ��д���";

		return false;
	}
	else
	{
		BitmapStruct *pBitmap = new BitmapStruct;

		HDC hdcScreen = CreateDC("DISPLAY", NULL, NULL, NULL);
		
		pBitmap->hdc = CreateCompatibleDC(hdcScreen);
		
		pBitmap->bitmap = CreateCompatibleBitmap(hdcScreen, m_lWidth, m_lHeight);
		
		SelectObject(pBitmap->hdc, pBitmap->bitmap);
		
		BitBlt(pBitmap->hdc, 0, 0, m_lWidth, m_lHeight, m_stack_current.top()->hdc, 0, 0, SRCCOPY);////

		HDC mdc1 = CreateCompatibleDC(hdcScreen);

		HDC mdc2 = CreateCompatibleDC(hdcScreen);

		HDC mdc3 = CreateCompatibleDC(hdcScreen);

		HBITMAP bitmap1 = CreateCompatibleBitmap(hdcScreen, m_lWidth, m_lHeight);

		HBITMAP bitmap2 = CreateCompatibleBitmap(hdcScreen, m_lWidth, m_lHeight);

		HBITMAP bitmap3 = CreateCompatibleBitmap(hdcScreen, m_lWidth, m_lHeight);

		SelectObject(mdc1, bitmap1);

		SelectObject(mdc2, bitmap2);

		SelectObject(mdc3, bitmap3);

		BitBlt(mdc1, 0, 0, m_lWidth, m_lHeight, pBitmap->hdc, 0, 0, SRCCOPY);

		SetBlackAndWhite(mdc2, mdc3, m_lWidth, m_lHeight);

		//------------------------------------------------------------------------
		
		BITMAP bm;

		GetObject(bitmap1, sizeof(BITMAP), &bm);

		BYTE *pData = new BYTE[bm.bmWidthBytes * bm.bmHeight];

		GetBitmapBits(bitmap1,  bm.bmWidthBytes * bm.bmHeight, pData);

		int bits = bm.bmWidthBytes / bm.bmWidth;
		

		for (int i = m_Shape.lMinY; i < m_Shape.lMaxY; i++)
		{
			for (int k = m_Shape.lMinX * bits; k < m_Shape.lMaxX * bits; k += bits)
			{
				pData[i * bm.bmWidthBytes + k]  = 255 - pData[i * bm.bmWidthBytes + k];
				pData[i * bm.bmWidthBytes + k + 1]  = 255 - pData[i * bm.bmWidthBytes + k + 1];
				pData[i * bm.bmWidthBytes + k + 2]  = 255 - pData[i * bm.bmWidthBytes + k + 2];
			}
		}

		SetBitmapBits(bitmap1,  bm.bmWidthBytes * bm.bmHeight, pData);



		delete []pData;

		BitBlt(mdc1, 0, 0, m_lWidth, m_lHeight, mdc3, 0, 0, SRCAND);

		BitBlt(pBitmap->hdc, 0, 0, m_lWidth, m_lHeight, mdc2, 0, 0, SRCAND);

		BitBlt(pBitmap->hdc, 0, 0, m_lWidth, m_lHeight, mdc1, 0, 0, SRCPAINT);

  
		//---------------------------------------------------------------------------
		
		m_stack_current.push(pBitmap);
		
		m_bHaveChanged = true;

		DeleteDC(hdcScreen);

		DeleteDC(mdc1);

		DeleteDC(mdc2);

		DeleteDC(mdc3);

		DeleteObject(bitmap1);

		DeleteObject(bitmap2);

		DeleteObject(bitmap3);
	}

	return true;
}


/********************************************************
*������: LightShadeDeal
*
*��������:��������
*
*����:  nValue Ҫ��ǿ��������,��-255��255֮��
*
*����ֵ:�ɹ�����true;ʧ�ܷ���false;
********************************************************/
bool CJellImage::LightShadeDeal(int nValue)
{
	if (!m_bIsVaild)
	{
		m_strLastError = "ͼƬ������,���ܽ��д���";

		return false;
	}
	else
	{
		BitmapStruct *pBitmap = new BitmapStruct;

		HDC hdcScreen = CreateDC("DISPLAY", NULL, NULL, NULL);
		
		pBitmap->hdc = CreateCompatibleDC(hdcScreen);
		
		pBitmap->bitmap = CreateCompatibleBitmap(hdcScreen, m_lWidth, m_lHeight);
		
		SelectObject(pBitmap->hdc, pBitmap->bitmap);
		
		BitBlt(pBitmap->hdc, 0, 0, m_lWidth, m_lHeight, m_stack_current.top()->hdc, 0, 0, SRCCOPY);////

		HDC mdc1 = CreateCompatibleDC(hdcScreen);

		HDC mdc2 = CreateCompatibleDC(hdcScreen);

		HDC mdc3 = CreateCompatibleDC(hdcScreen);

		HBITMAP bitmap1 = CreateCompatibleBitmap(hdcScreen, m_lWidth, m_lHeight);

		HBITMAP bitmap2 = CreateCompatibleBitmap(hdcScreen, m_lWidth, m_lHeight);

		HBITMAP bitmap3 = CreateCompatibleBitmap(hdcScreen, m_lWidth, m_lHeight);

		SelectObject(mdc1, bitmap1);

		SelectObject(mdc2, bitmap2);

		SelectObject(mdc3, bitmap3);

		BitBlt(mdc1, 0, 0, m_lWidth, m_lHeight, pBitmap->hdc, 0, 0, SRCCOPY);

		SetBlackAndWhite(mdc2, mdc3, m_lWidth, m_lHeight);

		//------------------------------------------------------------------------
		
		BITMAP bm;

		GetObject(bitmap1, sizeof(BITMAP), &bm);

		BYTE *pData = new BYTE[bm.bmWidthBytes * bm.bmHeight];

		GetBitmapBits(bitmap1,  bm.bmWidthBytes * bm.bmHeight, pData);

		int bits = bm.bmWidthBytes / bm.bmWidth;
		

		for (int i = m_Shape.lMinY; i < m_Shape.lMaxY; i++)
		{
			for (int k = m_Shape.lMinX * bits; k < m_Shape.lMaxX * bits; k += bits)
			{
				int newValue;
					

				newValue = pData[i * bm.bmWidthBytes + k] + nValue;

				if (newValue > 255)
				{
					newValue = 255;
				}
				else
				{
					if (newValue < 0)
					{
						newValue = 0;
					}
				}
				pData[i * bm.bmWidthBytes + k]  = newValue;



				newValue = pData[i * bm.bmWidthBytes + k+ 1] + nValue;

				if (newValue > 255)
				{
					newValue = 255;
				}
				else
				{
					if (newValue < 0)
					{
						newValue = 0;
					}
				}
				pData[i * bm.bmWidthBytes + k + 1]  = newValue;



				newValue = pData[i * bm.bmWidthBytes + k + 2] + nValue;

				if (newValue > 255)
				{
					newValue = 255;
				}
				else
				{
					if (newValue < 0)
					{
						newValue = 0;
					}
				}
				pData[i * bm.bmWidthBytes + k + 2]  = newValue;
				
			}
		}

		SetBitmapBits(bitmap1,  bm.bmWidthBytes * bm.bmHeight, pData);



		delete []pData;

		BitBlt(mdc1, 0, 0, m_lWidth, m_lHeight, mdc3, 0, 0, SRCAND);

		BitBlt(pBitmap->hdc, 0, 0, m_lWidth, m_lHeight, mdc2, 0, 0, SRCAND);

		BitBlt(pBitmap->hdc, 0, 0, m_lWidth, m_lHeight, mdc1, 0, 0, SRCPAINT);

  
		//---------------------------------------------------------------------------
		
		m_stack_current.push(pBitmap);
		
		m_bHaveChanged = true;

		DeleteDC(hdcScreen);

		DeleteDC(mdc1);

		DeleteDC(mdc2);

		DeleteDC(mdc3);

		DeleteObject(bitmap1);

		DeleteObject(bitmap2);

		DeleteObject(bitmap3);
	}

	return true;
}




/********************************************************
*������: RGBDeal
*
*��������:RGB��ɫ����
*
*����:  
nR Ҫ������Rֵ,��-255��255֮��
nG Ҫ������Gֵ,��-255��255֮��
nB Ҫ������Bֵ,��-255��255֮��
*
*����ֵ:�ɹ�����true;ʧ�ܷ���false;
********************************************************/
bool CJellImage::RGBDeal(int nR, int nG, int nB)
{
	if (!m_bIsVaild)
	{
		m_strLastError = "ͼƬ������,���ܽ��д���";

		return false;
	}
	else
	{
		BitmapStruct *pBitmap = new BitmapStruct;

		HDC hdcScreen = CreateDC("DISPLAY", NULL, NULL, NULL);
		
		pBitmap->hdc = CreateCompatibleDC(hdcScreen);
		
		pBitmap->bitmap = CreateCompatibleBitmap(hdcScreen, m_lWidth, m_lHeight);
		
		SelectObject(pBitmap->hdc, pBitmap->bitmap);
		
		BitBlt(pBitmap->hdc, 0, 0, m_lWidth, m_lHeight, m_stack_current.top()->hdc, 0, 0, SRCCOPY);////

		HDC mdc1 = CreateCompatibleDC(hdcScreen);

		HDC mdc2 = CreateCompatibleDC(hdcScreen);

		HDC mdc3 = CreateCompatibleDC(hdcScreen);

		HBITMAP bitmap1 = CreateCompatibleBitmap(hdcScreen, m_lWidth, m_lHeight);

		HBITMAP bitmap2 = CreateCompatibleBitmap(hdcScreen, m_lWidth, m_lHeight);

		HBITMAP bitmap3 = CreateCompatibleBitmap(hdcScreen, m_lWidth, m_lHeight);

		SelectObject(mdc1, bitmap1);

		SelectObject(mdc2, bitmap2);

		SelectObject(mdc3, bitmap3);

		BitBlt(mdc1, 0, 0, m_lWidth, m_lHeight, pBitmap->hdc, 0, 0, SRCCOPY);

		SetBlackAndWhite(mdc2, mdc3, m_lWidth, m_lHeight);

		//------------------------------------------------------------------------
		
		BITMAP bm;

		GetObject(bitmap1, sizeof(BITMAP), &bm);

		BYTE *pData = new BYTE[bm.bmWidthBytes * bm.bmHeight];

		GetBitmapBits(bitmap1,  bm.bmWidthBytes * bm.bmHeight, pData);

		int bits = bm.bmWidthBytes / bm.bmWidth;
		

		for (int i = m_Shape.lMinY; i < m_Shape.lMaxY; i++)
		{
			for (int k = m_Shape.lMinX * bits; k < m_Shape.lMaxX * bits; k += bits)
			{
				int newValue;

				newValue = pData[i * bm.bmWidthBytes + k] + nB;

				if (newValue > 255)
				{
					newValue = 255;
				}
				else
				{
					if (newValue < 0)
					{
						newValue = 0;
					}
				}
				pData[i * bm.bmWidthBytes + k]  = newValue;



				newValue = pData[i * bm.bmWidthBytes + k+ 1] + nG;

				if (newValue > 255)
				{
					newValue = 255;
				}
				else
				{
					if (newValue < 0)
					{
						newValue = 0;
					}
				}
				pData[i * bm.bmWidthBytes + k + 1]  = newValue;


				newValue = pData[i * bm.bmWidthBytes + k + 2] + nR;

				if (newValue > 255)
				{
					newValue = 255;
				}
				else
				{
					if (newValue < 0)
					{
						newValue = 0;
					}
				}
				pData[i * bm.bmWidthBytes + k + 2]  = newValue;
				
			}
		}

		SetBitmapBits(bitmap1,  bm.bmWidthBytes * bm.bmHeight, pData);



		delete []pData;

		BitBlt(mdc1, 0, 0, m_lWidth, m_lHeight, mdc3, 0, 0, SRCAND);

		BitBlt(pBitmap->hdc, 0, 0, m_lWidth, m_lHeight, mdc2, 0, 0, SRCAND);

		BitBlt(pBitmap->hdc, 0, 0, m_lWidth, m_lHeight, mdc1, 0, 0, SRCPAINT);

  
		//---------------------------------------------------------------------------
		
		m_stack_current.push(pBitmap);
		
		m_bHaveChanged = true;

		DeleteDC(hdcScreen);

		DeleteDC(mdc1);

		DeleteDC(mdc2);

		DeleteDC(mdc3);

		DeleteObject(bitmap1);

		DeleteObject(bitmap2);

		DeleteObject(bitmap3);
	}

	return true;
}




/********************************************************
*������: LineDeal
*
*��������:���ߴ���
*
*����:  
nZ  �Ƕ�
nLineWidth �߿�
nSpaceWidth �հ׼��
colLineColor �ߵ���ɫ
psLine �ߵ�����
nAlphaValue ͸����
*
*����ֵ:�ɹ�����true;ʧ�ܷ���false;
********************************************************/
bool CJellImage::LineDeal(int nZ, int nLineWidth, int nSpaceWidth, COLORREF colLineColor, int psLine, int nAlphaValue)
{
	if (!m_bIsVaild)
	{
		m_strLastError = "ͼƬ������,���ܽ��д���";

		return false;
	}
	else
	{
		BitmapStruct *pBitmap = new BitmapStruct;

		HDC hdcScreen = CreateDC("DISPLAY", NULL, NULL, NULL);
		
		pBitmap->hdc = CreateCompatibleDC(hdcScreen);
		
		pBitmap->bitmap = CreateCompatibleBitmap(hdcScreen, m_lWidth, m_lHeight);
		
		SelectObject(pBitmap->hdc, pBitmap->bitmap);
		
		BitBlt(pBitmap->hdc, 0, 0, m_lWidth, m_lHeight, m_stack_current.top()->hdc, 0, 0, SRCCOPY);////

		HDC mdc1 = CreateCompatibleDC(hdcScreen);

		HDC mdc2 = CreateCompatibleDC(hdcScreen);

		HDC mdc3 = CreateCompatibleDC(hdcScreen);

		HBITMAP bitmap1 = CreateCompatibleBitmap(hdcScreen, m_lWidth, m_lHeight);

		HBITMAP bitmap2 = CreateCompatibleBitmap(hdcScreen, m_lWidth, m_lHeight);

		HBITMAP bitmap3 = CreateCompatibleBitmap(hdcScreen, m_lWidth, m_lHeight);

		SelectObject(mdc1, bitmap1);

		SelectObject(mdc2, bitmap2);

		SelectObject(mdc3, bitmap3);

		BitBlt(mdc1, 0, 0, m_lWidth, m_lHeight, pBitmap->hdc, 0, 0, SRCCOPY);

		SetBlackAndWhite(mdc2, mdc3, m_lWidth, m_lHeight);

		//------------------------------------------------------------------------
		
		int nWidth = m_Shape.lMaxX - m_Shape.lMinX;

		int nHeight = m_Shape.lMaxY - m_Shape.lMinY;

		int nPerSpace = nSpaceWidth + nLineWidth;

		HDC tmpDC =  CreateCompatibleDC(hdcScreen);

		HBITMAP tmpBitmap = CreateCompatibleBitmap(hdcScreen, nWidth, nHeight);

		SelectObject(tmpDC, tmpBitmap);

		HPEN tmpPen = CreatePen(psLine, nLineWidth, colLineColor);

		SelectObject(tmpDC, tmpPen);

		SetBkMode(tmpDC, TRANSPARENT);

		BitBlt(tmpDC, 0, 0, nWidth, nHeight, mdc1, m_Shape.lMinX, m_Shape.lMinY, SRCCOPY);

		if (nZ == 0)
		{
			for (int i = 0; i < nHeight; i+=  nPerSpace)
			{
				MoveToEx(tmpDC, 0, i, NULL);

				LineTo(tmpDC, nWidth, i);
			}
		}
		else
		{
			if (nZ == 90)
			{
				for (int i = 0; i < nWidth; i += nPerSpace)
				{
					MoveToEx(tmpDC, i, 0, NULL);

					LineTo(tmpDC, i, nHeight);
				}
			}
		}

		BLENDFUNCTION b;

		b.BlendOp = AC_SRC_OVER; //��ֵ

		b.BlendFlags = 0; //��ֵ

		b.SourceConstantAlpha = nAlphaValue; //��������͸���ȣ���ÿһ���ص��Լ���alphaֵ

		b.AlphaFormat=0;

		AlphaBlend(mdc1, m_Shape.lMinX, m_Shape.lMinY, nWidth, nHeight, tmpDC, 0, 0, nWidth, nHeight, b);

		DeleteObject(tmpPen);

		DeleteObject(tmpBitmap);

		DeleteDC(tmpDC);
		

		BitBlt(mdc1, 0, 0, m_lWidth, m_lHeight, mdc3, 0, 0, SRCAND);

		BitBlt(pBitmap->hdc, 0, 0, m_lWidth, m_lHeight, mdc2, 0, 0, SRCAND);

		BitBlt(pBitmap->hdc, 0, 0, m_lWidth, m_lHeight, mdc1, 0, 0, SRCPAINT);

  
		//---------------------------------------------------------------------------
		
		m_stack_current.push(pBitmap);
		
		m_bHaveChanged = true;

		DeleteDC(hdcScreen);

		DeleteDC(mdc1);

		DeleteDC(mdc2);

		DeleteDC(mdc3);

		DeleteObject(bitmap1);

		DeleteObject(bitmap2);

		DeleteObject(bitmap3);
	}

	return true;
}





/********************************************************
*������: PointDeal
*
*��������:�㴦��
*
*����:  
nLeftRightSpace ���Ҽ��
nUpDownSpace ���¼��
colPointColor �����ɫ
nAlphaValue ͸����
*
*����ֵ:�ɹ�����true;ʧ�ܷ���false;
********************************************************/
bool CJellImage::PointDeal(int nLeftRightSpace, int nUpDownSpace, COLORREF colPointColor, int nAlphaValue)
{
	if (!m_bIsVaild)
	{
		m_strLastError = "ͼƬ������,���ܽ��д���";

		return false;
	}
	else
	{
		BitmapStruct *pBitmap = new BitmapStruct;

		HDC hdcScreen = CreateDC("DISPLAY", NULL, NULL, NULL);
		
		pBitmap->hdc = CreateCompatibleDC(hdcScreen);
		
		pBitmap->bitmap = CreateCompatibleBitmap(hdcScreen, m_lWidth, m_lHeight);
		
		SelectObject(pBitmap->hdc, pBitmap->bitmap);
		
		BitBlt(pBitmap->hdc, 0, 0, m_lWidth, m_lHeight, m_stack_current.top()->hdc, 0, 0, SRCCOPY);////

		HDC mdc1 = CreateCompatibleDC(hdcScreen);

		HDC mdc2 = CreateCompatibleDC(hdcScreen);

		HDC mdc3 = CreateCompatibleDC(hdcScreen);

		HBITMAP bitmap1 = CreateCompatibleBitmap(hdcScreen, m_lWidth, m_lHeight);

		HBITMAP bitmap2 = CreateCompatibleBitmap(hdcScreen, m_lWidth, m_lHeight);

		HBITMAP bitmap3 = CreateCompatibleBitmap(hdcScreen, m_lWidth, m_lHeight);

		SelectObject(mdc1, bitmap1);

		SelectObject(mdc2, bitmap2);

		SelectObject(mdc3, bitmap3);

		BitBlt(mdc1, 0, 0, m_lWidth, m_lHeight, pBitmap->hdc, 0, 0, SRCCOPY);

		SetBlackAndWhite(mdc2, mdc3, m_lWidth, m_lHeight);

		//------------------------------------------------------------------------
		
		int nWidth = m_Shape.lMaxX - m_Shape.lMinX;

		int nHeight = m_Shape.lMaxY - m_Shape.lMinY;

		HDC tmpDC =  CreateCompatibleDC(hdcScreen);

		HBITMAP tmpBitmap = CreateCompatibleBitmap(hdcScreen, nWidth, nHeight);

		SelectObject(tmpDC, tmpBitmap);

		BitBlt(tmpDC, 0, 0, nWidth, nHeight, mdc1, m_Shape.lMinX, m_Shape.lMinY, SRCCOPY);

		for (int i = 0; i < nHeight; i+= (nUpDownSpace + 1))
		{
			for (int k = 0; k < nWidth; k+= (nLeftRightSpace + 1))
			{
				SetPixel(tmpDC, k, i, colPointColor);
			}
		}

		BLENDFUNCTION b;

		b.BlendOp = AC_SRC_OVER; //��ֵ

		b.BlendFlags = 0; //��ֵ

		b.SourceConstantAlpha = nAlphaValue; //��������͸���ȣ���ÿһ���ص��Լ���alphaֵ

		b.AlphaFormat=0;

		AlphaBlend(mdc1, m_Shape.lMinX, m_Shape.lMinY, nWidth, nHeight, tmpDC, 0, 0, nWidth, nHeight, b);

		DeleteObject(tmpBitmap);

		DeleteDC(tmpDC);
		
		BitBlt(mdc1, 0, 0, m_lWidth, m_lHeight, mdc3, 0, 0, SRCAND);

		BitBlt(pBitmap->hdc, 0, 0, m_lWidth, m_lHeight, mdc2, 0, 0, SRCAND);

		BitBlt(pBitmap->hdc, 0, 0, m_lWidth, m_lHeight, mdc1, 0, 0, SRCPAINT);

  
		//---------------------------------------------------------------------------
		
		m_stack_current.push(pBitmap);
		
		m_bHaveChanged = true;

		DeleteDC(hdcScreen);

		DeleteDC(mdc1);

		DeleteDC(mdc2);

		DeleteDC(mdc3);

		DeleteObject(bitmap1);

		DeleteObject(bitmap2);

		DeleteObject(bitmap3);
	}

	return true;
}




/********************************************************
*������: GetFileExt
*
*��������:�õ��ļ���ꡣ�Ȼ��ת����FILETYPE��
*
*����:
lpszFileName   �ļ���
*
*����ֵ:��
********************************************************/
CJellImage::FILETYPE CJellImage::GetFileExt(char *lpszFileName)
{
	char szExt[_MAX_EXT];
	
	_splitpath(lpszFileName, NULL, NULL, NULL, szExt);
	
	strcpy(szExt, strlwr(szExt));//�ĳ�Сд��
	
	FILETYPE FileType;
	
	if (strcmp(szExt, ".bmp") == 0)
	{
		FileType = BMP;
	}
	else
	{
		if (strcmp(szExt, ".gif") == 0)
		{
			FileType = GIF;
		}
		else
		{
			if (strcmp(szExt, ".jpg") == 0)
			{
				FileType = JPG;
			}
			else
			{
				if (strcmp(szExt, ".jpeg") == 0)
				{
					FileType = JPEG;
				}
				else
				{
					if (strcmp(szExt, ".png") == 0)
					{
						FileType = PNG;
					}
					else
					{
						FileType = UNKNOW;
					}
				}
			}
		}
	}
	
	
	return FileType;
}





/********************************************************
*������: ColorFilterDeal
*
*��������:��ɫ����
*
*����:  bR �Ƿ�Ҫ��Rɫ����,ֻҪRֵ��nMinR��nMaxR֮�䣬R�͹���
bG �Ƿ�Ҫ��Gɫ����,ֻҪGֵ��nMinG��nMaxG֮�䣬G�͹���
bB �Ƿ�Ҫ��Bɫ����,ֻҪBֵ��nMinB��nMaxB֮�䣬B�͹���
bGray �Ƿ�Ҫ�ѹ��˺����ɫ��Ϊ��ɫ, nGrayValueΪ�Ҷ�ֵ
colFillColor ���ڲ��ѹ��˺����ɫ���Ϊһ����ɫ����ɫ
*
*����ֵ:�ɹ�����true;ʧ�ܷ���false;
********************************************************/
bool CJellImage::ColorFilterDeal(bool bR, int nMinR, int nMaxR,
								 bool bG, int nMinG, int nMaxG,
								 bool bB, int nMinB, int nMaxB,
								 bool bGray, int nGrayValue,
								 COLORREF colFillColor
								 )
{
	if (!m_bIsVaild)
	{
		m_strLastError = "ͼƬ������,���ܽ��д���";

		return false;
	}
	else
	{
		BitmapStruct *pBitmap = new BitmapStruct;

		HDC hdcScreen = CreateDC("DISPLAY", NULL, NULL, NULL);
		
		pBitmap->hdc = CreateCompatibleDC(hdcScreen);
		
		pBitmap->bitmap = CreateCompatibleBitmap(hdcScreen, m_lWidth, m_lHeight);
		
		SelectObject(pBitmap->hdc, pBitmap->bitmap);
		
		BitBlt(pBitmap->hdc, 0, 0, m_lWidth, m_lHeight, m_stack_current.top()->hdc, 0, 0, SRCCOPY);////

		HDC mdc1 = CreateCompatibleDC(hdcScreen);

		HDC mdc2 = CreateCompatibleDC(hdcScreen);

		HDC mdc3 = CreateCompatibleDC(hdcScreen);

		HBITMAP bitmap1 = CreateCompatibleBitmap(hdcScreen, m_lWidth, m_lHeight);

		HBITMAP bitmap2 = CreateCompatibleBitmap(hdcScreen, m_lWidth, m_lHeight);

		HBITMAP bitmap3 = CreateCompatibleBitmap(hdcScreen, m_lWidth, m_lHeight);

		SelectObject(mdc1, bitmap1);

		SelectObject(mdc2, bitmap2);

		SelectObject(mdc3, bitmap3);

		BitBlt(mdc1, 0, 0, m_lWidth, m_lHeight, pBitmap->hdc, 0, 0, SRCCOPY);

		SetBlackAndWhite(mdc2, mdc3, m_lWidth, m_lHeight);

		//------------------------------------------------------------------------
		int nRGBCount = 0;
		
		if (bR)
		{
			nRGBCount++;
		}
		
		if (bG)
		{
			nRGBCount++;
		}
		
		if (bB)
		{
			nRGBCount++;
		}
		
		if (nRGBCount > 0)
		{
			BITMAP bm;
			
			GetObject(bitmap1, sizeof(BITMAP), &bm);
			
			BYTE *pData = new BYTE[bm.bmWidthBytes * bm.bmHeight];
			
			GetBitmapBits(bitmap1,  bm.bmWidthBytes * bm.bmHeight, pData);
			
			int bits = bm.bmWidthBytes / bm.bmWidth;
			
			for (int i = m_Shape.lMinY; i < m_Shape.lMaxY; i++)
			{
				for (int k = m_Shape.lMinX * bits; k < m_Shape.lMaxX * bits; k += bits)
				{
					int nGoodCount = 0;

					BYTE newValue;
					
					newValue = pData[i * bm.bmWidthBytes + k];

					if (bB)
					{
						if (nMinB <= newValue && newValue <= nMaxB)
						{
							nGoodCount++;
						}
					}

					newValue = pData[i * bm.bmWidthBytes + k + 1];

					if (bG)
					{
						if (nMinG <= newValue && newValue <= nMaxG)
						{
							nGoodCount++;
						}
					}


					newValue = pData[i * bm.bmWidthBytes + k + 2];

					if (bR)
					{
						if (nMinR <= newValue && newValue <= nMaxR)
						{
							nGoodCount++;
						}
					}

					if (nGoodCount != nRGBCount)
					{
						if (bGray)
						{
							BYTE Gray = pData[i * bm.bmWidthBytes + k] * 0.3 + pData[i * bm.bmWidthBytes + k + 1] * 0.59 + pData[i * bm.bmWidthBytes + k + 2] * 0.11;
							
							if (Gray + nGrayValue > 255)
							{
								Gray = 255;
							}
							else
							{
								if (Gray + nGrayValue < 0)
								{
									Gray = 0;
								}
								else
								{
									Gray += nGrayValue;
								}
							}
							
							
							pData[i * bm.bmWidthBytes + k]  = Gray;
							pData[i * bm.bmWidthBytes + k + 1]  = Gray;
							pData[i * bm.bmWidthBytes + k + 2]  = Gray;
						}
						else
						{
							pData[i * bm.bmWidthBytes + k]  = GetBValue(colFillColor);
							pData[i * bm.bmWidthBytes + k + 1]  = GetGValue(colFillColor);
							pData[i * bm.bmWidthBytes + k + 2]  = GetRValue(colFillColor);
						}
					}
				}
			}
			
			SetBitmapBits(bitmap1,  bm.bmWidthBytes * bm.bmHeight, pData);
			
			delete []pData;
		}

		BitBlt(mdc1, 0, 0, m_lWidth, m_lHeight, mdc3, 0, 0, SRCAND);

		BitBlt(pBitmap->hdc, 0, 0, m_lWidth, m_lHeight, mdc2, 0, 0, SRCAND);

		BitBlt(pBitmap->hdc, 0, 0, m_lWidth, m_lHeight, mdc1, 0, 0, SRCPAINT);

  
		//---------------------------------------------------------------------------
		
		m_stack_current.push(pBitmap);
		
		m_bHaveChanged = true;

		DeleteDC(hdcScreen);

		DeleteDC(mdc1);

		DeleteDC(mdc2);

		DeleteDC(mdc3);

		DeleteObject(bitmap1);

		DeleteObject(bitmap2);

		DeleteObject(bitmap3);
	}

	return true;
}



/********************************************************
*������: SingleColorFillDeal
*
*��������:��ɫ���
*
*����:  
colFillColor ������ɫ
nAlphaBlend  ��͸����
*
*����ֵ:�ɹ�����true;ʧ�ܷ���false;
********************************************************/
bool CJellImage::SingleColorFillDeal(COLORREF colFillColor, int nAlphaBlend)
{
	if (!m_bIsVaild)
	{
		m_strLastError = "ͼƬ������,���ܽ��д���";

		return false;
	}
	else
	{
		BitmapStruct *pBitmap = new BitmapStruct;

		HDC hdcScreen = CreateDC("DISPLAY", NULL, NULL, NULL);
		
		pBitmap->hdc = CreateCompatibleDC(hdcScreen);
		
		pBitmap->bitmap = CreateCompatibleBitmap(hdcScreen, m_lWidth, m_lHeight);
		
		SelectObject(pBitmap->hdc, pBitmap->bitmap);
		
		BitBlt(pBitmap->hdc, 0, 0, m_lWidth, m_lHeight, m_stack_current.top()->hdc, 0, 0, SRCCOPY);////

		HDC mdc1 = CreateCompatibleDC(hdcScreen);

		HDC mdc2 = CreateCompatibleDC(hdcScreen);

		HDC mdc3 = CreateCompatibleDC(hdcScreen);

		HBITMAP bitmap1 = CreateCompatibleBitmap(hdcScreen, m_lWidth, m_lHeight);

		HBITMAP bitmap2 = CreateCompatibleBitmap(hdcScreen, m_lWidth, m_lHeight);

		HBITMAP bitmap3 = CreateCompatibleBitmap(hdcScreen, m_lWidth, m_lHeight);

		SelectObject(mdc1, bitmap1);

		SelectObject(mdc2, bitmap2);

		SelectObject(mdc3, bitmap3);

		BitBlt(mdc1, 0, 0, m_lWidth, m_lHeight, pBitmap->hdc, 0, 0, SRCCOPY);

		SetBlackAndWhite(mdc2, mdc3, m_lWidth, m_lHeight);

		//------------------------------------------------------------------------
		
		int nWidth = m_Shape.lMaxX - m_Shape.lMinX;

		int nHeight = m_Shape.lMaxY - m_Shape.lMinY;

		HDC tmpDC =  CreateCompatibleDC(hdcScreen);

		HBITMAP tmpBitmap = CreateCompatibleBitmap(hdcScreen, nWidth, nHeight);

		HBRUSH tmpBrush = CreateSolidBrush(colFillColor);

		SelectObject(tmpDC, tmpBitmap);

		RECT tmpRE = {0, 0, nWidth, nHeight};

		FillRect(tmpDC, &tmpRE, tmpBrush);

		BLENDFUNCTION b;

		b.BlendOp = AC_SRC_OVER; //��ֵ

		b.BlendFlags = 0; //��ֵ

		b.SourceConstantAlpha = nAlphaBlend; //��������͸���ȣ���ÿһ���ص��Լ���alphaֵ

		b.AlphaFormat=0;

		AlphaBlend(mdc1, m_Shape.lMinX, m_Shape.lMinY, nWidth, nHeight, tmpDC, 0, 0, nWidth, nHeight, b);

		DeleteObject(tmpBrush);

		DeleteObject(tmpBitmap);

		DeleteDC(tmpDC);
		
		BitBlt(mdc1, 0, 0, m_lWidth, m_lHeight, mdc3, 0, 0, SRCAND);

		BitBlt(pBitmap->hdc, 0, 0, m_lWidth, m_lHeight, mdc2, 0, 0, SRCAND);

		BitBlt(pBitmap->hdc, 0, 0, m_lWidth, m_lHeight, mdc1, 0, 0, SRCPAINT);

  
		//---------------------------------------------------------------------------
		
		m_stack_current.push(pBitmap);
		
		m_bHaveChanged = true;

		DeleteDC(hdcScreen);

		DeleteDC(mdc1);

		DeleteDC(mdc2);

		DeleteDC(mdc3);

		DeleteObject(bitmap1);

		DeleteObject(bitmap2);

		DeleteObject(bitmap3);
	}

	return true;
}




/********************************************************
*������: CopyBitmapToClipboard
*
*��������:���Ƶ�ǰѡ���ͼ���ڴ���
*
*����:  ��
*
*����ֵ:�ɹ�����true;ʧ�ܷ���false;
********************************************************/
bool CJellImage::CopyBitmapToClipboard()
{
	if (!m_bIsVaild)
	{
		m_strLastError = "ͼƬ������,���ܽ��и���";

		return false;
	}
	else
	{
		HDC hdcScreen = CreateDC("DISPLAY", NULL, NULL, NULL);

		HDC mdc1 = CreateCompatibleDC(hdcScreen);

		HDC mdc2 = CreateCompatibleDC(hdcScreen);

		HDC mdc3 = CreateCompatibleDC(hdcScreen);

		HBITMAP bitmap1 = CreateCompatibleBitmap(hdcScreen, m_lWidth, m_lHeight);

		HBITMAP bitmap2 = CreateCompatibleBitmap(hdcScreen, m_lWidth, m_lHeight);

		HBITMAP bitmap3 = CreateCompatibleBitmap(hdcScreen, m_lWidth, m_lHeight);

		SelectObject(mdc1, bitmap1);

		SelectObject(mdc2, bitmap2);

		SelectObject(mdc3, bitmap3);

		BitBlt(mdc1, 0, 0, m_lWidth, m_lHeight, m_stack_current.top()->hdc, 0, 0, SRCCOPY);

		SetBlackAndWhite(mdc2, mdc3, m_lWidth, m_lHeight);

		BitBlt(mdc1, 0, 0, m_lWidth, m_lHeight, mdc3, 0, 0, SRCAND);
		//------------------------------------------------------------------------
		
		int nWidth = m_Shape.lMaxX - m_Shape.lMinX;

		int nHeight = m_Shape.lMaxY - m_Shape.lMinY;

		HDC hdcCopy = CreateCompatibleDC(hdcScreen);

		HBITMAP bitmapCopy = CreateCompatibleBitmap(hdcScreen, nWidth, nHeight);

		SelectObject(hdcCopy, bitmapCopy);

		BitBlt(hdcCopy, 0, 0, nWidth, nHeight, mdc1, m_Shape.lMinX, m_Shape.lMinY, SRCCOPY);
		
		OpenClipboard(NULL);
		
		EmptyClipboard() ;
		
		SetClipboardData (CF_BITMAP, bitmapCopy) ;
		
		CloseClipboard();

		DeleteObject(bitmapCopy);

		DeleteDC(hdcCopy);
		//---------------------------------------------------------------------------

		DeleteDC(hdcScreen);

		DeleteDC(mdc1);

		DeleteDC(mdc2);

		DeleteDC(mdc3);

		DeleteObject(bitmap1);

		DeleteObject(bitmap2);

		DeleteObject(bitmap3);
	}

	return true;
}




/********************************************************
*������: TwoValueDeal
*
*��������:��ֵ������
*
*����:  
nValue ��ֵ
bReverse �෴
*
*����ֵ:�ɹ�����true;ʧ�ܷ���false;
********************************************************/
bool CJellImage::TwoValueDeal(int nValue, bool bReverse)
{
		if (!m_bIsVaild)
	{
		m_strLastError = "ͼƬ������,���ܽ��д���";

		return false;
	}
	else
	{
		BitmapStruct *pBitmap = new BitmapStruct;

		HDC hdcScreen = CreateDC("DISPLAY", NULL, NULL, NULL);
		
		pBitmap->hdc = CreateCompatibleDC(hdcScreen);
		
		pBitmap->bitmap = CreateCompatibleBitmap(hdcScreen, m_lWidth, m_lHeight);
		
		SelectObject(pBitmap->hdc, pBitmap->bitmap);
		
		BitBlt(pBitmap->hdc, 0, 0, m_lWidth, m_lHeight, m_stack_current.top()->hdc, 0, 0, SRCCOPY);////

		HDC mdc1 = CreateCompatibleDC(hdcScreen);

		HDC mdc2 = CreateCompatibleDC(hdcScreen);

		HDC mdc3 = CreateCompatibleDC(hdcScreen);

		HBITMAP bitmap1 = CreateCompatibleBitmap(hdcScreen, m_lWidth, m_lHeight);

		HBITMAP bitmap2 = CreateCompatibleBitmap(hdcScreen, m_lWidth, m_lHeight);

		HBITMAP bitmap3 = CreateCompatibleBitmap(hdcScreen, m_lWidth, m_lHeight);

		SelectObject(mdc1, bitmap1);

		SelectObject(mdc2, bitmap2);

		SelectObject(mdc3, bitmap3);

		BitBlt(mdc1, 0, 0, m_lWidth, m_lHeight, pBitmap->hdc, 0, 0, SRCCOPY);

		SetBlackAndWhite(mdc2, mdc3, m_lWidth, m_lHeight);

		//------------------------------------------------------------------------
		
		BITMAP bm;

		GetObject(bitmap1, sizeof(BITMAP), &bm);

		BYTE *pData = new BYTE[bm.bmWidthBytes * bm.bmHeight];

		GetBitmapBits(bitmap1,  bm.bmWidthBytes * bm.bmHeight, pData);

		int bits = bm.bmWidthBytes / bm.bmWidth;
		
		for (int i = m_Shape.lMinY; i < m_Shape.lMaxY; i++)
		{
			
			for (int k = m_Shape.lMinX * bits; k < m_Shape.lMaxX * bits; k += bits)
			{
				
				BYTE Gray = pData[i * bm.bmWidthBytes + k] * 0.3 + pData[i * bm.bmWidthBytes + k + 1] * 0.59 + pData[i * bm.bmWidthBytes + k + 2] * 0.11;

				if (bReverse)
				{
					if (Gray >= nValue)
					{
						Gray = 0;
					}
					else
					{
						Gray = 255;
					}
				}
				else
				{
					if (Gray >= nValue)
					{
						Gray = 255;
					}
					else
					{
						Gray = 0;
					}
				}
				
				pData[i * bm.bmWidthBytes + k]  = Gray;
				pData[i * bm.bmWidthBytes + k + 1]  = Gray;
				pData[i * bm.bmWidthBytes + k + 2]  = Gray;
			}
		}

		SetBitmapBits(bitmap1,  bm.bmWidthBytes * bm.bmHeight, pData);

		delete []pData;

		BitBlt(mdc1, 0, 0, m_lWidth, m_lHeight, mdc3, 0, 0, SRCAND);

		BitBlt(pBitmap->hdc, 0, 0, m_lWidth, m_lHeight, mdc2, 0, 0, SRCAND);

		BitBlt(pBitmap->hdc, 0, 0, m_lWidth, m_lHeight, mdc1, 0, 0, SRCPAINT);

  
		//---------------------------------------------------------------------------
		
		m_stack_current.push(pBitmap);
		
		m_bHaveChanged = true;

		DeleteDC(hdcScreen);

		DeleteDC(mdc1);

		DeleteDC(mdc2);

		DeleteDC(mdc3);

		DeleteObject(bitmap1);

		DeleteObject(bitmap2);

		DeleteObject(bitmap3);
	}

	return true;
}