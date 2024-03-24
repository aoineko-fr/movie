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
#define MSX_GL "\x01\x02\x03\x04\x05\x06"

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
	if(g_Segment >= (310 - 66) * 3 + 6)
		g_Segment = 6;
	g_Page = 1 - g_Page;
	SET_BANK_SEGMENT(3, g_Segment);
	g_Segment++;
	SET_BANK_SEGMENT(4, g_Segment);
	g_Segment++;
	SET_BANK_SEGMENT(5, g_Segment);
	g_Segment++;
	VDP_WriteVRAM_128K((const u8*)0x6000, 128 * 34 + (g_Page * 128 * 256), 0, 128 * 144);
}

//-----------------------------------------------------------------------------
//
void SwapFrame()
{
	VDP_SetPage(g_Page);
	VDP_SetPalette((const u8*)0x6000 + 128 * 144);
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