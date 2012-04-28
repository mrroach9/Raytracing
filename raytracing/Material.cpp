#include "Material.h"


Material::Material(){
	ambient = Color3(0,0,0);
	diffuse = Color3(0,0,0);
	specular = Color3(0,0,0);
	shininess = 1;
	reflect = Color3(0,0,0);
	refract = Color3(0,0,0);
	refract_index = 1;
}

Material::Material(
			Color3 amb, Color3 dif, Color3 spec, float shin,
			Color3 rfl, Color3 rfr, float rfr_ind){
	ambient = amb;
	diffuse = dif;
	specular = spec;
	shininess = shin;
	reflect = rfl;
	refract = rfr;
	refract_index = rfr_ind;
}

Material::Material(UINT material_ID) {
	ambient.r = AMBIENT[material_ID][0];
	diffuse.r = DIFFUSE[material_ID][0];
	specular.r = SPECULAR[material_ID][0];
	ambient.g = AMBIENT[material_ID][1];
	diffuse.g = DIFFUSE[material_ID][1];
	specular.g = SPECULAR[material_ID][1];
	ambient.b = AMBIENT[material_ID][2];
	diffuse.b = DIFFUSE[material_ID][2];
	specular.b = SPECULAR[material_ID][2];

	shininess = SHININESS[material_ID];

	reflect = Color3(0,0,0);
	refract = Color3(0,0,0);
	refract_index = 1;
}

Material& Material::operator = (const Material& m) {
	ambient = m.ambient;
	diffuse = m.diffuse;
	specular = m.specular;
	shininess = m.shininess;
	reflect = m.reflect;
	refract = m.refract;
	refract_index = m.refract_index;
	return (*this);
};