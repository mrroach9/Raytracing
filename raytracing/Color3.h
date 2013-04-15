#ifndef _COLOR3_H_
#define _COLOR3_H_

#include "jsoncpp\json.h"
#include "Geometry.h"

class Color3{
public:
	double r,g,b;
public:
	Color3():r(0),g(0),b(0){};
	Color3(double tr, double tg, double tb):r(tr),g(tg),b(tb){};
	Color3(const Color3 &c): r(c.r), g(c.g), b(c.b){};
	Color3(Json::Value json);
public:
	void truncate();

public:
	Color3& operator = (const Color3& c);
	Color3& operator+=(const Color3& c);
	Color3& operator-=(const Color3& c);
	Color3& operator*=(double u);
	Color3& operator*=(const Color3& c);

	friend Color3 operator+(const Color3& lc, const Color3& rc);
	friend Color3 operator-(const Color3& lc, const Color3& rc);
	friend Color3 operator*(const double u, const Color3& rc);
	friend Color3 operator*(const Color3& lc, const double u);
	friend Color3 operator*(const Color3& lc, const Color3& rc);
};

#endif //_COLOR3_H_
