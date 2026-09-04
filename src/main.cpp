#include <raylib.h>
#include <iostream>
#include <string>
#include <sstream>
#define SCREEN_WIDTH 600
#define SCREEN_HEIGHT 600
#define SCREEN_TITLE "Raylib"

//Gui function protypes
void GuiSliderMusic(Rectangle rec,Music & music,Color color);
int main(){
    //Initialize a Window
    InitWindow(SCREEN_WIDTH ,SCREEN_HEIGHT,SCREEN_TITLE);
   //Initialize Audio Device
    InitAudioDevice();
    Music music=LoadMusicStream("../../music_folder/Radiant Emerald ： Diamond In The Sky [J81_NbpiZb4].mp3");
    PlayMusicStream(music);
    //SLider rect 
    Rectangle seek_rec{50,500,300,10};
    seek_rec.x=GetScreenWidth()/2.0f-seek_rec.width/2.0f;
    //Texture2d
    Texture2D texture=LoadTexture("../../images/vinyl.png");
    texture.width=212;
    texture.height=212;
    while(!WindowShouldClose()){
        float tex_centerx=GetScreenWidth()/2.0f-texture.width/2;
        float tex_centery=GetScreenHeight()/2.0-texture.height/2;

        //Update Music Buffer with new MUsic Stream
        UpdateMusicStream(music);


        BeginDrawing();
        ClearBackground(DARKBLUE);
        DrawTexture(texture,tex_centerx,tex_centery,WHITE);
        GuiSliderMusic(seek_rec,music,SKYBLUE);
         
        EndDrawing();
    }
    UnloadMusicStream(music);
    CloseAudioDevice();
    CloseWindow();
}
void GuiSliderMusic(Rectangle rec,Music & music,Color color){
    Rectangle slider=rec;

    float time_played=GetMusicTimePlayed(music);
    float max_second=GetMusicTimeLength(music);
    bool isSliderHover=CheckCollisionPointRec(GetMousePosition(),rec);
    if(isSliderHover&&IsMouseButtonDown(MOUSE_BUTTON_LEFT)){
        float dx=GetMousePosition().x-rec.x;
        PauseMusicStream(music);
        slider.width=dx;
        SeekMusicStream(music,dx/(rec.width/max_second));
    }else{
        ResumeMusicStream(music);
        slider.width=(rec.width/max_second)*time_played;
    }
    //Point Slider
        float point_radius=rec.height/2;
    Vector2 circle_center={slider.x+slider.width,slider.y+point_radius};

    //Format : Convert second to minute.
    //Second to minute ->minute=second/60
    std::stringstream ss;
    ss<<(int)time_played/60<<":";//first show minutes
    ss<<(int)time_played%60;
    //Render
    DrawRectangleRounded(slider,1.0f,7,color);//slider
    DrawRectangleRounded(rec,1.0f,7,Fade(color,0.3f));
    DrawRectangleRoundedLinesEx(rec,3,1.0f,7,Fade(color,0.7));
    DrawCircleV(circle_center,point_radius,BLUE);
    DrawCircleLines(circle_center.x,circle_center.y,point_radius,BLACK);
    DrawText(ss.str().c_str(),rec.x-MeasureText(ss.str().c_str(),20)-10,rec.y,20,GREEN);
    ss.clear();
    ss= std::stringstream("");
    ss<<(int)max_second/60<<":";//first show minutes
    ss<<(int)max_second%60;
    DrawText(ss.str().c_str(),rec.x+rec.width+MeasureText(ss.str().c_str(),20)-15,rec.y,20,GREEN);

}
