#if !defined JELL_IMAGE_H_FILE_INCLUDE
#define JELL_IMAGE_H_FILE_INCLUDE

#include <stack>
#include <vector>
using namespace std;

#include "cximage/ximage.h"

#include "PickColor/PickColorLib.h"
#pragma   comment(lib, "PickColor/PickColorLib.lib")


#pragma   comment(lib,"cximage/CxImage.lib")
#pragma   comment(lib,"cximage/j2k.lib")
#pragma   comment(lib,"cximage/jasper.lib")
#pragma   comment(lib,"cximage/jbig.lib")
#pragma   comment(lib,"cximage/jpeg.lib")
#pragma   comment(lib,"cximage/png.lib")
#pragma   comment(lib,"cximage/Tiff.lib")
#pragma   comment(lib,"Msimg32.lib")



const double pi = 3.14159265359;


class CJellImage
{
public:
	

	/********************************************************
	*函数名: GrayDeal
	*
	*函数功能:灰度处理
	*
	*参数:  
		nGrade 表示灰度后处理明暗度, -255 到255之间
	*
	*返回值:成功返回true;失败返回false;
	********************************************************/
	bool GrayDeal(int nGrade);


	/********************************************************
	*函数名: ReverseColorDeal
	*
	*函数功能:反色处理
	*
	*参数:  无
	*
	*返回值:成功返回true;失败返回false;
	********************************************************/
	bool ReverseColorDeal();


	/********************************************************
	*函数名: LightShadeDeal
	*
	*函数功能:明暗处理
	*
	*参数:  nValue 要加强的明暗度,在-255到255之间
	*
	*返回值:成功返回true;失败返回false;
	********************************************************/
	bool LightShadeDeal(int nValue);



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
	bool RGBDeal(int nR, int nG, int nB);



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
	bool LineDeal(int nZ, int nLineWidth, int nSpaceWidth, COLORREF colLineColor, int psLine, int nAlphaValue);



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
	bool PointDeal(int nLeftRightSpace, int nUpDownSpace, COLORREF colPointColor, int nAlphaValue);
	


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
	bool ColorFilterDeal(bool bR, int nMinR, int nMaxR,
		                 bool bG, int nMinG, int nMaxG,
						 bool bB, int nMinB, int nMaxB,
						 bool bGray, int nGrayValue,
						 COLORREF colFillColor
						);


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
	bool SingleColorFillDeal(COLORREF colFillColor, int nAlphaBlend);


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
	bool TwoValueDeal(int nValue, bool bReverse);



	/********************************************************
	*函数名: CopyBitmapToClipboard
	*
	*函数功能:复制当前选择的图像到内存中
	*
	*参数:  无
	*
	*返回值:成功返回true;失败返回false;
	********************************************************/
	bool CopyBitmapToClipboard();
	



public:
	enum FILETYPE
	{
		GIF,
		JPG,
		JPEG,
		BMP,
		PNG,
		WMF,
		UNKNOW
	};

	enum SELECTTYPE
	{
		ALL = 0,//全选
		BOX = 1,//矩形
		ELLIPSE= 2,//椭圆
		POLYGON = 3,//多边形
		LINE = 4//线形
	};
private:

	class CShape
	{
	public:
		long lMinX;
		long lMinY;
		long lMaxX;
		long lMaxY;
		SELECTTYPE m_SelectType;//选择的类型
		vector<POINT> m_vecP;//点的类

		CShape()
		{
			Init();
		}

		~CShape()
		{
			m_vecP.clear();
		}

		void Init()
		{
			lMaxX = 0;
			lMaxY = 0;
			lMinX = 0;
			lMinY = 0;
			m_SelectType = ALL;
			m_vecP.clear();
		}

		void InsertPoint(POINT p)
		{
			if (m_vecP.size() <= 0)
			{
				lMaxX = p.x;
				lMinX = p.x;

				lMaxY = p.y;
				lMinY = p.y;
			}
			else
			{
				if (lMaxX < p.x)
				{
					lMaxX = p.x;
				}

				if (lMinX > p.x)
				{
					lMinX = p.x;
				}


				if (lMaxY < p.y)
				{
					lMaxY = p.y;
				}

				if (lMinY > p.y)
				{
					lMinY = p.y;
				}
			}

			m_vecP.push_back(p);
		}

		void InsertPoint(int x, int y)
		{
			if (m_vecP.size() <= 0)
			{
				lMaxX = x;
				lMinX = x;

				lMaxY = y;
				lMinY = y;
			}
			else
			{
				if (lMaxX < x)
				{
					lMaxX = x;
				}

				if (lMinX > x)
				{
					lMinX = x;
				}


				if (lMaxY < y)
				{
					lMaxY = y;
				}

				if (lMinY > y)
				{
					lMinY = y;
				}
			}

			POINT pInsert = {x, y};

			m_vecP.push_back(pInsert);
		}
	};

	

	class BitmapStruct
	{
	public:
		BitmapStruct()
		{
			hdc = NULL;

			bitmap = NULL;
		}

		~BitmapStruct()
		{
			ClearMemory();
		}

		void ClearMemory()
		{
			if (hdc != NULL)
			{
				DeleteDC(hdc);
			}

			if (bitmap != NULL)
			{
				DeleteObject(bitmap);
			}
		}
		
		HDC hdc;

		HBITMAP bitmap;
	};


	

public:
	/********************************************************
	*函数名: CJellImage
	*
	*函数功能:构造函数
	*
	*参数:  无
	*
	*返回值:无
	********************************************************/
	CJellImage();

	/********************************************************
	*函数名: ~CJellImage
	*
	*函数功能:析构函数
	*
	*参数:无
	*
	*返回值:无
	********************************************************/
	~CJellImage();


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
	void SetParent(HWND hWnd, HINSTANCE hint);

	/********************************************************
	*函数名: LoadFile
	*
	*函数功能:读取一个文件
	*
	*参数:无
	*
	*返回值:成功返回true, 失败返回false
	********************************************************/
	bool LoadFile();

	
	/********************************************************
	*函数名: IsVaild
	*
	*函数功能:当前是否可用
	*
	*参数:无
	*
	*返回值:返回是否可用
	********************************************************/
	bool IsVaild();


	/********************************************************
	*函数名: SaveFile
	*
	*函数功能:保存当前文件
	*
	*参数:无
	*
	*返回值:成功返回true, 失败返回false
	********************************************************/
	bool SaveFile();


	/********************************************************
	*函数名: SaveAsFile
	*
	*函数功能:文件另存为
	*
	*参数:无
	*
	*返回值:成功返回true, 失败返回false
	********************************************************/
	bool SaveAsFile();

	
	/********************************************************
	*函数名: CloseFile
	*
	*函数功能:关闭文件
	*
	*参数:无
	*
	*返回值:成功返回true, 失败返回false
	********************************************************/
	bool CloseFile();


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
	bool Draw(HDC DestDC, long DestX, long DestY, long DestWidth, long DestHeight, long SrcX, long SrcY, long SrcWidth, long SrcHeight, DWORD  DrawMode);



	/********************************************************
	*函数名: GetHeight
	*
	*函数功能:得到图片的高
	*
	*参数:无
	*
	*返回值:返回图片的高
	********************************************************/
	long GetHeight();



	/********************************************************
	*函数名: GetWidth
	*
	*函数功能:得到图片的宽
	*
	*参数:无
	*
	*返回值:返回图片的宽
	********************************************************/
	long GetWidth();



	/********************************************************
	*函数名: GetLastError
	*
	*函数功能:返回最后个错误消息
	*
	*参数:无
	*
	*返回值:返回最后个错误消息
	********************************************************/
	CString GetLastError();




	/********************************************************
	*函数名: GetFileName
	*
	*函数功能:得到当前的文件名
	*
	*参数:无
	*
	*返回值:返回当前的文件名
	********************************************************/
	CString GetFileName();




	/********************************************************
	*函数名: GetFileType
	*
	*函数功能:得到当前的文件类型
	*
	*参数:无
	*
	*返回值:返回前的文件类型  "gif", "jpeg", "bmp", "jpg", "png"
	********************************************************/
	CString GetFileType();




	/********************************************************
	*函数名: DeleteTop
	*
	*函数功能:删除当前最上面的状态
	*
	*参数:无
	*
	*返回值:成功返回true, 失败返回false
	********************************************************/
	bool DeleteTop();



	/********************************************************
	*函数名: Back
	*
	*函数功能:后退到前一个状态
	*
	*参数:无
	*
	*返回值:成功返回true, 失败返回false
	********************************************************/
	bool Back();



	/********************************************************
	*函数名: Forward
	*
	*函数功能:前进到后一个状态
	*
	*参数:无
	*
	*返回值:成功返回true, 失败返回false
	********************************************************/
	bool Forward();


	/********************************************************
	*函数名: GetBackCount
	*
	*函数功能:得到后退状态的个数
	*
	*参数:无
	*
	*返回值:返回个数
	********************************************************/
	int GetBackCount();




	/********************************************************
	*函数名: GetForwardCount
	*
	*函数功能:得到向前状态的个数
	*
	*参数:无
	*
	*返回值:返回个数
	********************************************************/
	int GetForwardCount();



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
	void SetBlackAndWhite(HDC BlackDC, HDC WhiteDC, long lWidth, long lHeight);



	
private:

	FILETYPE m_FileType;

	stack<BitmapStruct *> m_stack_current;//当前的位图

	stack<BitmapStruct *> m_stack_history;//被删除的位图

	CString m_strFileName;//当前的文件名

	bool m_bIsVaild;//当前的文件是否可用

	long m_lWidth;//位图的宽

	long m_lHeight;//位图的高

	CString m_strLastError;//最后一个错误的内容

	HWND m_hWndParent;//父窗口的句柄

	HINSTANCE m_hinstance;//主窗口的线程

	bool m_bHaveChanged;//图片是否已经改变过了

	

private:

	/********************************************************
	*函数名: ClearMemory
	*
	*函数功能:清空当前所有内容
	*
	*参数:无
	*
	*返回值:无
	********************************************************/
	void ClearMemory();



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
	FILETYPE GetFileExt(char *lpszFileName);
	

	
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
	DWORD TransFormFileTypeToCXImageDword(FILETYPE FileType);
	


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
	bool SaveHDCToFile(char *lpszFileName);




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
	bool OpenFileToHDC(char *lpszFileName);


public:
		CShape m_Shape;
};
#endif