#ifndef _CAMERA_H_
#define _CAMERA_H_

#include "Geometry.h"

#define DEFAULT_V_ANG	atan(0.3)
#define DEFAULT_H_ANG	atan(1.6/3)
#define DEFAULT_FORE	0.1
#define DEFAULT_BACK	1000

class Camera{
public:
	Vector3D	pos;
	Vector3D	up;
	Vector3D	dir;
	double		h_ang;		//Half angle(from center line to leftmost
	double		v_ang;
	double		f_plane;
	double		b_plane;
	//TODO: depth of field, focus length and aperture

public:
	Camera(){
		pos = Vector3D(0,0,0);
		up  = Vector3D(0,1,0);
		dir = Vector3D(0,0,1);
		h_ang = DEFAULT_H_ANG;
		v_ang = DEFAULT_V_ANG;
		f_plane = DEFAULT_FORE;
		b_plane = DEFAULT_BACK;
	};

	Camera(Vector3D pos, Vector3D up, double h_ang, double v_ang, double f, double b){
		this->pos = pos;
		this->up = up;
		this->h_ang = h_ang;
		this->v_ang = v_ang;
		this->f_plane = f;
		this->b_plane = b;
	}
};

#endif //_CAMERA_H_