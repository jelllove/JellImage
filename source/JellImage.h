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
	*������: GrayDeal
	*
	*��������:�Ҷȴ���
	*
	*����:  
		nGrade ��ʾ�ҶȺ���������, -255 ��255֮��
	*
	*����ֵ:�ɹ�����true;ʧ�ܷ���false;
	********************************************************/
	bool GrayDeal(int nGrade);


	/********************************************************
	*������: ReverseColorDeal
	*
	*��������:��ɫ����
	*
	*����:  ��
	*
	*����ֵ:�ɹ�����true;ʧ�ܷ���false;
	********************************************************/
	bool ReverseColorDeal();


	/********************************************************
	*������: LightShadeDeal
	*
	*��������:��������
	*
	*����:  nValue Ҫ��ǿ��������,��-255��255֮��
	*
	*����ֵ:�ɹ�����true;ʧ�ܷ���false;
	********************************************************/
	bool LightShadeDeal(int nValue);



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
	bool RGBDeal(int nR, int nG, int nB);



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
	bool LineDeal(int nZ, int nLineWidth, int nSpaceWidth, COLORREF colLineColor, int psLine, int nAlphaValue);



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
	bool PointDeal(int nLeftRightSpace, int nUpDownSpace, COLORREF colPointColor, int nAlphaValue);
	


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
	bool ColorFilterDeal(bool bR, int nMinR, int nMaxR,
		                 bool bG, int nMinG, int nMaxG,
						 bool bB, int nMinB, int nMaxB,
						 bool bGray, int nGrayValue,
						 COLORREF colFillColor
						);


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
	bool SingleColorFillDeal(COLORREF colFillColor, int nAlphaBlend);


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
	bool TwoValueDeal(int nValue, bool bReverse);



	/********************************************************
	*������: CopyBitmapToClipboard
	*
	*��������:���Ƶ�ǰѡ���ͼ���ڴ���
	*
	*����:  ��
	*
	*����ֵ:�ɹ�����true;ʧ�ܷ���false;
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
		ALL = 0,//ȫѡ
		BOX = 1,//����
		ELLIPSE= 2,//��Բ
		POLYGON = 3,//�����
		LINE = 4//����
	};
private:

	class CShape
	{
	public:
		long lMinX;
		long lMinY;
		long lMaxX;
		long lMaxY;
		SELECTTYPE m_SelectType;//ѡ�������
		vector<POINT> m_vecP;//�����

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
	*������: CJellImage
	*
	*��������:���캯��
	*
	*����:  ��
	*
	*����ֵ:��
	********************************************************/
	CJellImage();

	/********************************************************
	*������: ~CJellImage
	*
	*��������:��������
	*
	*����:��
	*
	*����ֵ:��
	********************************************************/
	~CJellImage();


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
	void SetParent(HWND hWnd, HINSTANCE hint);

	/********************************************************
	*������: LoadFile
	*
	*��������:��ȡһ���ļ�
	*
	*����:��
	*
	*����ֵ:�ɹ�����true, ʧ�ܷ���false
	********************************************************/
	bool LoadFile();

	
	/********************************************************
	*������: IsVaild
	*
	*��������:��ǰ�Ƿ����
	*
	*����:��
	*
	*����ֵ:�����Ƿ����
	********************************************************/
	bool IsVaild();


	/********************************************************
	*������: SaveFile
	*
	*��������:���浱ǰ�ļ�
	*
	*����:��
	*
	*����ֵ:�ɹ�����true, ʧ�ܷ���false
	********************************************************/
	bool SaveFile();


	/********************************************************
	*������: SaveAsFile
	*
	*��������:�ļ����Ϊ
	*
	*����:��
	*
	*����ֵ:�ɹ�����true, ʧ�ܷ���false
	********************************************************/
	bool SaveAsFile();

	
	/********************************************************
	*������: CloseFile
	*
	*��������:�ر��ļ�
	*
	*����:��
	*
	*����ֵ:�ɹ�����true, ʧ�ܷ���false
	********************************************************/
	bool CloseFile();


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
	bool Draw(HDC DestDC, long DestX, long DestY, long DestWidth, long DestHeight, long SrcX, long SrcY, long SrcWidth, long SrcHeight, DWORD  DrawMode);



	/********************************************************
	*������: GetHeight
	*
	*��������:�õ�ͼƬ�ĸ�
	*
	*����:��
	*
	*����ֵ:����ͼƬ�ĸ�
	********************************************************/
	long GetHeight();



	/********************************************************
	*������: GetWidth
	*
	*��������:�õ�ͼƬ�Ŀ�
	*
	*����:��
	*
	*����ֵ:����ͼƬ�Ŀ�
	********************************************************/
	long GetWidth();



	/********************************************************
	*������: GetLastError
	*
	*��������:��������������Ϣ
	*
	*����:��
	*
	*����ֵ:��������������Ϣ
	********************************************************/
	CString GetLastError();




	/********************************************************
	*������: GetFileName
	*
	*��������:�õ���ǰ���ļ���
	*
	*����:��
	*
	*����ֵ:���ص�ǰ���ļ���
	********************************************************/
	CString GetFileName();




	/********************************************************
	*������: GetFileType
	*
	*��������:�õ���ǰ���ļ�����
	*
	*����:��
	*
	*����ֵ:����ǰ���ļ�����  "gif", "jpeg", "bmp", "jpg", "png"
	********************************************************/
	CString GetFileType();




	/********************************************************
	*������: DeleteTop
	*
	*��������:ɾ����ǰ�������״̬
	*
	*����:��
	*
	*����ֵ:�ɹ�����true, ʧ�ܷ���false
	********************************************************/
	bool DeleteTop();



	/********************************************************
	*������: Back
	*
	*��������:���˵�ǰһ��״̬
	*
	*����:��
	*
	*����ֵ:�ɹ�����true, ʧ�ܷ���false
	********************************************************/
	bool Back();



	/********************************************************
	*������: Forward
	*
	*��������:ǰ������һ��״̬
	*
	*����:��
	*
	*����ֵ:�ɹ�����true, ʧ�ܷ���false
	********************************************************/
	bool Forward();


	/********************************************************
	*������: GetBackCount
	*
	*��������:�õ�����״̬�ĸ���
	*
	*����:��
	*
	*����ֵ:���ظ���
	********************************************************/
	int GetBackCount();




	/********************************************************
	*������: GetForwardCount
	*
	*��������:�õ���ǰ״̬�ĸ���
	*
	*����:��
	*
	*����ֵ:���ظ���
	********************************************************/
	int GetForwardCount();



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
	void SetBlackAndWhite(HDC BlackDC, HDC WhiteDC, long lWidth, long lHeight);



	
private:

	FILETYPE m_FileType;

	stack<BitmapStruct *> m_stack_current;//��ǰ��λͼ

	stack<BitmapStruct *> m_stack_history;//��ɾ����λͼ

	CString m_strFileName;//��ǰ���ļ���

	bool m_bIsVaild;//��ǰ���ļ��Ƿ����

	long m_lWidth;//λͼ�Ŀ�

	long m_lHeight;//λͼ�ĸ�

	CString m_strLastError;//���һ�����������

	HWND m_hWndParent;//�����ڵľ��

	HINSTANCE m_hinstance;//�����ڵ��߳�

	bool m_bHaveChanged;//ͼƬ�Ƿ��Ѿ��ı����

	

private:

	/********************************************************
	*������: ClearMemory
	*
	*��������:��յ�ǰ��������
	*
	*����:��
	*
	*����ֵ:��
	********************************************************/
	void ClearMemory();



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
	FILETYPE GetFileExt(char *lpszFileName);
	

	
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
	DWORD TransFormFileTypeToCXImageDword(FILETYPE FileType);
	


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
	bool SaveHDCToFile(char *lpszFileName);




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
	bool OpenFileToHDC(char *lpszFileName);


public:
		CShape m_Shape;
};
#endif