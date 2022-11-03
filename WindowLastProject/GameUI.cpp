#include "GameUI.h"

extern vector<Sprite*> SpriteData;

void GameUI::SetEnable(const bool& b) {
	enable = b;
}

const bool& GameUI::GetEnable() const
{
	return enable;
}

GameButton::GameButton()
{
	if (GameUIUpdate+1 < GAMEUI_MAX) {
		UIArr[GameUIUpdate] = this;
		GameUIUpdate += 1;
	}
	else {
		for (int i = 0; i < GAMEUI_MAX; ++i) {
			if (UIArr[i] == nullptr) {
				UIArr[i] = this;
				break;
			}
		}
	}
}

GameButton::~GameButton()
{
}

void ImageBtnRender(const GameButton* obj, const HDC& hdc) {
	if (shp::bPointInRectRange(GameUI::MousePos, obj->location)) {
		SpriteData[obj->parameter[0]]->DrawSprite(hdc, obj->location.fx - 0.1f * obj->location.getw(), obj->location.fy - 0.1f*obj->location.geth(),
			1.2f * obj->location.getw(), 1.2f * obj->location.geth());
	}
	else {
		SpriteData[obj->parameter[0]]->DrawSprite(hdc, obj->location.fx, obj->location.fy,
			obj->location.getw(), obj->location.geth());
	}
}