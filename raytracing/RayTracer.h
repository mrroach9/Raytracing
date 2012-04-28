#ifndef _RAY_TRACER_H_
#define _RAY_TRACER_H_

#include <iostream>
#include "Geometry.h"
#include "Color3.h"
#include "Model.h"
#include "Scene.h"
#include "LightShadeModel.h"
#include "KdTree.h"
#include "CImg.h"

#define DEFAULT_RESOLUTION_H	180
#define DEFAULT_RESOLUTION_W	320
#define DEFAULT_SUPER_SAMPLE	2
#define	DEFAULT_MAX_RECURSSION	4
#define DEFAULT_THRESHOLD		0.001

using namespace cimg_library;

class RayTracer{
private:
	Scene* scene;

	int res_H, res_W;
	int super_sample;
	int max_recurs;
	double threshold;

	CImg<unsigned char>* buffer;

public:
	RayTracer(Scene* scene);
	RayTracer(Scene* scene, int H, int W, int ss, 
				int max_recurs, double thresh);
	~RayTracer();
	void setResolution(int H, int W, int ss = 1);
	void setTerminateCondition(int max_recurs, double thresh);

	void render();

private:
	Color3 trace(Vector3D ori, Vector3D dir, Color3 rayColor,
				  int depth);
	bool findClosestFace(Vector3D ori, Vector3D dir, 
						 UINT &model_id, UINT &face_id,
						 double &t);
};

#endif //_RAY_TRACER_H_