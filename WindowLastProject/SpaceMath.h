#pragma once
#include <math.h>
#include <string>
#include <vector>

using namespace std;

namespace shp {
	
	const float PI = 3.141592f;

	//����ü��
	
	// ��
	class vec2f {
	public:
		float x;
		float y;

		vec2f() { x = 0; y = 0; }
		vec2f(const vec2f& ref) { x = ref.x; y = ref.y; }
		vec2f(float ix, float iy) { x = ix; y = iy; }

		vec2f operator+(vec2f v2) { return vec2f(x + v2.x, y + v2.y); }
		vec2f operator-(vec2f v2) { return vec2f(x - v2.x, y - v2.y); }
		vec2f operator/(float div) { return vec2f(x / div, y / div); }
		vec2f operator*(float div) { return vec2f(x * div, y * div); }
		bool operator==(const vec2f& v2) {
			if (x == v2.x && y == v2.y) return true;
			else return false;
		}
		bool operator!=(const vec2f& v2) {
			if (x != v2.x || y != v2.y) return true;
			else return false;
		}

		vec2f Inverse() const { return vec2f(-x, -y); }

		bool isActive() const {
			if ((isnan(x) == false && isinf(x) == false) && (isnan(y) == false && isinf(y) == false)) {
				return true;
			}
			else return false;
		}
	};

	// ����
	class straightLine {
	public:
		float xrate; // x ��ȭ��
		float yrate; // y ��ȭ��
		vec2f inDot; // �� ���� �ϳ��� ��.
		straightLine() { xrate = 0; yrate = 0; inDot = vec2f(); }
		straightLine(const straightLine& ref) { xrate = ref.xrate; yrate = ref.yrate; inDot = ref.inDot; }
		straightLine(float xr, float yr, vec2f indot) { xrate = xr; yrate = yr; inDot = indot; }
		straightLine(vec2f p1, vec2f p2) { xrate = p2.x - p1.x; yrate = p2.y - p1.y; inDot = vec2f(p1); }

		float GetYFromX(float x) const { return yrate * (x - inDot.x) / xrate + inDot.y; }
		float GetXFromY(float y) const { return xrate * (y - inDot.y) / yrate + inDot.x; }

		bool operator==(const straightLine& ref) {
			if (GetYFromX(0) == ref.GetYFromX(0) && GetXFromY(0) == ref.GetXFromY(0)) {
				return true;
			}
			else return false;
		}

		void turnRight() {
			float d = xrate;
			xrate = yrate;
			yrate = -d;
		}
	};

	// ���簢��
	class rect4f {
	private:
		void moveValue_tool1(char xy, float value, float mulminv, int plus) {
			float save;
			if (xy == 'x') {
				save = abs(lx - fx);
				fx = value - save * mulminv;
				lx = fx + save * plus;
			}
			else if (xy == 'y') {
				save = abs(ly - fy);
				fy = value - save * mulminv;
				ly = fy + save * plus;
			}
		}
	public:
		float fx;
		float fy;
		float lx;
		float ly;

		rect4f() { fx = 0; fy = 0; lx = 0; ly = 0; }
		rect4f(float FX, float FY, float LX, float LY) { fx = FX; fy = FY; lx = LX; ly = LY; }
		rect4f(const rect4f& ref) { fx = ref.fx; fy = ref.fy; lx = ref.lx; ly = ref.ly; }
		virtual ~rect4f() {}

		bool operator==(const rect4f& v2) {
			if ((v2.fx == fx && v2.fy == fy)
				&& v2.lx == lx && v2.ly == ly) return true;
			else return false;
		}

		vec2f getCenter() const { return (vec2f(fx, fy) + vec2f(lx, ly)) / 2; }
		float getw() const { return lx - fx; }
		float geth() const { return ly - fy; }

		void setCenter(const vec2f& pos) {
			float dx = abs(lx - fx);
			float dy = abs(ly - fy);
			fx = pos.x - dx / 2;
			fy = pos.y - dy / 2;
			lx = pos.x + dx / 2;
			ly = pos.y + dy / 2;
		}

		void moveValue(const string& name, float value) {
			float save;
			if (name.at(0) == 'f') {
				moveValue_tool1(name.at(1), value, 0, 1);
			}
			else if (name.at(0) == 'l') {
				moveValue_tool1(name.at(1), value, 0, -1);
				if (name.at(1) == 'x') {
					float a = lx;
					lx = fx;
					fx = a;
				}
				else {
					float a = ly;
					ly = fy;
					fy = a;
				}
			}
			else if (name.at(0) == 'c') {
				moveValue_tool1(name.at(1), value, 0.5f, 1);
			}
		}
	};

	// ���� - ����
	class angle2f {
	public:
		float dx;
		float dy;
		float radian;

		angle2f() { dx = 0; dy = 0; radian = 0; }
		angle2f(const angle2f& ref) { dx = ref.dx; dy = ref.dy; radian = ref.radian; }
		angle2f(float ix, float iy) { *this = usedxdy(ix, iy); }
		angle2f(float angle, bool radian) {
			if (radian) {
				*this = useRadian(angle);
			}
			else {
				*this = useNumAngle(angle);
			}
		}
		angle2f& operator= (const angle2f& ref)
		{
			dx = ref.dx;
			dy = ref.dy;
			radian = ref.radian;
			return *this;
		}

		angle2f operator+(const angle2f& v2) { 
			return useRadian(radian + v2.radian); 
		}
		angle2f operator-(const angle2f& v2) { return useRadian(radian - v2.radian); }
		//angle2f operator/(angle2f div) { return vec2f(x / div, y / div); }
		//angle2f operator*(angle2f div) { return vec2f(x * div, y * div); }
		bool operator==(const angle2f& v2) {
			if (radian == v2.radian) return true;
			else return false;
		}
		bool operator!=(const angle2f& v2) {
			if (radian != v2.radian) return true;
			else return false;
		}

		angle2f Inverse() const { return useRadian(radian + PI); }

		bool isActive() const {
			if ((isnan(dx) == false && isinf(dx) == false) && (isnan(dy) == false && isinf(dy) == false)) {
				return true;
			}
			else return false;
		}

		static angle2f useRadian(float radian);
		static angle2f usedxdy(float dx, float dy);
		static angle2f useNumAngle(float numangle);
	};

	// �ﰢ��
	class triangle3v {
	public:
		vec2f point[3];
		triangle3v() {
			for (int i = 0; i < 3; ++i) {
				point[i] = vec2f(0, 0);
			}
		}
		triangle3v(const triangle3v& ref) {
			for (int i = 0; i < 3; ++i) {
				point[i] = ref.point[i];
			}
		}
		triangle3v(vec2f v0, vec2f v1, vec2f v2) {
			point[0] = v0;
			point[1] = v1;
			point[2] = v2;
		}

		bool operator==(const triangle3v& v2) {
			int notconsider[3] = { 0, 0, 0 };
			int considerN = 0;
			for (int i = 0; i < 3; ++i) {
				for (int k = 0; k < 3; ++k) {
					bool out = false;
					for (int u = 0; u < considerN; ++u) {
						if (k == notconsider[u]) {
							out = true;
						}
					}
					if (out) {
						continue;
					}

					if (point[i] == v2.point[k]) {
						notconsider[considerN] = k;
						++considerN;
					}
				}
			}

			if (considerN >= 2) {
				return true;
			}
			else return false;
		}
		bool operator!=(const triangle3v& v2) {
			if (*this == v2) {
				return false;
			}
			else {
				return true;
			}
		}
	};

	// ���� ������ �Ÿ�
	float get_distance(vec2f v1, vec2f v2); 

	// �ϳ��� ���� �������� �� ���� �ش� ��ġ�� �ִ��� Ȯ��
	bool bPointInStraightRange(vec2f point, straightLine sl, vec2f dir); 

	// ���� ���� ���� ��ġ
	vec2f GetPointRangeInStraight(vec2f point, straightLine sl);

	// ���� ���� �ﰢ���� ��ġ : (0, 0)�̸� ��ġ�� ��.
	vec2f GetTriangleRangeInStraight(triangle3v tri, straightLine s1);

	// �μ��� ����
	vec2f GetCrossPoint(straightLine sl1, straightLine sl2);

	// �� ���� �߽��� �������� ȸ��
	vec2f GetRotatePos(vec2f center, vec2f pos, angle2f angle);

	// �� ������ ���� ������ �̷�� �ΰ��� ������ inpos�� ������ �ִ� ������ ��ȯ�Ѵ�.
	straightLine GetSameAngleLine(straightLine s1, straightLine s2, vec2f inpos); 

	// ���� �ﰢ�� ���� ��ġ�� �ִ��� ����
	bool bPointInTriangleRange(vec2f point, vec2f v1, vec2f v2, vec2f v3);

	// ���� �ﰢ�� ���� ��ġ�� �ִ��� ����
	bool bLineInTriangleRange(vec2f linep1, vec2f linep2, vec2f v1, vec2f v2, vec2f v3);

	// ���� �ٰ��� ���ο� �ִ��� ����
	bool bPointInPolygonRange(vec2f point, vector<vec2f> polygon);

	// �ﰢ���� �ٰ��� ���ο� �ִ��� ����
	bool bTriangleInPolygonRange(vector<vec2f> triangle, vector<vec2f> polygon);

	// ���� ���簢�� ���ο� �ִ��� ����
	bool bPointInRectRange(vec2f point, rect4f rt);

	// ���簢���� ���簢�� ���ο� �ִ��� ����
	bool bRectInRectRange(rect4f smallrt, rect4f bigrt, bool include_same, bool include_any_contact);

	// ����ٰ������� ���� �ﰢ������ ��ȯ
	vector<triangle3v> FreePolygonToTriangles(vector<vec2f> freepolygon);

	// �ﰢ���� ������ �ڸ��� ������ ������ ���� �ﰢ����� ��ȯ
	vector<triangle3v> TriangleCutStraightLine(triangle3v triangle, straightLine cutline, vec2f remaindir);
	
	// ���� �ﰢ���� ������ �ڸ��� ������ ������ ���� �ﰢ����� ��ȯ
	vector<triangle3v> TrianglesCutStraightLine(vector<triangle3v> triangles, straightLine cutline, vec2f remaindir);
	
	// �ٰ����� ���簢�� Ʋ�� ��� �ڸ���.
	vector<triangle3v> FreePolygonInRectRange(vector<vec2f> freepolygon, rect4f rt);
};