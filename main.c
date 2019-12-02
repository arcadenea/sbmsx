#include "VDP_TMS9918A.h"
#include "sprite.h"
#include "tilemap.h"
#include "input.h"


#define  HALT __asm halt __endasm

int inclr;
int incud;
int i;

const unsigned char TILESET_B0[]={0xAA,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF};
const unsigned char COLORSET_B0[]={LIGHT_RED,LIGHT_GREEN,LIGHT_GREEN,LIGHT_GREEN,LIGHT_GREEN,LIGHT_GREEN,LIGHT_GREEN,LIGHT_GREEN};
const unsigned char TILEMAP_B0[]={0x00,0x01,0x00,0x01,0x00,0x01,0x00,0x01};



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



//cargamos sprites
load_sprite(0, (unsigned int) nick_0, 16);
load_sprite(1, (unsigned int) nick_1, 16);

//cargamos tilemap
load_tileset((unsigned int) TILESET_B0, 8);
load_colormap((unsigned int) COLORSET_B0, 8); //temporal
load_tilemap((unsigned int) TILEMAP_B0, 8); //temporal

//load_sprite(i+2, (unsigned int) nick_2, 8);
//load_sprite(i+3, (unsigned int) nick_3, 8);




//FillVRAM(BASE12, 768, 0xA5); //character table
while(1){
	

	draw_sprite(0, 0x10+inclr, 0x10+incud, BLACK);
	draw_sprite(4, 0x50, 0x50, MAGENTA);



	if(joystick(0)==3) inclr=inclr+2; //derecha
	if(joystick(0)==7) inclr=inclr-2; //izquierda
	if(joystick(0)==1) incud=incud-2; //arriba
	if(joystick(0)==5) incud=incud+2; //abajo

	
	//VPRINT(0,1,text01);




	WAIT(1);

}



}
