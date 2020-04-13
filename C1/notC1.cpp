#include <GL/glut.h>
GLfloat xRotated, yRotated, zRotated;

void init(void){
        glClearColor(0,0,0,0);
}

void DrawCube(void){
        glMatrixMode(GL_MODELVIEW);
        glClear(GL_COLOR_BUFFER_BIT);   //clears the drawing buffer
        glLoadIdentity();
        glTranslatef(0.0, 0.0, -10.5);
        glRotatef(xRotated, 1.0, 0.0, 0.0);     //Rotate about x-axis
        glRotatef(yRotated, 0.0, 1.0, 0.0);     //Rotate about y-axis
        glRotatef(zRotated, 0.0, 0.0, 1.0);     //Rotate about z-axis
        glBegin(GL_QUADS);      //Draw the cube using quads
                
                glColor3f(0.0f,1.0f,0.0f);    // Color Green
                glVertex3f( 1.0f, 1.0f,-1.0f);    // Top Right Of The Quad (Top)
                glVertex3f(-1.0f, 1.0f,-1.0f);    // Top Left Of The Quad (Top)
                glVertex3f(-1.0f, 1.0f, 1.0f);    // Bottom Left Of The Quad (Top)
                glVertex3f( 1.0f, 1.0f, 1.0f);    // Bottom Right Of The Quad (Top)
                
                glColor3f(0.0f,1.0f,1.0f);    // Color Cyan
                glVertex3f( 1.0f,-1.0f, 1.0f);    // Top Right Of The Quad (Bottom)
                glVertex3f(-1.0f,-1.0f, 1.0f);    // Top Left Of The Quad (Bottom)
                glVertex3f(-1.0f,-1.0f,-1.0f);    // Bottom Left Of The Quad (Bottom)
                glVertex3f( 1.0f,-1.0f,-1.0f);    // Bottom Right Of The Quad (Bottom)
                
                glColor3f(1.0f,0.0f,0.0f);    // Color Red    
                glVertex3f( 1.0f, 1.0f, 1.0f);    // Top Right Of The Quad (Front)
                glVertex3f(-1.0f, 1.0f, 1.0f);    // Top Left Of The Quad (Front)
                glVertex3f(-1.0f,-1.0f, 1.0f);    // Bottom Left Of The Quad (Front)
                glVertex3f( 1.0f,-1.0f, 1.0f);    // Bottom Right Of The Quad (Front)
                
                glColor3f(1.0f,1.0f,0.0f);    // Color Yellow
                glVertex3f( 1.0f,-1.0f,-1.0f);    // Top Right Of The Quad (Back)
                glVertex3f(-1.0f,-1.0f,-1.0f);    // Top Left Of The Quad (Back)
                glVertex3f(-1.0f, 1.0f,-1.0f);    // Bottom Left Of The Quad (Back)
                glVertex3f( 1.0f, 1.0f,-1.0f);    // Bottom Right Of The Quad (Back)
                
                glColor3f(0.0f,0.0f,1.0f);    // Color Blue
                glVertex3f(-1.0f, 1.0f, 1.0f);    // Top Right Of The Quad (Left)
                glVertex3f(-1.0f, 1.0f,-1.0f);    // Top Left Of The Quad (Left)
                glVertex3f(-1.0f,-1.0f,-1.0f);    // Bottom Left Of The Quad (Left)
                glVertex3f(-1.0f,-1.0f, 1.0f);    // Bottom Right Of The Quad (Left)
                
                glColor3f(1.0f,0.0f,1.0f);    // Color Magenta
                glVertex3f( 1.0f, 1.0f,-1.0f);    // Top Right Of The Quad (Right)
                glVertex3f( 1.0f, 1.0f, 1.0f);    // Top Left Of The Quad (Right)
                glVertex3f( 1.0f,-1.0f, 1.0f);    // Bottom Left Of The Quad (Right)
                glVertex3f( 1.0f,-1.0f,-1.0f);    // Bottom Right Of The Quad (Right)
        
        glEnd();            // End Drawing The Cube
        glFlush();
        
}

void animation(void)
{
 
        yRotated += 0.01;
        xRotated += 0.02;
        DrawCube();
}


void reshape(int x, int y)
{
        if (y == 0 || x == 0) return;   //Nothing is visible then, so return
        glMatrixMode(GL_PROJECTION);    //Set a new projection matrix  
        glLoadIdentity();
        
        //Angle of view:40 degrees
        //Near clipping plane distance: 0.5
        //Far clipping plane distance: 20.0

        gluPerspective(40.0,(GLdouble)x/(GLdouble)y,0.5,20.0);
        glMatrixMode(GL_MODELVIEW);
        glViewport(0,0,x,y);  //Use the whole window for rendering
}

int main(int argc, char** argv){

        glutInit(&argc, argv);
        //Initialize the glut functions
        glutInitDisplayMode(GLUT_SINGLE|GLUT_RGB);
        glutInitWindowPosition(100, 100);
        glutCreateWindow(argv[0]);
        init();
        glutDisplayFunc(DrawCube);
        glutReshapeFunc(reshape);
        //Set the function for the animation.
        glutIdleFunc(animation);
        glutMainLoop();
        return 0;
}
