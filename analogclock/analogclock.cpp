//
// analog clock
//

#include "stdafx.h"
#include "analogclock.h"
#include "relo.h"

#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name

//
CRelo relo;

// Forward declarations of functions included in this code module:
LRESULT CALLBACK    WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

void OnPaint(HWND hWnd);
void OnCreate(HWND hWnd);
void OnDestroy(HWND hWnd);

void OnFileExit(HWND hWnd);

void OnAnimationStart(HWND hWnd);
void OnAnimationStop(HWND hWnd);

// main window
int APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // Initialize global strings
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_ANALOGCLOCK, szWindowClass, MAX_LOADSTRING);

    //  Registers the window class.
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ANALOGCLOCK));
    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_ANALOGCLOCK);
    wcex.lpszClassName = szWindowClass;
    wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    RegisterClassExW(&wcex);

    // Store instance handle in our global variable.
    hInst = hInstance; 

    // Create the main program window.
    int nWidth, nHeight, w, h;

    w = 800;
    h = 600;

    nWidth  = w + 16;
    nHeight = h + 58;

    HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, 0, nWidth, nHeight, nullptr, nullptr, hInstance, nullptr);

    if (!hWnd) return FALSE;

    // Display the main program window.
    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_ANALOGCLOCK));

    MSG msg;

    // Main message loop
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

//  Processes messages for the main window.
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDM_EXIT:  OnFileExit(hWnd);						break;
		case IDM_START: OnAnimationStart(hWnd);					break;
		case IDM_STOP:  OnAnimationStop(hWnd);					break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
        break;
	case WM_PAINT:   OnPaint(hWnd);									break;
	case WM_CREATE:  OnCreate(hWnd);								break;
	case WM_DESTROY: OnDestroy(hWnd);								break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

//
void OnPaint(HWND hWnd)
{
	PAINTSTRUCT ps;
	HDC hDC;

	hDC = BeginPaint(hWnd, &ps);
	relo.Draw(hDC);
	EndPaint(hWnd, &ps);
}

//
void OnCreate(HWND hWnd)
{
	HMENU hMenu = GetMenu(hWnd);
	EnableMenuItem(hMenu, IDM_START, MF_ENABLED);
	EnableMenuItem(hMenu, IDM_STOP,  MF_DISABLED);

	relo.Create(400, 300, 250);
}

//
void OnDestroy(HWND hWnd)
{
	PostQuitMessage(0);
}

//
void OnFileExit(HWND hWnd)
{
	DestroyWindow(hWnd);
}

//
void OnAnimationStart(HWND hWnd)
{
	HMENU hMenu = GetMenu(hWnd);
	EnableMenuItem(hMenu, IDM_START, MF_DISABLED);
	EnableMenuItem(hMenu, IDM_STOP,  MF_ENABLED);

	relo.Start(hWnd);
}

//
void OnAnimationStop(HWND hWnd)
{
	HMENU hMenu = GetMenu(hWnd);
	EnableMenuItem(hMenu, IDM_START, MF_ENABLED);
	EnableMenuItem(hMenu, IDM_STOP,  MF_DISABLED);

	relo.Stop();
}