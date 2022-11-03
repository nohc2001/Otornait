#include "EditStation.h"

GameUI* GameUI::UIArr[GAMEUI_MAX] = {};
int GameUI::GameUIUpdate = 0;

shp::vec2f GameUI::MousePos = shp::vec2f(0, 0);
bool GameUI::LBtnPressed = false;
bool GameUI::RBtnPressed = false;

bool EditStation::SetTempoDialogOpen = false;
int EditStation::STD_Out_SetTempo = 130;
bool EditStation::STD_OUT_enable = false;

bool EditStation::SetVariableDialogOpen = false;
int EditStation::SVD_Out_SetTime = 0;
int EditStation::SVD_Out_SetMaxShowTime = 1000;
bool EditStation::SVD_OUT_enable = false;

bool EditStation::HelpDialogOpen = false;

bool EditStation::PauseBtn_ModePlay = true;
bool EditStation::PauseBtn_Enable = false;
shp::vec2f EditStation::PauseBtn_Flow = shp::vec2f(0, 0.2f);

ROTPOS EditStation::PresentROTPOS = ROTPOS::bottom;

bool EditStation::LoadLevelBtn_Enable = false;
bool EditStation::LoadLevelBtn_Load = false;
char EditStation::LoadLevelBtn_FileName[128] = {};

bool EditStation::SaveLevelBtn_Enable = false;
bool EditStation::SaveLevelBtn_Load = false;
char EditStation::SaveLevelBtn_FileName[128] = {};

bool EditStation::LoadMusicBtn_Enable = false;
bool EditStation::LoadMusicBtn_Load = false;
char EditStation::LoadMusicBtn_FileName[128] = {};

SHOW_TYPE EditStation::SelectedShowType = SHOW_TYPE::None;
float EditStation::Parameter[10] = {};
bool EditStation::isAddShow = false;

extern HINSTANCE g_hInst;
extern vector<Sprite*> SpriteData;

EditStation::EditStation()
{
	SetObjType(OBJ_TYPE::EditStation);
}

EditStation::~EditStation()
{
	for (int i = 0; i < 4; ++i) {
		if (HeapDebugClass::HeapDebug[RotPosButton[i]] == true) {
			HeapDebugClass::HeapDelete<GameButton>(RotPosButton[i]);
		}
	}

	if (HeapDebugClass::HeapDebug[SetTempoButton] == true) {
		HeapDebugClass::HeapDelete<GameButton>(SetTempoButton);
	}

	if (HeapDebugClass::HeapDebug[SetVariableButton] == true) {
		HeapDebugClass::HeapDelete<GameButton>(SetVariableButton);
	}

	if (HeapDebugClass::HeapDebug[HelpButton] == true) {
		HeapDebugClass::HeapDelete<GameButton>(HelpButton);
	}

	if (HeapDebugClass::HeapDebug[PauseButton] == true) {
		HeapDebugClass::HeapDelete<GameButton>(PauseButton);
	}

	if (HeapDebugClass::HeapDebug[LevelLoadButton] == true) {
		HeapDebugClass::HeapDelete<GameButton>(LevelLoadButton);
	}

	if (HeapDebugClass::HeapDebug[LevelSaveButton] == true) {
		HeapDebugClass::HeapDelete<GameButton>(LevelSaveButton);
	}

	if (HeapDebugClass::HeapDebug[LoadMusicButton] == true) {
		HeapDebugClass::HeapDelete<GameButton>(LoadMusicButton);
	}
}

void EditStation::SetTempo(const int& t)
{
	if (enable) {
		Tempo = t;
	}
}

const int& EditStation::GetTempo() const
{
	if (enable) {
		return Tempo;
	}
	else {
		return 130;
	}
}

void EditStation::SetTime(const float& t)
{
	if (enable) {
		if (t >= 0) {
			Time = t;
		}
		else {
			Time = 0;
		}
	}
}

const float& EditStation::GetTime() const
{
	if (enable) {
		return Time;
	}
	else {
		return 0;
	}
}

void EditStation::SetMaxShowTime(const float& t) {
	if (enable) {
		MaxShowTime = t;
	}
}

const float& EditStation::GetMaxShowTime() const {
	if (enable) {
		return MaxShowTime;
	}
	else {
		return 3.0f;
	}
}

void EditStation::SetEditRT(const shp::rect4f& ert) {
	if (enable) {
		EditRt = ert;
	}
}

const shp::rect4f& EditStation::GetEditRT() const {
	if (enable) {
		return EditRt;
	}
	else {
		return shp::rect4f(0, 0, 0, 0);
	}
}

void EditStation::Arrange() {
	for (int i = 0; i < EditNote.size(); ++i) {
		for (int k = i + 1; k < EditNote.size(); ++k) {
			if (EditNote[i].time > EditNote[k].time) {
				Note ins = EditNote[i];
				EditNote[i] = EditNote[k];
				EditNote[k] = ins;
			}
		}
	}

	for (int i = 0; i < EditShow.size(); ++i) {
		for (int k = i + 1; k < EditShow.size(); ++k) {
			if (EditShow[i].time > EditShow[k].time) {
				Show ins = EditShow[i];
				EditShow[i] = EditShow[k];
				EditShow[k] = ins;
			}
		}
	}
}

int EditStation::FindNote(float time, int pos)
{
	int n = EditNote.size() / 2;
	int delta = EditNote.size() / 4;

	if (EditNote.size() == 0) {
		return -1;
	}

	for (int i = 0; i < EditNote.size(); ++i) {
		if (fabsf(EditNote[i].time - time) < 0.01f && EditNote[i].pos == pos) {
			return i;
		}
	}
	return -1;
}

const bool& EditStation::isNoteExist(float time, int pos) {
	int n = EditNote.size() / 2;
	int delta = EditNote.size() / 4;

	if (EditNote.size() == 0) {
		return false;
	}

	for (int i = 0; i < EditNote.size(); ++i) {
		if (fabsf(EditNote[i].time - time) < 0.01f && EditNote[i].pos == pos) {
			return true;
		}
	}
	return false;
}

void EditStation::LoadData(const char* str)
{
	if (enable && playStation != nullptr) {
		playStation->LoadData(str);
		
		EditNote.clear();
		EditShow.clear();

		int max = playStation->GetMaxNoteNum();
		for (int i = 0; i < max; ++i) {
			EditNote.push_back(playStation->GetNote(i));
		}

		int smax = playStation->GetMaxShowNum();
		for (int i = 0; i < smax; ++i) {
			EditShow.push_back(playStation->GetShow(i));
		}
	}
}

void EditStation::SaveData(const char* str)
{
	if (enable) {
		playStation->SaveData(str);
	}
}

int EditStation::FindShow(float time)
{
	int n = EditShow.size() / 2;
	int delta = EditShow.size() / 4;

	if (EditShow.size() == 0) {
		return -1;
	}

	for (int i = 0; i < EditShow.size(); ++i) {
		if (fabsf(EditShow[i].time - time) < 0.01f) {
			return i;
		}
	}
	return -1;
}

EditStation* EditStation::Init(const shp::rect4f& loc, const int& layer)
{
	SetLocation(loc);
	SetLayer(layer);
	return this;
}

void EditStation::FirstInit()
{
	if (first && gm != nullptr) {
		shp::rect4f loc = GetLocation();
		int layer = GetLayer();

		shp::rect4f psrt = shp::rect4f(loc.getCenter().x, loc.fx, loc.lx, loc.getCenter().y);
		playStation = HeapDebugClass::HeapNew<PlayStation>()->Init(psrt, true, layer);
		playStation->SetTime(0);

		GameManager* GM = (GameManager*)gm;
		GM->AddObject((GameObject*)playStation);

		shp::rect4f rpbtn[4] = {};
		float w = psrt.getw() / 4;
		for (int i = 0; i < 4; ++i) {
			rpbtn[i] = shp::rect4f(psrt.fx + i * w, psrt.ly, psrt.fx + (i + 1) * w, psrt.ly + 70);
			RotPosButton[i] = HeapDebugClass::HeapNew<GameButton>();
			RotPosButton[i]->location = rpbtn[i];
			switch (i) {
			case 0:
				RotPosButton[i]->SetSTR(L"GoBottom");
				RotPosButton[i]->OnClick = IFClickBottom;
				break;
			case 1:
				RotPosButton[i]->SetSTR(L"GoLeft");
				RotPosButton[i]->OnClick = IFClickLeft;
				break;
			case 2:
				RotPosButton[i]->SetSTR(L"GoTop");
				RotPosButton[i]->OnClick = IFClickTop;
				break;
			case 3:
				RotPosButton[i]->SetSTR(L"GoRight");
				RotPosButton[i]->OnClick = IFClickRight;
				break;
			}
		}

		shp::rect4f stbtn = shp::rect4f(psrt.fx, psrt.ly + 70, psrt.fx + w, psrt.ly + 140);

		SetTempoButton = HeapDebugClass::HeapNew<GameButton>();
		SetTempoButton->location = stbtn;
		SetTempoButton->SetSTR(L"SetTempo");
		SetTempoButton->OnClick = IFClickSetTempo;

		stbtn = shp::rect4f(psrt.fx + w, psrt.ly + 70, psrt.fx + 2 * w, psrt.ly + 140);
		SetVariableButton = HeapDebugClass::HeapNew<GameButton>();
		SetVariableButton->location = stbtn;
		SetVariableButton->SetSTR(L"SetVariable");
		SetVariableButton->OnClick = IFClickSetVariable;

		stbtn = shp::rect4f(psrt.fx + 2 * w, psrt.ly + 70, psrt.fx + 3 * w, psrt.ly + 140);
		HelpButton = HeapDebugClass::HeapNew<GameButton>();
		HelpButton->location = stbtn;
		HelpButton->SetSTR(L"Help");
		HelpButton->OnClick = IFClickHelp;

		stbtn = shp::rect4f(psrt.fx + 3 * w, psrt.ly + 70, psrt.fx + 4 * w, psrt.ly + 140);
		PauseButton = HeapDebugClass::HeapNew<GameButton>();
		PauseButton->location = stbtn;
		PauseButton->SetSTR(L"Stop");
		PauseButton->OnClick = IFClickPause;

		stbtn = shp::rect4f(psrt.fx, psrt.ly + 140, psrt.fx + w, psrt.ly + 210);
		LevelLoadButton = HeapDebugClass::HeapNew<GameButton>();
		LevelLoadButton->location = stbtn;
		LevelLoadButton->SetSTR(L"Load Level");
		LevelLoadButton->OnClick = IFClickLoadLevel;

		stbtn = shp::rect4f(psrt.fx + w, psrt.ly + 140, psrt.fx + 2*w, psrt.ly + 210);
		LevelSaveButton = HeapDebugClass::HeapNew<GameButton>();
		LevelSaveButton->location = stbtn;
		LevelSaveButton->SetSTR(L"Save Level");
		LevelSaveButton->OnClick = IFClickSaveLevel;

		stbtn = shp::rect4f(psrt.fx + 2*w, psrt.ly + 140, psrt.fx + 3*w, psrt.ly + 210);
		LoadMusicButton = HeapDebugClass::HeapNew<GameButton>();
		LoadMusicButton->location = stbtn;
		LoadMusicButton->SetSTR(L"Load Music");
		LoadMusicButton->OnClick = IFClickLoadMusic;

		//EditRT
		SetEditRT(shp::rect4f(loc.fx, loc.fy, loc.getCenter().x, loc.ly));

		first = false;
	}
}

void EditStation::Update(const float& delta)
{
	FirstInit();

	if (first == false) {
		GetDialogData(delta);
		playStation->SetIsPlaying(bPlay);
		if (bPlay) {
			SetTime(GetTime() + delta);
		}
	}
	
	if (enable && first == false) {

		for (int i = 0; i < 4; ++i) {
			RotPosButton[i]->Update(delta);
		}

		SetTempoButton->Update(delta);
		SetVariableButton->Update(delta);
		HelpButton->Update(delta);
		PauseButton->Update(delta);

		LevelLoadButton->Update(delta);
		LevelSaveButton->Update(delta);
		LoadMusicButton->Update(delta);
	}
}

void EditStation::Event(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	if (enable && first == false) {
		for (int i = 0; i < 4; ++i) {
			RotPosButton[i]->Event(hWnd, iMessage, wParam, lParam);
		}

		SetTempoButton->Event(hWnd, iMessage, wParam, lParam);
		SetVariableButton->Event(hWnd, iMessage, wParam, lParam);
		HelpButton->Event(hWnd, iMessage, wParam, lParam);
		PauseButton->Event(hWnd, iMessage, wParam, lParam);

		LevelLoadButton->Event(hWnd, iMessage, wParam, lParam);
		LevelSaveButton->Event(hWnd, iMessage, wParam, lParam);
		LoadMusicButton->Event(hWnd, iMessage, wParam, lParam);

		if (iMessage == WM_KEYDOWN) {
			if (wParam == VK_NUMPAD1 || wParam == 49) {
				AddNoteType = NOTE_TYPE::NormalNote;
			}

			if (wParam == VK_NUMPAD2 || wParam == 50) {
				AddNoteType = NOTE_TYPE::LongNote;
			}

			if (wParam == VK_NUMPAD3 || wParam == 51) {
				AddNoteType = NOTE_TYPE::SpaceLeftNote;
			}

			if (wParam == VK_NUMPAD4 || wParam == 52) {
				AddNoteType = NOTE_TYPE::SpaceNote;
			}

			if (wParam == VK_NUMPAD5 || wParam == 53) {
				AddNoteType = NOTE_TYPE::SpaceRightNote;
			}

			if (wParam == VK_NUMPAD6 || wParam == 54) {
				AddNoteType = NOTE_TYPE::None; // 이럴때는 선택하는 모드
			}

			float tb = 60.0f / (GetTempo() * 16.0f);
			if (wParam == VK_UP) {
				if (isAddShow == false) {
					EditNote[NoteSelected].endtime += tb;
					playStation->LoadNotes(EditNote);
				}
			}

			if (wParam == VK_DOWN) {
				if (isAddShow == false) {
					EditNote[NoteSelected].endtime -= tb;
					playStation->LoadNotes(EditNote);
				}
			}
		}
		if (iMessage == WM_MOUSEWHEEL) {
			((short)HIWORD(wParam) < 0) ? SetTime(GetTime() - 0.2f) : SetTime(GetTime() + 0.2f);
		}

		//note add
		float beatBottom = GetLocation().ly - 100;
		int b1 = GetTime() * GetTempo() / 60;
		float bitdur = 60.0f / (float)GetTempo();
		shp::rect4f ert = GetEditRT();

		float tempo = GetTempo();
		int n = 2 + MaxShowTime / bitdur;
		for (int i = 0; i < 2 * n; ++i) {
			float nextBeat = (b1 + i) * 60 / tempo;
			float nextBeat2 = (b1 + i + 1) * 60 / tempo;
			float displayPosy = beatBottom - GetLocation().geth() * (nextBeat - GetTime()) / MaxShowTime;

			for (int k = 0; k < 4; ++k) {
				float ftimeb = (nextBeat * (float)(4 - k) + nextBeat2 * (float)(k)) / 4.0f;
				float ftimenextb = (nextBeat * (float)(4 - (k + 1)) + nextBeat2 * (float)(k + 1)) / 4.0f;
				float dpyb = beatBottom - GetLocation().geth() * (ftimeb - GetTime()) / MaxShowTime;

				for (int l = 0; l < 4; ++l) {
					float ftime = (ftimeb * (float)(4 - l) + ftimenextb * (float)(l)) / 4.0f;
					float ftimenext = (ftimeb * (float)(4 - (l + 1)) + ftimenextb * (float)(l + 1)) / 4.0f;
					float dpy = beatBottom - GetLocation().geth() * (ftime - GetTime()) / MaxShowTime;

					float dw = GetLocation().geth() * bitdur / 2 * MaxShowTime;
					const float conw = 5;
					const float delw = 15;
					for (int u = 0; u < 5; ++u) {
						shp::rect4f prt = shp::rect4f(ert.fx + u * ert.getw() / 5, dpy - conw, ert.fx + (u + 1) * ert.getw() / 5, dpy + conw);
						shp::rect4f dprt = shp::rect4f(ert.fx + u * ert.getw() / 5, dpy - delw, ert.fx + (u + 1) * ert.getw() / 5, dpy + delw);
						if (u == 2) {
							if (shp::bPointInRectRange(GameUI::MousePos, prt) && GameUI::LBtnPressed) {
								if (isAddShow == false) {
									if (isNoteExist(ftime, 4) == true && AddNoteType == NOTE_TYPE::None) {
										NoteSelected = FindNote(ftime, 4);
									}
								}
								else {
									//show select
									ShowSelected = FindShow(ftime);
								}
							}

							if (shp::bPointInRectRange(GameUI::MousePos, prt) && GameUI::LBtnPressed) {
								if (isAddShow == false) {
									if (isNoteExist(ftime, 4) == false && ((int)AddNoteType > 2 && AddNoteType != NOTE_TYPE::None)) {
										Note note;
										note.enable = true;
										note.time = ftime;
										note.endtime = ftime;
										note.ntype = AddNoteType;
										note.rotPos = PresentROTPOS;
										note.pos = 4;

										EditNote.push_back(note);
										Arrange();
										playStation->LoadNotes(EditNote);
									}
								}
								else {
									//add show
									Show show;
									show.autoShow = true;
									show.time = ftime;
									show.stype = EditStation::SelectedShowType;
									for (int pi = 0; pi < 3; ++pi) {
										show.param[pi] = EditStation::Parameter[pi];
									}
									EditShow.push_back(show);
									Arrange();
									playStation->LoadShows(EditShow);
								}
								GameUI::LBtnPressed = false;
							}

							if (shp::bPointInRectRange(GameUI::MousePos, dprt) && GameUI::RBtnPressed) {
								if (isAddShow == false) {
									if (isNoteExist(ftime, 4) == true && ((int)AddNoteType > 2 && AddNoteType != NOTE_TYPE::None)) {
										int index = FindNote(ftime, 4);
										EditNote.erase(EditNote.begin() + index);
										Arrange();
										playStation->LoadNotes(EditNote);
									}
								}
								else {
									//erase show
									int index = FindShow(ftime);
									if (index >= 0 && index < EditShow.size()) {
										EditShow.erase(EditShow.begin() + index);
										Arrange();
										playStation->LoadShows(EditShow);
									}
								}
							}

							
						}
						else {
							int du = u;
							if (du > 2) {
								du -= 1;
							}

							if (shp::bPointInRectRange(GameUI::MousePos, prt) && GameUI::LBtnPressed) {
								if (isAddShow == false) {
									if (isNoteExist(ftime, du) == true && AddNoteType == NOTE_TYPE::None) {
										NoteSelected = FindNote(ftime, du);
									}
								}
								else {
									//show select
									ShowSelected = FindShow(ftime);
								}
								
							}

							if (shp::bPointInRectRange(GameUI::MousePos, prt) && GameUI::LBtnPressed) {
								if (isAddShow == false) {
									if (isNoteExist(ftime, du) == false && AddNoteType < NOTE_TYPE::SpaceLeftNote) {
										Note note;
										note.enable = true;
										note.time = ftime;
										note.endtime = ftime + 0.3f;
										note.ntype = AddNoteType;
										note.rotPos = PresentROTPOS;
										note.pos = du;

										EditNote.push_back(note);
										Arrange();
										playStation->LoadNotes(EditNote);
									}
								}
								else {
									Show show;
									show.autoShow = true;
									show.time = ftime;
									show.stype = EditStation::SelectedShowType;
									for (int pi = 0; pi < 3; ++pi) {
										show.param[pi] = EditStation::Parameter[pi];
									}
									EditShow.push_back(show);
									Arrange();
									playStation->LoadShows(EditShow);
								}
								
								GameUI::LBtnPressed = false;
							}

							if (shp::bPointInRectRange(GameUI::MousePos, dprt) && GameUI::RBtnPressed) {
								if (isAddShow == false) {
									if (isNoteExist(ftime, du) == true && AddNoteType != NOTE_TYPE::None) {
										int index = FindNote(ftime, du);
										if (index != -1) {
											EditNote.erase(EditNote.begin() + index);
											Arrange();
											playStation->LoadNotes(EditNote);
										}
									}
								}
								else {
									//erase show
									int index = FindShow(ftime);
									if (index >= 0 && index < EditShow.size()) {
										EditShow.erase(EditShow.begin() + index);
										Arrange();
										playStation->LoadShows(EditShow);
									}
								}
							}
						}
					}
				}
			}
		}
	}
}

void EditStation::Render(HDC hdc)
{
	if (enable && first == false) {
		HPEN LargePen = CreatePen(PS_SOLID, 8, RGB(0, 255, 255));
		HPEN BigPen = CreatePen(PS_SOLID, 5, RGB(0, 0, 0));
		HPEN REDPen = CreatePen(PS_SOLID, 5, RGB(128, 0, 0));
		HPEN SmallPen = CreatePen(PS_SOLID, 2, RGB(0, 0, 0));
		for (int i = 0; i < 4; ++i) {
			RotPosButton[i]->RenderObject(hdc);
		}

		SetTempoButton->RenderObject(hdc);
		SetVariableButton->RenderObject(hdc);
		HelpButton->RenderObject(hdc);
		PauseButton->RenderObject(hdc);

		LevelLoadButton->RenderObject(hdc);
		LevelSaveButton->RenderObject(hdc);
		LoadMusicButton->RenderObject(hdc);

		float beatBottom = GetLocation().ly - 100;
		int b1 = GetTime() * GetTempo() / 60;
		float bitdur = 60.0f / (float)GetTempo();

		TCHAR datastr[128] = {};
		TCHAR rotposstr[10] = {};
		switch(PresentROTPOS) {
		case ROTPOS::bottom:
			wcscpy_s(rotposstr, L"bottom");
			break;
		case ROTPOS::left:
			wcscpy_s(rotposstr, L"left");
			break;
		case ROTPOS::top:
			wcscpy_s(rotposstr, L"top");
			break;
		case ROTPOS::right:
			wcscpy_s(rotposstr, L"right");
			break;
		}
		swprintf_s(datastr, L"Tempo : %d / Time : %g / Beat : %d / RotPos : %s", GetTempo(), GetTime(), b1, rotposstr);
		shp::rect4f loc = GetLocation();
		RECT locrt = { loc.fx, loc.fy, loc.lx, loc.ly };
		DrawText(hdc, datastr, lstrlen(datastr), &locrt, DT_LEFT);

		shp::rect4f ert = GetEditRT();
		float w = ert.getw() / 4;
		for (int i = 1; i < 5; ++i) {
			MoveToEx(hdc, ert.fx + w * i, ert.fy, NULL);
			LineTo(hdc, ert.fx + w * i, ert.ly);
		}

		const float conw = 5;
		float tempo = GetTempo();
		int n = 2 + MaxShowTime / bitdur;
		for (int i = 0; i < 2*n; ++i) {
			float nextBeat = (b1 + i) * 60 / tempo;
			float nextBeat2 = (b1 + i+1) * 60 / tempo;
			float displayPosy = beatBottom - GetLocation().geth() * (nextBeat - GetTime()) / MaxShowTime;
			SelectObject(hdc, LargePen);
			MoveToEx(hdc, ert.fx, displayPosy, NULL);
			LineTo(hdc, ert.lx, displayPosy);

			
			for (int k = 0; k < 4; ++k) {
				float ftimeb = (nextBeat * (float)(4-k) + nextBeat2 * (float)(k)) / 4.0f;
				float ftimenextb = (nextBeat * (float)(4 - (k+1)) + nextBeat2 * (float)(k+1)) / 4.0f;
				float dpyb = beatBottom - GetLocation().geth() * (ftimeb - GetTime()) / MaxShowTime;
				SelectObject(hdc, BigPen);
				MoveToEx(hdc, ert.fx, dpyb, NULL);
				LineTo(hdc, ert.lx, dpyb);

				for (int l = 0; l < 4; ++l) {
					float ftime = (ftimeb * (float)(4 - l) + ftimenextb * (float)(l)) / 4.0f;
					float ftimenext = (ftimeb * (float)(4 - (l + 1)) + ftimenextb * (float)(l + 1)) / 4.0f;
					float dpy = beatBottom - GetLocation().geth() * (ftime - GetTime()) / MaxShowTime;
					SelectObject(hdc, SmallPen);
					MoveToEx(hdc, ert.fx, dpy, NULL);
					LineTo(hdc, ert.lx, dpy);

					float dw = GetLocation().geth() * bitdur / 2 * MaxShowTime;
					
					for (int u = 0; u < 5; ++u) {
						shp::rect4f prt = shp::rect4f(ert.fx + u * ert.getw() / 5, dpy - conw, ert.fx + (u + 1) * ert.getw() / 5, dpy + conw);
						if (u == 2) {
							shp::rect4f pdrt = shp::rect4f(ert.fx, dpy - conw, ert.lx, dpy + conw);
							if (shp::bPointInRectRange(GameUI::MousePos, prt)) {
								Rectangle(hdc, pdrt.fx, pdrt.fy, pdrt.lx, pdrt.ly);
							}
						}
						else {
							int du = u;
							if (du > 2) {
								du -= 1;
							}
							shp::rect4f pdrt = shp::rect4f(ert.fx + du * ert.getw() / 4, dpy - conw, ert.fx + (du + 1) * ert.getw() / 4, dpy + conw);
							if (shp::bPointInRectRange(GameUI::MousePos, prt)) {
								Rectangle(hdc, pdrt.fx, pdrt.fy, pdrt.lx, pdrt.ly);
							}
						}
					}
				}
			}
		}

		float t = GetTime();
		const float dconw = 10;
		for (int i = 0; i < EditNote.size(); ++i) {
			bool bshow = false;
			bshow = (t < EditNote[i].time || EditNote[i].endtime < t + MaxShowTime) ||
				((t > EditNote[i].time && EditNote[i].endtime > t + MaxShowTime));

			if ( bshow && (EditNote[i].rotPos == PresentROTPOS)) {
				float dpy = beatBottom - GetLocation().geth() * (EditNote[i].time - GetTime()) / MaxShowTime;
				float dpye = beatBottom - GetLocation().geth() * (EditNote[i].endtime - GetTime()) / MaxShowTime;
				Sprite* nspr;
				shp::rect4f cen;
				if (EditNote[i].ntype == NOTE_TYPE::NormalNote) {
					cen = shp::rect4f(ert.fx + EditNote[i].pos * w, dpy - dconw, ert.fx + (EditNote[i].pos+1) * w, dpy + dconw);
					nspr = SpriteData[0];
					nspr->DrawSprite(hdc, cen.fx, cen.fy, cen.getw(), cen.geth());
				}
				else if (EditNote[i].ntype == NOTE_TYPE::LongNote) {
					cen = shp::rect4f(ert.fx + EditNote[i].pos * w, dpye, ert.fx + (EditNote[i].pos + 1) * w, dpy);
					HBRUSH longB = CreateSolidBrush(RGB(128, 128, 0));
					SelectObject(hdc, longB);
					Rectangle(hdc, cen.fx, cen.fy, cen.lx, cen.ly);
					DeleteObject(longB);
					SelectObject(hdc, whiteB);
				}
				else if (EditNote[i].ntype == NOTE_TYPE::SpaceNote) {
					cen = shp::rect4f(ert.fx, dpy - dconw, ert.fx + ert.lx, dpy + dconw);
					nspr = SpriteData[2];
					nspr->DrawSprite(hdc, cen.fx, cen.fy, cen.getw(), cen.geth());
				}
				else if (EditNote[i].ntype == NOTE_TYPE::SpaceLeftNote) {
					cen = shp::rect4f(ert.fx, dpy - dconw, ert.fx + ert.lx, dpy + dconw);
					nspr = SpriteData[3];
					nspr->DrawSprite(hdc, cen.fx, cen.fy, cen.getw(), cen.geth());
				}
				else if (EditNote[i].ntype == NOTE_TYPE::SpaceRightNote) {
					cen = shp::rect4f(ert.fx, dpy - dconw, ert.fx + ert.lx, dpy + dconw);
					nspr = SpriteData[4];
					nspr->DrawSprite(hdc, cen.fx, cen.fy, cen.getw(), cen.geth());
				}
			}
		}

		for (int i = 0; i < EditShow.size(); ++i) {
			bool bshow = false;
			bshow = (t < EditShow[i].time && EditShow[i].time < t + MaxShowTime);

			if (bshow) {
				float dpy = beatBottom - GetLocation().geth() * (EditShow[i].time - GetTime()) / MaxShowTime;
				float dpye = beatBottom - GetLocation().geth() * (EditShow[i].time - GetTime()) / MaxShowTime;
				shp::rect4f cen = shp::rect4f(ert.fx, dpy - dconw, ert.lx, dpy + dconw);
				SpriteData[36]->DrawSprite(hdc, cen.fx, cen.fy, cen.getw(), cen.geth());
			}
		}

		SelectObject(hdc, REDPen);
		MoveToEx(hdc, ert.fx, beatBottom, NULL);
		LineTo(hdc, ert.lx, beatBottom);

		DeleteObject(LargePen);
		DeleteObject(BigPen);
		DeleteObject(REDPen);
		DeleteObject(SmallPen);
	}
}

void EditStation::GetDialogData(float delta)
{
	if (SVD_OUT_enable) {
		SVD_OUT_enable = false;
		if (SVD_Out_SetTime >= 0) {
			SetTime(SVD_Out_SetTime);
			playStation->GoPosition(SVD_Out_SetTime);
		}

		float mst = SVD_Out_SetMaxShowTime / 1000.0f;
		SetMaxShowTime(mst);
	}

	if (STD_OUT_enable) {
		STD_OUT_enable = false;
		SetTempo(STD_Out_SetTempo);
	}

	if (PauseBtn_Enable) {
		bPlay = PauseBtn_ModePlay;
		if (bPlay) {
			PauseButton->SetSTR(L"Stop");
			playStation->GoPosition(GetTime());
			Music::Play(0, true);
		}
		else {
			PauseButton->SetSTR(L"Play");
			Music::Play(0, false);
		}

		PauseBtn_Enable = false;
	}

	if (PauseBtn_Flow.y > PauseBtn_Flow.x) {
		PauseBtn_Flow.x += delta;
	}
	
	if (LoadLevelBtn_Load) {
		LoadLevelBtn_Load = false;
		LoadData(LoadLevelBtn_FileName);
	}

	if (SaveLevelBtn_Load) {
		SaveLevelBtn_Load = false;
		SaveData(SaveLevelBtn_FileName);
	}

	if (LoadMusicBtn_Load) {
		LoadMusicBtn_Load = false;

		playStation->LoadMusic(LoadMusicBtn_FileName);
	}
}

INT_PTR CALLBACK SetTempo_Dlalog_Proc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	switch (iMsg) {
	case WM_COMMAND:
		switch (LOWORD(wParam)) {
		case IDOK: //--- 버튼
		{
			BOOL SUCCESS = TRUE;
			int n = GetDlgItemInt(hDlg, IDC_EDIT_TEMPO, &SUCCESS, FALSE);
			if (SUCCESS == TRUE) {
				EditStation::STD_Out_SetTempo = n;
				EditStation::STD_OUT_enable = true;
			}
			SendMessage(hDlg, WM_CLOSE, 0, 0);
			break;
		}
		case IDCANCEL: //--- 버튼
			SendMessage(hDlg, WM_CLOSE, 0, 0);
			break;
		}
		break;
	case WM_CLOSE:
		DestroyWindow(hDlg);
		EditStation::SetTempoDialogOpen = false;
		break;
	}
	return 0;
}

INT_PTR CALLBACK SetVariable_Dlalog_Proc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	switch (iMsg) {
	case WM_INITDIALOG:
	{
		
		switch (EditStation::SelectedShowType) {
		case SHOW_TYPE::None:
		{
			SetDlgItemText(hDlg, IDC_PRESENT_SHOW_STATIC, L"None");
			SetDlgItemText(hDlg, IDC_P0STATIC, L"none");
			SetDlgItemText(hDlg, IDC_P1STATIC, L"none");
			SetDlgItemText(hDlg, IDC_P2STATIC, L"none");
		}
			break;
		case SHOW_TYPE::EdgeSpreadEF:
		{
			SetDlgItemText(hDlg, IDC_PRESENT_SHOW_STATIC, L"EdgeSpreadEF");
			SetDlgItemText(hDlg, IDC_P0STATIC, L"rpos - bottom(0), left(1), top(2), right(3)");
			SetDlgItemText(hDlg, IDC_P1STATIC, L"power");
			SetDlgItemText(hDlg, IDC_P2STATIC, L"none");
		}
			break;
		case SHOW_TYPE::LineDiamondRotateEF:
		{
			SetDlgItemText(hDlg, IDC_PRESENT_SHOW_STATIC, L"LineDiamondRotateEF");
			SetDlgItemText(hDlg, IDC_P0STATIC, L"dir - clockDir(0), inverseClockDir(1)");
			SetDlgItemText(hDlg, IDC_P1STATIC, L"power");
			SetDlgItemText(hDlg, IDC_P2STATIC, L"none");
		}
			break;
		case SHOW_TYPE::ExplosionEF:
		{
			SetDlgItemText(hDlg, IDC_PRESENT_SHOW_STATIC, L"ExplosionEF");
			SetDlgItemText(hDlg, IDC_P0STATIC, L"x cordinate");
			SetDlgItemText(hDlg, IDC_P1STATIC, L"y cordinate");
			SetDlgItemText(hDlg, IDC_P2STATIC, L"power");
		}
			break;
		case SHOW_TYPE::FlashEF:
		{
			SetDlgItemText(hDlg, IDC_PRESENT_SHOW_STATIC, L"FlashEF");
			SetDlgItemText(hDlg, IDC_P0STATIC, L"maxtime");
			SetDlgItemText(hDlg, IDC_P1STATIC, L"none");
			SetDlgItemText(hDlg, IDC_P2STATIC, L"none");
		}
			break;
		case SHOW_TYPE::LineDirEF:
		{
			SetDlgItemText(hDlg, IDC_PRESENT_SHOW_STATIC, L"LineDirEF");
			SetDlgItemText(hDlg, IDC_P0STATIC, L"dir - angle(float - max360)");
			SetDlgItemText(hDlg, IDC_P1STATIC, L"maxtime");
			SetDlgItemText(hDlg, IDC_P2STATIC, L"none");
		}
			break;
		}
		
	}
		break;
	case WM_COMMAND:
		switch (LOWORD(wParam)) {
		case IDOK: //--- 버튼
		{
			BOOL time_SUCCESS = TRUE;
			BOOL maxShowtime_SUCCESS = TRUE;
			BOOL SelectShow_SUCCESS = TRUE;
			BOOL Params_SUCCESS[10] = { TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE };
			BOOL ISADD_SUCCESE = TRUE;
			BOOL FSUC = TRUE;
			int data[10] = {};
			data[0] = GetDlgItemInt(hDlg, IDC_EDIT_TIME, &time_SUCCESS, FALSE);
			FSUC = (FSUC == TRUE || time_SUCCESS == TRUE) ? TRUE : FALSE;
			data[1] = GetDlgItemInt(hDlg, IDC_EDIT_MAXSHOWTIME, &maxShowtime_SUCCESS, FALSE);
			FSUC = (FSUC == TRUE || maxShowtime_SUCCESS == TRUE) ? TRUE : FALSE;

			data[2] = GetDlgItemInt(hDlg, IDC_EDIT_ADDINGSHOWTYPE, &SelectShow_SUCCESS, FALSE);
			FSUC = (FSUC == TRUE || SelectShow_SUCCESS == TRUE) ? TRUE : FALSE;
			data[3] = GetDlgItemInt(hDlg, IDC_EDIT_ISADD, &ISADD_SUCCESE, FALSE);
			FSUC = (FSUC == TRUE || ISADD_SUCCESE == TRUE) ? TRUE : FALSE;
			data[4] = GetDlgItemInt(hDlg, IDC_EDIT_PARAM0, &Params_SUCCESS[0], FALSE);
			FSUC = (FSUC == TRUE || Params_SUCCESS[0] == TRUE) ? TRUE : FALSE;
			data[5] = GetDlgItemInt(hDlg, IDC_EDIT_PARAM1, &Params_SUCCESS[1], FALSE);
			FSUC = (FSUC == TRUE || Params_SUCCESS[1] == TRUE) ? TRUE : FALSE;
			data[6] = GetDlgItemInt(hDlg, IDC_EDIT_PARAM2, &Params_SUCCESS[2], FALSE);
			FSUC = (FSUC == TRUE || Params_SUCCESS[2] == TRUE) ? TRUE : FALSE;

			if (FSUC == TRUE) {
				if (time_SUCCESS) {
					EditStation::SVD_Out_SetTime = data[0];
				}
				else {
					EditStation::SVD_Out_SetTime = -1;
				}
				
				if (maxShowtime_SUCCESS) {
					EditStation::SVD_Out_SetMaxShowTime = data[1];
				}

				if (SelectShow_SUCCESS) {
					EditStation::SelectedShowType = (SHOW_TYPE)data[2];
				}

				if (ISADD_SUCCESE) {
					if (data[3] > 0) {
						EditStation::isAddShow = true;
					}
					else {
						EditStation::isAddShow = false;
					}
				}

				if (Params_SUCCESS[0]) {
					EditStation::Parameter[0] = data[4];
				}

				if (Params_SUCCESS[1]) {
					EditStation::Parameter[1] = data[5];
				}

				if (Params_SUCCESS[2]) {
					EditStation::Parameter[2] = data[6];
				}
				
				EditStation::SVD_OUT_enable = true;
			}
			SendMessage(hDlg, WM_CLOSE, 0, 0);
			break;
		}
		case IDCANCEL: //--- 버튼
			SendMessage(hDlg, WM_CLOSE, 0, 0);
			break;
		}
		break;
	case WM_CLOSE:
		DestroyWindow(hDlg);
		EditStation::SetVariableDialogOpen = false;
		break;
	}
	return 0;
}

INT_PTR CALLBACK Help_Dlalog_Proc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	switch (iMsg) {
	case WM_COMMAND:
		switch (LOWORD(wParam)) {
		case IDOK: //--- 버튼
			SendMessage(hDlg, WM_CLOSE, 0, 0);
			break;
		case IDCANCEL: //--- 버튼
			SendMessage(hDlg, WM_CLOSE, 0, 0);
			break;
		}
		break;
	case WM_CLOSE:
		DestroyWindow(hDlg);
		EditStation::HelpDialogOpen = false;
		break;
	}
	return 0;
}

void IFClickBottom(const GameButton* obj, const HWND& hWnd, const UINT& iMessage, const WPARAM& wParam, const LPARAM& lParam) {
	EditStation::PresentROTPOS = ROTPOS::bottom;
}

void IFClickLeft(const GameButton* obj, const HWND& hWnd, const UINT& iMessage, const WPARAM& wParam, const LPARAM& lParam) {
	EditStation::PresentROTPOS = ROTPOS::left;
}

void IFClickTop(const GameButton* obj, const HWND& hWnd, const UINT& iMessage, const WPARAM& wParam, const LPARAM& lParam) {
	EditStation::PresentROTPOS = ROTPOS::top;
}

void IFClickRight(const GameButton* obj, const HWND& hWnd, const UINT& iMessage, const WPARAM& wParam, const LPARAM& lParam) {
	EditStation::PresentROTPOS = ROTPOS::right;
}

void IFClickSetTempo(const GameButton* obj, const HWND& hWnd, const UINT& iMessage, const WPARAM& wParam, const LPARAM& lParam)
{
	if (EditStation::SetTempoDialogOpen == false) {
		EditStation::SetTempoDialogOpen = true;
		DLGPROC func = SetTempo_Dlalog_Proc;
		HWND hDlg = CreateDialog(g_hInst, MAKEINTRESOURCE(IDD_DIALOG_SET_TEMPO), hWnd, func);
		ShowWindow(hDlg, SW_SHOW);
	}
}

void IFClickSetVariable(const GameButton* obj, const HWND& hWnd, const UINT& iMessage, const WPARAM& wParam, const LPARAM& lParam) {
	if (EditStation::SetVariableDialogOpen == false) {
		EditStation::SetVariableDialogOpen = true;
		DLGPROC func = SetVariable_Dlalog_Proc;
		HWND hDlg = CreateDialog(g_hInst, MAKEINTRESOURCE(IDD_DIALOG_SET_VAR), hWnd, func);

		BOOL SUCCESS = TRUE;
		ShowWindow(hDlg, SW_SHOW);
	}
}

void IFClickHelp(const GameButton* obj, const HWND& hWnd, const UINT& iMessage, const WPARAM& wParam, const LPARAM& lParam)
{
	if (EditStation::HelpDialogOpen == false) {
		EditStation::HelpDialogOpen = true;
		DLGPROC func = Help_Dlalog_Proc;
		HWND hDlg = CreateDialog(g_hInst, MAKEINTRESOURCE(IDD_DIALOG_HELPTEXT), hWnd, func);
		ShowWindow(hDlg, SW_SHOW);
	}
}

void IFClickPause(const GameButton* obj, const HWND& hWnd, const UINT& iMessage, const WPARAM& wParam, const LPARAM& lParam)
{
	if (EditStation::PauseBtn_Enable == false && EditStation::PauseBtn_Flow.y < EditStation::PauseBtn_Flow.x) {
		if (EditStation::PauseBtn_ModePlay) {
			EditStation::PauseBtn_ModePlay = false;
			EditStation::PauseBtn_Flow.x = 0;
		}
		else {
			EditStation::PauseBtn_ModePlay = true;
			EditStation::PauseBtn_Flow.x = 0;
		}

		EditStation::PauseBtn_Enable = true;

		GameUI::LBtnPressed = false;
	}
}

void IFClickLoadLevel(const GameButton* obj, const HWND& hWnd, const UINT& iMessage, const WPARAM& wParam, const LPARAM& lParam)
{
	if (EditStation::LoadLevelBtn_Enable == false) {
		EditStation::LoadLevelBtn_Enable = true;
		OPENFILENAME OFN;
		TCHAR filePathName[256] = L"";
		TCHAR lpstrFile[256] = L"";
		static TCHAR filter[] = L"모든 파일\0*.*\0텍스트 파일\0*.txt\0txt 파일\0*.txt";

		memset(&OFN, 0, sizeof(OPENFILENAME));
		OFN.lStructSize = sizeof(OPENFILENAME);
		OFN.hwndOwner = hWnd;
		OFN.lpstrFilter = filter;
		OFN.lpstrFile = lpstrFile;
		OFN.nMaxFile = 256;
		OFN.lpstrInitialDir = L".";

		if (GetOpenFileName(&OFN) != 0) {
			wsprintf(filePathName, L"%s 파일을 열겠습니까?", OFN.lpstrFile);
			MessageBox(hWnd, filePathName, L"열기 선택", MB_OK);

			wchar_t* str = OFN.lpstrFile;
			char* pStr;
			int strSize = WideCharToMultiByte(CP_ACP, 0, str, -1, NULL, 0, NULL, NULL);
			pStr = HeapDebugClass::HeapArrNew<char>(strSize);
			WideCharToMultiByte(CP_ACP, 0, str, -1, pStr, strSize, 0, 0);

			strcpy_s(EditStation::LoadLevelBtn_FileName, pStr);

			EditStation::LoadLevelBtn_Load = true;

			HeapDebugClass::HeapArrDelete<char>(pStr);
		}
		else {
			EditStation::LoadLevelBtn_Enable = false;
		}

		GameUI::LBtnPressed = false;
	}
	else {
		EditStation::LoadLevelBtn_Enable = false;
		GameUI::LBtnPressed = false;
	}
}

void IFClickSaveLevel(const GameButton* obj, const HWND& hWnd, const UINT& iMessage, const WPARAM& wParam, const LPARAM& lParam)
{
	if (EditStation::SaveLevelBtn_Enable == false) {
		EditStation::SaveLevelBtn_Enable = true;
		OPENFILENAME OFN;
		TCHAR filePathName[256] = L"";
		TCHAR lpstrFile[256] = L"";
		static TCHAR filter[] = L"모든 파일\0*.*\0텍스트 파일\0*.txt\0txt 파일\0*.txt";

		memset(&OFN, 0, sizeof(OPENFILENAME));
		OFN.lStructSize = sizeof(OPENFILENAME);
		OFN.hwndOwner = hWnd;
		OFN.lpstrFilter = filter;
		OFN.lpstrFile = lpstrFile;
		OFN.nMaxFile = 256;
		OFN.lpstrInitialDir = L".";

		if (GetSaveFileName(&OFN) != 0) {
			wsprintf(filePathName, L"%s 파일을 저장하겠습니까?", OFN.lpstrFile);
			MessageBox(hWnd, filePathName, L"저장 선택", MB_OK);

			wchar_t* str = OFN.lpstrFile;
			char* pStr;
			int strSize = WideCharToMultiByte(CP_ACP, 0, str, -1, NULL, 0, NULL, NULL);
			pStr = HeapDebugClass::HeapArrNew<char>(strSize);
			WideCharToMultiByte(CP_ACP, 0, str, -1, pStr, strSize, 0, 0);

			strcpy_s(EditStation::SaveLevelBtn_FileName, pStr);

			EditStation::SaveLevelBtn_Load = true;

			HeapDebugClass::HeapArrDelete<char>(pStr);
		}
		else {
			EditStation::SaveLevelBtn_Enable = false;
		}

		GameUI::LBtnPressed = false;
	}
	else {
		EditStation::SaveLevelBtn_Enable = false;
		GameUI::LBtnPressed = false;
	}
}

void IFClickLoadMusic(const GameButton* obj, const HWND& hWnd, const UINT& iMessage, const WPARAM& wParam, const LPARAM& lParam)
{
	if (EditStation::LoadMusicBtn_Enable == false) {
		TCHAR originPath[128] = {};
		GetCurrentDirectory(128, originPath);

		EditStation::LoadMusicBtn_Enable = true;
		OPENFILENAME OFN;
		TCHAR filePathName[200] = L"";
		TCHAR lpstrFile[200] = L"";
		static TCHAR filter[] = L"모든 파일\0*.*\0텍스트 파일\0*.txt\0txt 파일\0*.txt";

		memset(&OFN, 0, sizeof(OPENFILENAME));
		OFN.lStructSize = sizeof(OPENFILENAME);
		OFN.hwndOwner = hWnd;
		OFN.lpstrFilter = filter;
		OFN.lpstrFile = lpstrFile;
		OFN.nMaxFile = 200;
		OFN.lpstrInitialDir = L".";

		if (GetOpenFileName(&OFN) != 0) {
			wsprintf(filePathName, L"%s 파일을 열겠습니까?", OFN.lpstrFile);
			MessageBox(hWnd, filePathName, L"열기 선택", MB_OK);

			wchar_t* str = OFN.lpstrFile;
			char* pStr;
			int strSize = WideCharToMultiByte(CP_ACP, 0, str, -1, NULL, 0, NULL, NULL);
			pStr = HeapDebugClass::HeapArrNew<char>(strSize);
			WideCharToMultiByte(CP_ACP, 0, str, -1, pStr, strSize, 0, 0);

			char openpath[128] = {};
			char filename[128] = {};
			strcpy_s(openpath, pStr);
			_fullpath(filename, "", 128);

			int len = strlen(openpath);
			int flen = strlen(filename);
			char song[128] = "Sound";
			int k = 0;
			for (int i = flen; i < len; ++i) {
				k = 5 + i - flen;
				song[k] = openpath[i];
			}
			k += 1;
			song[k] = '\0';

			strcpy_s(EditStation::LoadMusicBtn_FileName, song);

			EditStation::LoadMusicBtn_Load = true;

			HeapDebugClass::HeapArrDelete<char>(pStr);
		}
		else {
			EditStation::LoadMusicBtn_Enable = false;
		}

		GameUI::LBtnPressed = false;

		SetCurrentDirectory(originPath);
	}
}
