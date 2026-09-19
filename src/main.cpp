#include <raylib.h>
#include <iostream>
#include <string>
#include <sstream>
#define SCREEN_WIDTH 600
#define SCREEN_HEIGHT 600
#define SCREEN_TITLE "Raylib"
bool isButtonHover;
bool isSliderHover;
bool isButtonHover2;
//Gui function protypes
void GuiSliderMusic(Rectangle rec,Music & music,Color color);
void GuiPlayButton(Vector2 position,Music & music,float radius, Color bgColor, Color fgColor);
void GuiPlayButton(Vector2 position,Music & music,float radius, Color bgColor, Color fgColor,Texture texture);

int main(){
    //Initialize a Window
    InitWindow(SCREEN_WIDTH ,SCREEN_HEIGHT,SCREEN_TITLE);
   //Initialize Audio Device
    InitAudioDevice();
    Music music=LoadMusicStream("../../music_folder/Radiant Emerald ： Diamond In The Sky [J81_NbpiZb4].mp3");
    PlayMusicStream(music);
    //PauseMusicStream(music);
    //SLider rect 
    Rectangle seek_rec{50,500,300,20};
    seek_rec.x=GetScreenWidth()/2.0f-seek_rec.width/2.0f;
    //Texture2d
    Texture2D texture=LoadTexture("../../images/vinyl.png");
    texture.width=212;
    texture.height=212;

    Texture2D next_track=LoadTexture("../../images/forward.png");
    next_track.width=42;
    next_track.height=42;

    Texture2D backtrack=LoadTexture("../../images/backwards.png");
    backtrack.width=42;
    backtrack.height=42;
    while(!WindowShouldClose()){
        float tex_centerx=GetScreenWidth()/2.0f-texture.width/2;
        float tex_centery=GetScreenHeight()/2.0-texture.height/2;

        //Update Music Buffer with new MUsic Stream
        if(isButtonHover||isSliderHover){
            SetMouseCursor(MOUSE_CURSOR_POINTING_HAND);
        }else{
            SetMouseCursor(MOUSE_CURSOR_DEFAULT);
        }
        UpdateMusicStream(music);


        BeginDrawing();
        ClearBackground(DARKBLUE);
        DrawTexture(texture,tex_centerx,tex_centery,WHITE);
        GuiPlayButton(Vector2{GetScreenWidth()/2.0f,GetScreenHeight()/2.0f},music,30,WHITE,BLACK);
        GuiPlayButton(Vector2{GetScreenWidth()/2.0f+180,GetScreenHeight()/2.0f},music,40,WHITE,BLACK,next_track);
        GuiPlayButton(Vector2{GetScreenWidth()/2.0f-180,GetScreenHeight()/2.0f},music,40,WHITE,BLACK,backtrack);


        GuiSliderMusic(seek_rec,music,SKYBLUE);
        DrawText("Radiant Emerald ： Diamond In The Sky",GetScreenWidth()/2.0f-MeasureText("Radiant Emerald ： Diamond In The Sky",22)/2.0f,GetScreenHeight()/2.0f+120,22,WHITE);
        EndDrawing();
    }
    UnloadMusicStream(music);
    CloseAudioDevice();
    UnloadTexture(texture);
    UnloadTexture(next_track);
    UnloadTexture(backtrack);
    CloseWindow();
}
void GuiSliderMusic(Rectangle rec,Music & music,Color color){
    Rectangle slider=rec;

    float time_played=GetMusicTimePlayed(music);
    float max_second=GetMusicTimeLength(music);
     isSliderHover=CheckCollisionPointRec(GetMousePosition(),rec);
    if(isSliderHover&&IsMouseButtonDown(MOUSE_BUTTON_LEFT)){
        float dx=GetMousePosition().x-rec.x;
        PauseMusicStream(music);
        slider.width=dx;
        SeekMusicStream(music,dx/(rec.width/max_second));
    }else{
        //ResumeMusicStream(music);
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
    DrawRectangleRounded(slider,1.0f,7,!isSliderHover?  color:Fade(RED,0.7f));//slider
    DrawRectangleRounded(rec,1.0f,7,Fade(color,0.3f));
    DrawRectangleRoundedLinesEx(rec,3,1.0f,7,isSliderHover?  color:Fade(RED,0.7f));
    DrawCircleV(circle_center,point_radius,BLUE);
    DrawCircleLines(circle_center.x,circle_center.y,point_radius,BLACK);
    DrawText(ss.str().c_str(),rec.x-MeasureText(ss.str().c_str(),20)-10,rec.y,20,GREEN);
    ss.clear();
    ss= std::stringstream("");
    ss<<(int)max_second/60<<":";//first show minutes
    ss<<(int)max_second%60;
    DrawText(ss.str().c_str(),rec.x+rec.width+MeasureText(ss.str().c_str(),20)-18,rec.y,20,WHITE);

}
void GuiPlayButton(Vector2 position,Music & music,float radius, Color bgColor, Color fgColor){
    //Update
    static bool isPlayed=true;
     isButtonHover =CheckCollisionPointCircle(position,GetMousePosition(),radius);
    float point =radius/3.0f; 

    if(isButtonHover&& IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){
        isPlayed=isPlayed ? false: true;
    }

    //Render
    DrawCircleV(position ,radius, !isButtonHover? bgColor :Fade(bgColor,0.8f));

    if(isPlayed){
        ResumeMusicStream(music);
        //Resume Symbol
        DrawRectangle(position.x-point,position.y-point,radius/4.0,radius/1.5,fgColor);//Left rect
        DrawRectangle(position.x+point-(radius/4.0f),position.y-point,radius/4.0,radius/1.5,fgColor);//Left rect

    }else{
        PauseMusicStream(music);
        DrawTriangle(
            Vector2{position.x - point, position.y - point},
            Vector2{position.x - point, position.y + point},
            Vector2{position.x + point, position.y},
            fgColor
        );
    }

}
void GuiPlayButton(Vector2 position,Music & music,float radius, Color bgColor, Color fgColor,Texture texture){
    //Update
    static bool isPlayed=true;
    isButtonHover2 =CheckCollisionPointCircle(position,GetMousePosition(),radius);
    float point =radius/3.0f; 

    if(isButtonHover2&& IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){
        isPlayed=isPlayed ? false: true;
    }

    //Render
    DrawCircleV(position ,radius, !isButtonHover2? bgColor :Fade(bgColor,0.8f));
    DrawTexture(texture,position.x-radius/2.0f,position.y-radius/2.0f,fgColor);
   

}
