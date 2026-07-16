#include <raylib.h>

#define SCREEN_WIDTH 600
#define SCREEN_HEIGHT 600
#define SCREEN_TITLE "Raylib"

int main(){
    //Initialize a Window
    InitWindow(SCREEN_WIDTH ,SCREEN_HEIGHT,SCREEN_TITLE);
   
    while(!WindowShouldClose()){
      
        BeginDrawing();

         
        EndDrawing();
    }

    CloseWindow();
}