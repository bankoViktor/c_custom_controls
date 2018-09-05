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
// Указывает, что кнопка отображает текст.
#define BCS_TEXT				0x00000000L
// Выравнивает текст по левому краю в прямоугольнике кнопок.
//#define BCS_LEFT				0x00000001L
// Выравнивает текст по правому краю в прямоугольнике кнопок.
//#define BCS_RIGHT				0x00000002L
// Центрирует текст по горизонтали в прямоугольнике кнопок.
//#define BCS_CENTER				0x00000004L
// Размещает текст на верхней части прямоугольника кнопок.
//#define BCS_TOP					0x00000008L
// Размещает текст на нижней части прямоугольника кнопок.
//#define BCS_BOTTOM				0x00000010L
// Центрирует текст по вертикали в прямоугольнике кнопок.
//#define BCS_VCENTER				0x00000020L
// Текст размещается в несколько строк, если строка слишком длинная, 
// чтобы поместиться в одну строку в прямоугольнике кнопки.
#define BCS_MULTILINE			0x00000040L
// Кнопка будет отправлять коды уведомлений BEN_KILLFOCUS и BEN_SETFOCUS в родительское окно. 
// Обратите внимание, что кнопки отправляют код уведомления BEN_CLICKED независимо от этого стяля.
#define BCS_NOTIFY				0x00000080L
// В нижней части кнопки (или справа) появится стрелка
#define BCS_ARROW				0x00000100L
// Кнопка будет иметь дополнительную секцию снизу со стрелкой
// раскрывающегося списка. Вместо сообщения WM_COMMAND, используемого для обычных кнопок,
// кнопка отправит код уведомления BEN_DROPDOWN.
#define BCS_SPLITBUTTON			0x00000200L
// Дополнительная секция будет расплолагаться справа.
#define BCS_SPLITRIGHT			0x00000400L
#define BCS_DROPDOWN			(BCS_ARROW|BCS_SPLITBUTTON)



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
// Данные контрола

typedef struct tagBUTTONCTRL
{
	DWORD		dwFlags;			// BF_*
	HFONT		hFont;
	HIMAGELIST	hImageList;
	INT			nImageIndex;
	LPVOID		hImage;				// Bitmap или Icon этот челен имеет приоритет выше, чем hImageList
	UINT		nArrowWidth;		// Ширина области со стрелкой
	RECT		rcSplit;			// Прямоугольник для интерактива
} BUTTONCTRL, *LPBUTTONCTRL;

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
