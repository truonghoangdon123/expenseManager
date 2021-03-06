// main.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "expenseManager.h"

HINSTANCE hInst;                                // current instance
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name
TCHAR* expenseItms[] = {
	L"Ăn uống",
	L"Di chuyển",
	L"Nhà cửa",
	L"Xe cộ",
	L"Nhu yếu phẩm",
	L"Dịch vụ"
};

// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: Place code here.

    // Initialize global strings
    LoadStringW(hInstance, IDC_EXPENSEMANAGER, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_EXPENSEMANAGER));

    MSG msg;

    // Main message loop:
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

//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_EXPENSEMANAGER));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_EXPENSEMANAGER);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // Store instance handle in our global variable

   HWND hWnd = CreateWindowW(szWindowClass, L"Quản Lý Chi Tiêu", WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,
      CW_USEDEFAULT, 0, 1000, 500, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_COMMAND  - process the application menu
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
		HANDLE_MSG(hWnd, WM_CREATE, OnCreate);
		HANDLE_MSG(hWnd, WM_COMMAND, OnCommand);
		HANDLE_MSG(hWnd, WM_PAINT, OnPaint);
		HANDLE_MSG(hWnd, WM_DESTROY, OnDestroy);
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
    return 0;
}

// Message handler for about box.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
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

void OnCommand(HWND hWnd, int id, HWND hwndCtl, UINT codeNotify)
{
	switch (id)
	{
	case IDM_ABOUT:
		DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
		break;
	case IDM_EXIT:
		DestroyWindow(hWnd);
		break;
	case ID_BUTTONADD:
		// Check Amount and Content existense first
		HWND hEditAmount = GetDlgItem(hWnd, ID_EDITAMOUNT),
			hEditContent = GetDlgItem(hWnd, ID_EDITCONTENT);
		int szAmount = GetWindowTextLength(hEditAmount),
			szContent = GetWindowTextLength(hEditContent);

		if (szAmount < 1)
		{
			MessageBox(hWnd, L"Vui lòng nhập số tiền.", L"Lỗi", MB_ICONWARNING);
		}
		else if (szAmount > 19)
		{
			MessageBox(hWnd, L"Số tiền quá lớn.", L"Lỗi", MB_ICONWARNING);
		}
		else if (szContent < 1)
		{
			MessageBox(hWnd, L"Vui lòng nhập nội dung.", L"Lỗi", MB_ICONWARNING);
		}
		else
		{
			// Extract Type
			HWND hCombobox = GetDlgItem(hWnd, ID_COMBOBOX);
			int iCurIndex = SendMessage(hCombobox, CB_GETCURSEL, 0, 0);
			TCHAR *wCurItm = expenseItms[iCurIndex];
			// Extract Content
			TCHAR *wContent;
			wContent = new TCHAR[szContent + 1];
			GetWindowText(hEditContent, wContent, szContent + 1);
			TCHAR *wAmount = new TCHAR[szAmount + 1];
			GetWindowText(hEditAmount, wAmount, szAmount + 1);
			// Export to database
			if (-1 == exportDB(iCurIndex, wContent, wAmount))
			{
				MessageBox(hWnd, L"Thêm chi tiêu mới thất bại.", L"Lỗi", MB_ICONWARNING);
			}
			// Release pointers
			delete wContent;
			delete wAmount;
			// Re-paint windows to update expense total and statistics bar
			InvalidateRect(hWnd, NULL, TRUE);
			break;
		}
	}
}

void OnPaint(HWND hWnd)
{
	PAINTSTRUCT ps;
	HDC hdc;
	unsigned long long ullExpenseTotal = 0;
	unsigned long long ullExpenseByType[NUMBER_OF_EXPENSE_TYPES] = { 0 };
	vector<expense> e;
	
	hdc = BeginPaint(hWnd, &ps);
	e = importDB();
	if (e.size() > 0)
	{	
		// Database does exist so start doing as following:
		// - Clear the listview
		// - Re-add items from the updated database to the listview
		// - Update new expense paramaters
		// - Re-draw windows aimed for the sake of total expense and statistics bars
		HWND hListView = GetDlgItem(hWnd, ID_LISTVIEW);
		vector<expense> eTmp = e;
		ListView_DeleteAllItems(hListView);
		while (!e.empty())
		{
			addRowToListView(hListView, 0, expenseItms[e.back().iTypeItm], stow(e.back().sContent), ulltow(e.back().ullAmount));
			e.pop_back();
		}
		updateReport(eTmp, ullExpenseTotal, ullExpenseByType);
		drawChart(hdc, ullExpenseTotal, ullExpenseByType);
	}
	else
	{
		MessageBox(hWnd, L"Dữ liệu trống hoặc có lỗi nhập dữ liệu.\nDữ liệu sẽ được tạo mới.", L"Chú ý", MB_ICONINFORMATION);
		// Not draw statistics bar if database not existed
		// Send ullExpenseTotal = -1 as a flag
		drawChart(hdc, -1, ullExpenseByType);
	}

	// Update expense total
	SetWindowText(GetDlgItem(hWnd, ID_LBLAMOUNT), ulltow(ullExpenseTotal));

	EndPaint(hWnd, &ps);
}

void OnDestroy(HWND hwnd)
{
	PostQuitMessage(0);
}

BOOL OnCreate(HWND hWnd, LPCREATESTRUCT lpCreateStruct)
{
	INITCOMMONCONTROLSEX icc;
	icc.dwSize = sizeof(icc);
	icc.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&icc);

	LOGFONT lf;
	GetObject(GetStockObject(DEFAULT_GUI_FONT), sizeof(LOGFONT), &lf);
	HFONT hFont = CreateFont(lf.lfHeight, lf.lfWidth,
		lf.lfEscapement, lf.lfOrientation, lf.lfWeight,
		lf.lfItalic, lf.lfUnderline, lf.lfStrikeOut, lf.lfCharSet,
		lf.lfOutPrecision, lf.lfClipPrecision, lf.lfQuality,
		lf.lfPitchAndFamily, lf.lfFaceName);

	// Group ADD NEW EXPENSE =======================================

	HWND hGrbAdd = CreateWindowEx(0, L"BUTTON", L"THÊM MỘT LOẠI CHI TIÊU", 
		WS_CHILD | WS_VISIBLE | BS_GROUPBOX | WS_GROUP, 20, 10, 445, 240, hWnd, 0, hInst, NULL);
	SendMessage(hGrbAdd, WM_SETFONT, WPARAM(hFont), TRUE);

	HWND hLbl = CreateWindowEx(0, L"STATIC", L"Loại chi tiêu", 
		WS_CHILD | WS_VISIBLE, 40, 35, 60, 15, hWnd, 0, hInst, NULL);
	SendMessage(hLbl, WM_SETFONT, WPARAM(hFont), TRUE);

	HWND hCbb = CreateWindowEx(0, L"COMBOBOX", L"", 
		CBS_DROPDOWNLIST | CBS_HASSTRINGS | WS_CHILD | WS_VISIBLE | WS_VSCROLL | CBS_AUTOHSCROLL, 40, 50, 405, 0, hWnd, (HMENU)ID_COMBOBOX, hInst, NULL);
	SendMessage(hCbb, WM_SETFONT, WPARAM(hFont), TRUE);
	initComboBox(hCbb, NUMBER_OF_EXPENSE_TYPES);

	hLbl = CreateWindowEx(0, L"STATIC", L"Nội dung", 
		WS_CHILD | WS_VISIBLE, 40, 85, 60, 15, hWnd, 0, hInst, NULL);
	SendMessage(hLbl, WM_SETFONT, WPARAM(hFont), TRUE);
	HWND hEditContent = CreateWindowEx(0, L"EDIT", L"", 
		WS_CHILD | WS_VISIBLE | WS_BORDER | SS_LEFT, 40, 100, 405, 80, hWnd, (HMENU)ID_EDITCONTENT, hInst, NULL);
	SendMessage(hEditContent, WM_SETFONT, WPARAM(hFont), TRUE);
	SendMessage(hEditContent, EM_SETCUEBANNER, 0, (LPARAM)TEXT("Chưa hỗ trợ lưu và nạp dữ liệu định dạng Unicode."));

	hLbl = CreateWindowEx(0, L"STATIC", L"Số tiền", 
		WS_CHILD | WS_VISIBLE, 40, 195, 60, 15, hWnd, 0, hInst, NULL);
	SendMessage(hLbl, WM_SETFONT, WPARAM(hFont), TRUE);
	HWND hEditAmount = CreateWindowEx(0, L"EDIT", L"", 
		WS_CHILD | WS_VISIBLE | WS_BORDER | SS_LEFT | ES_NUMBER, 40, 210, 280, 20, hWnd, (HMENU)ID_EDITAMOUNT, hInst, NULL);
	SendMessage(hEditAmount, WM_SETFONT, WPARAM(hFont), TRUE);
	SendMessage(hEditAmount, EM_SETCUEBANNER, 0, (LPARAM)TEXT("Bạn đã chi bao nhiêu?"));

	HWND hBtnAdd = CreateWindowEx(0, L"BUTTON", L"Thêm", 
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 335, 196, 110, 35, hWnd, (HMENU)ID_BUTTONADD, hInst, NULL);
	SendMessage(hBtnAdd, WM_SETFONT, WPARAM(hFont), TRUE);

	// Group EXPENSE LIST VIEW =====================================

	HWND hGrbLsv = CreateWindowEx(0, L"BUTTON", L"DANH SÁCH CHI TIÊU", 
		WS_CHILD | WS_VISIBLE | BS_GROUPBOX | WS_GROUP, 500, 10, 465, 415, hWnd, 0, hInst, NULL);
	SendMessage(hGrbLsv, WM_SETFONT, WPARAM(hFont), TRUE);
	HWND hLsv = CreateWindow(WC_LISTVIEWW, L"", 
		WS_CHILD | WS_VISIBLE | WS_VSCROLL | LVS_REPORT | WS_BORDER, 520, 35, 425, 370, hWnd, (HMENU)ID_LISTVIEW, hInst, NULL);
	SendMessage(hLsv, WM_SETFONT, WPARAM(hFont), TRUE);
	// LVS_EX_FULLROWSELECT is an extended window style (not normal one) so needed to be specified after the window is created
	SendMessage(hLsv, LVM_SETEXTENDEDLISTVIEWSTYLE,	LVS_EX_FULLROWSELECT, LVS_EX_FULLROWSELECT);
	initListViewColumn(hLsv);

	// Group EXPENSE STATISTICS ====================================

	HWND hGrbReport = CreateWindowEx(0, L"BUTTON", L"THỐNG KÊ",
		WS_CHILD | WS_VISIBLE | BS_GROUPBOX | WS_GROUP, 20, 260, 445, 165, hWnd, 0, hInst, NULL);
	SendMessage(hGrbReport, WM_SETFONT, WPARAM(hFont), TRUE);

	hLbl = CreateWindowEx(0, L"STATIC", L"Tổng cộng:",
		WS_CHILD | WS_VISIBLE, 40, 285, 60, 15, hWnd, 0, hInst, NULL);
	SendMessage(hLbl, WM_SETFONT, WPARAM(hFont), TRUE);
	
	HWND hAmountTotal = CreateWindowEx(0, L"STATIC", L"",
			WS_CHILD | WS_VISIBLE, 100, 285, 200, 15, hWnd, (HMENU)ID_LBLAMOUNT, hInst, NULL);
	SendMessage(hLbl, WM_SETFONT, WPARAM(hFont), TRUE);

	// Red
	hLbl = CreateWindowEx(0, L"STATIC", L"Ăn uống",
		WS_CHILD | WS_VISIBLE, 70, 310, 90, 15, hWnd, 0, hInst, NULL);
	SendMessage(hLbl, WM_SETFONT, WPARAM(hFont), TRUE);
	// Orange
	hLbl = CreateWindowEx(0, L"STATIC", L"Di chuyển",
		WS_CHILD | WS_VISIBLE, 70, 340, 90, 15, hWnd, 0, hInst, NULL);
	SendMessage(hLbl, WM_SETFONT, WPARAM(hFont), TRUE);
	// Yellow
	hLbl = CreateWindowEx(0, L"STATIC", L"Nhà cửa",
		WS_CHILD | WS_VISIBLE, 215, 310, 90, 15, hWnd, 0, hInst, NULL);
	SendMessage(hLbl, WM_SETFONT, WPARAM(hFont), TRUE);
	// Green
	hLbl = CreateWindowEx(0, L"STATIC", L"Xe cộ",
		WS_CHILD | WS_VISIBLE, 215, 340, 90, 15, hWnd, 0, hInst, NULL);
	SendMessage(hLbl, WM_SETFONT, WPARAM(hFont), TRUE);
	// Blue
	hLbl = CreateWindowEx(0, L"STATIC", L"Nhu yếu phẩm",
		WS_CHILD | WS_VISIBLE, 370, 310, 90, 15, hWnd, 0, hInst, NULL);
	SendMessage(hLbl, WM_SETFONT, WPARAM(hFont), TRUE);
	// Magenta
	hLbl = CreateWindowEx(0, L"STATIC", L"Dịch vụ",
		WS_CHILD | WS_VISIBLE, 370, 340, 90, 15, hWnd, 0, hInst, NULL);
	SendMessage(hLbl, WM_SETFONT, WPARAM(hFont), TRUE);
	
	return TRUE;
}