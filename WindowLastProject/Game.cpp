#include "Game.h"

GoCircleEF::GoCircleEF()
{
}

GoCircleEF::~GoCircleEF()
{
}

GoCircleEF* GoCircleEF::Init(const int& Layer, shp::vec2f startPos, shp::vec2f Dir, float maxtime, float Radius)
{
	enable = true;
	layer = Layer;
	StartPos = startPos;
	EndPos = StartPos + Dir;
	flow = shp::vec2f(0, maxtime);
	radius = Radius;
	return this;
}

void GoCircleEF::SetValue(float pow, COLORREF col)
{
	power = pow;
	color = col;
}

void GoCircleEF::Update(const float& delta)
{
	if (enable) {
		flow.x += delta;
		if (flow.x > flow.y) {
			enable = false;
		}
	}
}

void GoCircleEF::Render(HDC hdc)
{
	if (enable) {
		float rate = 0;
		float raiseR = 0.2f;
		if (flow.x < flow.y * raiseR) {
			rate = AnimClass::EaseIO(flow.x / (flow.y * raiseR), power);
		}
		else {
			rate = 1 - AnimClass::EaseIn((flow.x - (flow.y * raiseR)) / (flow.y * (1 - raiseR)), power);
		}

		float posrate = AnimClass::EaseOut(flow.x / flow.y, power);
		shp::vec2f pos = EndPos * posrate + StartPos * (1 - posrate);
		float pr = radius * rate;

		HBRUSH wb = CreateSolidBrush(color);
		HPEN wp = CreatePen(PS_SOLID, 0, color);
		SelectObject(hdc, wp);
		SelectObject(hdc, wb);
		Ellipse(hdc, pos.x - pr, pos.y - pr, pos.x + pr, pos.y + pr);
		DeleteObject(wb);
		DeleteObject(wp);
	}
}