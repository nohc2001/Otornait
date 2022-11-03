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
	// -> ������� �� �ĺ��� ���� ����

	//�� �̸�
	char* SongName;

	// ����
	int BPM_Tempo;

	//�� ����
	float songTime;

	//�۰���
	char* SongMaker;

	//�۰���
	char* LevelMaker;

	//�ٹ���Ʈ ������
	char* AlbumArtMaker;

	//���̵�
	Difficulty difficulty;

	//������ ���� ��Ʈ�� ����
	int noteNum;

	//-> ������ʹ� ����� ����(Score) ����

	//�ְ�ũ
	Rank BestRank;

	//�ְ� ����
	int BestScore;

	//Ǯ�޺� ����
	bool isFullCombo;

	//perfect ����
	int PerfectNum;

	//good ����
	int GoodNum;
	
	//miss ����
	int MissNum;
};

typedef struct ItemData {
	//������ �̸�
	char* Name = {};

	//��������Ʈ ���̵�
	size_t SpriteID = 0;

	//�߰� ����ġ ����
	float Extra_Exp = 0;

	//�߰� �ִ� HP ����
	float Extra_Hp = 0;

	//�߰� ȸ���� ����
	float Extra_Heal = 0;

	//�߰� ���ط� ����
	float Extra_Damage = 0;

	//�߰� ������ӸӴ� ����
	float Extra_Money = 0;
};

typedef struct Challenge {
	char* ChallengeName;
	size_t SpriteID = 0;
	//�����
	shp::vec2f PrograssFlow = shp::vec2f(0, 1);

	// ������� Ȯ���ϴ� �Լ�
	float (*GetPrograssFlowFunc)() = nullptr;
};

//���� �������θ� ��������.
class PlayerData {
public:
	static constexpr int STRMAX = 128;

	//�÷��̾� �̸�
	char* Name;

	//���� ���ӸӴ�
	int GameMoney = 0;

	//�� ����
	int TotalScore = 0;

	//�� Ǯ�޺�
	int TotalFullCombo = 0;

	//������ �뷡���� ����
	int SaveLevelNum = 1;

	// ���� ������ ������
	int EquipItemID = 0;

	// ���� ������ Īȣ
	int EquipTitleID = 0;

	//�� ��ũ ����
	//�迭 ù��° �μ�
	// 0: Easy | 1 : Normal | 2 : Hard | 3 : extraspace | 4 : extraspace | 5 : extraspace
	//�迭 �ι�° �μ�
	// 0: F | 1 : E | 2 : D | 3 : C | 4 : B | 5 : A | 6 : S | 7 : M | 8 : extraspace
	int RankNum[6][9] = {};

	//���� �� ���� / ���� ����
	int lsd_up = 0;
	static constexpr int LSD_MAX = 100;
	LevelScoreData levelScoreData[LSD_MAX] = {};
	

	//���� �����۵��� ����
	int itemdata_up = 0;
	static constexpr int ITEM_MAX = 100;
	ItemData itemData[ITEM_MAX] = {};
	

	//Īȣ, ��������, �����
	int cd_up = 0;
	static constexpr int CHALLENGE_MAX = 100;
	Challenge challengeData[CHALLENGE_MAX] = {};
	
	//��������
	
	//��ũ
	float sync_time = 0;

	//�뷡����
	float music_Volume = 1;

	//Ÿ���� ����
	float Rim_Volume = 1;

	//ȿ���� ����
	float Effect_Volume = 1;

	//��Ʈ �������� �ӵ�
	float basicSpeed = 1;

	//�뷡����
	float Background_music_Volume = 1;

public:
	PlayerData();
	virtual ~PlayerData();

	void LoadData();
	void SaveData();
	void Clear();
};