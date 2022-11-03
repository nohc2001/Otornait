#pragma once
#include <Windows.h>
#include "SpaceMath.h"
#include "HeapDebug.h"
#include "Game.h"

typedef enum class UIType {
	button = 1,
	none = 100
};

class GameUI {
protected:
	UIType uiType;
	bool enable;
public:

	static constexpr int GAMEUI_MAX = 100;
	static GameUI* UIArr[GAMEUI_MAX];
	static int GameUIUpdate;

	static shp::vec2f MousePos;
	static bool LBtnPressed;
	static bool RBtnPressed;

	GameUI() {
		uiType = UIType::none;
		enable = true;
	}

	virtual ~GameUI() {

	}

	void SetEnable(const bool& b);
	const bool& GetEnable() const;

	virtual void RenderObject(const HDC& hdc) const {
	}

	virtual void Event(const HWND& hWnd, const UINT& iMessage, const WPARAM& wParam, const LPARAM& lParam) {
	}

	virtual void Update(const float& delta) {
	}
};

class GameButton : GameUI{
public:
	shp::rect4f location;
	bool bePressed;
	float flow = 0;
	TCHAR str[128];

	void (*RenderFunc)(const GameButton* obj, const HDC&) = nullptr;
	void (*OnClick)(const GameButton*, const HWND&, const UINT&, const WPARAM&, const LPARAM&);

	GameObject* Parent = nullptr;

	int parameter[10] = {};

	GameButton();

	virtual ~GameButton();

	void SetSTR(const TCHAR* tstr) {
		if (enable) {

			if (wcslen(tstr) > 0) {
				wcscpy_s(str, tstr);
			}

			void* ptr = (void*)tstr;
			if (HeapDebugClass::HeapDebug[ptr] == true) {
				HeapDebugClass::HeapArrDelete(tstr);
			}
		}
	}

	virtual void RenderObject(const HDC& hdc) const {
		if (enable) {
			if (RenderFunc == nullptr) {
				Rectangle(hdc, location.fx, location.fy, location.lx, location.ly);
				RECT lrt = { location.fx, location.fy, location.lx, location.ly };
				SetTextColor(hdc, RGB(0, 0, 0));
				DrawText(hdc, str, lstrlen(str), &lrt, DT_CENTER);
			}
			else {
				RenderFunc(this, hdc);
			}
		}
	}

	virtual void Event(const HWND& hWnd, const UINT& iMessage, const WPARAM& wParam, const LPARAM& lParam) {
		if (enable) {
			if (OnClick == nullptr) {

			}
			else {
				if (LBtnPressed && shp::bPointInRectRange(MousePos, location)) {
					OnClick(this, hWnd, iMessage, wParam, lParam);
				}
			}
		}
	}

	virtual void Update(const float& delta) {
		if (enable) {
			flow += delta;
		}
	}

	void SetParent(GameObject* parent) {
		if (enable) {
			Parent = parent;
		}
	}
};

void ImageBtnRender(const GameButton* obj, const HDC& hdc);

class GameSlider : GameUI {
public:
	shp::rect4f location;
	bool bePressed;
	TCHAR str[128];
	float value = 0;
	bool move = false;

	void (*RenderFunc)(const GameSlider* obj, const HDC&) = nullptr;
	void (*OnEvent)(const GameSlider*, const HWND&, const UINT&, const WPARAM&, const LPARAM&);

	GameObject* Parent = nullptr;

	int parameter[10] = {};

	GameSlider() {

	}

	virtual ~GameSlider() {

	}

	void SetSTR(const TCHAR* tstr) {
		if (enable) {

			if (wcslen(tstr) > 0) {
				wcscpy_s(str, tstr);
			}

			void* ptr = (void*)tstr;
			if (HeapDebugClass::HeapDebug[ptr] == true) {
				HeapDebugClass::HeapArrDelete(tstr);
			}
		}
	}

	virtual void RenderObject(const HDC& hdc) const {
		if (enable) {
			if (RenderFunc == nullptr) {
				shp::vec2f cen = location.getCenter();
				float w = location.getw();
				float h = location.geth();
				
				shp::rect4f crt = shp::rect4f(cen.x - w / 2 + w * value - h / 2, cen.y - h / 2, cen.x - w / 2 + w * value + h / 2, cen.y + h / 2);
				
				Rectangle(hdc, cen.x - w / 2, cen.y - h / 8, cen.x + w / 2, cen.y + h / 8);
				Rectangle(hdc, cen.x - w / 2 + w*value - h / 8, cen.y - h/2, cen.x - w / 2 + w * value + h / 8, cen.y + h / 2);
				if (shp::bPointInRectRange(GameUI::MousePos, crt)) {
					Rectangle(hdc, cen.x - w / 2 + w * value - h / 16, cen.y - h / 2, cen.x - w / 2 + w * value + h / 16, cen.y + h / 2);
				}
				RECT lrt = { location.fx, location.fy, location.lx, location.ly };
				SetTextColor(hdc, RGB(0, 0, 0));

				DrawText(hdc, str, lstrlen(str), &lrt, DT_LEFT | DT_TOP);
			}
			else {
				RenderFunc(this, hdc);
			}
		}
	}

	virtual void Event(const HWND& hWnd, const UINT& iMessage, const WPARAM& wParam, const LPARAM& lParam) {
		if (enable) {
			if (OnEvent == nullptr) {
				shp::vec2f cen = location.getCenter();
				float w = location.getw();
				float h = location.geth();
				if (iMessage == WM_LBUTTONDOWN) {
					if (move == false) {
						shp::rect4f crt = shp::rect4f(cen.x - w / 2 + w * value - h / 8, cen.y - h / 2, cen.x - w / 2 + w * value + h / 8, cen.y + h / 2);
						if (shp::bPointInRectRange(GameUI::MousePos, crt)) {
							move = true;
						}
					}
				}
				else if (iMessage == WM_LBUTTONUP) {
					if (move) {
						move = false;
					}
				}
				else if (iMessage == WM_MOUSEMOVE) {
					if (move) {
						float xx = GameUI::MousePos.x;
						float fx = cen.x - w / 2;
						float lx = cen.x + w / 2;
						if (xx <= fx) value = 0;
						else if (fx < xx && xx < lx) value = (xx - fx) / w;
						else if (xx >= lx) value = 1;
					}
				}
				
			}
			else {
				OnEvent(this, hWnd, iMessage, wParam, lParam);
			}
		}
	}

	virtual void Update(const float& delta) {
		if (enable) {
			shp::vec2f cen = location.getCenter();
			float w = location.getw();
			float h = location.geth();
			if (move) {
				float xx = GameUI::MousePos.x;
				float fx = cen.x - w / 2;
				float lx = cen.x + w / 2;
				if (xx <= fx) value = 0;
				else if (fx < xx && xx < lx) value = (xx - fx) / w;
				else if (xx >= lx) value = 1;
			}
		}
	}

	void SetParent(GameObject* parent) {
		if (enable) {
			Parent = parent;
		}
	}
};