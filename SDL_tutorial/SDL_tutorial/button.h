#pragma once

#include "SDL.h"

class Button {
public:

    //Initialize the variables
    Button( int, int, int, int );
	~Button(void);

    //Handles events and set the button's sprite region
    void handle_events(SDL_Event);

    int getBoxX();
	int getBoxY();
	SDL_Rect *getRect();

private:

	void set_clips();

	//The attributes of the button
    SDL_Rect box;
    //The part of the button sprite sheet that will be shown
    SDL_Rect *buttonClip;
	SDL_Rect clips[ 4 ];

	//The button states in the sprite sheet
	static const int CLIP_MOUSEOVER = 0;
	static const int CLIP_MOUSEOUT = 1;
	static const int CLIP_MOUSEDOWN = 2;
	static const int CLIP_MOUSEUP = 3;
};

