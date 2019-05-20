#ifdef __APPLE__
#include <GLUT/glut.h>
#include <GLUT/glew.h>
#else
#include <GL/glew.h>
#include <GL/glut.h>
#endif

#include <stdio.h>
#include <cstring>
#include <math.h>

// ###########################################
// ####### VARIAVEIS
extern unsigned int frame, time, timebase;
extern char fpsStr[15];

extern int WINDOW_H;

// ###########################################
// ####### FUNÇÕES

void printFPS(){	
	int passedTime = time - timebase;
	
	if (passedTime > 1000) {
		float fps = (frame * 1000.0) / passedTime;
		
		// Reset do TEMPO e FRAMECOUNT
		frame = 0;
		timebase = time;	

		// Registar FPS
		sprintf(fpsStr, "%.2f fps", fps);
		glutSetWindowTitle(fpsStr);

		//textDrawing();
	}
}


void textDrawing(){
    int len;

    glColor3f(1,1,1);
    glMatrixMode( GL_PROJECTION );
    glPushMatrix();
    glLoadIdentity();
    gluOrtho2D( 0, 600, 0, 600 );
    glMatrixMode( GL_MODELVIEW );
    glPushMatrix();
    glLoadIdentity();
    glRasterPos2i(10, glutGet(GLUT_WINDOW_HEIGHT) - 25);

	// FPS
    len = strlen(fpsStr);
    for ( int i = 0; i < len; ++i ){
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, fpsStr[i]);
    }
	
    glPopMatrix();
    glMatrixMode( GL_PROJECTION );
    glPopMatrix();
    glMatrixMode( GL_MODELVIEW );
}