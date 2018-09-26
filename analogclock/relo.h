// draw relo

#pragma once

class CRelo
{
private:
	HANDLE hStopEvent;
	HANDLE hThread;
	DWORD dwThreadId;
	HWND hWnd;
	RECT rect;
	int x, y;
	int left2, top2, right2, bottom2;
	int left1, top1, right1, bottom1;
	POINT hour_unit[12][4], min_unit[48][4];
	POINT sec_hand[4], min_hand[4], hour_hand[4];
	double sx0, sy0, sx1, sy1, sx2, sy2, sx3, sy3;
	double mx0, my0, mx1, my1, mx2, my2, mx3, my3;
	double hx0, hy0, hx1, hy1, hx2, hy2, hx3, hy3;

	void Rotate(double *x, double *y, double a2, double b2);
	static DWORD WINAPI ThreadFunc(LPVOID lpParam);

public:
	CRelo();
	~CRelo();

	void Start(HWND hWnd);
	void Stop();

	void Create(int x, int y, int r);
	void Draw(HDC hDC);
};

