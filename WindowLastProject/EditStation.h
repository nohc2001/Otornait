#pragma once
#include "PlayStation.h"
#include "GameUI.h" 

class EditStation : GameObject{
	PlayStation* playStation; // 플레이 화면
	int Tempo = 120;
	float Time = 0;

	GameButton* RotPosButton[4]; // rotpos를 결정하는 버튼들
	
	GameButton* SetTempoButton; // 템포를 정하게 하는 버튼
	GameButton* SetVariableButton; // 각종 변수를 조정하는 버튼
	GameButton* PauseButton; // 플레이 / 스탑 버튼
	GameButton* HelpButton; // 도움말 버튼

	GameButton* LevelLoadButton; // 만든 레벨을 로드하는 버튼
	GameButton* LevelSaveButton; // 레벨을 저장하는 버튼
	GameButton* LoadMusicButton; // 음악을 로드하는 버튼

	bool first = true;
	bool bPlay = false;

	shp::rect4f EditRt;
	float MaxShowTime = 1.0f;

	NOTE_TYPE AddNoteType = NOTE_TYPE::NormalNote;
	vector<Note> EditNote;
	vector<Show> EditShow;
	shp::vec2f noteAddFlow = shp::vec2f(0, 0.2f);

	int NoteSelected = 0;
	int ShowSelected = 0;
public:
	static bool SetTempoDialogOpen;
	static int STD_Out_SetTempo;
	static bool STD_OUT_enable;
	
	static bool SetVariableDialogOpen;
	static int SVD_Out_SetTime;
	static int SVD_Out_SetMaxShowTime;
	static bool SVD_OUT_enable;

	static bool HelpDialogOpen;

	static bool PauseBtn_Enable;
	static bool PauseBtn_ModePlay;
	static shp::vec2f PauseBtn_Flow;

	static ROTPOS PresentROTPOS;

	static bool LoadLevelBtn_Enable;
	static bool LoadLevelBtn_Load;
	static char LoadLevelBtn_FileName[128];

	static bool SaveLevelBtn_Enable;
	static bool SaveLevelBtn_Load;
	static char SaveLevelBtn_FileName[128];

	static bool LoadMusicBtn_Enable;
	static bool LoadMusicBtn_Load;
	static char LoadMusicBtn_FileName[128];

	static SHOW_TYPE SelectedShowType;
	static float Parameter[10];
	static bool isAddShow;

	EditStation();
	
	virtual ~EditStation();

	void SetTempo(const int& t);
	const int& GetTempo() const;

	void SetTime(const float& t);
	const float& GetTime() const;

	void SetMaxShowTime(const float& t);
	const float& GetMaxShowTime() const;

	void SetEditRT(const shp::rect4f& ert);
	const shp::rect4f& GetEditRT() const;

	void Arrange();
	const bool& isNoteExist(float time, int pos);
	int FindNote(float time, int pos);
	int FindShow(float time);

	void LoadData(const char* str);
	void SaveData(const char* str);

	EditStation* Init(const shp::rect4f& loc, const int& layer);
	void FirstInit();

	void GetDialogData(float delta);

	virtual void Update(const float& delta) override;

	virtual void Event(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam) override;

	virtual void Render(HDC hdc) override;
};

void IFClickBottom(const GameButton* obj, const HWND& hWnd, const UINT& iMessage, const WPARAM& wParam, const LPARAM& lParam);
void IFClickLeft(const GameButton* obj, const HWND& hWnd, const UINT& iMessage, const WPARAM& wParam, const LPARAM& lParam);
void IFClickTop(const GameButton* obj, const HWND& hWnd, const UINT& iMessage, const WPARAM& wParam, const LPARAM& lParam);
void IFClickRight(const GameButton* obj, const HWND& hWnd, const UINT& iMessage, const WPARAM& wParam, const LPARAM& lParam);

void IFClickSetTempo(const GameButton* obj, const HWND& hWnd, const UINT& iMessage, const WPARAM& wParam, const LPARAM& lParam);
void IFClickSetVariable(const GameButton* obj, const HWND& hWnd, const UINT& iMessage, const WPARAM& wParam, const LPARAM& lParam);
void IFClickHelp(const GameButton* obj, const HWND& hWnd, const UINT& iMessage, const WPARAM& wParam, const LPARAM& lParam);
void IFClickPause(const GameButton* obj, const HWND& hWnd, const UINT& iMessage, const WPARAM& wParam, const LPARAM& lParam);

void IFClickLoadLevel(const GameButton* obj, const HWND& hWnd, const UINT& iMessage, const WPARAM& wParam, const LPARAM& lParam);
void IFClickSaveLevel(const GameButton* obj, const HWND& hWnd, const UINT& iMessage, const WPARAM& wParam, const LPARAM& lParam);
void IFClickLoadMusic(const GameButton* obj, const HWND& hWnd, const UINT& iMessage, const WPARAM& wParam, const LPARAM& lParam);

INT_PTR CALLBACK SetTempo_Dlalog_Proc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam);
INT_PTR CALLBACK SetVariable_Dlalog_Proc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam);
INT_PTR CALLBACK Help_Dlalog_Proc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam);