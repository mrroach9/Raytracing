#include "Color3.h"

Color3& Color3::operator=(const Color3& c){	
	r = c.r;	g = c.g;	b = c.b;
	return (*this);
}

Color3& Color3::operator+=(const Color3& c){	
	r += c.r;	g += c.g;	b += c.b;	
	return (*this);	
}

Color3& Color3::operator-=(const Color3& c){	
	r -= c.r;	g -= c.g;	b -= c.b;	
	return (*this);	
}

Color3& Color3::operator*=(double u){	
	r *= u;		g *= u;		b *= u;		
	return (*this);	
}

Color3 operator+(const Color3& lc, const Color3& rc){
	Color3 rel = lc;
	rel += rc;
	return rel;
}


Color3 operator-(const Color3& lc, const Color3& rc)
{
	Color3 rel = lc;
	rel -= rc;
	return rel;
}

Color3 operator*(const double u, const Color3& rc){
	Color3 rel = rc;
	rel *= u;
	return rel;
}

Color3 operator*(const Color3& lc, const double u){
	Color3 rel = lc;
	rel *= u;
	return rel;
}

Color3 operator*(const Color3& lc, const Color3& rc)
{
	return Color3(lc.r*rc.r, lc.g*rc.g, lc.b*rc.b);
}

void Color3::truncate(){
	if (r > 1) r = 1;
	if (g > 1) g = 1;
	if (b > 1) b = 1;
	if (r < 0) r = 0;
	if (g < 0) g = 0;
	if (b < 0) b = 0;
}


Color3& Color3::operator*=(const Color3& c) {
	this->r *= c.r;
	this->g *= c.g;
	this->b *= c.b;
	return (*this);
}