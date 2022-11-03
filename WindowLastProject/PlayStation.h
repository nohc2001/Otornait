#pragma once
#include <Windows.h>
#include <fstream>

#include "Game.h"
#include "Animation.h"
#include "HeapDebug.h"
#include "SpaceMath.h"
#include "Effect.h"
#include "Sprite.h"
#include "Music.h"

using namespace std;

typedef enum class NOTE_TYPE {
	None = 0,
	NormalNote = 1,
	LongNote = 2,
	SpaceLeftNote = 3,
	SpaceNote = 4,
	SpaceRightNote = 5
};

typedef enum class ROTPOS {
	bottom = 0,
	left = 1,
	top = 2,
	right = 3
};

typedef struct Note {
	float time = 0;
	float endtime = 0; // 롱 노트일 경우..
	ROTPOS rotPos; // 회전위치
	int pos; // 위치
	NOTE_TYPE ntype;

	//저장되지 않고 실시간으로만 존재하는 설정
	bool enable = true; // 쳐진 노트는 false처리됨.
};

typedef enum class SHOW_TYPE {
	None = 0,
	EdgeSpreadEF = 1,
	LineDiamondRotateEF = 2,
	ExplosionEF = 3,
	FlashEF = 4,
	LineDirEF = 5
};

class Show {
public:
	bool autoShow = true; // 노트를 치지 않아도 보여주는지..
	int nodenum; // 어떤 노트를 쳐야 보여주는지
	float time; // 언제 보여주는지
	SHOW_TYPE stype; // 무엇을 보여주는 지
	float param[10]; // 파라미터
	bool enable = true;

	Show() {
		for (int i = 0; i < 10; ++i) {
			param[i] = 0;
		}
	}

	virtual ~Show() {

	}

	virtual void Excute(GameManager* gm, shp::rect4f loc) {
		if (enable) {
			float ww = loc.geth() / 30.0f;
			constexpr int circleCount = 20;
			switch (stype) {
			case SHOW_TYPE::EdgeSpreadEF:
			{
				
				int v = param[0];
				switch (v) {
				case 0:
					//bottom
				{
					uintColor c1 = uintColor(255, 255, 255);
					uintColor c2 = uintColor(0, 0, 0);
					GameManager* GM = (GameManager*)gm;
					GM->AddObject((GameObject*)HeapDebugClass::HeapNew<ManyGoCircleEf>()->Init
					(shp::rect4f(0, 0, 0, 0), 9, shp::rect4f(loc.fx, loc.ly - 100, loc.lx, loc.ly), shp::angle2f::usedxdy(0, -1),
						shp::angle2f(0, true), shp::vec2f(param[1], 1), shp::vec2f(ww, ww/2), c1, c2, circleCount, shp::vec2f(500, 100), shp::vec2f(0.7f, 0.3f)));
				}
					break;
				case 1:
					//left
				{
					uintColor c1 = uintColor(255, 255, 255);
					uintColor c2 = uintColor(0, 0, 0);
					GameManager* GM = (GameManager*)gm;
					GM->AddObject((GameObject*)HeapDebugClass::HeapNew<ManyGoCircleEf>()->Init
					(shp::rect4f(0, 0, 0, 0), 9, shp::rect4f(loc.fx, loc.fy, loc.fx+100, loc.ly), shp::angle2f::usedxdy(1, 0),
						shp::angle2f(0, true), shp::vec2f(param[1], 1), shp::vec2f(ww, ww / 2), c1, c2, circleCount, shp::vec2f(500, 100), shp::vec2f(0.7f, 0.3f)));
				}
					break;
				case 2:
					//top
				{
					uintColor c1 = uintColor(255, 255, 255);
					uintColor c2 = uintColor(0, 0, 0);
					GameManager* GM = (GameManager*)gm;
					GM->AddObject((GameObject*)HeapDebugClass::HeapNew<ManyGoCircleEf>()->Init
					(shp::rect4f(0, 0, 0, 0), 9, shp::rect4f(loc.fx, 0, loc.lx, 100), shp::angle2f::usedxdy(0, 1),
						shp::angle2f(0, true), shp::vec2f(param[1], 1), shp::vec2f(ww, ww / 2), c1, c2, circleCount, shp::vec2f(500, 100), shp::vec2f(0.7f, 0.3f)));
				}
					break;
				case 3:
					//right
				{
					uintColor c1 = uintColor(255, 255, 255);
					uintColor c2 = uintColor(0, 0, 0);
					GameManager* GM = (GameManager*)gm;
					GM->AddObject((GameObject*)HeapDebugClass::HeapNew<ManyGoCircleEf>()->Init
					(shp::rect4f(0, 0, 0, 0), 9, shp::rect4f(loc.lx-100, loc.fy, loc.lx, loc.ly), shp::angle2f::usedxdy(-1, 0),
						shp::angle2f(0, true), shp::vec2f(param[1], 1), shp::vec2f(ww, ww / 2), c1, c2, circleCount, shp::vec2f(500, 100), shp::vec2f(0.7f, 0.3f)));
				}
					break;
				}
			}
				break;
			case SHOW_TYPE::LineDiamondRotateEF:
			{
				int p0 = param[0];
				shp::vec2f cen = loc.getCenter();
				float w = 100;
				shp::rect4f toprt = shp::rect4f(cen.x - w, loc.fy - w, cen.x + w, loc.fy + w);
				shp::rect4f bottomrt = shp::rect4f(cen.x - w, loc.ly - w, cen.x + w, loc.ly + w);
				shp::rect4f leftrt = shp::rect4f(loc.fx - w, cen.y - w, loc.lx + w, cen.y + w);
				shp::rect4f rightrt = shp::rect4f(loc.lx - w, cen.y - w, loc.lx + w, cen.y + w);
				switch (p0) {
				case 0:
				{
					//시계방향
					uintColor c1 = uintColor(255, 255, 255);
					uintColor c2 = uintColor(0, 0, 0);
					GameManager* GM = (GameManager*)gm;
					GM->AddObject((GameObject*)HeapDebugClass::HeapNew<ManyGoCircleEf>()->Init
					(shp::rect4f(0, 0, 0, 0), 9, bottomrt, shp::angle2f::usedxdy(-1, -1),
						shp::angle2f(0, true), shp::vec2f(param[1], 1), shp::vec2f(ww, ww / 2), c1, c2, circleCount, shp::vec2f(500, 100), shp::vec2f(0.7f, 0.3f)));
					GM->AddObject((GameObject*)HeapDebugClass::HeapNew<ManyGoCircleEf>()->Init
					(shp::rect4f(0, 0, 0, 0), 9, leftrt, shp::angle2f::usedxdy(1, -1),
						shp::angle2f(0, true), shp::vec2f(param[1], 1), shp::vec2f(ww, ww / 2), c1, c2, circleCount, shp::vec2f(500, 100), shp::vec2f(0.7f, 0.3f)));
					GM->AddObject((GameObject*)HeapDebugClass::HeapNew<ManyGoCircleEf>()->Init
					(shp::rect4f(0, 0, 0, 0), 9, toprt, shp::angle2f::usedxdy(1, 1),
						shp::angle2f(0, true), shp::vec2f(param[1], 1), shp::vec2f(ww, ww / 2), c1, c2, circleCount, shp::vec2f(500, 100), shp::vec2f(0.7f, 0.3f)));
					GM->AddObject((GameObject*)HeapDebugClass::HeapNew<ManyGoCircleEf>()->Init
					(shp::rect4f(0, 0, 0, 0), 9, rightrt, shp::angle2f::usedxdy(-1, 1),
						shp::angle2f(0, true), shp::vec2f(param[1], 1), shp::vec2f(ww, ww / 2), c1, c2, circleCount, shp::vec2f(500, 100), shp::vec2f(0.7f, 0.3f)));
				}
					break;
				case 1:
				{
					//시계반대방향
					uintColor c1 = uintColor(255, 255, 255);
					uintColor c2 = uintColor(0, 0, 0);
					GameManager* GM = (GameManager*)gm;
					GM->AddObject((GameObject*)HeapDebugClass::HeapNew<ManyGoCircleEf>()->Init
					(shp::rect4f(0, 0, 0, 0), 9, bottomrt, shp::angle2f::usedxdy(1, -1),
						shp::angle2f(0, true), shp::vec2f(param[1], 1), shp::vec2f(ww, ww / 2), c1, c2, 25, shp::vec2f(500, 100), shp::vec2f(0.7f, 0.3f)));
					GM->AddObject((GameObject*)HeapDebugClass::HeapNew<ManyGoCircleEf>()->Init
					(shp::rect4f(0, 0, 0, 0), 9, leftrt, shp::angle2f::usedxdy(1, 1),
						shp::angle2f(0, true), shp::vec2f(param[1], 1), shp::vec2f(ww, ww / 2), c1, c2, 25, shp::vec2f(500, 100), shp::vec2f(0.7f, 0.3f)));
					GM->AddObject((GameObject*)HeapDebugClass::HeapNew<ManyGoCircleEf>()->Init
					(shp::rect4f(0, 0, 0, 0), 9, toprt, shp::angle2f::usedxdy(-1, 1),
						shp::angle2f(0, true), shp::vec2f(param[1], 1), shp::vec2f(ww, ww / 2), c1, c2, 25, shp::vec2f(500, 100), shp::vec2f(0.7f, 0.3f)));
					GM->AddObject((GameObject*)HeapDebugClass::HeapNew<ManyGoCircleEf>()->Init
					(shp::rect4f(0, 0, 0, 0), 9, rightrt, shp::angle2f::usedxdy(-1, -1),
						shp::angle2f(0, true), shp::vec2f(param[1], 1), shp::vec2f(ww, ww / 2), c1, c2, 25, shp::vec2f(500, 100), shp::vec2f(0.7f, 0.3f)));
				}
					break;
				}
			}
				break;
			case SHOW_TYPE::ExplosionEF:
				break;
			case SHOW_TYPE::FlashEF:
				break;
			case SHOW_TYPE::LineDirEF:
				break;
			}
		}
	}

	virtual void Render(HDC hdc) {

	}
};

typedef struct KeyData {
	bool S_pressed = false;
	shp::vec2f S_dut = shp::vec2f(0, 0);
	bool SHit = false;

	bool D_pressed = false;
	shp::vec2f D_dut = shp::vec2f(0, 0);
	bool DHit = false;

	bool K_pressed = false;
	shp::vec2f K_dut = shp::vec2f(0, 0);
	bool KHit = false;

	bool L_pressed = false;
	shp::vec2f L_dut = shp::vec2f(0, 0);
	bool LHit = false;

	bool Space_pressed = false;
	shp::vec2f Space_dut = shp::vec2f(0, 0);
	bool SpaceHit = false;
};

class PlayStation : GameObject{
private:
	bool isplaying = true; // false면 멈출 수 있게
	bool autoplaying = false; // true면 input이 없어도 자동으로 쳐짐

	float Time = -1;
	int Score; // 1000000
	int Combo;
	int MaxCombo;

	int Tempo;
	ROTPOS rotation;

	Note* noteArr = nullptr;
	int noteMAX = 0;
	int nextNoteNum = 0; // 시간이 지날수록 그 시간에 맞는 노트를 가리키도록 함.

	Show* ShowArr = nullptr;
	int showMAX = 0;
	int nextShowNum = 0;

	Sprite* BackGround;
	float Impact = 0;
	float AddImpact = 0.15f;

	char songName[128] = {};
	int songSoundID = -1;

	int RimSoundID = 0;

	//감춰진 데이터--------------------------------------------
	int addScore = 1000000000;
	int perfectnum = 0;
	int goodnum = 0;
	int missnum = 0;

	float accelPower = 3;
	float noteWidthRate = 0.7f;
	float NoteVisibleLength = 1; // 몇초 후의 노트까지 보여줄 건지.
	static constexpr int readFutureNum = 8; // 미래 몇개를 볼건지
	int notelocArr[5][100] = { {} }; // (1, 2, 3, 4, space)다섯가지 위치에서 각각 판단하기 위해
	KeyData keydata;
	float perfectCloseValue = 0.05f; // 얼마나 정확하게 판정을 젤 것인가? (초)
	float goodCloseValue = 0.1f; // 얼마나 정확하게 판정을 젤 것인가? (초)
	float missCloseValue = 0.2f; // 얼마나 정확하게 판정을 젤 것인가? (초)

	//표시되는 good / perfect / miss
	char laststate = 'p'; // p : perfect / g : good / m : miss / n : none
	shp::vec2f stateFlow = shp::vec2f(0, 1);
	float statePow = 4;

	NumberEffect* NE;
	NumberEffect* ComboNE;

	//회전flow
	static constexpr float RotatePower = 5;
	shp::vec2f RotateFlow = shp::vec2f(0, 0.5f);
	bool rotating = false;
	bool rotateLeft = false;

	//롱노트를 어떻게 처리할 것인가?
	bool isLongState[5] = {};

	//누를 수 있는 노트를 표시
	shp::vec2f PushNote[4][2]; // 0 : 이전 위치, 1 : 이후 위치
	float pnW = 20;
	
	bool first = true;
	bool musicplay = false;
public:
	// variable access
	PlayStation();

	virtual ~PlayStation();

	PlayStation* Init(const shp::rect4f& loc, const bool& autoplay, const int& layer);
	void FirstInit();

	void SetIsPlaying(const bool& bplaying);
	const bool& GetIsPlaying() const;

	void SetAutoMod(const bool& mod);
	const bool& GetAutoMod() const;

	void SetTime(const float& t);
	const float& GetTime() const;

	void SetScore(const int& score);
	const int& GetScore() const;

	void SetCombo(const int& com);
	const int& GetCombo() const;

	void SetMaxCombo(const int& maxcom);
	const int& GetMaxCombo() const;

	void SetTempo(const int& tempo);
	const int& GetTempo() const;

	void SetRotPos(const ROTPOS& rot);
	const ROTPOS& GetRotPos() const;

	void SetNextNoteNum(const int& num);
	const int& GetNextNoteNum() const;

	void SetMaxNoteNum(const int& num);
	const int& GetMaxNoteNum() const;

	void SetNextShowNum(const int& num);
	const int& GetNextShowNum() const;

	void SetMaxShowNum(const int& num);
	const int& GetMaxShowNum() const;

	void SetNote(const int& index, const Note& note);
	const Note& GetNote(const int& index) const;

	void SetShow(const int& index, const Show& show);
	const Show& GetShow(const int& index) const;

	const int& GetPGMNum(char state);

	const int& GetSongSoundID();

	shp::rect4f GetPlayLoc() {
		shp::rect4f wloc = GetLocation();

		shp::vec2f center = wloc.getCenter();
		float w = 0.6f * wloc.geth()/2;
		shp::rect4f playloc = shp::rect4f(center.x - w, center.y - w,
			center.x + w, center.y + w);

		return playloc;
	}

	const shp::vec2f GetSPos(ROTPOS rot, int pos, const shp::rect4f& playloc);
	const shp::vec2f GetEPos(ROTPOS rot, int pos, const shp::rect4f& playloc);

	void Rotate(bool left);
	ROTPOS GetNextROTPOS(ROTPOS rot, bool left);

	void Rim();

	void Hit(char pan, Note note);

	void GoPosition(float position);

	void LoadMusic(const char* musicName);
	//save/load

	void SetBackGround(TCHAR* fileName);

	void LoadData(const char* filename);
	void SaveData(const char* filename);
	void LoadNotes(vector<Note> narr);
	void LoadShows(vector<Show> sarr);
	//arrange data
	void Arrange();

	virtual void Update(const float& delta) override;

	virtual void Event(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam) override;

	void DrawNote(HDC hdc, shp::rect4f playloc, Note note);
	virtual void Render(HDC hdc) override;
};