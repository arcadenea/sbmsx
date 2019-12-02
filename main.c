#include "VDP_TMS9918A.h"
#include "sprite.h"
#include "input.h"

#define  HALT __asm halt __endasm

int inclr;
int incud;
int i;

const unsigned char TILESET_B0[]={0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA};



const unsigned char nick_0[]={ 0x03, 0x07, 0x0f, 0x14, 0x14, 0x14, 0x1f, 0x30, 0x6f, 0x66, 0x69, 0x2f, 0x19, 0x1f, 0x0e, 0x1f, 0xb0, 0xd8, 0xd8, 0xf0, 0xec, 0xf6, 0xb6, 0x6e, 0x9e, 0x4e, 0xdc, 0xe8, 0xf0, 0xf8, 0xe0, 0xf0 };
const unsigned char nick_1[]={ 0x00, 0x07, 0x08, 0x10, 0x2b, 0x2b, 0x30, 0x7f, 0xb0, 0xbf, 0xf8, 0xff, 0x7f, 0x3f, 0x1f, 0x3f, 0x70, 0xf8, 0xf8, 0x70, 0x38, 0x3e, 0x7b, 0xf1, 0x69, 0xf3, 0xf7, 0xfe, 0xfc, 0xfc, 0xf8, 0xfc};

//const unsigned char nick_2[]={ 0xb0, 0xbf, 0xf8, 0xff, 0x7f, 0x3f, 0x1f, 0x3f };
//const unsigned char nick_3[]={ 0x69, 0xf3, 0xf7, 0xfe, 0xfc, 0xfc, 0xf8, 0xfc };

const char text01[] = "Macri Gato!"; 

void VPRINT(char column, char line, char* text);  //print in screen 1 or 2

void VPOKEARRAY(unsigned int vaddr, char* text)
{
  while(*(text)) VPOKE(vaddr++,*(text++));
}

void VPRINT(char column, char line, char* text)
{
  unsigned int vaddr = BASE10 + (line*32)+column; // calcula la posicion en la VRAM
  VPOKEARRAY(vaddr, text);
}

void WAIT(unsigned int cicles)
{
  int i;
  for(i=0;i<cicles;i++) HALT;
}

void POKE(unsigned int address, char value)
{
address;value;
__asm
  push IX
  ld   IX,#0
  add  IX,SP 
    
  ld   L,4(ix)
  ld   H,5(ix)
  ld   A,6(ix)
  ld   (HL),A

  pop  IX  
__endasm;
}

void fillOrdered()
{
  unsigned int i;
  for(i=0;i<255;i++) VPOKE(BASE11+i,i);
}


void main(void)
{


//initialize screen mode
POKE(0xF3AF,32); // set 32 columns per line
COLOR(15,15,15);
SCREEN(GRAPH1);

setup_sprites(SPRITE16X16, SPRITE_NO_ZOOM);

inclr = 0;
incud = 0;
//fillOrdered();
CopyToVRAM((unsigned int) TILESET_B0, BASE12, 16*8); //character table


//cargamos sprites

load_sprite(0, (unsigned int) nick_0, 16);
load_sprite(1, (unsigned int) nick_1, 16);
//load_sprite(i+2, (unsigned int) nick_2, 8);
//load_sprite(i+3, (unsigned int) nick_3, 8);



FillVRAM(BASE11, 100, LIGHT_GREEN); //color table
FillVRAM(BASE10, 768, 0x00); //name table
//FillVRAM(BASE12, 768, 0xA5); //character table
while(1){
	

	draw_sprite(0, 0x10+inclr, 0x10+incud, BLACK);
	draw_sprite(4, 0x50, 0x50, MAGENTA);
/*	draw_sprite(2, 0x10+incremento, 0x20, BLACK);
	draw_sprite(3, 0x20+incremento, 0x20, BLACK);
	draw_sprite(4, 0x30+incremento, 0x40, CYAN);
	draw_sprite(5, 0x40+incremento, 0x40, CYAN);
	draw_sprite(6, 0x30+incremento, 0x50, CYAN);
	draw_sprite(7, 0x40+incremento, 0x50, CYAN);

	draw_sprite(8, 0x10, 0x10+incremento, MAGENTA);
	draw_sprite(9, 0x20, 0x10+incremento, MAGENTA);
	draw_sprite(10, 0x10, 0x20+incremento, MAGENTA);
	draw_sprite(11, 0x20, 0x20+incremento, MAGENTA);
	draw_sprite(12, 0x20, 0x50+incremento, GREY);
	draw_sprite(13, 0x30, 0x50+incremento, GREY);
	draw_sprite(14, 0x20, 0x60+incremento, GREY);
	draw_sprite(15, 0x30, 0x60+incremento, GREY);

	draw_sprite(16, 0x10+incremento, 0x10+incremento, DARK_YELLOW);
	draw_sprite(17, 0x20+incremento, 0x10+incremento, DARK_YELLOW);
	draw_sprite(18, 0x10+incremento, 0x20+incremento, DARK_YELLOW);
	draw_sprite(19, 0x20+incremento, 0x20+incremento, DARK_YELLOW);
	draw_sprite(20, 0x50+incremento, 0x50+incremento, DARK_GREEN);
	draw_sprite(21, 0x60+incremento, 0x50+incremento, DARK_GREEN);
	draw_sprite(22, 0x50+incremento, 0x60+incremento, DARK_GREEN);
	draw_sprite(23, 0x60+incremento, 0x60+incremento, DARK_GREEN);

	draw_sprite(24, 0x60+incremento, 0x70, LIGHT_BLUE);
	draw_sprite(25, 0x70+incremento, 0x70, LIGHT_BLUE);
	draw_sprite(26, 0x60+incremento, 0x80, LIGHT_BLUE);
	draw_sprite(27, 0x70+incremento, 0x80, LIGHT_BLUE);*/



//	draw_sprite(27, 0x80+incremento, 0x50, 0x04);
//	draw_sprite(28, 0xA0+incremento, 0x50, 0x05);
//	draw_sprite(29, 0xC0+incremento, 0xD0, 0x06);
//	draw_sprite(30, 0xD0+incremento, 0xD0, 0x07);
//	draw_sprite(31, 0xF0+incremento, 0xD0, 0x08);



	if(joystick(0)==3) inclr=inclr+2; //derecha
	if(joystick(0)==7) inclr=inclr-2; //izquierda
	if(joystick(0)==1) incud=incud-2; //arriba
	if(joystick(0)==5) incud=incud+2; //abajo

	
	//VPRINT(0,1,text01);




	WAIT(1);

}



}
