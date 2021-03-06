#pragma once

#include "resource.h"
#include <windowsX.h>
#include <winuser.h>
#include <commctrl.h>
#include <vector>
#include "database.h"
#include "expenseAdd.h"
#include "expenseList.h"
#pragma comment(linker,"\"/manifestdependency:type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#pragma comment(lib, "ComCtl32.lib")

#define MAX_LOADSTRING 100
#define NUMBER_OF_EXPENSE_TYPES 6

const int iFullWidth = 405;
const int iSquareEdge = 15;
const COLORREF colorRect[6] = { RGB(255,0,0),RGB(255,127,0), RGB(255,255,0), RGB(0,255,0), RGB(0,0,255), RGB(148,0,211) };

// Update ullExpenseTotal, ullExpenseByType
void updateReport(vector<expense> & e, unsigned long long & ullExpenseTotal, unsigned long long ullExpenseByType[6]);

// Draw rectangles inside the Report groupbox to express statics and expense types notes
void drawChart(HDC hDC, unsigned long long ullExpenseTotal, unsigned long long ullExpenseByType[6]);

void OnCommand(HWND hWnd, int id, HWND hwndCtl, UINT codeNotify);
void OnPaint(HWND hWnd);
void OnDestroy(HWND hWnd);
BOOL OnCreate(HWND hWnd, LPCREATESTRUCT lpCreateStruct);