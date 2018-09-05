
#include "stdafx.h"
#include "Button.h"

#define STRSAFE_NO_CB_FUNCTIONS
#include <strsafe.h>

#pragma comment(lib, "Msimg32.lib") // для AlphaBlend

LRESULT CALLBACK Button_WndProc(HWND, UINT, WPARAM, LPARAM);

#define TEXT_MAX_COUNT				50

// Константы цветов
const COLORREF crParrentBk = RGB(243, 243, 243);
const COLORREF crNormal = crParrentBk;
const COLORREF crHighlighted = RGB(198, 198, 198);
const COLORREF crPressed = RGB(177, 177, 177);
const COLORREF crFocused = crHighlighted;
const COLORREF crArrow = RGB(109, 109, 109);

// Пропорции частей кнопки
const FLOAT ftRatioImage = 0.54f;
const FLOAT ftRatioInteractive = 1 - ftRatioImage;				// = 0.46f
const FLOAT ftRatioText = ftRatioInteractive * 0.53f;			// = 0.24f
const FLOAT ftRatioArrow = ftRatioInteractive - ftRatioText;	// = 0.22f

UINT p = 0;

void debug(HWND hWnd)
{
	BUTTONCTRL * pWndData = (BUTTONCTRL *)GetWindowLongPtr(hWnd, NULL);

	// Debug
	TCHAR szText[255];
	StringCchPrintf(szText, 255, TEXT("H%i P%i F%i H%i P%i"),
		pWndData->dwFlags & BF_HIGHLIGHT,
		pWndData->dwFlags & BF_PRESSED,
		pWndData->dwFlags & BF_FOCUSED,
		pWndData->dwFlags & BF_DROPDOWNHIGHLIGHT,
		p);
	//SetWindowText(GetParent(hWnd), szText);
}


void PreMultipliedAlpha(BITMAP * pbm, LPVOID pvBitsDest)
{
	for (DWORD y = 0; y < (DWORD)pbm->bmHeight; y++)
		for (DWORD x = 0; x < (DWORD)pbm->bmWidth; x++)
		{
			DWORD nIndex = x + y * pbm->bmWidth;
			DWORD crCurrent = ((DWORD*)pbm->bmBits)[nIndex];

			DWORD uR = (BYTE)(GetRValue(crCurrent) * GetAValue(crCurrent) >> 8);
			DWORD uG = (BYTE)(GetGValue(crCurrent) * GetAValue(crCurrent) >> 8);
			DWORD uB = (BYTE)(GetBValue(crCurrent) * GetAValue(crCurrent) >> 8);
			DWORD uA = GetAValue(crCurrent);
			((DWORD*)pvBitsDest)[nIndex] = (uA << 24) | RGB(uR, uG, uB);
		}
}


VOID DrawArrow(HDC hdc, RECT *rcArea)
{
	const INT sizeArrow = 3; // От подошвы до острия или *2 слева на права
	INT  x = rcArea->left + RECTWIDTH((*rcArea)) / 2;
	INT  y = rcArea->top + RECTHEIGHT((*rcArea)) / 2;
	
	HGDIOBJ hOldBrush = SelectObject(hdc, GetStockObject(DC_BRUSH));
	HGDIOBJ hOldPen = SelectObject(hdc, GetStockObject(DC_PEN));
	{
		COLORREF crOldBrush = SetDCBrushColor(hdc, crArrow);
		COLORREF crOldPen = SetDCPenColor(hdc, crArrow);
		POINT ptArrow[] = {
			{ x - sizeArrow + 1, y - sizeArrow / 2 },
			{ x + sizeArrow - 1, y - sizeArrow / 2 },
			{ x, y + sizeArrow / 2 }
		};
		Polygon(hdc, ptArrow, sizeof(ptArrow) / sizeof(ptArrow[0]));
		SetDCPenColor(hdc, crOldPen);
		SetDCBrushColor(hdc, crOldBrush);
	}
	SelectObject(hdc, hOldPen);
	SelectObject(hdc, hOldBrush);
}

VOID RibbonCtl_DrawText(HWND hWnd, HDC hdc, RECT *rcArea)
{
	BUTTONCTRL * pCtlData = (BUTTONCTRL *)GetWindowLongPtr(hWnd, NULL);
	
}


ATOM Button_RegisterClass()
{
	WNDCLASSEX wcex;
	wcex.cbSize = sizeof(wcex);
	wcex.style = 0;
	wcex.lpfnWndProc = Button_WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = sizeof(LPVOID);
	wcex.hInstance = GetModuleHandle(NULL);
	wcex.hIcon = NULL;
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = NULL;
	wcex.lpszMenuName = NULL;
	wcex.lpszClassName = WC_BUTTONCTRL;
	wcex.hIconSm = NULL;

	return RegisterClassEx(&wcex);
}


LRESULT CALLBACK Button_WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	RECT rcClient; // клиенская область компонента
	GetClientRect(hWnd, &rcClient);
	
	BUTTONCTRL * pCtlData = (BUTTONCTRL *)GetWindowLongPtr(hWnd, NULL);
	DWORD dwStyle = (DWORD)GetWindowLong(hWnd, GWL_STYLE);

	switch (uMsg)
	{
	case WM_CREATE:
	{
		CREATESTRUCT *pCreateStrcut = (CREATESTRUCT *)lParam;
		GetClientRect(hWnd, &rcClient);

		pCtlData = new BUTTONCTRL;
		SetWindowLongPtr(hWnd, NULL, (LONG_PTR)pCtlData);
		ZeroMemory(pCtlData, sizeof(*pCtlData));
		pCtlData->dwFlags = 0;
		pCtlData->nImageIndex = -1;
		

		// Меняем стандартный шрифт компонента
		HDC hdc = GetDC(hWnd);
		LONG lfHeight = -MulDiv(9, GetDeviceCaps(hdc, LOGPIXELSY), 72);
		ReleaseDC(hWnd, hdc);

		LOGFONT lf;
		lf.lfHeight = lfHeight;				// высота
		lf.lfWidth = 0;						// ширина
		lf.lfEscapement = NULL;				// не повёрнутый
		lf.lfOrientation = NULL;
		lf.lfWeight = FW_NORMAL;			// толщина
		lf.lfItalic = NULL;
		lf.lfUnderline = NULL;				// не подчёркнутый;
		lf.lfStrikeOut = NULL;				// не перечёркнутый
		lf.lfCharSet = DEFAULT_CHARSET;		// значение по умолчанию
		lf.lfOutPrecision = NULL;
		lf.lfClipPrecision = NULL;
		lf.lfQuality = PROOF_QUALITY;
		lf.lfPitchAndFamily = DEFAULT_PITCH; //значения по умолчанию
		StringCchCopy(lf.lfFaceName, LF_FACESIZE, TEXT("Segoe UI"));
		pCtlData->hFont = CreateFontIndirect(&lf);

		return 0;
	}

	case WM_DESTROY:
	{
		DeleteObject(pCtlData->hImage);
		delete pCtlData;

		return 0;
	}

	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hWnd, &ps);
		{
			// Задаем размер контейнера стрелки
			//pCtlData->nArrowWidth = 4 + 3 + 4;
			pCtlData->nArrowWidth = INT(RECTHEIGHT(rcClient) * ftRatioArrow + 0.5f);

			// Рисуем стрелку
			if (dwStyle & BCS_ARROW)
			{
				RECT rcArrow;
				if (dwStyle & BCS_SPLITRIGHT)
				{	
					// cправа
					SetRect(&rcArrow, rcClient.right - pCtlData->nArrowWidth,
						rcClient.top, rcClient.right, rcClient.bottom);
				} 
				else	
				{
					// снизу
					SetRect(&rcArrow, rcClient.left, rcClient.bottom - pCtlData->nArrowWidth,
						rcClient.right, rcClient.bottom);
				}
				DrawArrow(hdc, &rcArrow);
			}

			// Узнаем Размер изображения
			SIZE sizeImage = { 0 };
			if (pCtlData->hImage)
			{
				BITMAP bm;
				GetObject(pCtlData->hImage, sizeof(bm), &bm);
				sizeImage.cx = bm.bmWidth;
				sizeImage.cy = bm.bmHeight;
			}
			else if (pCtlData->nImageIndex > -1 && pCtlData->hImageList)
			{
				ImageList_GetIconSize(pCtlData->hImageList, (INT*)&sizeImage.cx, (INT*)&sizeImage.cy);
			}
			
			// Узнаем Размер текста
			SIZE  sizeWnd = { RECTWIDTH(rcClient), RECTHEIGHT(rcClient) }; // Размер окна
			INT   cchText = GetWindowTextLength(hWnd);	// Длина строки контрола
			TCHAR szBuffer[TEXT_MAX_COUNT + 1] = { 0 };	// Буффер с текстом 
			SIZE  sizeText = { 0 };						// Размер текстовой строки
			if (cchText)
			{
				GetWindowText(hWnd, szBuffer, TEXT_MAX_COUNT);
				GetTextExtentPoint32(hdc, szBuffer, cchText, &sizeText);
			}
			
			// Будет ли отрисовываться текст
			BOOL fText =
				cchText > 0 &&
				INT(RECTHEIGHT(rcClient) * ftRatioImage + 0.5f) - sizeImage.cy - 1 * 2 >= 0; // Отступ (1) * два раза

			// Расчитываем позицию изображения
			POINT ptImage = { 0 };
			if (dwStyle & BCS_SPLITRIGHT)
			{
				// Права
				ptImage.y = rcClient.top + (RECTHEIGHT(rcClient) - sizeImage.cy) / 2;
				if (fText)
				{
					// Текст влазиет
					ptImage.x = ptImage.y;
				}
				else
				{
					// Текст не влазиет
					ptImage.x = ptImage.y;
				}
			}
			else
			{
				// Низ
				ptImage.x = (RECTWIDTH(rcClient) - sizeImage.cx) / 2;
				if (fText)
					ptImage.y = (INT(RECTHEIGHT(rcClient) * ftRatioImage + 0.5f) - sizeImage.cy) / 2 + 1; // Текст влазиет
				else
					ptImage.y = (INT(RECTHEIGHT(rcClient) * (ftRatioImage + ftRatioText) + 0.5f) - sizeImage.cy) / 2; // Текст не влазиет
			}

			// Если иконка больше расчитанного под нее места
			const INT nCriticalOffset = 1;
			if (RECTWIDTH(rcClient) - 2 <= sizeImage.cx + nCriticalOffset * 2 ||
				INT(RECTHEIGHT(rcClient) * (ftRatioImage + ftRatioText) + 0.5f) - 2 <= sizeImage.cy + nCriticalOffset * 2 &&
				!fText) 
			{
				MessageBox(hWnd, TEXT("Иконка 32 px слишком большая, нужно рисовать иконку в 16 px"),
					TEXT("Caption"), 0);
			}
			
			// Рисуем изображение
			if (pCtlData->hImage)
			{
				HDC hdcMem = CreateCompatibleDC(hdc);
				HBITMAP hbmOld = (HBITMAP)SelectObject(hdcMem, pCtlData->hImage);
				{
					BITMAP bm;
					GetObject(pCtlData->hImage, sizeof(bm), &bm);

					BLENDFUNCTION bf;
					bf.BlendOp = AC_SRC_OVER;
					bf.BlendFlags = 0;
					bf.SourceConstantAlpha = 0xff;
					bf.AlphaFormat = AC_SRC_ALPHA;

					AlphaBlend(hdc, ptImage.x, ptImage.y, bm.bmWidth, bm.bmWidth,
						hdcMem, 0, 0, sizeImage.cx, sizeImage.cy, bf);
				}
				SelectObject(hdcMem, hbmOld);
				DeleteDC(hdcMem);
			}
			else if (pCtlData->nImageIndex > -1 && pCtlData->hImageList)
			{
				ImageList_Draw(pCtlData->hImageList, pCtlData->nImageIndex, hdc, ptImage.x, ptImage.y, 0);
			}
			
			// Рисуем текст
			if (fText)
			{
				RECT rcText;
				if (dwStyle & BCS_SPLITRIGHT)
				{
					// Право
					rcText.left = ptImage.x + sizeImage.cx + 3;
					rcText.top = rcClient.top;
					rcText.right = rcClient.right;
					rcText.bottom = rcClient.bottom;
				}
				else
				{
					// Низ
					rcText.left = rcClient.left;
					rcText.top = INT(RECTHEIGHT(rcClient) * ftRatioImage + 0.5f);
					rcText.bottom = rcClient.bottom - pCtlData->nArrowWidth;
					rcText.right = rcClient.right;
					
				}
				HGDIOBJ hOldFont = SelectObject(hdc, pCtlData->hFont); // старый шрифт
				{
					SetBkMode(hdc, TRANSPARENT);
					if (dwStyle & BCS_MULTILINE)
					{
						// Многострочный текст
						MessageBox(hWnd, TEXT("Многострочный текст"),
							TEXT("Caption"), 0);
					}
					else
					{
						DrawText(hdc, szBuffer, (INT)cchText, &rcText, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
					}
				}
				SelectObject(hdc, hOldFont);
			}

			// Вычисляем контейнер для интеректива
			if (dwStyle & BCS_SPLITRIGHT)
			{
				// справа 
				SetRect(&pCtlData->rcSplit, rcClient.right - pCtlData->nArrowWidth,
					rcClient.top, rcClient.right, rcClient.bottom);
			}
			else
			{
				// снизу 
				SetRect(&pCtlData->rcSplit, rcClient.left, rcClient.bottom - pCtlData->nArrowWidth,
					rcClient.right, rcClient.bottom);
				if (fText)
					pCtlData->rcSplit.top -= INT(RECTHEIGHT(rcClient) * ftRatioText + 0.5f);
			}
			
		}
		EndPaint(hWnd, &ps);
		return 0;
	}

	case WM_ERASEBKGND:
	{
		HDC	   hdc = (HDC)wParam;
		HBRUSH hDCBrush = (HBRUSH)GetStockObject(DC_BRUSH);
		COLORREF crOldBrush = 0;

		// Запрашиваем цвет фона у родителя
		NMHDR nmhdr;
		nmhdr.hwndFrom = hWnd;
		nmhdr.idFrom = GetWindowLong(hWnd, GWL_ID);
		nmhdr.code = BEN_BGCOLOR;
		COLORREF crParrent = (COLORREF)SNDMSG(GetParent(hWnd), WM_NOTIFY,
			(WPARAM)nmhdr.idFrom, (LPARAM)&nmhdr);
		
		// Определяемся с цветом
		COLORREF crCurrent = (!crParrent) ? crNormal : crParrent;

		if (pCtlData->dwFlags & BF_PRESSED)
			crCurrent = crPressed;
		else if (pCtlData->dwFlags & BF_HIGHLIGHT)
			crCurrent = crHighlighted;
		
		RECT rcFill;
		CopyRect(&rcFill, &rcClient);

		if (dwStyle & BCS_SPLITBUTTON)
		{
			// Определяем выделенный прямоугольник
			if (dwStyle & BCS_SPLITRIGHT)
			{
				if (pCtlData->dwFlags & BF_DROPDOWNHIGHLIGHT)
					rcFill.left = pCtlData->rcSplit.left;		// право
				else
					rcFill.right = pCtlData->rcSplit.left;		// лево
			}
			else
			{
				if (pCtlData->dwFlags & BF_DROPDOWNHIGHLIGHT)
					rcFill.top = pCtlData->rcSplit.top;			// низ
				else
				 	rcFill.bottom = pCtlData->rcSplit.top;		// верх
			}
						
			// Закрашиваем оставшийся обычный прямоугольник
			RECT rcNorm;
			SubtractRect(&rcNorm, &rcClient, &rcFill);
			crOldBrush = SetDCBrushColor(hdc, (!crParrent) ? crNormal : crParrent);
			FillRect(hdc, &rcNorm, hDCBrush);

			// Рамка
			if (pCtlData->dwFlags & BF_HIGHLIGHT)
			{
				SetDCBrushColor(hdc, crHighlighted);
				FrameRect(hdc, &rcClient, hDCBrush);
			}
		}

		SetDCBrushColor(hdc, crCurrent);
		FillRect(hdc, &rcFill, hDCBrush); // Закрашиваем выделенный прямоугольник
		SetDCBrushColor(hdc, crOldBrush);

		return TRUE;
	}

	case WM_SETFONT:
	{
		HFONT	hFont = (HFONT)wParam;
		BOOL	fRedraw = (BOOL)lParam;

		pCtlData->hFont = hFont;

		return NULL;
	}

	case WM_GETFONT:
	{
		return (LRESULT)pCtlData->hFont;
	}

	case WM_MOUSEMOVE:
	{
		POINT ptPos = { GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam) };

		// Доп секция
		if (dwStyle & BCS_SPLITBUTTON)
		{
			// Проверяем попадания мыши в прямоугольник
			if (PtInRect(&pCtlData->rcSplit, ptPos))
			{
				if (!(pCtlData->dwFlags & BF_DROPDOWNHIGHLIGHT))
				{
					pCtlData->dwFlags |= BF_DROPDOWNHIGHLIGHT;
					InvalidateRect(hWnd, 0, TRUE);
				}
			}
			else
			{
				if (pCtlData->dwFlags & BF_DROPDOWNHIGHLIGHT)
				{
					pCtlData->dwFlags &= ~BF_DROPDOWNHIGHLIGHT;
					InvalidateRect(hWnd, 0, TRUE);
				}
			}
		}

		// Обыч наведение
		if (!(pCtlData->dwFlags & BF_HIGHLIGHT))
		{
			pCtlData->dwFlags |= BF_HIGHLIGHT;
			InvalidateRect(hWnd, 0, TRUE);
		}

		// Включаем сообщение WM_MOUSELEAVE
		TRACKMOUSEEVENT tme;
		tme.cbSize = sizeof(tme);
		tme.dwFlags = TME_LEAVE;
		tme.hwndTrack = hWnd;
		tme.dwHoverTime = NULL;
		_TrackMouseEvent(&tme);

		debug(hWnd);

		return 0;
	}

	case WM_LBUTTONDOWN:
	{
		POINT ptPos = { GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam) };

		pCtlData->dwFlags &= ~BF_FOCUSED;

		if (!(pCtlData->dwFlags & BF_PRESSED))
		{
			pCtlData->dwFlags |= BF_PRESSED;
			InvalidateRect(hWnd, 0, TRUE);
		}

		// Отправляем команду или уведомление
		UINT nID = (UINT)GetWindowLong(hWnd, GWL_ID);
		HWND hParent = GetParent(hWnd);
		if (pCtlData->dwFlags & BF_DROPDOWNHIGHLIGHT)
		{
			NMBEDROPDOWN dropdown;
			dropdown.nmhdr.hwndFrom = hWnd;
			dropdown.nmhdr.idFrom = nID;
			dropdown.nmhdr.code = BEN_DROPDOWN;
			CopyRect(&dropdown.rcButton, &rcClient);
			SNDMSG(hParent, WM_NOTIFY, (WPARAM)nID, (LPARAM)&dropdown);
		}
		else
			SNDMSG(hParent, WM_COMMAND, (WPARAM)nID, (LPARAM)hWnd);

		return 0;
	}

	case WM_LBUTTONUP:
	{
		if (pCtlData->dwFlags & BF_PRESSED)
		{
			InvalidateRect(hWnd, 0, TRUE);
			pCtlData->dwFlags &= ~BF_PRESSED;
		}

		return 0;
	}

	case WM_RBUTTONUP:
	{
#ifdef _DEBUG
		HDC hdc = GetDC(hWnd);
		{
			SetDCBrushColor(hdc, RGB(0, 255, 0));
			FillRect(hdc, &rcClient, (HBRUSH)GetStockObject(DC_BRUSH));
		}
		ReleaseDC(hWnd, hdc);
#endif
		return 0;
	}

	case WM_MOUSELEAVE:
	{
		pCtlData->dwFlags &= ~BF_HIGHLIGHT;
		pCtlData->dwFlags &= ~BF_PRESSED;
		pCtlData->dwFlags &= ~BF_DROPDOWNHIGHLIGHT;
		InvalidateRect(hWnd, 0, TRUE);

		return 0;
	}

	case WM_SETFOCUS:
	{
		HWND hWndLoseFocus = (HWND)wParam;

		// Уведомляем родителя о получении фокуса
		if (dwStyle & BCS_NOTIFY)
		{
			NMHDR nmhdr;
			nmhdr.hwndFrom = hWnd;
			nmhdr.idFrom = GetWindowLong(hWnd, GWL_ID);
			nmhdr.code = BEN_SETFOCUS;
			SNDMSG(GetParent(hWnd), WM_NOTIFY, (WPARAM)nmhdr.idFrom, (LPARAM)&nmhdr);
		}

		pCtlData->dwFlags |= BF_FOCUSED;

		return 0;
	}

	case WM_KILLFOCUS:
	{
		HWND hWndGetFocus = (HWND)wParam;

		// Уведомляем родителя о потере фокуса
		if (dwStyle & BCS_NOTIFY)
		{
			NMHDR nmhdr;
			nmhdr.hwndFrom = hWnd;
			nmhdr.idFrom = GetWindowLong(hWnd, GWL_ID);
			nmhdr.code = BEN_KILLFOCUS;
			SNDMSG(GetParent(hWnd), WM_NOTIFY, (WPARAM)nmhdr.idFrom, (LPARAM)&nmhdr);
		}
			
		pCtlData->dwFlags &= ~BF_FOCUSED;

		return 0;
	}

	case WM_KEYDOWN:
	{
		UINT nVirtKey = (UINT)wParam;

		return 0;
	}

	case WM_SIZE:
	{
		// TODO удалить это сообщение (WM_SIZE)
		InvalidateRect(hWnd, 0, TRUE);
		return 0;
	}

	// --------------------------------

	case BEM_SETIMAGELIST:
	{
		HIMAGELIST hImgList = (HIMAGELIST)lParam;
		if (!hImgList)
			return FALSE;

		HIMAGELIST hOldImgList = pCtlData->hImageList;
		pCtlData->hImageList = hImgList;
		return (LRESULT)hOldImgList;
	}

	case BEM_GETIMAGELIST:
	{
		return (LRESULT)pCtlData->hImageList;
	}

	case BEM_SETIMAGE:
	{
		UINT nImageType = (UINT)wParam;
		LPVOID pImage = (LPVOID)lParam;

		if (!pImage)
			return FALSE;

		INT nSizeImage = 0;

		switch (nImageType)
		{
		case IMAGE_BITMAP:
		{
			pCtlData->dwFlags |= BF_IMAGEBITMAP;
			pCtlData->dwFlags &= ~BF_IMAGEICON;

			BITMAP bm;
			GetObject(pImage, sizeof(bm), &bm);

			nSizeImage = bm.bmWidth;

			BITMAPINFO bmi;
			bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
			bmi.bmiHeader.biWidth = bm.bmWidth;
			bmi.bmiHeader.biHeight = bm.bmHeight;
			bmi.bmiHeader.biPlanes = bm.bmPlanes;
			bmi.bmiHeader.biBitCount = 32;         // four 8-bit components 
			bmi.bmiHeader.biCompression = BI_RGB;
			bmi.bmiHeader.biSizeImage = bmi.bmiHeader.biWidth * bmi.bmiHeader.biHeight * (bmi.bmiHeader.biBitCount / 8);
			LPVOID pvBits = nullptr;

			HDC hdc = GetDC(hWnd);
			{
				pCtlData->hImage = CreateDIBSection(hdc, &bmi, DIB_RGB_COLORS, &pvBits, NULL, 0x00);
			}
			ReleaseDC(hWnd, hdc);

			PreMultipliedAlpha(&bm, pvBits);
			break;
		}

		case IMAGE_ICON:
			pCtlData->dwFlags |= BF_IMAGEICON;
			pCtlData->dwFlags &= ~BF_IMAGEBITMAP;
			return FALSE;
			break;

		default:
			return FALSE;
		}

		// TODO Уведомление *_SETIMAGE

		// Перерисовка иконки
		UINT nOffset = (RECTWIDTH(rcClient) - nSizeImage) / 2 + 1;
		RECT rcImage;
		SetRect(&rcImage,
			rcClient.left + nOffset,
			pCtlData->nArrowWidth - nSizeImage - 1,
			rcClient.left + nOffset + nSizeImage,
			pCtlData->nArrowWidth - 1
		);
		InvalidateRect(hWnd, &rcImage, TRUE);

		return TRUE;
	}

	default:
		return DefWindowProc(hWnd, uMsg, wParam, lParam);
	}
}