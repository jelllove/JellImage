#include "StdAfx.h"
#include "JellImage.h"




/********************************************************
*函数名: CJellImage
*
*函数功能:构造函数
*
*参数:  无
*
*返回值:无
********************************************************/
CJellImage::CJellImage()
{
	ClearMemory();
}

/********************************************************
*函数名: ~CJellImage
*
*函数功能:析构函数
*
*参数:无
*
*返回值:无
********************************************************/
CJellImage::~CJellImage()
{
	ClearMemory();
}


/********************************************************
*函数名: SetParentHwnd
*
*函数功能:得到当前父窗口的句柄 
*
*参数:
hWnd 表示当前窗口的句柄
hint 表示主程序的线程
*
*返回值:无
********************************************************/
void CJellImage::SetParent(HWND hWnd, HINSTANCE hint)
{
	m_hWndParent = hWnd;

	m_hinstance = hint; 
}






/********************************************************
*函数名: LoadFile
*
*函数功能:读取一个文件
*
*参数:无
*
*返回值:成功返回true, 失败返回false
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
	
	fileStruct.lpstrFilter   =  "所有图片文件\0*.gif;*.bmp;*.jpg;*.jpeg;*.png\0BMP文件(*.bmp)\0*.bmp\0JPG文件(*.jpg)\0*.jpg\0JPEG文件(*.jpeg)\0*.jpeg\0GIF文件(*.gif)\0*.gif\0PNG文件(*.png)\0*.png\0";
	
	fileStruct.nMaxFile   =   MAX_PATH;
	
	TCHAR   szFile[MAX_PATH]="";
	
	fileStruct.lpstrFile = szFile;
	
	fileStruct.lpstrTitle="";
	
	fileStruct.lpstrDefExt="";   
	
	fileStruct.nMaxFile   = MAX_PATH;   
	
	fileStruct.hInstance   =   m_hinstance;
	
	if(GetOpenFileName(&fileStruct) == FALSE)
	{
		m_strLastError = "用户取消了打开文件的操作";

		return false;
	}
	else
	{
		if (!OpenFileToHDC(fileStruct.lpstrFile))
		{
			//MessageBox(m_hWndParent, "文件: " + m_strFileName + " 打开错误", "错误", MB_OK | MB_ICONERROR);

			m_strLastError = "打开文件时发生错误";
		
			return false;
		}
	}
	return  true;
}


/********************************************************
*函数名: IsVaild
*
*函数功能:当前是否可用
*
*参数:无
*
*返回值:返回是否可用
********************************************************/
bool CJellImage::IsVaild()
{
	return m_bIsVaild;
}


/********************************************************
*函数名: SaveFile
*
*函数功能:保存当前文件
*
*参数:无
*
*返回值:成功返回true, 失败返回false
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
				m_strLastError = "在文件保存后,重新读取时发生错误";

				return false;
			}
		}
		else
		{
			m_strLastError = "文件无法保存";

			return false;
		}
	}
	else
	{

		m_strLastError = "当前没有打开文件";

		return false;
	}
}


/********************************************************
*函数名: SaveAsFile
*
*函数功能:文件另存为
*
*参数:
lpszFileName 表示要保存的文件名
*
*返回值:成功返回true, 失败返回false
********************************************************/
bool CJellImage::SaveAsFile()
{
	if (m_bIsVaild)
	{
		OPENFILENAME   fileStruct;     
		
		ZeroMemory(&fileStruct,   sizeof(OPENFILENAME));     
		
		fileStruct.lStructSize   =   sizeof(OPENFILENAME);   
		
		fileStruct.hwndOwner   =   NULL;     
		
		fileStruct.lpstrFilter   =  "BMP文件(*.bmp)\0*.bmp\0JPG文件(*.jpg)\0*.jpg\0JPEG文件(*.jpeg)\0*.jpeg\0GIF文件(*.gif)\0*.gif\0PNG文件(*.png)\0*.png\0";
		
		fileStruct.nMaxFile   =   MAX_PATH;
		
		TCHAR   szFile[MAX_PATH]="";
		
		fileStruct.lpstrFile = szFile;
		
		fileStruct.lpstrTitle="";
		
		fileStruct.lpstrDefExt="";   
		
		fileStruct.nMaxFile   = MAX_PATH;   
		
		fileStruct.hInstance   =   m_hinstance;
		
		if(GetSaveFileName(&fileStruct) == FALSE)
		{
			m_strLastError = "用户取消了文件另存为";

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
				m_strLastError = "在文件另存为后,重新读取时发生错误";

				return false;
			}
		}
		else
		{
			m_strLastError = "文件无法另存为";

			return false;
		}
	}
	else
	{
		m_strLastError = "当前没有打开文件";

		return false;
	}
}


/********************************************************
*函数名: CloseFile
*
*函数功能:关闭文件
*
*参数:无
*
*返回值:成功返回true, 失败返回false
********************************************************/
bool CJellImage::CloseFile()
{
	if (m_bIsVaild && m_bHaveChanged)
	{
		int nID = MessageBox(m_hWndParent, "是否要保存当前文件: " + m_strFileName, "询问", MB_YESNOCANCEL | MB_ICONQUESTION);

		switch(nID)
		{
		case IDYES:
			{
				if (!SaveHDCToFile(m_strFileName.GetBuffer(0)))
				{
					m_strLastError = "文件保存不成功";

					//MessageBox(m_hWndParent, "文件: " + m_strFileName + " 保存错误", "错误", MB_OK | MB_ICONERROR);
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

				m_strLastError = "用户取消了关闭文件的操作";

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
*函数名: Draw
*
*函数功能:把图片画到一个设备上
*
*参数:

  DestDC 目标设备
  DestX  目标X轴坐标
  DestY  目标Y轴坐标
  DestWidth 目标宽
  DestHeight 目标高
  
		  SrcX   源图X轴坐标
		  SrcY   源图Y轴坐标
		  SrcWidth 源图宽
		  SrcHeight 源图高
		  
			DrawMode 贴图模式
			
			  *
			  *返回值:成功返回true, 失败返回false
********************************************************/
bool CJellImage::Draw(HDC DestDC, long DestX, long DestY, long DestWidth, long DestHeight, long SrcX, long SrcY, long SrcWidth, long SrcHeight, DWORD  DrawMode)
{

	if (!m_bIsVaild)
	{
		m_strLastError = "当前图片不存在";

		return false;
	}
	else
	{
		if (!StretchBlt(DestDC, DestX, DestY, DestWidth, DestHeight, m_stack_current.top()->hdc, SrcX, SrcY, SrcWidth, SrcHeight, DrawMode))
		{
			m_strLastError = "贴图时发生错误";

			return false;
		}
	}
	return true;
}



/********************************************************
*函数名: GetHeight
*
*函数功能:得到图片的高
*
*参数:无
*
*返回值:返回图片的高
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
*函数名: GetWidth
*
*函数功能:得到图片的宽
*
*参数:无
*
*返回值:返回图片的宽
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
*函数名: GetLastError
*
*函数功能:返回最后个错误消息
*
*参数:无
*
*返回值:返回最后个错误消息
********************************************************/
CString CJellImage::GetLastError()
{
	return m_strLastError;
}




/********************************************************
*函数名: GetFileName
*
*函数功能:得到当前的文件名
*
*参数:无
*
*返回值:返回当前的文件名
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
*函数名: GetFileType
*
*函数功能:得到当前的文件类型
*
*参数:无
*
*返回值:返回前的文件类型  "gif", "jpeg", "bmp", "jpg", "png"
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
*函数名: Back
*
*函数功能:后退到前一个状态
*
*参数:无
*
*返回值:成功返回true, 失败返回false
********************************************************/
bool CJellImage::Back()
{
	if (!m_bIsVaild)
	{
		m_strLastError = "图片不存在";

		return false;
	}
	else
	{
		if (m_stack_current.size() <= 1)
		{
			m_strLastError = "已经撤消到第一个状态了";

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
*函数名: Forward
*
*函数功能:前进到后一个状态
*
*参数:无
*
*返回值:成功返回true, 失败返回false
********************************************************/
bool CJellImage::Forward()
{
	if (!m_bIsVaild)
	{
		m_strLastError = "图片不存在";

		return false;
	}
	else
	{
		if (m_stack_history.size() <= 0)
		{
			m_strLastError = "已经重复到最后一个状态了";

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
*函数名: GetBackCount
*
*函数功能:得到后退状态的个数
*
*参数:无
*
*返回值:返回个数
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
*函数名: GetForwardCount
*
*函数功能:得到向前状态的个数
*
*参数:无
*
*返回值:返回个数
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
*函数名: ClearMemory
*
*函数功能:清空当前所有内容
*
*参数:无
*
*返回值:无
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


	//m_strFileName = "";//当前的文件名

	m_bIsVaild = false;//当前的文件是否可用

	m_lWidth = 0;//位图的宽

	m_lHeight = 0;//位图的高

	m_strLastError = "";//最后一个错误的内容

	m_bHaveChanged = false;//图片是否已经改变过了
}


/********************************************************
*函数名: TransFormFileTypeToCXImageDword
*
*函数功能:把我自己的文件类型转化成CXImage的格式类型
*
*参数说明:
*       FILETYPE表示当前的文件类型
*
*函数返回说明: CXimage的格式类型
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
*函数名: SaveHDCToFile
*
*函数功能:把当前的内容保存到文件中
*
*参数:
lpszFileName  要保存的文件名
*
*返回值:成功返回true, 失败返回false
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
*函数名: OpenFileToHDC
*
*函数功能:打开文件
*
*参数:
lpszFileName  要保存的文件名
*
*返回值:成功返回true, 失败返回false
********************************************************/
bool CJellImage::OpenFileToHDC(char *lpszFileName)
{
	ClearMemory();
	
	CxImage image;
	
	image.Load(lpszFileName, TransFormFileTypeToCXImageDword(GetFileExt(lpszFileName)));
	
	if (!image.IsValid())
	{
		m_strLastError = "无法打开文件";

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
*函数名: GrayDeal
*
*函数功能:构造函数
*
*参数:  
nGrade 表示灰度后处理明暗度, -255 到255之间
*
*返回值:成功返回true;失败返回false;
********************************************************/
bool CJellImage::GrayDeal(int nGrade)
{
	if (!m_bIsVaild)
	{
		m_strLastError = "图片不存在,不能进行处理";

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
*函数名: DeleteTop
*
*函数功能:删除当前最上面的状态
*
*参数:无
*
*返回值:成功返回true, 失败返回false
********************************************************/
bool CJellImage::DeleteTop()
{
	if (!IsVaild())
	{
		m_strLastError = "图片不存在";

		return false;
	}
	else
	{
		if (m_stack_current.size() <= 1)
		{
			m_strLastError = "已经是最初状态了,无法删除";

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
*函数名: SetBlackAndWhite
*
*函数功能:制作黑白模板
*
*参数:
BlackDC  选择区填充黑色的设备
WhiteDC  选择区填充黑色的设备
lWidth   图片的宽
lHeight  图片的高
*
*返回值:无
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
*函数名: ReverseColorDeal
*
*函数功能:反色处理
*
*参数:  无
*
*返回值:成功返回true;失败返回false;
********************************************************/
bool CJellImage::ReverseColorDeal()
{
	if (!m_bIsVaild)
	{
		m_strLastError = "图片不存在,不能进行处理";

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
*函数名: LightShadeDeal
*
*函数功能:明暗处理
*
*参数:  nValue 要加强的明暗度,在-255到255之间
*
*返回值:成功返回true;失败返回false;
********************************************************/
bool CJellImage::LightShadeDeal(int nValue)
{
	if (!m_bIsVaild)
	{
		m_strLastError = "图片不存在,不能进行处理";

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
*函数名: RGBDeal
*
*函数功能:RGB颜色调整
*
*参数:  
nR 要调整的R值,在-255到255之间
nG 要调整的G值,在-255到255之间
nB 要调整的B值,在-255到255之间
*
*返回值:成功返回true;失败返回false;
********************************************************/
bool CJellImage::RGBDeal(int nR, int nG, int nB)
{
	if (!m_bIsVaild)
	{
		m_strLastError = "图片不存在,不能进行处理";

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
*函数名: LineDeal
*
*函数功能:画线处理
*
*参数:  
nZ  角度
nLineWidth 线宽
nSpaceWidth 空白间距
colLineColor 线的颜色
psLine 线的种类
nAlphaValue 透明度
*
*返回值:成功返回true;失败返回false;
********************************************************/
bool CJellImage::LineDeal(int nZ, int nLineWidth, int nSpaceWidth, COLORREF colLineColor, int psLine, int nAlphaValue)
{
	if (!m_bIsVaild)
	{
		m_strLastError = "图片不存在,不能进行处理";

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

		b.BlendOp = AC_SRC_OVER; //定值

		b.BlendFlags = 0; //定值

		b.SourceConstantAlpha = nAlphaValue; //不用总体透明度，用每一象素点自己的alpha值

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
*函数名: PointDeal
*
*函数功能:点处理
*
*参数:  
nLeftRightSpace 左右间距
nUpDownSpace 上下间距
colPointColor 点的颜色
nAlphaValue 透明度
*
*返回值:成功返回true;失败返回false;
********************************************************/
bool CJellImage::PointDeal(int nLeftRightSpace, int nUpDownSpace, COLORREF colPointColor, int nAlphaValue)
{
	if (!m_bIsVaild)
	{
		m_strLastError = "图片不存在,不能进行处理";

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

		b.BlendOp = AC_SRC_OVER; //定值

		b.BlendFlags = 0; //定值

		b.SourceConstantAlpha = nAlphaValue; //不用总体透明度，用每一象素点自己的alpha值

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
*函数名: GetFileExt
*
*函数功能:得到文件的辍，然后转化成FILETYPE型
*
*参数:
lpszFileName   文件名
*
*返回值:无
********************************************************/
CJellImage::FILETYPE CJellImage::GetFileExt(char *lpszFileName)
{
	char szExt[_MAX_EXT];
	
	_splitpath(lpszFileName, NULL, NULL, NULL, szExt);
	
	strcpy(szExt, strlwr(szExt));//改成小写的
	
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
*函数名: ColorFilterDeal
*
*函数功能:颜色过滤
*
*参数:  bR 是否要对R色过滤,只要R值在nMinR和nMaxR之间，R就过滤
bG 是否要对G色过滤,只要G值在nMinG和nMaxG之间，G就过滤
bB 是否要对B色过滤,只要B值在nMinB和nMaxB之间，B就过滤
bGray 是否要把过滤后的颜色变为灰色, nGrayValue为灰度值
colFillColor 是在不把过滤后的颜色填充为一种颜色的颜色
*
*返回值:成功返回true;失败返回false;
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
		m_strLastError = "图片不存在,不能进行处理";

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
*函数名: SingleColorFillDeal
*
*函数功能:单色填充
*
*参数:  
colFillColor 填充的颜色
nAlphaBlend  半透明度
*
*返回值:成功返回true;失败返回false;
********************************************************/
bool CJellImage::SingleColorFillDeal(COLORREF colFillColor, int nAlphaBlend)
{
	if (!m_bIsVaild)
	{
		m_strLastError = "图片不存在,不能进行处理";

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

		b.BlendOp = AC_SRC_OVER; //定值

		b.BlendFlags = 0; //定值

		b.SourceConstantAlpha = nAlphaBlend; //不用总体透明度，用每一象素点自己的alpha值

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
*函数名: CopyBitmapToClipboard
*
*函数功能:复制当前选择的图像到内存中
*
*参数:  无
*
*返回值:成功返回true;失败返回false;
********************************************************/
bool CJellImage::CopyBitmapToClipboard()
{
	if (!m_bIsVaild)
	{
		m_strLastError = "图片不存在,不能进行复制";

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
*函数名: TwoValueDeal
*
*函数功能:二值化处理
*
*参数:  
nValue 阈值
bReverse 相反
*
*返回值:成功返回true;失败返回false;
********************************************************/
bool CJellImage::TwoValueDeal(int nValue, bool bReverse)
{
		if (!m_bIsVaild)
	{
		m_strLastError = "图片不存在,不能进行处理";

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