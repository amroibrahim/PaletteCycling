#pragma once

#include <array>
#include <SDL.h>

#define iWidth 640   //320  //1920  // 3840
#define iHeight 400  //200  //1080  // 2160
#define iPaletteColorCount 15
#define iTimePerFrame 1000/60

class CyclingEffect
{
public:
   CyclingEffect();
   void InitScreenBuffer();
   virtual ~CyclingEffect();

   void ProcessInput();
   void Render();
   void Update();
   void Delay();
   void Quit();
   void Clean();

   bool IsOver();
   int GetTimePerFrame();

protected:
   virtual void InitColorLookup();

   SDL_Color* m_pPalette;

   SDL_Window* m_pWindow;

   SDL_Surface* m_pWindowSurface;
   SDL_Surface* m_pScreenBuffer;

   int m_iScreenWidth;
   int m_iScreenHeight;
   bool m_bIsOver;
};
