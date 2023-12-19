


#include "Eagle.hpp"
#include "Eagle/backends/Allegro5Backend.hpp"
#include "allegro5/allegro.h"

#include "SharkPlayer.hpp"



void PipeToEagleLog(const char* str) {
   EagleInfo() << str << std::endl;
}



int main(int argc , char** argv) {
   
   (void)argc;
   (void)argv;
   
   al_set_config_value(al_get_system_config() , "trace" , "level" , "error");
   
   al_register_trace_handler(PipeToEagleLog);
   
   SendOutputToFile("TINS2022.log" , "" , false);
   
   Allegro5System* sys = GetAllegro5System();
   
   if (sys->Initialize(EAGLE_FULL_SETUP) != EAGLE_FULL_SETUP) {
      EagleWarn() << "Some subsystems not initialized. Proceeding" << std::endl;
   }
   
   int sw = 800;
   int sh = 600;
   
   EagleGraphicsContext* win = sys->CreateGraphicsContext("Main Window" , sw , sh , EAGLE_OPENGL | EAGLE_WINDOWED | EAGLE_RESIZABLE);
   
   EAGLE_ASSERT(win && win->Valid());
   
//   EagleFont* font = win->GetFont("Verdana.ttf" , -20);
   
//   EAGLE_ASSERT(font && font->Valid());
   sys->GetSystemTimer()->Start();
   
   
   SharkPlayer player;
   player.Init(win);

   bool quit = false;
   bool redraw = true;
   while (!quit) {
      if (redraw) {
         win->SetDrawingTarget(win->GetBackBuffer());
         win->Clear(EagleColor(0,196,255));
         player.Draw(400,300);
         win->FlipDisplay();
         redraw = false;
      }
      while (!sys->UpToDate()) {
         EagleEvent e = sys->WaitForSystemEventAndUpdateState();
         if (e.type == EAGLE_EVENT_KEY_DOWN && e.keyboard.keycode == EAGLE_KEY_ESCAPE) {
            quit = true;
         }
         if (e.type == EAGLE_EVENT_DISPLAY_CLOSE) {
            quit = true;
         }
         if (e.type == EAGLE_EVENT_TIMER) {
            player.Update(e.timer.eagle_timer_source->SPT());
            redraw = true;
         }
         if (input_key_press(EAGLE_KEY_F)) {
            player.AdvanceFrame();
         }
         player.HandleEvent(e);
      }
   }
   return 0;
}


