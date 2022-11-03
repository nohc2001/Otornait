#pragma once

#pragma comment(lib, "msimg32.lib")

#include <Windows.h>
#include <vector>
#include <atlImage.h>
#include "resource.h"

#include "SpaceMath.h"

extern HINSTANCE hInst;
extern HBRUSH whiteB;
using namespace std;

class Sprite {
private:
	CImage image;
	//HBITMAP m_hBitmap;
	int mOriginWidth = 0;
	int mOriginHeight = 0;
public:
	int mMaxFrame = 0; // �ִϸ��̼��� �ִ� ������
	int mFrameW = 0; // �׸� ������ ǥ���Ҷ�, ���� ��Ʈ���� �ʺ� ��ŭ �� ����
	int mFrameH = 0; // �׸� ������ ǥ���Ҷ�, ���� ��Ʈ���� ���̸� ��ŭ �� ����
	//int mResouceId = 0; // resource.h�� ������ �׸��� ���̵�

	bool m_bInvisibleMod = false; // ����ȭ ����� ���ΰ�
	bool m_bBackgroundIsWhite = false; // ����� �Ͼ���ΰ� �������ΰ�

	Sprite();

	Sprite(bool blank);

	Sprite(LPCTSTR FileName);

	Sprite(LPCTSTR FileName, const int& originW, const int& originH, const int& frameW, const int& frameH, const bool& binvisible, const bool& bBackgroundIsWhite);

	Sprite* Init(LPCTSTR FileName);
	Sprite* Init(LPCTSTR FileName, const int& frameW, const int& frameH);

	virtual ~Sprite() final;

	void SetFrame(const int& maxframe, const int& width, const int& height);

	void DrawSprite(HDC hdc, int x, int y, int w, int h);

	void DrawSpriteAlphaBlend(HDC hdc, int x, int y, int w, int h, int alpha);

	static void DrawBITMAPAlphaBlend(HDC hdc, int x, int y, int w, int h, int alpha, HDC bitDC, int bitw, int bith);

	void DrawFrame(HDC hdc, int frame, int x, int y, int w, int h);

	void DrawFrameAlphaBlend(HDC hdc, int frame, int x, int y, int w, int h, int alpha);

	void DrawTransfromFrame(HDC hdc, int frame, int x, int y, int w, int h, float rotation_Angle, shp::vec2f centerPos);

	void DrawTransfromFrameAlphaBlend(HDC hdc, int frame, int x, int y, int w, int h, float rotation_Angle, shp::vec2f centerPos, int alpha);
};
