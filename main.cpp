#include <iostream>
#include <cstdlib>
#include <cmath>
#include <GL/glu.h>
#include <GL/glut.h>
#include "util.h"

struct Shared
{
  GLdouble planex= 0, planey= 0, planez= 0, roll= 0, pitch= 0, heading= 0; 
  double velocity= 0, acceleration= 0.001;
} shared;

void pilot()
{
  glRotated(shared.roll, 0, 0, 1);
  glRotated(shared.pitch, 0, 1, 0);
  glRotated(shared.heading, 1, 0, 0);
  glTranslated(-shared.planex, -shared.planey, -shared.planez);
  float mat[16];
  glGetFloatv(GL_MODELVIEW_MATRIX, mat);
  shared.planex-= mat[2]* shared.velocity;
  shared.planey-= mat[6]* shared.velocity;
  shared.planez-= mat[10]* shared.velocity;
}

void init_gl()
{
  glClearColor(0, 0, 0, 0);
  glShadeModel(GL_FLAT);
}

void resize_gl(int w, int h)
{
  glViewport(0, 0, w, h);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(45, w* 1.0/ h, 0, 1000);
  glMatrixMode(GL_MODELVIEW);
}

void render_gl()
{
  glClear(GL_COLOR_BUFFER_BIT);
  glColor3f(1.0, 1.0, 1.0);
  glLoadIdentity();
  pilot();
  glutWireSphere(1, 50, 50);
  glFlush();
}

void event_sdl(SDL_Event const &e)
{
  switch(e.type)
  {
    case SDL_KEYDOWN:
      switch(e.key.keysym.sym)
      {
        case SDLK_w:
          shared.heading-= 1;
        break;
        case SDLK_s:
          shared.heading+= 1;
        break;
        case SDLK_a:
          shared.roll-= 1;
        break;
        case SDLK_d:
          shared.roll+= 1;
        break;
        case SDLK_UP:
          shared.velocity+= shared.acceleration;
          if(shared.velocity> 10)
            shared.velocity= 10;
        break;
        case SDLK_DOWN:
          shared.velocity-= shared.acceleration;
          if(shared.velocity< 0)
            shared.velocity= 0;
        break;
      }
    break;
  }
}

int main(int argc, char* argv[]) { try
{
  SDL sdl(1280, 768, init_gl, resize_gl, render_gl, event_sdl); 
  glutInit(&argc, argv);
  sdl.run();

} catch(std::exception &ex) {
  SDL_Log("%s", ex.what());
} }

