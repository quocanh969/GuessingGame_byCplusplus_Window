// 1612018_GuessingGame.cpp : Defines the entry point for the application.
//

#include "stdafx.h"


#include "1612018_GuessingGame.h"
#include <windowsx.h>
#include "Question.h"
#include "HighScore.h"
#include "resource.h"


#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name

// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);


// Biến các màn hình
HWND mainWnd;
HWND secWnd;

// Biến toàn cục
HWND btnAns1, btnAns2;
HWND lblHint, lblQuiz;
HWND lblTimer;
HWND lblPoint;

HFONT customFont; // Biến lưu font chữ

Question quizToBeAsked[10];

// path
WCHAR path[260];

// Đếm số câu hỏi đã được chọn
int numberOfQuiz = 0;
int correctAns = 0; // Số câu trả lời đúng

// Biến mảng highScore
highScore arrHighScore[5];

// Hàm xử lý điểm high score
void loadHighScore(highScore arrHighScore[]);
void updateHighScore(int correctAns, int min, int sec);

//  Hàm thời gian
void IncreaseSeconds(HWND hwnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime);
int minutes = 0, seconds = 0;

// MỘt số hàm cần thiết cho secondWindow
ATOM registerSecondWinsClass(HINSTANCE hInstance);
LRESULT CALLBACK SecondWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
void displaySecondWnd(HWND hwnd);

_In_ int cmdShow;

// Các hàm ánh xạ của win 2
BOOL SecWnd_OnCreate(HWND hwnd, LPCREATESTRUCT lpCreateStruct);
void SecWnd_OnCommand(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify);
void SecWnd_OnPaint(HWND hwnd);
void SecWnd_OnDestroy(HWND hwnd);
void SecWnd_OnClose(HWND hwnd);

// MỘt số hàm thao tác nhấn nút
void Answer1Click(HWND);
void Answer2Click(HWND);
void prepareForNextQuiz(HWND hwnd);

// Các hàm ánh xạ
void OnCommand(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify);
BOOL OnCreate(HWND hwnd, LPCREATESTRUCT lpCreateStruct);
void OnPaint(HWND hwnd);
void OnDestroy(HWND hwnd);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: Place code here.

	// load config path
	GetCurrentDirectory(260, path);
	wsprintf(path, L"%s\\config.ini", path);

    // Initialize global strings
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_MY1612018GUESSINGGAME, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);
	// Register man hinh thu 2
	registerSecondWinsClass(hInstance);
	cmdShow = nCmdShow;

	// Thiết lập mảng các câu hỏi
	initQuestionData(quizToBeAsked);

	// Cập nhật high score
	loadHighScore(arrHighScore);

    // Perform application initialization:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_MY1612018GUESSINGGAME));

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
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON1));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_BTNFACE+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_MY1612018GUESSINGGAME);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_ICON1));

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

   // nạp thông số của màn hình vào
   int x, y, w, h;
   x = GetPrivateProfileInt(L"Wnd", L"X", 0, path);
   y = GetPrivateProfileInt(L"Wnd", L"Y", 0, path);
   h = GetPrivateProfileInt(L"Wnd", L"Height", 0, path);
   w = GetPrivateProfileInt(L"Wnd", L"Width", 0, path);

   mainWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      x, y, w, h, nullptr, nullptr, hInstance, nullptr);

   

   if (!mainWnd)
   {
      return FALSE;
   }

   ShowWindow(mainWnd, nCmdShow);
   UpdateWindow(mainWnd);

   return TRUE;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE: Processes messages for the main window.
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

// Ánh xạ hàm OnCommand
void OnCommand(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify)
{
	switch (id)
	{
	case IDM_ABOUT:
		DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hwnd, About);
		break;
	case IDM_EXIT:
		DestroyWindow(hwnd);
		break;
	case IDC_BTNSTART:
	{
		displaySecondWnd(hwnd);

		HACCEL hAccelTable = LoadAccelerators(hInst, MAKEINTRESOURCE(IDC_MY1612018GUESSINGGAME));

		MSG msg1;

		// Main message loop:
		while (GetMessage(&msg1, nullptr, 0, 0))
		{
			if (!TranslateAccelerator(msg1.hwnd, hAccelTable, &msg1))
			{
				TranslateMessage(&msg1);
				DispatchMessage(&msg1);
			}
		}

		return;
	}
	case IDC_BTNHIGHSCORE:
	{

		WCHAR txtHighScores[255] = L"";

		// Đọc vào highScore đầu tiên
		if (arrHighScore[0].score != 0 || arrHighScore[0].min != 0 && arrHighScore[0].sec != 0)
		{
			wsprintf(txtHighScores, L"1. Correct answer: %d/10. Time: %02d - %02d"
				, arrHighScore[0].score, arrHighScore[0].min, arrHighScore[0].sec);
		}
		else
		{
			wsprintf(txtHighScores, L"1.\n2.\n3.\n4.\n5");
			MessageBox(NULL, txtHighScores, L"High Scores", MB_OK);
			break;
		}

		for (int i = 1; i < 5; i++)
		{// Lần lượt đọc các highScore khác
			if (arrHighScore[i].score != 0 || arrHighScore[i].min != 0 && arrHighScore[i].sec != 0)
			{
				wsprintf(txtHighScores, L"%s\n%d. Correct answer: %d/10. Time: %02d - %02d"
					, txtHighScores, i + 1, arrHighScore[i].score, arrHighScore[i].min, arrHighScore[i].sec);
			}
			else
			{
				wsprintf(txtHighScores, L"%s\n%d.", txtHighScores, i + 1);
			}
		}

		// Hiển thị messageBox highScore
		MessageBox(NULL, txtHighScores, L"High Scores", MB_OK);

		break;
	}
	case IDC_BTNEXIT:
		DestroyWindow(hwnd);
		break;
	default:
		return;
	}
}

// Ánh xạ hàm OnCreate
BOOL OnCreate(HWND hwnd, LPCREATESTRUCT lpCreateStruct)
{
	// Lấy font hệ thống
	LOGFONT lf;
	GetObject(GetStockObject(DEFAULT_GUI_FONT), sizeof(LOGFONT), &lf);

	// Thay đổi độ cao của font lên 24
	customFont = CreateFont(24, lf.lfWidth,
		lf.lfEscapement, lf.lfOrientation, lf.lfWeight,
		lf.lfItalic, lf.lfUnderline, lf.lfStrikeOut, lf.lfCharSet,
		lf.lfOutPrecision, lf.lfClipPrecision, lf.lfQuality,
		lf.lfPitchAndFamily, lf.lfFaceName);

	// Khởi tạo nút Start
	HWND btnStart = CreateWindowEx(NULL, L"BUTTON", L"START",
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		535, 60, 186, 100, hwnd,
		(HMENU)IDC_BTNSTART, lpCreateStruct->hInstance, NULL);
	SetWindowFont(btnStart, customFont, TRUE);

	// Khởi tạo nút High Score
	HWND btnHighScore = CreateWindowEx(NULL, L"BUTTON", L"HIGH SCORES",
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		535, 180, 186, 100, hwnd,
		(HMENU)IDC_BTNHIGHSCORE, lpCreateStruct->hInstance, NULL);
	SetWindowFont(btnHighScore, customFont, TRUE);

	// Khởi tạo nút Eixt
	HWND btnExit = CreateWindowEx(NULL, L"BUTTON", L"EXIT",
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		535, 300, 186, 100, hwnd,
		(HMENU)IDC_BTNEXIT, lpCreateStruct->hInstance, NULL);
	SetWindowFont(btnExit, customFont, TRUE);

	return TRUE;
}

// Ánh xạ hàm OnPaint
void OnPaint(HWND hwnd)
{
	HDC hdc;
	PAINTSTRUCT ps;
	ULONG_PTR gdiplusToken;
	hdc = BeginPaint(hwnd, &ps);

	GdiplusStartupInput gdiplusStartupInput;
	GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);
	{
		Graphics graphics(hdc);
		Image image(L"source\\StartBackground.jpg");
		graphics.DrawImage(&image, 0, 0);
	}

	GdiplusShutdown(gdiplusToken);

	EndPaint(hwnd, &ps);
}

// Ánh xạ hàm OnDestroy
void OnDestroy(HWND hwnd)
{
	PostQuitMessage(0);
}


// Đăng kí màn hình 2
ATOM registerSecondWinsClass(HINSTANCE hInstance)
{
	WNDCLASSEXW secondWins;

	secondWins.cbSize = sizeof(WNDCLASSEX);

	secondWins.style = CS_HREDRAW | CS_VREDRAW;
	secondWins.lpfnWndProc = SecondWndProc;
	secondWins.cbClsExtra = 0;
	secondWins.cbWndExtra = 0;
	secondWins.hInstance = hInstance;
	secondWins.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON1));
	secondWins.hCursor = LoadCursor(nullptr, IDC_ARROW);
	secondWins.hbrBackground = (HBRUSH)(COLOR_BTNFACE + 1);
	secondWins.lpszMenuName = NULL;//MAKEINTRESOURCEW(IDC_MY1612018GUESSINGGAME);
	secondWins.lpszClassName = L"secondWindsClass";
	secondWins.hIconSm = LoadIcon(secondWins.hInstance, MAKEINTRESOURCE(IDI_ICON1));

	return RegisterClassExW(&secondWins);
}

// Khởi tạo ban đầu cho màn hình 2
void displaySecondWnd(HWND hwnd)
{
	// nạp thông số của màn hình vào
	int x, y, w, h;
	x = GetPrivateProfileInt(L"Wnd", L"X", 0, path);
	y = GetPrivateProfileInt(L"Wnd", L"Y", 0, path);
	h = GetPrivateProfileInt(L"Wnd", L"Height", 0, path);
	w = GetPrivateProfileInt(L"Wnd", L"Width", 0, path);

	secWnd = CreateWindowW(L"secondWindsClass", szTitle, WS_OVERLAPPEDWINDOW, 
		x, y, w, h, nullptr, nullptr, hInst, nullptr);

	DestroyWindow(hwnd);

	ShowWindow(secWnd, cmdShow);
	UpdateWindow(secWnd);
}

// CALL BACK cua man hinh 2
LRESULT CALLBACK SecondWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
		HANDLE_MSG(hWnd, WM_CREATE, SecWnd_OnCreate);
		HANDLE_MSG(hWnd, WM_PAINT, SecWnd_OnPaint);
		HANDLE_MSG(hWnd, WM_COMMAND, SecWnd_OnCommand);
		HANDLE_MSG(hWnd, WM_DESTROY, SecWnd_OnDestroy);
		HANDLE_MSG(hWnd, WM_CLOSE, SecWnd_OnClose);
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

// Ánh xạ hàm OnCreate cho màn hình 2
BOOL SecWnd_OnCreate(HWND hwnd, LPCREATESTRUCT lpCreateStruct)
{// Hàm khởi tạo của màn hình 2
			// Tạo label Hint gợi ý cho câu hỏi
		lblHint = CreateWindowEx(NULL, L"STATIC", L"Tên công việc:",
			WS_CHILD | WS_VISIBLE,
			400, 70, 370, 100, hwnd,
			NULL, lpCreateStruct->hInstance, NULL);
		SetWindowFont(lblHint, customFont, TRUE);

		// Tạo label Quiz tương trung cho hiện đang ở câu số mấy
		lblQuiz = CreateWindowEx(NULL, L"STATIC", L"Tên công việc:",
			WS_CHILD | WS_VISIBLE,
			510, 20, 120, 40, hwnd,
			NULL, lpCreateStruct->hInstance, NULL);
		SetWindowFont(lblQuiz, customFont, TRUE);

		// Tạo nút Answer1
		btnAns1 = CreateWindowEx(NULL, L"BUTTON", L"Ans1",
			WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
			520, 190, 190, 70, hwnd,
			(HMENU)IDC_BTNANS1, lpCreateStruct->hInstance, NULL);
		SetWindowFont(btnAns1, customFont, TRUE);

		// Tạo nút Answer2
		btnAns2 = CreateWindowEx(NULL, L"BUTTON", L"Ans2",
			WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
			520, 280, 190, 70, hwnd,
			(HMENU)IDC_BTNANS2, lpCreateStruct->hInstance, NULL);
		SetWindowFont(btnAns2, customFont, TRUE);

		// tạo label Point
		lblPoint = CreateWindowEx(NULL, L"STATIC", L"Point: 0",
			WS_CHILD | WS_VISIBLE,
			680, 20, 190, 40, hwnd,
			NULL, lpCreateStruct->hInstance, NULL);
		SetWindowFont(lblPoint, customFont, TRUE);

		// Tạo label timer
		lblTimer = CreateWindowEx(NULL, L"STATIC", L"00:00",
			WS_CHILD | WS_VISIBLE,
			460, 20, 50, 50, hwnd,
			NULL, lpCreateStruct->hInstance, NULL);
		SetWindowFont(btnAns2, customFont, TRUE);

		// Thiết lập timer
		SetTimer(hwnd, IDT_TIMER1, 1000, (TIMERPROC)IncreaseSeconds);

		// Nạp câu hỏi tiếp theo
		prepareForNextQuiz(hwnd);
	
	return TRUE;
}

// Ánh xạ hàm OnCommand cho màn hình 2
void SecWnd_OnCommand(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify)
{
	switch (id)
	{
	case IDC_BTNANS1:
	{// Xử lý khi chọn đáp án 1
		Answer1Click(hwnd);
		prepareForNextQuiz(hwnd);
		break;
	}
	case IDC_BTNANS2:
	{// Xử lý khi chọn đáp án 2
		
		Answer2Click(hwnd);
		prepareForNextQuiz(hwnd);
		break;
	}
	default:
		return;
	}
}

// Ánh xạ hàm OnPaint cho màn hinh 2
void SecWnd_OnPaint(HWND hwnd)
{// Tạo ảnh gợi ý
	HDC hdc;
	PAINTSTRUCT ps;
	ULONG_PTR gdiplusToken;
	hdc = BeginPaint(hwnd, &ps);

	GdiplusStartupInput gdiplusStartupInput;
	GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);
	{
		Graphics graphics(hdc);
		WCHAR *tempImgName = new WCHAR[255];
			lstrcpyW(tempImgName, quizToBeAsked[numberOfQuiz].imgName);
			Image image(tempImgName);
			graphics.DrawImage(&image, 54, 35);
	}

	GdiplusShutdown(gdiplusToken);

	EndPaint(hwnd, &ps);
}

// Ánh xạ hàm OnDestroy cho màn hình 2
void SecWnd_OnDestroy(HWND hwnd)
{// Thóa
	PostQuitMessage(0);
}

// Ánh xạ hàm OnClose
void SecWnd_OnClose(HWND hwnd)
{// Nhấn nút X trên thanh taskbar thì sẽ thoát chương trình luôn
	// Xóa các phần tử trên màn hình
	DestroyWindow(btnAns1);
	DestroyWindow(btnAns2);
	DestroyWindow(lblHint);
	DestroyWindow(lblQuiz);
	DestroyWindow(lblPoint);

	// Gửi msg thoát
	PostQuitMessage(0);
	
}

// Xử lý khi nhập nút 1
void Answer1Click(HWND hwnd)
{
	if (numberOfQuiz == 9)
	{
		if (wcscmp(quizToBeAsked[numberOfQuiz].ans1, quizToBeAsked[numberOfQuiz].trueAns) == 0)
		{// Trả lời câu hỏi đúng
			correctAns++;
		}
		// Cập nhật high score
		WCHAR score1[50];
		wsprintf(score1, L"Correct answer: %d/10. Time: %02d - %02d", correctAns, minutes, seconds);
		updateHighScore(correctAns, minutes, seconds);

		WCHAR* result1 = new WCHAR[50]; // chuỗi kết quả

		// Xử lý điểm + thời gian
		wsprintf(result1, L"Trò chơi kết thúc\nBạn đã trả lời đúng %d/10 câu\nThời gian: %02d:%02d"
			, correctAns, minutes, seconds);

		// Xuất thông báo về điểm và thời gian hoàn thành
		MessageBox(secWnd, result1, L"Thông báo", MB_OK);

		SendMessage(hwnd, WM_CLOSE, NULL, NULL);
	}
	else
	{

		if (wcscmp(quizToBeAsked[numberOfQuiz].ans1, quizToBeAsked[numberOfQuiz].trueAns) == 0)
		{// Trả lời câu hỏi đúng
			correctAns++;
			MessageBox(secWnd, L"Câu trả lời đúng !!", L"NOTICE", MB_OK);
		}
		else
		{
			MessageBox(secWnd, L"Câu trả lời sai !!", L"NOTICE", MB_OK);
		}

	}

	// Cập nhật câu hỏi mới
	numberOfQuiz++;
	if (numberOfQuiz != 10)
	{
		prepareForNextQuiz(secWnd);
	}
	// Vẽ đè lại hình cũ
	InvalidateRect(secWnd, NULL, true);
	// Cập nhật màn hình sau khi vẽ
	UpdateWindow(secWnd);
}

//Xử lý khi nhấn nút 2
void Answer2Click(HWND hwnd)
{
	if (numberOfQuiz == 9)
	{
		if (wcscmp(quizToBeAsked[numberOfQuiz].ans2, quizToBeAsked[numberOfQuiz].trueAns) == 0)
		{// Trả lời câu hỏi đúng
			correctAns++;
		}
		// Cập nhật high score
		WCHAR score2[50];
		wsprintf(score2, L"Correct answer: %d/10. Time: %02d - %02d", correctAns, minutes, seconds);
		updateHighScore(correctAns, minutes, seconds);

		WCHAR* result2 = new WCHAR[50]; // chuỗi kết quả

		// Xử lý điểm + thời gian
		wsprintf(result2, L"Trò chơi kết thúc\nBạn đã trả lời đúng %d/10 câu\nThời gian: %02d:%02d"
			, correctAns, minutes, seconds);

		// Xuất thông báo về điểm và thời gian hoàn thành
		MessageBox(secWnd, result2, L"Thông báo", MB_OK);

		SendMessage(hwnd, WM_CLOSE, NULL, NULL);
	}
	else
	{
		if (wcscmp(quizToBeAsked[numberOfQuiz].ans2, quizToBeAsked[numberOfQuiz].trueAns) == 0)
		{// Trả lời câu hỏi đúng
			correctAns++;
			MessageBox(secWnd, L"Câu trả lời đúng !!", L"NOTICE", MB_OK);
		}
		else
		{
			MessageBox(secWnd, L"Câu trả lời sai !!", L"NOTICE", MB_OK);
		}
	}
	

	// Cập nhật câu hỏi mới
	numberOfQuiz++;
	if (numberOfQuiz != 10)
	{
		prepareForNextQuiz(secWnd);
	}
	// Vẽ đè lên lại hình cũ
	InvalidateRect(secWnd, NULL, true);
	// Cập nhật lại màn hình sau khi vẽ
	UpdateWindow(secWnd);
}

// Hàm thiết lập cho câu hỏi tiếp theo
void prepareForNextQuiz(HWND hwnd)
{
	SetWindowText(btnAns1, quizToBeAsked[numberOfQuiz].ans1);
	SetWindowText(btnAns2, quizToBeAsked[numberOfQuiz].ans2);
	SetWindowText(lblHint, quizToBeAsked[numberOfQuiz].Hint);
	

	// Chuỗi hiển thị câu hỏi thứ/ Câu hỏi Point
	WCHAR temp[20];

	wsprintf(temp, L"Point: %d", correctAns);
	SetWindowText(lblPoint, temp);

	wsprintf(temp, L"Quiz: %d/10", numberOfQuiz + 1);
	SetWindowText(lblQuiz, temp);
}

// Hàm tăng thời gian
void IncreaseSeconds(HWND hwnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime)
{// Hàm tăng thời gian
	seconds++;
	if (seconds > 59)
	{
		minutes++;
		seconds = 0;
	}

	if (minutes > 99)
	{// Time out
		KillTimer(secWnd, IDT_TIMER1);
		MessageBox(secWnd, L"Time Out", L"Thông báo", MB_OK);
		PostQuitMessage(0);
	}

	if (numberOfQuiz == 10)
	{// Ngưng lại khi mà hết 10 câu
		KillTimer(hwnd, IDT_TIMER1);
	}

	WCHAR time[6];
	wsprintf(time, L"%02d:%02d", minutes, seconds);
	SetWindowText(lblTimer, time);
}

// Hàm tải high score từ file
void loadHighScore(highScore arrHighScore[])
{
	int dem = 0;

	// Nhập mảng 5 highScore
	int hiSc[5];
	hiSc[0] = GetPrivateProfileInt(L"HiScore", L"first", 0, path);
	hiSc[1] = GetPrivateProfileInt(L"HiScore", L"second", 0, path);
	hiSc[2] = GetPrivateProfileInt(L"HiScore", L"thirth", 0, path);
	hiSc[3] = GetPrivateProfileInt(L"HiScore", L"fourth", 0, path);
	hiSc[4] = GetPrivateProfileInt(L"HiScore", L"fifth", 0, path);

	for (int i = 0; i < 5; i++)
	{
		if (hiSc != 0)
		{
			arrHighScore[i].score = hiSc[i] / 10000;
			hiSc[i] = hiSc[i] % 10000;
			arrHighScore[i].min = hiSc[i] / 100;
			hiSc[i] = hiSc[i] % 100;
			arrHighScore[i].sec = hiSc[i];
		}
		else
		{
			break;
		}
	}

	fclose(stdin);
}

// Hàm cập nhật high score vào file
void updateHighScore(int correctAns, int min, int sec)
{
	// Biến kiểm tra xem điểm có phải highscore không
	bool isHighScore = false;

	// Kiểm tra xem điểm ghi được có phải high score không
	for (int i = 0; i < 5; i++)
	{
		if ((correctAns > arrHighScore[i].score) || 
			(correctAns == arrHighScore[i].score &&	(min * 60 + sec) > (arrHighScore[i].min * 60 + arrHighScore[i].sec)) ||
			(arrHighScore[i].score == 0 && arrHighScore[i].min == 0 && arrHighScore[i].sec == 0))
		{
			isHighScore = true;

			for (int j = 4; j > i; j--)
			{// dời mấy high score khac xuống 1 bậc
				arrHighScore[j] = arrHighScore[j - 1];
			}
			arrHighScore[i].score = correctAns;
			arrHighScore[i].min = min;
			arrHighScore[i].sec = sec;
			break;
		}
	}

	WCHAR buffer[250];
	int diem;
	
	diem = arrHighScore[0].score * 10000 + arrHighScore[0].min * 100 + arrHighScore[0].sec;
	wsprintf(buffer, L"%d", diem);
	WritePrivateProfileString(L"HiScore", L"first", buffer, path);
	
	diem = arrHighScore[1].score * 10000 + arrHighScore[1].min * 100 + arrHighScore[1].sec;
	wsprintf(buffer, L"%d", diem);
	WritePrivateProfileString(L"HiScore", L"second", buffer, path);
	
	diem = arrHighScore[2].score * 10000 + arrHighScore[2].min * 100 + arrHighScore[2].sec;
	wsprintf(buffer, L"%d", diem);
	WritePrivateProfileString(L"HiScore", L"thirth", buffer, path);

	diem = arrHighScore[3].score * 10000 + arrHighScore[3].min * 100 + arrHighScore[3].sec;
	wsprintf(buffer, L"%d", diem);
	WritePrivateProfileString(L"HiScore", L"fourth", buffer, path);

	diem = arrHighScore[4].score * 10000 + arrHighScore[4].min * 100 + arrHighScore[4].sec;
	wsprintf(buffer, L"%d", diem);
	WritePrivateProfileString(L"HiScore", L"fifth", buffer, path);

}
