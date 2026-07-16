#include <raylib.h>

#define SCREEN_WIDTH 600
#define SCREEN_HEIGHT 600
#define SCREEN_TITLE "Raylib"

int main(){
    //Initialize a Window
    InitWindow(SCREEN_WIDTH ,SCREEN_HEIGHT,SCREEN_TITLE);
   //Initialize Audio Device
    InitAudioDevice();
    Music music=LoadMusicStream("../../music_folder/Radiant Emerald ： Diamond In The Sky [J81_NbpiZb4].mp3");
    PlayMusicStream(music);
    while(!WindowShouldClose()){


        //Update Music Buffer with new MUsic Stream
        UpdateMusicStream(music);
        BeginDrawing();

         
        EndDrawing();
    }

    CloseWindow();
}