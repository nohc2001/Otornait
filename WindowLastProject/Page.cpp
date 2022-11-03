#include "Page.h"

extern vector<Sprite*> SpriteData;
extern RECT rt;
LogoPage::LogoPage()
{
	SetSprite(SpriteData[15]);
}

LogoPage::~LogoPage()
{
}

LogoPage* LogoPage::Init(const shp::rect4f& loc, const int& layer)
{
	SetLocation(loc);
	SetLayer(layer);
	return this;
}

void LogoPage::FirstInit()
{
	if (enable) {
		if (first && gm != nullptr) {
			GameManager* GM = (GameManager*)gm;
			GM->AddObject((GameObject*)HeapDebugClass::HeapNew<FlowerEf>()->Init(GetLocation(), SpriteData[16], 2));

			int n = Music::AddSound("Sound\\LogoMusic.mp3", false, false);
			Music::ConnectSound(0, n);
			Music::PlayOnce(0);

			first = false;
		}
	}
}

void LogoPage::Update(const float& delta)
{
	if (enable) {
		FirstInit();

		if (flow.x > flow.y && wait) {
			//다음 페이지 넘어가기
			Music::ClearSoundsAndChannels();
			NextPage();
			SetEnable(false);
		}

		if (flow.x > flow.y) {
			wait = true;
			flow.x = 0;
		}
		else {
			flow.x += delta;
		}
	}
}

void LogoPage::Event(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
}

void LogoPage::Render(HDC hdc)
{
	if (enable) {
		Sprite* spr = GetSprite();
		if (spr != nullptr) {
			if (wait) {
				float w = 200;
				shp::vec2f cen = GetLocation().getCenter();
				spr->DrawSpriteAlphaBlend(hdc, cen.x - w, cen.y - w, 2 * w, 2 * w, 255);
			}
			else {
				float trate = AnimClass::EaseIn(flow.x / flow.y, 4);


				if (trate > 1) {
					trate = 1;
				}
				float w = 200;
				shp::vec2f cen = GetLocation().getCenter();
				spr->DrawSpriteAlphaBlend(hdc, cen.x - w, cen.y - w, 2 * w, 2 * w, 255 * (trate));
			}
		}
	}
}

void LogoPage::NextPage()
{
	if (enable) {
		GameManager* GM = (GameManager*)gm;
		//GM->AddObject((GameObject*)HeapDebugClass::HeapNew<EditStation>()->Init(shp::rect4f(rt.left, rt.top, rt.right, rt.bottom), 1));
		GM->AddObject((GameObject*)HeapDebugClass::HeapNew<TitlePage>()->Init(shp::rect4f(rt.left, rt.top, rt.right, rt.bottom), 1));
	}
}

/// <summary>
/// 
/// </summary>

TitlePage::TitlePage()
{
}

TitlePage::~TitlePage()
{
	if (HeapDebugClass::HeapDebug[parEF] == true) {
		HeapDebugClass::HeapDelete<ParrellexEf>(parEF);
	}

	if (HeapDebugClass::HeapDebug[GameStartButton] == true) {
		HeapDebugClass::HeapDelete<GameButton>(GameStartButton);
	}

	if (HeapDebugClass::HeapDebug[NextMenu] == true) {
		HeapDebugClass::HeapDelete<GameButton>(NextMenu);
	}

	if (HeapDebugClass::HeapDebug[PrevMenu] == true) {
		HeapDebugClass::HeapDelete<GameButton>(PrevMenu);
	}

	if (HeapDebugClass::HeapDebug[SelectBtn] == true) {
		HeapDebugClass::HeapDelete<GameButton>(SelectBtn);
	}

	if (HeapDebugClass::HeapDebug[parEF] == true) {
		HeapDebugClass::HeapDelete<ParrellexEf>(parEF);
	}
}

TitlePage* TitlePage::Init(const shp::rect4f& loc, const int& layer)
{
	SetLocation(loc);
	SetLayer(layer);
	return this;
}

void TitlePage::FirstInit()
{
	if (first && gm != nullptr) {
		int n = Music::AddSound("Sound\\MelDrum3_Master.mp3", true, true);
		Music::ConnectSound(0, n);
		Music::PlayOnce(0);

		shp::rect4f loc = GetLocation();
		shp::vec2f cen = loc.getCenter();
		float w = loc.getw() / 8;
		float h = loc.geth() / 8;


		TitleSprite = SpriteData[17];

		if (HeapDebugClass::HeapDebug[parEF] == false) {
			parEF = HeapDebugClass::HeapNew<ParrellexEf>()->Init(GetLocation(), 0);
			parEF->AddParrelSpr(SpriteData[26], 1, 0, loc.geth());
			parEF->AddParrelSpr(SpriteData[27], 2, 3 * loc.geth() / 8, loc.geth());
			parEF->AddParrelSpr(SpriteData[28], 4, 3.05f * loc.geth() / 8, loc.geth());
			parEF->AddParrelSpr(SpriteData[29], 8, 3.15f * loc.geth() / 8, loc.geth());
			parEF->AddParrelSpr(SpriteData[30], 16, 2 * loc.geth() / 3, loc.geth() / 2);
		}
		
		GameUI::LBtnPressed = false;
		
		GameStartButton = HeapDebugClass::HeapNew<GameButton>();
		GameStartButton->location = shp::rect4f(cen.x - 2*w, cen.y + w, cen.x + 2 * w, cen.y + 1.5f*w);
		GameStartButton->SetSTR(L"GameStart");
		GameStartButton->OnClick = IFClickGameStart;
		GameStartButton->parameter[0] = 31;
		GameStartButton->RenderFunc = ImageBtnRender;
		GameStartButton->SetParent((GameObject*)this);

		NextMenu = HeapDebugClass::HeapNew<GameButton>();
		NextMenu->location = shp::rect4f(loc.lx - w, cen.y + w, loc.lx, cen.y + 1.5f*w);
		NextMenu->SetSTR(L"Next");
		NextMenu->OnClick = IFClickNext;
		NextMenu->parameter[0] = 32;
		NextMenu->RenderFunc = ImageBtnRender;
		NextMenu->SetParent((GameObject*)this);

		PrevMenu = HeapDebugClass::HeapNew<GameButton>();
		PrevMenu->location = shp::rect4f(loc.fx, cen.y + w, loc.fx + w, cen.y + 1.5f * w);
		PrevMenu->SetSTR(L"Prev");
		PrevMenu->OnClick = IFClickPrev;
		PrevMenu->parameter[0] = 33;
		PrevMenu->RenderFunc = ImageBtnRender;
		PrevMenu->SetParent((GameObject*)this);

		SelectBtn = HeapDebugClass::HeapNew<GameButton>();
		SelectBtn->location = shp::rect4f(cen.x - w/2, loc.ly - w, cen.x + w/2, loc.ly);
		SelectBtn->SetSTR(L"Select");
		SelectBtn->OnClick = IFClickSelect;
		SelectBtn->parameter[0] = 34;
		SelectBtn->RenderFunc = ImageBtnRender;
		SelectBtn->SetParent((GameObject*)this);

		IconImage[0] = SpriteData[18]; // music play
		IconImage[1] = SpriteData[19]; // edit
		IconImage[2] = SpriteData[22]; // setting
		IconImage[3] = SpriteData[23]; // credit
		//IconImage[4] = SpriteData[22]; // setting
		//IconImage[5] = SpriteData[23]; // credit

		first = false;
	}
}

void TitlePage::Update(const float& delta)
{
	if (enable) {
		FirstInit();
		
		parEF->Update(delta);

		if (ismenu == false) {
			GameStartButton->Update(delta);
		}
		else {
			NextMenu->Update(delta);
			PrevMenu->Update(delta);
			SelectBtn->Update(delta);
		}

		if (ismenu) {
			if (TitleAppearFlow.x + delta > TitleAppearFlow.y) {
				TitleAppearFlow.x = TitleAppearFlow.y;
			}
			else {
				TitleAppearFlow.x += delta;
			}

			if (iconChangeFlow.x + delta > iconChangeFlow.y) {
				iconChangeFlow.x = iconChangeFlow.y;
			}
			else {
				iconChangeFlow.x += delta;
			}
		}
	}
}

void TitlePage::Event(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	if (enable && first == false) {
		if (ismenu == false) {
			GameStartButton->Event(hWnd, iMessage, wParam, lParam);
		}
		else {
			if (iconChangeFlow.x > 0.2f) {
				NextMenu->Event(hWnd, iMessage, wParam, lParam);
				PrevMenu->Event(hWnd, iMessage, wParam, lParam);
				SelectBtn->Event(hWnd, iMessage, wParam, lParam);

				if (wParam == 'S' || wParam == VK_LEFT) {
					IFClickPrev(PrevMenu, hWnd, iMessage, wParam, lParam);
				}
				if (wParam == 'L' || wParam == VK_RIGHT) {
					IFClickNext(NextMenu, hWnd, iMessage, wParam, lParam);
				}
				if (wParam == VK_SPACE) {
					IFClickSelect(SelectBtn, hWnd, iMessage, wParam, lParam);
				}
			}
		}
	}
}

void TitlePage::Render(HDC hdc)
{
	if (enable && first == false) {
		parEF->Render(hdc);

		shp::rect4f loc = GetLocation();
		float trate = AnimClass::EaseOut(TitleAppearFlow.x / TitleAppearFlow.y, 3);

		float w = loc.getw() / 8;
		float h = loc.geth() / 8;
		shp::rect4f titlelocdest = shp::rect4f(loc.getCenter().x-h, loc.fy, loc.getCenter().x + h, loc.fy + 2*h);
		shp::vec2f cen = loc.getCenter();
		shp::rect4f titlelocfirst = shp::rect4f(cen.x - 3*h, cen.y - 3*h, cen.x + 3*h, cen.y + 3*h);

		shp::rect4f rrt = shp::rect4f(
			titlelocfirst.fx + trate * (titlelocdest.fx- titlelocfirst.fx), 
			titlelocfirst.fy + trate * (titlelocdest.fy - titlelocfirst.fy),
			titlelocfirst.lx + trate * (titlelocdest.lx - titlelocfirst.lx),
			titlelocfirst.ly + trate * (titlelocdest.ly - titlelocfirst.ly)
		);
		TitleSprite->DrawSprite(hdc, rrt.fx, rrt.fy, rrt.getw(), rrt.geth());

		if (ismenu == false) {
			GameStartButton->RenderObject(hdc);
		}
		else {
			float mrate = AnimClass::EaseIO(iconChangeFlow.x / iconChangeFlow.y, 5);
			if ((0 <= mrate && mrate <= 1) == false) {
				if (mrate < 0) {
					mrate = 0;
				}
				else {
					mrate = 1;
				}
			}

			NextMenu->RenderObject(hdc);
			PrevMenu->RenderObject(hdc);
			SelectBtn->RenderObject(hdc);

			float cenx = loc.getCenter().x + mdir * loc.getw() - mdir * loc.getw() * mrate;
			IconImage[iconNum]->DrawSprite(hdc, cenx - w/2, cen.y - w/2, w, w);

			float subcenx = loc.getCenter().x - mdir * loc.getw() * mrate;
			int prevnum = 0;
			if (mdir > 0) {
				prevnum = iconNum;
				if (prevnum - 1 >= 0) {
					prevnum -= 1;
				}
				else {
					prevnum = 3;
				}
			}
			else {
				prevnum = iconNum;
				if (prevnum + 1 < 4) {
					prevnum += 1;
				}
				else {
					prevnum = 0;
				}
			}
			IconImage[prevnum]->DrawSprite(hdc, subcenx -  w/2, cen.y - w/2, w, w);
		}
	}
}

void TitlePage::Select(char menuChar)
{
	if (enable) {
		nextChar = menuChar;
		NextPage();
		SetEnable(false);
	}
}

void TitlePage::NextPage()
{
	if (enable) {
		switch(nextChar) {
			case 'p':
			{
				GameManager* GM = (GameManager*)gm;
				LevelSelectPage* newpage = HeapDebugClass::HeapNew<LevelSelectPage>()->Init(shp::rect4f(rt.left, rt.top, rt.right, rt.bottom), 1);
				newpage->SetParrellexEF(HeapDebugClass::HeapNew<ParrellexEf>());
				newpage->GetParrellexEF()->Copy(parEF);
				GM->AddObject((GameObject*)newpage);

				/*GameManager* GM = (GameManager*)gm;
				PlayStation* ps = HeapDebugClass::HeapNew<PlayStation>()->Init(shp::rect4f(rt.left, rt.top, rt.right, rt.bottom), false, 1);
				ps->LoadMusic("Sound\\CG.wav");
				ps->LoadData("NoteData\\ChartreuseGreen_H.txt");
				GM->AddObject((GameObject*)ps);*/
			}
				break;
			case 'e':
			{
				Music::ClearSoundsAndChannels();
				GameManager* GM = (GameManager*)gm;
				GM->AddObject((GameObject*)HeapDebugClass::HeapNew<EditStation>()->Init(shp::rect4f(rt.left, rt.top, rt.right, rt.bottom), 1));
			}
				break;
			case 's':
			{
				
				break;
			}
			case 'f':
			{
				break;
			}
			case 't':
			{
				GameManager* GM = (GameManager*)gm;
				GM->AddObject((GameObject*)HeapDebugClass::HeapNew<SettingPage>()->Init(shp::rect4f(rt.left, rt.top, rt.right, rt.bottom), 1));
				break;
			}
			case 'c':
			{
				GameManager* GM = (GameManager*)gm;
				GM->AddObject((GameObject*)HeapDebugClass::HeapNew<CreditPage>()->Init(shp::rect4f(rt.left, rt.top, rt.right, rt.bottom), 1));
				break;
			}
		}
	}
}

void TitlePage::MoveMenu(int dir)
{
	if (enable) {
		mdir = dir;
		iconChangeFlow.x = 0;

		if (dir > 0) {
			if (iconNum + 1 < 4) {
				iconNum += 1;
			}
			else {
				iconNum = 0;
			}
		}
		else {
			if (iconNum - 1 >= 0) {
				iconNum -= 1;
			}
			else {
				iconNum = 3;
			}
		}
	}
}

void TitlePage::GameStart()
{
	if (enable) {
		ismenu = true;
	}
}

const int& TitlePage::GetIconNum()
{
	if (enable) {
		return iconNum;
	}
}

void IFClickGameStart(const GameButton* obj, const HWND& hWnd, const UINT& iMessage, const WPARAM& wParam, const LPARAM& lParam)
{
	TitlePage* tp = (TitlePage*)obj->Parent;
	tp->GameStart();
}

void IFClickNext(const GameButton* obj, const HWND& hWnd, const UINT& iMessage, const WPARAM& wParam, const LPARAM& lParam)
{
	GameUI::LBtnPressed = false;
	TitlePage* tp = (TitlePage*)obj->Parent;
	tp->MoveMenu(1);
}

void IFClickPrev(const GameButton* obj, const HWND& hWnd, const UINT& iMessage, const WPARAM& wParam, const LPARAM& lParam)
{
	GameUI::LBtnPressed = false;
	TitlePage* tp = (TitlePage*)obj->Parent;
	tp->MoveMenu(-1);
}

void IFClickSelect(const GameButton* obj, const HWND& hWnd, const UINT& iMessage, const WPARAM& wParam, const LPARAM& lParam)
{
	GameUI::LBtnPressed = false;
	TitlePage* tp = (TitlePage*)obj->Parent;
	switch (tp->GetIconNum()) {
	case 0:
		tp->Select('p'); // play
		break;
	case 1:
		tp->Select('e'); // edit
		break;
	case 2:
		tp->Select('t'); // setting
		break;
		//tp->Select('s'); // shop
		//break;
	case 3:
		tp->Select('c'); // credit
		break;
		//tp->Select('f'); // profile 
		//break;
	//case 4:
	//	
	//case 5:
	//	tp->Select('c'); // credit
	//	break;
	}
	
}



LevelSelectPage::LevelSelectPage()
{
}

LevelSelectPage::~LevelSelectPage()
{
	//if (HeapDebugClass::HeapDebug[ShopBtn] == true) {
	//	HeapDebugClass::HeapDelete<GameButton>(ShopBtn);
	//}

	//if (HeapDebugClass::HeapDebug[ProfileBtn] == true) {
	//	HeapDebugClass::HeapDelete<GameButton>(ProfileBtn);
	//}

	//if (HeapDebugClass::HeapDebug[SettingBtn] == true) {
	//	HeapDebugClass::HeapDelete<GameButton>(SettingBtn);
	//}

	//if (HeapDebugClass::HeapDebug[PlaySettingBtn] == true) {
	//	HeapDebugClass::HeapDelete<GameButton>(PlaySettingBtn);
	//}

	if (HeapDebugClass::HeapDebug[ReturnBtn] == true) {
		HeapDebugClass::HeapDelete<GameButton>(ReturnBtn);
	}

	if (HeapDebugClass::HeapDebug[PlayBtn] == true) {
		HeapDebugClass::HeapDelete<GameButton>(PlayBtn);
	}

	if (HeapDebugClass::HeapDebug[NextBtn] == true) {
		HeapDebugClass::HeapDelete<GameButton>(NextBtn);
	}

	if (HeapDebugClass::HeapDebug[PrevBtn] == true) {
		HeapDebugClass::HeapDelete<GameButton>(PrevBtn);
	}

	if (HeapDebugClass::HeapDebug[parEF] == true) {
		HeapDebugClass::HeapDelete<ParrellexEf>(parEF);
	}

	if (HeapDebugClass::HeapDebug[Level_AlbumImage] == true) {
		HeapDebugClass::HeapDelete<CImage>(Level_AlbumImage);
	}


}

LevelSelectPage* LevelSelectPage::Init(const shp::rect4f& loc, const int& layer)
{
	SetLocation(loc);
	SetLayer(layer);
	return this;
}

void LevelSelectPage::FirstInit()
{
	if (first && gm != nullptr) {
		shp::rect4f loc = GetLocation();
		shp::vec2f cen = loc.getCenter();
		float w = loc.getw() / 32;
		float h = loc.geth() / 32;

		if (HeapDebugClass::HeapDebug[parEF] == false) {
			parEF = HeapDebugClass::HeapNew<ParrellexEf>()->Init(GetLocation(), 0);
			parEF->AddParrelSpr(SpriteData[26], 1, 0, loc.geth());
			parEF->AddParrelSpr(SpriteData[27], 2, 3 * loc.geth() / 8, loc.geth());
			parEF->AddParrelSpr(SpriteData[28], 4, 3.05f * loc.geth() / 8, loc.geth());
			parEF->AddParrelSpr(SpriteData[29], 8, 3.15f * loc.geth() / 8, loc.geth());
			parEF->AddParrelSpr(SpriteData[30], 16, 2 * loc.geth() / 3, loc.geth() / 2);
		}

		/*ShopBtn = HeapDebugClass::HeapNew<GameButton>();
		ShopBtn->location = shp::rect4f(loc.fx + w, loc.fy + w, loc.fx + 5 * w, loc.fy + 4 * w);
		ShopBtn->SetSTR(L"Shop");
		ShopBtn->OnClick = nullptr;
		ShopBtn->parameter[0] = 20;
		ShopBtn->RenderFunc = ImageBtnRender;
		ShopBtn->SetParent((GameObject*)this);

		ProfileBtn = HeapDebugClass::HeapNew<GameButton>();
		ProfileBtn->location = shp::rect4f(loc.fx + w, loc.fy + 4*w, loc.fx + 5 * w, loc.fy + 8 * w);
		ProfileBtn->SetSTR(L"Profile");
		ProfileBtn->OnClick = nullptr;
		ProfileBtn->parameter[0] = 21;
		ProfileBtn->RenderFunc = ImageBtnRender;
		ProfileBtn->SetParent((GameObject*)this);

		SettingBtn = HeapDebugClass::HeapNew<GameButton>();
		SettingBtn->location = shp::rect4f(loc.fx +  w, loc.fy + 8*w, loc.fx + 5 * w, loc.fy + 12 * w);
		SettingBtn->SetSTR(L"Setting");
		SettingBtn->OnClick = IFClickSetting;
		SettingBtn->parameter[0] = 22;
		SettingBtn->RenderFunc = ImageBtnRender;
		SettingBtn->SetParent((GameObject*)this);*/

		//PlaySettingBtn = HeapDebugClass::HeapNew<GameButton>();
		//PlaySettingBtn->location = shp::rect4f(loc.fx +  w, loc.fy + 12 * w, loc.fx + 4 * w, loc.fy + 16 * w);
		//PlaySettingBtn->SetSTR(L"PlaySet");
		//PlaySettingBtn->OnClick = nullptr;
		//PlaySettingBtn->SetParent((GameObject*)this);

		//

		ReturnBtn = HeapDebugClass::HeapNew<GameButton>();
		ReturnBtn->location = shp::rect4f(loc.lx - 3 * w, loc.ly - 3 * w, loc.lx - w, loc.ly - w);
		ReturnBtn->SetSTR(L"return");
		ReturnBtn->OnClick = IFClickReturnTitle;
		ReturnBtn->parameter[0] = 35;
		ReturnBtn->RenderFunc = ImageBtnRender;
		ReturnBtn->SetParent((GameObject*)this);

		PlayBtn = HeapDebugClass::HeapNew<GameButton>();
		PlayBtn->location = shp::rect4f(cen.x - 4 * w, cen.y - 4 * w, cen.x + 4 * w, cen.y + 4 * w);
		PlayBtn->SetSTR(L"Play");
		PlayBtn->OnClick = IFClickLevelPlay;
		PlayBtn->SetParent((GameObject*)this);

		NextBtn = HeapDebugClass::HeapNew<GameButton>();
		NextBtn->location = shp::rect4f(loc.fx + w, cen.y + 4 * w, loc.fx + 5 * w, cen.y + 6 * w);
		NextBtn->SetSTR(L"Prev");
		NextBtn->parameter[0] = 33;
		NextBtn->RenderFunc = ImageBtnRender;
		NextBtn->OnClick = IFClickPrevLevel;
		NextBtn->SetParent((GameObject*)this);

		PrevBtn = HeapDebugClass::HeapNew<GameButton>();
		PrevBtn->location = shp::rect4f(loc.lx - 5* w, cen.y + 4 * w, loc.lx - w, cen.y + 6 * w);
		PrevBtn->SetSTR(L"Next");
		PrevBtn->parameter[0] = 32;
		PrevBtn->RenderFunc = ImageBtnRender;
		PrevBtn->OnClick = IFClickNextLevel;
		PrevBtn->SetParent((GameObject*)this);

		LoadLevelName();

		first = false;
	}
}

void LevelSelectPage::Update(const float& delta)
{
	FirstInit();
	if (enable && first == false) {
		/*ShopBtn->Update(delta);
		ProfileBtn->Update(delta);
		SettingBtn->Update(delta);*/
		//PlaySettingBtn->Update(delta);
		ReturnBtn->Update(delta);
		PlayBtn->Update(delta);
		NextBtn->Update(delta);
		PrevBtn->Update(delta);

		parEF->Update(delta);

		if (LevelChangeFlow.x + delta > LevelChangeFlow.y) {
			LevelChangeFlow.x = LevelChangeFlow.y;
		}
		else {
			LevelChangeFlow.x += delta;
		}
	}
}

void LevelSelectPage::Event(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	if (enable && first == false) {
		/*ShopBtn->Event(hWnd, iMessage, wParam, lParam);
		ProfileBtn->Event(hWnd, iMessage, wParam, lParam);
		SettingBtn->Event(hWnd, iMessage, wParam, lParam);*/
		//PlaySettingBtn->Event(hWnd, iMessage, wParam, lParam);
		ReturnBtn->Event(hWnd, iMessage, wParam, lParam);
		PlayBtn->Event(hWnd, iMessage, wParam, lParam);
		NextBtn->Event(hWnd, iMessage, wParam, lParam);
		PrevBtn->Event(hWnd, iMessage, wParam, lParam);
	}
}

void LevelSelectPage::Render(HDC hdc)
{
	if (enable && first == false) {
		shp::rect4f loc = GetLocation();
		shp::vec2f cen = loc.getCenter();
		float w = loc.getw() / 32;
		float h = loc.geth() / 32;

		parEF->Render(hdc);
		/*ShopBtn->RenderObject(hdc);
		ProfileBtn->RenderObject(hdc);
		SettingBtn->RenderObject(hdc);*/
		//PlaySettingBtn->RenderObject(hdc);
		ReturnBtn->RenderObject(hdc);
		PlayBtn->RenderObject(hdc);
		NextBtn->RenderObject(hdc);
		PrevBtn->RenderObject(hdc);
		
		if (Level_Name != nullptr) {
			TCHAR* wstr = nullptr;
			wstr = StringFuncs::stringToWStr(Level_Name);
			float ww = 5 * w;
			RECT airt = { cen.x - ww, cen.y - ww, cen.x + ww, cen.y + ww };
			RECT dtrt = { cen.x - ww, cen.y + ww, cen.x + ww, cen.y + 2 * ww };
			if (HeapDebugClass::HeapDebug[wstr] == true) {
				DrawText(hdc, wstr, lstrlen(wstr), &dtrt, DT_CENTER);
				HeapDebugClass::HeapArrDelete<wchar_t>(wstr);
			}

			if (Level_AlbumImage != nullptr) {
				Level_AlbumImage->Draw(hdc, airt);
			}

			SpriteData[24]->DrawFrame(hdc, (int)pdata->levelScoreData[selectLevel].BestRank, cen.x - 2 * w, cen.y - ww - 4 * w, 4 * w, 4 * w);
			SpriteData[25]->DrawFrame(hdc, (int)pdata->levelScoreData[selectLevel].difficulty, cen.x - 2 * w, cen.y + 7*w, 4 * w, 4 * w);
		}
		else {
			LoadLevelName();
		}
	}
}

void LevelSelectPage::NextPage()
{
	if (nextpage == 'p' && Level_Name != nullptr) {
		Music::ClearSoundsAndChannels();
		GameManager* GM = (GameManager*)gm;
		LevelPlayPage* lpp = HeapDebugClass::HeapNew<LevelPlayPage>()->Init(shp::rect4f(rt.left, rt.top, rt.right, rt.bottom), 2, selectLevel, pdata->levelScoreData[selectLevel].songTime);
		GM->AddObject((GameObject*)lpp);

		SetEnable(false);
	}
	else if (nextpage == 't') {
		Music::ClearSoundsAndChannels();
		GameManager* GM = (GameManager*)gm;
		//GM->AddObject((GameObject*)HeapDebugClass::HeapNew<EditStation>()->Init(shp::rect4f(rt.left, rt.top, rt.right, rt.bottom), 1));
		TitlePage* newpage = HeapDebugClass::HeapNew<TitlePage>()->Init(shp::rect4f(rt.left, rt.top, rt.right, rt.bottom), 1);
		newpage->SetParrellexEF(HeapDebugClass::HeapNew<ParrellexEf>());
		newpage->GetParrellexEF()->Copy(parEF);
		GM->AddObject((GameObject*)newpage);
		SetEnable(false);
	}
	else if (nextpage == 's') {
		GameManager* GM = (GameManager*)gm;
		GM->AddObject((GameObject*)HeapDebugClass::HeapNew<SettingPage>()->Init(shp::rect4f(rt.left, rt.top, rt.right, rt.bottom), 1));
	}
}

void LevelSelectPage::MoveSong(int dir)
{
	if (enable) {
		sdir = dir;
		LevelChangeFlow.x = 0;

		if (dir > 0) {
			if (selectLevel + 1 < pdata->lsd_up) {
				selectLevel += 1;
			}
			else {
				selectLevel = 0;
			}
		}
		else {
			if (selectLevel - 1 >= 0) {
				selectLevel -= 1;
			}
			else {
				selectLevel = pdata->lsd_up - 1;
			}
		}

		LoadLevelName();
	}
}

void LevelSelectPage::GoTitle()
{
	nextpage = 't';
	NextPage();
	SetEnable(false);
}

void LevelSelectPage::GoSetting()
{
	if (enable) {
		nextpage = 's';
		NextPage();
		SetEnable(false);
	}
}

void LevelSelectPage::LoadLevelName()
{
	if (enable && first == false) {
		GameManager* GM = (GameManager*)gm;
		pdata = GM->GetPlayerData();
		Level_Name = pdata->levelScoreData[selectLevel].SongName;

		if (Level_Name != nullptr) {
			//데이터 로드
			if (HeapDebugClass::HeapDebug[Level_AlbumImage] == true) {
				HeapDebugClass::HeapDelete<CImage>(Level_AlbumImage);
			}
			Level_AlbumImage = new CImage();
			TCHAR* levelwstr = StringFuncs::stringToWStr(Level_Name);
			TCHAR wstr[128] = {};
			StringFuncs::AddTSTR(wstr, L"Image\\LevelImage\\", levelwstr, 128);
			StringFuncs::AddTSTR(wstr, wstr, L".png", 128);
			HeapDebugClass::HeapArrDelete<wchar_t>(levelwstr);

			Level_AlbumImage->Load(wstr);
		}
		
	}
}

void LevelSelectPage::GoPlayStation()
{
	if (enable) {
		nextpage = 'p';
		NextPage();
		SetEnable(false);
	}
}

void IFClickLevelPlay(const GameButton* obj, const HWND& hWnd, const UINT& iMessage, const WPARAM& wParam, const LPARAM& lParam)
{
	GameUI::LBtnPressed = false;
	LevelSelectPage* tp = (LevelSelectPage*)obj->Parent;
	tp->GoPlayStation();
}

void IFClickNextLevel(const GameButton* obj, const HWND& hWnd, const UINT& iMessage, const WPARAM& wParam, const LPARAM& lParam)
{
	GameUI::LBtnPressed = false;
	LevelSelectPage* tp = (LevelSelectPage*)obj->Parent;
	tp->MoveSong(1);
}

void IFClickPrevLevel(const GameButton* obj, const HWND& hWnd, const UINT& iMessage, const WPARAM& wParam, const LPARAM& lParam)
{
	GameUI::LBtnPressed = false;
	LevelSelectPage* tp = (LevelSelectPage*)obj->Parent;
	tp->MoveSong(-1);
}

void IFClickReturnTitle(const GameButton* obj, const HWND& hWnd, const UINT& iMessage, const WPARAM& wParam, const LPARAM& lParam)
{
	GameUI::LBtnPressed = false;
	LevelSelectPage* tp = (LevelSelectPage*)obj->Parent;
	tp->GoTitle();
}

void IFClickSetting(const GameButton* obj, const HWND& hWnd, const UINT& iMessage, const WPARAM& wParam, const LPARAM& lParam)
{
	GameUI::LBtnPressed = false;
	LevelSelectPage* tp = (LevelSelectPage*)obj->Parent;
	tp->GoSetting();
}


LevelPlayPage::LevelPlayPage()
{
}

LevelPlayPage::~LevelPlayPage()
{
	if (HeapDebugClass::HeapDebug[ESCButton]) {
		HeapDebugClass::HeapDelete<GameButton>(ESCButton);
		ESCButton = nullptr;
	}

	if (HeapDebugClass::HeapDebug[pdata]) {
		((GameObject*)pdata)->SetEnable(false);
	}
}

LevelPlayPage* LevelPlayPage::Init(const shp::rect4f& loc, const int& layer, int levelNum, const int& maxtime)
{
	SetLocation(loc);
	SetLayer(layer);
	levelnum = levelNum;
	MaxTime = maxtime;
	return this;
}

void LevelPlayPage::FirstInit()
{
	if (first && gm != nullptr) {
		GameManager* GM = (GameManager*)gm;
		PlayerData* pd = nullptr;
		pd = GM->GetPlayerData();
		strcpy_s(songName, pd->levelScoreData[levelnum].SongName);

		shp::rect4f loc = GetLocation();
		shp::vec2f cen = loc.getCenter();
		float w = loc.getw() / 32;
		float h = loc.geth() / 32;

		pdata = HeapDebugClass::HeapNew<PlayStation>()->Init(shp::rect4f(rt.left, rt.top, rt.right, rt.bottom), true, 1);

		char* ptr = songName;

		char str[128] = {};

		StringFuncs::AddStr(str, "Sound\\", ptr, 128);
		StringFuncs::AddStr(str, str, ".mp3", 128);
		pdata->LoadMusic(str);

		StringFuncs::AddStr(str, "NoteData\\", ptr, 128);
		StringFuncs::AddStr(str, str, ".txt", 128);
		pdata->LoadData(str);

		StringFuncs::AddStr(str, "Image\\LevelImage\\", ptr, 128);
		StringFuncs::AddStr(str, str, ".png", 128);
		TCHAR* wstr = StringFuncs::stringToWStr(str);
		pdata->SetBackGround(wstr);
		HeapDebugClass::HeapArrDelete<TCHAR>(wstr);

		GM->AddObject((GameObject*)pdata);

		ESCButton = HeapDebugClass::HeapNew<GameButton>();
		ESCButton->location = shp::rect4f(loc.fx + w, loc.fy + w, loc.fx + 3 * w, loc.fy + 3 * w);
		ESCButton->SetSTR(L"ESC");
		ESCButton->OnClick = IFClickESC;
		ESCButton->SetParent((GameObject*)this);

		first = false;
	}
}

void LevelPlayPage::Update(const float& delta)
{
	FirstInit();
	if (enable && first == false) {
		ESCButton->Update(delta);

		if (pdata->GetTime() > MaxTime) {
			int sid = pdata->GetSongSoundID();
			Music::Stop(sid);
			Music::ClearSoundsAndChannels();
			pstate = 'e';
			NextPage();
			SetEnable(false);
		}
	}
}

void LevelPlayPage::Event(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	if (enable && first == false) {
		ESCButton->Event(hWnd, iMessage, wParam, lParam);
	}
}

void LevelPlayPage::Render(HDC hdc)
{
	if (enable && first == false) {
		ESCButton->RenderObject(hdc);
	}
}

void LevelPlayPage::NextPage()
{
	if (enable && first == false) {
		if (pstate == 'e') {
			// end play go evaluation page
			

			GameManager* GM = (GameManager*)gm;

			PlayerData* playerData = GM->GetPlayerData();
			int bestScore = playerData->levelScoreData[levelnum].BestScore;
			Rank bestRank = playerData->levelScoreData[levelnum].BestRank;

			if (pdata->GetPGMNum('m') == 0 && pdata->GetPGMNum('g') == 0) {
				pdata->SetScore(1000000000);
			}

			Rank rank;
			int score = pdata->GetScore();
			score = (int)(1000000000 * (1.0f - powf(1.0f - (float)score / 1000000000.0f, 2)));
			if (score == 1000000000) {
				rank = Rank::Master;
			}
			else if (score >= 950000000) {
				rank = Rank::S;
			}
			else if (score >= 900000000) {
				rank = Rank::A;
			}
			else if (score >= 800000000) {
				rank = Rank::B;
			}
			else if (score >= 700000000) {
				rank = Rank::C;
			}
			else if (score >= 600000000) {
				rank = Rank::D;
			}
			else if (score >= 500000000) {
				rank = Rank::E;
			}
			else {
				rank = Rank::F;
			}

			bool fullc = false;
			if (pdata->GetPGMNum('m') == 0) {
				fullc = true;
			}

			if (score > bestScore) {
				playerData->levelScoreData[levelnum].BestScore = score;
				playerData->levelScoreData[levelnum].BestRank = rank;
				playerData->levelScoreData[levelnum].PerfectNum = pdata->GetPGMNum('p');
				playerData->levelScoreData[levelnum].GoodNum = pdata->GetPGMNum('g');
				playerData->levelScoreData[levelnum].MissNum = pdata->GetPGMNum('m');
				if (playerData->levelScoreData[levelnum].isFullCombo == false) {
					playerData->levelScoreData[levelnum].isFullCombo = fullc;
				}
			}

			playerData->SaveData();

			EvaluatePage* ep = HeapDebugClass::HeapNew<EvaluatePage>()->Init(GetLocation(), GetLayer(),
				rank, score, pdata->GetPGMNum('p'), pdata->GetPGMNum('g'), pdata->GetPGMNum('m'), pdata->GetMaxCombo(), fullc);
			ep->SetSongNum(levelnum);
			GM->AddObject((GameObject*)ep);
		}

		if (pstate == 'l') {
			GameManager* GM = (GameManager*)gm;
			GM->AddObject((GameObject*)HeapDebugClass::HeapNew<LevelSelectPage>()->Init(shp::rect4f(rt.left, rt.top, rt.right, rt.bottom), 1));
			SetEnable(false);
		}
	}
}

void IFClickESC(const GameButton* obj, const HWND& hWnd, const UINT& iMessage, const WPARAM& wParam, const LPARAM& lParam)
{
	GameUI::LBtnPressed = false;
	LevelPlayPage* lp = (LevelPlayPage*)obj->Parent;
	lp->pstate = 'l';
	lp->NextPage();
}


EvaluatePage::EvaluatePage()
{
}

EvaluatePage::~EvaluatePage()
{
	if (HeapDebugClass::HeapDebug[RetryButton]) {
		HeapDebugClass::HeapDelete<GameButton>(RetryButton);
		RetryButton = nullptr;
	}

	if (HeapDebugClass::HeapDebug[NextButton]) {
		HeapDebugClass::HeapDelete<GameButton>(NextButton);
		NextButton = nullptr;
	}

	if (HeapDebugClass::HeapDebug[ScoreNE]) {
		HeapDebugClass::HeapDelete<NumberEffect>(ScoreNE);
		ScoreNE = nullptr;
	}
}

EvaluatePage* EvaluatePage::Init(const shp::rect4f& loc, const int& layer, Rank r, int score, int perfectN, 
	int goodN, int missN, int MaxCombo, bool FullCombo)
{
	SetLocation(loc);
	SetLayer(layer);
	rank = r;
	Score = score;
	perfectnum = perfectN;
	goodnum = goodN;
	missnum = missN;
	maxcombo = MaxCombo;
	isfullcombo = FullCombo;
	return this;
}

void EvaluatePage::FirstInit()
{
	if (first && gm != nullptr) {
		GameManager* GM = (GameManager*)gm;

		shp::rect4f loc = GetLocation();
		shp::vec2f cen = loc.getCenter();
		float w = loc.getw() / 32;
		float h = loc.geth() / 32;

		RetryButton = HeapDebugClass::HeapNew<GameButton>();
		RetryButton->location = shp::rect4f(loc.fx + w, loc.ly - 4 * w, loc.fx + 5 * w, loc.ly - w);
		RetryButton->SetSTR(L"Retry");
		RetryButton->OnClick = IFClickEvRetry;
		RetryButton->SetParent((GameObject*)this);

		NextButton = HeapDebugClass::HeapNew<GameButton>();
		NextButton->location = shp::rect4f(loc.lx - 5 * w, loc.ly - 4 * w, loc.lx - w, loc.ly - w);
		NextButton->SetSTR(L"Next");
		NextButton->OnClick = IFClickEvNext;
		NextButton->SetParent((GameObject*)this);

		shp::rect4f tloc = shp::rect4f(cen.x, cen.y - 10 * w, loc.lx - w, cen.y - 7 * w);
		ScoreNE = HeapDebugClass::HeapNew<NumberEffect>()->Init(SpriteData[9], tloc.getCenter(),
			shp::vec2f(tloc.getw() / 2, tloc.geth() / 2), 1.5f, 5, 9);
		GM->AddObject((GameObject*)ScoreNE);

		ScoreNE->SetNumber(Score);

		first = false;
	}
}

void EvaluatePage::Update(const float& delta)
{
	FirstInit();
	if (enable && first == false) {
		RetryButton->Update(delta);
		NextButton->Update(delta);
	}
}

void EvaluatePage::Event(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	if (enable && first == false) {
		RetryButton->Event(hWnd, iMessage, wParam, lParam);
		NextButton->Event(hWnd, iMessage, wParam, lParam);
	}
}

void EvaluatePage::Render(HDC hdc)
{
	constexpr int rankimg = 24;
	if (enable && first == false) {
		
		

		shp::rect4f loc = GetLocation();
		

		shp::vec2f cen = loc.getCenter();
		float w = loc.getw() / 32;
		float h = loc.geth() / 32;

		HBRUSH BlackB = CreateSolidBrush(RGB(25, 25, 25));
		SelectObject(hdc, BlackB);
		Rectangle(hdc, loc.fx, loc.fy, loc.lx, loc.ly);

		RetryButton->RenderObject(hdc);
		NextButton->RenderObject(hdc);

		SpriteData[rankimg]->DrawFrame(hdc, (int)rank, cen.x - 10 * w, cen.y - 10 * w, 10 * w, 10 * w);
		DeleteObject(BlackB);
	}
}

void EvaluatePage::SetSongNum(int n)
{
	if (enable) {
		levelNum = n;
	}
}

void EvaluatePage::NextPage()
{
	if (nstate == 's') {
		//Music::ClearSoundsAndChannels();
		GameManager* GM = (GameManager*)gm;
		GM->AddObject((GameObject*)HeapDebugClass::HeapNew<LevelSelectPage>()->Init(shp::rect4f(rt.left, rt.top, rt.right, rt.bottom), 1));

		SetEnable(false);
	}
	else if (nstate == 'p') {
		//Music::ClearSoundsAndChannels();
		GameManager* GM = (GameManager*)gm;
		PlayerData* pdata = nullptr;
		pdata = GM->GetPlayerData();

		LevelPlayPage* lpp = HeapDebugClass::HeapNew<LevelPlayPage>()->Init(shp::rect4f(rt.left, rt.top, rt.right, rt.bottom), 2, levelNum, pdata->levelScoreData[levelNum].songTime);
		GM->AddObject((GameObject*)lpp);

		SetEnable(false);
	}
}

void EvaluatePage::GoLevelSelect()
{
	if (enable) {
		nstate = 's';
		NextPage();
	}
	
}

void EvaluatePage::GoPlayLevel()
{
	if (enable) {
		nstate = 'p';
		NextPage();
	}
	
}

void IFClickEvNext(const GameButton* obj, const HWND& hWnd, const UINT& iMessage, const WPARAM& wParam, const LPARAM& lParam)
{
	GameUI::LBtnPressed = false;
	EvaluatePage* tp = (EvaluatePage*)obj->Parent;
	tp->GoLevelSelect();
}

void IFClickEvRetry(const GameButton* obj, const HWND& hWnd, const UINT& iMessage, const WPARAM& wParam, const LPARAM& lParam)
{
	GameUI::LBtnPressed = false;
	EvaluatePage* tp = (EvaluatePage*)obj->Parent;
	tp->GoPlayLevel();
}

SettingPage::SettingPage()
{
}

SettingPage::~SettingPage()
{
	if (HeapDebugClass::HeapDebug[ReturnBtn] == true) {
		HeapDebugClass::HeapDelete<GameButton>(ReturnBtn);
	}

	if (HeapDebugClass::HeapDebug[BackGroundMusicSlider] == true) {
		HeapDebugClass::HeapDelete<GameSlider>(BackGroundMusicSlider);
	}

	if (HeapDebugClass::HeapDebug[PlayMusicSlider] == true) {
		HeapDebugClass::HeapDelete<GameSlider>(PlayMusicSlider);
	}

	if (HeapDebugClass::HeapDebug[SoundEffectSlider] == true) {
		HeapDebugClass::HeapDelete<GameSlider>(SoundEffectSlider);
	}

	if (HeapDebugClass::HeapDebug[SyncSlider] == true) {
		HeapDebugClass::HeapDelete<GameSlider>(SyncSlider);
	}

	if (HeapDebugClass::HeapDebug[BasicSpeedSlider] == true) {
		HeapDebugClass::HeapDelete<GameSlider>(BasicSpeedSlider);
	}

	if (HeapDebugClass::HeapDebug[RimSoundVolumeSlider] == true) {
		HeapDebugClass::HeapDelete<GameSlider>(RimSoundVolumeSlider);
	}

}

SettingPage* SettingPage::Init(const shp::rect4f& loc, const int& layer)
{
	SetLocation(loc);
	SetLayer(layer);
	return this;
}

void SettingPage::FirstInit()
{
	if (enable && first) {
		shp::rect4f loc = GetLocation();
		shp::vec2f cen = loc.getCenter();
		float w = loc.getw() / 32;
		float h = loc.geth() / 32;

		GameManager* GM = (GameManager*)gm;

		ReturnBtn = HeapDebugClass::HeapNew<GameButton>();
		ReturnBtn->location = shp::rect4f(loc.lx - 3 * w, loc.ly - 3 * w, loc.lx - w, loc.ly - w);
		ReturnBtn->SetSTR(L"return");
		ReturnBtn->OnClick = IFClickSetReturn;
		ReturnBtn->parameter[0] = 35;
		ReturnBtn->RenderFunc = ImageBtnRender;
		ReturnBtn->SetParent((GameObject*)this);

		BackGroundMusicSlider = HeapDebugClass::HeapNew<GameSlider>();
		BackGroundMusicSlider->location = shp::rect4f(loc.fx + 10, loc.fy + 10, loc.fx + 10 + 15 * w, loc.fy + 10 + 2 * w);
		BackGroundMusicSlider->SetSTR(L"BackGround Music Volume");
		BackGroundMusicSlider->OnEvent = nullptr;
		BackGroundMusicSlider->SetParent((GameObject*)this);
		BackGroundMusicSlider->value = GM->GetPlayerData()->Background_music_Volume;

		PlayMusicSlider = HeapDebugClass::HeapNew<GameSlider>();
		PlayMusicSlider->location = shp::rect4f(loc.fx + 10, loc.fy + 10 + 3 * w, loc.fx + 10 + 15 * w, loc.fy + 10 + 5 * w);
		PlayMusicSlider->SetSTR(L"Game Play Music Volume");
		PlayMusicSlider->OnEvent = nullptr;
		PlayMusicSlider->SetParent((GameObject*)this);
		PlayMusicSlider->value = GM->GetPlayerData()->music_Volume;

		SoundEffectSlider = HeapDebugClass::HeapNew<GameSlider>();
		SoundEffectSlider->location = shp::rect4f(loc.fx + 10, loc.fy + 10 + 6 * w, loc.fx + 10 + 15 * w, loc.fy + 10 + 8 * w);
		SoundEffectSlider->SetSTR(L"Sound Effect Volume");
		SoundEffectSlider->OnEvent = nullptr;
		SoundEffectSlider->SetParent((GameObject*)this);
		SoundEffectSlider->value = GM->GetPlayerData()->Effect_Volume;

		SyncSlider = HeapDebugClass::HeapNew<GameSlider>();
		SyncSlider->location = shp::rect4f(loc.fx + 10, loc.fy + 10 + 9 * w, loc.fx + 10 + 15 * w, loc.fy + 10 + 11 * w);
		SyncSlider->SetSTR(L"Sync Time");
		SyncSlider->OnEvent = nullptr;
		SyncSlider->SetParent((GameObject*)this);
		SyncSlider->value = GM->GetPlayerData()->sync_time;

		BasicSpeedSlider = HeapDebugClass::HeapNew<GameSlider>();
		BasicSpeedSlider->location = shp::rect4f(loc.fx + 10, loc.fy + 10 + 12 * w, loc.fx + 10 + 15 * w, loc.fy + 10 + 14 * w);
		BasicSpeedSlider->SetSTR(L"Basic Speed");
		BasicSpeedSlider->OnEvent = nullptr;
		BasicSpeedSlider->SetParent((GameObject*)this);
		BasicSpeedSlider->value = GM->GetPlayerData()->basicSpeed;

		RimSoundVolumeSlider = HeapDebugClass::HeapNew<GameSlider>();
		RimSoundVolumeSlider->location = shp::rect4f(loc.fx + 10, loc.fy + 10 + 15 * w, loc.fx + 10 + 15 * w, loc.fy + 10 + 17 * w);
		RimSoundVolumeSlider->SetSTR(L"Rim Sound Volume");
		RimSoundVolumeSlider->OnEvent = nullptr;
		RimSoundVolumeSlider->SetParent((GameObject*)this);
		RimSoundVolumeSlider->value = GM->GetPlayerData()->Rim_Volume;

		first = false;
	}
}

void SettingPage::Update(const float& delta)
{
	FirstInit();
	if (enable && first == false) {
		ReturnBtn->Update(delta);
		BackGroundMusicSlider->Update(delta);
		PlayMusicSlider->Update(delta);
		SoundEffectSlider->Update(delta);
		SyncSlider->Update(delta);
		BasicSpeedSlider->Update(delta);
		RimSoundVolumeSlider->Update(delta);

		GameManager* GM = (GameManager*)gm;
		GM->GetPlayerData()->Background_music_Volume = BackGroundMusicSlider->value;
		GM->GetPlayerData()->sync_time = SyncSlider->value;
		GM->GetPlayerData()->music_Volume = PlayMusicSlider->value;
		GM->GetPlayerData()->Effect_Volume = SoundEffectSlider->value;
		GM->GetPlayerData()->basicSpeed = BasicSpeedSlider->value;
		GM->GetPlayerData()->Rim_Volume = RimSoundVolumeSlider->value;
	}
}

void SettingPage::Event(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	if (enable && first == false) {
		ReturnBtn->Event(hWnd, iMessage, wParam, lParam);

		BackGroundMusicSlider->Event(hWnd, iMessage, wParam, lParam);
		PlayMusicSlider->Event(hWnd, iMessage, wParam, lParam);
		SoundEffectSlider->Event(hWnd, iMessage, wParam, lParam);
		SyncSlider->Event(hWnd, iMessage, wParam, lParam);
		BasicSpeedSlider->Event(hWnd, iMessage, wParam, lParam);
		RimSoundVolumeSlider->Event(hWnd, iMessage, wParam, lParam);
	}
}

void SettingPage::Render(HDC hdc)
{
	if (enable && first == false) {
		shp::rect4f loc = GetLocation();
		Sprite* backspr = SpriteData[26];
		backspr->DrawSprite(hdc, loc.fx, loc.fy, loc.getw(), loc.geth());

		ReturnBtn->RenderObject(hdc);

		BackGroundMusicSlider->RenderObject(hdc);
		PlayMusicSlider->RenderObject(hdc);
		SoundEffectSlider->RenderObject(hdc);
		SyncSlider->RenderObject(hdc);
		BasicSpeedSlider->RenderObject(hdc);
		RimSoundVolumeSlider->RenderObject(hdc);
	}
}

void SettingPage::NextPage()
{
	if (enable) {
		if (nstate == 't') {
			Music::ClearSoundsAndChannels();
			GameManager* GM = (GameManager*)gm;
			//GM->AddObject((GameObject*)HeapDebugClass::HeapNew<EditStation>()->Init(shp::rect4f(rt.left, rt.top, rt.right, rt.bottom), 1));
			TitlePage* newpage = HeapDebugClass::HeapNew<TitlePage>()->Init(shp::rect4f(rt.left, rt.top, rt.right, rt.bottom), 1);
			GM->AddObject((GameObject*)newpage);
			SetEnable(false);
		}
		else if (nstate == 'l') {

		}
	}
}

void IFClickSetReturn(const GameButton* obj, const HWND& hWnd, const UINT& iMessage, const WPARAM& wParam, const LPARAM& lParam)
{
	GameUI::LBtnPressed = false;
	SettingPage* set = (SettingPage*)obj->Parent;
	set->NextPage();
}



CreditPage::CreditPage()
{
}

CreditPage::~CreditPage()
{
	if (ReturnBtn != nullptr && HeapDebugClass::HeapDebug[ReturnBtn] == true) {
		HeapDebugClass::HeapDelete(ReturnBtn);
	}
}

CreditPage* CreditPage::Init(const shp::rect4f& loc, const int& layer)
{
	SetLocation(loc);
	SetLayer(layer);
	return this;
}

void CreditPage::FirstInit()
{
	if (enable && first) {
		shp::rect4f loc = GetLocation();
		shp::vec2f cen = loc.getCenter();
		float w = loc.getw() / 32;
		float h = loc.geth() / 32;

		ReturnBtn = HeapDebugClass::HeapNew<GameButton>();
		ReturnBtn->location = shp::rect4f(loc.lx - 3 * w, loc.ly - 3 * w, loc.lx - w, loc.ly - w);
		ReturnBtn->SetSTR(L"return");
		ReturnBtn->OnClick = IFClickCriReturn;
		ReturnBtn->parameter[0] = 35;
		ReturnBtn->RenderFunc = ImageBtnRender;
		ReturnBtn->SetParent((GameObject*)this);
		first = false;
	}
	
}

void CreditPage::Update(const float& delta)
{
	FirstInit();
	if (enable && first == false) {
		ReturnBtn->Update(delta);
	}
}

void CreditPage::Event(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	if (enable && first == false) {
		ReturnBtn->Event(hWnd, iMessage, wParam, lParam);
	}
}

void CreditPage::Render(HDC hdc)
{
	if (enable && first == false) {
		shp::rect4f loc = GetLocation();
		Sprite* backspr = SpriteData[26];
		backspr->DrawSprite(hdc, loc.fx, loc.fy, loc.getw(), loc.geth());

		RECT rt = { loc.fx, loc.fy, loc.getw(), loc.geth() };
		DrawText(hdc, creditTEXT, wcslen(creditTEXT), &rt, DT_CENTER);

		ReturnBtn->RenderObject(hdc);
	}
}

void CreditPage::NextPage()
{
	if (enable) {
		if (nstate == 't') {
			Music::ClearSoundsAndChannels();
			GameManager* GM = (GameManager*)gm;
			//GM->AddObject((GameObject*)HeapDebugClass::HeapNew<EditStation>()->Init(shp::rect4f(rt.left, rt.top, rt.right, rt.bottom), 1));
			TitlePage* newpage = HeapDebugClass::HeapNew<TitlePage>()->Init(shp::rect4f(rt.left, rt.top, rt.right, rt.bottom), 1);
			GM->AddObject((GameObject*)newpage);
			SetEnable(false);
		}
		else if (nstate == 'l') {

		}
	}
}

void IFClickCriReturn(const GameButton* obj, const HWND& hWnd, const UINT& iMessage, const WPARAM& wParam, const LPARAM& lParam)
{
	GameUI::LBtnPressed = false;
	CreditPage* set = (CreditPage*)obj->Parent;
	set->NextPage();
}