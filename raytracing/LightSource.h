#ifndef _LIGHTSOURCE_H_
#define _LIGHTSOURCE_H_

#include "Geometry.h"
#include "Material.h"

class LightSource{
public:
	Vector3D pos;
	Material material;

public:
	LightSource();
	LightSource(Vector3D pos, Material mat);
	LightSource(const LightSource &ls);
	//TODO: attenuation
	virtual double attenuate(double dist);
};

#endif //_LIGHTSOURCE_H_