#include "button.h"


Button::Button(int x, int y, int w, int h) {
	set_clips();
	//Set the button's attributes
    box.x = x;
    box.y = y;
    box.w = w;
    box.h = h;

    //Set the default sprite
    buttonClip = &clips[ CLIP_MOUSEOUT ];
}

Button::~Button(void) {}

void Button::handle_events(SDL_Event event) {
    //The mouse offsets
    int x = 0, y = 0;

    //If the mouse moved
    if( event.type == SDL_MOUSEMOTION ) {
        x = event.motion.x;
        y = event.motion.y;

        //If the mouse is over the button
        if ( ( x < box.x ) || ( x > box.x + box.w ) ||
			 ( y < box.y ) || ( y > box.y + box.h ) )
			 buttonClip = &clips[ CLIP_MOUSEOUT ];
		else if ( ( buttonClip != &clips[ CLIP_MOUSEUP ] ) &&
			      ( buttonClip != &clips[ CLIP_MOUSEDOWN ] ) )
				  buttonClip = &clips[ CLIP_MOUSEOVER ];
    }
    //If a mouse button was pressed
    if ( event.type == SDL_MOUSEBUTTONDOWN ) {
        //If the left mouse button was pressed
        if( event.button.button == SDL_BUTTON_LEFT ) {
            x = event.button.x;
            y = event.button.y;

            //If the mouse is over the button
            if( ( x > box.x ) && ( x < box.x + box.w ) && ( y > box.y ) && ( y < box.y + box.h ) )
				buttonClip = &clips[ CLIP_MOUSEDOWN ];
        }
    }
    //If a mouse button was released
    if( event.type == SDL_MOUSEBUTTONUP ) {
        //If the left mouse button was released
        if( event.button.button == SDL_BUTTON_LEFT ) {
            //Get the mouse offsets
            x = event.button.x;
            y = event.button.y;

            //If the mouse is over the button
            if( ( x > box.x ) && ( x < box.x + box.w ) && ( y > box.y ) && ( y < box.y + box.h ) )
				buttonClip = &clips[ CLIP_MOUSEUP ];
        }
    }
}

int Button::getBoxX() { return box.x; }
int Button::getBoxY() { return box.y; }
SDL_Rect* Button::getRect() { return buttonClip; }

void Button::set_clips()
{
    //Clip the sprite sheet
    clips[ CLIP_MOUSEOVER ].x = 0;
    clips[ CLIP_MOUSEOVER ].y = 0;
    clips[ CLIP_MOUSEOVER ].w = 125;
    clips[ CLIP_MOUSEOVER ].h = 100;

    clips[ CLIP_MOUSEOUT ].x = 125;
    clips[ CLIP_MOUSEOUT ].y = 0;
    clips[ CLIP_MOUSEOUT ].w = 125;
    clips[ CLIP_MOUSEOUT ].h = 100;

    clips[ CLIP_MOUSEDOWN ].x = 0;
    clips[ CLIP_MOUSEDOWN ].y = 100;
    clips[ CLIP_MOUSEDOWN ].w = 125;
    clips[ CLIP_MOUSEDOWN ].h = 100;

    clips[ CLIP_MOUSEUP ].x = 125;
    clips[ CLIP_MOUSEUP ].y = 100;
    clips[ CLIP_MOUSEUP ].w = 125;
    clips[ CLIP_MOUSEUP ].h = 100;
}