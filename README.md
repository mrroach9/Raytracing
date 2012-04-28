RoaChacing
===========

A graphics modeling and rendering framework using raytracing, platform for experiment.

Last Updated: 04/27/2012

Features:

* Loading models from wavefront meshes, storing in DCEL.
* Supporting all transformations of images and camera.
* Object-oriented scene buiding using materials, light sources and cameras.
* Supporting Phong lighting material plus reflection, refraction and refractive index.
* Phong shading with normal vector interpolation on faces.
* Fast and robust raytracing with kd-tree and supersampling.


TODO:
* Approximate illumination estimation with occluded transparent models.
* Fix the bugs with multiple light sources.
* Extending light sources to attenuable, area-light with soft shadow.
* Accelaration using GPU or parallel programing.
* Configuration File System (CFS) and World Description System (WDS), planning to use XML.
* Special effects with cameras: focal length, aperture, etc.

Howto:
* Pull the code from repo, compile with VC 9 or higher version compiler. Tested under Windows.
* Read main.cpp to see the demo of constructing models and scene.
* CImg library is included for generating renderred image.