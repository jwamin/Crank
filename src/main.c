//
//  main.c
//  Extension
//
//  Created by Joss Manger on 7/30/14.
//  Copyright (c) 2014 Jossy, Inc. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>

#include "pd_api.h"

static int update(void* userdata);
const char* fontpath = "/System/Fonts/Asheville-Sans-14-Bold.pft";
LCDFont* font = NULL;

#ifdef _WINDLL
__declspec(dllexport)
#endif

typedef uint8_t Pattern[8];

static Pattern patterns[] =
{
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
    { 0x80, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00 },
    { 0x88, 0x00, 0x00, 0x00, 0x88, 0x00, 0x00, 0x00 },
    { 0x88, 0x00, 0x20, 0x00, 0x88, 0x00, 0x02, 0x00 },
    { 0x88, 0x00, 0x22, 0x00, 0x88, 0x00, 0x22, 0x00 },
    { 0xa8, 0x00, 0x22, 0x00, 0x8a, 0x00, 0x22, 0x00 },
    { 0xaa, 0x00, 0x22, 0x00, 0xaa, 0x00, 0x22, 0x00 },
    { 0xaa, 0x00, 0xa2, 0x00, 0xaa, 0x00, 0x2a, 0x00 },
    { 0xaa, 0x00, 0xaa, 0x00, 0xaa, 0x00, 0xaa, 0x00 },
    { 0xaa, 0x40, 0xaa, 0x00, 0xaa, 0x04, 0xaa, 0x00 },
    { 0xaa, 0x44, 0xaa, 0x00, 0xaa, 0x44, 0xaa, 0x00 },
    { 0xaa, 0x44, 0xaa, 0x10, 0xaa, 0x44, 0xaa, 0x01 },
    { 0xaa, 0x44, 0xaa, 0x11, 0xaa, 0x44, 0xaa, 0x11 },
    { 0xaa, 0x54, 0xaa, 0x11, 0xaa, 0x45, 0xaa, 0x11 },
    { 0xaa, 0x55, 0xaa, 0x11, 0xaa, 0x55, 0xaa, 0x11 },
    { 0xaa, 0x55, 0xaa, 0x51, 0xaa, 0x55, 0xaa, 0x15 },
    { 0xaa, 0x55, 0xaa, 0x55, 0xaa, 0x55, 0xaa, 0x55 },
    { 0xba, 0x55, 0xaa, 0x55, 0xab, 0x55, 0xaa, 0x55 },
    { 0xbb, 0x55, 0xaa, 0x55, 0xbb, 0x55, 0xaa, 0x55 },
    { 0xbb, 0x55, 0xea, 0x55, 0xbb, 0x55, 0xae, 0x55 },
    { 0xbb, 0x55, 0xee, 0x55, 0xbb, 0x55, 0xee, 0x55 },
    { 0xfb, 0x55, 0xee, 0x55, 0xbf, 0x55, 0xee, 0x55 },
    { 0xff, 0x55, 0xee, 0x55, 0xff, 0x55, 0xee, 0x55 },
    { 0xff, 0x55, 0xfe, 0x55, 0xff, 0x55, 0xef, 0x55 },
    { 0xff, 0x55, 0xff, 0x55, 0xff, 0x55, 0xff, 0x55 },
    { 0xff, 0x55, 0xff, 0xd5, 0xff, 0x55, 0xff, 0x5d },
    { 0xff, 0x55, 0xff, 0xdd, 0xff, 0x55, 0xff, 0xdd },
    { 0xff, 0x75, 0xff, 0xdd, 0xff, 0x57, 0xff, 0xdd },
    { 0xff, 0x77, 0xff, 0xdd, 0xff, 0x77, 0xff, 0xdd },
    { 0xff, 0x77, 0xff, 0xfd, 0xff, 0x77, 0xff, 0xdf },
    { 0xff, 0x77, 0xff, 0xff, 0xff, 0x77, 0xff, 0xff },
    { 0xff, 0xf7, 0xff, 0xff, 0xff, 0x7f, 0xff, 0xff },
    { 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff }
};

int eventHandler(PlaydateAPI* pd, PDSystemEvent event, uint32_t arg)
{
	//(void)arg; // arg is currently only used for event = kEventKeyPressed
    const char* err;
    PDButtons pushed;
	switch (event)
	{
        case kEventInit:
            font = pd->graphics->loadFont(fontpath, &err);

            if ( font == NULL )
                pd->system->error("%s:%i Couldn't load font %s: %s", __FILE__, __LINE__, fontpath, err);

            // Note: If you set an update callback in the kEventInit handler, the system assumes the game is pure C and doesn't run any Lua code in the game
            pd->system->setUpdateCallback(update, pd);
            break;
        case kEventKeyPressed:
            pd->system->getButtonState(NULL, &pushed, NULL);
            pd->system->logToConsole("Button down! %d %d %d\n",event, pushed, arg);
            break;
        case kEventKeyReleased:

            break;
        default:
            break;
    }

	return 0;
}


#define TEXT_WIDTH 86
#define TEXT_HEIGHT 16
#define DRAW_MAX 33

int x = (400-TEXT_WIDTH)/2;
int y = (240-TEXT_HEIGHT)/2;
int dx = 1;
int dy = 2;
unsigned int pressing = 0;
unsigned int drawCount = 0;


static int update(void* userdata)
{
	PlaydateAPI* pd = userdata;
    PDButtons current, pushed, released;

    float crankValue = pd->system->getCrankAngle();

    pd->system->getButtonState(&current, &pushed, &released);
    if (pushed || released){
        char buffer[33];
        sprintf(buffer, "%d",pushed);
        //pd->system->logToConsole("Button! %d %d %d\n",current, pushed, released);
        pd->system->logToConsole("%s",buffer);
        if (pushed) {
            pressing = 1;
        } else if (released) {
            pressing = 0;
        }
    }





    if ((pushed & kButtonB) == kButtonB) {
        pd->system->logToConsole("b button pushed");
    }

    if ((pushed & kButtonA) == kButtonA) {
        pd->system->logToConsole("a button pushed");
    }

    if ((released & kButtonA) == kButtonA) {
        pd->system->logToConsole("a button released");
    }

    if ((released & kButtonB) == kButtonB) {
        pd->system->logToConsole("b button released");
    }


    uint8_t *ptr = (uint8_t*)&patterns[drawCount];

//    int endAngle = 360;
//    if (drawCount < DRAW_MAX) {
//        endAngle = (unsigned int)(float)(360 * ((float)drawCount / DRAW_MAX));
//        drawCount++;
//    } else {
//        drawCount = 0;
//        endAngle = 0;
//    }

    int endAngle = crankValue;
    drawCount = 32 - (32 * (crankValue / 360));

    LCDPattern colorPattern;
    LCDColor color;
    *colorPattern = *ptr;
    //{0xaa, 0x55, 0xaa, 0x55, 0xaa, 0x55, 0xaa, 0x55,1,1,1,1,1,1,1,1};
    //LCDPattern myPattern = { 0xaa, 0x55, 0xaa, 0x55, 0xaa, 0x55, 0xaa, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55 };

    for (int i = 0; i < 16; i++) {
        if (i<8){
            colorPattern[i] = *(uint8_t *)&ptr[i];
        } else {
            colorPattern[i] = 0x55;
        }

    }

    pd->graphics->clear(kColorWhite);

    //pd->graphics->fillRect(0,0,LCD_COLUMNS,LCD_ROWS,(LCDColor)*color);
    unsigned int width = LCD_COLUMNS - 50;
    unsigned int height = width * 0.5;

    unsigned int originX = (float)(LCD_COLUMNS / 2) - (float)(width / 2);
    unsigned int originY = (float)(LCD_ROWS / 2) - (float)(height / 2);

    if (endAngle > 270) {
        color = kColorBlack;
    } else if (endAngle < 90) {
        color = kColorWhite;
    } else {
        color = (LCDColor)colorPattern;
    }

    pd->graphics->fillEllipse(originX,originY,width,height,0,endAngle,color);
    pd->graphics->drawEllipse(originX,originY,width,height,3,0,endAngle,kColorBlack);

	pd->graphics->setFont(font);
    const char* hello = "Hello Jossy!";
	pd->graphics->drawText(hello, strlen(hello), kASCIIEncoding, x, y);

	x += dx;
	y += dy;

	if ( x < 0 || x > LCD_COLUMNS - TEXT_WIDTH )
		dx = -dx;

	if ( y < 0 || y > LCD_ROWS - TEXT_HEIGHT )
		dy = -dy;

	pd->system->drawFPS(0,0);

    if (pressing) {
        pd->graphics->clear(kColorXOR);
    }

	return 1;
}
