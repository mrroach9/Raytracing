#ifndef _CAMERA_H_
#define _CAMERA_H_

#include "Geometry.h"
#include "jsoncpp\json.h"

#define DEFAULT_V_ANG	atan(0.3)
#define DEFAULT_H_ANG	atan(1.6/3)
#define DEFAULT_FORE	0.1
#define DEFAULT_BACK	1000

class Camera{
public:
	Vector3D	pos;
	Vector3D	up;
	Vector3D	dir;
	double		h_ang;		//Half angle (from center to leftmost border)
	double		v_ang;		//Half angle (from center to upmost border)
	double		f_plane;
	double		b_plane;
	double		focal_length;
	double		aperture;	//The aperture here is the F-number: N = f/d, where
							//f is the focal length and d is the diameter of
							//aperture.

public:
	Camera();

	Camera(Vector3D pos, Vector3D up, double h_ang, double v_ang, double f, double b);

	Camera(Json::Value json);
};

#endif //_CAMERA_H_