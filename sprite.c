

#include "VDP_TMS9918A.h"
#include "sprite.h"

void setup_sprites(char spritesize, char zoom) __naked {

	/*
		CLRSPR (0069H)		*3
		Function:	initialises all sprites. The sprite pattern is cleared to
		null, the sprite number to the sprite plane number, the
		sprite colour to the foregtound colour. The vertical location
		of the sprite is set to 209 (mode 0 to 3) or 217
		(mode 4 to 8).
		Input:	SCRMOD (FCAFH) for the screen mode
		Output:	none
		Registers:	all

		SETT32 (007BH)		*3
		Function:	set only VDP in GRAPHIC1 mode (32x24)
		Input:	same as INIT32
		Output:	none
		Registers:	all
	*/

	__asm

	push ix  	; prologue
	ld	ix,#0
	add	ix,sp

	push af    ; body
	push bc
	push de
	push hl
	push iy
  
	ld b,#0x00
	ld a,4(ix)
	and #0x0f
	cp #0x08
	jr z,$1
	set 1,b ; --- if 16x16 sprites => set bit 1
$1:
	ld a,5(ix)
	cp #0x00
	jr z, $2
	set 0,b ; --- if zoom sprites => set bit 0
$2:
	ld hl,#0xf3e0 ; --- read vdp(1) from mem
	ld a,(hl)
	and #0xfc
	or b
	ld (hl),a
	call 0x007b ; --- change vdp(1) bits

	ld a,#0x01
	ld hl,#0xfcaf
	ld (hl),a
	call 0x0069 ; --- reset sprite attributes

	pop iy
	pop hl
	pop de
	pop bc
	pop af

	pop	ix           ;epilogue
	ret

	__endasm;
}



void load_sprite(int index, unsigned int sprite, int size)
{


	if (size==8) 
	{
 		CopyToVRAM((unsigned int) sprite, BASE14+(index*8), 8);
	} else if (size==16)
	{
		CopyToVRAM((unsigned int) sprite, BASE14+(index*32), 32);
	}
}


/*The lower 4 bits of ``colour sprite'' define the colour. Bit 7 is the EC.
Bits 6 - 4 should be zero officially, but they are ignored. If the colour is 
0, the sprite is transparent (not visible).*/

void draw_sprite(int index, int posx, int posy, int color)
{

	VPOKE(BASE13+(index*4), posy); //sprite1y
	VPOKE(BASE13+(index*4)+0x01, posx); //sprite1x
	VPOKE(BASE13+(index*4)+0x02, index); //sprite1index
	VPOKE(BASE13+(index*4)+0x03, color); //sprite1color

}
