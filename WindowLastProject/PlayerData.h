#pragma once
#include <fstream>
#include "SpaceMath.h"
#include "HeapDebug.h"

typedef enum class Difficulty {
	Easy = 0,
	Normal = 1,
	Hard = 2
};

typedef enum class Rank {
	F = 0,
	E = 1,
	D = 2,
	C = 3,
	B = 4,
	A = 5,
	S = 6,
	Master = 7
};

typedef struct LevelScoreData {
	// -> 여기부터 곡 식별을 위한 변수

	//곡 이름
	char* SongName;

	// 템포
	int BPM_Tempo;

	//곡 길이
	float songTime;

	//작곡자
	char* SongMaker;

	//작곡자
	char* LevelMaker;

	//앨범아트 제작자
	char* AlbumArtMaker;

	//난이도
	Difficulty difficulty;

	//레벨이 가진 노트의 개수
	int noteNum;

	//-> 여기부터는 곡에대한 성과(Score) 변수

	//최고랭크
	Rank BestRank;

	//최고 점수
	int BestScore;

	//풀콤보 유무
	bool isFullCombo;

	//perfect 개수
	int PerfectNum;

	//good 개수
	int GoodNum;
	
	//miss 개수
	int MissNum;
};

typedef struct ItemData {
	//아이템 이름
	char* Name = {};

	//스프라이트 아이디
	size_t SpriteID = 0;

	//추가 경험치 비율
	float Extra_Exp = 0;

	//추가 최대 HP 비율
	float Extra_Hp = 0;

	//추가 회복량 비율
	float Extra_Heal = 0;

	//추가 피해량 비율
	float Extra_Damage = 0;

	//추가 보상게임머니 비율
	float Extra_Money = 0;
};

typedef struct Challenge {
	char* ChallengeName;
	size_t SpriteID = 0;
	//진행률
	shp::vec2f PrograssFlow = shp::vec2f(0, 1);

	// 진행률을 확인하는 함수
	float (*GetPrograssFlowFunc)() = nullptr;
};

//오직 스택으로만 존재하자.
class PlayerData {
public:
	static constexpr int STRMAX = 128;

	//플레이어 이름
	char* Name;

	//가진 게임머니
	int GameMoney = 0;

	//총 점수
	int TotalScore = 0;

	//총 풀콤보
	int TotalFullCombo = 0;

	//소지한 노래레벨 개수
	int SaveLevelNum = 1;

	// 지금 장착한 아이템
	int EquipItemID = 0;

	// 지금 장착한 칭호
	int EquipTitleID = 0;

	//총 랭크 개수
	//배열 첫번째 인수
	// 0: Easy | 1 : Normal | 2 : Hard | 3 : extraspace | 4 : extraspace | 5 : extraspace
	//배열 두번째 인수
	// 0: F | 1 : E | 2 : D | 3 : C | 4 : B | 5 : A | 6 : S | 7 : M | 8 : extraspace
	int RankNum[6][9] = {};

	//가진 곡 정보 / 성과 정보
	int lsd_up = 0;
	static constexpr int LSD_MAX = 100;
	LevelScoreData levelScoreData[LSD_MAX] = {};
	

	//가진 아이템들의 개수
	int itemdata_up = 0;
	static constexpr int ITEM_MAX = 100;
	ItemData itemData[ITEM_MAX] = {};
	

	//칭호, 도전과제, 진행률
	int cd_up = 0;
	static constexpr int CHALLENGE_MAX = 100;
	Challenge challengeData[CHALLENGE_MAX] = {};
	
	//설정변수
	
	//씽크
	float sync_time = 0;

	//노래음량
	float music_Volume = 1;

	//타격음 음량
	float Rim_Volume = 1;

	//효과음 음량
	float Effect_Volume = 1;

	//노트 내려오는 속도
	float basicSpeed = 1;

	//노래음량
	float Background_music_Volume = 1;

public:
	PlayerData();
	virtual ~PlayerData();

	void LoadData();
	void SaveData();
	void Clear();
};