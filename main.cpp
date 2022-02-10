#include<stdio.h>
#include<stdlib.h>
#include<math.h>

#include <windows.h>
#include <glut.h>

#define pi (2*acos(0.0))

double cameraHeight;
double cameraAngle;
int drawgrid;
int drawaxes;
double angle;

struct point
{
	double x,y,z;
};


struct vector
{
    public:
        double x,y,z;

        vector rotate(double angle){
            struct vector perp, ans;
            perp.x= -z; perp.y= y; perp.z=x;
            ans.x= x*cos(angle)+ perp.x*sin(angle);
            ans.y= y*cos(angle)+ perp.y*sin(angle);
            ans.z= z*cos(angle)+ perp.z*sin(angle);
            return ans;
        }

};

//my wheel
struct point pos1[100];
float theta=0;
float cx=0, cy=0, cz=0;             //wheel center
int rad=40;
struct vector forwardVector;     //forwardVector for wheel

void drawAxes()
{
	if(drawaxes==1)
	{
		glColor3f(1.0, 1.0, 1.0);
		glBegin(GL_LINES);{
			glVertex3f( 100,0,0);
			glVertex3f(-100,0,0);

			glVertex3f(0,-100,0);
			glVertex3f(0, 100,0);

			glVertex3f(0,0, 100);
			glVertex3f(0,0,-100);
		}glEnd();
	}
}


void drawGrid()
{
	int i;
	if(drawgrid==1)
	{
		glColor3f(0.6, 0.6, 0.6);	//grey
		glBegin(GL_LINES);{
			for(i=-30;i<=30;i++){

				//if(i==0)
					//continue;	//SKIP the MAIN axes

				//lines parallel to Y-axis
				//glVertex3f(i*15, -500, -1*rad);
				//glVertex3f(i*15,500, -1*rad);

				//lines parallel to X-axis
				glVertex3f(-500, -1*rad, i*15);
				glVertex3f( 500, -1*rad, i*15);

				glVertex3f(i*15, -1*rad, -500);        //parallel to z
				glVertex3f(i*15, -1*rad, 500);
			}
		}glEnd();
	}
}



int centerx=0, centery=0;
void drawWheelByCircle(double radius,int segments)
{
    int i;
    struct point points[100];
    glColor3f(0.7,0.7,0.7);
    //generate points
    for(i=0;i<=segments;i++)
    {
        points[i].x=radius*cos(((double)i/(double)segments)*2*pi);      // theta= s/r; = 2*pi*r*(i/segments)/r-> each i koto tuku kore jayga akbe/ kototuku theta nibe
        points[i].y=radius*sin(((double)i/(double)segments)*2*pi);
    }
    //draw segments using generated points
    double shade=2;
    for(i=0;i<segments;i++)
    {
        if(i<segments/2)  shade=2*(double)i/(double)segments;
        else shade=2*(1.0-(double)i/(double)segments);
        glColor3f(shade,shade,shade);
        glBegin(GL_QUADS);
        {
			glVertex3f(points[i].x,points[i].y,0);
			glVertex3f(points[i+1].x,points[i+1].y,0);
			glVertex3f(points[i+1].x,points[i+1].y,30);
			glVertex3f(points[i].x,points[i].y,30);

        }
        glEnd();
    }



}

int fr=0;
float rotAngle=0.0;
//float theta=0.0;        //sir said
int fmove=0;
float prevcx, prevcz, dist, prevcy;
float extraAngle=0.0;
void keyboardListener(unsigned char key, int x,int y){
	switch(key){

		case '1':
			drawgrid=1-drawgrid;
			break;

        case 'a':       //rotate like earth
                fr=1;
                rotAngle+=4.0;
                //forwardVector.x= cos(rotAngle);
                //forwardVector.y= sin(rotAngle);
                break;
        case 'd':       //rotate like earth
                fr=1;
                rotAngle-=4.0;
                //forwardVector.x= cos(rotAngle);
                //forwardVector.y= sin(rotAngle);
                break;

        case 'w':       //move forward
                prevcx=cx; prevcy=cy, prevcz=cz;
                cx += forwardVector.x; cy+=forwardVector.y;
                //cy =cx* (forwardVector.y/forwardVector.x) ;      //st. line eqn
                cz += forwardVector.z;//cx* (forwardVector.z/forwardVector.x) ;
                fmove=1;
                dist=  sqrt( (prevcx-cx)*(prevcx-cx) + (prevcy-cy)*(prevcy-cy)+ (prevcz-cz)*(prevcz-cz) );

                theta -= dist/rad;
                //cx= prevcx+ cos(extraAngle);
                //cz= prevcz+ sin(extraAngle);
                //cx=prevcx; cy=prevcy; cz=prevcz;
                break;

         case 's':       //move forward
                prevcx=cx; prevcy=cy; prevcz=cz;
                cx -= forwardVector.x;   cy-=forwardVector.y;
                //cy = 0.0;//cx* (forwardVector.y/forwardVector.x) ;      //st. line eqn
                //cz=  prevcz+ ((cx-prevcx)* ( (forwardVector.y-prevcz)/(forwardVector.x-prevcx))) ;
                //cy=  cx* (forwardVector.y/forwardVector.x) ;
                cz -= forwardVector.z;//cx* (forwardVector.z/forwardVector.x) ;
                fmove=1;
                dist=  sqrt( (prevcx-cx)*(prevcx-cx) + (prevcy-cy)*(prevcy-cy)+ (prevcz-cz)*(prevcz-cz) );

                theta += dist/rad;
                //cx=prevcx; cy=prevcy; cz=prevcz;
                break;
		default:
			break;
	}
}


void specialKeyListener(int key, int x,int y){
	switch(key){
		case GLUT_KEY_DOWN:		//down arrow key
			cameraHeight -= 3.0;
			break;
		case GLUT_KEY_UP:		// up arrow key
			cameraHeight += 3.0;
			break;

		case GLUT_KEY_RIGHT:
			cameraAngle += 0.03;
			break;
		case GLUT_KEY_LEFT:
			cameraAngle -= 0.03;
			break;

		case GLUT_KEY_PAGE_UP:
			break;
		case GLUT_KEY_PAGE_DOWN:
			break;

		case GLUT_KEY_INSERT:
			break;

		case GLUT_KEY_HOME:
			break;
		case GLUT_KEY_END:
			break;

		default:
			break;
	}
}


void mouseListener(int button, int state, int x, int y){	//x, y is the x-y of the screen (2D)
	switch(button){
		case GLUT_LEFT_BUTTON:
			if(state == GLUT_DOWN){		// 2 times?? in ONE click? -- solution is checking DOWN or UP
				drawaxes=1-drawaxes;
			}
			break;

		case GLUT_RIGHT_BUTTON:
			//........
			break;

		case GLUT_MIDDLE_BUTTON:
			//........
			break;

		default:
			break;
	}
}

//****************************start******************************


void wheel( ){

    int segments=30;

    if(fr==1){
            //forwardVector.x= rad*cos(rotAngle);
            //forwardVector.y= 0;//rad*sin(rotAngle);
           // forwardVector.z= rad*sin(rotAngle);
            //
            //extraAngle+=rotAngle; fmove=0;
            forwardVector.rotate(4.0);
            glTranslatef(cx,cy,0);
            glRotatef(rotAngle,0,1,0);
            glTranslatef(-cx,-cy,0);
    }

    /*if(fmove==1){ fr=0;
            glTranslatef(cx,cy,cz);
            glRotatef(extraAngle,0,1,0);
            glTranslatef(-cx,-cy,-cz);
    }*/

    glPushMatrix();{
        for(int i=0;i<=segments;i++)
        {
            pos1[i].x=cx+ rad*cos( ( ((double)i/(double)segments)*2*pi) + theta); //x     // theta= s/r; = 2*pi*r*(i/segments)/r-> each i koto tuku kore jayga akbe/ kototuku theta nibe
            pos1[i].y=cz+ rad*sin( ( ((double)i/(double)segments)*2*pi) + theta); //z
        }
        float shade;
        int width=10;           //wheel koto mota hbe
        int pwidth= width/2;    //wheel er majher line 2tar width
        for(int i=0;i<segments;i++)
        {
            if(i<segments/2)  shade=2*(double)i/(double)segments;
            else shade=2*(1.0-(double)i/(double)segments);
            glColor3f(shade,shade,shade);


            glBegin(GL_QUADS);
            {
                glVertex3f(pos1[i].x,pos1[i].y,cz+width);
                glVertex3f(pos1[i+1].x,pos1[i+1].y,cz+width);
                glVertex3f(pos1[i+1].x,pos1[i+1].y,cz-width);
                glVertex3f(pos1[i].x,pos1[i].y,cz-width);

            }
            glEnd();
        }

        glColor3f(0.658824,0.658824,0.658824);  //lightgrey
        glBegin(GL_QUADS);                      //horizontal+vertical plate
        {

                glVertex3f(cx+rad*cos(theta),cy+rad*sin(theta),cz+pwidth);
                glVertex3f(cx+rad*cos(theta),cy+rad*sin(theta),cz-pwidth);
                glVertex3f(cx-rad*cos(theta),cy-rad*sin(theta),cz-pwidth);
                glVertex3f(cx-rad*cos(theta),cy-rad*sin(theta),cz+pwidth);


                /*glVertex3f(cx+rad,rad*theta,pwidth);
                glVertex3f(cx+rad,rad*theta,-pwidth);
                glVertex3f(cx-rad,-rad*theta,-pwidth);
                glVertex3f(cx-rad,-rad*theta,pwidth);*/    //maybe vul

                //vertical
                glVertex3f(cx+rad*cos(3.1416/2+theta),cy+rad*sin(3.1416/2+theta),cz+pwidth);
                glVertex3f(cx+rad*cos(3.1416/2+theta),cy+rad*sin(3.1416/2+theta),cz-pwidth);
                glVertex3f(cx-rad*cos(3.1416/2+theta),cy-rad*sin(3.1416/2+theta),cz-pwidth);
                glVertex3f(cx-rad*cos(3.1416/2+theta),cy-rad*sin(3.1416/2+theta),cz+pwidth);

        }
         glEnd();
        }
    glPopMatrix();

}




void display(){

	//clear the display
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0,0,0,0);	//color black
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	/********************
	/ set-up camera here
	********************/
	//load the correct matrix -- MODEL-VIEW matrix
	glMatrixMode(GL_MODELVIEW);

	//initialize the matrix
	glLoadIdentity();

	//now give three info
	//1. where is the camera (viewer)?
	//2. where is the camera looking?
	//3. Which direction is the camera's UP direction?

	//gluLookAt(100,100,100,	0,0,0,	0,0,1);
	gluLookAt(200*cos(cameraAngle), cameraHeight, 200*sin(cameraAngle),		0,0,0,		0,1,0);
	//gluLookAt(50,50,100,	0,0,0,	0,1,0);


	//again select MODEL-VIEW
	glMatrixMode(GL_MODELVIEW);


	/****************************
	/ Add your objects from here
	****************************/
	//add objects

	//drawAxes();
	drawgrid=1;
	drawGrid();

    //glColor3f(1,0,0);
    //drawSquare(10);

    //drawSS();

    //drawCircle(30,80);          //wheel
    wheel();

    //drawCone(20,50,24);

	//drawSphere(30,24,20);           //radius, slices, stack;

    //glutSolidTorus(20,50,5,3);

	//ADD this line in the end --- if you use double buffer (i.e. GL_DOUBLE)
	glutSwapBuffers();
}


void animate(){
	angle+=0.05;
	//codes for any changes in Models, Camera
	glutPostRedisplay();
}

void init(){
	//codes for initialization
	drawgrid=0;
	drawaxes=1;
	cameraHeight=150.0;
	cameraAngle=1.0;
	angle=0;

	//clear the screen
	glClearColor(0,0,0,0);

	/************************
	/ set-up projection here
	************************/
	//load the PROJECTION matrix
	glMatrixMode(GL_PROJECTION);

	//initialize the matrix
	glLoadIdentity();

	//give PERSPECTIVE parameters
	gluPerspective(80,	1,	1,	1000.0);
	//field of view in the Y (vertically)
	//aspect ratio that determines the field of view in the X direction (horizontally)
	//near distance
	//far distance

	//ME: X er dike mukh kore aktesi, so forwardVector set kore dei
	forwardVector.x= 1;
	forwardVector.y=0;
	forwardVector.z=0;

}

int main(int argc, char **argv){
	glutInit(&argc,argv);
	glutInitWindowSize(500, 500);
	glutInitWindowPosition(0, 0);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGB);	//Depth, Double buffer, RGB color

	glutCreateWindow("My OpenGL Program");

	init();

	glEnable(GL_DEPTH_TEST);	//enable Depth Testing

	glutDisplayFunc(display);	//display callback function
	glutIdleFunc(animate);		//what you want to do in the idle time (when no drawing is occuring)

	glutKeyboardFunc(keyboardListener);
	glutSpecialFunc(specialKeyListener);
	glutMouseFunc(mouseListener);

	glutMainLoop();		//The main loop of OpenGL

	return 0;
}
