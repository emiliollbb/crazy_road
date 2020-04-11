//
// background_demo.c
// Essential tilemap loading: the start of backgroundstar from ducksoid 1
//
// (20060221 - 20070216, cearn)

#include <string.h>
#include <tonc.h>

#include "background.h"
#include "sprite_sheet.h"



struct car_s {
    int e,x,y,vx,vy;
    OBJ_ATTR *object;
};

struct game_s {
    struct car_s car;
    int objects;
    u32 frame;
};

OBJ_ATTR obj_buffer[128];
OBJ_AFFINE *obj_aff_buffer= (OBJ_AFFINE*)obj_buffer;

struct game_s game;

void init_game() {
    int i=0;
    game.objects=0;
    
    game.car.x=10;
    game.car.y=10;
    game.car.vx=1;
        game.car.vy=0;
        game.car.e=1;
        game.car.object=&obj_buffer[game.objects++];
    obj_set_attr(car.object, 
			ATTR0_SQUARE,
			ATTR1_SIZE_32,
			ATTR2_PALBANK(0) | 0);
}

void render_frame() {
  obj_set_pos(game.car.object, game.car.x, game.car.y);
  
  oam_copy(oam_mem, obj_buffer, game.objects);
}

void update_game() {
   
}


void init_sound() {
    // turn sound on
	REG_SNDSTAT= SSTAT_ENABLE;
	// snd1 on left/right ; both full volume
	REG_SNDDMGCNT = SDMG_BUILD_LR(SDMG_SQR1 | SDMG_NOISE, 7);
	// DMG ratio to 100%
	REG_SNDDSCNT= SDS_DMG100;
	// no sweep
	REG_SND1SWEEP= SSW_OFF;
	// envelope: vol=12, decay, max step time (7) ; 50% duty
	REG_SND1CNT= SSQR_ENV_BUILD(12, 0, 1) | SSQR_DUTY1_2;
	REG_SND1FREQ= 0;
    REG_SND4CNT= SSQR_ENV_BUILD(12, 0, 1) | SSQR_DUTY1_2;
	REG_SND4FREQ= 0;
}

void load_background() {
    // Load palette
	memcpy(pal_bg_mem, backgroundPal, backgroundPalLen);
	// Load tiles into CBB 0
	memcpy(&tile_mem[0][0], backgroundTiles, backgroundTilesLen);
	// Load map into SBB 30
	memcpy(&se_mem[30][0], backgroundMap, backgroundMapLen);
	
    // set up BG0 for a 4bpp 64x32t map, using
	//   using charblock 0 and screenblock 31
	REG_BG0CNT= BG_CBB(0) | BG_SBB(30) | BG_4BPP | BG_REG_64x32;
	
	// Backround H scroll
	REG_BG0HOFS= 0;
    // Background V scroll
	REG_BG0VOFS= 0;
	
}

void load_sprites() {
    // Load ducks sprites
	memcpy(pal_obj_mem, sprite_sheetPal, sprite_sheetPalLen);
	memcpy(&tile_mem[4][0], sprite_sheetTiles, sprite_sheetTilesLen);
	oam_init(obj_buffer, 128);
	
}

int main()
{
	// Init interrupts, and enable VBlank irq
	irq_init(NULL);
	irq_add(II_VBLANK, NULL);
	
	init_sound();
	
	load_background();
	
	//load_sprites();
		
    // Init Display
    REG_DISPCNT= DCNT_MODE0 | DCNT_BG0 | DCNT_OBJ | DCNT_OBJ_1D;
    
    
	init_game();
	
	while(1)
	{
		VBlankIntrWait();
		game.frame++;
		key_poll();

		//update_game();
		render_frame();
	}

	return 0;
}
