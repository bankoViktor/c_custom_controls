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
// В нижней части кнопки (или справа) появится стрелка
#define BCS_ARROW				0x00000001L
// Кнопка будет иметь дополнительную секцию снизу со стрелкой
// раскрывающегося списка. Вместо сообщения WM_COMMAND, используемого для обычных кнопок,
// кнопка отправит код уведомления BEN_DROPDOWN.
#define BCS_SPLIT				0x00000002L
#define BCS_DROPDOWN			(BCS_ARROW|BCS_SPLIT)
// Компонент сам подберет себе подходящюю ширину при создании.
#define BCS_AUTOWIDTH			0x00000004L
// Текст будет располагаться справа он изображения
//#define BCS_RIGHT				0x00000004L



// =====================================================================
// Сообщения

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
// Уведомления

#define BEN_FIRST				(BEM_LAST+1)

#define BEN_CLICKED				(BEN_FIRST+0)
#define BEN_DROPDOWN			(BEN_FIRST+1)
#define BEN_BGCOLOR				(BEN_FIRST+2)
//#define BEN_GETIMAGE			(BEN_FIRST+2)
//#define BEN_SETIMAGE			(BEN_FIRST+3)

#define BEN_LAST				BEN_DROPDOWN

typedef struct tagNMBEDROPDOWN {
	NMHDR nmhdr;
	RECT  rcButton; // ??? нужна ли
} NMBEDROPDOWN;

// =====================================================================
// Данные контрола

struct BUTTONCTRL
{
	DWORD		dwFlags;		// BF_*
	HFONT		hFont;
	HIMAGELIST	hImageList;
	INT			nImageIndex;
	LPVOID		hImage;			// Bitmap или Icon
	UINT		nSplitWidth;
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
