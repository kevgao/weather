#include "weather.h"

void initParticle(GLuint file){
    //partTexture = texture[9]
	//partTexture = file;
	for (loop=0;loop<MAX_PARTICLES;loop++){
		particle[loop].active=true;				
		particle[loop].life=0.5f;										      
		particle[loop].fade=10.0;
		particle[loop].r=174.0/255.0;
		particle[loop].g=237.0/255.0;
		particle[loop].b=69.0/255.0;
		particle[loop].x=(float(rand()%25+19));	
		particle[loop].y=(float(rand()%20));	
		particle[loop].z=-float(rand()%28)+4;	
		
		particle[loop].xi=float((rand()%50)-25.0f)*10.0f;
		particle[loop].yi=float((rand()%50)-25.0f)*10.0f;
		particle[loop].zi=float((rand()%50)-25.0f)*10.0f;
	}
	for (loop=0;loop<MAX_PARTICLES;loop++){
		particleStar[loop].active=true;				
		particleStar[loop].life=1.0f;										      
		particleStar[loop].fade=10.0;
		particleStar[loop].r=1.0;
		particleStar[loop].g=1.0;
		particleStar[loop].b=1.0;	
		particleStar[loop].x=(float(rand()%600)-220);	
		particleStar[loop].y=100.0f;	
		particleStar[loop].z=(float(rand()%520)-220);	
	}
	for (loop=0;loop<MAX_PARTICLES+800;loop++){
		particleRain[loop].active=true;				
		particleRain[loop].life=1.0f;										      
		particleRain[loop].fade=10.0;
		particleRain[loop].r=1.0;
		particleRain[loop].g=0.5;
		particleRain[loop].b=0.5;
		particleRain[loop].xi=0;		
		particleRain[loop].yi=5;	
		particleRain[loop].zi=0;				   
		particleRain[loop].xg=0.0f;					
		particleRain[loop].yg=-0.9f;				
		particleRain[loop].zg=0.0f;
	}
	for (loop=0;loop<MAX_PARTICLES+800;loop++){
		particleSnow[loop].active=true;				
		particleSnow[loop].life=0.6f;										      
		particleSnow[loop].fade=float(rand()%100)/1000.0f;	 
		particleSnow[loop].r=0.5;
		particleSnow[loop].g=0.5;
		particleSnow[loop].b=0.5;
		particleSnow[loop].x=(float(rand()%200)-64);	
		particleSnow[loop].y=(float(rand()%10)+55);	
		particleSnow[loop].z=float(rand()%160)-64;	
		particleSnow[loop].xi=float((rand()%100)-50.0f);
		particleSnow[loop].yi=-float((rand()%30)+60);
		particleSnow[loop].zi=0;				   
		particleSnow[loop].xg=0.3f;					
		particleSnow[loop].yg=-0.9f;				
		particleSnow[loop].zg=0.0f;
	}
}

void drawParticle(){
	
	glEnable(GL_BLEND);	
	glDisable(GL_DEPTH_TEST);
	glBlendFunc(GL_SRC_ALPHA,GL_ONE);
	
	for (loop=0;loop<MAX_PARTICLES;loop++){
		if (particle[loop].active){

	        float x=particle[loop].x;		
			float y=particle[loop].y;		
			float z=particle[loop].z+zoom;		
			glColor4f(particle[loop].r,particle[loop].g,particle[loop].b,particle[loop].life);
			glBindTexture(GL_TEXTURE_2D, partTexture);
			glBegin(GL_TRIANGLE_STRIP);		
		    glTexCoord2d(1,1); glVertex3f(x+0.3f,y+0.3f,z); 
			glTexCoord2d(0,1); glVertex3f(x-0.3f,y+0.3f,z); 
			glTexCoord2d(1,0); glVertex3f(x+0.3f,y-0.3f,z); 
			glTexCoord2d(0,0); glVertex3f(x-0.3f,y-0.3f,z); 
			glEnd();				
			particle[loop].x+=particle[loop].xi/(slowdown*20000);
			particle[loop].y+=particle[loop].yi/(slowdown*20000);
			particle[loop].z+=particle[loop].zi/(slowdown*20000);
			particle[loop].xi+=particle[loop].xg;	
			particle[loop].yi+=particle[loop].yg;	
			particle[loop].zi+=particle[loop].zg;	
			particle[loop].life-=0.1*particle[loop].fade;


		    if (particle[loop].life<0.0f){

			    particle[loop].life=0.5f;	
			    particle[loop].fade=float(rand()%100)/1000.0f;
			    particle[loop].x=(float(rand()%23+21));	
			    particle[loop].y=(float(rand()%10+2));	
			    particle[loop].z=float(rand()%24+1);	
				
			    particle[loop].xi=float((rand()%50)-25.0f)*10.0f;
			    particle[loop].yi=float((rand()%50)-25.0f)*10.0f;
			    particle[loop].zi=float((rand()%50)-25.0f)*10.0f;
	        }
        }
    }
	
		for (loop=0;loop<MAX_PARTICLES;loop++)
	{
		if (particleStar[loop].active)
		{

	        float x=particleStar[loop].x;		
			float y=particleStar[loop].y;		
			float z=particleStar[loop].z;		
			glBindTexture(GL_TEXTURE_2D, partTexture);
			glColor4f(particleStar[loop].r,particleStar[loop].g,particleStar[loop].b,particleStar[loop].life);
			glBegin(GL_TRIANGLE_STRIP);		
		    glTexCoord2d(1,1); glVertex3f(x+0.9f,y,z+0.9f); 
			glTexCoord2d(0,1); glVertex3f(x-0.9f,y,z+0.9f); 
			glTexCoord2d(1,0); glVertex3f(x+0.9f,y,z-0.9f); 
			glTexCoord2d(0,0); glVertex3f(x-0.9f,y,z-0.9f); 
			glEnd();				
			particleStar[loop].life-=0.1*particleStar[loop].fade;

		if (particleStar[loop].life<0.0f)				
			{
				particleStar[loop].life=0.35f;	
				particleStar[loop].fade=float(rand()%100)/1000.0f+0.005f;
				particleStar[loop].x=(float(rand()%600)-220);	
				particleStar[loop].y=100.0f;	
				particleStar[loop].z=(float(rand()%520)-220);	
			}

		}}
		
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_BLEND);		
	
}


void snow(){
	glPushMatrix();
	
	glEnable(GL_BLEND);	
	glDisable(GL_DEPTH_TEST);
	glBlendFunc(GL_SRC_ALPHA,GL_ONE);

	for (loop=0;loop<100;loop++){
        std::cout << "snow" << std::endl;
		if (particleSnow[loop].active){

	        float x=particleSnow[loop].x;		
			float y=particleSnow[loop].y;		
			float z=particleSnow[loop].z+zoom;		
			glColor4f(particleSnow[loop].r,particleSnow[loop].g,particleSnow[loop].b,particleSnow[loop].life);
			glBindTexture(GL_TEXTURE_2D, texture[9]);
			glBegin(GL_TRIANGLE_STRIP);		
		    glTexCoord2d(1,1); glVertex3f(x+0.8f,y+0.8f,z); 
			glTexCoord2d(0,1); glVertex3f(x-0.8f,y+0.8f,z); 
			glTexCoord2d(1,0); glVertex3f(x+0.8f,y-0.8f,z); 
			glTexCoord2d(0,0); glVertex3f(x-0.8f,y-0.8f,z); 
			glEnd();				
			particleSnow[loop].x+=particleSnow[loop].xi/(slowdown*300);
			particleSnow[loop].y+=particleSnow[loop].yi/(slowdown*300);
			particleSnow[loop].z+=particleSnow[loop].zi/(slowdown*300);
			particleSnow[loop].xi+=particleSnow[loop].xg;	
			particleSnow[loop].yi+=particleSnow[loop].yg;	
			particleSnow[loop].zi+=particleSnow[loop].zg;	
			particleSnow[loop].life-=0.3*particleSnow[loop].fade;

			/*if(particle[loop].y<-3.0f)
			{
               particle[loop].xi=particle[loop].xi;	
	        	particle[loop].yi=-particle[loop].yi;		
			}*/


		if (particleSnow[loop].life<0.0f){
				particleSnow[loop].life=0.6f;	
				particleSnow[loop].fade=float(rand()%100)/1000.0f;
				particleSnow[loop].x=(float(rand()%200)-64);	
				particleSnow[loop].y=(float(rand()%10)+55);	
				particleSnow[loop].z=float(rand()%160)-64;	
				particleSnow[loop].xi=float((rand()%100)-50.0f);
				particleSnow[loop].yi=-float((rand()%30)+60);
				particleSnow[loop].zi=0;	//float((rand()%50)-25.0f)*10.0f;
			}

		}
	}
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_BLEND);
	glPopMatrix();
}




