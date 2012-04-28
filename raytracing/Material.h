//MaterialProperties.h

#ifndef _MaterialProperties_H_
#define _MaterialProperties_H_

#include "Geometry.h"
#include "Color3.h"

class Material{
public:
	Color3 ambient;
	Color3 diffuse;
	Color3 specular;
	float shininess;
	Color3 reflect;
	Color3 refract;
	float refract_index;
public:
	Material();
	Material(Color3 amb, Color3 dif, Color3 spec, float shininess,
			 Color3 rfl, Color3 rfr, float rfr_ind);
	Material(UINT material_ID);
	
	Material& operator = (const Material& m);
};

/* emerald, jade, obsidian, pearl, ruby, turquoise, brass, bronze, chrome, copper, gold, silver, silver2
   black, cyan, green, red, white, yellow plastic, black, cyan, green, red, white, yellow rubber*/
static float AMBIENT[][4] = {{0.2f,0.2f,0.2f,1.0f}, {0.0215f,0.1745f,0.0215f,1.0f}, //default, emerald 0-1
	{0.135f,0.2225f,0.1575f,1.0f},{0.05375f,0.05f,0.06625f,1.0f}, //jade, obsidian 2-3
	{0.25f,0.20725f,0.20725f,1.0f}, {0.1745f,0.01175f,0.01175f,1.0f}, //pearl, ruby 4-5
	{0.1f,0.18725f,0.1745f,1.0f}, {0.329412f,0.223529f,0.027451f,1.0f}, //turquoise, brass 6-7
	{0.2125f,0.1275f,0.054f,1.0f}, {0.25f,0.25f,0.25f,1.0f}, //bronze, chrome 8-9
	{0.19125f,0.0735f,0.0225f,1.0f}, {0.24725f,0.1995f,0.0745f,1.0f}, //copper, gold 10-11
	{0.19225f,0.19225f,0.19225f,1.0f}, {0.15f,0.15f,0.15f,1.0f}, {0.0f,0.0f,0.0f,1.0f}, //silver, silver2, black plastic 12-14
	{0.0f,0.1f,0.06f,1.0f}, {0.0f,0.0f,0.0f,1.0f}, {0.0f,0.0f,0.0f,1.0f}, //cyan, green, red plastic 15-17
	{0.0f,0.0f,0.0f,1.0f}, {0.0f,0.0f,0.0f,1.0f}, {0.02f,0.02f,0.02f,1.0f}, //white, yellow plastic, black rubber 18-20
	{0.0f,0.05f,0.05f,1.0f}, {0.0f,0.05f,0.0f,1.0f}, {0.05f,0.0f,0.0f,1.0f}, //cyan, green, red rubber 21-23
	{0.05f,0.05f,0.05f,1.0f}, {0.05f,0.05f,0.0f,1.0f}}; //yellow rubber 24-25
	
static float DIFFUSE[][4] = {{0.8f,0.8f,0.8f,1.0f}, {0.07568f,0.61424f,0.07568f,1.0f}, //0-1
	{0.54f,0.89f,0.63f,1.0f}, {0.18275f,0.17f,0.22525f,1.0f}, //2-3
	{1.0f,0.829f,0.829f,1.0f}, {0.61424f,0.04136f,0.04136f,1.0f}, //4-5
	{0.396f,0.74151f,0.69102f,1.0f}, {0.780392f,0.568627f,0.113725f,1.0f}, //6-7
	{0.714f,0.4284f,0.18144f,1.0f}, {0.4f,0.4f,0.4f,1.0f}, //8-9
	{0.7038f,0.27048f,0.0828f,1.0f}, {0.74164f,0.60648f,0.22648f,1.0f}, //10-11
	{0.50754f,0.50754f,0.50754f,1.0f}, {0.50754f,0.50754f,0.50754f,1.0f}, {0.01f,0.01f,0.01f,1.0f}, //12-14
	{0.0f,0.50980392f,0.50980392f,1.0f}, {0.1f,0.35f,0.1f,1.0f}, //15-16
	{0.5f,0.0f,0.0f,1.0f}, {0.55f,0.55f,0.55f,1.0f}, //17-18
	{0.5f,0.5f,0.0f,1.0f}, {0.01f,0.01f,0.01f,1.0f}, //19-20
	{0.4f,0.5f,0.5f,1.0f}, {0.4f,0.5f,0.4f,1.0f}, //21-22
	{0.5f,0.4f,0.4f,1.0f}, {0.5f,0.5f,0.5f,1.0f},{0.5f,0.5f,0.4f,1.0f}}; //23-25

static float SPECULAR[][4] = {{0.0f,0.0f,0.0f,1.0f}, {0.633f,0.727811f,0.633f,1.0f}, //0-1
	{0.316228f,0.316228f,0.316228f,1.0f}, {0.332741f,0.328634f,0.346435f,1.0f}, //2-3
	{0.296648f,0.296648f,0.296648f,1.0f}, {0.727811f,0.626959f,0.626959f,1.0f}, //4-5
	{0.297254f,0.30829f,0.306678f,1.0f}, {0.992157f,0.941176f,0.807843f,1.0f}, //6-7
	{0.393548f,0.271906f,0.166721f,1.0f}, {0.774597f,0.774597f,0.774597f,1.0f}, //8-9
	{0.256777f,0.137622f,0.086014f,1.0f}, {0.628821f,0.555802f,0.366065f,1.0f}, //10-11
	{0.508273f,0.508273f,0.508273f,1.0f}, {0.508273f,0.508273f,0.508273f,1.0f}, {0.5f,0.5f,0.5f,1.0f}, //12-14
	{0.50196078f,0.50196078f,0.50196078f,1.0f}, {0.45f,0.55f,0.45f,1.0f}, //15-16
	{0.7f,0.6f,0.6f,1.0f}, {0.7f,0.7f,0.7f,1.0f}, //17-18
	{0.6f,0.6f,0.5f,1.0f}, {0.4f,0.4f,0.4f,1.0f}, //19-20
	{0.04f,0.7f,0.7f,1.0f}, {0.04f,0.7f,0.04f,1.0f}, //21-22
	{0.7f,0.04f,0.04f,1.0f}, {0.7f,0.7f,0.7f,1.0f},{0.7f,0.7f,0.04f,1.0f}}; //23-25

static float SHININESS[] = {0.0f, 0.6f, 0.1f, 0.3f, 0.088f, 0.6f, 0.1f, //0-6
	0.21794872f, 0.2f, 0.6f, 0.1f, 0.4f, 0.4f, 0.4f, 0.25f, 0.25f, 0.25f, 0.25f, 0.25f, //7-18
	0.25f, 0.078125f, 0.078125f, 0.078125f, 0.078125f, 0.078125f, 0.078125f}; //19-25

#endif