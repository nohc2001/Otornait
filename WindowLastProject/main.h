#pragma once

#include <tchar.h>
#include <time.h>
#include <Windows.h>
#include <vector>
#include <fmod.hpp>
#include <map>

#include "Animation.h"
#include "Sprite.h"
#include "Music.h"
#include "Game.h"
#include "PlayStation.h"
#include "EditStation.h"
#include "HeapDebug.h"
#include "Page.h"
#include "resource.h"

using namespace std;

constexpr int SOUND_DATA_MAX = 100;
FMOD::Sound * SoundData[SOUND_DATA_MAX];
int soundUpdate = 0;

constexpr int CHANNEL_MAX = 20;
FMOD::Channel* Channels[CHANNEL_MAX];
int channelUpdate = 0;

FMOD::System* SoundSystem = nullptr;
#define FALLTHROUGH

//추가할 것 : 스레드 관리, 힙메모리 관리, 사운드 이팩트, UI, 이미지 활용 (선형변환, 회전? 여러 함수들), 소스코드 나누기
/*
* 사용가능한것
* 스레드 - 사용가능
* 좌표계와 카메라 - 사용가능
* 사운드 라이브러리 - 사용가능함
* 무빙 애니메이션 함수
*/

//sprite data
vector<Sprite*> SpriteData;

const int& AddSpriteToData(Sprite* const sprite) { // int spriteid = AddSpriteToData(new Sprite()); // 이런 식으로 스프라이트 저장함.
	int at = SpriteData.size();
	SpriteData.push_back(sprite);
	return at;
}

void ClearSpriteData() {
	for (int i = 0; i < SpriteData.size(); ++i) {
		if (SpriteData[i] != nullptr) {
			HeapDebugClass::HeapDelete<Sprite>(SpriteData[i]);
			SpriteData[i] = nullptr;
		}
	}
	SpriteData.clear();
}

void InitSprite() {
	AddSpriteToData(HeapDebugClass::HeapNew<Sprite>()->Init(L"Image\\NormalNote.png")); // 0
	AddSpriteToData(HeapDebugClass::HeapNew<Sprite>()->Init(L"Image\\LongNote.png", 179, 179)); // 1
	AddSpriteToData(HeapDebugClass::HeapNew<Sprite>()->Init(L"Image\\SpaceNote.png")); // 2
	AddSpriteToData(HeapDebugClass::HeapNew<Sprite>()->Init(L"Image\\SpaceLeftNote.png")); // 3
	AddSpriteToData(HeapDebugClass::HeapNew<Sprite>()->Init(L"Image\\SpaceRightNote.png")); // 4
	AddSpriteToData(HeapDebugClass::HeapNew<Sprite>()->Init(L"Image\\SizeEffect.png")); // 5
	AddSpriteToData(HeapDebugClass::HeapNew<Sprite>()->Init(L"Image\\PerfectImage.png")); // 6
	AddSpriteToData(HeapDebugClass::HeapNew<Sprite>()->Init(L"Image\\GoodImage.png")); // 7
	AddSpriteToData(HeapDebugClass::HeapNew<Sprite>()->Init(L"Image\\MissImage.png")); // 8
	AddSpriteToData(HeapDebugClass::HeapNew<Sprite>()->Init(L"Image\\NumberImage.png", 100, 100)); // 9
	AddSpriteToData(HeapDebugClass::HeapNew<Sprite>()->Init(L"Image\\PushNote.png", 100, 100)); // 10
	AddSpriteToData(HeapDebugClass::HeapNew<Sprite>()->Init(L"Image\\BackGround.png")); // 11
	AddSpriteToData(HeapDebugClass::HeapNew<Sprite>()->Init(L"Image\\Back.png")); // 12
	AddSpriteToData(HeapDebugClass::HeapNew<Sprite>()->Init(L"Image\\Horizonal.png")); // 13
	AddSpriteToData(HeapDebugClass::HeapNew<Sprite>()->Init(L"Image\\Vertical.png")); // 14
	AddSpriteToData(HeapDebugClass::HeapNew<Sprite>()->Init(L"Image\\Logo.png")); // 15
	AddSpriteToData(HeapDebugClass::HeapNew<Sprite>()->Init(L"Image\\Logo2.png")); // 16
	AddSpriteToData(HeapDebugClass::HeapNew<Sprite>()->Init(L"Image\\Title.png")); // 17
	AddSpriteToData(HeapDebugClass::HeapNew<Sprite>()->Init(L"Image\\MusicIcon.png")); // 18
	AddSpriteToData(HeapDebugClass::HeapNew<Sprite>()->Init(L"Image\\ToolIcon.png")); // 19
	AddSpriteToData(HeapDebugClass::HeapNew<Sprite>()->Init(L"Image\\Shop.png")); // 20
	AddSpriteToData(HeapDebugClass::HeapNew<Sprite>()->Init(L"Image\\Profile.png")); // 21
	AddSpriteToData(HeapDebugClass::HeapNew<Sprite>()->Init(L"Image\\SettingIcon.png")); // 22
	AddSpriteToData(HeapDebugClass::HeapNew<Sprite>()->Init(L"Image\\Credit.png")); // 23
	AddSpriteToData(HeapDebugClass::HeapNew<Sprite>()->Init(L"Image\\Rank.png", 100, 100)); // 24
	AddSpriteToData(HeapDebugClass::HeapNew<Sprite>()->Init(L"Image\\Difficulty.png", 100, 100)); // 25
	AddSpriteToData(HeapDebugClass::HeapNew<Sprite>()->Init(L"Image\\BG.png")); // 26
	AddSpriteToData(HeapDebugClass::HeapNew<Sprite>()->Init(L"Image\\cityfar.png")); // 27
	AddSpriteToData(HeapDebugClass::HeapNew<Sprite>()->Init(L"Image\\citymid.png")); // 28
	AddSpriteToData(HeapDebugClass::HeapNew<Sprite>()->Init(L"Image\\cityclose.png")); // 29
	AddSpriteToData(HeapDebugClass::HeapNew<Sprite>()->Init(L"Image\\city.png")); // 30
	AddSpriteToData(HeapDebugClass::HeapNew<Sprite>()->Init(L"Image\\GameStart.png")); // 31
	AddSpriteToData(HeapDebugClass::HeapNew<Sprite>()->Init(L"Image\\NextBtn.png")); // 32
	AddSpriteToData(HeapDebugClass::HeapNew<Sprite>()->Init(L"Image\\PrevBtn.png")); // 33
	AddSpriteToData(HeapDebugClass::HeapNew<Sprite>()->Init(L"Image\\SelectBtn.png")); // 34
	AddSpriteToData(HeapDebugClass::HeapNew<Sprite>()->Init(L"Image\\ReturnBtn.png")); // 35
	AddSpriteToData(HeapDebugClass::HeapNew<Sprite>()->Init(L"Image\\ShowImage.png")); // 36
	AddSpriteToData(HeapDebugClass::HeapNew<Sprite>()->Init(L"Image\\NormalNote_Vert.png")); // 37
	AddSpriteToData(HeapDebugClass::HeapNew<Sprite>()->Init(L"Image\\SpaceNote_Vert.png")); // 38
	AddSpriteToData(HeapDebugClass::HeapNew<Sprite>()->Init(L"Image\\SpaceLeftNote_Vert.png")); //39
	AddSpriteToData(HeapDebugClass::HeapNew<Sprite>()->Init(L"Image\\SpaceRightNote_Vert.png")); // 40
}

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
HINSTANCE g_hInst;
const wchar_t* lpszClass = L"Otornait";

HWND hWnd;
HINSTANCE hInst;
bool anim_frame_change = false;

int WINAPI CALLBACK WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpszCmdParam, _In_ int nCmdShow);

RECT rt;

HBITMAP hBit;
HBRUSH whiteB;
int n = 0;
int ntime = 0;
float ftime = 0;

clock_t prec, savec;
int stackTime = 0;
constexpr int TIME_UPDATE = 15;

DWORD WINAPI TimeLoop(LPVOID lpParameter);

HANDLE hTimeLoopThread;
LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam);

GameManager gm;

bool first = true;
void InitObjs() {
	if (first) {
		//((EditStation*)gm.GetGameObject(0))->LoadData("sampleTest.txt");
		first = false;
	}
}