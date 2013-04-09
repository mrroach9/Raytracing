#ifndef _LIGHTSOURCE_H_
#define _LIGHTSOURCE_H_

#include "Geometry.h"
#include "Material.h"
#include "Color3.h"

class LightSource{
public:
	Vector3D pos;
	Color3 color;

public:
	LightSource();
	LightSource(Vector3D pos, Color3 color);
	LightSource(const LightSource &ls);
	//TODO: attenuation
	virtual double attenuate(double dist);
};

#endif //_LIGHTSOURCE_H_