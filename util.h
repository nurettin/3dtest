#ifndef UTIL_HPP
#define UTIL_HPP

#include <memory>
#include <utility>
#include <stdexcept>
#include <GL/gl.h>
#include <SDL.h>

struct SDL
{
  typedef std::unique_ptr<int, std::function<void(int*)>> sdl_t;
  typedef std::unique_ptr<SDL_Window, void(*)(SDL_Window*)> win_t;
  typedef std::unique_ptr<void, void(*)(void*)> ctx_t;
  typedef std::function<void()> fun_t;
  typedef std::function<void(int, int)> fun_resize_t;
  typedef std::function<void(SDL_Event const &)> fun_event_t;
  
  sdl_t sdl;
  win_t win;
  ctx_t ctx;
  fun_t init;
  fun_resize_t resize;
  fun_t render;
  fun_event_t event;

  SDL(int w, int h, fun_t init, fun_resize_t resize, fun_t render, fun_event_t event)
  : sdl(gc(SDL_Init(SDL_INIT_EVERYTHING), [](int*){ SDL_Quit(); }))
  , win(SDL_CreateWindow("SDL2 OpenGL Test"
      , SDL_WINDOWPOS_UNDEFINED
      , SDL_WINDOWPOS_UNDEFINED
      , w, h
      , SDL_WINDOW_OPENGL| SDL_WINDOW_FULLSCREEN), &SDL_DestroyWindow)
  , ctx(SDL_GL_CreateContext(win.get()), &SDL_GL_DeleteContext)
  , init(init)
  , resize(resize)
  , render(render)
  , event(event)
  {
    ensure_sdl(*sdl.get());
    ensure_sdl(win.get());
    ensure_sdl(ctx.get());
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    SDL_GL_SetSwapInterval(1);

    init();
    resize(w, h);
  }

  ~SDL(){ SDL_Quit(); }

  void run()
  {
    SDL_Event e;
    for(bool run= true; run; SDL_Delay(10))
    {
      bool handled= false;
      while(SDL_PollEvent(&e))
      {
        switch(e.type)
        {
          case SDL_QUIT:
            run= false;
            break;
          case SDL_WINDOWEVENT:
            if(e.window.event== SDL_WINDOWEVENT_RESIZED)
              resize(e.window.data1, e.window.data2);
            handled= true;
            break;
        }
        if(!handled) event(e);
      }
      render();
      SDL_GL_SwapWindow(win.get());
    }
  }

  void ensure_sdl(int i){ if(i< 0) throw std::runtime_error(SDL_GetError()); }

  template <typename T>
  void ensure_sdl(T* p){ if(p== 0) throw std::runtime_error(SDL_GetError()); }
  
  template <typename T, typename D>
  std::unique_ptr<T, D> gc(T p, D d)
  {
    return std::unique_ptr<T, D>(&p, d);
  }

  template <typename T, typename D>
  std::unique_ptr<T, D> gc(T* p, D d)
  {
    return std::unique_ptr<T, D>(p, d);
  }
};

#endif

