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
#include "jsoncpp\json.h"
#include <omp.h>

#define DEFAULT_RESOLUTION_H	360
#define DEFAULT_RESOLUTION_W	640
#define DEFAULT_SUPER_SAMPLE	1
#define	DEFAULT_MAX_RECURSSION	4
#define DEFAULT_THRESHOLD		0.001
#define DEFAULT_OUTPUT			"render.jpg"
#define DEFAULT_OMP_THREAD		8
#define DEFAULT_OMP_BLOCK		4

using namespace cimg_library;

class RayTracer{
private:
	Scene* scene;

	int res_H, res_W;
	int super_sample;
	int max_recurs;
	double threshold;

	int omp_thread;
	int omp_block;

	CImg<unsigned char>* buffer;
	string output_filename;

public:
	RayTracer(Scene* scene);
	RayTracer(Scene* scene, int H, int W, int ss, 
				int max_recurs, double thresh);
	RayTracer(Scene* scene, Json::Value json);
	~RayTracer();
	void setResolution(int H, int W, int ss = 1);
	void setTerminateCondition(int max_recurs, double thresh);

	void render();
	void saveImage();

private:
	Color3 trace(Vector3D ori, Vector3D dir, Color3 rayColor,
				  int depth);
	bool findClosestFace(Vector3D ori, Vector3D dir, 
						 UINT &model_id, UINT &face_id,
						 double &t, double max_t, Color3 &opaque);
	bool calcRefracDir(Vector3D dir, Vector3D n, double refrac_ind,
					   Vector3D &out);
};

#endif //_RAY_TRACER_H_