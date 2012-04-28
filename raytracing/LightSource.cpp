#include "LightSource.h"

LightSource::LightSource(){
	pos = Vector3D(0,0,0);
	material = Material();
}

LightSource::LightSource(Vector3D pos, Material mat){
	this->pos = pos;
	this->material = mat;
}

LightSource::LightSource(const LightSource &ls){
	this->pos = ls.pos;
	this->material = ls.material;
}

double LightSource::attenuate(double dist){
	return 1.0;
}
