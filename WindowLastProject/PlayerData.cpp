#include "PlayerData.h"

using namespace std;

PlayerData::PlayerData()
{
}

PlayerData::~PlayerData()
{
	HeapDebugClass::HeapArrDelete<char>(Name);

	for (int i = 0; i < lsd_up; ++i) {
		HeapDebugClass::HeapArrDelete<char>(levelScoreData[i].SongName);
		HeapDebugClass::HeapArrDelete<char>(levelScoreData[i].SongMaker);
		HeapDebugClass::HeapArrDelete<char>(levelScoreData[i].LevelMaker);
		HeapDebugClass::HeapArrDelete<char>(levelScoreData[i].AlbumArtMaker);
	}

	for (int i = 0; i < itemdata_up; ++i) {
		HeapDebugClass::HeapArrDelete<char>(itemData[i].Name);
	}

	for (int i = 0; i < cd_up; ++i) {
		HeapDebugClass::HeapArrDelete<char>(challengeData[i].ChallengeName);
	}
}

void PlayerData::LoadData()
{
	ifstream in;
	in.open("Data\\Player.txt");

	if (HeapDebugClass::HeapDebug[Name] == false) {
		Name = HeapDebugClass::HeapArrNew<char>(STRMAX);
	}
	in >> Name;

	in >> GameMoney;

	in >> TotalScore;

	in >> TotalFullCombo;

	in >> SaveLevelNum;

	in >> EquipItemID;

	in >> EquipTitleID;

	for (int d = 0; d < 6; ++d) {
		for (int r = 0; r < 9; ++r) {
			in >> RankNum[d][r];
		}
	}

	in >> lsd_up;
	for (int i = 0; i < lsd_up; ++i) {
		if (HeapDebugClass::HeapDebug[levelScoreData[i].SongName] == false) {
			levelScoreData[i].SongName = HeapDebugClass::HeapArrNew<char>(STRMAX);
		}
		in >> levelScoreData[i].SongName;
		
		in >> levelScoreData[i].BPM_Tempo;
		
		in >> levelScoreData[i].songTime;

		if (HeapDebugClass::HeapDebug[levelScoreData[i].SongMaker] == false) {
			levelScoreData[i].SongMaker = HeapDebugClass::HeapArrNew<char>(STRMAX);
		}
		in >> levelScoreData[i].SongMaker;

		if (HeapDebugClass::HeapDebug[levelScoreData[i].LevelMaker] == false) {
			levelScoreData[i].LevelMaker = HeapDebugClass::HeapArrNew<char>(STRMAX);
		}
		in >> levelScoreData[i].LevelMaker;
		
		if (HeapDebugClass::HeapDebug[levelScoreData[i].AlbumArtMaker] == false) {
			levelScoreData[i].AlbumArtMaker = HeapDebugClass::HeapArrNew<char>(STRMAX);
		}
		in >> levelScoreData[i].AlbumArtMaker;

		int d = 0;
		in >> d;
		levelScoreData[i].difficulty = (Difficulty)d;

		in >> levelScoreData[i].noteNum;

		int r = 0;
		in >> r;
		levelScoreData[i].BestRank = (Rank)r;

		in >> levelScoreData[i].BestScore;
		in >> levelScoreData[i].isFullCombo;
		in >> levelScoreData[i].PerfectNum;
		in >> levelScoreData[i].GoodNum;
		in >> levelScoreData[i].MissNum;
	}

	in >> itemdata_up;
	for (int i = 0; i < itemdata_up; ++i) {
		if (HeapDebugClass::HeapDebug[itemData[i].Name] == false) {
			itemData[i].Name = HeapDebugClass::HeapArrNew<char>(STRMAX);
		}
		in >> itemData[i].Name;

		in >> itemData[i].SpriteID;

		in >> itemData[i].Extra_Exp;

		in >> itemData[i].Extra_Hp;

		in >> itemData[i].Extra_Heal;

		in >> itemData[i].Extra_Damage;

		in >> itemData[i].Extra_Money;
	}

	in >> cd_up;
	for (int i = 0; i < cd_up; ++i) {
		if (HeapDebugClass::HeapDebug[challengeData[i].ChallengeName] == false) {
			challengeData[i].ChallengeName = HeapDebugClass::HeapArrNew<char>(STRMAX);
		}
		in >> challengeData[i].ChallengeName;

		in >> challengeData[i].SpriteID;

		in >> challengeData[i].PrograssFlow.x;

		in >> challengeData[i].PrograssFlow.y;
	}

	in >> sync_time;
	in >> music_Volume;
	in >> Rim_Volume;
	in >> Effect_Volume;
	in >> basicSpeed;
	in >> Background_music_Volume;

	in.close();
}

void PlayerData::SaveData()
{
	ofstream out;
	out.open("Data\\Player.txt");

	out << Name << '\n';
	out << GameMoney << '\n';
	out << TotalScore << '\n';
	out << TotalFullCombo << '\n';
	out << SaveLevelNum << '\n';
	out << EquipItemID << '\n';
	out << EquipTitleID << '\n';

	for (int d = 0; d < 6; ++d) {
		for (int r = 0; r < 9; ++r) {
			out << RankNum[d][r] << ' ';
		}
	}
	out << '\n';

	out << lsd_up << '\n';
	for (int i = 0; i < lsd_up; ++i) {
		out << levelScoreData[i].SongName << ' ';
		out << levelScoreData[i].BPM_Tempo << ' ';
		out << levelScoreData[i].songTime << ' ';
		out << levelScoreData[i].SongMaker << ' ';
		out << levelScoreData[i].LevelMaker << ' ';
		out << levelScoreData[i].AlbumArtMaker << ' ';

		int d = (int)levelScoreData[i].difficulty;
		out << d << ' ';

		out << levelScoreData[i].noteNum << ' ';

		int r = (int)levelScoreData[i].BestRank;
		out << r << ' ';

		out << levelScoreData[i].BestScore << ' ';
		out << levelScoreData[i].isFullCombo << ' ';
		out << levelScoreData[i].PerfectNum << ' ';
		out << levelScoreData[i].GoodNum << ' ';
		out << levelScoreData[i].MissNum << '\n';
	}

	out << itemdata_up << '\n';
	for (int i = 0; i < itemdata_up; ++i) {
		out << itemData[i].Name << ' ';
		out << itemData[i].SpriteID << ' ';
		out << itemData[i].Extra_Exp << ' ';
		out << itemData[i].Extra_Hp << ' ';
		out << itemData[i].Extra_Heal << ' ';
		out << itemData[i].Extra_Damage << ' ';
		out << itemData[i].Extra_Money << '\n';
	}

	out << cd_up << '\n';
	for (int i = 0; i < cd_up; ++i) {
		out << challengeData[i].ChallengeName << ' ';
		out << challengeData[i].SpriteID << ' ';
		out << challengeData[i].PrograssFlow.x << ' ';
		out << challengeData[i].PrograssFlow.y << '\n';
	}

	out << sync_time << '\n';
	out << music_Volume << '\n';
	out << Rim_Volume << '\n';
	out << Effect_Volume << '\n';
	out << basicSpeed << '\n';
	out << Background_music_Volume << '\n';

	out.close();
}

void PlayerData::Clear()
{
	HeapDebugClass::HeapArrDelete<char>(Name);

	for (int i = 0; i < lsd_up; ++i) {
		HeapDebugClass::HeapArrDelete<char>(levelScoreData[i].SongName);
		HeapDebugClass::HeapArrDelete<char>(levelScoreData[i].SongMaker);
		HeapDebugClass::HeapArrDelete<char>(levelScoreData[i].LevelMaker);
		HeapDebugClass::HeapArrDelete<char>(levelScoreData[i].AlbumArtMaker);
	}

	for (int i = 0; i < itemdata_up; ++i) {
		HeapDebugClass::HeapArrDelete<char>(itemData[i].Name);
	}

	for (int i = 0; i < cd_up; ++i) {
		HeapDebugClass::HeapArrDelete<char>(challengeData[i].ChallengeName);
	}
}
