
#include "raylib.h"
#include "raymath.h"

#define RAYGUI_IMPLEMENTATION
#include "raygui.h"
#include <vector>

int screenWidth = 1200;
int screenHeight = 800;

const unsigned int TARGET_FPS = 50;

// Struct to contain position and velocity state (For Exercise 2)
class PhysicsBody
{
public:
    Vector2 position = Vector2{ 0,0 };
    Vector2 velocity = Vector2{ 0,0 };
    float mass = 1.0f;
    float gravityScale = 1.0f;
    float drag = 0.01f;
    Color color = YELLOW;
    float radius = 10.0f;
    

};


bool isOverlapping(PhysicsBody a, PhysicsBody b)
{
    Vector2 disp = b.position - a.position;
    float dist = Vector2LengthSqr(disp);
    float sumRadius = a.radius + b.radius;
    return dist < sumRadius * sumRadius;
}


//Class to contain global properties of Physics simulation, and to operate on them each frame
class PhysicsSimulation
{
public:
    
    Vector2 gravity = { 0, 200 }; // Global acceleration due to gravity, in pixels/second/second
    const float FIXED_DELTA_TIME = 1.0f / (float)TARGET_FPS; // A fixed delta time variable is important to physics simulations


    void CheckCollisions()
    {

        for (int i = 0; i < bodies.size(); i++)
        {
            bodies[i].color = GREEN;
        }

        //Check Collisions
        for (int i = 0; i < bodies.size(); i++)
        {

            for (int j = 1; j < bodies.size(); j++)
            {
                if (isOverlapping(bodies[i], bodies[j]))
                {
                    bodies[i].color = RED;
                    bodies[j].color = RED;
                }
                else 
                {
                    bodies[i].color = GREEN;
                    bodies[j].color = GREEN;
                }

            }

        }
    }

    void Update()
    {
        //update positions w/ kinematics
        for (int i = 0; i < bodies.size(); i++)
        {
            //PhysicsBody body = bodies[i];
            //velocity is defined in pixels/second --> we need pixels/frame
            bodies[i].position += bodies[i].velocity * FIXED_DELTA_TIME;
            //acceleration is change in velocity over time, gravity is our acceleration in pixels/sec/sec

            bodies[i].velocity += gravity * bodies[i].gravityScale * FIXED_DELTA_TIME;


            //Apply drag to counteract velocity!
            bodies[i].velocity *= 1.0f - (bodies[i].drag * FIXED_DELTA_TIME);
        }

        CheckCollisions();
    }

    void Draw()
    {
        for (int i = 0; i < bodies.size(); i++)
        {
            DrawCircleV(bodies[i].position, bodies[i].radius, bodies[i].color);
        }
    }

    void Add(PhysicsBody body)
    {
        bodies.push_back(body);
    }
private:
    std::vector<PhysicsBody> bodies; // A container of all PhysicsBody in the simulation
};

//PhysicsBody bird;// = { Vector2{-1000,-1000}, Vector2{0,0} };
PhysicsSimulation sim;

Vector2 launchPosition; // Slingshot position
float launchPositionAdjustmentSpeed = 50.0f;
float launchSpeed = 100.0f; // Speed of the bird when launched
float launchAngle = 0.0f; // Angle to launch bird, from horizontal, in degrees
float spawnDrag = 0.0f; // damping value to apply to physics object velocities
float spawnRadius = 10.0f;

int main()
{

    InitWindow(screenWidth, screenHeight, "Mitch Elborn 101614291");
    SetTargetFPS(TARGET_FPS);


    PhysicsBody target;
    target.radius = 50;
    target.position = { 900, (float)screenHeight - 200 };
    target.gravityScale = 0.0f;
    sim.Add(target);


    launchPosition = { 30, (float)(screenHeight - 50) };

    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(Color{ 100, 100, 166, 255 });

        // GUI
        DrawRectangle(0, 0, 260, 600, Color{ 255, 255, 255, 200 });

        GuiSlider(Rectangle{ 100, 5, 100, 20 }, "LaunchSpeed", TextFormat("%.2f", launchSpeed), &launchSpeed, 1, 500);

        GuiSlider(Rectangle{ 100, 30, 100, 20 }, "LaunchAngle", TextFormat("%.2f", launchAngle), &launchAngle, -89.67f, 89);

        GuiSlider(Rectangle{ 100, 60, 100, 20 }, "Gravity", TextFormat("%.2f", sim.gravity.y), &sim.gravity.y, -1000, 1000);
        GuiSlider(Rectangle{ 100, 90, 100, 20 }, "Drag", TextFormat("%.2f", spawnDrag), &spawnDrag, 0, 10);

        GuiSlider(Rectangle{ 100, 120, 100, 20 }, "Radius", TextFormat("%.2f", spawnRadius), &spawnRadius, 2, 50);


        if (IsKeyDown(KEY_UP))
        {
            launchPosition.y -= launchPositionAdjustmentSpeed * GetFrameTime();
        }
        if (IsKeyDown(KEY_DOWN))
        {
            launchPosition.y += launchPositionAdjustmentSpeed * GetFrameTime();
        }

        Vector2 velocityPreview = { cosf(launchAngle * DEG2RAD) * launchSpeed, sinf(launchAngle * DEG2RAD) * launchSpeed};

        DrawCircleV(launchPosition, 5, BROWN);
        DrawLineEx(launchPosition, launchPosition + velocityPreview, 2, RED);

        // Spawn Bird (For Exercise 2)
        if (IsKeyPressed(KEY_SPACE))
        {
            PhysicsBody birdToLaunch;
            birdToLaunch.color = YELLOW;
            birdToLaunch.radius = spawnRadius;
            birdToLaunch.position = launchPosition;
            birdToLaunch.velocity = velocityPreview;
            birdToLaunch.drag = spawnDrag;

            sim.Add(birdToLaunch);
        }

        sim.Update();
        sim.Draw();

        // Draw bird (For Exercise 2)
        // DrawCircleV(bird.position, 10, RED);
        

        //Vector2 mouseDelta = launchPosition - GetMousePosition();
        //DrawLineV(launchPosition, GetMousePosition(), Color{0,0,0,60});

        //GuiSliderBar(Rectangle{ 60, 5, 1000, 10 }, "Time", TextFormat("%.2f", time), &time, 0, 240);

        EndDrawing();
    }

    CloseWindow();
    return 0;
}
