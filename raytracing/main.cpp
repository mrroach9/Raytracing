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
#include "jsoncpp\json.h"
#include <fstream>
#include <ctime>

using namespace std;

bool parseJson(string filename, Json::Value &root) {
	Json::Reader reader;
	ifstream fin(filename.c_str());
	return reader.parse(fin, root);
}

int main(){
	int start_time = (int)clock();
	cout << "Parsing JSON scene descriptor..." << endl;
	Json::Value root;
	if (!parseJson("scene.json", root)) {
		cout  << "Failed to parse scene descriptor!" << endl;
		return 1;
	}

	cout << "Importing models and building the scene..." << endl;
	Scene* scene = new Scene(root["scene"]);

	cout << "Configuring raytracer..." << endl;
	RayTracer* rt = new RayTracer(scene, root["renderer"]);

	int render_start_time = clock();
	cout << "Start rendering..." << endl;
	rt->render();
	cout << "Rendering finished!" << endl;
	int render_end_time = clock();

	cout << "Saving image..." << endl;
	rt->saveImage();
	int end_time = clock();

	cout << "All finished!" << endl;
	cout << "Render Time: " << render_end_time - render_start_time << "ms." << endl;
	cout << " Total Time: " << end_time - start_time << "ms." << endl;

	return 0;
}