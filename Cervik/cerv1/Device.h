#pragma once 
//#include <Source/Main/Application.h>

class CFrameBuffer {
private:

	LPVOID m_pBuffer;
	HDC m_pDC;
	HANDLE m_Old;
	HBITMAP m_ourbitmap;
	LONG m_lWidth;
	LONG m_lHeight;

	LONG m_lWindowWidth;
	LONG m_lWindowHeight;

public:
	CFrameBuffer()
	{
		m_lWidth = GetSystemMetrics(SM_CXSCREEN);
		m_lHeight = GetSystemMetrics(SM_CYSCREEN);

		m_lWindowWidth = m_lWidth;
		m_lWindowHeight = m_lHeight;

		HDC hDC;
		BITMAPINFO bitmapinfo;
		hDC=CreateCompatibleDC(NULL);
		bitmapinfo.bmiHeader.biSize=sizeof(BITMAPINFOHEADER);
		bitmapinfo.bmiHeader.biWidth=Width();
		bitmapinfo.bmiHeader.biHeight=-Height(); /* top-down */
		bitmapinfo.bmiHeader.biPlanes=1;
		bitmapinfo.bmiHeader.biBitCount=32;
		bitmapinfo.bmiHeader.biCompression=BI_RGB;
		bitmapinfo.bmiHeader.biSizeImage=0;
		bitmapinfo.bmiHeader.biClrUsed=256;
		bitmapinfo.bmiHeader.biClrImportant=256;
		m_pBuffer = NULL;
		m_ourbitmap=CreateDIBSection(hDC,&bitmapinfo,DIB_RGB_COLORS,&m_pBuffer,0,0);
		m_pDC=CreateCompatibleDC(NULL);
		m_Old=SelectObject(m_pDC,m_ourbitmap);
		DeleteDC(hDC);

		FillMemory(m_pBuffer, sizeof(DWORD)*Width()*Height(), 0xb0);
	}

	LPVOID GetBuffer()
	{
		return m_pBuffer;
	}

	void Blit(HDC hDC)
	{
		BitBlt(hDC,0,0,Width(),Height(),m_pDC,0,0,SRCCOPY);
	}

	HDC GetDc()
	{
		return m_pDC;
	}

	~CFrameBuffer()
	{
		SelectObject(m_pDC,m_Old);
		DeleteDC(m_pDC);
		DeleteObject(m_ourbitmap);
	}

	LONG Width()
	{
		return m_lWidth;
	}

	LONG Height()
	{
		return m_lHeight;
	}

	LONG WindowWidth()
	{
		return m_lWindowWidth;
	}

	LONG WindowHeight()
	{
		return m_lWindowHeight;
	}

	void SetDisplay(int w, int h)
	{
		m_lWindowWidth = min(m_lWidth, w);
		m_lWindowHeight = min(m_lHeight, h);
	}
};

class CDevice {
public:
	static CDevice* m_pInstance;
	CFrameBuffer display;
	int keys;
	int keybuf[1000];
	int mousex, mousey, moused;

public:
	CDevice()
	{
		m_pInstance = this;
		keys = 0;
		memset(keybuf, 0, sizeof(keybuf));
		mousex = -1;
		mousey = -1;
		moused = 0;
	}
	~CDevice()
	{
	}
	void operator ()()
	{
	}
	void Blit( HDC hDC )
	{
		display.Blit( hDC );
	}
	void OnKeyDown(int k)
	{
		_ASSERT( k >= 0 && k < 1000 );
		keybuf[k] = 1;
		//keys |= k;
	}
	void OnKeyUp(int k)
	{
		_ASSERT( k >= 0 && k < 1000 );
		keybuf[k] = 0;
		//keys |= k;
	}
	void OnMouseMove(int x, int y)
	{
		mousex = x;
		mousey = y;
	}
	void OnMouseDown()
	{
		moused = 1;
	}
	void OnMouseUp()
	{
		//moused = 0;
	}
	int* GetKeys()
	{
		return keybuf;
	}
};