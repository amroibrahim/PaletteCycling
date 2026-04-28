#include "CyclingEffect.h"
#include <iostream>

CyclingEffect::CyclingEffect()
   : m_pPalette(nullptr),
   m_pWindow(nullptr),
   m_pWindowSurface(nullptr),
   m_pScreenBuffer(nullptr),
   m_iScreenWidth(iWidth),
   m_iScreenHeight(iHeight),
   m_bIsOver(false)
{
   if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
   {
      std::cout << "SDL failed to initialize! SDL_Error: " << SDL_GetError() << std::endl;
      m_bIsOver = true;
      return;
   }

   m_pWindow = SDL_CreateWindow("Palette Cycling Demo", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, m_iScreenWidth, m_iScreenHeight, SDL_WINDOW_SHOWN);

   if (!m_pWindow)
   {
      std::cout << "SDL failed to create window! SDL_Error: " << SDL_GetError() << std::endl;
      m_bIsOver = true;
      return;
   }
   m_pWindowSurface = SDL_GetWindowSurface(m_pWindow);

   // Create 8-bit screen buffer
   m_pScreenBuffer = SDL_CreateRGBSurface(0, m_iScreenWidth, m_iScreenHeight, 8, 0, 0, 0, 0);
   if (!m_pScreenBuffer)
   {
      std::cout << "SDL failed to create screen buffer! SDL_Error: " << SDL_GetError() << std::endl;
      m_bIsOver = true;
      return;
   }
   SDL_FillRect(m_pScreenBuffer, NULL, 0);

   InitColorLookup();
   if (!m_pPalette)
   {
      std::cout << "Palette initialization failed." << std::endl;
      m_bIsOver = true;
      return;
   }

   SDL_SetPaletteColors(m_pScreenBuffer->format->palette, m_pPalette, 0, iPaletteColorCount);

   InitScreenBuffer();
}

void CyclingEffect::InitScreenBuffer()
{
   if (!m_pScreenBuffer)
   {
      return;
   }

   uint8_t* pPixels = (uint8_t*)m_pScreenBuffer->pixels;
   const int iPitch = m_pScreenBuffer->pitch;

   int iPaletteOffset = 0;
   int iAlternateWidth = 40;
   int iCurrentWidth = 0;

   int iPaletteStretch = 0;
   int iPaletteStretchMax = 5;

   for (int x = 0; x < m_iScreenWidth; ++x)
   {
      int iCurrentPaletteIndex = 0;

      if (iCurrentWidth >= iAlternateWidth)
      {
         iPaletteOffset = iPaletteColorCount / 2;
         iCurrentWidth = -iAlternateWidth;
      }
      else  if (iCurrentWidth == 0)
      {
         iCurrentWidth = 0;
         iPaletteOffset = 0;
      }

      iCurrentWidth++;

      for (int y = 0; y < m_iScreenHeight; ++y)
      {
         if (iPaletteStretch >= iPaletteStretchMax)
         {
            iPaletteStretch = 0;
            iCurrentPaletteIndex++;
         }

         iPaletteStretch++;
         pPixels[iPitch * y + x] = (iCurrentPaletteIndex + iPaletteOffset) % iPaletteColorCount;
      }
   }
}

void CyclingEffect::InitColorLookup()
{
   int Index = 0;
   m_pPalette = new SDL_Color[iPaletteColorCount];

   m_pPalette[Index++] = { 50, 125, 100 };
   m_pPalette[Index++] = { 50, 125, 100 };
   m_pPalette[Index++] = { 50, 125, 100 };
   m_pPalette[Index++] = { 35, 85, 135 };
   m_pPalette[Index++] = { 35, 85, 135 };
   m_pPalette[Index++] = { 35, 85, 135 };
}

void CyclingEffect::Clean()
{
   SDL_FreeSurface(m_pScreenBuffer);
   m_pScreenBuffer = nullptr;

   // SDL_GetWindowSurface() returns a window-owned surface.
   m_pWindowSurface = nullptr;

   SDL_DestroyWindow(m_pWindow);
   m_pWindow = nullptr;

   delete[] m_pPalette;
   m_pPalette = nullptr;

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
   if (!m_pPalette || !m_pScreenBuffer)
   {
      return;
   }

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
   if (!m_pScreenBuffer || !m_pWindowSurface || !m_pWindow)
   {
      return;
   }

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
