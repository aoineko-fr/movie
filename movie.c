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
#include "mglv/mglv_player.h"

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
#define MOVIE_ADDR			((const u8*)(0x8000))

#if (ROM_MAPPER == ROM_NEO8)
	#define MOVIE_BANK			4
	#define MOVIE_SEGMENT		6
#elif (ROM_MAPPER == ROM_NEO16)
	#define MOVIE_BANK			2
	#define MOVIE_SEGMENT		3
#elif (ROM_MAPPER == ROM_YAMANOOTO)
	#define MOVIE_BANK			2
	#define MOVIE_SEGMENT		4
#elif (ROM_MAPPER == ROM_ASCII16X)
	#define MOVIE_BANK			1
	#define MOVIE_SEGMENT		2
#endif

//=============================================================================
// VARIABLE
//=============================================================================

u8 g_Frame = 0;
bool g_VBlank = false;
u16 g_Segment = MOVIE_SEGMENT;
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


// //-----------------------------------------------------------------------------
// // Write data from RAM to VRAM [MSX2/2+/TR]
// void WriteVRAM(const u8* src, u16 dest)
// {
// 	src;  // HL
// 	dest; // DE

// 	__asm

// 		// Setup address register 
// 		ld		a, d
// 		rlca
// 		rlca
// 		and		a, #0x03
// 		out		(P_VDP_ADDR), a			// RegPort = (page << 2) + (dest >> 14)
// 		ld		a, #VDP_REG(14)
// 		out		(P_VDP_REG), a			// RegPort = VDP_REG(14)
// 		ld		a, e
// 		out		(P_VDP_ADDR), a			// RegPort = (dest & 0xFF)
// 		ld		a, d
// 		and		a, #0x3F
// 		or		a, #F_VDP_WRIT
// 		out		(P_VDP_ADDR), a			// RegPort = ((dest >> 8) & 0x3F) + F_VDP_WRIT

// 		// Copy setup
// 		ld		c, #P_VDP_DATA			// data register
// 		ld		a, #(MOVIE_BYTES / 256)

// 		// Copy loop
// 	vram_write_loop:
// 		.rept 256
// 		outi
// 		.endm
// 		dec		a
// 		jp		nz, vram_write_loop

// 	__endasm;
// }

//-----------------------------------------------------------------------------
// Interrupt handler called at each V-Blank
void VDP_InterruptHandler()
{
	MGLV_VBlankHandler();
	g_VBlank = TRUE;
	g_Frame++;
}

//-----------------------------------------------------------------------------
// Wait of V-Blank signal
void WaitVBLank()
{
	while(!g_VBlank) {}
	g_VBlank = FALSE;
}

//-----------------------------------------------------------------------------
// Default event callback
void MovieEventCallback(u8 cmd)
{
	switch(cmd)
	{
	case MGLV_CMD_END_VIDEO:
		g_MGLV_Pointer = g_MGLV_Start;
		g_Segment = MOVIE_SEGMENT;
#if (ROM_MAPPER >= ROM_MAPPER_16K)
		SET_BANK_SEGMENT(MOVIE_BANK, g_Segment);
#else // ROM_MAPPER_8K
		SET_BANK_SEGMENT(MOVIE_BANK + 0, g_Segment);
		SET_BANK_SEGMENT(MOVIE_BANK + 1, ++g_Segment);
#endif
		break;

	case MGLV_CMD_END_SEGMENT:
		g_MGLV_Pointer = MOVIE_ADDR;
#if (ROM_MAPPER >= ROM_MAPPER_16K)
		SET_BANK_SEGMENT(MOVIE_BANK, ++g_Segment);
#else // ROM_MAPPER_8K
		SET_BANK_SEGMENT(MOVIE_BANK + 0, ++g_Segment);
		SET_BANK_SEGMENT(MOVIE_BANK + 1, ++g_Segment);
#endif
		break;
	}
}

//-----------------------------------------------------------------------------
// Program entry point
void main()
{
	VDP_SetMode(VDP_MODE_SCREEN5);
	VDP_SetColor(COLOR_BLACK);
	VDP_EnableDisplay(FALSE);
	VDP_ClearVRAM();
	VDP_EnableSprite(FALSE);

	MGLV_Init(MovieEventCallback);
	MGLV_SetFrameDuration(5);
	MovieEventCallback(MGLV_CMD_END_VIDEO); // Initialize mapper
	MGLV_Play(MOVIE_ADDR);

#if ((ROM_MAPPER == ROM_YAMANOOTO) || (ROM_MAPPER == ROM_ASCII16X))
	Bios_SetHookCallback(H_TIMI, VDP_InterruptHandler);
#endif

	VDP_EnableDisplay(TRUE);

	u8 count = 0;
	while(1)
	{
		WaitVBLank();
		// DisableInterrupt();
		MGLV_Decode();
		// EnableInterrupt();
	}
}