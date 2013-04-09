Raytracing Graphic Rendering System
===========

A graphics modeling and rendering framework using raytracing, platform for experiment.

Last Updated: 04/09/2013

###Features

* Loading models from wavefront meshes, storing in DCEL.
* Supporting all transformations of images and camera.
* Object-oriented scene buiding using materials, light sources and cameras.
* Supporting Phong lighting material plus reflection, refraction and refractive index.
* Phong shading with normal vector interpolation on faces.
* Fast and robust raytracing with kd-tree and supersampling.
* Transparency objects with shallow shadows and refraction.

###TODO:
* Extending light sources to attenuable, area-light with soft shadow (radiosity).
* Accelaration using GPU or parallel programing.
* Configuration File System (CFS) and World Description System (WDS), planning to use XML.
* Special effects with cameras: focal length, depth of field, etc.
* Refraction with Snell's law, dispersion, caustics (photon map).

###Howto:
* Pull the code from repo, compile with VC 9 or higher version compiler. Tested under Windows.
* Read main.cpp to see the demo of constructing models and scene.
* CImg library is included for generating renderred image.

###Change Log:
04/09/2013		Cleaned old codes, adding refraction for transparent objects, fixing several bugs.
