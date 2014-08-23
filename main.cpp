#include <cstdlib>
#include <cmath>
#include <GL/glu.h>
#include <GL/glut.h>
#include "util.h"
#include "camera.h"

struct Shared
{
  Camera cam;
  double aax= 0.01, aaz= 0.01, vax= 0, vaz= 0;
  double vf= 0, af= 0.001;
} shared;

void draw_ship()
{
  glTranslatef(0, -0.5, -1);
  glPushMatrix();
    // left wing
    glPushMatrix();
      glTranslatef(1, 0, 0);
      glScalef(1, 0.5, 0.5);
      glutWireCube(1);
    glPopMatrix();
    // right wing
    glPushMatrix();
      glTranslatef(-1, 0, 0);
      glScalef(1, 0.5, 0.5);
      glutWireCube(1);
    glPopMatrix();
    // torso
    glTranslatef(0, 0, -1);
    glScalef(1, 0.5, 2);
    glutWireCube(1);
  glPopMatrix();
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
  draw_ship();
  shared.cam.RotateX(shared.vax);
  shared.cam.RotateZ(shared.vaz);
  shared.cam.MoveForward(-shared.vf);
  shared.cam.Render();
  glTranslatef(0, 0, -200);
  glutWireSphere(100, 100, 100);
  glTranslatef(0, -200, -200);
  glutWireSphere(100, 100, 100);
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
          shared.vax-= shared.aax;
          if(shared.vax< -shared.aax)
            shared.vax= -shared.aax;
        break;
        case SDLK_s:
          shared.vax+= shared.aax;
          if(shared.vax> shared.aax)
            shared.vax= shared.aax;
        break;
        case SDLK_a:
          shared.vaz+= shared.aaz;
          if(shared.vaz> shared.aaz)
            shared.vaz= shared.aaz;
        break;
        case SDLK_d:
          shared.vaz-= shared.aaz;
          if(shared.vaz< -shared.aaz)
            shared.vaz= -shared.aaz;
        break;
        case SDLK_UP:
          shared.vf+= shared.af;
          if(shared.vf> 10)
            shared.vf= 10;
        break;
        case SDLK_DOWN:
          shared.vf-= shared.af;
          if(shared.vf< 0)
            shared.vf= 0;
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

