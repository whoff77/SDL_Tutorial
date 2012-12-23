#include <string>
#include <sstream>
#include "SDL.h"
#include "SDL_image.h"
#include "SDL_ttf.h"
#include "SDL_mixer.h"
#include "Button.h"
#include "Timer.h"

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;
const int SCREEN_BPP = 32;

SDL_Surface *screen = NULL;
SDL_Surface *background = NULL;
SDL_Surface *image = NULL;
SDL_Surface *dots = NULL;
SDL_Surface *buttonSheet = NULL;

SDL_Surface *message = NULL;
SDL_Surface *dirMessage = NULL;
SDL_Surface *curMessage = NULL;
SDL_Surface *upMessage = NULL;
SDL_Surface *downMessage = NULL;
SDL_Surface *leftMessage = NULL;
SDL_Surface *rightMessage = NULL;

SDL_Surface *aPress = NULL;
SDL_Surface *sPress = NULL;
SDL_Surface *dPress = NULL;
SDL_Surface *fPress = NULL;
SDL_Surface *aRelease = NULL;
SDL_Surface *sRelease = NULL;
SDL_Surface *dRelease = NULL;
SDL_Surface *fRelease = NULL;

SDL_Surface *startStop = NULL;
SDL_Surface *seconds = NULL;

SDL_Surface *startStop2 = NULL;
SDL_Surface *seconds2 = NULL;
SDL_Surface *pauseMessage = NULL;

SDL_Event event;
SDL_Rect clip[ 4 ];
SDL_Color textColor = { 255, 255, 255 };
SDL_Color textColor2 = { 155, 155, 155 };
TTF_Font *font = NULL;

Mix_Music *music = NULL;
Mix_Chunk *scratch = NULL;
Mix_Chunk *high = NULL;
Mix_Chunk *med = NULL;
Mix_Chunk *low = NULL;

Button *myButton = NULL;
Timer *myTimer = NULL;

std::stringstream time, time2;
bool quit, runningTimer;
Uint32 startTimer;


SDL_Surface *load_image( std::string filename ) {

    SDL_Surface* loadedImage = NULL;
    SDL_Surface* optimizedImage = NULL;
    loadedImage = IMG_Load( filename.c_str() );

    if( loadedImage != NULL ) {
        optimizedImage = SDL_DisplayFormat( loadedImage );
        SDL_FreeSurface( loadedImage );
		if( optimizedImage != NULL ) {
			Uint32 colorkey = SDL_MapRGB( optimizedImage->format, 0x00, 0xFF, 0xFF );
			SDL_SetColorKey( optimizedImage, SDL_SRCCOLORKEY, colorkey );
		}
    }
    return optimizedImage;
}

void apply_surface( int x, int y, SDL_Surface* source, SDL_Surface* destination, SDL_Rect* rectangle = NULL ) {

    SDL_Rect offset;
    offset.x = x;
    offset.y = y;
    SDL_BlitSurface( source, rectangle, destination, &offset );
}

void apply_surfaces() {

	//Apply the surfaces to the screen
    apply_surface( 0, 0, background, screen, NULL);
    apply_surface( 320, 0, background, screen, NULL);
    apply_surface( 0, 240, background, screen, NULL);
    apply_surface( 320, 240, background, screen, NULL);

	//Apply the sprites to the screen
    apply_surface( 0, 0, dots, screen, &clip[ 0 ] );
    apply_surface( 540, 0, dots, screen, &clip[ 1 ] );
    apply_surface( 0, 380, dots, screen, &clip[ 2 ] );
    apply_surface( 540, 380, dots, screen, &clip[ 3 ] );

    apply_surface( 180, 80, image, screen, NULL);
	apply_surface( 20, 300, message, screen );
	apply_surface( ( SCREEN_WIDTH - curMessage->w ) / 2, ( SCREEN_HEIGHT - curMessage->h ) / 2, curMessage, screen );
	apply_surface( ( SCREEN_WIDTH - seconds->w ) / 2, 0, seconds, screen );
	apply_surface( ( SCREEN_WIDTH - startStop->w ) / 2, 30, startStop, screen );
	apply_surface( ( SCREEN_WIDTH - seconds2->w ) / 2, 60, seconds2, screen );
	apply_surface( ( SCREEN_WIDTH - startStop2->w ) / 2, 90, startStop2, screen );
	apply_surface( ( SCREEN_WIDTH - pauseMessage->w ) / 2, 120, pauseMessage, screen );

	apply_surface(myButton->getBoxX(), myButton->getBoxY(), buttonSheet, screen, myButton->getRect());

	Uint8 *keystates = SDL_GetKeyState( NULL );
	if( keystates[ SDLK_a ] ) apply_surface( ( SCREEN_WIDTH - aPress->w ) / 2 - 60, SCREEN_HEIGHT / 2 + 20, aPress, screen );
	else apply_surface( ( SCREEN_WIDTH - aRelease->w ) / 2 - 60, SCREEN_HEIGHT / 2 + 20, aRelease, screen );
	if( keystates[ SDLK_s ] ) apply_surface( ( SCREEN_WIDTH - sPress->w ) / 2 - 20, SCREEN_HEIGHT / 2 + 20, sPress, screen );
	else apply_surface( ( SCREEN_WIDTH - sRelease->w ) / 2 - 20, SCREEN_HEIGHT / 2 + 20, sRelease, screen );
	if( keystates[ SDLK_d ] ) apply_surface( ( SCREEN_WIDTH - dPress->w ) / 2 + 20, SCREEN_HEIGHT / 2 + 20, dPress, screen );
	else apply_surface( ( SCREEN_WIDTH - dRelease->w ) / 2 + 20, SCREEN_HEIGHT / 2 + 20, dRelease, screen );
	if( keystates[ SDLK_f ] ) apply_surface( ( SCREEN_WIDTH - fPress->w ) / 2 + 60, SCREEN_HEIGHT / 2 + 20, fPress, screen );
	else apply_surface( ( SCREEN_WIDTH - fRelease->w ) / 2 + 60, SCREEN_HEIGHT / 2 + 20, fRelease, screen );
}

bool init() {

	if( SDL_Init( SDL_INIT_EVERYTHING ) == -1 ) return false;
	screen = SDL_SetVideoMode( SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP, SDL_SWSURFACE );
	if( screen == NULL ) return false;
	if( TTF_Init() == -1 ) return false;
	if( Mix_OpenAudio( 22050, MIX_DEFAULT_FORMAT, 2, 4096 ) == -1 ) return false; 
	SDL_WM_SetCaption( "SDL_tutorial", NULL );
	return true;
}

bool load_files() {

	background = load_image( "background.bmp" );
    image = load_image( "look.png" );
	dots = load_image( "dots.png" );
	buttonSheet = load_image( "button.png" );
	font = TTF_OpenFont( "lazy.ttf", 28 );

	music = Mix_LoadMUS( "beat.wav" );
	scratch = Mix_LoadWAV( "scratch.wav" );
    high = Mix_LoadWAV( "high.wav" );
    med = Mix_LoadWAV( "medium.wav" );
    low = Mix_LoadWAV( "low.wav" );

	if( background == NULL ||
		image == NULL ||
		dots == NULL ||
		buttonSheet == NULL ||
		font == NULL ||
		music == NULL ||
		scratch == NULL ||
		high == NULL ||
		med == NULL ||
		low == NULL )
		return false;
	return true;
}

void clean_up() {

	delete myButton; myButton = NULL;
	delete myTimer; myTimer = NULL;
	curMessage = NULL;

    SDL_FreeSurface( image );
    SDL_FreeSurface( background );
	SDL_FreeSurface( dots );
	SDL_FreeSurface( buttonSheet );
	SDL_FreeSurface( message );
	SDL_FreeSurface( dirMessage);
	SDL_FreeSurface( upMessage );
    SDL_FreeSurface( downMessage );
    SDL_FreeSurface( leftMessage );
    SDL_FreeSurface( rightMessage );
    SDL_FreeSurface( startStop );
	SDL_FreeSurface( seconds );
	SDL_FreeSurface( startStop2 );
	SDL_FreeSurface( seconds2 );
    SDL_FreeSurface( pauseMessage );

	SDL_FreeSurface( aPress );
	SDL_FreeSurface( sPress );
	SDL_FreeSurface( dPress );
	SDL_FreeSurface( fPress );
	SDL_FreeSurface( aRelease );
	SDL_FreeSurface( sRelease );
	SDL_FreeSurface( dRelease );
	SDL_FreeSurface( fRelease );

    Mix_FreeChunk( scratch );
    Mix_FreeChunk( high );
    Mix_FreeChunk( med );
    Mix_FreeChunk( low );
    Mix_FreeMusic( music );

	TTF_CloseFont( font );
	TTF_Quit();
	SDL_Quit();
}

int main( int argc, char* args[] ) {

	if( init() == false ) return 1;
	if( load_files() == false ) return 1;

	quit = false;
	runningTimer = true;
	startTimer = 0;
	time << "Timer: " << startTimer;
	startTimer = SDL_GetTicks();
	
	myTimer = new Timer();
	time2 << "Timer2: " << myTimer->get_ticks();
	myTimer->start();

	myButton = new Button( SCREEN_WIDTH/2-62, 360, 125, 100 );

	message = TTF_RenderText_Solid( font, "Sounds(1,2,3,4), Play/Pause(9), Stop(0)", textColor );
	dirMessage = TTF_RenderText_Solid( font, "Press a direction arrow.", textColor );
	curMessage = dirMessage;
	if( message == NULL ) return 1; // do this for all message rendering
	upMessage = TTF_RenderText_Solid( font, "Up was pressed.", textColor );
    downMessage = TTF_RenderText_Solid( font, "Down was pressed.", textColor );
    leftMessage = TTF_RenderText_Solid( font, "Left was pressed", textColor );
    rightMessage = TTF_RenderText_Solid( font, "Right was pressed", textColor );

	aPress = TTF_RenderText_Solid( font, "A", textColor );
    sPress = TTF_RenderText_Solid( font, "S", textColor );
    dPress = TTF_RenderText_Solid( font, "D", textColor );
    fPress = TTF_RenderText_Solid( font, "F", textColor );
	aRelease = TTF_RenderText_Solid( font, "A", textColor2 );
	sRelease = TTF_RenderText_Solid( font, "S", textColor2 );
	dRelease = TTF_RenderText_Solid( font, "D", textColor2 );
	fRelease = TTF_RenderText_Solid( font, "F", textColor2 );

	startStop = TTF_RenderText_Solid( font, "Press T to start or stop the timer", textColor );
	seconds = TTF_RenderText_Solid( font, time.str().c_str(), textColor );
	startStop2 = TTF_RenderText_Solid( font, "Press O to start or stop the 2nd timer", textColor );
	seconds2 = TTF_RenderText_Solid( font, time2.str().c_str(), textColor );
    pauseMessage = TTF_RenderText_Solid( font, "Press P to pause or unpause the 2nd timer", textColor );

    clip[ 0 ].x = 0; clip[ 0 ].y = 0; clip[ 0 ].w = 100; clip[ 0 ].h = 100;
    clip[ 1 ].x = 100; clip[ 1 ].y = 0; clip[ 1 ].w = 100; clip[ 1 ].h = 100;
    clip[ 2 ].x = 0; clip[ 2 ].y = 100; clip[ 2 ].w = 100; clip[ 2 ].h = 100;
    clip[ 3 ].x = 100; clip[ 3 ].y = 100; clip[ 3 ].w = 100; clip[ 3 ].h = 100;

	//Fill the screen white
    //SDL_FillRect( screen, &screen->clip_rect, SDL_MapRGB( screen->format, 0xFF, 0xFF, 0xFF ) );

	apply_surfaces();

	//While the user hasn't quit
	while( quit == false ) {
		//While there's an event to handle
		while( SDL_PollEvent( &event ) ) {
			//If a key was pressed
			if( event.type == SDL_KEYDOWN ) {
				//Set the proper message surface
				switch( event.key.keysym.sym ) {
					
                    case SDLK_UP: curMessage = upMessage; break;
                    case SDLK_DOWN: curMessage = downMessage; break;
                    case SDLK_LEFT: curMessage = leftMessage; break;
                    case SDLK_RIGHT: curMessage = rightMessage; break;

					case SDLK_t:
						if( runningTimer == true ) {
							runningTimer = false;
							startTimer = 0;
						} else {
							runningTimer = true;
							startTimer = SDL_GetTicks();
						}
						break;
					case SDLK_o:
						if ( myTimer->is_started() == true ) { myTimer->stop(); }
						else { myTimer->start(); }
						break;
					case SDLK_p:
						if( myTimer->is_paused() == true ) { myTimer->unpause(); }
						else { myTimer->pause(); }
						break;

					case SDLK_1: if( Mix_PlayChannel( -1, scratch, 0 ) == -1 ) return 1; break;
					case SDLK_2: if( Mix_PlayChannel( -1, high, 0 ) == -1 ) return 1; break;
					case SDLK_3: if( Mix_PlayChannel( -1, med, 0 ) == -1 ) return 1; break;
					case SDLK_4: if( Mix_PlayChannel( -1, low, 0 ) == -1 ) return 1; break;

					case SDLK_9:
						if( Mix_PlayingMusic() == 0 ) {
							if( Mix_PlayMusic( music, -1 ) == -1 ) return 1;
						} else {
							if( Mix_PausedMusic() == 1 ) {
								Mix_ResumeMusic();
							} else {
								Mix_PauseMusic();
							}
						}
						break;
					case SDLK_0: Mix_HaltMusic(); break;

					default: break;
                }
            }

            myButton->handle_events(event);

			if( event.type == SDL_QUIT ) quit = true;
		}

		if( runningTimer == true ) {
			time.clear();
			time.str("");
            time << "Timer: " << SDL_GetTicks() - startTimer;
			SDL_FreeSurface( seconds );
            seconds = TTF_RenderText_Solid( font, time.str().c_str(), textColor );
        }

		time2.clear();
		time2.str("");
        time2 << "Timer2: " << myTimer->get_ticks();
		SDL_FreeSurface( seconds2 );
		seconds2 = TTF_RenderText_Solid( font, time2.str().c_str(), textColor );

		apply_surfaces();

		if( SDL_Flip( screen ) == -1 ) return 1;
	}

	clean_up();
	return 0;
}
