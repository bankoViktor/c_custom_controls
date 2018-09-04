#pragma once

#include "Common.h"

#define WC_BUTTONCTRLW			TEXT(WC_BUTTONCTRLA)

#ifdef UNICODE
#define WC_BUTTONCTRL			WC_BUTTONCTRLW
#else
#define WC_BUTTONCTRL			WC_BUTTONCTRLA
#endif

// =====================================================================
// Оконные стили контрола (первые два байта Window Style)

// Кнопка будет отправлять WM_COMMAND в родительское окно,
// когда пользователь нажимает на нее.
#define BCS_PUSHBUTTON			0x00000000L
// Кнопка будет иметь дополнительную секцию снизу со стрелкой
// раскрывающегося списка. Вместо сообщения WM_COMMAND, используемого для обычных кнопок,
// кнопка отправит код уведомления BEN_DROPDOWN.
#define BCS_WHOLEDROPDOWN		0x00000001L


// =====================================================================
// Сообщения

//#define BEM_FIRST				(WM_USER+2600)

#define BEM_SETIMAGELIST		(BEM_FIRST+0)
#define ButtonCtrl_SetImageList(hwnd, pbuttonImagelist)	\
	(BOOL)SNDMSG((hwnd), BEM_SETIMAGELIST, 0, (LPARAM)(pbuttonImagelist))

#define BEM_GETIMAGELIST		(BEM_FIRST+1)
#define ButtonCtrl_GetImageList(hwnd, pbuttonImagelist)	\
	(BOOL)SNDMSG((hwnd), BEM_GETIMAGELIST, 0, (LPARAM)(puttonImagelist))

#define BEM_SETIMAGE			(BEM_FIRST+2)
#define ButtonCtrl_SetImage(hwnd, imagetype, hbitmap)	\
	(BOOL)SNDMSG((hwnd), BEM_SETIMAGE, (WPARAM)(imagetype), (LPARAM)(hbitmap))
// IMAGE_BITMAP / IMAGE_ICON

#define BEM_GETIMAGE			(BEM_FIRST+3)
#define ButtonCtrl_GetImage(hwnd, imagetype, hbitmap)	\
	(BOOL)SNDMSG((hwnd), BEM_GETIMAGE, (WPARAM)(imagetype), (LPARAM)(hbitmap))

#define BEM_LAST				BEM_GETIMAGE

// =====================================================================
// Уведомления

#define BEN_FIRST				(BEM_LAST+1)

#define BEN_DROPDOWN			(BEN_FIRST+0)
#define BEN_CLICKED				(BEN_FIRST+1)
#define BEN_GETIMAGE			(BEN_FIRST+2)
#define BEN_SETIMAGE			(BEN_FIRST+3)

#define BEN_LAST				BEN_SETIMAGE

// =====================================================================
// Данные контрола

struct BUTTONCTRL
{
	DWORD		dwFlags;	// BF_*
	HFONT		hFont;
	HIMAGELIST	hImageList;
	INT			nImageIndex;
	LPVOID		hImage;
	UINT		nSplitY;
};

// =====================================================================
// Флаги состояния члена dwFlags структуры данных контрола

#define BF_HIGHLIGHT			(1 << 0)
#define BF_PRESSED				(1 << 1)
#define BF_FOCUSED				(1 << 2)
#define BF_DROPDOWNHIGHLIGHT	(1 << 3)
#define BF_IMAGEBITMAP			(1 << 4)
#define BF_IMAGEICON			(1 << 5)

// =====================================================================
// Публичные прототипы 

ATOM Button_RegisterClass();
