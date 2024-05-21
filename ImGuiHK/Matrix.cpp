#include "pch.h"
#include "Matrix.h"


size_t ReadMem(int64_t address, void* recv, int size)
{
	__try
	{
		size_t read_size;
		ReadProcessMemory((HANDLE)-1, (LPCVOID)address, recv, size, &read_size);
		return read_size;
	}
	__except (true) { return 0; }
	
}



bool Matrix::GetMatrixArr()
{
	// 4 * 4
	memset(m_matrixArray, 0, sizeof(float) * 16);

	//memcpy(m_matrixArray, (PVOID)m_matrixAddr, sizeof(float) * 16);
	if (ReadMem(m_matrixAddr, m_matrixArray, sizeof(float) * 16))
	{/*
		LOG1("m_matrixArray=>%.2f %.2f %.2f %.2f | %.2f %.2f %.2f %.2f | %.2f %.2f %.2f %.2f | %.2f %.2f %.2f %.2f \n",
			m_matrixArray[0][0],
			m_matrixArray[0][1],
			m_matrixArray[0][2],
			m_matrixArray[0][3],

			m_matrixArray[1][0],
			m_matrixArray[1][1],
			m_matrixArray[1][2],
			m_matrixArray[1][3],

			m_matrixArray[2][0],
			m_matrixArray[2][1],
			m_matrixArray[2][2],
			m_matrixArray[2][3],

			m_matrixArray[3][0],
			m_matrixArray[3][1],
			m_matrixArray[3][2],
			m_matrixArray[3][3]
		);
		*/
		return true;
	}
		

	

	return false;
}



// 4x4 ¡–æÿ’Û
bool Matrix::GetCoreInfo(float* location, int& x, int& y, int& w, int& h)
{
	int x1, y1, window_width, window_heigt;
	GetWindowSize(x1, y1, window_width, window_heigt);
	float to_W = m_matrixArray[0][3] * location[0] + m_matrixArray[1][3] * location[1]
		+ m_matrixArray[2][3] * location[2] + m_matrixArray[3][3];
	if (to_W < 0.01f)
	{
		x = y = 0;
		return false;
	}

	to_W = 1 / to_W;

	window_width /= 2;
	window_heigt /= 2;

	float to_X = window_width + (m_matrixArray[0][0] * location[0]
		+ m_matrixArray[1][0] * location[1]
		+ m_matrixArray[2][0] * location[2]
		+ m_matrixArray[3][0]) * to_W * window_width;

	float to_Y = window_heigt - (m_matrixArray[0][1] * location[0]
		+ m_matrixArray[1][1] * location[1]
		+ m_matrixArray[2][1] * (location[2] + 82)
		+ m_matrixArray[3][1]) * to_W * window_heigt;

	float to_Z = window_heigt - (m_matrixArray[0][1] * location[0]
		+ m_matrixArray[1][1] * location[1]
		+ m_matrixArray[2][1] * (location[2] - 95)
		+ m_matrixArray[3][1]) * to_W * window_heigt;

	float to_H = to_Z - to_Y;

	x = to_X - to_H / 4;
	y = to_Y;
	h = to_H;
	w = to_H / 2.5;

	return true;
}

void Matrix::GetWindowSize(int& x, int& y, int& width, int& height)
{
	RECT rect;
	GetWindowRect(m_hwnd, &rect);
	x = rect.left;
	y = rect.top;
	width = rect.right - rect.left;
	height = rect.bottom - rect.top;
	if (GetWindowLongA(m_hwnd, GWL_STYLE) & WS_CAPTION)
	{
		x += 8;
		width -= 8;
		y += 30;
		height -= 30;
	}
}
