#pragma once
#include "Game.h"
#include "GameUI.h"
#include "Effect.h"
#include "EditStation.h"
#include "StringFuncs.h"

class LogoPage : GameObject{
	bool first = true;
	bool wait = false;
	shp::vec2f flow = shp::vec2f(0, 3);
public:
	LogoPage();

	virtual ~LogoPage();

	LogoPage* Init(const shp::rect4f& loc, const int& layer);
	void FirstInit();

	virtual void Update(const float& delta) override;

	virtual void Event(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam) override;

	virtual void Render(HDC hdc) override;

	void NextPage();
};

class TitlePage : GameObject {
	bool first = true;
	bool wait = false;
	shp::vec2f flow = shp::vec2f(0, 3);

	Sprite* TitleSprite = nullptr;
	Sprite* BackGroundSprite = nullptr;
	ParrellexEf* parEF = nullptr;
	shp::vec2f TitleAppearFlow = shp::vec2f(0, 1);

	GameButton* GameStartButton = nullptr;

	GameButton* NextMenu = nullptr;
	GameButton* PrevMenu = nullptr;
	GameButton* SelectBtn = nullptr;

	Sprite* IconImage[4] = {};
	shp::vec2f iconChangeFlow = shp::vec2f(0, 0.5f);
	int mdir = 1;
	int iconNum = 0;

	bool ismenu = false;
	char nextChar = 'p';
public:
	TitlePage();

	virtual ~TitlePage();

	TitlePage* Init(const shp::rect4f& loc, const int& layer);
	void FirstInit();

	virtual void Update(const float& delta) override;

	virtual void Event(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam) override;

	virtual void Render(HDC hdc) override;

	void Select(char menuChar);

	void NextPage();

	void MoveMenu(int dir);

	void GameStart();

	const int& GetIconNum();

	ParrellexEf* GetParrellexEF() {
		if (enable) {
			return parEF;
		}
		else {
			return nullptr;
		}
	}

	void SetParrellexEF(ParrellexEf* pe) {
		parEF = pe;
	}
};

void IFClickGameStart(const GameButton* obj, const HWND& hWnd, const UINT& iMessage, const WPARAM& wParam, const LPARAM& lParam);

void IFClickNext(const GameButton* obj, const HWND& hWnd, const UINT& iMessage, const WPARAM& wParam, const LPARAM& lParam);

void IFClickPrev(const GameButton* obj, const HWND& hWnd, const UINT& iMessage, const WPARAM& wParam, const LPARAM& lParam);

void IFClickSelect(const GameButton* obj, const HWND& hWnd, const UINT& iMessage, const WPARAM& wParam, const LPARAM& lParam);

class LevelSelectPage : GameObject {
	bool first = true;
	bool wait = false;
	shp::vec2f flow = shp::vec2f(0, 3);

	PlayerData* pdata;

	ParrellexEf* parEF = nullptr;

	/*GameButton* ShopBtn = nullptr;
	GameButton* ProfileBtn = nullptr;
	GameButton* SettingBtn = nullptr;*/
	//GameButton* PlaySettingBtn = nullptr;

	GameButton* ReturnBtn = nullptr;

	GameButton* PlayBtn = nullptr;

	GameButton* NextBtn = nullptr;
	GameButton* PrevBtn = nullptr;

	int selectLevel = 0;
	int sdir = -1;
	shp::vec2f LevelChangeFlow = shp::vec2f(0, 0);

	char* Level_Name = nullptr;
	CImage* Level_AlbumImage = nullptr;
	
	char nextpage = 'p';
public:
	LevelSelectPage();

	virtual ~LevelSelectPage();

	LevelSelectPage* Init(const shp::rect4f& loc, const int& layer);
	void FirstInit();

	virtual void Update(const float& delta) override;

	virtual void Event(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam) override;

	virtual void Render(HDC hdc) override;

	void NextPage();
	void MoveSong(int dir);
	void GoTitle();
	void GoSetting();

	ParrellexEf* GetParrellexEF() {
		if (enable) {
			return parEF;
		}
		else {
			return nullptr;
		}
	}

	void SetParrellexEF(ParrellexEf* pe) {
		parEF = pe;
	}

	void LoadLevelName();

	void GoPlayStation();
};

void IFClickLevelPlay(const GameButton* obj, const HWND& hWnd, const UINT& iMessage, const WPARAM& wParam, const LPARAM& lParam);

void IFClickNextLevel(const GameButton* obj, const HWND& hWnd, const UINT& iMessage, const WPARAM& wParam, const LPARAM& lParam);

void IFClickPrevLevel(const GameButton* obj, const HWND& hWnd, const UINT& iMessage, const WPARAM& wParam, const LPARAM& lParam);

void IFClickReturnTitle(const GameButton* obj, const HWND& hWnd, const UINT& iMessage, const WPARAM& wParam, const LPARAM& lParam);

void IFClickSetting(const GameButton* obj, const HWND& hWnd, const UINT& iMessage, const WPARAM& wParam, const LPARAM& lParam);

class LevelPlayPage : GameObject{
	bool first = true;
	bool wait = false;
	shp::vec2f flow = shp::vec2f(0, 3);

	PlayStation* pdata;

	int levelnum = 0;
	char songName[128] = {};

	GameButton* ESCButton = nullptr;

	float MaxTime = 0;

	
public:
	char pstate = 'e';
	LevelPlayPage();

	virtual ~LevelPlayPage();

	LevelPlayPage* Init(const shp::rect4f& loc, const int& layer, int levelNum, const int& maxtime);
	void FirstInit();

	virtual void Update(const float& delta) override;

	virtual void Event(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam) override;

	virtual void Render(HDC hdc) override;

	void NextPage();
};

void IFClickESC(const GameButton* obj, const HWND& hWnd, const UINT& iMessage, const WPARAM& wParam, const LPARAM& lParam);

class EvaluatePage : GameObject {
	bool first = true;
	bool wait = false;
	shp::vec2f flow = shp::vec2f(0, 3);

	GameButton* RetryButton = nullptr;
	GameButton* NextButton = nullptr;

	Rank rank = Rank::F;
	int Score = 0;
	int perfectnum;
	int goodnum;
	int missnum;
	int maxcombo;
	bool isfullcombo;

	NumberEffect* ScoreNE = nullptr;

	int levelNum = 0;
	char nstate = 's';
	
public:
	EvaluatePage();

	virtual ~EvaluatePage();

	EvaluatePage* Init(const shp::rect4f& loc, const int& layer, Rank r, int score, int perfectN,
		int goodN, int missN, int MaxCombo, bool FullCombo);
	void FirstInit();

	virtual void Update(const float& delta) override;

	virtual void Event(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam) override;

	virtual void Render(HDC hdc) override;

	void SetSongNum(int n);

	void NextPage();

	void GoLevelSelect();

	void GoPlayLevel();
};

void IFClickEvNext(const GameButton* obj, const HWND& hWnd, const UINT& iMessage, const WPARAM& wParam, const LPARAM& lParam);
void IFClickEvRetry(const GameButton* obj, const HWND& hWnd, const UINT& iMessage, const WPARAM& wParam, const LPARAM& lParam);

class SettingPage : GameObject {
	bool first = true;
	char nstate = 't';

	GameButton* ReturnBtn = nullptr;

	GameSlider* BackGroundMusicSlider = nullptr;
	GameSlider* PlayMusicSlider = nullptr;
	GameSlider* SoundEffectSlider = nullptr;
	GameSlider* SyncSlider = nullptr;
	GameSlider* BasicSpeedSlider = nullptr;
	GameSlider* RimSoundVolumeSlider = nullptr;

public:
	SettingPage();

	virtual ~SettingPage();

	SettingPage* Init(const shp::rect4f& loc, const int& layer);
	void FirstInit();

	virtual void Update(const float& delta) override;

	virtual void Event(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam) override;

	virtual void Render(HDC hdc) override;

	void NextPage();
};

void IFClickSetReturn(const GameButton* obj, const HWND& hWnd, const UINT& iMessage, const WPARAM& wParam, const LPARAM& lParam);

class CreditPage : GameObject {
	bool first = true;
	char nstate = 't';
	TCHAR creditTEXT[600] = L"credit\n«—±π∞¯«–¥Î«–±≥ ¿©µµøÏ «¡∑Œ±◊∑°π÷ 12∆¿\nOtornait\n\n«¡∑Œ±◊∑°π÷ : ≥Î»∆√∂\n±‚»π : ≥Î»∆√∂, ¿Ã√¢πŒ\næ∆∆Æ : ≥Î»∆√∂, ¿Ã√¢πŒ\n≥Î∆Æµ•¿Ã≈Õ : ≥Î»∆√∂, ¿Ã√¢πŒ\n\n≥Î∑°ºˆ±ﬁ : ¿Ã√¢πŒ\nChartreuseGreen_H : t + pazolite\nHiatus_Departure : Aoi\nRaSeed : Aoi\n\n¿€∞Ó : ≥Î»∆√∂\nRift\ndays20\n\n∞Ó πË∞Ê ¿ÃπÃ¡ˆ : suke\nRift : lumberjacck(WallHeaven - https:/whvn.cc/j5oqqq)\nday20 : JustJon (WallHeaven - https:/whvn.cc/1kjyjv)\n\nªÁøÓµÂ Ω√Ω∫≈€ : FMOD SoundSystem\n∞≥πﬂ ææÓ : C++\nAPI : Windows\n\nIcon from.Iconfinder\n\n«√∑π¿Ã «ÿ¡÷º≈º≠ ∞®ªÁ«’¥œ¥Ÿ.";

	GameButton* ReturnBtn = nullptr;
public:
	CreditPage();

	virtual ~CreditPage();

	CreditPage* Init(const shp::rect4f& loc, const int& layer);
	void FirstInit();

	virtual void Update(const float& delta) override;

	virtual void Event(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam) override;

	virtual void Render(HDC hdc) override;

	void NextPage();
};

void IFClickCriReturn(const GameButton* obj, const HWND& hWnd, const UINT& iMessage, const WPARAM& wParam, const LPARAM& lParam);