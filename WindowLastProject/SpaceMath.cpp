#include "SpaceMath.h"
#include <list>

float shp::get_distance(vec2f v1, vec2f v2) {
	float delta[2];
	delta[0] = v2.x - v1.x;
	delta[1] = v2.y - v1.y;
	return sqrtf(delta[0] * delta[0] + delta[1] * delta[1]);
}

shp::angle2f shp::angle2f::useRadian(float radian)
{
	angle2f a = angle2f();
	if (radian > 2.0f * PI) {
		int a = radian / (2.0f * PI);
		radian = radian - 2.0f * (float)a * PI;
	}
	if (radian < 0) {
		int a = -radian / (2.0f * PI);
		radian = ((float)(a + 1) * 2.0f * PI) - radian;
	}
	a.radian = radian;
	a.dx = cosf(radian);
	a.dy = sinf(radian);
	return a;
}

shp::angle2f shp::angle2f::useNumAngle(float numangle) {
	return useRadian(shp::PI * numangle / 180.0f);
}

shp::angle2f shp::angle2f::usedxdy(float dx, float dy)
{
	angle2f a = angle2f();
	float l = get_distance(vec2f(0, 0), vec2f(dx, dy));
	dx = dx / l;
	dy = dy / l;
	float angle = (asinf(fabsf(dy)) + acosf(fabsf(dx))) / 2.0f;

	if (dy > 0) {
		if (dx > 0) {
			a.radian = angle;
		}
		else {
			a.radian = PI - angle;
		}
	}
	else {
		if (dx > 0) {
			a.radian = 2 * PI - angle;
		}
		else {
			a.radian = PI + angle;
		}
	}
	a.dx = dx;
	a.dy = dy;
	return a;
}

bool shp::bPointInStraightRange(vec2f point, straightLine sl, vec2f dir)
{
	float x2 = sl.xrate * (point.y - sl.inDot.y) / sl.yrate + sl.inDot.x;
	float y2 = sl.yrate * (point.x - sl.inDot.x) / sl.xrate + sl.inDot.y;
	x2 = x2 - point.x;
	y2 = y2 - point.y;
	if (x2 * dir.x > 0 && y2 * dir.y > 0)
		return true;
	else return false;
}

bool shp::bPointInTriangleRange(vec2f point, vec2f v1, vec2f v2, vec2f v3)
{
	int k = 0;
	straightLine l = straightLine(v1, v2);
	vec2f dir1 = GetPointRangeInStraight(v3, l);
	vec2f dir2 = GetPointRangeInStraight(point, l);
	if (dir1 == dir2) k++;
	else return false;

	l = straightLine(v1, v3);
	dir1 = GetPointRangeInStraight(v2, l);
	dir2 = GetPointRangeInStraight(point, l);
	if (dir1 == dir2) k++;
	else return false;

	l = straightLine(v2, v3);
	dir1 = GetPointRangeInStraight(v1, l);
	dir2 = GetPointRangeInStraight(point, l);
	if (dir1 == dir2) k++;
	else return false;

	if (k == 3) return true;
	else return false;
}

bool shp::bLineInTriangleRange(vec2f linep1, vec2f linep2, vec2f v1, vec2f v2, vec2f v3) {
	straightLine l = straightLine(linep1, linep2);
	vec2f cross[3];
	int k = 0;

	//v의 선중 같은 선분이 있을 경우 false 함

	cross[k] = GetCrossPoint(l, straightLine(v1, v2));
	if (cross[k].isActive()) {
		if (((v1.x <= cross[k].x && cross[k].x <= v2.x) || (v2.x <= cross[k].x && cross[k].x <= v1.x))
			|| ((v1.y <= cross[k].y && cross[k].y <= v2.y) || (v2.y <= cross[k].y && cross[k].y <= v1.y))) {
			k++;
		}
	}

	cross[k] = GetCrossPoint(l, straightLine(v1, v3));
	if (cross[k].isActive()) {
		if (((v1.x <= cross[k].x && cross[k].x <= v3.x) || (v3.x <= cross[k].x && cross[k].x <= v1.x))
			|| ((v1.y <= cross[k].y && cross[k].y <= v3.y) || (v3.y <= cross[k].y && cross[k].y <= v1.y))) {
			k++;
		}
	}

	cross[k] = GetCrossPoint(l, straightLine(v2, v3));
	if (cross[k].isActive()) {
		if (((v2.x <= cross[k].x && cross[k].x <= v3.x) || (v3.x <= cross[k].x && cross[k].x <= v2.x))
			|| ((v2.y <= cross[k].y && cross[k].y <= v3.y) || (v3.y <= cross[k].y && cross[k].y <= v2.y))) {
			k++;
		}
	}

	if (k >= 2) {
		for (int i = 0; i < k; i++) {
			for (int j = 0; j < k; i++) {
				if (j == i) continue;
				if (cross[i] != cross[j]) return true;
			}
		}
		return false;
	}
	else return false;
}

bool shp::bPointInPolygonRange(vec2f point, vector<vec2f> polygon) {
	int h = 0;

	for (int o = 0; o < polygon.size(); o++) {
		vec2f p1;
		vec2f p2;
		if (o == polygon.size() - 1) {
			p1 = polygon.at(o);
			p2 = polygon.at(0);
		}
		else {
			p1 = polygon.at(o);
			p2 = polygon.at(o + 1);
		}

		straightLine sl = straightLine(p1, p2);
		float xx = sl.GetXFromY(point.y);
		if (point.x < xx &&
			((p1.y < point.y && point.y < p2.y) || (p2.y < point.y && point.y < p1.y))) {
			h++;
		}
	}

	if (h % 2 == 1) {
		return true;
	}
	else return false;
}

bool shp::bTriangleInPolygonRange(vector<vec2f> triangle, vector<vec2f> polygon)
{
	if (triangle.size() != 3) return false;
	vec2f gcenter = (triangle.at(0) + triangle.at(1) + triangle.at(2)).operator/(3);
	if (bPointInPolygonRange(gcenter, polygon) == false) {
		return false;
	}
	for (int i = 0; i < 3; i++) {
		vec2f considerP = (gcenter + triangle.at(0) * 3) / 4;
		if (bPointInPolygonRange(considerP, polygon) == false) {
			return false;
		}
	}

	return true;
}

bool shp::bPointInRectRange(vec2f point, rect4f rt)
{
	if ((rt.fx < point.x && point.x < rt.lx)
		&& (rt.fy < point.y && point.y < rt.ly)) {
		return true;
	}
	else return false;
}

bool shp::bRectInRectRange(rect4f smallrt, rect4f bigrt, bool include_same, bool include_any_contact)
{
	int xn = 0;
	int yn = 0;
	if (include_same) {
		if (bigrt.fx <= smallrt.fx && smallrt.fx <= bigrt.lx) xn++;
		if (bigrt.fx <= smallrt.lx && smallrt.lx <= bigrt.lx) xn++;
		if (bigrt.fy <= smallrt.fy && smallrt.fy <= bigrt.ly) yn++;
		if (bigrt.fy <= smallrt.ly && smallrt.ly <= bigrt.ly) yn++;
	}
	else {
		if (bigrt.fx < smallrt.fx && smallrt.fx < bigrt.lx) xn++;
		if (bigrt.fx < smallrt.lx && smallrt.lx < bigrt.lx) xn++;
		if (bigrt.fy < smallrt.fy && smallrt.fy < bigrt.ly) yn++;
		if (bigrt.fy < smallrt.ly && smallrt.ly < bigrt.ly) yn++;
	}

	if (include_any_contact) {
		if (xn > 0 && yn > 0) return true;
		else return false;
	}
	else {
		if (xn > 1 && yn > 1) return true;
		else return false;
	}
}

shp::vec2f shp::GetPointRangeInStraight(vec2f point, straightLine sl)
{
	float x2 = sl.xrate * (point.y - sl.inDot.y) / sl.yrate + sl.inDot.x;
	float y2 = sl.yrate * (point.x - sl.inDot.x) / sl.xrate + sl.inDot.y;
	x2 = point.x - x2;
	y2 = point.y - y2;
	vec2f rv = vec2f(x2 / abs(x2), y2 / abs(y2));
	if (isnan<float>(rv.x)) rv.x = 0;
	if (isnan<float>(rv.y)) rv.y = 0;
	return rv;
}

shp::vec2f shp::GetTriangleRangeInStraight(triangle3v tri, straightLine s1)
{
	vec2f dir[3];
	for (int i = 0; i < 3; ++i) {
		dir[i] = shp::GetPointRangeInStraight(tri.point[i], s1);
	}

	if (dir[0] == dir[1] && dir[1] == dir[2]) {
		return dir[0];
	}
	else {
		return vec2f(0, 0);
	}
}

shp::vec2f shp::GetCrossPoint(straightLine sl1, straightLine sl2)
{
	vec2f cross;
	if (sl1.xrate * sl2.xrate != 0 || sl1.yrate * sl2.yrate != 0) {
		cross.x = (sl1.inDot.x * sl1.yrate / sl1.xrate - sl1.inDot.y - sl2.inDot.x * sl2.yrate / sl2.xrate + sl2.inDot.y) / (sl1.yrate / sl1.xrate - sl2.yrate / sl2.xrate);
		cross.y = sl1.GetYFromX(cross.x);
	}
	else {
		if (sl1.xrate == 0 || sl2.xrate == 0) {
			cross.x = (sl1.xrate == 0) ? sl1.inDot.x : sl2.inDot.x;
			if (sl1.yrate == 0 || sl2.yrate == 0) {
				cross.y = (sl1.yrate == 0) ? sl1.inDot.y : sl2.inDot.y;
			}
			else {
				cross.y = (sl1.xrate == 0) ? sl2.GetYFromX(cross.x) : sl1.GetYFromX(cross.x);
			}
		}
		else {
			cross.y = (sl1.yrate == 0) ? sl1.inDot.y : sl2.inDot.y;
			cross.x = (sl1.yrate == 0) ? sl2.GetXFromY(cross.y) : sl1.GetXFromY(cross.y);
		}

	}
	return cross;
}

shp::vec2f shp::GetRotatePos(vec2f center, vec2f pos, angle2f angle)
{
	float length = get_distance(center, pos);
	vec2f wh = pos - center;
	angle2f rAngle = angle + angle2f::usedxdy(wh.x, wh.y);
	vec2f rPos = vec2f(center.x + rAngle.dx * length, center.y + rAngle.dy * length);
	return rPos;
}

shp::straightLine shp::GetSameAngleLine(straightLine s1, straightLine s2, vec2f inpos)
{
	straightLine rs;
	vec2f cross = GetCrossPoint(s1, s2);
	vec2f dir[2];
	dir[0].x = cosf((atanf(s1.yrate / s1.xrate) + atanf(s2.yrate / s2.xrate)) / 2);
	dir[0].y = sinf((atanf(s1.yrate / s1.xrate) + atanf(s2.yrate / s2.xrate)) / 2);
	dir[1].x = dir[0].y;
	dir[1].y = -dir[0].x;

	vec2f d1 = GetPointRangeInStraight(inpos, s1);
	vec2f d2 = GetPointRangeInStraight(inpos, s2);

	vec2f d3 = GetPointRangeInStraight(cross + dir[0], s1);
	vec2f d4 = GetPointRangeInStraight(cross + dir[0], s2);
	if ((d1 == d3 && d2 == d4) || d1.Inverse() == d3 && d2.Inverse() == d4) {
		rs.xrate = dir[0].x;
		rs.yrate = dir[0].y;
		rs.inDot = cross;
	}
	else {
		rs.xrate = dir[1].x;
		rs.yrate = dir[1].y;
		rs.inDot = cross;
	}
	return rs;
}

vector<shp::triangle3v> shp::FreePolygonToTriangles(vector<vec2f> freepolygon)
{
	list<vec2f> lt;
	vector<triangle3v> triangles;
	for (int i = 0; i < freepolygon.size(); ++i) {
		lt.push_back(freepolygon[i]);
	}

	int siz = lt.size();
	while (lt.size() >= 3) {
		list<vec2f>::iterator lti = lt.begin();
		for (int i = 0; i < lt.size() - 2; i++) {

			bool bdraw = true;
			list<vec2f>::iterator ltk = lt.begin();
			for (int k = 0; k < lt.size(); k++) {
				if (i <= k && k <= i + 2) {
					++ltk;
					continue;
				}

				list<vec2f>::iterator inslti = lti;
				vec2f kv = *ltk;
				vec2f iv[3] = { *inslti , *(++inslti) , *(++inslti) };

				if (bPointInTriangleRange(kv, iv[0], iv[1], iv[3])) {
					bdraw = false;
					break;
				}
			}

			if (bdraw == true) {
				list<vec2f>::iterator inslti = lti;
				vec2f pi = *inslti;
				vec2f pi1 = *(++inslti);
				vec2f pi2 = *(++inslti);
				vec2f gcenter = vec2f((pi.x + pi1.x + pi2.x) / 3, (pi.y + pi1.y + pi2.y) / 3);
				vector<vec2f> tri;
				tri.resize(3); tri[0] = pi; tri[1] = pi1; tri[2] = pi2;
				if (bTriangleInPolygonRange(tri, freepolygon)) {
					triangle3v tri;
					tri.point[0] = pi;
					tri.point[1] = pi1;
					tri.point[2] = pi2;
					triangles.push_back(tri);

					list<vec2f>::iterator inslti2 = lti;
					lt.erase(++inslti);
				}
			}

			++lti;
		}

		if (siz == lt.size()) {
			break;
		}
		else {
			siz = lt.size();
		}

	}
	return triangles;
}

vector<shp::triangle3v> shp::TriangleCutStraightLine(triangle3v triangle, straightLine cutline, vec2f remaindir)
{
	vector<triangle3v> returnTri;
	vec2f dir[3];
	for (int i = 0; i < 3; ++i) {
		dir[i] = shp::GetPointRangeInStraight(triangle.point[i], cutline);
	}

	int count[3] = { 0, 0, 0 };
	int cnum = 0;
	for (int i = 0; i < 3; ++i) {
		for (int k = i + 1; k < 3; ++k) {
			if (dir[i] != dir[k]) {
				++count[i];
				++count[k];
			}
		}

		if (count[i] == 2) {
			cnum = i;
			break;
		}
	}

	vec2f crossPos[2];
	int crossN = 0;
	for (int i = 0; i < 3; ++i) {
		if (i == cnum) continue;

		straightLine l = straightLine(triangle.point[cnum], triangle.point[i]);
		crossPos[crossN] = shp::GetCrossPoint(l, cutline);
		crossN++;
	}

	if (remaindir.x == dir[cnum].x || remaindir.y == dir[cnum].y) {
		triangle3v t = triangle3v(triangle.point[cnum], crossPos[0], crossPos[1]);
		returnTri.push_back(t);
	}
	else {
		int cnnum[2];
		int cc = 0;
		for (int i = 0; i < 3; ++i) {
			if (i == cnum) continue;
			cnnum[cc] = i;
			cc++;
		}
		triangle3v t1 = triangle3v(crossPos[1], crossPos[0], triangle.point[cnnum[0]]);
		returnTri.push_back(t1);

		if (shp::get_distance(crossPos[0], triangle.point[cnnum[1]]) >
			shp::get_distance(crossPos[1], triangle.point[cnnum[1]])) {
			triangle3v t2 = triangle3v(triangle.point[cnnum[0]], crossPos[1], triangle.point[cnnum[1]]);
			returnTri.push_back(t2);
		}
		else {
			triangle3v t2 = triangle3v(triangle.point[cnnum[0]], crossPos[0], triangle.point[cnnum[1]]);
			returnTri.push_back(t2);
		}

	}
	return returnTri;
}

vector<shp::triangle3v> shp::TrianglesCutStraightLine(vector<triangle3v> triangles, straightLine cutline, vec2f remaindir)
{
	vector<triangle3v> returnpoly;
	for (int i = 0; i < triangles.size(); ++i) {
		vec2f dir = shp::GetTriangleRangeInStraight(triangles[i], cutline);
		if (dir == remaindir) {
			returnpoly.push_back(triangles[i]);
		}
		else if (dir == vec2f(0, 0)) {
			vector<triangle3v> t = shp::TriangleCutStraightLine(triangles[i], cutline, remaindir);
			for (int k = 0; k < t.size(); ++k) {
				returnpoly.push_back(t[k]);
			}
		}
	}

	return returnpoly;
}

vector<shp::triangle3v> shp::FreePolygonInRectRange(vector<vec2f> freepolygon, rect4f rt)
{
	rect4f location = rect4f(freepolygon[0].x, freepolygon[0].y, freepolygon[0].x, freepolygon[0].y);
	for (int i = 0; i < freepolygon.size(); ++i) {
		vec2f pos = freepolygon[i];
		if (location.fx > pos.x) {
			location.fx = pos.x;
		}
		if (location.fy > pos.y) {
			location.fy = pos.y;
		}
		if (location.lx < pos.x) {
			location.lx = pos.x;
		}
		if (location.ly < pos.y) {
			location.ly = pos.y;
		}
	}

	vector<triangle3v> tris = shp::FreePolygonToTriangles(freepolygon);
	if (shp::bRectInRectRange(location, rt, true, false)) {
		return tris;
	}
	else {
		vec2f edge[4] = { vec2f(location.fx, location.fy),
		vec2f(location.fx, location.ly), vec2f(location.lx, location.ly),
		vec2f(location.lx, location.fy) };
		straightLine rtLine[4] = { straightLine(edge[0], edge[1]),
		straightLine(edge[1], edge[2]), straightLine(edge[2], edge[3]),
		straightLine(edge[3], edge[1]) };
		vector<triangle3v> returntri;
		for (int i = 0; i < 4; ++i) {
			vec2f rdir = vec2f(0, 0);
			switch (i) {
			case 0:
				rdir = vec2f(1, 0);
				break;
			case 1:
				rdir = vec2f(0, -1);
				break;
			case 2:
				rdir = vec2f(-1, 0);
				break;
			case 3:
				rdir = vec2f(0, 1);
				break;
			}
			returntri = shp::TrianglesCutStraightLine(tris, rtLine[i], rdir);
			tris.clear();
			tris = returntri;
		}

		return returntri;
	}
}