#include "VDP_TMS9918A.h"
#include "tilemap.h"



void load_tileset(unsigned int tileset, int size)
{
	CopyToVRAM((unsigned int) tileset, BASE12, size); //character table
}


void load_colormap(unsigned int tileset, int size)
{
	//FillVRAM(BASE11, 100, LIGHT_GREEN); //color table
	CopyToVRAM((unsigned int) tileset, BASE11, size);
}


void load_tilemap(unsigned int tileset, int size)
{
	//FillVRAM(BASE10, 768, 0x00); //name table
	CopyToVRAM((unsigned int) tileset, BASE10, size);
}
