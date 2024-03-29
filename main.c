#include "VDP_TMS9918A.h"
#include "sprite.h"
#include "tilemap.h"
#include "input.h"


#define  HALT __asm halt __endasm
#define DEBUG_COLLISIONS 1

#define DIRECTION_UP 1
#define DIRECTION_UPRIGHT 2
#define DIRECTION_RIGHT 3
#define DIRECTION_DOWNRIGHT 4
#define DIRECTION_DOWN 5
#define DIRECTION_DOWNLEFT 6
#define DIRECTION_LEFT 7
#define DIRECTION_UPLEFT 8






int pos1x;
int pos1y;
int jump1;
int land1;
int walk1;
int sprite1;
int i;



const unsigned char TILESET_B0[]={	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, //0x00 tile vacío (fondo)
									0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF, //0x01 tile completo (plataforma)
									0x01,0x03,0x07,0x0F,0x1F,0x3F,0x7F,0xFF, //0x02 tile diagonal (caida)
									0x01,0x03,0x07,0x0F,0x1F,0x3F,0x7F,0xF}; //0x03 tile para detectar el comienzo de una diagonal
const unsigned char COLORSET_B0[]={	0x33, 0x33, 0x33, 0x33, 0x33, 0x33, 0x33, 0x33,
									0x95, 0x95, 0x95, 0x95, 0x95, 0x95, 0x95, 0x95,
									0x93, 0x93, 0x93, 0x93, 0x93, 0x93, 0x93, 0x93};
const unsigned char TILEMAP_B0[]={
0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,
0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,
0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,
0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,
0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,
0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,
0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,
0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,
0x01,0x00,0x00,0x00,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x00,0x00,0x00,0x01,
0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,
0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,
0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,
0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,
0x01,0x00,0x00,0x00,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x00,0x00,0x00,0x00,0x02,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x00,0x00,0x00,0x01,
0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x02,0x03,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,
0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x02,0x03,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,
0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x02,0x03,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,
0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x02,0x03,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,
0x01,0x00,0x00,0x00,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x03,0x00,0x00,0x00,0x00,0x00,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x00,0x00,0x00,0x01,
0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,
0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,
0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,
0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,
0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01
};



const unsigned char nick_0[]={ 0x03, 0x07, 0x0f, 0x14, 0x14, 0x14, 0x1f, 0x30, 0x6f, 0x66, 0x69, 0x2f, 0x19, 0x1f, 0x0e, 0x1f, 0xb0, 0xd8, 0xd8, 0xf0, 0xec, 0xf6, 0xb6, 0x6e, 0x9e, 0x4e, 0xdc, 0xe8, 0xf0, 0xf8, 0xe0, 0xf0 };
const unsigned char nick_1[]={ 0x03, 0x07, 0x0f, 0x14, 0x74, 0xd4, 0xff, 0x70, 0x2f, 0x36, 0x1c, 0x1f, 0x19, 0x1f, 0x0e, 0x0c, 0xb0, 0xd8, 0xd8, 0xf3, 0xe5, 0xed, 0x44, 0x66, 0xfe, 0x7c, 0xf8, 0xf0, 0xf0, 0xf0, 0xe0, 0xc0};

const unsigned char nick_r1[]={ 0x07,0x1f,0x27,0x27,0x26,0x3d,0x03,0x1f,0x1d,0x03,0x0e,0x06,0x05,0x03,0x01,0x03,0xb8,0xd8,0xe8,0xf0,0xf0,0xf0,0xf0,0xb8,0x5c,0x6c,0xb4,0xd4,0xec,0xfc,0xf8,0xf8};

const unsigned char nick_r2[]={0x07,0x1f,0x27,0x27,0x26,0x3d,0x03,0x1f,0x0d,0x03,0x0d,0x0d,0x0b,0x07,0x0f,0x1f,0xb8,0xd8,0xe8,0xf0,0xf0,0xf0,0xc8,0xbc,0xbe,0xde,0xee,0xf0,0xf8,0xfc,0x7c,0xf8,};

const unsigned char nick_r3[]={ 0x07,0x0f,0x13,0x13,0x13,0x1e,0x01,0x0f,0x0f,0x06,0x0d,0x0d,0x06,0x0f,0x1f,0x1e,0xb0,0xd8,0xd8,0xe8,0x70,0xf0,0xf0,0xb8,0x58,0xd8,0xd8,0xb8,0x78,0xfc,0x7c,0xf8};


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


int checkCollision(int posx, int posy, int direction)
{

	unsigned int vaddr1 = (((posy&0xF8)+16)*4)+((posx&0xf8)/8); // calcula la posicion en la VRAM
	unsigned int vaddr2 = (((posy&0xF8)+16)*4)+(((posx&0xf8)+8)/8); // calcula la posicion en la VRAM
#ifdef DEBUG_COLLISIONS
	load_tilemap((unsigned int) TILEMAP_B0, 768); //32*24 tiles	
	VPOKE(BASE10 + vaddr1, 0x03);
	VPOKE(BASE10 + vaddr2, 0x03);
#endif

	if(direction==DIRECTION_LEFT) //left
	{
		switch(TILEMAP_B0[vaddr1])
		{
			case 0x03: return 2;
			break;
			case 0x02: return 2;
			break;
			case 0x01: return 1;
			break;
			default: if(TILEMAP_B0[vaddr2]==0x01) return 1;
		}
	}else{ //right
		switch(TILEMAP_B0[vaddr2])
		{
			case 0x03: return 2;
			break;
			case 0x02: return 2;
			break;
			case 0x01: return 1;
			break;
			default: if(TILEMAP_B0[vaddr1]==0x01) return 1;
		}

	}
	//if((TILEMAP_B0[vaddr1]==0x01)||(TILEMAP_B0[vaddr2]==0x01)||(TILEMAP_B0[vaddr1]==0x02)||(TILEMAP_B0[vaddr2]==0x02)) return 1;

	return 0;
} 


void main(void)
{


//initialize screen mode
//POKE(0xF3AF,32); // set 32 columns per line
COLOR(15,15,15);

setup_sprites(SPRITE16X16, SPRITE_NO_ZOOM);
SCREEN(GRAPH1);



pos1x = 50;
pos1y = 168;
jump1 = 0;
land1 = 1;
walk1 = 0; //estado de la animación de caminar
sprite1=0;


//cargamos sprites
load_sprite(0, (unsigned int) nick_0, 16);
load_sprite(1, (unsigned int) nick_1, 16);
load_sprite(2, (unsigned int) nick_r1, 16);
load_sprite(3, (unsigned int) nick_r2, 16);
load_sprite(4, (unsigned int) nick_r3, 16);

//cargamos tilemap
load_tileset((unsigned int) TILESET_B0, 24, BANK0);
load_tileset((unsigned int) TILESET_B0, 24, BANK1);
load_tileset((unsigned int) TILESET_B0, 24, BANK2);
load_colormap((unsigned int) COLORSET_B0, 24, BANK0);
load_colormap((unsigned int) COLORSET_B0, 24, BANK1);
load_colormap((unsigned int) COLORSET_B0, 24, BANK2); 
load_tilemap((unsigned int) TILEMAP_B0, 768); //32*24 tiles

//load_sprite(i+2, (unsigned int) nick_2, 8);
//load_sprite(i+3, (unsigned int) nick_3, 8);




//FillVRAM(BASE12, 768, 0xA5); //character table
while(1){
	

	switch(walk1)
	{
		case 0:	
		case 3: load_sprite(0, (unsigned int) nick_r1, 16);
		break;
		case 4:	
		case 7: load_sprite(0, (unsigned int) nick_r2, 16);
		break;
		case 8:	
		case 11: load_sprite(0, (unsigned int) nick_r3, 16);
		break;

	}

	draw_sprite(0, pos1x, pos1y, LIGHT_BLUE);
	draw_sprite(4, 120, 168, LIGHT_RED);


	if((joystick(0)==DIRECTION_RIGHT)&&(pos1x<232))
	{ 
		pos1x=pos1x+2; //derecha
		switch (checkCollision(pos1x, pos1y, DIRECTION_RIGHT)) 
		{	
			case 0: land1=0;
			break;
			case 2: pos1y=pos1y-2;
			break;
		}
	}else if((joystick(0)==DIRECTION_LEFT)&&(pos1x>=10))
	{	 
		pos1x=pos1x-2; //izquierda
		walk1++;
		if(walk1==12) walk1=0;
		switch (checkCollision(pos1x, pos1y, DIRECTION_LEFT)) 
		{	
			case 0: land1=0;
			break;
			case 2: pos1y=pos1y-2;
			break;
		}
	}

	
	//si no aterrizó y no está saltando, hacer que caiga (gravedad)
	if((!land1)&&(!jump1)) 
	{	
		pos1y=pos1y+2;
		load_sprite(0, (unsigned int) nick_1, 16);
		if(checkCollision(pos1x, pos1y, 0))
		{ 
			land1=1; //si llegó al piso, setear el flag que está en una plataforma
			pos1y=pos1y&0xF8;
			load_sprite(0, (unsigned int) nick_0, 16);

		}	 
	}else if((!jump1)&&(land1))
	{
		switch (joystick(0))
		{
			case DIRECTION_UP: jump1=1; 
			land1=0;
			load_sprite(0, (unsigned int) nick_1, 16);
			break;
			case DIRECTION_UPRIGHT: jump1=1; 
			land1=0;
			pos1x=pos1x+2;
			break;
			case DIRECTION_UPLEFT: jump1=1; 
			land1=0;
			pos1x=pos1x-2;
			break; 
		}
	}else if(jump1){ 
		pos1y=pos1y-2;
		jump1++;
		if(jump1>25) jump1=0; //fin del salto
	}
	



	WAIT(1);

}



}
