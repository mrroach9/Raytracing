#include "RayTracer.h"

RayTracer::RayTracer(Scene* scene){
	this->scene = scene;
	this->res_H = DEFAULT_RESOLUTION_H;
	this->res_W = DEFAULT_RESOLUTION_W;
	this->super_sample = DEFAULT_SUPER_SAMPLE;
	this->max_recurs = DEFAULT_MAX_RECURSSION;
	this->threshold = DEFAULT_THRESHOLD;
}

RayTracer::RayTracer(Scene* scene, int H, int W, int ss, 
					 int max_recurs, double thresh){
	this->scene = scene;
	this->res_H = H;
	this->res_W = W;
	this->super_sample = ss;
	this->max_recurs = max_recurs;
	this->threshold = thresh;
}

RayTracer::~RayTracer(){
	delete buffer;
	buffer = NULL;
}

void RayTracer::setResolution(int H, int W, int ss){
	this->res_H = H;
	this->res_W = W;
	this->super_sample = ss;
}

void RayTracer::setTerminateCondition(int max_recurs, double thresh){
	this->max_recurs = max_recurs;
	this->threshold = thresh;
}

void RayTracer::render(){
	if (scene == NULL || res_H <= 0 || res_W <= 0 ||
		super_sample <= 0 || max_recurs < 0 || threshold < DOUBLE_EPS) {
		return;
	}

	buffer = new CImg<unsigned char>(res_W, res_H, 1, 3);

	Vector3D origin(0,0,0);
	Color3 startColor(1,1,1);

	double boundx = tan(scene->camera->h_ang);
	double boundy = tan(scene->camera->v_ang);

	int count = 0;
//	#pragma omp parallel for num_threads(DEFAULT_OMP_THREAD)
	for (int i = 0; i < res_H; ++i) {
		for (int j = 0; j < res_W; ++j){
			Color3 finalColor(0,0,0);
			for (UINT it = 0; it < (UINT)super_sample; ++it) {
				for (UINT jt = 0; jt < (UINT)super_sample; ++jt) {
					double rx = double(j*super_sample + jt) / double(res_W * super_sample);
					double ry = double(i*super_sample + it) / double(res_H * super_sample);
					Vector3D dir((2*rx - 1) * boundx, 
								 (2*ry - 1) * boundy, 1);
					dir.normalize();
					Color3 c = trace(origin, dir, startColor, 0);
					finalColor += c;
				}
			}
			finalColor *= 1 / double(super_sample*super_sample);
			finalColor.truncate();
			unsigned char cr = (unsigned char)(finalColor.r * 255);
			unsigned char cg = (unsigned char)(finalColor.g * 255);
			unsigned char cb = (unsigned char)(finalColor.b * 255);
			buffer->set_linear_atXY(cr,(float)j,(float)i,0,0);
			buffer->set_linear_atXY(cg,(float)j,(float)i,0,1);
			buffer->set_linear_atXY(cb,(float)j,(float)i,0,2);
		}
		++ count;
		if ((count + 1) % 10 == 0){		
			std::cout << "\t" << (count + 1) << "/" << res_H << endl;
			buffer->save(DEFAULT_OUTPUT);
		}
	}
}

Color3 RayTracer::trace(Vector3D ori, Vector3D dir, Color3 rayColor, int depth){
	if (depth > this->max_recurs) return Color3(0,0,0);
	if (rayColor.r < this->threshold &&
		rayColor.g < this->threshold &&
		rayColor.b < this->threshold){
			return Color3(0,0,0);
	}

	Color3 totalColor(0,0,0);

	double t = 0;
	UINT model_id = -1;
	UINT face_id = -1;

	if (!findClosestFace(ori, dir, model_id, face_id, t, Color3())){
		return totalColor;
	}
	Material mat = scene->modelList[model_id]->material;
	Vector3D p = ori + dir * t;
	Vector3D n = LightShadeModel::calcNormal(p, 
					scene->modelList[model_id], face_id);

	Vector3D rfr;
	bool totalReflect = !calcRefracDir(dir, n, mat.refract_index, rfr);

	for (UINT i = 0; i < scene->lightList.size(); ++i) {
		LightSource* ls = scene->lightList[i];
		Vector3D d = ls->pos - p;
		d.normalize();
		UINT tmp_mid = -1, tmp_fid = -1;
		double tmp_t = 0;
		Color3 currentColor(0,0,0);
		Color3 opaque;
		bool in_shadow = findClosestFace(p, d, tmp_mid, tmp_fid, tmp_t, opaque);
		UINT mask = MASK_AMBIENT;
		if (!in_shadow) {
			mask |= (MASK_DIFFUSE | MASK_SPECULAR);
		}
		Color3 current_ambient = 
			LightShadeModel::calcColor(p, dir, n, rayColor, ls, 
							scene->modelList[model_id], face_id, MASK_AMBIENT);
		Color3 current_diffspec = 
			LightShadeModel::calcColor(p, dir, n, rayColor, ls,
							scene->modelList[model_id], face_id, MASK_DIFFUSE | MASK_SPECULAR);
		currentColor = current_ambient + current_diffspec * opaque;
		if (!totalReflect) {
			currentColor *= (Color3(1, 1, 1) - mat.refract);
		}
		totalColor += currentColor;
	}

	
	Vector3D v = Vector3D(0,0,0) - dir;
	Vector3D rfl = 2 * (v * n) * n - v;

	Color3 rflRayColor = rayColor;
	if (!totalReflect) {
		rflRayColor *= mat.reflect;
	} else {
		rflRayColor *= (mat.reflect + mat.refract);
	}
	Color3 rfl_color = trace(p, rfl, rflRayColor, depth+1);
	
	Color3 rfrRayColor = rayColor;
	rfrRayColor *= mat.refract;
	Color3 rfr_color(0, 0, 0);
	if (!totalReflect) {
		rfr_color = trace(p, rfr, rfrRayColor, depth+1);	
	}
	totalColor += rfl_color + rfr_color;
	
	return totalColor;
}

bool RayTracer::calcRefracDir(Vector3D dir, Vector3D n, double refrac_ind,
				Vector3D &out) {
	dir = -1 * dir;
	double c1 = dir * n;
	double l = 0;
	if (c1 < 0) {	// From glass to air
		l = refrac_ind;	// refrac_ind > 1
		n = -1 * n;
	} else {	// From air to glass
		l = 1.0 / refrac_ind;
	}
	c1 = dir * n;
	double c2 = 1 - l * l + l * l * c1 * c1; 
	if (c2 < DOUBLE_EPS) {	// total reflection
		return false;
	}
	c2 = c1 * l - sqrt(c2);
	out = c2 * n - l * dir;
	out.normalize();
	return true;
}

bool RayTracer::findClosestFace(Vector3D ori, Vector3D dir, 
								UINT &model_id, UINT &face_id,
								double &t_on_line, Color3 &opaque){
	model_id = -1;
	face_id = -1;
	t_on_line = 1e10;
	opaque = Color3(1, 1, 1);

	vector<UINT> vec;
	vec.clear(); 
	
	for (UINT i = 0; i < scene->modelList.size(); ++i){
		vec.clear();
		KdTree * kdtree = scene->modelList[i]->kdtree;
		CMesh * mesh = scene->modelList[i]->mesh;
		kdtree->searchLine(ori, dir, DOUBLE_EPS, scene->camera->b_plane, vec);
		bool pierce = false;
		for (UINT j = 0; j < vec.size(); ++j) {
			UINT fid = vec[j];
			Vector3D p(0,0,0);
			double t = 0;

			Vector3D a = mesh->m_pFace[fid].m_vMassPoint;
			Vector3D n = mesh->m_pFace[fid].m_vNormal;
			double dn = dir * n;
			if (EQUALZERO(dn)) {
				if (EQUALZERO(10 * (a - ori) * n)) {
					p = a;
					t = (a - ori).normalize();
				} else {
					continue;
				}
			} else {
				t = (a * n - ori * n) / dn;	
				p = ori + dir * t;
			}
			UINT* vList = mesh->m_pFace[fid].m_piVertex;
			Vector3D alpha;
			if (!inTriangle(mesh->m_pVertex[vList[0]].m_vPosition,
						    mesh->m_pVertex[vList[1]].m_vPosition,
						    mesh->m_pVertex[vList[2]].m_vPosition,
						   p, alpha)) {
				continue;
			}
			if (t < scene->camera->f_plane || 
				t > scene->camera->b_plane ||
				t < DOUBLE_EPS) {
				continue;
			}
			if (t < t_on_line) {
				t_on_line = t;
				model_id = i;
				face_id = fid;
				pierce = true;
			}
		}
		if (pierce) {
			opaque *= scene->modelList[i]->material.refract;
		}
	}
	if (model_id == -1 || face_id == -1 || t_on_line > 1e9){
		return false;
	}
	return true;
}

