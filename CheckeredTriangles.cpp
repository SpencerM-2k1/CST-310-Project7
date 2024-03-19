// This application is a trivial illustration of texture mapping.  It draws
// several triangles, each with a texture mapped on to it.  The same texture
// is used for each triangle, but the mappings vary quite a bit so it looks as
// if each triangle has a different texture.

// GLEW
// #define GLEW_STATIC
// #include <GL/glew.h>

#ifdef __APPLE_CC__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
#include <cstdlib>



// GLFW
#include <GLFW/glfw3.h>

//GL Mathematics
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// GLUT escape key id for readability
#define KEY_ESCAPE 27

// Define a 2 x 2 red and yellow checkered pattern using RGB colors.
#define red {0xff, 0x00, 0x00}
#define yellow {0xff, 0xff, 0x00}
#define magenta {0xff, 0, 0xff}
GLubyte texture[][3] = {
    red, yellow,
    yellow, red,
};

glm::mat4 model = glm::mat4(1.0f);

//Rotation control
GLboolean rotateFigure = false;

//Offset control
GLfloat figureX = 0.0f,
        figureY = 0.0f,
        figureZ = 0.0f;

//Zoom control
GLfloat scale = 1.0f;

// Fixes up camera and remaps texture when window reshaped.
void reshape(int width, int height) {
  glViewport(0, 0, width, height);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(80, GLfloat(width)/height, 1, 40);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  gluLookAt(2, -1, 5, 0, 0, 0, 0, 1, 0);
  glEnable(GL_TEXTURE_2D);
  glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
  glTexImage2D(GL_TEXTURE_2D,
               0,                    // level 0
               3,                    // use only R, G, and B components
               2, 2,                 // texture has 2x2 texels
               0,                    // no border
               GL_RGB,               // texels are in RGB format
               GL_UNSIGNED_BYTE,     // color components are unsigned bytes
               texture);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
}

// Draws three textured triangles.  Each triangle uses the same texture,
// but the mappings of texture coordinates to vertex coordinates is
// different in each triangle.
void display() {
  glfwPollEvents();
  
  glClear(GL_COLOR_BUFFER_BIT);
  glLoadIdentity();
  gluLookAt(2, -1, 5, 0, 0, 0, 0, 1, 0); // Set camera

  //Player-controlled transforms
  //  IMPORTANT REMINDER: Transformations here are applied in "last-to-first" order

  //Offset
  glTranslatef(figureX, figureY, figureZ);

  //Rotation
  static float angle = 0.0f;
  if (rotateFigure)
    angle += 0.01f;
  glRotatef(angle, 0.0f, 0.0f, 1.0f); // Rotate around the y-axis

  //Scale
  glScalef(scale, scale, scale);
  
  //Draw
  glBegin(GL_TRIANGLES);
    glTexCoord2f(0.5, 1.0);    glVertex2f(-3, 3);
    glTexCoord2f(0.0, 0.0);    glVertex2f(-3, 0);
    glTexCoord2f(1.0, 0.0);    glVertex2f(0, 0);

    glTexCoord2f(4, 8);        glVertex2f(3, 3);
    glTexCoord2f(0.0, 0.0);    glVertex2f(0, 0);
    glTexCoord2f(8, 0.0);      glVertex2f(3, 0);

    glTexCoord2f(5, 5);        glVertex2f(0, 0);
    glTexCoord2f(0.0, 0.0);    glVertex2f(-1.5, -3);
    glTexCoord2f(4, 0.0);      glVertex2f(1.5, -3);
  glEnd();

  glFlush();
  glutPostRedisplay(); //Loop display
}

// Keyboard callback
void key_callback(unsigned char key, int dummy1, int dummy2)
{
  switch(key)
  {
    case (KEY_ESCAPE): //Quit
      exit(0);
    case ('p'): //Stop rotation
      rotateFigure = false;
      break;
    case ('c'): //Start rotation
      rotateFigure = true;
      break;
    case ('u'): //Move up
      figureY += 0.1f;
      break;
    case ('d'): //Move down
      figureY -= 0.1f;
      break;
    case ('l'): //Move left
      figureX -= 0.1f;
      break;
    case ('r'): //Move right
      figureX += 0.1f;
      break;
    case ('+'): //Zoom in
    case ('='): //Allow '=' for ease of use (SHIFT + '=' not required for non-keypad input)
      scale += 0.1f;
      if (scale > 3.0f)
        scale = 3.0f;
      break;
    case ('-'): //Zoom out
      scale -= 0.1f;
      if (scale < 0.1f)
        scale = 0.1f;
      break;
    default:
      break;
  }
}

// Initializes GLUT and enters the main loop.
int main(int argc, char** argv) {
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
  glutInitWindowSize(520, 390);
  glutCreateWindow("Textured Triangles");
  glutDisplayFunc(display);
  glutReshapeFunc(reshape);
  glutKeyboardFunc(key_callback); // Register the keyboard callback function
  glutMainLoop();
}