#include <stdlib.h> 
#include <GL/glut.h>
#include <stdio.h>
#include <math.h>
#include <time.h>

#include <iostream>
#include <vector>
using namespace std;

#define FPS 40
//#define RAND_MAX 33452

struct point {
	float x, y, z;
};

const  float PI = 3.14;
//const  float PI = (4.0*atan(1.0));
bool paused = false;
static vector<point> path;
float ptx, pty, ptz;
float new_ptx, new_pty, new_ptz;



float rotation = 0;
float axis[3] = {0.0,1.0,1.0};

int randomInt (int low, int high) { 
	//cout << "max " << RAND_MAX;
	//int range=(high-low)+1;
	//return low+int(range*rand()/(RAND_MAX + 1.0));
	return low + rand() % (high-low) + 1;
}

float randomAngle() {
	// choose random radains between 0 and 2PI
	float random = (float)randomInt(0, (int)(PI*2*100));

	return random/100;
}

void drawPath() {
	glColor4f(1.0, 0.0, 1.0, 1.0);
   // for loop draws the full path of the particle
   for (int i=0; i<path.size()-1; i++) {
	   glBegin(GL_LINES);
	   	glVertex3f(path[i].x, path[i].y, path[i].z);
	   	glVertex3f(path[i+1].x, path[i+1].y, path[i+1].z);
	   glEnd();
   }
}

void drawParticle() {
	glColor4f(0.0, 0.0, 1.0, 1.0);
	glTranslatef(ptx, pty, ptz);
	glutSolidCube(0.05);
	glTranslatef(-ptx, -pty, -ptz);
}

void drawParticleAndPath() {
	point pt;
	pt.x = ptx;
	pt.y = pty;
	pt.z = ptz;
	path.push_back(pt);
	if (!paused) {

		int range = 100;
		
		new_ptx = (float)randomInt(-100*range, 100*range)/100;
		new_pty = (float)randomInt(-100*range, 100*range)/100;
		new_ptz = (float)randomInt(-100*range, 100*range)/100;
		
		
		// convert into unit vector
		float length = sqrt( (new_ptx-ptx)*(new_ptx-ptx) + (new_pty-pty)*(new_pty-pty) + (new_ptz-ptz)*(new_ptz-ptz) ) ;

		float magnitude = 1.0/50.0;

		ptx = ptx+(new_ptx-ptx)/length*magnitude;
		pty = pty+(new_pty-pty)/length*magnitude;
		ptz = ptz+(new_ptz-ptz)/length*magnitude;

		
	}
	
	drawPath();
	drawParticle();
}

void drawAxis() {
	// draw axis
	glBegin(GL_LINES);
		// y white
		glColor4f(1.0f,1.0f,1.0f,1.0);
		glVertex3f( 0.0f,-7.0f,0.0f);
		glVertex3f( 0.0f, 7.0f,0.0f);
		// x pink
		glColor4f(1.0f,0.5f,0.5f,1.0);
		glVertex3f(-7.0f, 0.0f,0.0f);
		glVertex3f( 7.0f, 0.0f,0.0f);
		// z green
		glColor4f(0.5f,1.0f,0.5f,1.0);
		glVertex3f( 0.0f, 0.0f,-7.0f);
		glVertex3f( 0.0f, 0.0f, 7.0f);
	glEnd();
}

void display() {

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	glTranslatef(0.0, 0.0, -5.0);
	glRotatef(rotation, axis[0], axis[1], axis[2]);
	
	drawParticleAndPath();
	drawAxis();
	
	glutSwapBuffers();
}


void reshape(int w, int h) {
	if (h==0)										// Prevent A Divide By Zero By
		h=1;										// Making Height Equal One

	glViewport(0,0,w,h);						// Reset The Current Viewport

	glMatrixMode(GL_PROJECTION);						// Select The Projection Matrix
	glLoadIdentity();									// Reset The Projection Matrix

	// Calculate The Aspect Ratio Of The Window
	gluPerspective(45.0f,(GLfloat)w/(GLfloat)h,0.1f,100.0f);

	glMatrixMode(GL_MODELVIEW);							// Select The Modelview Matrix
	glLoadIdentity();	
}

void keyboardHandler(unsigned char key, int x, int y) {
	printf("Pressed %c (code %d) key...\n", key, (int)key);
}

void mouseHandler(int button, int state, int x, int y) {
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
		paused = false;
		//trackMouse = TRUE;
		// init beginning mouse positions
		//x_last = x;
		//y_last = y;
	} else if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN)
		paused = true;
		//trackMouse = FALSE;
}

void motionHandler (int x, int y) {

}

void timer(int value) {
	glutPostRedisplay();
	rotation += 2.0;
	glutTimerFunc(1000/FPS, timer, 0);
}

void initGL() {
	glShadeModel(GL_SMOOTH);							// Enable Smooth Shading
	glClearColor(0.0f, 0.0f, 0.0f, 0.5f);				// Black Background
	glClearDepth(1.0f);									// Depth Buffer Setup
	glEnable(GL_DEPTH_TEST);							// Enables Depth Testing
	glDepthFunc(GL_LEQUAL);								// The Type Of Depth Testing To Do
	glEnable(GL_POLYGON_SMOOTH);						// makes edges nice and anti-aliased
	glEnable(GL_LINE_SMOOTH);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);	// Really Nice Perspective Calculations
	
	int range = 1;
	ptx = (float)randomInt(-100*range, 100*range)/100;
	pty = (float)randomInt(-100*range, 100*range)/100;
	ptz = (float)randomInt(-100*range, 100*range)/100;
}

int main(int argc, char** argv) {
	srand((unsigned)time(0)); // needed for random numbers
	//Polyhedron x = initPolyhedron();

	glutInit(&argc, argv);
	glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowSize (640, 480); 
	glutInitWindowPosition (600, 300);
	glutCreateWindow (argv[0]);
	glEnable (GL_BLEND); 
	glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	initGL ();

	glutDisplayFunc(display); 
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keyboardHandler);
	glutMouseFunc(mouseHandler);
	glutMotionFunc(motionHandler);
	glutTimerFunc(1000/FPS, timer, 0);

	glutMainLoop();
	cout << "Between 0 and " << PI*2 << endl;
    for(int index=0; index<20; index++){
        cout << randomInt(0, 10) << endl; 
    }

	return 1;
}
