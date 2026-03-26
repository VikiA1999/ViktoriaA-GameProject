#include "raylib.h"
#include <math.h>
#include <stdlib.h>
#include <time.h>

#define MAX_BLAST 20
#define MAX_ALIENS 100
#define ALIEN_SPEED 2
#define BLAST_SPEED 10
#define ALIEN_SPAWN_DELAY 0.5f

//Initialise blast variables
void InitBlasts(float BlastX[], float BlastY[], float BlastDirectionX[], float BlastDirectionY[], bool BlastActive[], int size)
{
  for (int i = 0; i < size; i++)
  {
     BlastX[i] = 0;
     BlastY[i] = 0;
     BlastDirectionX[i] = 0;
     BlastDirectionY[i] = 0;
     BlastActive[i] = false;
  }
}

//Initialise aliens variables
void InitAliens(float AlienX[], float AlienY[], float AlienDirectionX[], float AlienDirectionY[], int AlienType[], bool AlienAlive[], int size)
{
  for (int i = 0; i < size; i++)
  {
     AlienX[i] = 0;
     AlienY[i] = 0;
     AlienDirectionX[i] = 0;
     AlienDirectionY[i] = 0;
     AlienType[i] = 0;
     AlienAlive[i] = false;
  }
}

//Initialise explosion variables
void InitExplosion(float ExplosionTimer[], bool ExplosionActive[], int size)
{
  for (int i = 0; i < size; i++)
  {
     ExplosionTimer[i] = 0;
     ExplosionActive[i] = false;
  }
}

//Ship collision position
Rectangle GetShipCollision(float ShipX, float ShipY, Texture2D Ship)
{
   Rectangle collision;
   collision.x = ShipX - Ship.width /2;
   collision.y = ShipY - Ship.height / 2;
   collision.width = Ship.width;
   collision.height = Ship.height;
   return collision;
}

//Blast collision position
Rectangle GetBlastRectangle(float BlastX, float BlastY, Texture2D Blast)
{
   Rectangle BlastRectangle;
   BlastRectangle.x = BlastX;
   BlastRectangle.y = BlastY;
   BlastRectangle.width = Blast.width;
   BlastRectangle.height = Blast.height;
   return BlastRectangle;
}
 
//Alien collision position
Rectangle GetAlienRectangle(float AlienX, float AlienY, Texture2D Alien)
{
   Rectangle AlienRectangle;
   AlienRectangle.x = AlienX;
   AlienRectangle.y = AlienY;
   AlienRectangle.width = Alien.width;
   AlienRectangle.height = Alien.height;
   return AlienRectangle;
}

//Centre vector for the ship
Vector2 GetShipOrigin(Texture2D Ship)
{
   Vector2 origin;
   origin.x = Ship.width / 2.0f;
   origin.y = Ship.height / 2.0f;
   return origin;
}

//Ship rotation
Rectangle GetShipRectangle(float x, float y, Texture2D Ship)
{
   Rectangle ShipRectangle;
   ShipRectangle.x = x;
   ShipRectangle.y = y;
   ShipRectangle.width = Ship.width;
   ShipRectangle.height = Ship.height;
   return ShipRectangle;
}

//Alien type
Texture2D GetAlienType(int type, Texture2D Alien1, Texture2D Alien2, Texture2D Alien3)
{
   if (type == 1)
   {
      return Alien1;
   }
   else if (type == 2)
   {
      return Alien2;
   }
   else
   {
      return Alien3;
   }
}

int main(void)
{
    //Screen initialisation
    const int screenWidth = 700;
    const int screenHeight = 700;
    InitWindow(screenWidth, screenHeight, "Space Shooter");

    //Set frame rate
    SetTargetFPS(60);

    //Initialise random number generator
    srand(time(NULL));

    //Textures
    Texture2D Ship = LoadTexture("rocket.png");
    Texture2D Blast = LoadTexture("blast.png");
    Texture2D Explosion = LoadTexture("explosion.png");
    Texture2D Score = LoadTexture("score.png");
    Texture2D Heart = LoadTexture("heart.png");
    Texture2D Alien1 = LoadTexture("alien1.png");
    Texture2D Alien2 = LoadTexture("alien2.png");
    Texture2D Alien3 = LoadTexture("alien3.png");

    //Ship position
    float ShipX = screenWidth / 2.0f;
    float ShipY = screenHeight / 2.0f;
    float ShipAngle = 0.0f;

   //Declare arrays
   float BlastX[MAX_BLAST];
   float BlastY[MAX_BLAST];
   float BlastDirectionX[MAX_BLAST];
   float BlastDirectionY[MAX_BLAST];
   bool BlastActive[MAX_BLAST];
   float AlienX[MAX_ALIENS];
   float AlienY[MAX_ALIENS];
   float AlienDirectionX[MAX_ALIENS];
   float AlienDirectionY[MAX_ALIENS];
   int AlienType[MAX_ALIENS];
   bool AlienAlive[MAX_ALIENS];
   float ExplosionTimer[MAX_ALIENS];
   bool ExplosionActive[MAX_ALIENS];

   //Initialise blasts
   InitBlasts(BlastX, BlastY, BlastDirectionX, BlastDirectionY, BlastActive, MAX_BLAST);

   //Initialise Aliens
   InitAliens(AlienX, AlienY, AlienDirectionX, AlienDirectionY, AlienType, AlienAlive, MAX_ALIENS);

   //Initialise explosions
   InitExplosion(ExplosionTimer, ExplosionActive, MAX_ALIENS);

   //Play-again button
   Rectangle PlayButton;
   PlayButton.x = screenWidth / 2 - 90;
   PlayButton.y = screenHeight / 2 + 40;
   PlayButton.width = 180;
   PlayButton.height = 50;
   
   //Main loop prep
   int ScoreValue = 0;
   int hearts = 3;
   bool GameOver = false;
   float SpawnTimer = 0.5f;
   
   while (!WindowShouldClose())
   {
      //Update spawn timer
      float DeltaTime = GetFrameTime();
      if(!GameOver) SpawnTimer += DeltaTime;

      //Ship rotation
      Vector2 MousePosition = GetMousePosition();
      ShipAngle = atan2f(MousePosition.y - ShipY, MousePosition.x - ShipX) * 180.0 / PI;

      //Shooting algorithm
      if (!GameOver && IsKeyPressed(KEY_ENTER))
      {
         for (int i = 0; i < MAX_BLAST; i++)
         {
            if (!BlastActive[i])
            {
              BlastActive[i] = true;
       float AngleRad = ShipAngle * PI / 180.0f;
     float ShipNoseOffset = Ship.height / 2.0f;
     BlastX[i] = ShipX + sinf(AngleRad) * ShipNoseOffset - Blast.width / 2.0f;
              BlastY[i] = ShipY - cosf(AngleRad) * ShipNoseOffset;
     BlastDirectionX[i] = sinf(AngleRad);
              BlastDirectionY[i] = -cosf(AngleRad);
              break;
            }
         }
      }

      //Blast movement
      for (int i = 0; i < MAX_BLAST; i++)
      {
          if (BlastActive[i])
          {
             BlastX[i] += BlastDirectionX[i] * BLAST_SPEED;
             BlastY[i] += BlastDirectionY[i] * BLAST_SPEED;
 
             if (BlastX[i] < 0 || BlastX[i] > screenWidth || BlastY[i] < 0 || BlastY[i] > screenHeight)
             {
       BlastActive[i] = false;
             }
          }
      }

      //Spawn Aliens
      if (!GameOver && SpawnTimer >= ALIEN_SPAWN_DELAY)
      {
         SpawnTimer = 0.0f;
         for (int i = 0; i < MAX_ALIENS; i++)
         {
            if (!AlienAlive[i])
            {
      AlienAlive[i] = true;
      int SpawnEdge = rand() % 4;
      switch(SpawnEdge)
      {
         case 0: AlienX[i] = rand() % screenWidth; AlienY[i] = -50; break;
         case 1: AlienX[i] = rand() % screenWidth; AlienY[i] = screenHeight + 50; break;
         case 2: AlienX[i] = screenWidth + 50; AlienY[i] = rand() % screenHeight; break;
 case 3: AlienX[i] = -50; AlienY[i] = rand() % screenHeight; break;
      }
               AlienType[i] = rand() % 3 + 1;
      float dx = ShipX - AlienX[i];
               float dy = ShipY - AlienY[i];
               float distance = sqrtf(dx*dx + dy*dy);
               AlienDirectionX[i] = dx / distance;
               AlienDirectionY[i] = dy / distance;
               break;
            }
         }
      }

      //Aliens movement
      for (int i = 0; i < MAX_ALIENS; i++)
      if (AlienAlive[i])
      {
         AlienX[i] += AlienDirectionX[i] * ALIEN_SPEED;
         AlienY[i] += AlienDirectionY[i] * ALIEN_SPEED;
      }
     
      // Explosion timer
      for (int i = 0; i < MAX_ALIENS; i++)
      if (ExplosionActive[i])
      {
         ExplosionTimer[i] += DeltaTime;
         if (ExplosionTimer[i] > 2.0f) ExplosionActive[i] = false;
      }
     
      //Collision detection
      Rectangle ShipCollision = GetShipCollision(ShipX, ShipY, Ship);
      for (int i = 0; i < MAX_ALIENS; i++)
      {
         if (!AlienAlive[i])
         continue;
         Texture2D AlienTexture = GetAlienType(AlienType[i], Alien1, Alien2, Alien3);
         Rectangle AlienRectangle = GetAlienRectangle(AlienX[i], AlienY[i], AlienTexture);

         if(CheckCollisionRecs(ShipCollision, AlienRectangle))
         {
            AlienAlive[i] = false;
            hearts--;
            if (hearts <=0)
            {
               GameOver = true;
            }
            continue;
         }
           
         for (int b = 0; b < MAX_BLAST; b++)
         {
            if(!BlastActive[b])
   continue;
            Rectangle BlastRectangle = GetBlastRectangle(BlastX[b], BlastY[b], Blast);
               
            if(CheckCollisionRecs(BlastRectangle, AlienRectangle))
            {
               AlienAlive[i] = false;
               BlastActive[b] = false;
               ExplosionActive[i] = true;
               ExplosionTimer[i] = 0.0f;

               if (!GameOver)
               {
                  ScoreValue++;
               }
               break;
            }
         }
      }
         
                     
      //Draw game
      BeginDrawing();
      ClearBackground(DARKBLUE);
     
      //Score
      DrawTexture(Score, 10, 20, WHITE);
      DrawText(TextFormat("%d", ScoreValue), 15 + Score.width , 30, 20, WHITE);

      //Hearts
      for (int i = 0; i < hearts; i++)
      DrawTexture(Heart , screenWidth - 10 - (i+1)*(Heart.width + 5), 10, WHITE);

      //Ship
      Vector2 ShipOrigin = GetShipOrigin(Ship);
      Rectangle ShipRectangle = GetShipRectangle(0, 0, Ship);
      Rectangle ShipDestination = GetShipRectangle(ShipX, ShipY, Ship);
      DrawTexturePro(Ship, ShipRectangle, ShipDestination, ShipOrigin, ShipAngle, WHITE);
     
     
      //Explosions
      for (int i = 0; i < MAX_ALIENS; i++)
      {
         if (ExplosionActive[i])
         {
            DrawTexture(Explosion, AlienX[i], AlienY[i], WHITE);
         }
      }
 
      //Blasts
      for (int i = 0; i < MAX_BLAST; i++)
      {
         if (BlastActive[i])
         {
            DrawTexture(Blast, BlastX[i], BlastY[i], WHITE);
         }
      }

      // Aliens
      for (int i = 0; i < MAX_ALIENS; i++)
      {
         if (AlienAlive[i])
         {
            Texture2D Alien;
            if (AlienType[i] == 1)
            {
               Alien = Alien1;
            }
            else if(AlienType[i] == 2)
            {
               Alien = Alien2;
            }
            else
            {
               Alien = Alien3;
            }
            DrawTexture(Alien, AlienX[i], AlienY[i], WHITE);
         }
      }

      // Game Over
      if (GameOver)
      {
         DrawText("GAME OVER", screenWidth/2 - 100, screenHeight/2 - 20, 40, RED);
         DrawRectangleRec(PlayButton, DARKGRAY);
         int textWidth = MeasureText("PLAY AGAIN", 25);
         DrawText("PLAY AGAIN", PlayButton.x + (PlayButton.width - textWidth)/2, PlayButton.y + 13, 25, WHITE);
 
         if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
         {
            Vector2 mouse = GetMousePosition();
            if (CheckCollisionPointRec(mouse, PlayButton))
            {
               // Reset game
               hearts = 3;
               ScoreValue = 0;
               SpawnTimer = 0;
               GameOver = false;
               for (int i = 0; i < MAX_ALIENS; i++)
               {
                  AlienAlive[i] = false;
                  ExplosionActive[i] = false;
               }
                  for (int i = 0; i < MAX_BLAST; i++)
                  BlastActive[i] = false;
               }
            }  
         }
 
      EndDrawing();
   }
 
   // Unload textures
   UnloadTexture(Ship);
   UnloadTexture(Blast);
   UnloadTexture(Explosion);
   UnloadTexture(Score);
   UnloadTexture(Heart);
   UnloadTexture(Alien1);
   UnloadTexture(Alien2);
   UnloadTexture(Alien3);
 
   CloseWindow();
   return 0;
}

//clang  main.c -o game -I/opt/homebrew/opt/raylib/include -L/opt/homebrew/opt/raylib/lib -lraylib -framework OpenGL -framework Cocoa -framework IOKit -framework CoreVideo
// cd Desktop/ViktoriaA-GameProject




