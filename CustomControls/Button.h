#pragma once

#include "Common.h"

#define WC_BUTTONCTRLW			TEXT(WC_BUTTONCTRLA)

#ifdef UNICODE
#define WC_BUTTONCTRL			WC_BUTTONCTRLW
#else
#define WC_BUTTONCTRL			WC_BUTTONCTRLA
#endif

// =====================================================================
// ������� ����� �������� (������ ��� ����� Window Style)

// ������ ����� ���������� WM_COMMAND � ������������ ����,
// ����� ������������ �������� �� ���.
#define BCS_PUSHBUTTON			0x00000000L
// ���������, ��� ������ ���������� �����.
#define BCS_TEXT				0x00000000L
// ����������� ����� �� ������ ���� � �������������� ������.
//#define BCS_LEFT				0x00000001L
// ����������� ����� �� ������� ���� � �������������� ������.
//#define BCS_RIGHT				0x00000002L
// ���������� ����� �� ����������� � �������������� ������.
//#define BCS_CENTER				0x00000004L
// ��������� ����� �� ������� ����� �������������� ������.
//#define BCS_TOP					0x00000008L
// ��������� ����� �� ������ ����� �������������� ������.
//#define BCS_BOTTOM				0x00000010L
// ���������� ����� �� ��������� � �������������� ������.
//#define BCS_VCENTER				0x00000020L
// ����� ����������� � ��������� �����, ���� ������ ������� �������, 
// ����� ����������� � ���� ������ � �������������� ������.
#define BCS_MULTILINE			0x00000040L
// ������ ����� ���������� ���� ����������� BEN_KILLFOCUS � BEN_SETFOCUS � ������������ ����. 
// �������� ��������, ��� ������ ���������� ��� ����������� BEN_CLICKED ���������� �� ����� �����.
#define BCS_NOTIFY				0x00000080L
// � ������ ����� ������ (��� ������) �������� �������
#define BCS_ARROW				0x00000100L
// ������ ����� ����� �������������� ������ ����� �� ��������
// ��������������� ������. ������ ��������� WM_COMMAND, ������������� ��� ������� ������,
// ������ �������� ��� ����������� BEN_DROPDOWN.
#define BCS_SPLITBUTTON			0x00000200L
// �������������� ������ ����� �������������� ������.
#define BCS_SPLITRIGHT			0x00000400L
#define BCS_DROPDOWN			(BCS_ARROW|BCS_SPLITBUTTON)



// =====================================================================
// ���������

#define BEM_SETIMAGELIST		(BEM_FIRST+0)
#define ButtonCtrl_SetImageList(hwnd, pimagelist, index)	\
	(HIMAGELIST)SNDMSG((hwnd), BEM_SETIMAGELIST, \
	(WPARAM)(INT)(index), (LPARAM)(HIMAGELIST)(pimagelist))

#define BEM_GETIMAGELIST		(BEM_FIRST+1)
#define ButtonCtrl_GetImageList(hwnd)	\
	(HIMAGELIST)SNDMSG((hwnd), BEM_GETIMAGELIST, 0, 0)

#define BEM_SETIMAGE			(BEM_FIRST+2)
#define ButtonCtrl_SetImage(hwnd, imagetype, hbitmap)	\
	(BOOL)SNDMSG((hwnd), BEM_SETIMAGE, \
	(WPARAM)(UINT)(imagetype), (LPARAM)(HBITMAP)(hbitmap))

//#define BEM_GETIMAGE			(BEM_FIRST+3)
//#define ButtonCtrl_GetImage(hwnd, imagetype, hbitmap)	\
//	(BOOL)SNDMSG((hwnd), BEM_GETIMAGE, (WPARAM)(imagetype), (LPARAM)(hbitmap))

#define BEM_LAST				BEM_SETIMAGE

// =====================================================================
// �����������

#define BEN_CLICKED				1
#define BEN_DROPDOWN			2
#define BEN_BGCOLOR				3
#define BEN_SETFOCUS			4
#define BEN_KILLFOCUS			5
//#define BEN_GETIMAGE			4
//#define BEN_SETIMAGE			5

typedef struct tagNMBEDROPDOWN {
	NMHDR nmhdr;
	RECT  rcButton;
} NMBEDROPDOWN, *LPNMBEDROPDOWN;

// =====================================================================
// ������ ��������

typedef struct tagBUTTONCTRL
{
	DWORD		dwFlags;			// BF_*
	HFONT		hFont;
	HIMAGELIST	hImageList;
	INT			nImageIndex;
	LPVOID		hImage;				// Bitmap ��� Icon ���� ����� ����� ��������� ����, ��� hImageList
	UINT		nArrowWidth;		// ������ ������� �� ��������
	RECT		rcSplit;			// ������������� ��� �����������
} BUTTONCTRL, *LPBUTTONCTRL;

// =====================================================================
// ����� ��������� ����� dwFlags ��������� ������ ��������

#define BF_HIGHLIGHT			(1 << 0)
#define BF_PRESSED				(1 << 1)
#define BF_FOCUSED				(1 << 2)
#define BF_DROPDOWNHIGHLIGHT	(1 << 3)
#define BF_IMAGEBITMAP			(1 << 4)
#define BF_IMAGEICON			(1 << 5)

// =====================================================================
// ��������� ��������� 

ATOM Button_RegisterClass();
