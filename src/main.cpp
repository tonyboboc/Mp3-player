#include <raylib.h>
#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#define SCREEN_WIDTH 600
#define SCREEN_HEIGHT 600
#define SCREEN_TITLE "Raylib"
bool isButtonHover;
bool isSliderHover;
int current_item=0;
int text_movement=0;
double last_text_move=0;
double song_start=0;
//Gui function protypes
void GuiSliderMusic(Rectangle rec,Music & music,Color color);
void GuiPlayButton(Vector2 position,Music & music,float radius, Color bgColor, Color fgColor);
void GuiPlayButton(Vector2 position,std::vector<Music> music,float radius, Color bgColor, Color fgColor,Texture texture, std::string action);
void DrawSongName(std::string song_name,int posx,int posy,Color txtcolor,int fontsize);
int size;
int main(){
    //Initialize a Window
    InitWindow(SCREEN_WIDTH ,SCREEN_HEIGHT,SCREEN_TITLE);
   //Initialize Audio Device
    InitAudioDevice();
    std::vector<Music> music;
    std::vector<std::string> songNames;
    FilePathList files =LoadDirectoryFilesEx("../../music_folder", ".mp3", false);
        for (unsigned int i = 0; i < files.count; i++)
    {
        music.push_back(LoadMusicStream(files.paths[i]));

        songNames.push_back(GetFileNameWithoutExt(files.paths[i]));
    }

UnloadDirectoryFiles(files);

    size=music.size();
    PlayMusicStream(music[current_item]);
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

        //Update Music Buffer with new Music Stream
        
        UpdateMusicStream(music[current_item]);


        BeginDrawing();
        ClearBackground(DARKBLUE);
        DrawTexture(texture,tex_centerx,tex_centery,WHITE);
        GuiPlayButton(Vector2{GetScreenWidth()/2.0f,GetScreenHeight()/2.0f},music[current_item],30,WHITE,BLACK);
        GuiPlayButton(Vector2{GetScreenWidth()/2.0f+180,GetScreenHeight()/2.0f},music,40,WHITE,BLACK,next_track,"next");
        GuiPlayButton(Vector2{GetScreenWidth()/2.0f-180,GetScreenHeight()/2.0f},music,40,WHITE,BLACK,backtrack,"back");
        DrawSongName(songNames[current_item],GetScreenWidth()/2.0,GetScreenHeight()/2.0f+120,WHITE,24);
        GuiSliderMusic(seek_rec,music[current_item],SKYBLUE);
        EndDrawing();
    }
    UnloadMusicStream(music[current_item]);
    CloseAudioDevice();
    UnloadTexture(texture);
    UnloadTexture(next_track);
    UnloadTexture(backtrack);
    CloseWindow();
    for (Music& song : music){
        UnloadMusicStream(song);
    }
}
void GuiSliderMusic(Rectangle rec,Music & music,Color color){
    Rectangle slider=rec;

    float time_played=GetMusicTimePlayed(music);
    float max_second=GetMusicTimeLength(music);
     isSliderHover=CheckCollisionPointRec(GetMousePosition(),rec);
     if(isSliderHover){
        SetMouseCursor(MOUSE_CURSOR_POINTING_HAND);
     }
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
    if(isButtonHover){
        SetMouseCursor(MOUSE_CURSOR_POINTING_HAND);
    }
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
void GuiPlayButton(Vector2 position,std::vector<Music> music,float radius, Color bgColor, Color fgColor,Texture texture, std::string action){
    //Update
    bool isButtonHover2 =CheckCollisionPointCircle(position,GetMousePosition(),radius);
    if(isButtonHover2){
        SetMouseCursor(MOUSE_CURSOR_POINTING_HAND);
    }
    if(isButtonHover2&& IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){
        if(action=="next"){
            if(current_item<size-1){
                song_start=GetTime();
                text_movement=0;
                last_text_move=0;
                StopMusicStream(music[current_item]);
                current_item++;
                PlayMusicStream(music[current_item]);
            }
        }
        if(action=="back"){
            if(current_item>0){
                song_start=GetTime();
                text_movement=0;
                last_text_move=0;
                StopMusicStream(music[current_item]);
                current_item--;
                PlayMusicStream(music[current_item]);
            }
        }
    }

    //Render
    DrawCircleV(position ,radius, !isButtonHover2? bgColor :Fade(bgColor,0.8f));
    DrawTexture(texture,position.x-radius/2.0f,position.y-radius/2.0f,fgColor);
   

}
void DrawSongName(std::string song_name,int posx,int posy,Color txtcolor,int fontsize){
    static double elapsed;
    static std::string scrolling_text;
    elapsed=GetTime()-song_start;
    if(song_name.size()>20){
        DrawText(song_name.substr(text_movement,25).c_str(),posx-MeasureText(song_name.substr(text_movement,20).c_str(),fontsize)/2,posy,fontsize,txtcolor);
        if(elapsed-last_text_move>=0.3&&elapsed>3.0f){
            
            text_movement+=1;
            last_text_move=elapsed;
        }
        if(text_movement>=song_name.size()-1-21){
            text_movement=0;
        }

    }
    else{
        DrawText(song_name.c_str(),posx-MeasureText(song_name.c_str(),fontsize)/2,posy,fontsize,txtcolor);
    }
}