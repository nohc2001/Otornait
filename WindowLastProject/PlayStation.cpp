#include "PlayStation.h"

extern vector<Sprite*> SpriteData;

PlayStation::PlayStation()
{
	SetObjType(OBJ_TYPE::PlayStation);
	SetIsPlaying(true);
	SetAutoMod(false);
	SetTime(-1);
	SetScore(0);
	SetCombo(0);
	SetMaxCombo(0);
	SetTempo(130);
	SetRotPos(ROTPOS::bottom);
}

PlayStation::~PlayStation()
{
	if (noteArr != nullptr && HeapDebugClass::HeapDebug[noteArr] == true) {
		HeapDebugClass::HeapArrDelete<Note>(noteArr);
		noteArr = nullptr;
	}

	if (ShowArr != nullptr && HeapDebugClass::HeapDebug[ShowArr] == true) {
		HeapDebugClass::HeapArrDelete<Show>(ShowArr);
		ShowArr = nullptr;
	}

	if (NE != nullptr && HeapDebugClass::HeapDebug[NE] == true) {
		NE->Clear();
	}

	if (ComboNE != nullptr && HeapDebugClass::HeapDebug[ComboNE] == true) {
		ComboNE->Clear();
	}

	if (HeapDebugClass::HeapDebug[BackGround] == true) {
		HeapDebugClass::HeapArrDelete<Sprite>(BackGround);
		BackGround = nullptr;
	}
}

PlayStation* PlayStation::Init(const shp::rect4f& loc, const bool& autoplay, const int& layer)
{
	SetLocation(loc);
	SetAutoMod(autoplay);
	SetLayer(layer);
	pnW = GetPlayLoc().getw() / 12;
	musicplay = false;
	BackGround = HeapDebugClass::HeapNew<Sprite>()->Init(L"Image\\BackGround.png");
	return this;
}

void PlayStation::FirstInit()
{
	if (first && gm != nullptr) {
		shp::rect4f tloc = GetPlayLoc();
		tloc.fy = (tloc.ly + GetLocation().ly) / 2;
		tloc.ly = tloc.fy + tloc.getw() / 10;
		GameManager* GM = (GameManager*)gm;
		NE = HeapDebugClass::HeapNew<NumberEffect>()->Init(SpriteData[9], tloc.getCenter(),
			shp::vec2f(tloc.getw()/2, tloc.geth()/2), 1.5f, 5, 9);
		GM->AddObject((GameObject*)NE);

		tloc = GetPlayLoc();
		tloc.ly = (GetPlayLoc().fy + GetLocation().fy) / 2;
		tloc.fy = tloc.ly - tloc.getw() / 10;
		
		ComboNE = HeapDebugClass::HeapNew<NumberEffect>()->Init(SpriteData[9], tloc.getCenter(),
			shp::vec2f(tloc.getw() / 2, tloc.geth() / 2), 1.5f, 5, 9);
		GM->AddObject((GameObject*)ComboNE);

		shp::rect4f playloc = GetPlayLoc();
		float m = 0;
		float w = playloc.getw();
		float h = playloc.geth();
		if (rotation == ROTPOS::bottom) {
			for (int i = 0; i < 4; ++i) {
				PushNote[i][1] = shp::vec2f(
					playloc.fx + i * w / 4 + w / 8, playloc.ly + m);
			}
		}
		else if (rotation == ROTPOS::left) {
			for (int i = 0; i < 4; ++i) {
				PushNote[i][1] = shp::vec2f(
					playloc.fx - m, playloc.fy + i * h / 4 + h / 8);
			}
		}
		else if (rotation == ROTPOS::top) {
			for (int i = 0; i < 4; ++i) {
				PushNote[i][1] = shp::vec2f(
					playloc.lx - i * w / 4 - w / 8, playloc.fy - m);
			}
		}
		else if (rotation == ROTPOS::right) {
			for (int i = 0; i < 4; ++i) {
				PushNote[i][1] = shp::vec2f(
					playloc.lx + m, playloc.fy - i * h / 4 - h / 8);
			}
		}

		if (songSoundID != -1) {
			Music::Play(0, true);
			int ms = 0;
			Music::SetChannelPos(0, ms);
		}
		first = false;
	}
}

void PlayStation::SetIsPlaying(const bool& bplaying)
{
	if (enable) {
		isplaying = bplaying;
	}
}

const bool& PlayStation::GetIsPlaying() const
{
	if (enable) {
		return isplaying;
	}
	else {
		return false;
	}
}

void PlayStation::SetAutoMod(const bool& mod)
{
	if (enable) {
		autoplaying = mod;
	}
}

const bool& PlayStation::GetAutoMod() const
{
	if (enable) {
		return autoplaying;
	}
	else {
		return false;
	}
}

void PlayStation::SetTime(const float& t)
{
	if (enable) {
		Time = t;
	}
}

const float& PlayStation::GetTime() const
{
	if (enable) {
		return Time;
	}
	else {
		return 0;
	}
}

void PlayStation::SetScore(const int& score)
{
	if (enable) {
		Score = score;
	}
}

const int& PlayStation::GetScore() const
{
	if (enable) {
		return Score;
	}
	else {
		return 0;
	}
}

void PlayStation::SetCombo(const int& com)
{
	if (enable) {
		Combo = com;
	}
}

const int& PlayStation::GetCombo() const
{
	if (enable) {
		return Combo;
	}
	else {
		return 0;
	}
}

void PlayStation::SetMaxCombo(const int& maxcom)
{
	if (enable) {
		MaxCombo = maxcom;
	}
}

const int& PlayStation::GetMaxCombo() const
{
	if (enable) {
		return MaxCombo;
	}
	else {
		return 0;
	}
}

void PlayStation::SetTempo(const int& tempo)
{
	if (enable) {
		Tempo = tempo;
	}
}

const int& PlayStation::GetTempo() const
{
	if (enable) {
		return Tempo;
	}
	else {
		return 0;
	}
}

void PlayStation::SetRotPos(const ROTPOS& rot)
{
	if (enable) {
		rotation = rot;
	}
}

const ROTPOS& PlayStation::GetRotPos() const
{
	if (enable) {
		return rotation;
	}
	else {
		return (ROTPOS)0;
	}
}

void PlayStation::SetNextNoteNum(const int& num)
{
	if (enable) {
		nextNoteNum = num;
	}
}

const int& PlayStation::GetNextNoteNum() const
{
	if (enable) {
		return nextNoteNum;
	}
	else {
		return 0;
	}
}

void PlayStation::SetMaxNoteNum(const int& num)
{
	if (enable) {
		noteMAX = num;
	}
}

const int& PlayStation::GetMaxNoteNum() const
{
	if (enable) {
		return noteMAX;
	}
	else {
		return 0;
	}
}

void PlayStation::SetNextShowNum(const int& num)
{
	if (enable) {
		nextShowNum = num;
	}
}

const int& PlayStation::GetNextShowNum() const
{
	if (enable) {
		return nextShowNum;
	}
	else {
		return 0;
	}
}

void PlayStation::SetMaxShowNum(const int& num)
{
	if (enable) {
		showMAX = num;
	}
}

const int& PlayStation::GetMaxShowNum() const
{
	if (enable) {
		return showMAX;
	}
	else {
		return 0;
	}
}

void PlayStation::SetNote(const int& index, const Note& note)
{
	if (enable) {
		if (noteArr != nullptr && index < noteMAX) {
			noteArr[index] = note;
		}
	}
}

const Note& PlayStation::GetNote(const int& index) const
{
	Note zero;
	if (enable) {
		if (noteArr != nullptr && index < noteMAX) {
			return noteArr[index];
		}
		else return zero;
	}
	else {
		return zero;
	}
}

void PlayStation::SetShow(const int& index, const Show& show)
{
	if (enable) {
		if (ShowArr != nullptr && index < showMAX) {
			ShowArr[index] = show;
		}
	}
}

const Show& PlayStation::GetShow(const int& index) const
{
	Show zero;
	if (enable) {
		if (ShowArr != nullptr && index < showMAX) {
			return ShowArr[index];
		}
		else {
			return zero;
		}
	}
	else {
		return zero;
	}
}

const int& PlayStation::GetPGMNum(char state)
{
	if (state == 'p') {
		return perfectnum;
	}
	else if (state == 'g') {
		return goodnum;
	}
	else{
		return missnum;
	}
}

const int& PlayStation::GetSongSoundID()
{
	if (enable) {
		return songSoundID;
	}
	else return -1;
}

const shp::vec2f PlayStation::GetSPos(ROTPOS rot, int pos, const shp::rect4f& playloc)
{
	shp::vec2f spos;
	float pw = playloc.getw();
	float ph = playloc.geth();
	if (pos <= 3) {
		if (rot == ROTPOS::bottom) {
			spos = shp::vec2f(playloc.fx + pos * pw / 4 + pw / 8, playloc.fy);
		}
		else if (rot == ROTPOS::left) {
			spos = shp::vec2f(playloc.lx, playloc.fy + pos * ph / 4 + ph / 8);
		}
		else if (rot == ROTPOS::top) {
			spos = shp::vec2f(playloc.lx - pos * pw / 4 - pw / 8, playloc.ly);
		}
		else if (rot == ROTPOS::right) {
			spos = shp::vec2f(playloc.fx, playloc.ly - pos * ph / 4 - ph / 8);
		}
		return spos;
	}
	else {
		if (rot == ROTPOS::bottom) {
			spos = shp::vec2f(playloc.getCenter().x, playloc.fy);
		}
		else if (rot == ROTPOS::left) {
			spos = shp::vec2f(playloc.lx, playloc.getCenter().y);
		}
		else if (rot == ROTPOS::top) {
			spos = shp::vec2f(playloc.getCenter().x, playloc.ly);
		}
		else if (rot == ROTPOS::right) {
			spos = shp::vec2f(playloc.fx, playloc.getCenter().y);
		}
		return spos;
	}
}

const shp::vec2f PlayStation::GetEPos(ROTPOS rot, int pos, const shp::rect4f& playloc)
{
	shp::vec2f epos;
	float pw = playloc.getw();
	float ph = playloc.geth();
	if (pos <= 3) {
		if (rot == ROTPOS::bottom) {
			epos = shp::vec2f(playloc.fx + pos * pw / 4 + pw / 8, playloc.ly);
		}
		else if (rot == ROTPOS::left) {
			epos = shp::vec2f(playloc.fx, playloc.fy + pos * ph / 4 + ph / 8);
		}
		else if (rot == ROTPOS::top) {
			epos = shp::vec2f(playloc.lx - pos * pw / 4 - pw / 8, playloc.fy);
		}
		else if (rot == ROTPOS::right) {
			epos = shp::vec2f(playloc.lx, playloc.ly - pos * ph / 4 - ph / 8);
		}
		return epos;
	}
	else {
		if (rot == ROTPOS::bottom) {
			epos = shp::vec2f(playloc.getCenter().x, playloc.ly);
		}
		else if (rot == ROTPOS::left) {
			epos = shp::vec2f(playloc.fx, playloc.getCenter().y);
		}
		else if (rot == ROTPOS::top) {
			epos = shp::vec2f(playloc.getCenter().x, playloc.fy);
		}
		else if (rot == ROTPOS::right) {
			epos = shp::vec2f(playloc.lx, playloc.getCenter().y);
		}
		return epos;
	}
}

void PlayStation::Rotate(bool left)
{
	if (enable) {
		if (rotating) {
			if (rotateLeft) {
				if (rotation >= (ROTPOS)3) {
					rotation = ROTPOS::bottom;
				}
				else {
					rotation = (ROTPOS)((int)rotation + 1);
				}
			}
			else {
				if (rotation <= (ROTPOS)0) {
					rotation = ROTPOS::right;
				}
				else {
					rotation = (ROTPOS)((int)rotation - 1);
				}
			}
		}

		shp::rect4f playloc = GetPlayLoc();
		float w = playloc.getw();
		float h = playloc.geth();
		float m = 0;
		if (rotation == ROTPOS::bottom) {
			for (int i = 0; i < 4; ++i) {
				PushNote[i][0] = shp::vec2f(
					playloc.fx + i * w / 4 + w / 8, playloc.ly + m);
			}
		}
		else if (rotation == ROTPOS::left) {
			for (int i = 0; i < 4; ++i) {
				PushNote[i][0] = shp::vec2f(
					playloc.fx - m, playloc.fy + i * h / 4 + h / 8);
			}
		}
		else if (rotation == ROTPOS::top) {
			for (int i = 0; i < 4; ++i) {
				PushNote[i][0] = shp::vec2f(
					playloc.lx - i * w / 4 - w / 8, playloc.fy - m);
			}
		}
		else if (rotation == ROTPOS::right) {
			for (int i = 0; i < 4; ++i) {
				PushNote[i][0] = shp::vec2f(
					playloc.lx + m, playloc.ly - i * h / 4 - h / 8);
			}
		}

		ROTPOS nrot = GetNextROTPOS(rotation, left);

		if (nrot == ROTPOS::bottom) {
			for (int i = 0; i < 4; ++i) {
				PushNote[i][1] = shp::vec2f(
					playloc.fx + i * w / 4 + w / 8, playloc.ly + m);
			}
		}
		else if (nrot == ROTPOS::left) {
			for (int i = 0; i < 4; ++i) {
				PushNote[i][1] = shp::vec2f(
					playloc.fx - m, playloc.fy + i * h / 4 + h / 8);
			}
		}
		else if (nrot == ROTPOS::top) {
			for (int i = 0; i < 4; ++i) {
				PushNote[i][1] = shp::vec2f(
					playloc.lx - i * w / 4 - w / 8, playloc.fy - m);
			}
		}
		else if (nrot == ROTPOS::right) {
			for (int i = 0; i < 4; ++i) {
				PushNote[i][1] = shp::vec2f(
					playloc.lx + m, playloc.ly - i * h / 4 - h / 8);
			}
		}

		rotating = true;
		rotateLeft = left;
		RotateFlow.x = 0;
	}
}

ROTPOS PlayStation::GetNextROTPOS(ROTPOS rot, bool left)
{
	ROTPOS rr;
	if (left) {
		if (rot == ROTPOS::right) {
			rr = ROTPOS::bottom;
		}
		else {
			rr = (ROTPOS)((int)rot + 1);
		}
	}
	else {
		if (rot == ROTPOS::bottom) {
			rr = ROTPOS::right;
		}
		else {
			rr = (ROTPOS)((int)rot - 1);
		}
	}

	return rr;
}

void PlayStation::Rim()
{
	Music::PlayOnce(1);
	Music::SetChannelVolume(1, 0.4f);
}

void PlayStation::Hit(char pan, Note note)
{
	constexpr float degr = 0.7f;
	constexpr float power = 15;
	constexpr float eftime = 1.5f;

	if (pan == 'p') {
		SetCombo(GetCombo() + 1);
		if (GetCombo() > GetMaxCombo()) {
			SetMaxCombo(GetCombo());
		}

		if (note.ntype == NOTE_TYPE::LongNote) {
			Score += addScore/2;
		}
		else {
			Score += addScore;
		}
		
		//SetScore(GetScore() + addScore);
		perfectnum += 1;

		laststate = 'p';
		stateFlow.x = 0;

		shp::vec2f efpos = GetEPos(note.rotPos, note.pos, GetPlayLoc());
		float wr = GetPlayLoc().getw() / 4;
		GameManager* GM = (GameManager*)gm;

		
		if (note.rotPos == ROTPOS::bottom || note.rotPos == ROTPOS::top) {
			if (note.ntype < NOTE_TYPE::SpaceLeftNote) {
				GM->AddObject((GameObject*)HeapDebugClass::HeapNew<SizeEffect>()->Init(
					SpriteData[5], efpos, shp::vec2f(wr * degr, wr), eftime, power, 9));
			}
			else {
				GM->AddObject((GameObject*)HeapDebugClass::HeapNew<SizeEffect>()->Init(
					SpriteData[5], efpos, shp::vec2f(2 * wr, wr), eftime, power, 9));
			}
		}
		else {
			if (note.ntype < NOTE_TYPE::SpaceLeftNote) {
				GM->AddObject((GameObject*)HeapDebugClass::HeapNew<SizeEffect>()->Init(
					SpriteData[5], efpos, shp::vec2f(wr, wr * degr), eftime, power, 9));
			}
			else {
				GM->AddObject((GameObject*)HeapDebugClass::HeapNew<SizeEffect>()->Init(
					SpriteData[5], efpos, shp::vec2f(wr, 2 * wr), eftime, power, 9));
			}
		}

		int sc = (int)(1000000000 * (1.0f - powf(1.0f - (float)GetScore() / 1000000000.0f, 2)));
		NE->SetNumber(sc);
		ComboNE->SetNumber(GetCombo());

		if (note.ntype == NOTE_TYPE::SpaceLeftNote) {
			Rotate(true);
		}
		if (note.ntype == NOTE_TYPE::SpaceRightNote) {
			Rotate(false);
		}

		Impact += AddImpact;
		if (Impact > 1) Impact = 1;
		Rim();
	}
	else if (pan == 'g') {
		SetCombo(GetCombo() + 1);
		if (GetCombo() > GetMaxCombo()) {
			SetMaxCombo(GetCombo());
		}

		if (note.ntype == NOTE_TYPE::LongNote) {
			Score += addScore / 4;
		}
		else {
			Score += addScore / 2;
		}
		//SetScore(GetScore() + addScore / 2);

		goodnum += 1;

		laststate = 'g';
		stateFlow.x = 0;

		shp::vec2f efpos = GetEPos(note.rotPos, note.pos, GetPlayLoc());
		float wr = GetPlayLoc().getw() / 4;
		GameManager* GM = (GameManager*)gm;
		if (note.rotPos == ROTPOS::bottom || note.rotPos == ROTPOS::top) {
			if (note.ntype < NOTE_TYPE::SpaceLeftNote) {
				GM->AddObject((GameObject*)HeapDebugClass::HeapNew<SizeEffect>()->Init(
					SpriteData[5], efpos, shp::vec2f(wr * degr, wr), eftime, power, 9));
			}
			else {
				GM->AddObject((GameObject*)HeapDebugClass::HeapNew<SizeEffect>()->Init(
					SpriteData[5], efpos, shp::vec2f(2 * wr, wr), eftime, power, 9));
			}
		}
		else {
			if (note.ntype < NOTE_TYPE::SpaceLeftNote) {
				GM->AddObject((GameObject*)HeapDebugClass::HeapNew<SizeEffect>()->Init(
					SpriteData[5], efpos, shp::vec2f(wr, wr * degr), eftime, power, 9));
			}
			else {
				GM->AddObject((GameObject*)HeapDebugClass::HeapNew<SizeEffect>()->Init(
					SpriteData[5], efpos, shp::vec2f(wr, 2 * wr), eftime, power, 9));
			}
		}

		int sc = (int)(1000000000 * (1.0f - powf(1.0f - (float)GetScore() / 1000000000.0f, 2)));
		NE->SetNumber(sc);
		ComboNE->SetNumber(GetCombo());

		if (note.ntype == NOTE_TYPE::SpaceLeftNote) {
			Rotate(true);
		}
		if (note.ntype == NOTE_TYPE::SpaceRightNote) {
			Rotate(false);
		}

		Impact += AddImpact;
		if (Impact > 1) Impact = 1;
		Rim();
	}
	else if(pan == 'm') {
		SetCombo(0);

		missnum += 1;

		laststate = 'm';
		stateFlow.x = 0;

		ComboNE->SetNumber(GetCombo());

		if (note.ntype == NOTE_TYPE::SpaceLeftNote) {
			Rotate(true);
		}
		if (note.ntype == NOTE_TYPE::SpaceRightNote) {
			Rotate(false);
		}
	}
	
	if (note.ntype != NOTE_TYPE::LongNote && pan != 'm') {
		switch (note.pos) {
		case 0:
			keydata.SHit = true;
			break;
		case 1:
			keydata.DHit = true;
			break;
		case 2:
			keydata.KHit = true;
			break;
		case 3:
			keydata.LHit = true;
			break;
		case 4:
			keydata.SpaceHit = true;
			break;
		}
	}
	
}

void PlayStation::GoPosition(float position)
{
	if (position < 0) return;

	if (enable) {
		SetTime(position);
		nextNoteNum = 0;
		ROTPOS rot = ROTPOS::bottom;
		bool isrot = true;
		for (int i = 0; i < noteMAX; ++i) {
			if (noteArr[i].time >= position) {
				noteArr[i].enable = true;
				isrot = false;
			}
			else {
				nextNoteNum = i;
				noteArr[i].enable = false;
			}

			if (isrot) {
				if (noteArr[i].ntype == NOTE_TYPE::SpaceLeftNote) {
					rot = GetNextROTPOS(rot, true);
				}

				if (noteArr[i].ntype == NOTE_TYPE::SpaceRightNote) {
					rot = GetNextROTPOS(rot, false);
				}
			}
			
		}

		SetRotPos(rot);
		nextShowNum = 0;
		for (int i = 0; i < GetMaxShowNum(); ++i) {
			if (ShowArr[i].time > position) {
				ShowArr[i].enable = true;
			}
			else {
				ShowArr[i].enable = false;
				if (i < showMAX) {
					nextShowNum = i + 1;
				}
			}
		}

		shp::rect4f playloc = GetPlayLoc();
		float m = 0;
		float w = playloc.getw();
		float h = playloc.geth();
		if (rot == ROTPOS::bottom) {
			for (int i = 0; i < 4; ++i) {
				PushNote[i][1] = shp::vec2f(
					playloc.fx + i * w / 4 + w / 8, playloc.ly + m);
			}
		}
		else if (rot == ROTPOS::left) {
			for (int i = 0; i < 4; ++i) {
				PushNote[i][1] = shp::vec2f(
					playloc.fx - m, playloc.fy + i * h / 4 + h / 8);
			}
		}
		else if (rot == ROTPOS::top) {
			for (int i = 0; i < 4; ++i) {
				PushNote[i][1] = shp::vec2f(
					playloc.lx - i * w / 4 - w / 8, playloc.fy - m);
			}
		}
		else if (rot == ROTPOS::right) {
			for (int i = 0; i < 4; ++i) {
				PushNote[i][1] = shp::vec2f(
					playloc.lx + m, playloc.fy - i * h / 4 - h / 8);
			}
		}

		if (songSoundID != -1) {
			int ms = 1000 * position;
			Music::SetChannelPos(0, ms);
		}
	}
}

void PlayStation::LoadMusic(const char* musicName)
{
	if (songSoundID == -1) {
		//Music::ClearSound(0);
		int n = Music::AddSound(musicName, false, true);
		songSoundID = n;
		strcpy_s(songName, musicName);

		Music::ConnectSound(0, songSoundID);
		Music::SetChannelVolume(0, 0);
		musicplay = false;
	}
	else {
		//clear one sound
		//Music::ClearSound(songSoundID);
		int n = Music::AddSound(musicName, false, true);
		songSoundID = n;
		strcpy_s(songName, musicName);

		Music::ConnectSound(0, songSoundID);
		Music::SetChannelVolume(0, 0);
		musicplay = false;
	}
}

void PlayStation::SetBackGround(TCHAR* fileName)
{
	if (enable) {
		if (HeapDebugClass::HeapDebug[BackGround] == true) {
			HeapDebugClass::HeapDelete<Sprite>(BackGround);
			BackGround = nullptr;
		}
		BackGround = HeapDebugClass::HeapNew<Sprite>()->Init(fileName);
	}
}

void PlayStation::LoadData(const char* filename)
{
	if (enable) {
		Arrange();

		ifstream in;
		in.open(filename);

		int maxNoteNum = 0;
		in >> maxNoteNum;
		SetMaxNoteNum(maxNoteNum);

		in >> showMAX;

		if (noteArr != nullptr && HeapDebugClass::HeapDebug[noteArr] == true) {
			HeapDebugClass::HeapArrDelete<Note>(noteArr);
			noteArr = nullptr;
		}

		if (ShowArr != nullptr && HeapDebugClass::HeapDebug[ShowArr] == true) {
			HeapDebugClass::HeapArrDelete<Show>(ShowArr);
			ShowArr = nullptr;
		}

		maxNoteNum = GetMaxNoteNum();
		noteArr = HeapDebugClass::HeapArrNew<Note>(maxNoteNum);

		for (int i = 0; i < maxNoteNum; ++i) {
			in >> noteArr[i].time;
			int n = 0;
			in >> n;
			noteArr[i].ntype = (NOTE_TYPE)n;
			in >> n;
			noteArr[i].rotPos = (ROTPOS)n;
			in >> n;
			noteArr[i].pos = n;
			float f = 0;
			in >> f;
			noteArr[i].endtime = f;
		}

		ShowArr = HeapDebugClass::HeapArrNew<Show>(showMAX);

		for (int i = 0; i < showMAX; ++i) {
			in >> ShowArr[i].time;
			int st;
			in >> st;
			ShowArr[i].stype = (SHOW_TYPE)st;
			for (int pi = 0; pi < 10; ++pi) {
				in >> ShowArr[i].param[pi];
			}
		}

		Arrange();

		in.close();
	}
}

void PlayStation::SaveData(const char* filename)
{
	if (enable) {
		Arrange();

		ofstream out;
		out.open(filename);

		out << GetMaxNoteNum();
		out << '\n';

		out << showMAX;
		out << '\n';

		for (int i = 0; i < GetMaxNoteNum(); ++i) {
			Note n = GetNote(i);
			out << n.time;
			out << ' ';
			out << (int)n.ntype;
			out << ' ';
			out << (int)n.rotPos;
			out << ' ';
			out << (int)n.pos;
			out << ' ';
			out << (float)n.endtime;
			out << '\n';
		}

		for (int i = 0; i < showMAX; ++i) {
			Show n = GetShow(i);
			out << n.time;
			out << ' ';
			out << (int)n.stype;
			for (int pi = 0; pi < 10; ++pi) {
				out << ' ';
				out << (int)n.param[pi];
			}
			out << '\n';
			//out << ' ';
			//out << (int)n.pos;
			//out << ' ';
			//out << (float)n.endtime;
			//out << '\n';
		}

		out.close();

		Arrange();
	}
}

void PlayStation::LoadNotes(vector<Note> narr)
{
	if (noteArr != nullptr && HeapDebugClass::HeapDebug[noteArr] == true) {
		nextNoteNum = 0;
		noteMAX = 0;
		HeapDebugClass::HeapArrDelete<Note>(noteArr);
		noteArr = nullptr;
	}

	noteMAX = narr.size();
	noteArr = HeapDebugClass::HeapArrNew<Note>(noteMAX);
	
	for (int i = 0; i < narr.size(); ++i) {
		noteArr[i] = narr[i];
	}
}

void PlayStation::LoadShows(vector<Show> sarr)
{
	if (ShowArr != nullptr && HeapDebugClass::HeapDebug[ShowArr] == true) {
		nextShowNum = 0;
		showMAX = 0;
		HeapDebugClass::HeapArrDelete<Show>(ShowArr);
		ShowArr = nullptr;
	}

	showMAX = sarr.size();
	ShowArr = HeapDebugClass::HeapArrNew<Show>(showMAX);

	for (int i = 0; i < sarr.size(); ++i) {
		ShowArr[i] = sarr[i];
	}
}

void PlayStation::Arrange()
{
	if (enable) {
		if (noteArr != nullptr && HeapDebugClass::HeapDebug[noteArr] == true) {
			for (int i = 0; i < GetMaxNoteNum(); ++i) {
				for (int k = i+1; k < GetMaxNoteNum(); ++k) {
					if (GetNote(i).time > GetNote(k).time) {
						Note n = GetNote(i);
						SetNote(i, GetNote(k));
						SetNote(k, n);
					}
				}
			}
		}

		if (ShowArr != nullptr && HeapDebugClass::HeapDebug[ShowArr] == true) {
			for (int i = 0; i < GetMaxShowNum(); ++i) {
				for (int k = i+1; k < GetMaxShowNum(); ++k) {
					if (GetShow(i).time > GetShow(k).time) {
						Show n = GetShow(i);
						SetShow(i, GetShow(k));
						SetShow(k, n);
					}
				}
			}
		}

		
	}
}

void PlayStation::Update(const float& delta)
{
	FirstInit();
	if (enable) {
		if (musicplay == false && GetTime() > 0) {
			musicplay = true;
			Music::Play(0, true);
			Music::SetChannelPos(0, 1000 * GetTime());
			Music::SetChannelVolume(0, 1);
		}

		//if (songSoundID != -1) {
		//	unsigned int mspos = 0;
		//	Music::Channels[0]->getPosition(&mspos, FMOD_TIMEUNIT_MS);
		//	if (fabsf((float)mspos / 1000.0f - GetTime()) > 0.1f) {
		//		Music::Play(0, false);
		//		Music::SetChannelPos(0, GetTime() * 1000);
		//		Music::Play(0, true);
		//	}
		//}

		shp::rect4f playloc = GetPlayLoc();

		Impact -= delta;
		if (Impact < 0) Impact = 0;

		if (rotating) {
			RotateFlow.x += delta;
			if (RotateFlow.x > RotateFlow.y) {
				RotateFlow.x = RotateFlow.y;
				rotating = false;
				if (rotateLeft) {
					if (rotation >= (ROTPOS)3) {
						rotation = ROTPOS::bottom;
					}
					else {
						rotation = (ROTPOS)((int)rotation + 1);
					}
				}
				else {
					if (rotation <= (ROTPOS)0) {
						rotation = ROTPOS::right;
					}
					else {
						rotation = (ROTPOS)((int)rotation - 1);
					}
				}
			}
		}

		if (GetIsPlaying() == true) {
			SetTime(GetTime() + delta);

			if (GetMaxNoteNum() == 0) return;
			addScore = 1000000000 / (int)(GetMaxNoteNum()*1.23f);

			for (int i = GetNextNoteNum(); i < GetMaxNoteNum(); ++i) {
				Note note = GetNote(i);
				if (note.enable) {
					if (note.ntype != NOTE_TYPE::LongNote) {
						if (GetNote(i).time > GetTime() - missCloseValue) {
							SetNextNoteNum(i);
							break;
						}
					}
					else {
						if (GetNote(i).endtime < GetTime() - missCloseValue ||
							(GetNote(i).time < GetTime() - missCloseValue && isLongState[GetNote(i).pos] == false)) {
							Note note = GetNote(i);
							note.enable = false;
							isLongState[note.pos] = false;
							SetNote(i, note);

							SetCombo(0);

							Hit('m', GetNote(i));
						}

						if ((GetNote(i).time > GetTime() - missCloseValue) ||
							(GetNote(i).endtime > GetTime() - missCloseValue && isLongState[GetNote(i).pos])) {
							SetNextNoteNum(i);
							break;
						}
					}
				}
			}

			for (int i = 0; i < GetNextNoteNum(); ++i) {
				if (GetNote(i).enable) {
					//miss
					Note note = GetNote(i);
					note.enable = false;
					SetNote(i, note);

					SetCombo(0);

					missnum += 1;

					laststate = 'm';
					stateFlow.x = 0;

					ComboNE->SetNumber(GetCombo());

					if (note.ntype == NOTE_TYPE::SpaceLeftNote) {
						Rotate(true);
					}
					if (note.ntype == NOTE_TYPE::SpaceRightNote) {
						Rotate(false);
					}
					if (note.ntype == NOTE_TYPE::LongNote) {
						isLongState[note.pos] = false;
					}
				}
			}

			for (int i = 0; i < 5; ++i) {
				for (int k = 0; k < 100; ++k) {
					notelocArr[i][k] = -1;
				}
			}

			int flow[5] = {};
			for (int i = GetNextNoteNum(); i < GetMaxNoteNum(); ++i) {
				if (GetNote(i).enable == false) continue;

				int n = GetNote(i).pos;
				if (GetNote(i).ntype >= NOTE_TYPE::SpaceLeftNote) {
					if (flow[4] + 1 < readFutureNum) {
						notelocArr[4][flow[4]] = i;
						flow[4] += 1;
					}
					else {
						continue;
					}
				}
				else if (0 <= n && n <= 3) {
					if (flow[n] + 1 < readFutureNum) {
						notelocArr[n][flow[n]] = i;
						flow[n] += 1;
					}
					else {
						continue;
					}
				}

				bool out = true;
				for (int k = 0; k < 5; ++k) {
					if (flow[k] < readFutureNum) {
						out = false;
						break;
					}
				}

				if (out) {
					break;
				}
			}
			
			for (int i = 0; i < 5; ++i) {
				int n = notelocArr[i][0];
				if (n == -1) continue;
				if (GetNote(n).enable == false) continue;

				//회전중 회전된 결과 ROTPOS에서 노트를 칠 수 있게 함.
				if (GetNote(n).rotPos != rotation) {
					if (rotating) {
 						if (GetNextROTPOS(rotation, rotateLeft) == GetNote(n).rotPos) {
							
						}
						else {
							continue;
						}
					}
					else {
						continue;
					}
				}

				bool bin = fabsf(GetNote(n).time - GetTime()) < missCloseValue;
				bool bkeyd = false;
				bool bkeyu = false;
				bool bkeyHit = false;
				shp::vec2f key2;
				switch (i) {
				case 0:
					bkeyd = keydata.S_pressed;
					key2 = keydata.S_dut;
					bkeyHit = keydata.SHit;
					break;
				case 1:
					bkeyd = keydata.D_pressed;
					key2 = keydata.D_dut;
					bkeyHit = keydata.DHit;
					break;
				case 2:
					bkeyd = keydata.K_pressed;
					key2 = keydata.K_dut;
					bkeyHit = keydata.KHit;
					break;
				case 3:
					bkeyd = keydata.L_pressed;
					key2 = keydata.L_dut;
					bkeyHit = keydata.LHit;
					break;
				case 4:
					bkeyd = keydata.Space_pressed;
					key2 = keydata.Space_dut;
					bkeyHit = keydata.SpaceHit;
					break;
				}

				Note note = GetNote(n);

				if (fabsf(note.endtime - GetTime()) < 0.01f && note.ntype == NOTE_TYPE::LongNote) {
					isLongState[i] = false;
					note.enable = false;
					SetNote(n, note);

					Hit('p', note);
				}

				if (bkeyHit && note.ntype != NOTE_TYPE::LongNote) {
					continue;
				}

				if (fabsf(key2.x - GetTime()) > perfectCloseValue) {
					if (autoplaying == false) {
						if ((bin == false && bkeyd == false) && (GetNote(n).ntype == NOTE_TYPE::LongNote && isLongState[i])) {
							if (fabsf(note.endtime - GetTime()) < perfectCloseValue) {
								isLongState[i] = false;
								note.enable = false;
								SetNote(n, note);
								Hit('p', note);

								if (n == GetMaxNoteNum() - 1 && (GetPGMNum('g') + GetPGMNum('m') == 0)) {
									Score = 1000000000;
								}
							}
							else if (fabsf(note.endtime - GetTime()) < goodCloseValue) {
								isLongState[i] = false;
								note.enable = false;
								SetNote(n, note);
								Hit('g', note);
							}
							else {
								isLongState[i] = false;
								note.enable = false;
								SetNote(n, note);
								Hit('m', note);
							}
						}
					}
				}
				else {
					if ((bin && bkeyd) && GetNote(n).ntype != NOTE_TYPE::LongNote) {
						if (fabsf(GetNote(n).time - key2.x) < perfectCloseValue) {
							Note note = GetNote(n);
							note.enable = false;
							SetNote(n, note);

							Hit('p', note);
							if (n == GetMaxNoteNum() - 1 && (GetPGMNum('g') + GetPGMNum('m') == 0)) {
								Score = 1000000000;
							}
						}
						else if (fabsf(GetNote(n).time - key2.x) < goodCloseValue) {
							Note note = GetNote(n);
							note.enable = false;
							SetNote(n, note);

							Hit('g', note);
						}
						else if (fabsf(GetNote(n).time - key2.x) < missCloseValue) {
							Note note = GetNote(n);
							note.enable = false;
							SetNote(n, note);

							Hit('m', note);
						}
					}

					
					if ((bin && bkeyd) && (GetNote(n).ntype == NOTE_TYPE::LongNote && isLongState[i] == false)) {
						isLongState[i] = true;

						if (fabsf(note.time - key2.x) < perfectCloseValue) {
							Hit('p', note);
							if (n == GetMaxNoteNum() - 1 && (GetPGMNum('g') + GetPGMNum('m') == 0)) {
								Score = 1000000000;
							}
						}
						else if (fabsf(GetNote(n).time - key2.x) < goodCloseValue) {
							Hit('g', note);
						}
						else if (fabsf(GetNote(n).time - key2.x) < missCloseValue) {
							isLongState[note.pos] = false;
							note.enable = false;
							SetNote(n, note);

							SetCombo(0);

							Hit('m', note);
						}
					}

				}
			
				constexpr float degr = 0.7f;
				constexpr float power = 15;
				constexpr float eftime = 1.5f;

				if (autoplaying) {
					Note note = GetNote(n);
					if (fabsf(note.time - GetTime()) < 0.01f) {
						if (note.ntype != NOTE_TYPE::LongNote) {
							Note note = GetNote(n);
							note.enable = false;
							SetNote(n, note);

							SetCombo(GetCombo() + 1);
							if (GetCombo() > GetMaxCombo()) {
								SetMaxCombo(GetCombo());
							}

							Score += addScore;
							if (n == GetMaxNoteNum() - 1 && (GetPGMNum('g') + GetPGMNum('m') == 0)) {
								Score = 1000000000;
							}
							//SetScore(GetScore() + addScore);
							perfectnum += 1;

							laststate = 'p';
							stateFlow.x = 0;

							shp::vec2f efpos = GetEPos(note.rotPos, note.pos, GetPlayLoc());
							float wr = GetPlayLoc().getw() / 4;
							GameManager* GM = (GameManager*)gm;


							if (note.rotPos == ROTPOS::bottom || note.rotPos == ROTPOS::top) {
								if (note.ntype < NOTE_TYPE::SpaceLeftNote) {
									GM->AddObject((GameObject*)HeapDebugClass::HeapNew<SizeEffect>()->Init(
										SpriteData[5], efpos, shp::vec2f(wr * degr, wr), eftime, power, 9));
								}
								else {
									GM->AddObject((GameObject*)HeapDebugClass::HeapNew<SizeEffect>()->Init(
										SpriteData[5], efpos, shp::vec2f(2 * wr, wr), eftime, power, 9));
								}
							}
							else {
								if (note.ntype < NOTE_TYPE::SpaceLeftNote) {
									GM->AddObject((GameObject*)HeapDebugClass::HeapNew<SizeEffect>()->Init(
										SpriteData[5], efpos, shp::vec2f(wr, wr * degr), eftime, power, 9));
								}
								else {
									GM->AddObject((GameObject*)HeapDebugClass::HeapNew<SizeEffect>()->Init(
										SpriteData[5], efpos, shp::vec2f(wr, 2 * wr), eftime, power, 9));
								}
							}

							int sc = (int)(1000000000 * (1.0f - powf(1.0f - (float)GetScore() / 1000000000.0f, 2)));
							NE->SetNumber(sc);
							ComboNE->SetNumber(GetCombo());

							if (note.ntype == NOTE_TYPE::SpaceLeftNote) {
								Rotate(true);
							}
							if (note.ntype == NOTE_TYPE::SpaceRightNote) {
								Rotate(false);
							}

							Impact += AddImpact;
							if (Impact > 1) Impact = 1;
							Rim();
						}
						else {
							isLongState[i] = true;

							SetCombo(GetCombo() + 1);
							if (GetCombo() > GetMaxCombo()) {
								SetMaxCombo(GetCombo());
							}

							Score += addScore/2;
							if (n == GetMaxNoteNum() - 1 && (GetPGMNum('g') + GetPGMNum('m') == 0)) {
								Score = 1000000000;
							}
							//SetScore(GetScore() + addScore / 2);

							perfectnum += 1;

							laststate = 'p';
							stateFlow.x = 0;

							shp::vec2f efpos = GetEPos(note.rotPos, note.pos, GetPlayLoc());
							float wr = GetPlayLoc().getw() / 4;
							GameManager* GM = (GameManager*)gm;


							if (note.rotPos == ROTPOS::bottom || note.rotPos == ROTPOS::top) {
								if (note.ntype < NOTE_TYPE::SpaceLeftNote) {
									GM->AddObject((GameObject*)HeapDebugClass::HeapNew<SizeEffect>()->Init(
										SpriteData[5], efpos, shp::vec2f(wr * degr, wr), eftime, power, 9));
								}
								else {
									GM->AddObject((GameObject*)HeapDebugClass::HeapNew<SizeEffect>()->Init(
										SpriteData[5], efpos, shp::vec2f(2 * wr, wr), eftime, power, 9));
								}
							}
							else {
								if (note.ntype < NOTE_TYPE::SpaceLeftNote) {
									GM->AddObject((GameObject*)HeapDebugClass::HeapNew<SizeEffect>()->Init(
										SpriteData[5], efpos, shp::vec2f(wr, wr * degr), eftime, power, 9));
								}
								else {
									GM->AddObject((GameObject*)HeapDebugClass::HeapNew<SizeEffect>()->Init(
										SpriteData[5], efpos, shp::vec2f(wr, 2 * wr), eftime, power, 9));
								}
							}

							int sc = (int)(1000000000 * (1.0f - powf(1.0f - (float)GetScore() / 1000000000.0f, 2)));
							NE->SetNumber(sc);
							ComboNE->SetNumber(GetCombo());
							Impact += AddImpact / 2.0f;
							if (Impact > 1) Impact = 1;
							Rim();
						}
					}

					if (fabsf(note.endtime - GetTime()) < 0.01f && note.ntype == NOTE_TYPE::LongNote) {
						isLongState[i] = false;
						note.enable = false;
						SetNote(n, note);

						Hit('p', note);
						if (n == GetMaxNoteNum() - 1 && (GetPGMNum('g') + GetPGMNum('m') == 0)) {
							Score = 1000000000;
						}
					}
				}
			}

			float ftime = GetTime();
			if (nextShowNum < showMAX) {
				if (ShowArr[nextShowNum].enable && fabsf(ShowArr[nextShowNum].time - ftime) < 0.01f) {
					ShowArr[nextShowNum].Excute((GameManager*)gm, GetLocation());
					ShowArr[nextShowNum].enable = false;
					if (nextShowNum + 1 < showMAX) {
						nextShowNum += 1;
					}
				}

				if (ShowArr[nextShowNum].enable && ShowArr[nextShowNum].time < ftime) {
					ShowArr[nextShowNum].Excute((GameManager*)gm, GetLocation());
					ShowArr[nextShowNum].enable = false;
					if (nextShowNum + 1 < showMAX) {
						nextShowNum += 1;
					}
				}
			}
		}

		
		if (stateFlow.x < stateFlow.y) {
			stateFlow.x += delta;
		}
	}
}

void PlayStation::Event(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	if (enable) {
		if (iMessage == WM_KEYDOWN) {
			if (wParam == 'S') {
				keydata.S_pressed = true;
				keydata.S_dut.x = GetTime();
			}

			if (wParam == 'D') {
				keydata.D_pressed = true;
				keydata.D_dut.x = GetTime();
			}

			if (wParam == 'K') {
				keydata.K_pressed = true;
				keydata.K_dut.x = GetTime();
			}

			if (wParam == 'L') {
				keydata.L_pressed = true;
				keydata.L_dut.x = GetTime();
			}

			if (wParam == VK_SPACE) {
				keydata.Space_pressed = true;
				keydata.Space_dut.x = GetTime();
			}
		}

		if (iMessage == WM_KEYUP) {
			if (wParam == 'S') {
				keydata.S_pressed = false;
				keydata.S_dut.y = GetTime();
				keydata.SHit = false;
			}

			if (wParam == 'D') {
				keydata.D_pressed = false;
				keydata.D_dut.y = GetTime();
				keydata.DHit = false;
			}

			if (wParam == 'K') {
				keydata.K_pressed = false;
				keydata.K_dut.y = GetTime();
				keydata.KHit = false;
			}

			if (wParam == 'L') {
				keydata.L_pressed = false;
				keydata.L_dut.y = GetTime();
				keydata.LHit = false;
			}

			if (wParam == VK_SPACE) {
				keydata.Space_pressed = false;
				keydata.Space_dut.y = GetTime();
				keydata.SpaceHit = false;
			}
		}
	}
}

void PlayStation::DrawNote(HDC hdc, shp::rect4f playloc, Note note)
{
	shp::vec2f spos;
	shp::vec2f epos;
	Sprite* noteSprite = nullptr;
	shp::angle2f sprAngle;

	float pw = playloc.getw();
	float ph = playloc.geth();
	spos = GetSPos(note.rotPos, note.pos, playloc);
	epos = GetEPos(note.rotPos, note.pos, playloc);

	if (note.rotPos == ROTPOS::bottom) {
		sprAngle = shp::angle2f::usedxdy(1, 0);
	}
	else if (note.rotPos == ROTPOS::left) {
		sprAngle = shp::angle2f::usedxdy(0, -1);
	}
	else if (note.rotPos == ROTPOS::top) {
		sprAngle = shp::angle2f::usedxdy(-1, 0);
	}
	else if (note.rotPos == ROTPOS::right) {
		sprAngle = shp::angle2f::usedxdy(0, 1);
	}
	
	float nw = pw / 8;
	float nh = ph / 64;

	if (SpriteData.size() >= 5) {
		if (note.rotPos == ROTPOS::bottom || note.rotPos == ROTPOS::top) {
			if (note.ntype == NOTE_TYPE::NormalNote) {
				noteSprite = SpriteData[0];

				if (noteSprite != nullptr) {
					float f = (note.time - GetTime());
					shp::vec2f nowpos = spos + (epos - spos) *
						AnimClass::EaseIn(1.0f - (f / NoteVisibleLength), accelPower);
					
					//noteSprite->DrawSprite(hdc, nowpos.x - nw * noteWidthRate,
					//	nowpos.y - nh, 2 * nw * noteWidthRate, 2 * nh);
					noteSprite->DrawSprite(hdc, nowpos.x - nw * noteWidthRate,
						nowpos.y - nh, 2 * nw * noteWidthRate, 2 * nh);
					/*noteSprite->DrawTransfromFrame(hdc, 0, nowpos.x - nw * noteWidthRate,
						nowpos.y - nh, 2 * nw * noteWidthRate, 2 * nh, sprAngle.radian, nowpos);*/
				}
			}
			else if (note.ntype == NOTE_TYPE::LongNote) {
				noteSprite = SpriteData[1];

				if (noteSprite != nullptr) {
					float f = (note.time - GetTime());
					float fe = (note.endtime - GetTime());
					shp::vec2f nowpos = spos + (epos - spos) *
						AnimClass::EaseIn(1.0f - (f / NoteVisibleLength), accelPower);
					//noteSprite->DrawSprite(hdc, nowpos.x - nw * noteWidthRate,
					//	nowpos.y - nh, 2 * nw * noteWidthRate, 2 * nh);
					noteSprite->DrawTransfromFrame(hdc, 2, nowpos.x - nw * noteWidthRate,
						nowpos.y - nh, 2 * nw * noteWidthRate, 2 * nh, sprAngle.radian, nowpos);

					if (note.endtime > GetTime() + NoteVisibleLength) {
						shp::vec2f nowpose = spos + (epos - spos) *
							AnimClass::EaseIn(1.0f - (fe / NoteVisibleLength), accelPower);
						
						shp::rect4f conrt = shp::rect4f(nowpose.x - nw * noteWidthRate, playloc.fy,
							nowpose.x + nw * noteWidthRate, nowpos.y);

						if (conrt.geth() > 0 && (conrt.lx > 0 && conrt.fy > 0)) {
							noteSprite->DrawTransfromFrame(hdc, 1, conrt.fx,
								conrt.fy, conrt.getw(), conrt.geth(), sprAngle.radian, nowpose);
						}
					}
					else {
						shp::vec2f nowpose = spos + (epos - spos) *
							AnimClass::EaseIn(1.0f - (fe / NoteVisibleLength), accelPower);

						noteSprite->DrawTransfromFrame(hdc, 0, nowpose.x - nw * noteWidthRate,
							nowpose.y - nh, 2 * nw * noteWidthRate, 2 * nh, sprAngle.radian, nowpose);

						shp::rect4f conrt = shp::rect4f(nowpose.x - nw * noteWidthRate, nowpose.y,
							nowpose.x + nw * noteWidthRate, nowpos.y);

						if (conrt.geth() > 0 && (conrt.lx > 0 && conrt.fy > 0)) {
							noteSprite->DrawTransfromFrame(hdc, 1, conrt.fx,
								conrt.fy, conrt.getw(), conrt.geth(), sprAngle.radian, nowpose);
						}
					}
					
				}
			}
			else if (note.ntype == NOTE_TYPE::SpaceNote) {
				noteSprite = SpriteData[2];

				if (noteSprite != nullptr) {
					float f = (note.time - GetTime());
					shp::vec2f nowpos = spos + (epos - spos) *
						AnimClass::EaseIn(1.0f - (f / NoteVisibleLength), accelPower);
					noteSprite->DrawSprite(hdc, playloc.fx + nw * (1.0f - noteWidthRate),
						nowpos.y - 2 * nh, pw - 2 * nw * (1.0f - noteWidthRate), 4 * nh);
					
					/*noteSprite->DrawTransfromFrame(hdc, 0, playloc.fx + nw * (1.0f - noteWidthRate),
						nowpos.y - 2*nh, pw - 2 * nw * (1.0f - noteWidthRate), 4 * nh, sprAngle.radian, nowpos);*/
				}
			}
			else if (note.ntype == NOTE_TYPE::SpaceLeftNote) {
				noteSprite = SpriteData[3];
				if (noteSprite != nullptr) {
					float f = (note.time - GetTime());
					shp::vec2f nowpos = spos + (epos - spos) *
						AnimClass::EaseIn(1.0f - (f / NoteVisibleLength), accelPower);
					//noteSprite->DrawSprite(hdc, nowpos.x - nw * noteWidthRate,
					//	nowpos.y - nh, 2 * nw * noteWidthRate, 2 * nh);

					noteSprite->DrawSprite(hdc, playloc.fx + nw * (1.0f - noteWidthRate),
						nowpos.y - 3 * nh, pw - 2 * nw * (1.0f - noteWidthRate), 6 * nh);

					/*noteSprite->DrawTransfromFrame(hdc, 0, playloc.fx + nw * (1.0f - noteWidthRate),
						nowpos.y - 3*nh, pw - 2 * nw * (1.0f - noteWidthRate), 6 * nh, sprAngle.radian, nowpos);*/
				}
			}
			else if (note.ntype == NOTE_TYPE::SpaceRightNote) {
				noteSprite = SpriteData[4];
				if (noteSprite != nullptr) {
					float f = (note.time - GetTime());
					if (fabsf(f) > NoteVisibleLength) return;

					shp::vec2f nowpos = spos + (epos - spos) *
						AnimClass::EaseIn(1.0f - (f / NoteVisibleLength), accelPower);
					//noteSprite->DrawSprite(hdc, nowpos.x - nw * noteWidthRate,
					//	nowpos.y - nh, 2 * nw * noteWidthRate, 2 * nh);

					noteSprite->DrawSprite(hdc, playloc.fx + nw * (1.0f - noteWidthRate),
						nowpos.y - 3 * nh, pw - 2 * nw * (1.0f - noteWidthRate), 6 * nh);

					/*noteSprite->DrawTransfromFrame(hdc, 0, playloc.fx + nw * (1.0f - noteWidthRate),
						nowpos.y -3* nh, pw - 2 * nw * (1.0f - noteWidthRate), 6 * nh, sprAngle.radian, nowpos);*/
				}
			}
		}
		else {
			if (note.ntype == NOTE_TYPE::NormalNote) {
				noteSprite = SpriteData[37];

				if (noteSprite != nullptr) {
					float f = (note.time - GetTime());
					shp::vec2f nowpos = spos + (epos - spos) *
						AnimClass::EaseIn(1.0f - (f / NoteVisibleLength), accelPower);
					//noteSprite->DrawSprite(hdc, nowpos.x - nw * noteWidthRate,
					//	nowpos.y - nh, 2 * nw * noteWidthRate, 2 * nh);

					noteSprite->DrawSprite(hdc, nowpos.x - nh,
						nowpos.y - nw * noteWidthRate,  2 * nh, 2 * nw * noteWidthRate);
					/*noteSprite->DrawTransfromFrame(hdc, 0, nowpos.x - nw * noteWidthRate,
						nowpos.y - nh , 2 * nw * noteWidthRate, 2 * nh , sprAngle.radian, nowpos);*/
				}
			}
			else if (note.ntype == NOTE_TYPE::LongNote) {
				noteSprite = SpriteData[1];

				if (noteSprite != nullptr) {
					float f = (note.time - GetTime());
					float fe = (note.endtime - GetTime());
					shp::vec2f nowpos = spos + (epos - spos) *
						AnimClass::EaseIn(1.0f - (f / NoteVisibleLength), accelPower);
					//noteSprite->DrawSprite(hdc, nowpos.x - nw * noteWidthRate,
					//	nowpos.y - nh, 2 * nw * noteWidthRate, 2 * nh);
					noteSprite->DrawTransfromFrame(hdc, 0, nowpos.x - nw * noteWidthRate,
						nowpos.y - nh, 2 * nw * noteWidthRate, 2 * nh, sprAngle.radian, nowpos);

					shp::vec2f nowpose = spos + (epos - spos) *
						AnimClass::EaseIn(1.0f - (fe / NoteVisibleLength), accelPower);
					noteSprite->DrawTransfromFrame(hdc, 2, nowpose.x - nw * noteWidthRate,
						nowpose.y - nh, 2 * nw * noteWidthRate, 2 * nh, sprAngle.radian, nowpose);

					shp::rect4f conrt = shp::rect4f(nowpose.x, nowpose.y - nw * noteWidthRate* noteWidthRate,
						nowpos.x, nowpos.y + nw * noteWidthRate* noteWidthRate);

					if (conrt.geth() > 0 && (conrt.lx > 0 && conrt.fy > 0)) {
						noteSprite->DrawTransfromFrame(hdc, 1, conrt.fx - nh,
							conrt.fy, conrt.getw() + 2*nh, conrt.geth(), 0, nowpose);
					}
				}
			}
			else if (note.ntype == NOTE_TYPE::SpaceNote) {
				//noteSprite = SpriteData[2];

				//if (noteSprite != nullptr) {
				//	float f = (note.time - GetTime());
				//	shp::vec2f nowpos = spos + (epos - spos) *
				//		AnimClass::EaseIn(1.0f - (f / NoteVisibleLength), accelPower);
				//	//noteSprite->DrawSprite(hdc, nowpos.x - nw * noteWidthRate,
				//	//	nowpos.y - nh, 2 * nw * noteWidthRate, 2 * nh);
				//	noteSprite->DrawTransfromFrame(hdc, 0, nowpos.x - nw * 4 + nw * (1.0f - noteWidthRate),
				//		nowpos.y - 2*nh, pw - 2 * nw * (1.0f - noteWidthRate), 4 * nh, sprAngle.radian, nowpos);
				//}

				noteSprite = SpriteData[38];
				if (noteSprite != nullptr) {
					float f = (note.time - GetTime());
					shp::vec2f nowpos = spos + (epos - spos) *
						AnimClass::EaseIn(1.0f - (f / NoteVisibleLength), accelPower);

					noteSprite->DrawSprite(hdc, nowpos.x - 2 * nh, playloc.fy + nw * (1.0f - noteWidthRate),
						4 * nh, pw - 2 * nw * (1.0f - noteWidthRate));
					/*noteSprite->DrawTransfromFrame(hdc, 0, nowpos.x - nw * 4 + nw * (1.0f - noteWidthRate),
						nowpos.y - 3 * nh, pw - 2 * nw * (1.0f - noteWidthRate), 6 * nh, sprAngle.radian, nowpos);*/
				}
			}
			else if (note.ntype == NOTE_TYPE::SpaceLeftNote) {
				noteSprite = SpriteData[39];
				if (noteSprite != nullptr) {
					float f = (note.time - GetTime());
					shp::vec2f nowpos = spos + (epos - spos) *
						AnimClass::EaseIn(1.0f - (f / NoteVisibleLength), accelPower);
					noteSprite->DrawSprite(hdc, nowpos.x - 3 * nh, playloc.fy + nw * (1.0f - noteWidthRate),
						6 * nh, pw - 2 * nw * (1.0f - noteWidthRate));
					/*noteSprite->DrawTransfromFrame(hdc, 0, nowpos.x - nw * 4 + nw * (1.0f - noteWidthRate),
						nowpos.y -3* nh, pw - 2 * nw * (1.0f - noteWidthRate), 6 * nh, sprAngle.radian, nowpos);*/
				}
			}
			else if (note.ntype == NOTE_TYPE::SpaceRightNote) {
				noteSprite = SpriteData[40];
				if (noteSprite != nullptr) {
					float f = (note.time - GetTime());
					if (fabsf(f) > NoteVisibleLength) return;

					shp::vec2f nowpos = spos + (epos - spos) *
						AnimClass::EaseIn(1.0f - (f / NoteVisibleLength), accelPower);
					noteSprite->DrawSprite(hdc, nowpos.x - 3 * nh, playloc.fy + nw * (1.0f - noteWidthRate),
						6 * nh, pw - 2 * nw * (1.0f - noteWidthRate));
					/*noteSprite->DrawTransfromFrame(hdc, 0, nowpos.x - nw*4 + nw * (1.0f - noteWidthRate),
						nowpos.y - 3*nh, pw - 2 * nw * (1.0f - noteWidthRate), 6 * nh, sprAngle.radian, nowpos);*/
				}
			}
		}
	}
}

void PlayStation::Render(HDC hdc)
{
	if (enable) {
		shp::rect4f wloc = GetLocation();
		HPEN FramePen = CreatePen(PS_SOLID, 2, RGB(255, 255, 255));
		SelectObject(hdc, FramePen);
		//Rectangle(hdc, wloc.fx, wloc.fy, wloc.lx, wloc.ly);

		HPEN BackPen = CreatePen(PS_SOLID, 2, RGB(25, 25, 25));
		SelectObject(hdc, BackPen);
		shp::vec2f center = wloc.getCenter();
		float w = wloc.geth();
		shp::rect4f playloc = GetPlayLoc();

		Rectangle(hdc, wloc.fx, wloc.fy, wloc.lx, wloc.ly);
		HBRUSH BlackB = CreateSolidBrush(RGB(25, 25, 25));
		SelectObject(hdc, BlackB);
		Rectangle(hdc, wloc.fx, wloc.fy, wloc.lx, wloc.ly);

		float ww = wloc.geth() / 2 + (wloc.geth() / 4) * AnimClass::EaseIn(Impact, 5) / 2;
		float hh = wloc.geth() / 2 + (wloc.geth() / 4) * AnimClass::EaseIn(Impact, 5) / 2;
		Rectangle(hdc, wloc.getCenter().x - ww, wloc.getCenter().y - hh, wloc.getCenter().x + ww, wloc.getCenter().y + hh);
		DeleteObject(BlackB);
		BackGround->DrawSpriteAlphaBlend(hdc, wloc.getCenter().x - ww, wloc.getCenter().y - hh, 2*ww, 2*hh, 127 + 128 * Impact);

		SpriteData[12]->DrawSpriteAlphaBlend(hdc, playloc.fx, playloc.fy, playloc.getw(), playloc.geth(), 255 - 128 * Impact);
		SpriteData[13]->DrawSpriteAlphaBlend(hdc, playloc.fx, playloc.fy, playloc.getw(), playloc.geth(), 255 - 128 * Impact);
		SpriteData[14]->DrawSpriteAlphaBlend(hdc, playloc.fx, playloc.fy, playloc.getw(), playloc.geth(), 255 - 128 * Impact);

		HPEN DotPen = CreatePen(PS_DOT, 1, RGB(0, 0, 0));

		if (rotating) {
			float numangle = 0;
			float rate = AnimClass::EaseOut(RotateFlow.x / RotateFlow.y, RotatePower);

			if (rotateLeft) {
				//왼쪽 회전중
				numangle = shp::PI/2.0f;
				
			}
			else {
				//오른쪽 회전중
				numangle = shp::PI / 2.0f;
				rate = 1.0f - rate;
			}

			if (rotation == ROTPOS::bottom || rotation == ROTPOS::top) {
				if (rotateLeft) {
					SelectObject(hdc, FramePen);
				}
				else {
					SelectObject(hdc, DotPen);
				}

				for (float i = 1; i < 4; ++i) {
					float x = playloc.fx + i * playloc.getw() / 4;

					shp::vec2f p1 = shp::GetRotatePos(playloc.getCenter(), 
						shp::vec2f(x, playloc.fy), shp::angle2f::useRadian(numangle * rate));
					shp::vec2f p2 = shp::GetRotatePos(playloc.getCenter(),
						shp::vec2f(x, playloc.ly), shp::angle2f::useRadian(numangle * rate));

					MoveToEx(hdc, p1.x, p1.y, NULL);
					LineTo(hdc, p2.x, p2.y);
				}

				if (rotateLeft) {
					SelectObject(hdc, DotPen);
				}
				else {
					SelectObject(hdc, FramePen);
				}
				for (float i = 1; i < 4; ++i) {
					float y = playloc.fy + i * playloc.geth() / 4;

					shp::vec2f p1 = shp::GetRotatePos(playloc.getCenter(),
						shp::vec2f(playloc.fx, y), shp::angle2f::useRadian(numangle * rate));
					shp::vec2f p2 = shp::GetRotatePos(playloc.getCenter(),
						shp::vec2f(playloc.lx, y), shp::angle2f::useRadian(numangle * rate));

					
					MoveToEx(hdc, p1.x, p1.y, NULL);
					LineTo(hdc, p2.x, p2.y);
				}
			}
			else {
				if (rotateLeft) {
					SelectObject(hdc, DotPen);
				}
				else {
					SelectObject(hdc, FramePen);
				}

				for (float i = 1; i < 4; ++i) {
					float x = playloc.fx + i * playloc.getw() / 4;

					shp::vec2f p1 = shp::GetRotatePos(playloc.getCenter(),
						shp::vec2f(x, playloc.fy), shp::angle2f::useRadian(numangle * rate));
					shp::vec2f p2 = shp::GetRotatePos(playloc.getCenter(),
						shp::vec2f(x, playloc.ly), shp::angle2f::useRadian(numangle * rate));

					MoveToEx(hdc, p1.x, p1.y, NULL);
					LineTo(hdc, p2.x, p2.y);
				}

				if (rotateLeft) {
					SelectObject(hdc, FramePen);
				}
				else {
					SelectObject(hdc, DotPen);
				}
				
				for (float i = 1; i < 4; ++i) {
					float y = playloc.fy + i * playloc.geth() / 4;

					shp::vec2f p1 = shp::GetRotatePos(playloc.getCenter(),
						shp::vec2f(playloc.fx, y), shp::angle2f::useRadian(numangle * rate));
					shp::vec2f p2 = shp::GetRotatePos(playloc.getCenter(),
						shp::vec2f(playloc.lx, y), shp::angle2f::useRadian(numangle * rate));

					MoveToEx(hdc, p1.x, p1.y, NULL);
					LineTo(hdc, p2.x, p2.y);
				}

			}

			rate = AnimClass::EaseOut(RotateFlow.x / RotateFlow.y, RotatePower);
			for (int i = 0; i < 4; ++i) {
				if (GetNextROTPOS(rotation, rotateLeft) == ROTPOS::bottom) {
					int a = 0;
					a += 1;
				}
				shp::vec2f mdelta = (PushNote[i][1] - PushNote[i][0]);
				shp::vec2f pnpos = PushNote[i][0] + mdelta * rate;
				int frame = 0;
				switch (i) {
				case 0:
					if (keydata.S_pressed) {
						frame = 1;
					}
					break;
				case 1:
					if (keydata.D_pressed) {
						frame = 1;
					}
					break;
				case 2:
					if (keydata.K_pressed) {
						frame = 1;
					}
					break;
				case 3:
					if (keydata.L_pressed) {
						frame = 1;
					}
					break;
				}

				if (keydata.Space_pressed) {
					frame = 1;
				}

				float ww = pnW;
				SpriteData[10]->DrawFrame(hdc, frame, pnpos.x - ww, pnpos.y - ww, 2 * ww, 2 * ww);
			}
		}
		else {
			if (rotation == ROTPOS::bottom || rotation == ROTPOS::top) {
				SelectObject(hdc, FramePen);
				for (float i = 1; i < 4; ++i) {
					float x = playloc.fx + i * playloc.getw() / 4;
					MoveToEx(hdc, x, playloc.fy, NULL);
					LineTo(hdc, x, playloc.ly);
				}

				SelectObject(hdc, DotPen);
				for (float i = 1; i < 4; ++i) {
					float y = playloc.fy + i * playloc.geth() / 4;
					MoveToEx(hdc, playloc.fx, y, NULL);
					LineTo(hdc, playloc.lx, y);
				}
			}
			else {
				SelectObject(hdc, DotPen);
				for (float i = 1; i < 4; ++i) {
					float x = playloc.fx + i * playloc.getw() / 4;
					MoveToEx(hdc, x, playloc.fy, NULL);
					LineTo(hdc, x, playloc.ly);
				}
				

				SelectObject(hdc, FramePen);
				for (float i = 1; i < 4; ++i) {
					float y = playloc.fy + i * playloc.geth() / 4;
					MoveToEx(hdc, playloc.fx, y, NULL);
					LineTo(hdc, playloc.lx, y);
				}
				
			}

			for (int i = 0; i < 4; ++i) {
				shp::vec2f pnpos = PushNote[i][1];
				int frame = 0;
				switch (i) {
				case 0:
					if (keydata.S_pressed) {
						frame = 1;
					}
					break;
				case 1:
					if (keydata.D_pressed) {
						frame = 1;
					}
					break;
				case 2:
					if (keydata.K_pressed) {
						frame = 1;
					}
					break;
				case 3:
					if (keydata.L_pressed) {
						frame = 1;
					}
					break;
				}

				if (keydata.Space_pressed) {
					frame = 1;
				}

				float ww = pnW;
				SpriteData[10]->DrawFrame(hdc, frame, pnpos.x - ww, pnpos.y - ww, 2 * ww, 2 * ww);
			}
		}

		DeleteObject(FramePen);
		DeleteObject(DotPen);
		DeleteObject(BlackB);
		DeleteObject(BackPen);

		for (int rot = 0; rot < 5; ++rot) {
			for (int rfn = 0; rfn < readFutureNum; ++rfn) {
				int n = notelocArr[rot][rfn];
				if (n != -1) {
					Note note = GetNote(n);
					if (note.ntype == NOTE_TYPE::LongNote) {
						DrawNote(hdc, playloc, note);
					}
				}
			}
		}

		for (int rot = 0; rot < 5; ++rot) {
			for (int rfn = 0; rfn < readFutureNum; ++rfn) {
				int n = notelocArr[rot][rfn];
				if (n != -1) {
					Note note = GetNote(n);
					if (note.ntype >= NOTE_TYPE::SpaceLeftNote) {
						DrawNote(hdc, playloc, note);
					}
				}
			}
		}

		for (int rot = 0; rot < 5; ++rot) {
			for (int rfn = 0; rfn < readFutureNum; ++rfn) {
				int n = notelocArr[rot][rfn];
				if (n != -1) {
					Note note = GetNote(n);
					if (note.ntype == NOTE_TYPE::NormalNote) {
						DrawNote(hdc, playloc, note);
					}
				}
			}
		}

		float wr = playloc.getw() / 4;
		float sw = wr + wr/2 * (1.0f - AnimClass::EaseOut(stateFlow.x / stateFlow.y, statePow));
		float sh = wr/4 + (wr/8) * (1.0f - AnimClass::EaseOut(stateFlow.x / stateFlow.y, statePow));
		shp::vec2f cen = playloc.getCenter() + shp::vec2f(0, 1.2f*wr);
		shp::rect4f ShowStateLoc = shp::rect4f(cen.x - sw, cen.y - sh, cen.x + sw, cen.y + sh);
		Sprite* StateSprite = nullptr;
		switch (laststate) {
		case 'p':
			StateSprite = SpriteData[6];
			break;
		case 'g':
			StateSprite = SpriteData[7];
			break;
		case 'm':
			StateSprite = SpriteData[8];
			break;
		}

		if (StateSprite != nullptr) {
			StateSprite->DrawSpriteAlphaBlend(hdc, ShowStateLoc.fx, ShowStateLoc.fy,
				ShowStateLoc.getw(), ShowStateLoc.geth(), 255 - 255 * 0.7f * (stateFlow.x / stateFlow.y));
		}
		
	}
}
