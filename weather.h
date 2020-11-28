#define	MAX_PARTICLES	200

typedef struct 				
{
	bool	active;				
	float	life;				
	float	fade;				
	float	r;				
	float	g;				
	float	b;				
	float	x;				
	float	y;				
	float	z;				
	float	xi;				
	float	yi;				
	float	zi;				
	float	xg;				
	float	yg;				
	float	zg;				
}particles;	


particles particle[MAX_PARTICLES];
particles particleStar[MAX_PARTICLES];
particles particleRain[MAX_PARTICLES+800];
particles particleSnow[MAX_PARTICLES+800];