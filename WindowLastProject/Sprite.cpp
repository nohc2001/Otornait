#include "Sprite.h"

extern RECT rt;
//sprite class
Sprite::Sprite() {
	
}

Sprite::Sprite(bool blank)
{
	if (blank) {

	}
	else
	{
		image.Load(L"Image\\defaultImage.png");
		mMaxFrame = 0;
		mOriginWidth = image.GetWidth();
		mOriginHeight = image.GetHeight();
		mFrameW = image.GetWidth();
		mFrameH = image.GetHeight();
		m_bInvisibleMod = false;
		m_bBackgroundIsWhite = true;
	}
}

Sprite::Sprite(LPCTSTR FileName)
{
	image.Load(FileName);
	mMaxFrame = 0;
	mOriginWidth = image.GetWidth();
	mOriginHeight = image.GetHeight();
	mFrameW = image.GetWidth();
	mFrameH = image.GetHeight();
	m_bInvisibleMod = false;
	m_bBackgroundIsWhite = true;
}

Sprite::Sprite(LPCTSTR FileName, const int& originW, const int& originH, const int& frameW, const int& frameH, const bool& binvisible, const bool& bBackgroundIsWhite) :
	mOriginWidth(originW), mOriginHeight(originH),
	mFrameW(frameW), mFrameH(frameH)
{
	image.Load(FileName);
	mMaxFrame = (originW / mFrameW) * (originH / mFrameH);
	m_bInvisibleMod = binvisible;
	m_bBackgroundIsWhite = bBackgroundIsWhite;
}

Sprite* Sprite::Init(LPCTSTR FileName)
{
	if (FileName == L"") {
		image.Load(L"Image\\defaultImage.png");
		mMaxFrame = 0;
		mOriginWidth = image.GetWidth();
		mOriginHeight = image.GetHeight();
		mFrameW = image.GetWidth();
		mFrameH = image.GetHeight();
		m_bInvisibleMod = false;
		m_bBackgroundIsWhite = true;
	}
	else {
		image.Load(FileName);
		mMaxFrame = 0;
		mOriginWidth = image.GetWidth();
		mOriginHeight = image.GetHeight();
		mFrameW = image.GetWidth();
		mFrameH = image.GetHeight();
		m_bInvisibleMod = false;
		m_bBackgroundIsWhite = true;

		return this;
	}
	
}

Sprite* Sprite::Init(LPCTSTR FileName, const int& frameW, const int& frameH)
{
	if (FileName == L"") {
		image.Load(L"Image\\defaultImage.png");
		mMaxFrame = 0;
		mOriginWidth = image.GetWidth();
		mOriginHeight = image.GetHeight();
		mFrameW = image.GetWidth();
		mFrameH = image.GetHeight();
		m_bInvisibleMod = false;
		m_bBackgroundIsWhite = true;
	}
	else {
		image.Load(FileName);
		mMaxFrame = 0;
		mOriginWidth = image.GetWidth();
		mOriginHeight = image.GetHeight();
		mFrameW = frameW;
		mFrameH = frameH;
		mMaxFrame = (mOriginWidth / mFrameW) * (mOriginHeight / mFrameH);
		m_bInvisibleMod = false;
		m_bBackgroundIsWhite = true;

		return this;
	}
	return nullptr;
}

Sprite::~Sprite() {
	if (image != NULL) {
		image.GetDC();
		image.ReleaseDC();
		image.Destroy();
	}
}

void Sprite::SetFrame(const int& maxframe, const int& width, const int& height) {
	mMaxFrame = maxframe;
	mFrameW = width;
	mFrameH = height;
}

void Sprite::DrawSprite(HDC hdc, int x, int y, int w, int h) {
	image.TransparentBlt(hdc, { x, y, x + w, y + h }, RGB(255, 255, 255));
}

void Sprite::DrawSpriteAlphaBlend(HDC hdc, int x, int y, int w, int h, int alpha)
{
	RECT dd = { 0, 0, w, h };

	RECT ScreenRT = rt;
	shp::rect4f scrt = shp::rect4f(x, y, x+w, y+h);
	if (x + w > ScreenRT.right) {
		scrt.lx = ScreenRT.right;
	}
	if (x < ScreenRT.left) {
		scrt.fx = ScreenRT.left;
	}
	if (y + h > ScreenRT.bottom) {
		scrt.ly = ScreenRT.bottom;
	}
	if (y < ScreenRT.top) {
		scrt.fy = ScreenRT.top;
	}

	HBITMAP hAlphaBit;
	hAlphaBit = CreateCompatibleBitmap(hdc, w, h);
	HDC hAlphaDC = CreateCompatibleDC(hdc);
	HBITMAP OldBit = (HBITMAP)SelectObject(hAlphaDC, hAlphaBit);

	FillRect(hAlphaDC, &dd, whiteB);
	TransparentBlt(hAlphaDC, (int)scrt.fx - x, (int)scrt.fy - y, (int)scrt.getw(), (int)scrt.geth(),
		hdc, (int)scrt.fx, (int)scrt.fy, (int)scrt.getw(), (int)scrt.geth(), RGB(255, 255, 255));

	int iw = image.GetWidth();
	int ih = image.GetHeight();
	TransparentBlt(hAlphaDC, 0, 0, w, h,
		image.GetDC(), 0, 0, iw, ih, RGB(255, 255, 255));

	BLENDFUNCTION bf;
	bf.BlendOp = AC_SRC_OVER;
	bf.BlendFlags = 0;
	bf.SourceConstantAlpha = alpha;
	bf.AlphaFormat = 0;
	AlphaBlend(hdc, scrt.fx, scrt.fy, scrt.getw(), scrt.geth(),
		hAlphaDC, (int)scrt.fx - x, (int)scrt.fy - y, (int)scrt.getw(), (int)scrt.geth(), bf);

	image.ReleaseDC();
	SelectObject(hAlphaDC, OldBit);
	DeleteDC(hAlphaDC);
	DeleteObject(hAlphaBit);
}

void Sprite::DrawBITMAPAlphaBlend(HDC hdc, int x, int y, int w, int h, int alpha, HDC bitDC, int bitw, int bith)
{
	RECT dd = { 0, 0, w, h };

	RECT ScreenRT = rt;
	shp::rect4f scrt = shp::rect4f(x, y, x + w, y + h);
	if (x + w > ScreenRT.right) {
		scrt.lx = ScreenRT.right;
	}
	if (x < ScreenRT.left) {
		scrt.fx = ScreenRT.left;
	}
	if (y + h > ScreenRT.bottom) {
		scrt.ly = ScreenRT.bottom;
	}
	if (y < ScreenRT.top) {
		scrt.fy = ScreenRT.top;
	}

	HBITMAP hAlphaBit;
	hAlphaBit = CreateCompatibleBitmap(hdc, w, h);
	HDC hAlphaDC = CreateCompatibleDC(hdc);
	HBITMAP OldBit = (HBITMAP)SelectObject(hAlphaDC, hAlphaBit);

	FillRect(hAlphaDC, &dd, whiteB);
	TransparentBlt(hAlphaDC, (int)scrt.fx - x, (int)scrt.fy - y, (int)scrt.getw(), (int)scrt.geth(),
		hdc, (int)scrt.fx, (int)scrt.fy, (int)scrt.getw(), (int)scrt.geth(), RGB(255, 255, 255));

	int iw = bitw;
	int ih = bith;
	TransparentBlt(hAlphaDC, 0, 0, w, h,
		bitDC, 0, 0, iw, ih, RGB(255, 255, 255));

	BLENDFUNCTION bf;
	bf.BlendOp = AC_SRC_OVER;
	bf.BlendFlags = 0;
	bf.SourceConstantAlpha = alpha;
	bf.AlphaFormat = 0;
	AlphaBlend(hdc, scrt.fx, scrt.fy, scrt.getw(), scrt.geth(),
		hAlphaDC, (int)scrt.fx - x, (int)scrt.fy - y, (int)scrt.getw(), (int)scrt.geth(), bf);

	SelectObject(hAlphaDC, OldBit);
	DeleteDC(hAlphaDC);
	DeleteObject(hAlphaBit);
}

void Sprite::DrawFrame(HDC hdc, int frame, int x, int y, int w, int h) {
	int fxN = mOriginWidth / mFrameW;
	int fyN = mOriginHeight / mFrameH;
	int fx = mFrameW * (frame % fxN);
	int fy = mFrameH * (frame / fxN);
	RECT destRT = { x, y, x + w, y + h };
	RECT srcRT = { fx, fy, fx + mFrameW, fy + mFrameH };
	image.TransparentBlt(hdc, destRT, srcRT, RGB(255, 255, 255));
}

void Sprite::DrawFrameAlphaBlend(HDC hdc, int frame, int x, int y, int w, int h, int alpha)
{
	RECT dd = { 0, 0, w, h };

	RECT ScreenRT = rt;
	shp::rect4f scrt = shp::rect4f(x, y, x + w, y + h);
	if (x + w > ScreenRT.right) {
		scrt.lx = ScreenRT.right;
	}
	if (x < ScreenRT.left) {
		scrt.fx = ScreenRT.left;
	}
	if (y + h > ScreenRT.bottom) {
		scrt.ly = ScreenRT.bottom;
	}
	if (y < ScreenRT.top) {
		scrt.fy = ScreenRT.top;
	}

	HBITMAP hAlphaBit;
	hAlphaBit = CreateCompatibleBitmap(hdc, w, h);
	HDC hAlphaDC = CreateCompatibleDC(hdc);
	HBITMAP OldBit = (HBITMAP)SelectObject(hAlphaDC, hAlphaBit);

	FillRect(hAlphaDC, &dd, whiteB);
	TransparentBlt(hAlphaDC, (int)scrt.fx - x, (int)scrt.fy - y, (int)scrt.getw(), (int)scrt.geth(),
		hdc, (int)scrt.fx, (int)scrt.fy, (int)scrt.getw(), (int)scrt.geth(), RGB(255, 255, 255));

	DrawFrame(hAlphaDC, frame, 0, 0, w, h);

	BLENDFUNCTION bf;
	bf.BlendOp = AC_SRC_OVER;
	bf.BlendFlags = 0;
	bf.SourceConstantAlpha = alpha;
	bf.AlphaFormat = 0;
	AlphaBlend(hdc, scrt.fx, scrt.fy, scrt.getw(), scrt.geth(),
		hAlphaDC, (int)scrt.fx - x, (int)scrt.fy - y, (int)scrt.getw(), (int)scrt.geth(), bf);

	SelectObject(hAlphaDC, OldBit);
	DeleteDC(hAlphaDC);
	DeleteObject(hAlphaBit);
}

void Sprite::DrawTransfromFrame(HDC hdc, int frame, int x, int y, int w, int h, float rotation_Angle, shp::vec2f centerPos) {
	RECT rotateRT;
	shp::vec2f rtPos[4] = {};
	rtPos[0].x = x;
	rtPos[0].y = y;
	rtPos[1].x = x + w;
	rtPos[1].y = y;
	rtPos[2].x = x + w;
	rtPos[2].y = y + h;
	rtPos[3].x = x;
	rtPos[3].y = y + h;

	rtPos[0] = shp::GetRotatePos(centerPos, rtPos[0], shp::angle2f(rotation_Angle, true));
	rtPos[1] = shp::GetRotatePos(centerPos, rtPos[1], shp::angle2f(rotation_Angle, true));
	rtPos[2] = shp::GetRotatePos(centerPos, rtPos[2], shp::angle2f(rotation_Angle, true));
	rtPos[3] = shp::GetRotatePos(centerPos, rtPos[3], shp::angle2f(rotation_Angle, true));
	
	shp::vec2f mnW = shp::vec2f(0, 0);
	shp::vec2f mnH = shp::vec2f(0, 0);
	for (int i = 0; i < 4; ++i) {
		rtPos[i] = rtPos[i] - shp::vec2f(x, y);
		if (rtPos[i].x > mnW.y) {
			mnW.y = rtPos[i].x;
		}

		if (rtPos[i].x < mnW.x) {
			mnW.x = rtPos[i].x;
		}

		if (rtPos[i].y > mnH.y) {
			mnH.y = rtPos[i].y;
		}

		if (rtPos[i].y < mnH.x) {
			mnH.x = rtPos[i].y;
		}
	}

	shp::rect4f drawRT = shp::rect4f(x + mnW.x, y + mnH.x, x + mnW.y, y + mnH.y);
	HBITMAP hBit;
	hBit = CreateCompatibleBitmap(hdc, drawRT.lx - drawRT.fx, drawRT.ly - drawRT.fy);
	HDC insDC = CreateCompatibleDC(hdc);
	HBITMAP OldBit = (HBITMAP)SelectObject(insDC, hBit);

	RECT drawRECT = { drawRT.fx, drawRT.fy, drawRT.lx, drawRT.ly };
	RECT dd = { 0, 0, drawRT.lx - drawRT.fx, drawRT.ly - drawRT.fy };
	FillRect(insDC, &dd, whiteB);

	POINT trpos[4] = {};
	trpos[0].x = rtPos[0].x - mnW.x;
	trpos[0].y = rtPos[0].y - mnH.x;
	trpos[1].x = rtPos[1].x - mnW.x;
	trpos[1].y = rtPos[1].y - mnH.x;
	trpos[2].x = rtPos[3].x - mnW.x;
	trpos[2].y = rtPos[3].y - mnH.x;

	int fxN = mOriginWidth / mFrameW;
	int fyN = mOriginHeight / mFrameH;
	int fx = mFrameW * (frame % fxN);
	int fy = mFrameH * (frame / fxN);

	PlgBlt(insDC, trpos, image.GetDC(), fx, fy, mFrameW, mFrameH, 0, 0, 0);

	TransparentBlt(hdc, drawRT.fx, drawRT.fy, drawRT.lx - drawRT.fx, drawRT.ly - drawRT.fy,
		insDC, 0, 0, drawRT.lx - drawRT.fx, drawRT.ly - drawRT.fy, RGB(255, 255, 255));

	image.ReleaseDC();
	SelectObject(insDC, OldBit);
	DeleteDC(insDC);
	DeleteObject(hBit);
}

void Sprite::DrawTransfromFrameAlphaBlend(HDC hdc, int frame, int x, int y, int w, int h, float rotation_Angle, shp::vec2f centerPos, int alpha)
{
	RECT dd = { 0, 0, w, h };

	RECT ScreenRT = rt;
	shp::rect4f scrt = shp::rect4f(x, y, x + w, y + h);
	if (x + w > ScreenRT.right) {
		scrt.lx = ScreenRT.right;
	}
	if (x < ScreenRT.left) {
		scrt.fx = ScreenRT.left;
	}
	if (y + h > ScreenRT.bottom) {
		scrt.ly = ScreenRT.bottom;
	}
	if (y < ScreenRT.top) {
		scrt.fy = ScreenRT.top;
	}

	HBITMAP hAlphaBit;
	hAlphaBit = CreateCompatibleBitmap(hdc, w, h);
	HDC hAlphaDC = CreateCompatibleDC(hdc);
	HBITMAP OldBit = (HBITMAP)SelectObject(hAlphaDC, hAlphaBit);

	FillRect(hAlphaDC, &dd, whiteB);
	TransparentBlt(hAlphaDC, (int)scrt.fx - x, (int)scrt.fy - y, (int)scrt.getw(), (int)scrt.geth(),
		hdc, (int)scrt.fx, (int)scrt.fy, (int)scrt.getw(), (int)scrt.geth(), RGB(255, 255, 255));

	DrawTransfromFrame(hAlphaDC, frame, 0, 0, w, h, rotation_Angle, centerPos);

	BLENDFUNCTION bf;
	bf.BlendOp = AC_SRC_OVER;
	bf.BlendFlags = 0;
	bf.SourceConstantAlpha = alpha;
	bf.AlphaFormat = 0;
	AlphaBlend(hdc, scrt.fx, scrt.fy, scrt.getw(), scrt.geth(),
		hAlphaDC, (int)scrt.fx - x, (int)scrt.fy - y, (int)scrt.getw(), (int)scrt.geth(), bf);

	SelectObject(hAlphaDC, OldBit);
	DeleteDC(hAlphaDC);
	DeleteObject(hAlphaBit);
}
