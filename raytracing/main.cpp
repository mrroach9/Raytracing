#include <iostream>
#include "Mesh.h"
#include "Model.h"
#include "Geometry.h"
#include "Camera.h"
#include "Material.h"
#include "Color3.h"
#include "Scene.h"
#include "LightSource.h"
#include "RayTracer.h"

using namespace std;

int main(){
	cout << "Loading meshes and building models...";

	//Load Mesh
	CMesh* plane_mesh1 = new CMesh();
	plane_mesh1->load("./model/plane1.obj");
	Model* plane_model1 = new Model();
	//Construct Model
	plane_model1->mesh = new CMesh(*plane_mesh1);
	plane_model1->translation = Vector3D(0, -1, 25);
	plane_model1->scale = Vector3D(10, 5, 1);
	plane_model1->rotation = Vector3D(PI, 0, 0);
	plane_model1->material = Material(18);
	plane_model1->material.shininess = 1;
	plane_model1->material.reflect = Color3(0.02, 0.02, 0.02);
	plane_model1->material.refract = Color3(0, 0, 0);
	plane_model1->material.refract_index = 1;
	plane_model1->getTransformedMesh();

	//Load Mesh
	CMesh* plane_mesh2 = new CMesh();
	plane_mesh2->load("./model/plane1.obj");
	Model* plane_model2 = new Model();
	//Construct Model
	plane_model2->mesh = new CMesh(*plane_mesh2);
	plane_model2->translation = Vector3D(0, 3, 15);
	plane_model2->scale = Vector3D(10, 10, 1);
	plane_model2->rotation = Vector3D(PI * 0.50, 0, 0);
	plane_model2->material = Material(18);
	plane_model2->material.shininess = 1;
	plane_model2->material.reflect = Color3(0, 0, 0);
	plane_model2->material.refract = Color3(0, 0, 0);
	plane_model2->material.refract_index = 1;
	plane_model2->getTransformedMesh();

	//Load Mesh
	CMesh* bunny_mesh = new CMesh();
	bunny_mesh->load("./model/5000_bunny.obj");
	Model* bunny_model = new Model();
	//Construct Model
	bunny_model->mesh = new CMesh(*bunny_mesh);
	bunny_model->translation = Vector3D(-1.5, 1.7, 10);
	bunny_model->scale = Vector3D(1.3, 1.3, 1.3);
	bunny_model->rotation = Vector3D(0, PI, PI);
	bunny_model->material = Material(11);
	bunny_model->material.shininess = 10;
	bunny_model->material.reflect = Color3(0.6,0.6,0.6);
	bunny_model->material.refract = Color3(0,0,0);
	bunny_model->material.refract_index = 1;
	bunny_model->getTransformedMesh();

	
	//Load Mesh
	CMesh* sphere_mesh = new CMesh();
	sphere_mesh->load("model/sphere_2000_uniform.obj");
	Model* sphere_model = new Model();
	//Construct Model
	sphere_model->mesh = new CMesh(*sphere_mesh);
	sphere_model->translation = Vector3D(-1.5, 0.8, 16);
	sphere_model->scale = Vector3D(2, 2, 2);
	sphere_model->material = Material(12);
	sphere_model->material.shininess = 10;
	sphere_model->material.reflect = Color3(0.8, 0.8, 0.8);
	sphere_model->material.refract = Color3(0, 0, 0);
	sphere_model->material.refract_index = 1;
	sphere_model->getTransformedMesh();

	//Load Mesh
	CMesh* crystal_mesh = new CMesh();
	crystal_mesh->load("model/sphere_2000_uniform.obj");
	Model* crystal_model = new Model();
	//Construct Model
	crystal_model->mesh = new CMesh(*crystal_mesh);
	crystal_model->translation = Vector3D(1.5, 1.5, 10);
	crystal_model->scale = Vector3D(0.9, 0.9, 0.9);
	crystal_model->material.ambient = Color3(0, 0, 0.05);
	crystal_model->material.diffuse = Color3(0, 0, 0.1);
	crystal_model->material.specular = Color3(1, 1, 1);
	crystal_model->material.shininess = 20;
	crystal_model->material.reflect = Color3(0.2, 0.2, 0.2);
	crystal_model->material.refract = Color3(0.7, 0.7, 0.8);
	crystal_model->material.refract_index = 1.03;
	crystal_model->getTransformedMesh();

	cout<<"done."<<endl;
	cout<<"Constructing light sources and camera...";

	//Construct Light Source
	LightSource* ls = new LightSource();
	ls->pos = Vector3D(-3, -3, -2);
	ls->color = Color3(0.6, 0.6, 0.6);

	//Construct Light Source
	LightSource* ls2 = new LightSource();
	ls2->pos = Vector3D(3, -5, -4);
	ls2->color = Color3(0.3, 0.1, 0);

	//Construct Camera
	Camera* camera = new Camera();
	camera->pos = Vector3D(1, 0.5, 0);
	camera->up = Vector3D(0, 1, 0);
	camera->dir = Vector3D(-0.15, 0, 1);
	camera->up.normalize();
	camera->dir.normalize();

	cout<<"done."<<endl;
	cout<<"Building scene and transforming models...";

	//Construct Scene
	Scene* scene = new Scene();
	scene->addModel(bunny_model);
	scene->addModel(sphere_model);
	scene->addModel(crystal_model);
	scene->addModel(plane_model1);
	scene->addModel(plane_model2);
	scene->addLight(ls);
	scene->addLight(ls2);
	scene->setCamera(camera);
	scene->transformModelAndLight();
	bunny_model->buildKdTree();
	sphere_model->buildKdTree();
	crystal_model->buildKdTree();
	plane_model1->buildKdTree();
	plane_model2->buildKdTree();

	cout<<"done."<<endl;
	cout<<"Configuring lighter, shader and renderer...";

	//Config Light and Shade Model
	LightShadeModel::setLightModel(LightModel::PhongLighting);
	LightShadeModel::setShadeModel(ShadeModel::PhongShading);

	//Constructing Ray Tracer
	RayTracer rt(scene);

	cout << "done." <<endl;

	cout << "Start rendering." << endl;
	rt.render();
	cout << "Render finished!" << endl;
	return 0;
}