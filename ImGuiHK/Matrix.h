#pragma once
class Matrix
{

public:

	Matrix(UINT64 pMatrix , HWND hwnd) : m_matrixAddr(pMatrix),m_hwnd(hwnd) {};

	bool GetMatrixArr();

	bool GetCoreInfo(float* location, int& x, int& y, int& w, int& h);

private:
	// 获取窗口位置大小
	void GetWindowSize(int& x, int& y, int& width, int& height);

private:
	float m_matrixArray[4][4] = { 0 };
	UINT64 m_matrixAddr;
	HWND m_hwnd;



};

