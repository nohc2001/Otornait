#pragma once
#include "HeapDebug.h"
#include "Sprite.h"
#include "SpaceMath.h"
#include "Music.h"
#include "PlayerData.h"
#include "Animation.h"

constexpr int MAX_LAYER = 10;

class GoCircleEF {
private:
	shp::vec2f StartPos = shp::vec2f(0, 0);
	shp::vec2f EndPos = shp::vec2f(0, 0);
	shp::vec2f flow = shp::vec2f(0, 1);
	COLORREF color = RGB(255, 255, 255);
	float power = 5;
	float radius = 10;

	bool enable = false;
	int layer = 1;
public:
	GoCircleEF();
	virtual ~GoCircleEF();
	GoCircleEF* Init(const int& Layer, shp::vec2f startPos, shp::vec2f Dir, float maxtime, float Radius);
	void SetValue(float pow, COLORREF col);

	void Update(const float& delta);

	void Render(HDC hdc);

	bool GetEnable() {
		return enable;
	}

	int Getlayer() {
		return layer;
	}
};

class Ptr {
public:
	Ptr() {

	}

	virtual ~Ptr() {

	}
};

typedef enum class OBJ_TYPE {
	None = 0,
	PlayStation = 1,
	EditStation = 2
};

class GameObject {
protected:
	Sprite* spr;
	shp::rect4f location;
	bool enable = true;

	int layerNum = 0;

	Ptr* gm; // 이 게임오브젝트를 가진 게임 매니저
	OBJ_TYPE type = OBJ_TYPE::None;
public:
	GameObject() {
		spr = nullptr;
		location = { 0, 0, 0, 0 };
		enable = true;
	}

	virtual ~GameObject() {
		enable = false;
	}

	void SetSprite(Sprite* const sprite) {
		if (enable) {
			spr = sprite;
		}
	}

	Sprite* GetSprite() const {
		if (enable) {
			return spr;
		}
		else {
			return nullptr;
		}
	}

	void SetLocation(const shp::rect4f loc) {
		if (enable) {
			location = loc;
		}
	}

	const shp::rect4f& GetLocation() const {
		if (enable) {
			return location;
		}
		else {
			return shp::rect4f(0, 0, 0, 0);
		}
	}

	void SetEnable(const bool Enable) {
		enable = Enable;
	}

	const bool& GetEnable() const {
		return enable;
	}

	void SetLayer(const int& layer) {
		if (enable) {
			if (layer > MAX_LAYER) {
				layerNum = MAX_LAYER;
			}
			else {
				layerNum = layer;
			}
		}
	}

	const int& GetLayer() const {
		if (enable) {
			return layerNum;
		}
		else {
			return 0;
		}
	}

	void SetGameManager(Ptr* const gameM) {
		if (enable) {
			gm = gameM;
		}
	}

	Ptr* GetGameManager() const {
		if (enable) {
			return gm;
		}
		else {
			return nullptr;
		}
	}

	void SetObjType(OBJ_TYPE t) {
		if (enable) {
			type = t;
		}
	}

	const OBJ_TYPE GetObjType() const {
		if (enable) {
			return type;
		}
		else {
			return OBJ_TYPE::None;
		}
	}

	virtual void Update(const float& delta) {
		if (enable) {

		}
	}

	virtual void Event(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam) {
		if (enable) {

		}
	}

	virtual void Render(HDC hdc) {
		if (enable) {
			Sprite* s = GetSprite();
			shp::rect4f loc = GetLocation();
			if (s != nullptr) {
				s->DrawSprite(hdc, loc.fx, loc.fy, loc.getw(), loc.geth());
			}
		}
	}
};

class GameManager : Ptr {
private:
	bool end = false;

	//플레이어 데이터
	PlayerData pdata;

	static constexpr int OBJ_MAX = 2000;
	GameObject* objPool[OBJ_MAX] = {};
	
	GoCircleEF particleObj[OBJ_MAX] = {};
	int particleUp = 0;

	//10초마다 힙맵 정리와 플레이어 데이터 저장
	shp::vec2f HeapCheckFlow = shp::vec2f(0, 10);
public:
	int objup = 0;

	GameManager() {
		pdata.LoadData();
		InitMusic();
	}

	virtual ~GameManager() {
		end = true;
		pdata.Clear();

		for (int i = 0; i < objup; ++i) {
			if (objPool[i] != nullptr) {
				HeapDebugClass::HeapDelete<GameObject>(objPool[i]);
				objPool[i] = nullptr;
			}
		}

		Music::ClearSoundsAndChannels();
	}

	void Clear() {
		end = true;
		pdata.Clear();

		for (int i = 0; i < objup; ++i) {
			GameObject* obj = GetGameObject(i);
			if (HeapDebugClass::HeapDebug[obj] == true) {
				HeapDebugClass::HeapDelete<GameObject>(obj);
				objPool[i] = nullptr;
			}
		}

		objup = 0;
	}

	void InitMusic() {
		Music::ClearSoundsAndChannels();
		Music::Init();
		Music::AddSound("Sound\\rim.mp3", false, false);
		Music::ConnectSound(1, 0);
		Music::SetChannelVolume(1, 0.1f);
	}

	void AddObject(GameObject* const go) {
		if (objup + 1 < OBJ_MAX) {
			objPool[objup] = go;
			objPool[objup]->SetGameManager((Ptr*)this);
			objup += 1;
		}
		else {
			for (int i = 0; i < OBJ_MAX; ++i) {
				if (objPool[i] == nullptr) {
					objPool[i] = go;
					objPool[i]->SetGameManager((Ptr*)this);
					break;
				}
			}
		}

	}

	void AddParticle(GoCircleEF copyObj) {
		if (particleUp + 1 < OBJ_MAX) {
			particleObj[particleUp] = copyObj;
			particleUp += 1;
		}
		else {
			for (int i = 0; i < OBJ_MAX; ++i) {
				if (particleObj[i].GetEnable() == false) {
					particleObj[i] = copyObj;
					break;
				}
			}
		}
	}

	GameObject* GetGameObject(const int& index) {
		if (0 <= index && index < objup) {
			if (HeapDebugClass::HeapDebug[objPool[index]] == true) {
				return objPool[index];
			}
			else {
				objPool[index] = nullptr;
				return nullptr;
			}
		}
		else {
			return nullptr;
		}
	}

	void Update(const float& delta) {
		if (end) return;

		Music::Update();

		HeapCheckFlow.x += delta;
		if (HeapCheckFlow.x > HeapCheckFlow.y) {
			HeapCheckFlow.x = 0;
			HeapDebugClass::MapClear();
			pdata.SaveData();
		}

		for (int i = 0; i < objup; ++i) {
			if (objPool[i] != nullptr && HeapDebugClass::HeapDebug[objPool[i]] == true)
			{
				objPool[i]->Update(delta);
			}
		}

		for (int i = 0; i < objup; ++i) {
			if (objPool[i] != nullptr && HeapDebugClass::HeapDebug[objPool[i]] == true)
			{
				if (objPool[i]->GetEnable() == false) {
					HeapDebugClass::HeapDelete<GameObject>(objPool[i]);
					objPool[i] = nullptr;
				}
			}
		}

		for (int l = 0; l < MAX_LAYER; ++l) {
			for (int i = 0; i < particleUp; ++i) {
				if (particleObj[i].GetEnable())
				{
					if (particleObj[i].Getlayer() == l) {
						particleObj[i].Update(delta);
					}
				}
			}
		}
	}

	void Event(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam) {
		for (int i = 0; i < objup; ++i) {
			if (objPool[i] != nullptr && HeapDebugClass::HeapDebug[objPool[i]] == true)
			{
				objPool[i]->Event(hWnd, iMessage, wParam, lParam);
			}
		}
	}

	void Render(HDC hdc) {
		for (int l = 0; l < MAX_LAYER; ++l) {
			for (int i = 0; i < objup; ++i) {
				if (objPool[i] != nullptr && HeapDebugClass::HeapDebug[objPool[i]] == true)
				{
					if (objPool[i]->GetLayer() == l) {
						objPool[i]->Render(hdc);
					}
				}
			}

			for (int i = 0; i < particleUp; ++i) {
				if (particleObj[i].GetEnable())
				{
					if (particleObj[i].Getlayer() == l) {
						particleObj[i].Render(hdc);
					}
				}
			}
		}
	}

	PlayerData* GetPlayerData() {
		return &pdata;
	}
};