

#ifndef  __SPRITE_H__
#define  __SPRITE_H__

#define SPRITE8X8      8
#define SPRITE16X16   32

#define SPRITE_NO_ZOOM 0
#define SPRITE_ZOOM    1

void setup_sprites(char spritesize, char zoom);

void load_sprite(int index, unsigned int sprite, int size);

void draw_sprite(int index, int posx, int posy, int color);


#endif
