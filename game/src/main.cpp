
#include "raylib.h"
#include "raymath.h"

#define RAYGUI_IMPLEMENTATION
#include "raygui.h"

int screenWidth = 1200;
int screenHeight = 800;

const unsigned int TARGET_FPS = 50;
const float FIXED_DELTA_TIME = 1.0f / (float)TARGET_FPS;

// Struct to contain position and velocity state (For Exercise 2)
struct PhysicsBody
{
    Vector2 position;
    Vector2 velocity;
};

PhysicsBody bird = { Vector2{-1000, -1000}, Vector2{0, 0} };

Vector2 launchPosition; // Slingshot position
float launchPositionAdjustmentSpeed = 50.0f;
float launchSpeed = 100.0f; // Speed of the bird when launched
float launchAngle = 0.0f; // Angle to launch bird, from horizontal, in degrees

int main()
{
    InitWindow(screenWidth, screenHeight, "Physics-1");
    SetTargetFPS(TARGET_FPS);

    launchPosition = { 30, (float)(screenHeight - 50) };

    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(Color{ 100, 100, 166, 255 });

        // GUI
        DrawRectangle(0, 0, 260, 600, Color{ 255, 255, 255, 200 });

        GuiSlider(Rectangle{ 100, 5, 100, 20 },
            "LaunchSpeed",
            TextFormat("%.2f", launchSpeed),
            &launchSpeed, 1, 500);

        GuiSlider(Rectangle{ 100, 30, 100, 20 },
            "LaunchAngle",
            TextFormat("%.2f", launchAngle),
            &launchAngle, -89.67f, 89);

        if (IsKeyDown(KEY_UP))
        {
            launchPosition.y -= launchPositionAdjustmentSpeed * GetFrameTime();
        }
        if (IsKeyDown(KEY_DOWN))
        {
            launchPosition.y += launchPositionAdjustmentSpeed * GetFrameTime();
        }

        Vector2 velocityPreview = {
            cosf(launchAngle * DEG2RAD) * launchSpeed,
            sinf(launchAngle * DEG2RAD) * launchSpeed
        };

        DrawCircleV(launchPosition, 5, BROWN);
        DrawLineEx(launchPosition, launchPosition + velocityPreview, 2, RED);

        if (IsKeyPressed(KEY_SPACE))
        {
            bird.position = launchPosition;
            bird.velocity = velocityPreview;
        }

        // Draw bird
        DrawCircleV(bird.position, 50, RED);

        //Vector2 mouseDelta = launchPosition - GetMousePosition();
        //DrawLineV(launchPosition, GetMousePosition(), Color{0,0,0,60});

        //GuiSliderBar(Rectangle{ 60, 5, 1000, 10 },
        //             "Time", TextFormat("%.2f", time), &time, 0, 240);

        EndDrawing();
    }

    CloseWindow();
    return 0;
}
