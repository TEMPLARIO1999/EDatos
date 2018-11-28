#include "graficos.h"

class menu {
	private:
		SDL_Event event;
	public:
		menu() {}
		~menu() {}
		int disp_menu() {
			bool quit = false;
			int acc = 3;
			apply_surface(0, 0, m0, screen);
			SDL_Flip(screen);
			while( acc!=0 and acc!=1 and acc!=2 ) {
				if( SDL_PollEvent( &event ) ) {
					acc = handle_events();
				}
			}
			return acc;
		}
		int handle_events() {
			int x = 0, y = 0, acc=3;
			if( event.type == SDL_MOUSEMOTION ) {
				x = event.motion.x;
				y = event.motion.y;
				if(y>=300 and y<=550 and x>=420 and x<=820)
					apply_surface(0, 0, m1, screen);
				else if(y>=330 and y<=400 and x>=0 and x<=320)
					apply_surface(0, 0, m2, screen);
				else if(y>=330 and y<=400 and x>=960 and x<=1280)
					apply_surface(0, 0, m3, screen);
				else
					apply_surface(0, 0, m0, screen);
				SDL_Flip(screen);
			}
			if( event.type == SDL_MOUSEBUTTONDOWN ) {
				if( event.button.button == SDL_BUTTON_LEFT ) {
					x = event.button.x;
					y = event.button.y;
					if(y>=300 and y<=550 and x>=420 and x<=820)
						acc = 0;
					else if(y>=330 and y<=400 and x>=0 and x<=320)
						acc = 1;
					else if(y>=330 and y<=400 and x>=960 and x<=1280)
						acc = 2;
				}
			}
			return acc;
		}
};

int main(int argc, char **argv){
	init();
	load_files();
	menu m;
	m.disp_menu();
}
