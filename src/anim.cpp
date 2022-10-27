#include "anim.h"
#define SPRITE_DIR "./assets/textures/sprites/"

void InitAnimations()
{
    ani_explosion.init(SPRITE_DIR "explosion.png", 7, .1F);   // GRENADE EXPLOSION ANIM
    ani_lowerBody.init(SPRITE_DIR "lowerBody.png", 18, .06F);  // LOWER BODY ANIM. WALK / CROUCH / JUMP SPRITES 
    ani_upperBody.init(SPRITE_DIR "rifle.png", 16, .04F);      // UPPER BODY. AIM / SHOOT / MELEE / GRENADE SPRITES
}


