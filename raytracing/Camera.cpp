#include "Camera.h"

Camera::Camera(){
	pos = Vector3D(0,0,0);
	up  = Vector3D(0,1,0);
	dir = Vector3D(0,0,1);
	h_ang = DEFAULT_H_ANG;
	v_ang = DEFAULT_V_ANG;
	f_plane = DEFAULT_FORE;
	b_plane = DEFAULT_BACK;
};

Camera::Camera(Vector3D pos, Vector3D up, double h_ang, double v_ang, double f, double b){
	this->pos = pos;
	this->up = up;
	this->h_ang = h_ang;
	this->v_ang = v_ang;
	this->f_plane = f;
	this->b_plane = b;
}

Camera::Camera(Json::Value json) {
	pos = Vector3D(json["position"]);
	up = Vector3D(json["up"]);
	dir = Vector3D(json["direction"]);
	h_ang = json["horizontal_angle"].asDouble();
	v_ang = json["vertical_angle"].asDouble();
	f_plane = json["front_clip_plane"].asDouble();
	b_plane = json["back_clip_plane"].asDouble();
	focal_length = json["focal_length"].asDouble();
	aperture = json["aperture"].asDouble();
}