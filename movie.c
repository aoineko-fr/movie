// ____________________________
// ██▀▀█▀▀██▀▀▀▀▀▀▀█▀▀█        │   ▄▄▄                ▄▄      
// ██  ▀  █▄  ▀██▄ ▀ ▄█ ▄▀▀ █  │  ▀█▄  ▄▀██ ▄█▄█ ██▀▄ ██  ▄███
// █  █ █  ▀▀  ▄█  █  █ ▀▄█ █▄ │  ▄▄█▀ ▀▄██ ██ █ ██▀  ▀█▄ ▀█▄▄
// ▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀────────┘                 ▀▀
//  Program template
//─────────────────────────────────────────────────────────────────────────────

//=============================================================================
// INCLUDES
//=============================================================================
#include "msxgl.h"

//=============================================================================
// DEFINES
//=============================================================================

// Library's logo
#define MSX_GL 				"\x01\x02\x03\x04\x05\x06"

#define MOVIE_WIDTH			256
#define MOVIE_HEIGHT		144
#define MOVIE_POS_X			0
#define MOVIE_POS_Y			((212 - MOVIE_HEIGHT) / 2)
#define MOVIE_BPP			4
#define MOVIE_SCAN			(MOVIE_WIDTH * MOVIE_BPP / 8)
#define MOVIE_BYTES			(MOVIE_SCAN * MOVIE_HEIGHT)

//=============================================================================
// VARIABLE
//=============================================================================

bool g_VBlank = false;
u16 g_Segment = 6;
u8 g_Page = 1;

//=============================================================================
// READ-ONLY DATA
//=============================================================================

// Fonts data
#include "font/font_mgl_sample6.h"

// Animation characters
const u8 g_ChrAnim[] = { '-', '/', '|', '\\' };

//=============================================================================
// MAIN LOOP
//=============================================================================

//-----------------------------------------------------------------------------
// Write data from RAM to VRAM [MSX2/2+/TR]
void WriteVRAM(const u8* src, u16 dest)
{
	src;  // HL
	dest; // DE

	__asm

		// Setup address register 
		ld		a, d
		rlca
		rlca
		and		a, #0x03
		out		(P_VDP_ADDR), a			// RegPort = (page << 2) + (dest >> 14)
		ld		a, #VDP_REG(14)
		out		(P_VDP_REG), a			// RegPort = VDP_REG(14)
		ld		a, e
		out		(P_VDP_ADDR), a			// RegPort = (dest & 0xFF)
		ld		a, d
		and		a, #0x3F
		or		a, #F_VDP_WRIT
		out		(P_VDP_ADDR), a			// RegPort = ((dest >> 8) & 0x3F) + F_VDP_WRIT

		// Copy setup
		ld		c, #P_VDP_DATA			// data register
		ld		a, #(MOVIE_BYTES / 256)

		// Copy loop
	vram_write_loop:
		.rept 256
		outi
		.endm
		dec		a
		jp		nz, vram_write_loop

	__endasm;
}

//-----------------------------------------------------------------------------
//
void VDP_InterruptHandler()
{
	g_VBlank = TRUE;
}

//-----------------------------------------------------------------------------
//
void WaitVBLank()
{
	while(!g_VBlank) {}
	g_VBlank = FALSE;
}

//-----------------------------------------------------------------------------
//
void LoadFrame()
{
	if(g_Segment >= (335 - 66) * 3 + 6)
		g_Segment = 6;
	g_Page = 1 - g_Page;
	SET_BANK_SEGMENT(3, g_Segment);
	g_Segment++;
	SET_BANK_SEGMENT(4, g_Segment);
	g_Segment++;
	SET_BANK_SEGMENT(5, g_Segment);
	g_Segment++;
	WriteVRAM((const u8*)0x6000, MOVIE_SCAN * MOVIE_POS_Y + (g_Page * MOVIE_SCAN * 256));
}

//-----------------------------------------------------------------------------
//
void SwapFrame()
{
	VDP_SetPage(g_Page);
	VDP_SetPalette((const u8*)0x6000 + MOVIE_BYTES);
}

//-----------------------------------------------------------------------------
// Program entry point
void main()
{
	VDP_SetMode(VDP_MODE_SCREEN5);
	VDP_SetColor(COLOR_TRANSPARENT);
	VDP_EnableDisplay(FALSE);
	VDP_EnableVBlank(TRUE);
	VDP_ClearVRAM();

	// Print_SetBitmapFont(g_Font_MGL_Sample6);
	// Print_SetColor(COLOR_WHITE, COLOR_TRANSPARENT);
	// Print_SetPosition(0, 0);
	// Print_DrawText(MSX_GL" Movie player sample");
	// Print_SetPosition(0, 256);
	// Print_DrawText(MSX_GL" Movie player sample");

	LoadFrame();
	SwapFrame();
	VDP_EnableDisplay(TRUE);

	u8 count = 0;
	while(1)
	{
		LoadFrame();
		Halt();
		// WaitVBLank();
		SwapFrame();
	}
}