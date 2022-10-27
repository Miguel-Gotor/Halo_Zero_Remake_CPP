#include <math.h>

#include "raylib.h"

#include "audio.h"
#include "anim.h"
#include "text.h"

#define vertical_align() ani_upperBody.y = ani_lowerBody.y - ani_upperBody.frameHeight + correctionY
#define horizontal_align() (ani_upperBody.direction == 1)?(ani_upperBody.x = ani_lowerBody.x + correctionX):(ani_upperBody.x = ani_lowerBody.x - (ani_upperBody.frameWidth - ani_lowerBody.frameWidth + correctionX))


// 1st variables, 2nd textures, 3rd sounds

enum ApplicationStates
{
	Startup,
	Loading,
	Menu,
	Running,
	Paused,
	GameOver,
	Quitting
};

ApplicationStates ApplicationState = Startup;

enum Weapons
{
    Rifle = 0,
    Handgun = 1,
    Plasmagun = 2,
    Minigun = 3,
    Knife = 4
};

Weapons currentWeapon = Rifle;

// gunfireStyle

// enum FireModes
// {
//     fullauto = 0,
//     singleshot = 1,
// };

/*########################################################################################################################################################################################

                                                                                G L O B A L    V A R I A B L E S

##########################################################################################################################################################################################*/

// SOUNDS

Sound snd_stepGrass;
Sound snd_stepGravel1;
Sound snd_stepGravel2;
Sound snd_jump2;
Sound snd_jump1;
Sound snd_gear;
Sound snd_rifle;
Sound snd_shotgun;
Sound snd_handgun;
Sound snd_melee;
Sound snd_lasergun;
Sound snd_minigun;
Sound snd_ambience;
Sound snd_noAmmo;
Sound snd_reload;
Sound snd_bullet;
Sound snd_pause;
Sound snd_unpause;
Sound snd_screenshot;
Sound snd_loop;
Sound snd_grenade;
Sound snd_enterCar;
Sound snd_leaveCar;
Sound snd_explosion;
Sound snd_damage;
Sound snd_death;
Sound snd_gameOver;
Sound snd_hints;
Sound snd_heartBeat;
Sound snd_knifeOut;
Sound snd_knifeSlash;

// ANIMATIONS

Animation ani_upperBody;
Animation ani_lowerBody;
Animation ani_explosion;
Animation ani_warthog;

// TEXTURES

Texture2D t2D_weapon;
Texture2D t2D_grass; 
Texture2D t2D_sea;
Texture2D t2D_sky;
Texture2D t2D_warthog;
Texture2D t2D_grenade;
Texture2D t2D_masterChief;
Texture2D t2D_death;

// PLAYER STATUS

static int correctionY;
static int correctionX;

static int InitialWorldX;
static int inc;
static bool closeToGrenade;

static bool shooting;
static int  walking;
static bool jumping;
static bool melee;
static bool throwing;
static bool driving;

static bool stabbing;

static bool upperBodyIdle;

static bool dead;

static bool grenade;
static bool explosion;
static bool canEnterVehicle;

static int crouching;
static int ammo;
static int health;

static int weapon;

// static float jumpDuration = 2;
// static int jumpHeight = 64;

// HUD - UI

static bool paused;
static bool hints;

// Trigonometrics

static float angle = 0;
static int opposite_side = 0;
static int adjacent_side = 0;

// Coordinates

static int worldX = 0;
static int shoulderX, shoulderY;
static int mouseX = 0;
static int mouseY = 0;
static int absoluteX = 0;

static int groundLevel;
static int hipLevel;
static int moveStep = 5;

// Auxiliar

int aux = 1;

static int mouseWheelMove = 0;
static bool LMB_Pressed, LMB_Down;

static int warthogX = 0;
static int speed = 0;

// Grenade

static int grenadeY         = 0.F;
static int apparentGrenadeY = 0.F;
static int grenadeX         = 0.F;
static int apparentGrenadeX = 0.F;

static float grenadeSpeedY = 0.F;
static float grenadeSpeedX = 0.F;
static int grenadeRotation = 0;

static float grenadeTimer1 = 0.F;
static float grenadeTimer2 = 0.F;

static int distanceToGrenade = 0;

static float gameClock = 0.F;

constexpr int screenWidth = 854, screenHeight = 480;    // 16:9    Stardard FWVGA 

Sound *ptr_snd_shoot;

// FUNCTIONS

void EquipWeapon()
{
    // "Reset" upper body
    shooting = melee = throwing = false;

    switch (currentWeapon) // Switch Case because more options will be added 
    {
        case Rifle: // RIFLE SHOOTING. AUTOMATIC. HOLD KEY DOWN TO BLAST NONSTOP

            ani_upperBody.init("./assets/textures/sprites/rifle.png", 16, 0.04);
            ptr_snd_shoot = &snd_rifle;

            correctionY = 34;
            correctionX = 8;

            if (not crouching)
            {
                moveStep = 5;
                ani_lowerBody.timeStep = 0.06f;
            }

            break;

        case Handgun: // HANDGUN. NOT AUTOMATIC, BUT THERE IS NO "COOLDOWN".

            ani_upperBody.init("./assets/textures/sprites/handgun.png", 16, 0.12);
            ptr_snd_shoot = &snd_handgun;

            correctionY = 29;
            correctionX = 8;

            if (not crouching)
            {
                moveStep = 6;
                ani_lowerBody.timeStep = 0.066f;
            }

            break;

        case Plasmagun: // MINIGUN. AUTOMATIC. HOLD KEY DOWN TO BLAST NONSTOP

            ani_upperBody.init("./assets/textures/sprites/lasergun.png", 2, 0.06);
            ptr_snd_shoot = &snd_lasergun;

            correctionY = 7;
            correctionX = 7;

            if (not crouching)
            {
                moveStep = 4;
                ani_lowerBody.timeStep = 0.08f;
            }

            break;

        case Minigun: // MINIGUN. AUTOMATIC. HOLD KEY DOWN TO BLAST NONSTOP

            ani_upperBody.init("./assets/textures/sprites/minigun.png", 2, 0.04);
            ptr_snd_shoot = &snd_minigun;

            correctionY = 22;
            correctionX = 7;

            if (not crouching)
            {
                moveStep = 6;
                ani_lowerBody.timeStep = 0.08;
            }

            break;

        case Knife:

            ani_upperBody.init("./assets/textures/sprites/knife.png", 4, 0.2);
            ptr_snd_shoot = &snd_knifeSlash;

            correctionY = 16;
            correctionX = 0;

            if (not crouching)
            {
                moveStep = 6;
                ani_lowerBody.timeStep = 0.066F;
            }

            break;
            
        default:
            break;
    }

    vertical_align();
    horizontal_align();
}

void InitPlayer()
{
    walking = false;
    jumping = false;
    driving = false;
    paused = false;
    hints = false;
    closeToGrenade = false;
    dead = false;
    grenade = false;
    explosion = false;
    crouching = 0;

    groundLevel = screenHeight  - t2D_grass.height / 3;
    hipLevel = groundLevel - ani_lowerBody.frameHeight;
    moveStep = 5;

    InitialWorldX = 0;
    inc = 0;

    ammo = 32;
    health = 100;

    ani_lowerBody.x = 105;    
    ani_lowerBody.y = hipLevel;

    worldX = 0;

    EquipWeapon();
}

void SetupWindow(int screenWidth, int screenHeight)
{
    InitWindow(screenWidth, screenHeight, "Halo Zero");
    SetWindowState(FLAG_VSYNC_HINT);
    SetExitKey(KEY_NULL);

    Image WindowIcon = LoadImage("./assets/icons/prueba.png");
    ImageFormat(&WindowIcon, PIXELFORMAT_UNCOMPRESSED_R8G8B8A8); 
    SetWindowIcon(WindowIcon);
    UnloadImage(WindowIcon);

    HideCursor();
}

void checkHealth()
{
    if (health < 1) 
    {
        health = 0;
        aux = 0;

        walking = false;  
        dead = true;
                
        ani_lowerBody.timer = 0.F;

        PlaySound(snd_death);
        // PlaySound(snd_gameOver);
    }
}

void Restart()
{
    InitAnimations();
    InitPlayer();
}

int main(void)
{

    SetupWindow(screenWidth, screenHeight);
    LoadTextures();
    InitAudio();

    Restart();

/*###############################################################################################################################################################################################

                                                                                   S U P E R L O O P

###############################################################################################################################################################################################*/

    while(!WindowShouldClose() && ApplicationState != Quitting)
    {
        // INPUT HANDLING
        
        // 1ST - LIST OF ACTIONS THE PLAYER CAN PERFORM IN ALL CASES

        if (IsKeyPressed(KEY_F11))      // SCREENSHOTS
        {
            PlaySound(snd_screenshot);
            TakeScreenshot(TextFormat("./screenshots/%.2f.png", GetTime()));  // For now filename will be taken from time elapsed so it doesn't overwrite
        }

        else if (IsKeyPressed(KEY_ESCAPE))
        {
            CloseWindow();
        }

        else if (IsKeyPressed(KEY_P))   // PAUSE / UNPAUSE GAME
        {
            paused = !paused;

            if (paused) 
            {
                PlaySound(snd_pause);
                StopSound(snd_ambience);
            }
            else
            {        
                PlaySound(snd_unpause);
                StopSound(snd_loop);
            }
        }

        else if (IsKeyPressed(KEY_H))   // SHOW / HIDE HINTS
        {
            hints = !hints;
            if (hints) PlaySound(snd_hints);
        }


        if (paused)
        {
            if (!IsSoundPlaying(snd_loop)) PlaySound(snd_loop);
        }

/*###############################################################################################################################################################################################                                                                            G A M E    L O O P

###############################################################################################################################################################################################*/

        else // not paused - game running
        {
            if (!IsSoundPlaying(snd_ambience)) PlaySound(snd_ambience);

            // Update timers

            upperBodyIdle = !throwing && !melee;

            gameClock += GetFrameTime();

            ani_lowerBody.timer  += GetFrameTime();
            ani_upperBody.timer  += GetFrameTime();

            // 1 - Shoulder

            shoulderX = ani_lowerBody.x + 38 - 6 * ani_lowerBody.direction;
            shoulderY = ani_lowerBody.y - 12;

            // 2 - Mouse 

            mouseX = GetMouseX();
            mouseY = GetMouseY();
            mouseWheelMove = GetMouseWheelMove();
            LMB_Pressed = IsMouseButtonPressed(0);
            LMB_Down = IsMouseButtonDown(0);
            
            if (dead && ani_lowerBody.timer > .2F)
            {
                ani_lowerBody.timer = 0.F;
                if (aux < 2) aux ++;
            }

            if (!dead)
            {

                if (health < 30 && !IsSoundPlaying(snd_heartBeat)) PlaySound(snd_heartBeat);

                canEnterVehicle = (abs((warthogX + t2D_warthog.width / 2) - (ani_lowerBody.x + ani_lowerBody.frameWidth / 2)) < 25);

                // TRIGONOMETRY

                // Compute weapon angle
                
                opposite_side = (shoulderY - mouseY);
                adjacent_side = (mouseX - shoulderX);

                // CHANGE DIRECTION

                if ((adjacent_side < 0 && ani_upperBody.direction == 1) || (adjacent_side > 0 && ani_upperBody.direction == -1))
                {
                    ani_upperBody.direction = (ani_lowerBody.direction *= -1);
                    horizontal_align();
                    PlaySound(snd_stepGrass);
                }

                if (adjacent_side != 0 && currentWeapon < 2)
                angle = atan((float)opposite_side / adjacent_side) * 180 / PI * ani_upperBody.direction ;

                // Constrain to ±30º

                if (angle > 30) angle = 30;
                else if (angle < -30) angle = -30;

                // HANDLE INPUT

                if (IsKeyPressed(KEY_E) && canEnterVehicle)
                {
                    driving = !driving;

                    if (driving)
                    {
                        UnloadTexture(t2D_warthog);
                        t2D_warthog = LoadTexture ("./assets/textures/sprites/warthog2.png");
                        PlaySound(snd_enterCar);   
                    }

                    else
                    {            
                        UnloadTexture(t2D_warthog);
                        t2D_warthog = LoadTexture ("./assets/textures/sprites/warthog.png");
                        PlaySound(snd_leaveCar);
                    }      
                }

                else if (!driving)
                {

                    warthogX = 50 - worldX;

                    // RELOAD
                    if (IsKeyPressed(KEY_R) && ammo < 32) 
                    {
                        ammo = 32;
                        PlaySound(snd_reload);
                    }

                    // SWITCH WEAPON 
                    
                    if ((mouseWheelMove > 0 && currentWeapon < Weapons(4)) || (mouseWheelMove < 0 && currentWeapon > Weapons(0)))
                    {
                        currentWeapon = Weapons(currentWeapon + mouseWheelMove);
                        EquipWeapon();
                        if (currentWeapon == Knife)
                        PlaySound(snd_knifeOut);
                        else
                        PlaySound(snd_gear);
                    }

                    else if (upperBodyIdle)
                    {
                                
                        // SHOOT
                        if (currentWeapon == Knife && ! stabbing &&IsMouseButtonPressed(0))
                        {
                            stabbing = true;
                            ani_upperBody.timer = 0.F;
                            PlaySound(*ptr_snd_shoot);
                        }
                        else if ((((currentWeapon != Handgun) && !shooting && IsMouseButtonDown(0)) || (currentWeapon == Handgun && IsMouseButtonPressed(0))) && ammo > 0)
                        {
                            shooting = true;
                            ani_upperBody.timer = 0.F;
                            ammo--;
                            PlaySound(*ptr_snd_shoot);
                            if (currentWeapon == Handgun &&  ani_upperBody.timer < ani_upperBody.timeStep) ani_upperBody.timer = ani_upperBody.timeStep - .1F;
                            // if (currentWeapon == Handgun && shooting) ani_upperBody.timer += .1F;
                        }

                        // else if (LMB_Pressed)
                        // {
                        //     PlaySound(snd_noAmmo);
                        // }

                        // MELEE ATTACK
                        if (IsKeyDown(KEY_LEFT_CONTROL) && currentWeapon < Plasmagun)
                        {
                            melee = true;
                            ani_upperBody.timer = 0.F;
                            PlaySound(snd_melee);
                        }

                        // GRENADE
                        else if (IsKeyPressed(KEY_G) && !grenade)
                        {
                            throwing = true;

                            ani_upperBody.timer = 0.F;

                            ani_upperBody.init("./assets/textures/sprites/throw.png", 5, 0);

                            correctionY = 8;
                            correctionX = 3;
                            
                            vertical_align();
                            horizontal_align();
                        }
                    }

                    // CROUCH
                    if (IsKeyPressed(KEY_C) && crouching == 0 && !jumping)
                    {
                        crouching = 1;
                        aux = 1;
                        ani_lowerBody.timer = 0.F;
                        PlaySound(snd_stepGravel1);
                    }

                    // JUMP / STAND UP
                    else if (IsKeyPressed(KEY_SPACE) && !jumping)
                    {
                        if (crouching == 2)
                        {
                            crouching = -1;
                            ani_lowerBody.timer = 0.F;
                            PlaySound(snd_stepGravel2);
                        }

                        else if (crouching == 0)
                        {
                            jumping = true;
                            ani_lowerBody.timer = 0.F;
                            PlaySound(snd_jump1);
                        }
                    }

                    // WALK
                    else if (IsKeyDown(KEY_D))
                    {
                        walking = 1;
                    }

                    else if (IsKeyDown(KEY_A))
                    {
                        walking = -1;
                    }

                    else
                    {
                        walking = false;
                    }
                }

                // DRIVING

                if (IsKeyDown(KEY_SPACE) && driving)
                {
                    if (speed < 600)
                    speed += 10;
                }
                else if (speed > 0)
                { 
                    speed -= 5;
                }
                else if (speed < 0) speed = 0;

                if (speed > 0)
                {
                    if ((ani_lowerBody.x > screenWidth / 2) || (ani_lowerBody.x < ani_lowerBody.frameWidth)) 
                    {
                        worldX += speed * GetFrameTime(); // CAMERA FOLLOWS CHARACTER
                    }
                    else
                    {
                        ani_lowerBody.x += speed * GetFrameTime();; // FIX CAMERA
                    }

                    warthogX  = ani_lowerBody.x + ani_lowerBody.frameWidth / 2  - t2D_warthog.width / 2;

                    //warthogX = ani_lowerBody.x - t2D_warthog.width / 2;
                }


            }  // END OF INPUT HANDLING

            // PLAYER DEAD ↓

            else if (IsKeyPressed(KEY_R))
            {
                Restart();
            }
            else if (IsKeyPressed(KEY_Q))
            {
                ApplicationState = Quitting;
            }

/*###############################################################################################################################################################################################

                                                                                   A C T I O N S

###############################################################################################################################################################################################*/
            
            // 1st - Upper Body Animations
            
            if (throwing)
            {
                if (ani_upperBody.timer < .08F)      ani_upperBody.frame = 3;
                else if (ani_upperBody.timer < .16F) ani_upperBody.frame = 2;
                else if (ani_upperBody.timer < .24F) ani_upperBody.frame = 1;
                else if (ani_upperBody.timer < .4F) ani_upperBody.frame = 0;
                else if (ani_upperBody.timer < .5F) ani_upperBody.frame = 1;
                else if (ani_upperBody.timer < .6F)
                {
                    if (ani_upperBody.frame !=2) PlaySound(snd_grenade);
                    ani_upperBody.frame = 2;
                }
                else if (ani_upperBody.timer < .64F) 
                {
                    ani_upperBody.frame = 3;

                    if (!grenade)
                    {
                        grenade = true;
                        InitialWorldX = worldX;
                        grenadeTimer1 = grenadeTimer2 = 0.F;
                        grenadeX = shoulderX + 22;
                        grenadeY = shoulderY - 6;
                        grenadeSpeedX = adjacent_side + 50 ;
                        grenadeSpeedY = opposite_side * -2.5 - 50;
                    }
                }
                else if (ani_upperBody.timer < .8F) ani_upperBody.frame = 4;

                else 
                {
                    throwing = false;
                    EquipWeapon();
                }
            } // throwing

            else if (stabbing)
            {
                // if (ani_upperBody.timer < .6F) ani_upperBody.frame = 1;
                // else if (ani_upperBody.timer < .8F) ani_upperBody.frame = 2;
                // else if (ani_upperBody.timer < 1.2F) ani_upperBody.frame = 3;
                // else stabbing = false;   
                if (ani_upperBody.timer < .06F) ani_upperBody.frame = 1;
                else if (ani_upperBody.timer < .12F) ani_upperBody.frame = 2;
                else if (ani_upperBody.timer < .26F) ani_upperBody.frame = 3;
                else stabbing = false;   
            }

            else if (melee)
            {
                if (ani_upperBody.timer < .12F)      ani_upperBody.frame = 5;
                else if (ani_upperBody.timer < .24F) ani_upperBody.frame = 14;
                else if (ani_upperBody.timer < .36F) ani_upperBody.frame = 15;
                else if (ani_upperBody.timer < .4F) ani_upperBody.frame = 3;
                else melee = false;   
            }

            else if (shooting && currentWeapon != Knife)
            {
                if (ani_upperBody.timer < ani_upperBody.timeStep)           ani_upperBody.frame = 10 + (int)angle / 10 ;
                else if (ani_upperBody.timer < ani_upperBody.timeStep * 2)  ani_upperBody.frame = 3  + (int)angle / 10;
                else shooting = false;
            }

            else if (currentWeapon < Weapons(2))
            {
                ani_upperBody.frame = 3 + (int)angle / 10;
            }
            else ani_upperBody.frame = 0;

            // 2nd - Lower Body Animations

            if (crouching == 1)
            {
                if (ani_lowerBody.timer < .1F) // UPDATE FRAME
                {
                    ani_lowerBody.frame = 14;
                    ani_lowerBody.y = hipLevel + 6;
                }
                else
                {
                    ani_lowerBody.frame = 15;
                    ani_lowerBody.y = hipLevel + 29;
                    ani_lowerBody.timeStep = .2F;
                    moveStep = 2;
                    crouching = 2;
                }

                vertical_align();
            }

            else if (crouching == -1)
            {
                if (ani_lowerBody.timer < .1F) // UPDATE FRAME
                {
                    ani_lowerBody.frame = 14;
                    ani_lowerBody.y = hipLevel + 6;
                }
                else 
                {
                    ani_lowerBody.y = hipLevel;
                    ani_lowerBody.frame = 0;

                    crouching = 0;

                    ani_lowerBody.timeStep = .06F;
                    moveStep = 5;
                }

                vertical_align();
            }
                    
            else if (walking || jumping)
            {
                if (walking)
                {
                    // CHECK IF CHARACTER IS GETTING TOO CLOSE TO SCREEN EDGE
                    if ((walking == 1 && ani_lowerBody.x > screenWidth / 2) || (walking == -1 && ani_lowerBody.x < ani_lowerBody.frameWidth)) 
                    {
                        worldX += moveStep * walking; // CAMERA FOLLOWS CHARACTER
                    }
                    else
                    {
                        ani_lowerBody.x += moveStep * walking; // FIX CAMERA.
                        ani_upperBody.x += moveStep * walking;
                    }

                    // CHECK TIMER
                    
                    if (ani_lowerBody.timer >= ani_lowerBody.timeStep && !jumping)     // TIME ELAPSED > TIME STEP?
                    {
                        ani_lowerBody.timer = 0.F;    // RESET TIMER

                        if (crouching == 2) // Player is crouching
                        {

                            if      (ani_lowerBody.frame == 17 ) aux = -1;
                            else if (ani_lowerBody.frame == 16 ) PlaySound(snd_stepGrass);
                            else if (ani_lowerBody.frame == 15 ) aux = 1;

                            ani_lowerBody.frame += aux;
                        }
                        else // Player is Standing
                        {
                            ani_lowerBody.frame += walking * ani_lowerBody.direction;

                            if (currentWeapon < 2)
                            {
                                if (ani_lowerBody.frame > 10) ani_lowerBody.frame = 1;
                                else if (ani_lowerBody.frame < 1) ani_lowerBody.frame = 9;
                            }
                            else
                            {
                                if (ani_lowerBody.frame > 9) ani_lowerBody.frame = 1;
                                else if (ani_lowerBody.frame < 1) ani_lowerBody.frame = 9;
                            }

                            ani_lowerBody.y = hipLevel + (abs(ani_lowerBody.frame - 5)); // Slighty sway side to side

                            if (ani_lowerBody.frame == 2)      PlaySound(snd_stepGravel1);
                            else if (ani_lowerBody.frame == 7) PlaySound(snd_stepGravel2);
                        }
                    }
                    vertical_align();
                }

                // JUMPING

                if (jumping)
                {     
                    if (ani_lowerBody.timer < .5F) // UPDATE FRAME
                    {
                        ani_lowerBody.y = hipLevel - 64 * sin( 2 * PI * ani_lowerBody.timer);

                        if (ani_lowerBody.timer < .1F)       ani_lowerBody.frame = 13;
                        else if (ani_lowerBody.timer < .2F)  ani_lowerBody.frame = 12;
                        else if (ani_lowerBody.timer < .3F)  ani_lowerBody.frame = 11;
                        else if (ani_lowerBody.timer < .4F)  ani_lowerBody.frame = 12;
                        else if (ani_lowerBody.timer < .5F)  ani_lowerBody.frame = 13;
                    }              
                    // if (ani_lowerBody.timer < 1/jumpDuration) // UPDATE FRAME
                    // {
                    //     ani_lowerBody.y = hipLevel - jumpHeight * sin( jumpDuration * PI * ani_lowerBody.timer);
                    //     // ani_lowerBody.y = hipLevel - 64 * sin( (float)2 / 3 * PI * ani_lowerBody.timer);

                    //     if (ani_lowerBody.timer < 0.1f)       ani_lowerBody.frame = 13;
                    //     else if (ani_lowerBody.timer < 0.2f)  ani_lowerBody.frame = 12;
                    //     else if (ani_lowerBody.timer < 0.3f)  ani_lowerBody.frame = 11;
                    //     else if (ani_lowerBody.timer < 0.4f)  ani_lowerBody.frame = 12;
                    //     else if (ani_lowerBody.timer < 0.5f)  ani_lowerBody.frame = 13;
                    // }
                    else
                    {
                        ani_lowerBody.frame = 0;
                        
                        ani_lowerBody.y = hipLevel; // FOOT ON THE GROUND
                        jumping = false;

                        PlaySound(snd_jump2);
                    }
                    
                    // VERTICAL ALIGN (NEEDS TO BE AT THE END )
                    
                    vertical_align();
                } 
            }
            else if (crouching == 0) ani_lowerBody.frame = 0;
            else ani_lowerBody.frame = 15;

            // GRENADE

            if (grenade) // Grenade Flight
            {
                // Always update grenade timers

                grenadeTimer1 += GetFrameTime();
                grenadeTimer2 += GetFrameTime();

                // Now we compute relative speed to the player's movement.

                inc = InitialWorldX - worldX;
                
                closeToGrenade = ((distanceToGrenade = abs(shoulderX - grenadeX - inc)) < 200);

                // CINEMÁTICA DE LA GRANADA

                if (grenadeY < groundLevel - 20) // Before hitting the floor
                {
                    grenadeSpeedY += 1300 * GetFrameTime();     // Update speed in order to...
                    grenadeY += grenadeSpeedY * GetFrameTime(); // Update position

                    if (grenadeY > groundLevel - 20)
                    { 
                        grenadeY = groundLevel - 20;
                        PlaySound(snd_stepGrass);
                    }
                }
               
                else if (abs(grenadeSpeedX) > 5) // Grenade in the ground
                {
                    grenadeSpeedX *= .95F; // Slow speed by friction

                    if (grenadeTimer1 > 8 / abs(grenadeSpeedX)) // rotation speed proportional to movement speed like IRL
                    {
                        grenadeTimer1 = 0.F;
                        if (grenadeSpeedX > 0) grenadeRotation += 90;
                        else                   grenadeRotation -= 90;
                        if (grenadeRotation == 360) grenadeRotation = 0;
                        else if (grenadeRotation == 0) grenadeRotation = 270;
                    }
                }

                else 
                {
                    grenadeSpeedX = 0;
                }

                grenadeX += GetFrameTime() * grenadeSpeedX;
                
                if (grenadeRotation == 0)
                {
                    apparentGrenadeX = grenadeX - t2D_grenade.width / 2 + inc;
                    apparentGrenadeY = grenadeY - t2D_grenade.height / 2;
                }
                else if (grenadeRotation == 90)
                {
                    apparentGrenadeX = grenadeX + t2D_grenade.width / 2 + inc;
                    apparentGrenadeY = grenadeY - t2D_grenade.height / 2;
                }
                else if (grenadeRotation == 180)
                {
                    apparentGrenadeX = grenadeX + t2D_grenade.width / 2 + inc;
                    apparentGrenadeY = grenadeY + t2D_grenade.height / 2;
                }
                else if (grenadeRotation == 270)
                {
                    apparentGrenadeX = grenadeX - t2D_grenade.width / 2 + inc;
                    apparentGrenadeY =  grenadeY + t2D_grenade.height / 2;
                }

                if (grenadeTimer2 > 2.F || (CheckCollisionPointCircle(Vector2{(float)mouseX, (float)mouseY}, Vector2{(float)grenadeX, (float)grenadeY}, 5.F) && LMB_Down))
                {
                    grenade = false;
                    explosion = true;

                    ani_explosion.x = apparentGrenadeX - ani_explosion.frameWidth / 2;
                    ani_explosion.y = groundLevel - ani_explosion.frameHeight;

                    PlaySound(snd_explosion);
                    
                    if (closeToGrenade)
                    {
                        health -= (200 - distanceToGrenade);

                        checkHealth();

                        if (!dead) PlaySound(snd_damage);
                    }
                }
            }

            if (explosion)
            {
                ani_explosion.timer += GetFrameTime();

                inc = InitialWorldX - worldX;

                ani_explosion.x = grenadeX - ani_explosion.frameWidth / 2 + inc;
                ani_explosion.y = groundLevel - ani_explosion.frameHeight;

                if (ani_explosion.timer > ani_explosion.timeStep)
                {
                    ani_explosion.timer = 0.F;
                    if (++ ani_explosion.frame == 7) 
                    {
                        explosion = false;
                        ani_explosion.frame = 0;
                    }
                }
            }

            absoluteX = worldX + ani_lowerBody.x;
   

        } // GAME LOOP END

/*###############################################################################################################################################################################################

                                                                                  D R A W I N G

###############################################################################################################################################################################################*/

        BeginDrawing();
        ClearBackground(CLITERAL(Color){0, 0, 255, 255}); // Pure blue to match the sky color and fill it.

        // PHARALLAX

        // SKY TILE
        for (int i = 0; i <= screenWidth / t2D_sky.width; i++)
        {
            DrawTextureRec(t2D_sky, Rectangle{(float)worldX / 4, 0.F, (float)t2D_sky.width, (float)t2D_sky.height}, Vector2{(float)i * t2D_sky.width, (float)screenHeight - t2D_grass.height - t2D_sea.height + 8 - t2D_sky.height}, WHITE);
        }

        // SEA TILE
        for (int i = 0; i <= screenWidth / t2D_sea.width; i++)
        {
            DrawTextureRec(t2D_sea, Rectangle{(float)worldX / 2, 0.F, (float)t2D_sea.width, (float)t2D_sea.height}, Vector2{(float)i * t2D_sea.width, (float)screenHeight - t2D_grass.height - t2D_sea.height + 8}, WHITE);
        }

        // GRASS TILE
        for (int i = 0; i <= screenWidth / t2D_grass.width; i++)
        {
            DrawTextureRec(t2D_grass, Rectangle{(float)worldX, 0.F, (float)t2D_grass.width, (float)t2D_grass.height}, Vector2{(float)i * t2D_grass.width, (float)screenHeight - t2D_grass.height }, WHITE);
        }    

        if (shooting && currentWeapon == Weapons::Minigun && ani_upperBody.timer == 0.F)
        {
            ani_lowerBody.x--;
            ani_upperBody.x--;
            // DrawLineEx(Vector2{(float)shoulderX, (float)shoulderY + GetRandomValue(0,2) + 15}, Vector2{(float)shoulderX + screenWidth, (float)shoulderY + GetRandomValue(0,2) + 15}, GetRandomValue(1,2), WHITE);
            DrawRectangle(shoulderX, shoulderY + 15 + GetRandomValue(0,2), screenWidth, GetRandomValue(1,2), WHITE);
        }

        // WARTHOG
        if (canEnterVehicle)    DrawTexture(t2D_warthog, warthogX, groundLevel - t2D_warthog.height, WHITE);
        else                    DrawTexture(t2D_warthog, warthogX, groundLevel - t2D_warthog.height, LIGHTGRAY);

        if (grenade)
        {
            DrawRing(Vector2{(float)grenadeX + inc + 20, (float)grenadeY - 20}, 0.F, 10.F, 0.F, grenadeTimer2 / 2.F * 360.F, 30, YELLOW);
            DrawTextureEx(t2D_grenade, Vector2{(float)apparentGrenadeX, (float)apparentGrenadeY}, (float)grenadeRotation, 1.F, WHITE);
            if (closeToGrenade)
            DrawText("(!)", shoulderX, shoulderY - 55, 20, RED);
        }

        else if (explosion)
        {
            if (ani_explosion.frame == 0)
            {
                DrawRectangle(0, 0, screenWidth, screenHeight, WHITE); // FLASH EFFECT
                animColor = BLACK;
            }
            else
            {
                animColor = WHITE;
            }

            ani_explosion.Draw();
        }

        // MASTER CHIEF

        if (dead)
        {
            DrawTextureRec(t2D_death, Rectangle{(float)t2D_death.width / 3 * aux, 0, (float)t2D_death.width / 3, (float)t2D_death.height}, Vector2{(float)ani_lowerBody.x, (float)groundLevel - t2D_death.height}, WHITE);
        }


        else if (!driving)
        {
            // Shadow

            DrawEllipse(ani_lowerBody.x + ani_lowerBody.frameWidth / 2 - 2, screenHeight - t2D_grass.height / 2, 28, 10, CLITERAL(Color){ 0, 0, 0, (unsigned char)(64 - (hipLevel - ani_lowerBody.y))});
            ani_lowerBody.Draw();
            ani_upperBody.Draw();
        }

        // CROSSHAIR
        DrawLineEx(Vector2{(float)mouseX, (float)mouseY - 12}, Vector2{(float)mouseX, (float)mouseY + 12}, 2.0,RED);
        DrawLineEx(Vector2{(float)mouseX - 12, (float)mouseY}, Vector2{(float)mouseX + 12, (float)mouseY}, 2.0,RED);

        // DRIVING MESSAGE

        if (driving)
        DrawText("Press E to exit the vehicle", warthogX + t2D_warthog.width / 2 - MeasureText("Press E to enter the vehicle", 10) / 2, groundLevel - t2D_warthog.height - 20, 10, WHITE);
        else if (canEnterVehicle)
        DrawText("Press E to enter the vehicle", warthogX + t2D_warthog.width / 2 - MeasureText("Press E to enter the vehicle", 10) / 2, groundLevel - t2D_warthog.height - 20, 10, WHITE);

        //-----------------------
        // INFO

        DrawFPS(screenWidth / 2, 20);

        // PLAYER AND CAR POSITION

        DrawTriangle(Vector2{(float)ani_lowerBody.x + ani_lowerBody.frameWidth / 2 - 10, screenHeight / 2 + 40}, 
                        Vector2{(float)ani_lowerBody.x + ani_lowerBody.frameWidth / 2, screenHeight / 2 + 50}, 
                        Vector2{(float)ani_lowerBody.x + ani_lowerBody.frameWidth  /2 + 10, screenHeight / 2 + 40},
                        GREEN);

        DrawTriangle(Vector2{(float)warthogX + t2D_warthog.width / 2 - 10, screenHeight / 2 + 40}, 
                        Vector2{(float)warthogX + t2D_warthog.width / 2, screenHeight / 2 + 50}, 
                        Vector2{(float)warthogX + t2D_warthog.width / 2 + 10, screenHeight / 2 + 40},
                        GREEN);

        // VARIABLES

        //if (health < 30) DrawRectangle(0,0,screenWidth,screenHeight, CLITERAL(Color){0,0,0,128}); // FLASH EFFECT

        // HUD
        DrawRectangleRounded(Rectangle{10, 10, 240, 60}, .5F, 10,  CLITERAL(Color){0, 0, 0, 128});
        DrawTexture(t2D_masterChief, 20, 20, WHITE);
        DrawText(TextFormat("HP:   %i/100", health), 64, 24, 10, WHITE); DrawRectangle(140.F, 20.F, (float)health, 10.F, RED );
        for (int i = 0; i < ammo; i++)
        {
            DrawRectangle(80 + i * 5, 45, 3, 10, YELLOW);
        }

        DrawRectangleRounded(Rectangle{screenWidth - 260 , 10, 250, 60}, 1.F, 8,  CLITERAL(Color){0, 0, 0, 128});
        DrawTextureRec(t2D_weapon, Rectangle{0, (float)t2D_weapon.height / 3 * weapon, (float)t2D_weapon.width, (float)t2D_weapon.height / 3}, Vector2{screenWidth - 240, 30}, WHITE);
        DrawRing(Vector2{screenWidth - 40, 40}, 20, 30, 0, (360.F / 32.F) * ammo, 30, CLITERAL(Color){255,0,0,128});
        DrawText(TextFormat("%i/32", ammo), screenWidth - 55, 35, 10, WHITE);
        DrawText("MA5B Assault Rifle", screenWidth - 180, 20, 10, WHITE);

        if (hints)
        {
            DrawRectangleRounded(Rectangle{screenWidth - 260, 85, 250, 360}, 0.25f, 10,  CLITERAL(Color){0, 0, 0, 160});

            DrawText(TextFormat("distance to grenade : %d ", distanceToGrenade), 630, 100, 10, WHITE);
            DrawText(TextFormat("world x: %i", worldX), 630, 120, 10, WHITE);
            DrawText(TextFormat("player x: %i", ani_lowerBody.x), 630, 140, 10, WHITE);
            DrawText(TextFormat("absolute x: %i", absoluteX), 630, 160, 10, WHITE);
            DrawText(TextFormat("y: %i", ani_lowerBody.y), 630, 180, 10, WHITE);

            DrawText(TextFormat("Health: %i", health), 630, 200, 10, WHITE);
            DrawText(TextFormat("Total Elapsed Time: %02.02f s", GetTime()), 630, 220, 10, WHITE);

            DrawText(TextFormat("walking: %d", walking),630, 240, 10, CLITERAL(Color){(unsigned char)(255 * !walking), (unsigned char)(255 * walking), (unsigned char)(255 * !walking), 255});
            DrawText(TextFormat("jumping: %d", jumping),630, 260, 10, CLITERAL(Color){(unsigned char)(255 * !jumping), (unsigned char)(255 * jumping), (unsigned char)(255 * !jumping), 255});
            DrawText(TextFormat("shooting: %d", shooting),630, 280, 10, CLITERAL(Color){(unsigned char)(255 * !shooting), (unsigned char)(255 * shooting), (unsigned char)(255 * !shooting), 255});
            DrawText(TextFormat("crouching: %d", crouching),630, 300, 10, CLITERAL(Color){(unsigned char)(255 * !crouching), (unsigned char)(255 * crouching), (unsigned char)(255 * !crouching), 255});
            DrawText(TextFormat("throwing: %d", throwing),630, 320, 10, CLITERAL(Color){(unsigned char)(255 * !throwing), (unsigned char)(255 * throwing), (unsigned char)(255 * !throwing), 255});
            DrawText(TextFormat("melee: %d", melee),630, 340, 10, CLITERAL(Color){(unsigned char)(255 * !melee), (unsigned char)(255 * melee), (unsigned char)(255 * !melee), 255});

            DrawText(TextFormat("frame: %d ", ani_lowerBody.frame),630, 360, 10, WHITE);
            DrawText(TextFormat("angle : %f ", angle),630, 380, 10, WHITE);
            DrawText(TextFormat("Game Elapsed Time: %2.02f s", gameClock), 630, 400, 10, WHITE);
        }

        if (dead)
        {
            for (int i = 0; i < screenHeight; i+= 2) DrawLine(0, i, screenWidth, i, CLITERAL(Color){0, 0, 0, 64});

            DrawText("GAME OVER", screenWidth / 2 - MeasureText("GAME OVER", 40) / 2, screenHeight / 2 - 80, 40, WHITE);

            DrawText("[R]estart", screenWidth / 2 - MeasureText("[R]estart", 20) / 2, screenHeight / 2, 20, WHITE);
            DrawText("[Q]uit", screenWidth / 2 - MeasureText("[Q]uit", 20) / 2, screenHeight / 2 + 40, 20, WHITE);
        }

        else if (paused)
        {
            for (int i = 0; i < screenHeight; i += 3) DrawLine(0, i, screenWidth, i, CLITERAL(Color){0, 0, 0, 64});

            DrawText("GAME PAUSED", screenWidth / 2 - MeasureText("GAME PAUSED", 20) / 2, screenHeight / 2, 20, WHITE);
        }

        EndDrawing();
    }

    UnloadTextures();
    UnloadSounds();

    CloseWindow();
 
    return 0;
}