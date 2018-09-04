#pragma once

#include <Windows.h>
#include <Windowsx.h>
#include <CommCtrl.h>

#pragma comment(lib, "Comctl32.lib")

// WC_BVCTL*

// ======= Ribbon ==================================================

//#define WC_BVCTLRIBBONA				"BVCtrl_Ribbon"
//#define RBM_FIRST					(WM_USER+2300)

// ======= Button ==================================================

#define WC_BUTTONCTRLA				"ButtonCtrl"
#define BEM_FIRST					(WM_USER+2400)

// =================================================================


#define GetAValue(dw)				(HIWORD(dw)>>8)
#define RECTWIDTH(rc)				((rc.right) - (rc.left))
#define RECTHEIGHT(rc)				((rc.bottom) - (rc.top))

//#define RectangleEx(hdc, prc)		Rectangle((hdc), (prc.left), (prc.top), (prc.right), (prc.bottom));
