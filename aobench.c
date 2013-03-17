// AOBench implementation with fixed point numbers
// #include <stdio.h>
#include "Types.h"
#include "aobench.h"
#define INFINI_FAR (0x7fffffffL)
#define MAKE_GRAYSCALE(intencity) ((intencity) << 11) | ((intencity) << 6) | (intencity)

static int sPictureWidth;
static int sPictureHeight;
static int sNumAOSamples;
static Scene* spScene;

extern void onPixelRendered();

void aoInit(int w, int h, int nao, Scene* pScene) {
    sPictureWidth  = w;
    sPictureHeight = h;
    sNumAOSamples  = nao;
    spScene = pScene;
}

void aoRender(FBPixel* pFrameBuffer, int fb_pitch) {
	int x, y, i;
    long fbPos;
	Fixed hw, hh;
	Fixed px, py;
	Ray ray; // Primary ray
	Isect isect; // Hit info of primary ray
    Sphere* spheres = spScene->spheres;
    Plane* plane = &spScene->plane;

	hw = sPictureWidth  >> 1;
	hh = sPictureHeight >> 1;

    fbPos = 0;
	for (y = 0;y < sPictureHeight;++y) {
		py  = -(y - hh);
		py *= FIX_SCALE;
		py /= hh;
        fbPos = (long)fb_pitch * (long)y;

		for (x = 0;x < sPictureWidth;++x) {
			px = x - hw;
			px *= FIX_SCALE;
			px /= hw;
            
			// Setup primary ray
			ray.org.x = 0;
			ray.org.y = 0;
			ray.org.z = 0;
            
            // 0 , 0 , -1.0
			ray.dir.x = px;
			ray.dir.y = py;
			ray.dir.z = -FIX_SCALE;
            
			vnormalize(&(ray.dir));
            
			// Cast primary ray
			isect.t = INFINI_FAR;
			isect.hit = 0;
            
			ray_sphere_intersect(&isect, &ray, &spheres[0], 0);
            ray_plane_intersect (&isect, &ray, plane, 0);
			onPixelRendered(); // to suppress screen flashing

			if (isect.hit) { // Primary ray hit!
                
                // Cast secondary rays to calculate pixel color
				int intencity = (int)ambient_occlusion(&isect, spheres, plane);
				pFrameBuffer[fbPos] = (FBPixel)(MAKE_GRAYSCALE(intencity));
            }

			onPixelRendered();

            // Move write pos
            ++fbPos;
        }
    }
}


void ray_sphere_intersect(Isect *isect, const Ray *ray, const Sphere *sphere, int hittest_only) {
	vec rs;
	Fixed B, C, D;
	
	rs.x = ray->org.x - sphere->center.x;
	rs.y = ray->org.y - sphere->center.y;
	rs.z = ray->org.z - sphere->center.z;
	B = vdot(rs, ray->dir);
    
	C = vdot(rs, rs) - (sphere->radius * sphere->radius) / FIX_SCALE;
	D = (B * B)/FIX_SCALE - C;
    
	if (D > 0) {
		Fixed t = -B - fxsqrt(D);
		
		if ((t > 0) && (t < isect->t)) {
			isect->t = t;
			isect->hit = 1;
			if (hittest_only) { return; }
            
			// Hit position
			isect->p.x = ray->org.x + (ray->dir.x * t) / FIX_SCALE;
			isect->p.y = ray->org.y + (ray->dir.y * t) / FIX_SCALE;
			isect->p.z = ray->org.z + (ray->dir.z * t) / FIX_SCALE;
			
			// N
			isect->n.x = isect->p.x - sphere->center.x;
			isect->n.y = isect->p.y - sphere->center.y;
			isect->n.z = isect->p.z - sphere->center.z;
            
			vnormalize(&(isect->n));
		}
	}
}

void ray_plane_intersect(Isect *isect, const Ray *ray, const Plane *plane, int hittest_only) {
	Fixed t;
	Fixed d = -vdot(plane->p, plane->n);
	Fixed v = vdot(ray->dir, plane->n);
	Fixed av = (v < 0) ? -v : v;
    
	if (av < 8) return;
    
	t = -(vdot(ray->org, plane->n) + d) * FIX_SCALE / v;
    
	if (t > 0 && t < isect->t) {
		isect->t = t;
		isect->hit = 1;
		if (hittest_only) { return; }
        
		isect->p.x = ray->org.x + (ray->dir.x * t) / FIX_SCALE;
		isect->p.y = ray->org.y + (ray->dir.y * t) / FIX_SCALE;
		isect->p.z = ray->org.z + (ray->dir.z * t) / FIX_SCALE;
        
		isect->n = plane->n;
	}
}

void orthoBasis(vec* basis, vec n)
{
    Fixed thresh = FIX_SCALE * 6 / 10;  // 0.6
    basis[2] = n;
    basis[1].x = 0; basis[1].y = 0; basis[1].z = 0;
    
    if ((n.x < thresh) && (n.x > -thresh)) {
        basis[1].x = FIX_SCALE;
    } else if ((n.y < thresh) && (n.y > -thresh)) {
        basis[1].y = FIX_SCALE;
    } else if ((n.z < thresh) && (n.z > -thresh)) {
        basis[1].z = FIX_SCALE;
    } else {
        basis[1].x = FIX_SCALE;
    }
    
    vcross(&basis[0], basis[1], basis[2]);
    vnormalize(&basis[0]);
    
    vcross(&basis[1], basis[2], basis[0]);
    vnormalize(&basis[1]);
}


short ambient_occlusion(const Isect *isect, Sphere* spheres, Plane* plane) {
	Ray ray;
	Isect occIsect;
	vec p;
	vec basis[3];
	Fixed occlusion = 0;
	const Fixed far_limit = 10 * FIX_SCALE;
	int	i, j;
	int	ntheta = sNumAOSamples;
	int	nphi   = sNumAOSamples;
    
	const int r_eps = 512;
	
	p.x = isect->p.x + isect->n.x / r_eps;
	p.y = isect->p.y + isect->n.y / r_eps;
	p.z = isect->p.z + isect->n.z / r_eps;
    
	orthoBasis(basis, isect->n);
    
	onPixelRendered(); // to suppress screen flashing
	for (j = 0; j < ntheta; j++) {
		for (i = 0; i < nphi; i++) {
			const Fixed theta = fxsqrt(fxrand());
			const Fixed phi = 2 * fxrand();
            
			Fixed x = fxcos(phi) * theta / FIX_SCALE;
			Fixed y = fxsin(phi) * theta / FIX_SCALE;
			Fixed z = fxsqrt(FIX_SCALE - ((theta * theta) >> FIX_BITS) );
            
			Fixed rx = (x * basis[0].x + y * basis[1].x + z * basis[2].x) / FIX_SCALE;
			Fixed ry = (x * basis[0].y + y * basis[1].y + z * basis[2].y) / FIX_SCALE;
			Fixed rz = (x * basis[0].z + y * basis[1].z + z * basis[2].z) / FIX_SCALE;
			onPixelRendered(); // to suppress screen flashing
            
			// Make secondary ray
			ray.org = p;
			ray.dir.x = rx;
			ray.dir.y = ry;
			ray.dir.z = rz;
            
			occIsect.t   = INFINI_FAR;
			occIsect.hit = 0;
            
			ray_sphere_intersect(&occIsect, &ray, &spheres[0], 1);
			//ray_sphere_intersect(&occIsect, &ray, &spheres[1], 1);
			//ray_sphere_intersect(&occIsect, &ray, &spheres[2], 1);
			ray_plane_intersect (&occIsect, &ray, plane, 1);
            
			if (occIsect.hit && isect->t <= far_limit) {
				occlusion += FIX_SCALE;
			}

		}
		onPixelRendered(); // to suppress screen flashing
	}
    
	occlusion = (ntheta * nphi * FIX_SCALE - occlusion) / (ntheta * nphi);
	return occlusion * 31 / FIX_SCALE;
}

