#include "LightSource.h"

LightSource::LightSource(){
	pos = Vector3D(0,0,0);
	color = Color3(0, 0, 0);
}

LightSource::LightSource(Json::Value json) {
	pos = Vector3D(json["position"]);
	color = Color3(json["color"]);
}

LightSource::LightSource(Vector3D pos, Color3 color){
	this->pos = pos;
	this->color = color;
}

LightSource::LightSource(const LightSource &ls){
	this->pos = ls.pos;
	this->color = ls.color;
}

double LightSource::attenuate(double dist){
	return 1.0;
}
