#include <iostream>
using namespace std;
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_ttf.h>
#include <vector>

const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 720;
const int SCREEN_BPP = 32;
SDL_Surface *screen = NULL;
SDL_Surface *m0 = NULL;
SDL_Surface *m1 = NULL;
SDL_Surface *m2 = NULL;
SDL_Surface *m3 = NULL;
SDL_Surface *mundo = NULL;
SDL_Surface *destello = NULL;
SDL_Surface *tablero_img = NULL;
vector <vector<SDL_Surface *>> gemas;
SDL_Surface *message = NULL;
TTF_Font *font = NULL;
TTF_Font *font_big = NULL;
SDL_Color white = {255, 255, 255};

SDL_Surface *load_image( std::string filename ) {
	SDL_Surface* loadedImage = NULL;
	SDL_Surface* optimizedImage = NULL;
	loadedImage = IMG_Load( filename.c_str() );
	if( loadedImage != NULL ) {
		optimizedImage = SDL_DisplayFormatAlpha( loadedImage );
		SDL_FreeSurface( loadedImage );
		if( optimizedImage != NULL ) {
			SDL_SetColorKey( optimizedImage, SDL_SRCCOLORKEY, SDL_MapRGB( optimizedImage->format, 0, 0xFF, 0xFF ) );
		}
	}
	return optimizedImage;
}

bool load_files() {
	SDL_Surface *aux = NULL;
	vector <SDL_Surface *> vec_aux;
	string gema_nombre;
	m0 = load_image( "menu/menu_0.bmp" );
	m1 = load_image( "menu/menu_1.bmp" );
	m2 = load_image( "menu/menu_2.bmp" );
	m3 = load_image( "menu/menu_3.bmp" );
	mundo = load_image( "mundos/world.jpg" );
	tablero_img = load_image( "mundos/tablero.png" );
	destello = load_image( "gemas/destello.png" );
	font = TTF_OpenFont( "fuente.ttf", 20);
	font_big = TTF_OpenFont( "fuente.ttf", 48);
	for(int i=1; i<8; i++){
		for(int j=1; j<5; j++){
			gema_nombre = "gemas/gema_"+to_string(i)+"_t"+to_string(j)+".png";
			aux = load_image( gema_nombre );
			if(aux==NULL)
				SDL_Quit();
			vec_aux.push_back(aux);
		}
		gemas.push_back(vec_aux);
		vec_aux.clear();
	}
	return true;
}

void apply_surface( int x, int y, SDL_Surface* source, SDL_Surface* destination, SDL_Rect* clip = NULL ) {
	SDL_Rect offset;
	offset.x = x;
	offset.y = y;
	SDL_BlitSurface( source, clip, destination, &offset );
}

bool init() {
	if( SDL_Init( SDL_INIT_EVERYTHING ) == -1 ) return false;
	screen = SDL_SetVideoMode( SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP, SDL_SWSURFACE );
	if( screen == NULL ) return false;
	if( TTF_Init() == -1 ) return false;
	SDL_WM_SetCaption( "Proyecto ED", NULL );
	return true;
}

void clean_up() {
//	SDL_FreeSurface( background );
//	SDL_FreeSurface( message );
	TTF_CloseFont( font );
	TTF_Quit();
	SDL_Quit();
}

