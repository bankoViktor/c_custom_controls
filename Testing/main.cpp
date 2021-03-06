// WinAppTemplate.cpp : Определяет точку входа для приложения.
//

#include "stdafx.h"
#include "main.h"
#include <CommCtrl.h>

#ifdef _DEBUG
#define _CRTDBG_MAP_ALLOC 
#include <stdlib.h>
#include <crtdbg.h>  
#endif

#include "..\CustomControls\CustomCtrl.h"


#ifdef _WIN64
#pragma comment(lib, "..\\CustomControls\\x64\\Debug\\CustomControls.lib")
#else
#pragma comment(lib, "..\\CustomControls\\x86\\Debug\\CustomControls.lib")
#endif

#define MAX_LOADSTRING			100

#define OFFSET					200	
#define CTRL_HEIGHT				25
#define CTRL_WIDTH				160
#define CTRL_VOFFSET			CTRL_HEIGHT + 30
#define CTRL_LINE_1				OFFSET + CTRL_VOFFSET * 1
#define CTRL_LINE_2				OFFSET + CTRL_VOFFSET * 2
#define CTRL_LINE_3				OFFSET + CTRL_VOFFSET * 3
#define CTRL_LINE_4				OFFSET + CTRL_VOFFSET * 4
#define CTRL_LINE_5				OFFSET + CTRL_VOFFSET * 5
#define CTRL_LINE_6				OFFSET + CTRL_VOFFSET * 6
#define CTRL_LINE_7				OFFSET + CTRL_VOFFSET * 7
#define CTRL_LINE_8				OFFSET + CTRL_VOFFSET * 8
#define CTRL_LINE_9				OFFSET + CTRL_VOFFSET * 9
#define CTRL_LINE_10			OFFSET + CTRL_VOFFSET * 10
#define CTRL_LINE_11			OFFSET + CTRL_VOFFSET * 11
#define CTRL_LINE_12			OFFSET + CTRL_VOFFSET * 12

#define IDC_BVSRIBBOB			150 
#define IDC_INSERTITEM			801
#define IDC_GETITEM				802
#define IDC_SETITEM				803
#define IDC_GETITEMRECT			804
#define IDC_SETCOMPACTMODE		805
#define IDC_GETCOMPACTMODE		806
#define IDC_GETITEMACTIVE		807
#define IDC_SETITEMHEIGHT		808
#define IDC_GETITEMHEIGHT		809
#define IDC_MOVEITEM			810
#define IDC_DELETEITEM			811
#define IDC_DELETEALL			812
#define IDC_SWAPITEMS			813
#define IDC_BUTTON_1			814
#define IDC_BUTTON_2			815

#define IDC_INDEX				831
#define IDC_TEXT				832
#define IDC_RECT				833	 
#define IDC_BOOL				834
#define IDC_INDEX_1				835
#define IDC_INDEX_2				836

const COLORREF crBackground = RGB(240, 240, 180);


// Глобальные переменные:
HINSTANCE hInst;                                // текущий экземпляр
WCHAR	  szTitle[MAX_LOADSTRING];              // Текст строки заголовка
WCHAR	  szWindowClass[MAX_LOADSTRING];        // имя класса главного окна

// Отправить объявления функций, включенных в этот модуль кода:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, INT);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

INT APIENTRY _tWinMain(_In_ HINSTANCE hInstance,
                      _In_opt_ HINSTANCE,
                      _In_ LPTSTR    lpCmdLine,
                      _In_ INT       nCmdShow)
{
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: Разместите код здесь.

    // Инициализация глобальных строк
    LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadString(hInstance, IDC_WINAPPTEMPLATE, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Выполнить инициализацию приложения:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_WINAPPTEMPLATE));

    MSG msg;

    // Цикл основного сообщения:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}


// Регистрирует класс окна.
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEX wcex;
    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_WINAPPTEMPLATE));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = 0;
    wcex.lpszMenuName   = 0;
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}


// В этой функции маркер экземпляра сохраняется в глобальной переменной, а также
// создается и выводится главное окно программы.
BOOL InitInstance(HINSTANCE hInstance, INT nCmdShow)
{
	Init_CustonControl();

#ifdef _DEBUG
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

   hInst = hInstance; // Сохранить маркер экземпляра в глобальной переменной

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}


// Обрабатывает сообщения в главном окне.
LRESULT CALLBACK WndProc(HWND hWnd, UINT nMsg, WPARAM wParam, LPARAM lParam)
{
	RECT rcClient;
	GetClientRect(hWnd, &rcClient);

	switch (nMsg)
	{
	case WM_CREATE:
	{
		/*
		Ribbon_RegisterClass(hInst);
		Button_RegisterClass(hInst);
		HWND hRibbon = CreateWindow(WC_RIBBON, nullptr,
			WS_CHILD | WS_VISIBLE | WS_TABSTOP,
			0, 0, 0, 0,
			hWnd, (HMENU)IDC_BVSRIBBOB, hInst, nullptr);

		
		RNITEM item;
		item.bmMask = RNIF_TEXT;

		item.pszText = (LPTSTR)TEXT("ГЛАВНАЯ");
		Ribbon_InsertItem(hRibbon, -1, &item);

		item.pszText = (LPTSTR)TEXT("ВСТАВКА");
		Ribbon_InsertItem(hRibbon, -1, &item);

		item.pszText = (LPTSTR)TEXT("КОНСТРУКТОР");
		Ribbon_InsertItem(hRibbon, -1, &item);

		item.pszText = (LPTSTR)TEXT("МАКЕТ");
		Ribbon_InsertItem(hRibbon, -1, &item);

		item.pszText = (LPTSTR)TEXT("ССЫЛКИ");
		Ribbon_InsertItem(hRibbon, -1, &item);

		item.pszText = (LPTSTR)TEXT("РАССЫЛКИ");
		Ribbon_InsertItem(hRibbon, -1, &item);

		item.pszText = (LPTSTR)TEXT("РЕЦЕНЗИРОВАНИЕ");
		Ribbon_InsertItem(hRibbon, -1, &item);

		// -------------------------------------------------------------

		HWND hPage_1 = CreateDialog(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hRibbon, Ribbon_PageProc);
		RECT rc;
		Ribbon_GetClientRect(hRibbon, &rc);
		SetWindowPos(hPage_1, nullptr, rc.left, rc.top, 0, 0, SWP_NOSIZE | SWP_NOACTIVATE);
		ShowWindow(hPage_1, SW_SHOW);

		// -------------------------------------------------------------

		CreateWindow(WC_BUTTON, TEXT("Insert Item"), WS_VISIBLE | WS_CHILD,
			20, CTRL_LINE_1, CTRL_WIDTH, CTRL_HEIGHT, hWnd, (HMENU)IDC_INSERTITEM, hInst, 0);

		CreateWindow(WC_EDIT, TEXT("-1"), WS_VISIBLE | WS_CHILD | WS_BORDER,
			20 + CTRL_WIDTH + 20, CTRL_LINE_1, 40, CTRL_HEIGHT, hWnd, (HMENU)IDC_INDEX, hInst, 0);

		CreateWindow(WC_EDIT, TEXT("Text"), WS_VISIBLE | WS_CHILD | WS_BORDER,
			20 + CTRL_WIDTH + 20 + 40 + 20, CTRL_LINE_1, 80, CTRL_HEIGHT, hWnd, (HMENU)IDC_TEXT, hInst, 0);

		CreateWindow(WC_BUTTON, TEXT("Get Item"), WS_VISIBLE | WS_CHILD,
			20, CTRL_LINE_2, CTRL_WIDTH, CTRL_HEIGHT, hWnd, (HMENU)IDC_GETITEM, hInst, 0);

		CreateWindow(WC_BUTTON, TEXT("Set Item"), WS_VISIBLE | WS_CHILD,
			20, CTRL_LINE_3, CTRL_WIDTH, CTRL_HEIGHT, hWnd, (HMENU)IDC_SETITEM, hInst, 0);

		CreateWindow(WC_BUTTON, TEXT("Get Item Rect"), WS_VISIBLE | WS_CHILD,
			20, CTRL_LINE_4, CTRL_WIDTH, CTRL_HEIGHT, hWnd, (HMENU)IDC_GETITEMRECT, hInst, 0);

		CreateWindow(WC_EDIT, TEXT(""), WS_VISIBLE | WS_CHILD | WS_BORDER,
			20 + CTRL_WIDTH + 20, CTRL_LINE_4, 300, CTRL_HEIGHT, hWnd, (HMENU)IDC_RECT, hInst, 0);

		CreateWindow(WC_BUTTON, TEXT("Compact Mode"), WS_VISIBLE | WS_CHILD,
			20, CTRL_LINE_5, CTRL_WIDTH, CTRL_HEIGHT, hWnd, (HMENU)IDC_SETCOMPACTMODE, hInst, 0);

		CreateWindow(WC_BUTTON, TEXT("Delete Item"), WS_VISIBLE | WS_CHILD,
			20, CTRL_LINE_6, CTRL_WIDTH, CTRL_HEIGHT, hWnd, (HMENU)IDC_DELETEITEM, hInst, 0);

		CreateWindow(WC_BUTTON, TEXT("Delete All"), WS_VISIBLE | WS_CHILD,
			20, CTRL_LINE_7, CTRL_WIDTH, CTRL_HEIGHT, hWnd, (HMENU)IDC_DELETEALL, hInst, 0);

		CreateWindow(WC_BUTTON, TEXT("Move Item"), WS_VISIBLE | WS_CHILD,
			20, CTRL_LINE_8, CTRL_WIDTH, CTRL_HEIGHT, hWnd, (HMENU)IDC_MOVEITEM, hInst, 0);

		CreateWindow(WC_EDIT, TEXT("6"), WS_VISIBLE | WS_CHILD | WS_BORDER,
			20 + CTRL_WIDTH + 20, CTRL_LINE_8, 40, CTRL_HEIGHT, hWnd, (HMENU)IDC_INDEX_1, hInst, 0);

		CreateWindow(WC_EDIT, TEXT("0"), WS_VISIBLE | WS_CHILD | WS_BORDER,
			20 + CTRL_WIDTH + 20 + 40 + 20, CTRL_LINE_8, 40, CTRL_HEIGHT, hWnd, (HMENU)IDC_INDEX_2, hInst, 0);

		CreateWindow(WC_BUTTON, TEXT("Swap Items"), WS_VISIBLE | WS_CHILD,
			20, CTRL_LINE_9, CTRL_WIDTH, CTRL_HEIGHT, hWnd, (HMENU)IDC_SWAPITEMS, hInst, 0);

			*/

		HINSTANCE hInstanceModule = GetModuleHandle(NULL);
		HINSTANCE hInstanceWnd = (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE);

		HWND hButton1 = CreateWindow(WC_BUTTONCTRL, TEXT("Вставить"),
			WS_VISIBLE | WS_CHILD | BCS_PUSHBUTTON, 500, 140, 53, 69, hWnd, (HMENU)IDC_BUTTON_1, hInstanceWnd, 0);
		HWND hButton2 = CreateWindow(WC_BUTTONCTRL, TEXT("Вставить"),
			WS_VISIBLE | WS_CHILD | BCS_DROPDOWN, 560, 140, 53, 69, hWnd, (HMENU)IDC_BUTTON_2, hInstanceWnd, 0);

		HBITMAP hBmp = (HBITMAP)LoadImage(hInstanceWnd, MAKEINTRESOURCE(IDB_BITMAP_A1),
			IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION);

		ButtonCtrl_SetImage(hButton1, IMAGE_BITMAP, hBmp);
		ButtonCtrl_SetImage(hButton2, IMAGE_BITMAP, hBmp);

		DeleteObject(hBmp);

		return 0;
	}

	case WM_NOTIFY:
	{
		INT nID = (INT)wParam;
		NMHDR * pnmhdr = (NMHDR *)lParam;

		if (pnmhdr->code == BEN_BGCOLOR)
		{
			return crBackground;
		}
		
		TCHAR szText[255];
		StringCchPrintf(szText, 255, TEXT("Clicked NOTIFY:%i / code:%i"), pnmhdr->idFrom, pnmhdr->code);
		SetWindowText(hWnd, szText);

		SetWindowText(hWnd, TEXT("Clicked NOTIFY"));

		return 0;
	}

	case WM_ERASEBKGND:
	{
		HDC hdc = (HDC)wParam;
		SetDCBrushColor(hdc, crBackground);
		FillRect(hdc, &rcClient, (HBRUSH)GetStockObject(DC_BRUSH));
		return TRUE;
	}

	case WM_SIZE:
	{
		UINT fwSizeType = (UINT)wParam;
		INT nWidth = LOWORD(lParam);
		INT nHeight = HIWORD(lParam);
		HWND hRibbon = GetDlgItem(hWnd, IDC_BVSRIBBOB);
		RECT rcRibbon;
		GetWindowRect(hRibbon, &rcRibbon);
		SetWindowPos(hRibbon, nullptr, NULL, NULL, nWidth, rcRibbon.bottom - rcRibbon.top, SWP_NOACTIVATE | SWP_NOMOVE);
		return 0;
	}

	case WM_COMMAND:
	{
		INT nID = LOWORD(wParam);

		const UINT cchBufferMax = 50;
		TCHAR szBuffer[cchBufferMax + 1] = { 0 };
		CHAR szBufferInt[4 + 1] = { 0 };

		GetWindowTextA(GetDlgItem(hWnd, IDC_INDEX), szBufferInt, 4);
		INT i = atoi(szBufferInt);

		//RNITEM item;
		//item.bmMask = RNIF_TEXT;

		// Разобрать выбор в меню:
		switch (nID)
		{
		case IDM_ABOUT:
			CreateDialog(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;

		case IDC_INSERTITEM:
		{
			//item.pszText = szBuffer;
			GetWindowText(GetDlgItem(hWnd, IDC_TEXT), szBuffer, cchBufferMax);
			//Ribbon_InsertItem(hRibbon, i, &item);
		}
		break;
		case IDC_GETITEM:
		{
			//item.cchTextMax = sizeof(szBuffer);
			//item.pszText = szBuffer;
			//Ribbon_GetItem(hRibbon, i, &item);
			SetWindowText(GetDlgItem(hWnd, IDC_TEXT), szBuffer);
		}
		break;
		case IDC_SETITEM:
		{
			//item.pszText = szBuffer;
			GetWindowText(GetDlgItem(hWnd, IDC_TEXT), szBuffer, cchBufferMax);
			//Ribbon_SetItem(hRibbon, i, &item);
		}
		break;
		case IDC_GETITEMRECT:
		{
			RECT rc = { 0 };
			//Ribbon_GetItemRect(hRibbon, i, &rc);
			StringCchPrintf(szBuffer, sizeof(szBuffer), TEXT("LT (%i, %i) RB(%i, %i) [%i x %i]"),
				rc.left, rc.top, rc.right, rc.bottom, rc.right - rc.left, rc.bottom - rc.top);
			SetWindowText(GetDlgItem(hWnd, IDC_RECT), szBuffer);
			//HWND hDlg = CreateDialog(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hRibbon, About);
			//ShowWindow(hDlg, SW_SHOW);
		}
		break;
		case IDC_SETCOMPACTMODE:
		{
			//BOOL fCompact = Ribbon_GetCompactModeState(hRibbon);
			//Ribbon_SetCompactMode(hRibbon, !fCompact);
		}
		break;
		case IDC_DELETEITEM:
		{
			//Ribbon_DeleteItem(hRibbon, i);
		}
		break;

		case IDC_DELETEALL:
		{
			//Ribbon_DeleteAllItems(hRibbon);
		}
		break;

		case IDC_MOVEITEM:
		{
			GetWindowTextA(GetDlgItem(hWnd, IDC_INDEX_1), szBufferInt, 4);
			INT i = atoi(szBufferInt);
			GetWindowTextA(GetDlgItem(hWnd, IDC_INDEX_2), szBufferInt, 4);
			INT i_new = atoi(szBufferInt);
			//Ribbon_MoveItem(hRibbon, i, i_new);
		}
		break;

		case IDC_SWAPITEMS:
		{
			GetWindowTextA(GetDlgItem(hWnd, IDC_INDEX_1), szBufferInt, 4);
			INT iA = atoi(szBufferInt);
			GetWindowTextA(GetDlgItem(hWnd, IDC_INDEX_2), szBufferInt, 4);
			INT iB = atoi(szBufferInt);
			//Ribbon_SwapItems(hRibbon, iA, iB);
		}
		break;

		case IDC_BUTTON_1:
		case IDC_BUTTON_2:
		{
			TCHAR szText[255];
			StringCchPrintf(szText, 255, TEXT("Clicked CMD:%i"), nID);
			SetWindowText(hWnd, szText);
			return 0;
		}
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		default:
			return DefWindowProc(hWnd, nMsg, wParam, lParam);
		}
		//InvalidateRect(hWnd, 0, TRUE);
		break;
	}

	case WM_PAINT:
	{
		//RIBBON *pWndData = (RIBBON*)GetWindowLongPtr(hRibbon, NULL);

		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hWnd, &ps);
		{
			/*
			const int cchBuffer = 255;
			const int left = 800;
			const int top = 200;
			const int voffset = 20;

			TCHAR szBuffer[cchBuffer + 1];
			SetBkMode(hdc, TRANSPARENT);

			StringCchPrintf(szBuffer, cchBuffer, TEXT("Flags:    %x"), pWndData->bmFlags);
			TextOut(hdc, left, top + voffset * 0, szBuffer, lstrlen(szBuffer));

			StringCchPrintf(szBuffer, cchBuffer, TEXT("Active Item:     %i"), pWndData->nItemActive);
			TextOut(hdc, left, top + voffset * 1, szBuffer, lstrlen(szBuffer));

			StringCchPrintf(szBuffer, cchBuffer, TEXT("Count Item:      %i"), pWndData->nCountItem);
			TextOut(hdc, left, top + voffset * 2, szBuffer, lstrlen(szBuffer));

			StringCchPrintf(szBuffer, cchBuffer, TEXT("Allocate Memory Item: %i"), pWndData->nCountItemAlloc);
			TextOut(hdc, left, top + voffset * 3, szBuffer, lstrlen(szBuffer));

			StringCchPrintf(szBuffer, cchBuffer, TEXT("Height:          %i"), pWndData->nHeight);
			TextOut(hdc, left, top + voffset * 4, szBuffer, lstrlen(szBuffer));

			StringCchPrintf(szBuffer, cchBuffer, TEXT("Height Item:     %i"), pWndData->nItemHeight);
			TextOut(hdc, left, top + voffset * 5, szBuffer, lstrlen(szBuffer));

			StringCchPrintf(szBuffer, cchBuffer, TEXT("Highlighte Item: %i"), pWndData->nItemHighlighted);
			TextOut(hdc, left, top + voffset * 6, szBuffer, lstrlen(szBuffer));

			int right = ITEM_OFFSET;
			if (pWndData->nCountItem > 0)
			{
				RNITEMEX	*pItemLast = (RNITEMEX *)pWndData->ppItems[pWndData->nCountItem - 1];
				right = pItemLast->nItemLeft + pItemLast->nItemWidth;
			}
			StringCchPrintf(szBuffer, cchBuffer, TEXT("Right:           %i"), right);
			TextOut(hdc, left, top + voffset * 7, szBuffer, lstrlen(szBuffer));
			*/
		}
		EndPaint(hWnd, &ps);
		break;
	}

	case WM_SYSKEYDOWN:
	{
		UINT nVirtKey = (UINT)wParam;
		UINT fRepet = LOWORD(lParam);
		UINT nScanCode = HIWORD(lParam) & 0x0F;
		BOOL fExtendKey = (BOOL)lParam & 0x1000000;
		BOOL fAltPressed = (BOOL)lParam & 20000000;
		BOOL fLastStateKey = (BOOL)lParam & 40000000;
		BOOL fDowningKey = (BOOL)lParam & 80000000;


		//if (fAltPressed)
		//	SetFocus(hRibbon);

		return 0;
	}

	case WM_DESTROY:
	{
		PostQuitMessage(0);
		break;
	}
        
    default:
        return DefWindowProc(hWnd, nMsg, wParam, lParam);
    }
	return 0;
}

// Обработчик сообщений для окна "О программе".
INT_PTR CALLBACK About(HWND hDlg, UINT nMsg, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);

    switch (nMsg)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}


INT_PTR CALLBACK Ribbon_PageProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_INITDIALOG:
	{
		return (INT_PTR)TRUE;
	}

	case WM_ERASEBKGND:
	{
		HWND hParentWnd = GetParent(hDlg);

		return TRUE;
	}

	case WM_DRAWITEM:
	{
		UINT nIDCtl = (UINT)wParam;
		DRAWITEMSTRUCT *pDIS = (DRAWITEMSTRUCT *)lParam;

		HDC hdc = pDIS->hDC;
		RECT * prc = &pDIS->rcItem;

		COLORREF crButton = 0;// RGB(228, 228, 228);

		if (pDIS->itemAction & ODA_DRAWENTIRE)
		{
			if (pDIS->itemState & ODS_HOTLIGHT)
			{
				SetDCBrushColor(hdc, RGB(0, 0, 255));
				FillRect(hdc, prc, (HBRUSH)GetStockObject(DC_BRUSH));
			}
			else
			{
				SetDCBrushColor(hdc, RGB(255, 0, 255));
				FillRect(hdc, prc, (HBRUSH)GetStockObject(DC_BRUSH));
			}
		}

		if (pDIS->itemAction & ODA_FOCUS)
		{
			RECT rc;
			SetRect(&rc, prc->left, prc->top, prc->right, prc->bottom);
			InflateRect(&rc, -3, -3);

			if (pDIS->itemState & ODS_FOCUS)
			{
				SetDCBrushColor(hdc, RGB(255, 0, 0));
				FillRect(hdc, &rc, (HBRUSH)GetStockObject(DC_BRUSH));
			}
			else
			{
				SetDCBrushColor(hdc, RGB(0, 255, 0));
				FillRect(hdc, &rc, (HBRUSH)GetStockObject(DC_BRUSH));
			}
		}

		/*
		if (pDIS->itemState & ODS_HOTLIGHT)
			crButton = RGB(255, 0, 0);
		else if (pDIS->itemState & ODS_DEFAULT)
			crButton = RGB(228, 228, 228);
		else if (pDIS->itemState & ODS_DISABLED)
			crButton = RGB(0, 255, 0);
		else if (pDIS->itemState & ODS_SELECTED)
			crButton = RGB(0, 255, 255);
		else
			crButton = RGB(255, 255, 0);;


		SetDCBrushColor(hdc, crButton);
		FillRect(hdc, prc, (HBRUSH)GetStockObject(DC_BRUSH));
		*/
		/*
		// Focus change?
		if (pDIS->itemAction & ODA_FOCUS) {
			// Toggle focus rectangle
			DrawFocusRect(pDIS->hDC, &pDIS->rcItem);
		}
		else if (pDIS->itemAction & ODA_DRAWENTIRE) {
			// Not a focus change -> render rectangle if requested
			if (pDIS->itemState & ODS_FOCUS) {
				DrawFocusRect(pDIS->hDC, &pDIS->rcItem);
			}
		}
		*/
		return 0;
	}

	case WM_COMMAND:
	{
		return (INT_PTR)TRUE;
	}

	}

	return (INT_PTR)FALSE;
}

