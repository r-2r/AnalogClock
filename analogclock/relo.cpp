// draw relo

#include "stdafx.h"
#include "relo.h"

// constructor
CRelo::CRelo()
{
	// initialize handle
	hThread    = NULL;
	hStopEvent = NULL;

	// create event
	hStopEvent = CreateEvent(NULL, TRUE, FALSE, NULL);

	// error checking display in output window
    if (hStopEvent == NULL) 
        OutputDebugString(L"Create event failed.\n");
}

// destructor
CRelo::~CRelo()
{
	// close all handles
	if(hThread != NULL)    CloseHandle(hThread);
	if(hStopEvent != NULL) CloseHandle(hStopEvent);
}

// multiply complex number to rotate point
void CRelo::Rotate(double *x, double *y, double a2, double b2)
{
	double s, a1, b1, a, b, r;

	a1 = *x;
	b1 = *y;

	s = sqrt(a1*a1 + b1*b1);
	a1 /= s;
	b1 /= s;

	a = a1*a2 - b1*b2;
	b = a2*b1 + a1*b2;

	r = sqrt(a*a + b*b);
	a /= r;
	b /= r;

	*x = s * a;
	*y = s * b;
}

// thread function
DWORD WINAPI CRelo::ThreadFunc(LPVOID lpParam)
{
	// Cast the parameter lpParam to the class CCounter.
	CRelo* p = (CRelo*)lpParam;

	// display thread status to output window
	wchar_t szText[100];
	swprintf_s(szText, 100, L"THE THREAD 0x%x HAS STARTED.\n", p->dwThreadId);
	OutputDebugString(szText);

	DWORD dwWaitResult, dwDelay;
	bool loop;
	double a, srx, sry, mrx, mry, hrx, hry, rad;

	// second hand
	// 1 complete rotation per 360 unit time
	a = 360.0 / 360.0;
	rad = (-a / 180.0) * M_PI;
	srx = cos(rad);
	sry = sin(rad);

	// minute hand
	// 1 complete rotation per (360 * 60) 21600 unit time
	a = 360.0 / 21600.0; 
	rad = (-a / 180.0) * M_PI;
	mrx = cos(rad);
	mry = sin(rad);

	// hour hand
	// 1 complete rotation per (360 * 60 * 12) 259200 unit time
	a = 360.0 / 259200.0; 
	rad = (-a / 180.0) * M_PI;
	hrx = cos(rad);
	hry = sin(rad);

	dwDelay = 1;
	loop = true;
	while (loop)
	{
		// second hand

		p->Rotate(&p->sx0, &p->sy0, srx, sry);
		p->Rotate(&p->sx1, &p->sy1, srx, sry);
		p->Rotate(&p->sx2, &p->sy2, srx, sry);
		p->Rotate(&p->sx3, &p->sy3, srx, sry);
		
		p->sec_hand[0].x = p->x + (LONG)p->sx0;     p->sec_hand[0].y = p->y - (LONG)p->sy0;
		p->sec_hand[1].x = p->x + (LONG)p->sx1;     p->sec_hand[1].y = p->y - (LONG)p->sy1;
		p->sec_hand[2].x = p->x + (LONG)p->sx2;     p->sec_hand[2].y = p->y - (LONG)p->sy2;
		p->sec_hand[3].x = p->x + (LONG)p->sx3;     p->sec_hand[3].y = p->y - (LONG)p->sy3;

		// minute hand

		p->Rotate(&p->mx0, &p->my0, mrx, mry);
		p->Rotate(&p->mx1, &p->my1, mrx, mry);
		p->Rotate(&p->mx2, &p->my2, mrx, mry);
		p->Rotate(&p->mx3, &p->my3, mrx, mry);
		
		p->min_hand[0].x = p->x + (LONG)p->mx0;     p->min_hand[0].y = p->y - (LONG)p->my0;
		p->min_hand[1].x = p->x + (LONG)p->mx1;     p->min_hand[1].y = p->y - (LONG)p->my1;
		p->min_hand[2].x = p->x + (LONG)p->mx2;     p->min_hand[2].y = p->y - (LONG)p->my2;
		p->min_hand[3].x = p->x + (LONG)p->mx3;     p->min_hand[3].y = p->y - (LONG)p->my3;

		// hour hand

		p->Rotate(&p->hx0, &p->hy0, hrx, hry);
		p->Rotate(&p->hx1, &p->hy1, hrx, hry);
		p->Rotate(&p->hx2, &p->hy2, hrx, hry);
		p->Rotate(&p->hx3, &p->hy3, hrx, hry);
		
		p->hour_hand[0].x = p->x + (LONG)p->hx0;     p->hour_hand[0].y = p->y - (LONG)p->hy0;
		p->hour_hand[1].x = p->x + (LONG)p->hx1;     p->hour_hand[1].y = p->y - (LONG)p->hy1;
		p->hour_hand[2].x = p->x + (LONG)p->hx2;     p->hour_hand[2].y = p->y - (LONG)p->hy2;
		p->hour_hand[3].x = p->x + (LONG)p->hx3;     p->hour_hand[3].y = p->y - (LONG)p->hy3;

		// update display
		InvalidateRect(p->hWnd, &p->rect, FALSE);

		// exit loop if event handle is set
		dwWaitResult = WaitForSingleObject(p->hStopEvent, dwDelay);
		if (dwWaitResult == WAIT_OBJECT_0) loop = false;
	}

	// display thread status to output window
	swprintf_s(szText, 100, L"THE THREAD 0x%x HAS STOPED.\n", p->dwThreadId);
	OutputDebugString(szText);

	return 0;
}

//
void CRelo::Start(HWND hWnd)
{
	// make a copy of window handle
	this->hWnd = hWnd;	

	// reset event handle to prevent stoping the loop
	ResetEvent(hStopEvent);

	// close thread handle
	if(hThread != NULL) CloseHandle(hThread);

	// create thread
	hThread = CreateThread(NULL, 0, ThreadFunc, this, 0, &dwThreadId);

	// error checking display in output window
   if (hThread == NULL) 
      OutputDebugString(L"Create thread failed.\n" ); 
}

//
void CRelo::Stop()
{
	// set event to stop animation
	SetEvent(hStopEvent);
}

//
void CRelo::Create(int x, int y, int r)
{
	this->x = x;
	this->y = y;

	left2   = x - r;    right2  = x + r;
	top2    = y - r;    bottom2 = y + r;

	left1   = x - 20;    right1  = x + 20;
	top1    = y - 20;    bottom1 = y + 20;
	
	rect.left   = left2;
	rect.right  = right2;
	rect.top    = top2;
	rect.bottom = bottom2;

	double x0, y0, x1, y1, x2, y2, x3, y3;
	double a1, b1, a2, b2, rx, ry, rad;
	double w, h, c;
	int i, k;

	//
	//     SECOND OR MINUTE MARK
	//
	//     y-axis
	//      ^
	//      |
	//      |
	//    0   1
	//    +---+           b2 +---+
	//    |   |              |   |
	//    |   | h            |   |
	//    |   |              |   |
	//    +---+           b1 +---+
	//      w                a1  a2
	//    3   2
	//
	c = 2.0 * M_PI * (double)r;
	w = c / 600.0;
	h = (double)r / 25.0;

	a2 = w / 2.0;   a1 = -a2;
	b2 = r - 8;     b1 = b2 - h;

	x0 = a1; y0 = b2;     x1 = a2; y1 = b2;
	x3 = a1; y3 = b1;     x2 = a2; y2 = b1;

	// 360 / 60 = 6
	rad = (6.0 / 180.0) * M_PI;
	rx = cos(rad);
	ry = sin(rad);

	// sixty lines for minute mark
	k = 0;
	for (i = 0; i < 60; i++)
	{
		Rotate(&x0, &y0, rx, ry);
		Rotate(&x1, &y1, rx, ry);
		Rotate(&x2, &y2, rx, ry);
		Rotate(&x3, &y3, rx, ry);

		if (((i+1) % 5) == 0) continue;

		min_unit[k][0].x = x + (LONG)x0;     min_unit[k][0].y = y - (LONG)y0;
		min_unit[k][1].x = x + (LONG)x1;     min_unit[k][1].y = y - (LONG)y1;
		min_unit[k][2].x = x + (LONG)x2;     min_unit[k][2].y = y - (LONG)y2;
		min_unit[k][3].x = x + (LONG)x3;     min_unit[k][3].y = y - (LONG)y3;

		k++;
	}

	//
	//     HOUR MARK
	//
	//     y-axis
	//      ^
	//      |
	//      |
	//    0   1
	//    +---+           b2 +---+
	//    |   |              |   |
	//    |   | h            |   |
	//    |   |              |   |
	//    +---+           b1 +---+
	//      w                a1  a2
	//    3   2
	//
	w = c / 150.0;
	h = (double)r / 10.0;

	a2 = w / 2.0;   a1 = -a2;
	b2 = r - 7;     b1 = b2 - h;

	x0 = a1; y0 = b2;     x1 = a2; y1 = b2;
	x3 = a1; y3 = b1;     x2 = a2; y2 = b1;

	// 360 / 12 = 30
	rad = (30.0 / 180.0) * M_PI;
	rx = cos(rad);
	ry = sin(rad);

	// twelve lines for hour mark
	k = 0;
	for (i = 0; i < 12; i++)
	{
		Rotate(&x0, &y0, rx, ry);
		Rotate(&x1, &y1, rx, ry);
		Rotate(&x2, &y2, rx, ry);
		Rotate(&x3, &y3, rx, ry);

		hour_unit[k][0].x = x + (LONG)x0;     hour_unit[k][0].y = y - (LONG)y0;
		hour_unit[k][1].x = x + (LONG)x1;     hour_unit[k][1].y = y - (LONG)y1;
		hour_unit[k][2].x = x + (LONG)x2;     hour_unit[k][2].y = y - (LONG)y2;
		hour_unit[k][3].x = x + (LONG)x3;     hour_unit[k][3].y = y - (LONG)y3;

		k++;
	}

	//
	//     SECOND HAND
	//
	//     y-axis
	//      ^
	//      |
	//      |
	//    0   1
	//    +---+           b2 +---+
	//    |   |              |   |
	//    | + | h            |   |
	//    |   |              |   |
	//    +---+           b1 +---+
	//      w                a1  a2
	//    3   2
	//
	w = 2.0;

	a2 = w / 2.0;       a1 = -a2;
	b2 = (double)r;     b1 = -(double)r * 0.3;

	sx0 = a1; sy0 = b2;     sx1 = a2; sy1 = b2;
	sx3 = a1; sy3 = b1;     sx2 = a2; sy2 = b1;

	sec_hand[0].x = x + (LONG)sx0;     sec_hand[0].y = y - (LONG)sy0;
	sec_hand[1].x = x + (LONG)sx1;     sec_hand[1].y = y - (LONG)sy1;
	sec_hand[2].x = x + (LONG)sx2;     sec_hand[2].y = y - (LONG)sy2;
	sec_hand[3].x = x + (LONG)sx3;     sec_hand[3].y = y - (LONG)sy3;

	//
	//     MINUTE HAND
	//
	//     y-axis
	//      ^
	//      |
	//      |
	//    0   1
	//    +---+           b2 +---+
	//    |   |              |   |
	//    | + | h            |   |
	//    |   |              |   |
	//    +---+           b1 +---+
	//      w                a1  a2
	//    3   2
	//
	w = c / 160.0;

	a2 = w / 2.0;           a1 = -a2;
	b2 = (double)r * 0.97;  b1 = -(double)r * 0.2;

	mx0 = a1; my0 = b2;     mx1 = a2; my1 = b2;
	mx3 = a1; my3 = b1;     mx2 = a2; my2 = b1;

	min_hand[0].x = x + (LONG)mx0;     min_hand[0].y = y - (LONG)my0;
	min_hand[1].x = x + (LONG)mx1;     min_hand[1].y = y - (LONG)my1;
	min_hand[2].x = x + (LONG)mx2;     min_hand[2].y = y - (LONG)my2;
	min_hand[3].x = x + (LONG)mx3;     min_hand[3].y = y - (LONG)my3;

	//
	//     HOUR HAND
	//
	//     y-axis
	//      ^
	//      |
	//      |
	//    0   1
	//    +---+           b2 +---+
	//    |   |              |   |
	//    | + | h            |   |
	//    |   |              |   |
	//    +---+           b1 +---+
	//      w                a1  a2
	//    3   2
	//
	w = c / 160.0;

	a2 = w / 2.0;           a1 = -a2;
	b2 = (double)r * 0.65;  b1 = -(double)r * 0.2;

	hx0 = a1; hy0 = b2;     hx1 = a2; hy1 = b2;
	hx3 = a1; hy3 = b1;     hx2 = a2; hy2 = b1;

	hour_hand[0].x = x + (LONG)hx0;     hour_hand[0].y = y - (LONG)hy0;
	hour_hand[1].x = x + (LONG)hx1;     hour_hand[1].y = y - (LONG)hy1;
	hour_hand[2].x = x + (LONG)hx2;     hour_hand[2].y = y - (LONG)hy2;
	hour_hand[3].x = x + (LONG)hx3;     hour_hand[3].y = y - (LONG)hy3;
}

//
void CRelo::Draw(HDC hDC)
{
	HBRUSH hBrush1, hBrush2, hBrush3;
	HPEN hPen1, hPen2, hPen3;
	int i;

	hBrush1 = CreateSolidBrush(RGB(255, 255, 255));
	hBrush2 = CreateSolidBrush(RGB(0, 0, 0));
	hBrush3 = CreateSolidBrush(RGB(128, 128, 128));
	hPen1   = CreatePen(PS_SOLID, 8, RGB(0, 0, 0));
	hPen2   = CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
	hPen3   = CreatePen(PS_SOLID, 1, RGB(64, 64, 64));

	// clock face
	SelectObject(hDC,hPen1);
	SelectObject(hDC,hBrush1);
	Ellipse(hDC, left2, top2, right2, bottom2);

	SelectObject(hDC, hPen2);
	SelectObject(hDC, hBrush2);

	// second or minute mark
	for (i = 0; i < 48; i++)
		Polygon(hDC, &min_unit[i][0], 4);

	// hour mark
	for (i = 0; i < 12; i++)
		Polygon(hDC, &hour_unit[i][0], 4);

	SelectObject(hDC, hPen3);
	SelectObject(hDC, hBrush3);


	// minute hand
	Polygon(hDC, min_hand, 4);

	// hour hand
	Polygon(hDC, hour_hand, 4);

	// second hand
	Polygon(hDC, sec_hand, 4);

	//
	Ellipse(hDC, left1, top1, right1, bottom1);

	// release
	DeleteObject(hPen1);
	DeleteObject(hPen2);
	DeleteObject(hPen3);
	DeleteObject(hBrush1);
	DeleteObject(hBrush2);
	DeleteObject(hBrush3);
}

//