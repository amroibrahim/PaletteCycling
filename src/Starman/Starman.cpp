#include "CyclingEffect.h"
#include <iostream>


int main(int argc, char* argv[])
{
   CyclingEffect* pCyclingEffect = new CyclingEffect();
   while (!pCyclingEffect->IsOver())
   {
      pCyclingEffect->ProcessInput();
      pCyclingEffect->Update();
      pCyclingEffect->Render();
      pCyclingEffect->Delay();
   }

   pCyclingEffect->Clean();

   delete pCyclingEffect;

   return 0;
}
