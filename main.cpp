#include"raylib.h"
#include<iostream>
#include<iostream>
#include<vector>
#include<math.h>

using namespace std;

struct Player
{
    Texture2D img;
    Rectangle rect;
    bool active;
};


struct Knife
{
    Texture2D img;
    Rectangle rect;
    bool stuck{};
    float rot{};
    float rotSpeed;
};

enum Scene { MENU = 0, GAME };

int main()
{
    const int WIDTH = { 400 };
    const int HEIGHT = { 600 };
    InitWindow(WIDTH, HEIGHT, "Knife hit close");
    InitAudioDevice();
    SetTargetFPS(60);

    // load background
    Texture2D bg = LoadTexture("assets/Background.png");
    Texture2D bg2 = LoadTexture("assets/homeBg.png");

    // load play button
    Texture2D playButton = LoadTexture("assets/playButton.png");
    Rectangle playRect = { WIDTH / 2 - 50, HEIGHT / 2 + 120, (float)playButton.width * 0.5f, (float)playButton.height * 0.5f };

    // load restart button
    Texture2D restartButton = LoadTexture("assets/Skip.png");
    Rectangle restartRect = { 330, 10, (float)restartButton.width * 0.2f, (float)restartButton.height * 0.2f };

    // load target 
    Texture2D target = LoadTexture("assets/log.png");
    float scale = 0.2f;
    Rectangle targetDest = { WIDTH / 2 , HEIGHT / 4, (float)target.width * scale, (float)target.height * scale };
    Vector2 targetOrigin = { (float)target.width / 2 * scale,(float)target.height / 2 * scale };

    // load player
    float knifeScale = 0.1f;
    Player player;
    // player.img = LoadTexture("assets/images/knife/knife1.png");
    player.img = LoadTexture("assets/Knife.png");
    player.rect = { WIDTH / 2 - 15, HEIGHT - 140, (float)player.img.width * knifeScale, (float)player.img.height * knifeScale };
    player.active = true;

    // load knife-count
    Texture2D bgKnife = LoadTexture("assets/images/knife-count/background.png");
    Texture2D fgKnife = LoadTexture("assets/images/knife-count/foreground.png");
    int lengthFgKnife = 5;

    // load sound ketika knife stuck di target
    Sound hitted = LoadSound("assets/sounds/target_hitted.mp3");

    // game variables
    float rot{};
    float targetRotSpeed = 2;
    Scene currentScene = MENU;
    int score{};
    bool gameOver{};


    // test knife nyangkut
    vector<Knife> arr;


    // TODO:
    // Bikin story


    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(RAYWHITE);

        switch (currentScene)
        {
        case MENU:
            DrawTextureEx(bg2, Vector2{ 0 }, 0.0, 0.8, WHITE);

            if (CheckCollisionPointRec(GetMousePosition(), playRect) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) currentScene = GAME;

            DrawText("Pepega\n Knife", WIDTH / 2 - 55, HEIGHT / 2 - 200, 35, WHITE);

            DrawTextureEx(player.img, Vector2{ WIDTH / 2 - 5, HEIGHT / 2 - 50 }, 0.0, 0.1, WHITE);

            DrawTextureEx(playButton, Vector2{ playRect.x, playRect.y }, 0.0, 0.5, WHITE);

            DrawText("created by Aji Mustofa @pepega90", 20, HEIGHT - 40, 20, YELLOW);
            break;

        case GAME:
            DrawTextureEx(bg, Vector2{ 0 }, 0.0, 0.8, WHITE);
            if (IsKeyPressed(KEY_SPACE) && player.active && lengthFgKnife > 0)
            {
                player.active = false;
                lengthFgKnife--;
            }

            if (!player.active)  player.rect.y -= 50;
            if (player.rect.y < 0) player.active = true;

            if (player.active)
            {
                // player.active = true;
                player.rect.y = HEIGHT - 140;
            }

            // check collision antara pisau player dengan target
            // jika iya, maka stuck pisau ke target
            if (CheckCollisionRecs(player.rect, targetDest))
            {
                player.active = true;

                Knife kn;
                // kn.img = LoadTexture("assets/images/knife/knife1.png");
                kn.img = LoadTexture("assets/Knife.png");
                kn.rect.x = targetDest.x;
                kn.rect.y = targetDest.y;
                kn.rect.width = (float)kn.img.width * knifeScale;
                kn.rect.height = (float)kn.img.height * knifeScale;
                kn.rot = 0;
                kn.stuck = true;
                kn.rotSpeed = targetRotSpeed;
                arr.push_back(kn);
                PlaySound(hitted);
                score += 10;

            }


            // draw player knife
            DrawTextureEx(player.img, Vector2{ player.rect.x, player.rect.y }, 0.0, knifeScale, WHITE);

            // draw background knife-count
            for (int i = 0; i < 5; i++)
            {
                DrawTextureEx(bgKnife, Vector2{ 10, 350.f + i * 45.f }, 0.0, 0.5, WHITE);
            }

            // draw foreground knife-count
            for (int i = 0; i < lengthFgKnife; i++)
            {
                DrawTextureEx(fgKnife, Vector2{ 10, 350.f + i * 45.f }, 0.0, 0.5, WHITE);
            }

            if (lengthFgKnife < 1) gameOver = true;

            if (!gameOver)
                // rotasi untuk target
                rot += targetRotSpeed;
            else
                DrawTextureEx(restartButton, Vector2{ restartRect.x, restartRect.y }, 0.0, 0.2, WHITE);

            // restart game
            if (CheckCollisionPointRec(GetMousePosition(), restartRect) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
            {
                targetRotSpeed += 1;
                gameOver = false;
                arr.clear();
                lengthFgKnife = 5;
            }

            // DrawTextureEx(target, Vector2{ WIDTH / 2 - 100, HEIGHT / 4 }, rot, 0.3, WHITE);

            // draw pisau player rectangle collision lines
            // DrawRectangleLinesEx(player.rect, 2.0, RED);

            // draw pisau yang stuck di kayu
            for (unsigned int i = 0; i < arr.size(); i++)
            {
                if (arr[i].stuck)
                {
                    if (!gameOver)
                        arr[i].rot += arr[i].rotSpeed;
                    // DrawTextureEx(arr[i].img, Vector2{ arr[i].rect.x, arr[i].rect.y }, arr[i].rot, 0.3, WHITE);

                    DrawTexturePro(arr[i].img, Rectangle{ 0,0, (float)arr[i].img.width , (float)arr[i].img.height }, Rectangle{ arr[i].rect.x, arr[i].rect.y, arr[i].img.width * knifeScale, arr[i].img.height * knifeScale }, Vector2{ 15, -35 }, arr[i].rot, WHITE);
                }
            }

            // Draw target kayu
            DrawTexturePro(target, Rectangle{ 0,0,(float)target.width, (float)target.height }, targetDest, targetOrigin, rot, WHITE);

            // Draw score
            string textScore = "Score: " + to_string(score);
            DrawText(textScore.c_str(), 10, 10, 20, WHITE);

            break;
        }

        // draw helper mouse position
        // string pos = "X = " + to_string(GetMouseX()) + ", Y = " + to_string(GetMouseY());
        // DrawText(pos.c_str(), GetMouseX(), GetMouseY(), 20, WHITE);

        EndDrawing();
    }

    UnloadTexture(bg);
    UnloadTexture(bg2);
    UnloadTexture(playButton);
    UnloadTexture(fgKnife);
    UnloadTexture(bgKnife);
    UnloadTexture(player.img);
    UnloadTexture(target);
    CloseAudioDevice();
    CloseWindow();


    return 0;
}