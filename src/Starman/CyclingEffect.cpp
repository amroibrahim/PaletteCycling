#include "CyclingEffect.h"
#include <iostream>

CyclingEffect::CyclingEffect() : m_bIsOver(false)
{
   //Initialize SDL
   m_iScreenWidth = iWidth;
   m_iScreenHeight = iHeight;

   if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
   {
      std::cout << "SDL failed to initialize! SDL_Error: " << SDL_GetError() << std::endl;
   }

   m_pWindow = SDL_CreateWindow("Palette Cycling Demo", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, m_iScreenWidth, m_iScreenHeight, SDL_WINDOW_SHOWN);

   if (m_pWindow)
   {
      m_pWindowSurface = SDL_GetWindowSurface(m_pWindow);
   }

   // Create 8-bit screen buffer
   m_pScreenBuffer = SDL_CreateRGBSurface(0, m_iScreenWidth, m_iScreenHeight, 8, 0, 0, 0, 0);
   SDL_FillRect(m_pScreenBuffer, NULL, 0);

   InitColorLookup();

   SDL_SetPaletteColors(m_pScreenBuffer->format->palette, m_pPalette, 0, iPaletteColorCount);

   InitScreenBuffer();
}

void CyclingEffect::InitScreenBuffer()
{
   uint8_t* pPixels = (uint8_t*)m_pScreenBuffer->pixels;

   for (int x = 0; x < m_iScreenWidth; ++x)
   {
      for (int y = 0; y < m_iScreenHeight; ++y)
      {
         //pPixels[m_iScreenWidth * y + x] = (y + x) % iPaletteColorCount;
         //pPixels[m_iScreenWidth * y + x] = (y * x) % iPaletteColorCount;
         pPixels[m_iScreenWidth * y + x] = (y * x / 2) % iPaletteColorCount;
         //pPixels[m_iScreenWidth * y + x] = (y * x *iPaletteStretch) % iPaletteColorCount;
      }
   }
}

void CyclingEffect::InitColorLookup()
{
   int Index = 0;
   m_pPalette = new SDL_Color[iPaletteColorCount];

   m_pPalette[Index++] = { 165, 0, 150 };
   m_pPalette[Index++] = { 150, 0, 160 };
   m_pPalette[Index++] = { 115, 0, 140 };
   m_pPalette[Index++] = { 90, 0, 135 };
   m_pPalette[Index++] = { 60, 0, 120 };
   m_pPalette[Index++] = { 40, 0, 110 };
   m_pPalette[Index++] = { 25, 0, 90 };
   m_pPalette[Index++] = { 10, 0, 75 };
   m_pPalette[Index++] = { 10, 0, 75 };
   m_pPalette[Index++] = { 25, 0, 90 };
   m_pPalette[Index++] = { 40, 0, 110 };
   m_pPalette[Index++] = { 0, 0, 70 };
   m_pPalette[Index++] = { 60, 0, 120 };
   m_pPalette[Index++] = { 90, 0, 135 };
   m_pPalette[Index++] = { 115, 0, 140 };
}

void CyclingEffect::Clean()
{
   SDL_FreeSurface(m_pScreenBuffer);
   m_pScreenBuffer = nullptr;

   SDL_FreeSurface(m_pWindowSurface);
   m_pWindowSurface = nullptr;

   SDL_DestroyWindow(m_pWindow);
   m_pWindow = nullptr;

   SDL_Quit();
}

CyclingEffect::~CyclingEffect()
{
}

void CyclingEffect::ProcessInput()
{
   SDL_Event event;
   while (SDL_PollEvent(&event))
   {
      switch (event.type)
      {
      case SDL_QUIT:
         Quit();
         break;

      default:
         break;
      }
   }
}

void CyclingEffect::Update()
{
   SDL_Color SaveColor = m_pPalette[0];
   for (int i = 0; i < iPaletteColorCount - 1; ++i)
   {
      m_pPalette[i] = m_pPalette[i + 1];
   }
   m_pPalette[iPaletteColorCount - 1] = SaveColor;

   SDL_SetPaletteColors(m_pScreenBuffer->format->palette, m_pPalette, 0, iPaletteColorCount);
}

bool CyclingEffect::IsOver()
{
   return m_bIsOver;
}

void CyclingEffect::Delay()
{
   SDL_Delay(GetTimePerFrame());
}

void CyclingEffect::Render()
{
   SDL_BlitSurface(m_pScreenBuffer, nullptr, m_pWindowSurface, nullptr);
   SDL_UpdateWindowSurface(m_pWindow);
}

void  CyclingEffect::Quit()
{
   m_bIsOver = true;
}

int  CyclingEffect::GetTimePerFrame()
{
   return iTimePerFrame;
}
