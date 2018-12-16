#include <iostream>
using namespace std;
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_ttf.h>
#include <vector>
#include <string.h>

const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 720;
const int SCREEN_BPP = 32;
const float AUMENTO_PUNTUACION = 0.5;
float PUNTUACION_PARA_PASAR_DE_NIVEL = 2500;

SDL_Event event;
SDL_Surface *recjugador = NULL;
SDL_Surface *screen = NULL;
SDL_Surface *m0 = NULL;
SDL_Surface *m1 = NULL;
SDL_Surface *m2 = NULL;
SDL_Surface *m3 = NULL;
vector <SDL_Surface *> mundos;
SDL_Surface *destello = NULL;
SDL_Surface *tablero_img = NULL;
SDL_Surface *puntos = NULL;
SDL_Surface *status = NULL;
SDL_Surface *record = NULL;
SDL_Surface *ganador_con_record = NULL;
SDL_Surface *ganador = NULL;
vector <vector<SDL_Surface *>> gemas;
SDL_Surface *pos_blanca = NULL;
SDL_Surface *message = NULL;
SDL_Surface *nombre_jugador = NULL;
TTF_Font *font = NULL;
TTF_Font *font_big = NULL;
TTF_Font *status_font = NULL;
SDL_Color white = {255, 255, 255};
SDL_Color black = {0, 0, 0};

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
	record = load_image( "Frecord.bmp" );
	nombre_jugador = load_image( "menu/nombre_jugador.png" );
	tablero_img = load_image( "mundos/tablero.png" );
	status = load_image( "mundos/status.png" );
	destello = load_image( "gemas/destello.png" );
	pos_blanca = load_image("gemas/blanco.png");
	ganador = load_image("ganador.png");
	ganador_con_record = load_image("ganador_record.png");
	font = TTF_OpenFont( "fuente.ttf", 20);
	status_font = TTF_OpenFont( "status2.ttf", 30);
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
	for(int i=1; i<=5; i++){
		gema_nombre = "mundos/world-"+to_string(i)+".jpg";
		aux = load_image( gema_nombre );
		if(aux==NULL)
			SDL_Quit();
		mundos.push_back(aux);
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

void imprimir_rectangulo (SDL_Surface *screen, SDL_Color color, int prof) {
	float calculo = (int)(((float)391 / PUNTUACION_PARA_PASAR_DE_NIVEL)*prof);	
	SDL_Rect rect = {1111, 229+(391-calculo), 40, calculo};
	Uint32 a = SDL_MapRGB(screen->format, color.r, color.g, color.b);
	SDL_FillRect(screen, &rect, a);
	SDL_UpdateRect(screen, rect.x, rect.y, rect.w, rect.h);
}

void clean_up() {
//	SDL_FreeSurface( background );
//	SDL_FreeSurface( message );
	TTF_CloseFont( font );
	TTF_Quit();
	SDL_Quit();
}

