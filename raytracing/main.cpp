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
	CMesh* bunny_mesh = new CMesh();
	bunny_mesh->load("5000_bunny.obj");
	Model* bunny_model = new Model();
	//Construct Model
	bunny_model->mesh = new CMesh(*bunny_mesh);
	bunny_model->translation = Vector3D(-3,-1,10);
	bunny_model->scale = Vector3D(1.5,1.5,1.5);
	bunny_model->rotation = Vector3D(0, PI, 0);
	bunny_model->material = Material(11);
	bunny_model->material.shininess = 20;
	bunny_model->material.reflect = Color3(0.9,0.9,0.9);
	bunny_model->material.refract = Color3(0,0,0);
	bunny_model->getTransformedMesh();

	
	//Load Mesh
	CMesh* sphere_mesh = new CMesh();
	sphere_mesh->load("sphere_2000_uniform.obj");
	Model* sphere_model = new Model();
	//Construct Model
	sphere_model->mesh = new CMesh(*sphere_mesh);
	sphere_model->translation = Vector3D(-1.5,0,15);
	sphere_model->scale = Vector3D(2.5,2.5,2.5);
	sphere_model->material = Material(12);
	sphere_model->material.shininess = 20;
	sphere_model->material.reflect = Color3(1,1,1);
	sphere_model->material.refract = Color3(0,0,0);
	sphere_model->getTransformedMesh();
/*
	//Load Mesh
	CMesh* crystal_mesh = new CMesh();
	crystal_mesh->load("sphere_2000_uniform.obj");
	Model* crystal_model = new Model();
	//Construct Model
	crystal_model->mesh = new CMesh(*crystal_mesh);
	crystal_model->translation = Vector3D(1.5, -0.5, 8);
	crystal_model->scale = Vector3D(1,1,1);
	crystal_model->material.ambient = Color3(0,0,0.05);
	crystal_model->material.diffuse = Color3(0,0,0.1);
	crystal_model->material.specular = Color3(1,1,1);
	crystal_model->material.shininess = 40;
	crystal_model->material.reflect = Color3(0.1,0.1,0.1);
	crystal_model->material.refract = Color3(0.7,0.7,0.9);
	crystal_model->getTransformedMesh();

	//Load Mesh
	CMesh* hand_mesh = new CMesh();
	hand_mesh->load("skel.hand.obj");
	Model* hand_model = new Model();
	//Construct Model
	hand_model->mesh = new CMesh(*hand_mesh);
	hand_model->translation = Vector3D(1.5, -2.5, 8);
	hand_model->scale = Vector3D(2,2,2);
	hand_model->rotation = Vector3D(3*PI/2, PI/4, 0);
	hand_model->material = Material(3);
	hand_model->material.shininess = 1;
	hand_model->material.reflect = Color3(0.1,0.1,0.1);
	hand_model->material.refract = Color3(0,0,0);
	hand_model->getTransformedMesh();
*/
	cout<<"done."<<endl;
	cout<<"Constructing light sources and camera...";

	//Construct Light Source
	LightSource* ls = new LightSource();
	ls->pos = Vector3D(-3, 10, 0);
	ls->material.ambient = Color3(0.2,0.2,0.2);
	ls->material.diffuse = Color3(1,1,1);
	ls->material.specular = Color3(1,1,1);
/*
	//Construct Light Source
	LightSource* ls2 = new LightSource();
	ls2->pos = Vector3D(3, 10, -2);
	ls2->material.ambient = Color3(0.3,0.3,0.3);
	ls2->material.diffuse = Color3(1,1,1);
	ls2->material.specular = Color3(1,1,1);

	
	//Construct Light Source
	LightSource* ls3 = new LightSource();
	ls3->pos = Vector3D(1, 20, 40);
	ls3->material.ambient = Color3(0.1,0.1,0.1);
	ls3->material.diffuse = Color3(1,1,1);
	ls3->material.specular = Color3(1,1,1);
*/
	//Construct Camera
	Camera* camera = new Camera();
	camera->pos = Vector3D(0, 0, 0);
	camera->up = Vector3D(0, 1, 0);
	camera->dir = Vector3D(0, 0, 1);
	camera->up.normalize();
	camera->dir.normalize();

	cout<<"done."<<endl;
	cout<<"Building scene and transforming models...";

	//Construct Scene
	Scene* scene = new Scene();
	scene->addModel(bunny_model);
	scene->addModel(sphere_model);
	//scene->addModel(crystal_model);
	//scene->addModel(hand_model);
	scene->addLight(ls);
	//scene->addLight(ls2);
	//scene->addLight(ls3);
	scene->setCamera(camera);
	scene->transformModelAndLight();
	bunny_model->buildKdTree();
	sphere_model->buildKdTree();
	//crystal_model->buildKdTree();
	//hand_model->buildKdTree();

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