#include <citro2d.h>

#include <3ds.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "days/day.h"
#include "days/day1.h"
#include "days/day2.h"
#include "days/day3.h"
#include "days/day4.h"
#include "days/day5.h"
#include "days/day6.h"
#include "days/day7.h"
#include "days/day8.h"
#include "days/day9.h"
#include "days/day10.h"
#include "days/day11.h"
#include "days/day12.h"
#include "util.h"

int main(int argc, char* argv[]) {
	gfxInitDefault();
	C3D_Init(C3D_DEFAULT_CMDBUF_SIZE);
	C2D_Init(C2D_DEFAULT_MAX_OBJECTS);
	C2D_Prepare();
	consoleInit(GFX_BOTTOM, NULL);

	bool error = false;
	
	// Init romfs just to check
	Result rc = romfsInit();
	if (rc)
	{
		printf("romfsInit: %08lX\n", rc);
		error = true;
	}

	// Create screens
	C3D_RenderTarget* top = C2D_CreateScreenTarget(GFX_TOP, GFX_LEFT);

	// Create colors
	constexpr u32 COL_Tri1 = C2D_Color32(0xFF, 0x15, 0x00, 0xFF);
	constexpr u32 COL_Tri2 = C2D_Color32(0x27, 0x69, 0xE5, 0xFF);

	constexpr u32 COL_Rec2 = C2D_Color32(0xFF, 0xFF, 0x2C, 0xFF);
	constexpr u32 COL_Rec1 = C2D_Color32(0x9A, 0x6C, 0xB9, 0xFF);
	constexpr u32 COL_Rec3 = C2D_Color32(0xD8, 0xF6, 0x0F, 0xFF);
	constexpr u32 COL_Rec4 = C2D_Color32(0x40, 0xEA, 0x87, 0xFF);

	constexpr u32 COL_Clear = C2D_Color32(0xFF, 0xD8, 0xB0, 0x68);

	u8 selectedDay = 1;
	Day* dayInstance = nullptr;

	while (aptMainLoop())
	{
		hidScanInput();

		u32 kDown = hidKeysDown();
		if (kDown & KEY_START)
		{
			break;
		}

		if (!error)
		{
			if (dayInstance == nullptr)
			{
				printf("\x1b[1;1HSelecting day %u\x1b[K", selectedDay);
				printf("\x1b[2;1HPress A to execute");
			}
			else
			{
				error = !dayInstance->Tick();
			}

			if (dayInstance == nullptr)
			{
				if (kDown & KEY_A)
				{
					consoleClear();

					switch (selectedDay)
					{
						case 1: dayInstance = new Day1(); break;
						case 2: dayInstance = new Day2(); break;
						case 3: dayInstance = new Day3(); break;
						case 4: dayInstance = new Day4(); break;
						case 5: dayInstance = new Day5(); break;
						case 6: dayInstance = new Day6(); break;
						case 7: dayInstance = new Day7(); break;
						case 8: dayInstance = new Day8(); break;
						case 9: dayInstance = new Day9(); break;
						case 10: dayInstance = new Day10(); break;
						case 11: dayInstance = new Day11(); break;
						case 12: dayInstance = new Day12(); break;
					}

					error = !dayInstance->Init();
				}
				else
				{
					if (kDown & KEY_CPAD_RIGHT || kDown & KEY_DRIGHT)
					{
						// Wrap from 12 to 1
						selectedDay = (selectedDay % 12) + 1;
					}
					if (kDown & KEY_CPAD_LEFT || kDown & KEY_DLEFT)
					{
						// Wrap from 1 to 12
						selectedDay = ((selectedDay + 10) % 12) + 1;
					}
				}
			}
			else
			{
				if (kDown & KEY_B)
				{
					// Kill current day and return to selection
					dayInstance->Cleanup();
					delete dayInstance;
					dayInstance = nullptr;
					
					consoleClear();
				}
			}
		}

		printf("\x1b[26;1H          Advent of Code 2025");
		printf("\x1b[27;1H          CPU:     %6.2f%%\x1b[K", C3D_GetProcessingTime()*6.0f);
		printf("\x1b[28;1H          GPU:     %6.2f%%\x1b[K", C3D_GetDrawingTime()*6.0f);
		printf("\x1b[29;1H          CmdBuf:  %6.2f%%\x1b[K", C3D_GetCmdBufUsage()*100.0f);

		C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
		C2D_TargetClear(top, COL_Clear);
		C2D_SceneBegin(top);

		if (dayInstance == nullptr)
		{
			C2D_DrawTriangle(50 / 2, SCREEN_HEIGHT - 50, COL_WHITE, 
				0,  SCREEN_HEIGHT, COL_Tri1,
				50, SCREEN_HEIGHT, COL_Tri2, 0);
			C2D_DrawRectangle(SCREEN_WIDTH - 50, 0, 0, 50, 50, COL_Rec1, COL_Rec2, COL_Rec3, COL_Rec4);
		}
		else if (!error)
		{
			error |= !dayInstance->Draw();
		}

		C3D_FrameEnd(0);
	}

	if (dayInstance != nullptr)
	{
		dayInstance->Cleanup();
		delete dayInstance;
	}

	// Deinit libs
	C2D_Fini();
	C3D_Fini();
	romfsExit();
	gfxExit();
	return 0;
}
