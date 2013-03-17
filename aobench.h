#ifndef AOBENCH_H_INCLUDED
#define AOBENCH_H_INCLUDED

#include "tinymath.h"

typedef struct _Ray {
	vec	org;
	vec	dir;
} Ray;

typedef struct _Isect {
	Fixed t;
	vec	p;
	vec	n;
	int	hit;
} Isect;

typedef struct _Sphere {
	vec   center;
	Fixed radius;
} Sphere;

typedef struct _Plane {
	vec	p;
	vec	n;
} Plane;

typedef struct _Scene {
    Sphere spheres[3];
    Plane plane;
} Scene;

void aoInit(int w, int h, int nao, Scene* pScene);
void aoRender(FBPixel* pFrameBuffer, int fb_pitch);
void ray_sphere_intersect(Isect *isect, const Ray *ray, const Sphere *sphere, int hittest_only);
void ray_plane_intersect(Isect *isect, const Ray *ray, const Plane *plane, int hittest_only);
short ambient_occlusion(const Isect *isect, Sphere* spheres, Plane* plane);

#endif