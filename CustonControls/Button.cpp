
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
		pCtlData->nSplitWidth = INT((FLOAT)rcClient.top + (FLOAT)RECTHEIGHT(rcClient) *  0.53f + 0.5f);

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
		BUTTONCTRL * pWndData = (BUTTONCTRL *)GetWindowLongPtr(hWnd, NULL);

		DeleteObject(pCtlData->hImage);

		delete pWndData;

		return 0;
	}

	case WM_PAINT:
	{
		p++;

		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hWnd, &ps);
		{
			// Рисуем изображение
			if (pCtlData->hImage)
			{
				HDC hdcMem = CreateCompatibleDC(hdc);
				HBITMAP hbmOld = (HBITMAP)SelectObject(hdcMem, pCtlData->hImage);
				{
					BITMAP bm;
					GetObject(pCtlData->hImage, sizeof(bm), &bm);

					LONG nOffset = (RECTWIDTH(rcClient) - bm.bmWidth) / 2 + 1;
					POINT ptImage = {
						nOffset,
						(LONG)pCtlData->nSplitWidth - bm.bmWidth - 2
					};

					BLENDFUNCTION bf;
					bf.BlendOp = AC_SRC_OVER;
					bf.BlendFlags = 0;
					bf.SourceConstantAlpha = 0xff;
					bf.AlphaFormat = AC_SRC_ALPHA;

					AlphaBlend(hdc, ptImage.x, ptImage.y, bm.bmWidth, bm.bmWidth,
						hdcMem, 0, 0, 32, 32, bf);
				}
				SelectObject(hdcMem, hbmOld);
				DeleteDC(hdcMem);
			}
			/*
			else
			{
				UINT nSizeBmb = 32;
				RECT rcImage;
				UINT nOffset = (RECTWIDTH(rcClient) - nSizeBmb) / 2 + 1;
				SetRect(&rcImage,
					rcClient.left + nOffset,
					pCtlData->nSplitWidth - nSizeBmb - 1,
					rcClient.left + nOffset + nSizeBmb,
					pCtlData->nSplitWidth - 1
				);
				SetDCBrushColor(hdc, RGB(127, 0, 127));
				FillRect(hdc, &rcImage, (HBRUSH)GetStockObject(DC_BRUSH));
			}*/

			// Рисуем текст
			const UINT nTopOffset = 1;
			TCHAR szText[TEXT_MAX_COUNT + 1]; // Текст
			GetWindowText(hWnd, szText, TEXT_MAX_COUNT);
			size_t cchText = 0; // Количество символов
			StringCchLength(szText, TEXT_MAX_COUNT, &cchText);
			HFONT hOldFont = (HFONT)SelectObject(hdc, pCtlData->hFont); // старый шрифт
			{
				SetBkMode(hdc, TRANSPARENT);
				RECT rcTextSize; // Полученный габаритный контейнер текста
				DrawText(hdc, szText, (INT)cchText, &rcTextSize, DT_CALCRECT);
				RECT rcText; // Окончательный габаритный контейнер текста
				SetRect(&rcText,
					rcClient.left,
					pCtlData->nSplitWidth + nTopOffset,
					rcClient.right,
					pCtlData->nSplitWidth + nTopOffset + RECTHEIGHT(rcTextSize)
				);
				DrawText(hdc, szText, (INT)cchText, &rcText, DT_CENTER | DT_VCENTER);
			}
			SelectObject(hdc, hOldFont);

			// Рисуем стрелку
			if (dwStyle & BCS_DROPDOWN)
			{
				INT nBottomOffsetArrow = 9;
				INT  nHorizMid = RECTWIDTH(rcClient) / 2;
				HBRUSH hOldBrush = (HBRUSH)SelectObject(hdc, GetStockObject(DC_BRUSH));
				HPEN hOldPen = (HPEN)SelectObject(hdc, GetStockObject(DC_PEN));
				{
					SetDCBrushColor(hdc, crArrow);
					SetDCPenColor(hdc, crArrow);
					POINT ptArrow[] = {
						{ nHorizMid - 2, rcClient.bottom - nBottomOffsetArrow },
						{ nHorizMid + 2, rcClient.bottom - nBottomOffsetArrow },
						{ nHorizMid,     rcClient.bottom - nBottomOffsetArrow + 2 }
					};
					Polygon(hdc, ptArrow, sizeof(ptArrow) / sizeof(ptArrow[0]));
				}
				SelectObject(hdc, hOldPen);
				SelectObject(hdc, hOldBrush);
			}
		}
		EndPaint(hWnd, &ps);
		return 0;
	}

	case WM_ERASEBKGND:
	{
		HDC		hdc = (HDC)wParam;

		HBRUSH hDCBrush = (HBRUSH)GetStockObject(DC_BRUSH);

		COLORREF crCurrent = crNormal;

		if (pCtlData->dwFlags & BF_PRESSED)
			crCurrent = crPressed;
		else if (pCtlData->dwFlags & BF_HIGHLIGHT)
			crCurrent = crHighlighted;


		RECT rc;
		CopyRect(&rc, &rcClient);

		if (dwStyle & BCS_DROPDOWN)
		{
			// Выделенная
			if (pCtlData->dwFlags & BF_DROPDOWNHIGHLIGHT)
				rc.top = pCtlData->nSplitWidth; // Низ
			else
				rc.bottom = pCtlData->nSplitWidth; // Верх

			// Нормальная
			RECT rcNorm;
			SubtractRect(&rcNorm, &rcClient, &rc);
			SetDCBrushColor(hdc, crNormal);
			FillRect(hdc, &rcNorm, hDCBrush);

			// Рамка
			if (pCtlData->dwFlags & BF_HIGHLIGHT)
			{
				SetDCBrushColor(hdc, crHighlighted);
				FrameRect(hdc, &rcClient, hDCBrush);
			}
		}

		SetDCBrushColor(hdc, crCurrent);
		FillRect(hdc, &rc, hDCBrush);

		/*
		if (dwStyle & BCS_DROPDOWN)
		{
			if (pCtlData->dwFlags & BF_HIGHLIGHT)
			{
				SetDCBrushColor(hdc, crHighlighted);
				FrameRect(hdc, &rcClient, hDCBrush);
			}

			RECT rcHighlight;
			if (pCtlData->dwFlags & BF_DROPDOWNHIGHLIGHT)
			{
				SetRect(&rcHighlight, rcClient.left, pCtlData->nSplitWidth, rcClient.right, rcClient.bottom);
				SetRect(&rc, rcClient.left + 1, rcClient.top + 1, rcClient.right - 1, pCtlData->nSplitWidth);
			}
			else
			{
				SetRect(&rcHighlight, rcClient.left, rcClient.top, rcClient.right, pCtlData->nSplitWidth);
				SetRect(&rc, rcClient.left + 1, pCtlData->nSplitWidth, rcClient.right - 1, rcClient.bottom - 1);
			}

			// rcClient оставщейся области
			SetDCBrushColor(hdc, crNormal);
			FillRect(hdc, &rc, hDCBrush);

			SetDCBrushColor(hdc, crCurrent);
			FillRect(hdc, &rcHighlight, hDCBrush);
		}

	*/

	// debug
		debug(hWnd);

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
		if (dwStyle & BCS_DROPDOWN)
		{
			RECT rc;
			SetRect(&rc, rcClient.left, pCtlData->nSplitWidth,
				rcClient.right, rcClient.bottom);
			if (PtInRect(&rc, ptPos))
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
			NMHDR nmhdr;
			nmhdr.hwndFrom = hWnd;
			nmhdr.idFrom = nID;
			nmhdr.code = BEN_DROPDOWN;
			SNDMSG(hParent, WM_NOTIFY, (WPARAM)nID, (LPARAM)&nmhdr);
		}
		else
			SNDMSG(hParent, WM_COMMAND, (WPARAM)nID, (LPARAM)hWnd);

		return 0;
	}

	case WM_LBUTTONUP:
	{
		if ((pCtlData->dwFlags & BF_PRESSED))
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

		pCtlData->dwFlags |= BF_FOCUSED;

		return 0;
	}

	case WM_KILLFOCUS:
	{
		HWND hWndGetFocus = (HWND)wParam;

		pCtlData->dwFlags &= ~BF_FOCUSED;

		return 0;
	}

	case WM_KEYDOWN:
	{
		UINT nVirtKey = (UINT)wParam;

		return 0;
	}

	// --------------------------------

	case BEM_SETIMAGELIST:
	{
		HIMAGELIST hImgList = (HIMAGELIST)lParam;
		if (!hImgList)
			return FALSE;
		pCtlData->hImageList = hImgList;
		return TRUE;
	}

	case BEM_GETIMAGELIST:
	{
		HIMAGELIST *phImgList = (HIMAGELIST *)lParam;
		if (!phImgList)
			return FALSE;
		*phImgList = pCtlData->hImageList;
		return TRUE;
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
			pCtlData->nSplitWidth - nSizeImage - 1,
			rcClient.left + nOffset + nSizeImage,
			pCtlData->nSplitWidth - 1
		);
		InvalidateRect(hWnd, &rcImage, TRUE);

		return TRUE;
	}

	/*
	case BEM_GETIMAGE:
	{

		return FALSE;
	}
	*/

	default:
		return DefWindowProc(hWnd, uMsg, wParam, lParam);
	}
}