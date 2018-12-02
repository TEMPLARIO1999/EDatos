#include "graficos.h"
#include <time.h>

class Gema{
	public:
		int color;   //morado amarillo naranja verde azul rojo blanco 
		int tipo;	 //1=Normal 2=Estelar 3=Explosiva 4=Cubo
		Gema(){
			color=(rand()%7)+1;
			tipo=1;
		}
};

class Jugador{
	public:
		char *alias;
		int nivel;
		int puntuacion;
		Jugador(char *alias){
			this->alias=alias;
			nivel=1;
			puntuacion=0;
		}
};

class Tablero{
	private:
		Jugador *Player;
		Gema tab[8][8];
		int vx,vy;
	public:
		Tablero(char *alias){
			vx=0; vy=0;
			Player = new Jugador(alias);
			for(int i=0;i<8;i++){
				for(int j=0;j<8;j++){
					tab[i][j]=Gema();
				}
			}
		}
		void imprimir_gemas(){
			int posx = 60, posy = 120;
			for(int i=0; i<8; i++, posy+=60){
				for(int j=0; j<8; j++, posx+=60){
					apply_surface(posx, posy, gemas[tab[i][j].color-1][tab[i][j].tipo-1], screen);
				}
				posx=60;
			}
		}
		bool B_trio(int dir,int y,int x){    //Direccion 0=Horizontal 1=vertical busca trios
			if(dir==0){
				if(y-2>=0 && x>=0 && x<=7){
					if(tab[y][x].color!=0 && tab[y][x].color==tab[y-1][x].color && tab[y-1][x].color==tab[y-2][x].color){
						vx=x; vy=y;
						tab[y-1][x].color=0; tab[y-2][x].color=0;
						return true;
					}else return false;
				}else return false;
			}else if(dir==1){
				if(y+2<=7 && x<=7 && x>=0){
					if(tab[y][x].color!=0 && tab[y][x].color==tab[y+1][x].color && tab[y+1][x].color==tab[y+2][x].color){
						vx=x; vy=y;
						tab[y+1][x].color=0; tab[y+2][x].color=0;
						return true;
					}else return false;
				}else return false;
			}
		}
		void Check_Est(){					//Comprueba si hay combinaciones estelares
			for(int i=0;i<8;i++){
				for(int j=0;j<8;j++){
					if(tab[i][j].color!=0 && tab[i][j].color==tab[i][j+1].color && tab[i][j+1].color==tab[i][j+2].color){
						if(j+2<=7) 
						if(B_trio(0,i,j) || B_trio(1,i,j) || B_trio(0,i,j+1) || B_trio(1,i,j+1) || B_trio(0,i,j+2) || B_trio(1,i,j+2)){
							int aux=tab[vy][vx].color;
							tab[i][j+1].color=0; tab[i][j+2].color=0; tab[i][j].color=0;
							tab[vy][vx].color=aux;
							tab[vy][vx].tipo=2;
						}
					}
					else if(tab[i][j].color!=0 && tab[i][j].color==tab[i+1][j].color && tab[i+1][j].color==tab[i+2][j].color){
						if(i+2<=7){
							if(tab[i+1][j].color!=0 && tab[i+1][j].color==tab[i+1][j+1].color && tab[i+1][j+1].color==tab[i+1][j+2].color){
								if(j+2<=7){
									tab[i+1][j].tipo=2;
									tab[i][j].color=0; tab[i+2][j].color=0;
									tab[i+1][j+1].color=0; tab[i+1][j+2].color=0;
								}
							}else if(tab[i+1][j].color!=0 && tab[i+1][j].color==tab[i+1][j-1].color && tab[i+1][j-1].color==tab[i+1][j-2].color){
								if(j-2>=0){
									tab[i+1][j].tipo=2;
									tab[i][j].color=0; tab[i+2][j].color=0;
									tab[i+1][j-1].color=0; tab[i+1][j-2].color=0;
								}
							}
						}
					}
				}
			}
		}
		// Se mover�n las gemas de acuerdo al movimiento del usuario.
		// Por ello se necesita el sentido y la casilla que se movi�.
		// Siempre se llama en casos dentro del tablero, por ello posx y posy no necesitan comprobarse.
		void movimiento_gemas(int sentido, int posx, int posy){
			Gema aux = tab[posy][posx];
			// Arriba
			if(sentido == 1 and posy > 0){
				tab[posy][posx] = tab[posy-1][posx];
				tab[posy-1][posx] = aux;
			}
			if(sentido == 2 and posx < 7){
				tab[posy][posx] = tab[posy][posx+1];
				tab[posy][posx+1] = aux;
			}
			if(sentido == 3 and posy < 7){
				tab[posy][posx] = tab[posy+1][posx];
				tab[posy+1][posx] = aux;
			}
			if(sentido == 4 and posx > 0 ){
				tab[posy][posx] = tab[posy][posx-1];
				tab[posy][posx-1] = aux;
			}
		}
		bool Comb(int cont,int y,int x,int dir){			//Busca casillas adyacentes 
			if(dir){
				if(x+1<=7 && tab[y][x].color!=0 && tab[y][x].color==tab[y][x+1].color) Comb(cont+1,y,x+1,1);
				else{
					if(cont>=3) {
						vx=cont;
						tab[y][x].color=0;
						return true;
					}else return false;
				}
				if(vx>=3) {
					tab[y][x].color=0;
					return true;
				}return false;
			}else{
				if(y+1<=7 && tab[y][x].color!=0 && tab[y][x].color==tab[y+1][x].color) Comb(cont+1,y+1,x,0);
				else{
					if(cont>=3) {
						vx=cont;
						tab[y][x].color=0;
						return true;
					}else return false;
				}
				if(vx>=3){
					tab[y][x].color=0;
					return true;
				}return false;
			}
		
		}
		void Chek_Comb(){   //Comprueba si hay combinaciones de 3 4 o 5 gemas :3
			for(int i=0;i<8;i++){
				for(int j=0;j<8;j++){
					vx=0; vy=tab[i][j].color;
					if(Comb(1,i,j,1)){
						if(vx==3) tab[i][j].color=0;
						else if(vx==4) tab[i][j].tipo=3;
						else if(vx>=5) tab[i][j].tipo=4;
						if(vx>3) tab[i][j].color=vy;
					}else if(Comb(1,i,j,0)){
						if(vx==3) tab[i][j].color=0;
						else if(vx==4) tab[i][j].tipo=3;
						else if(vx>=5) tab[i][j].tipo=4; 
						if(vx>3) tab[i][j].color=vy;
					}
				}
			}
		}
		void powers(Gema gem,int y,int x){
			if(gem.tipo==1) return;
			else if(gem.tipo==2){
				tab[y][x].color=0;
				for(int i=0;i<8;i++) {
					if(tab[i][x].tipo!=gem.tipo && tab[i][x].color!=gem.color){
						tab[i][x].color=0;
						powers(tab[i][x],i,x);
					}
				}
				for(int j=0;j<8;j++){
					if(tab[y][j].tipo!=gem.tipo && tab[y][j].color!=gem.color){
						tab[y][j].color=0;
						powers(tab[y][j],y,j);
					}
				}
			}else if(gem.tipo==3){
				tab[y][x].color=0;
				powers(tab[y-1][x-1],y-1,x-1); tab[y-1][x-1].color=0; 
				powers(tab[y-1][x],y-1,x); tab[y-1][x].color=0;
				powers(tab[y-1][x+1],y-1,x+1); tab[y-1][x+1].color=0;
				powers(tab[y][x-1],y,x-1); tab[y][x-1].color=0;
				powers(tab[y][x+1],y,x+1); tab[y][x+1].color=0;
				powers(tab[y+1][x-1],y+1,x-1); tab[y+1][x-1].color=0;
				powers(tab[y+1][x],y+1,x); tab[y+1][x].color=0;
				powers(tab[y+1][x+1],y+1,x+1); tab[y+1][x+1].color=0;
			}else if(gem.tipo==4){
				vx=tab[y][x].color;tab[y][x].color=0;
				for (int i=0;i<8;i++){
					for(int j=0;j<8;j++){
						if(tab[i][j].color==vx){
							powers(tab[i][j],i,j);
							tab[i][j].color=0;
						}
					}
				}
			}
		}
		void Re_Fill(){
			for(int i=7;i>=0;i--){
				for(int j=0;j<8;j++){
					if(tab[i][j].color==0){
						for(int k=i;k>=0;k--){
							if(tab[k][j].color!=0){
								tab[i][j]=tab[k][j];
								tab[k][j].color=0;
								break;
							}
						}
					}
				}
			}
			for(int i=7;i>=0;i--) for(int j=0;j<8;j++) if(tab[i][j].color==0) tab[i][j]=Gema();
		}
		void show(){
			for(int i=0;i<8;i++){
				for(int j=0;j<8;j++){
					cout<<tab[i][j].color;
				}
				cout<<endl;
			}
		}
};

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

class EventManager{
	private:
		// x, y coordenadas normales en px.
		// x_ant, y_ant coordenadas donde iniciar� el arrastre del mouse.
		int x, y, x_ant, y_ant;
	public:
		// x_vec, y_vec representaci�n de x y para la gema de la matriz seleccionada.
		// publicas para evitar la fatiga siempre que se da click. xD
		int x_vec, y_vec;
		// Se hace en forma de clase para no tener que declarar muchos eventos.
		SDL_Event event;
		EventManager(){}
		~EventManager(){}
		// Identificamos donde est� el rat�n para aplicar un destello de fondo
		// y saber que gema estamos resaltando.
		void mouse_motion_ev(){
			x = (event.motion.x-60)/60;
			y = (event.motion.y-120)/60;
			if (x <= 7 and y <= 7 and x >= 0 and y >= 0) apply_surface((x*60)+60, (y*60)+120, destello, screen);
		}
		// Identificamos la casilla del tablero, y sus coordenadas x y.
		void mouse_click_ev(){
			x_vec = (event.motion.x-60)/60;
			y_vec = (event.motion.y-120)/60;
			x_ant = event.motion.x;
			y_ant = event.motion.y;
		}
		// Cuando se libera el mouse, calculamos la trayectoria y si fue
		// un movimiento v�lido.
		int mouse_upclick_ev(){
			int mov = 0;
			// Si el movimiento es en y, el absoluto de x tiene que ser menor de 40.
			// Lo mismo para x.
			int x_cur = abs(event.motion.x - x_ant);
			int y_cur = abs(event.motion.y - y_ant);
			if (x_vec <= 7 and y_vec <= 7 and x_vec >= 0 and y_vec >= 0){
				// Movimiento arriba (1) o abajo (3).
				if(x_cur <= 25){
					mov = (event.motion.y - y_ant) <= -50 ? 1 : (event.motion.y - y_ant) >= 50 ? 3: 0;
				}
				// Movimiento izquierda (4) o derecha (2).
				else if (y_cur <= 25){
					mov = (event.motion.x - x_ant) >= 50 ? 2 : (event.motion.x - x_ant) <= -50 ? 4 : 0;
				}
			}
			return mov;
		}
		
};

void juego(){
	// movimiento, cuando se da click identificamos hacia donde se mueve
	// 1 arriba, 2 derecha, 3 abajo, 4 izquierda. Sentido del reloj.
	int mov;
	// Objeto que nos permitir� el manejo de eventos a lo largo del juego.
	EventManager manager;
	// Inicializamos el tablero.
	Tablero tablero("FuckedUp");
	// Eliminamos cualquier combinaci�n mayor a 3 o especial antes de iniciar.
	tablero.Check_Est();
	tablero.Chek_Comb();
	tablero.Re_Fill();
	apply_surface(0, 0, mundo, screen);
	while(1){
		// Siempre se va a sobre escribir el tablero a la espera de nuevas instrucciones.
		if( SDL_PollEvent( &manager.event )){
			apply_surface(60, 120, tablero_img, screen);
			// Si el mouse se mueve, mouse_motion_ev se encarga del movimiento.
			if( manager.event.type == SDL_MOUSEMOTION ) {
				manager.mouse_motion_ev();
			}
			// Si el mouse da click, mouse_click_ev se encarga del evento.
			if( manager.event.type == SDL_MOUSEBUTTONDOWN ) {
				if( manager.event.button.button == SDL_BUTTON_LEFT ) {
					manager.mouse_click_ev(); // Se identifica el movimiento de arrastre.
				}
			}
			if( manager.event.type == SDL_MOUSEBUTTONUP ) {
				mov = manager.mouse_upclick_ev(); // Se calcula la direcci�n del arrastre para movimiento de gemas.
				if(mov){
					// Casos para el movimiento, 1 arriba, 2 dcha, 3 abajo, 4 izqda
					tablero.movimiento_gemas(mov, manager.x_vec, manager.y_vec);
				}
			}
			// Se imprimen las gemas, para cualquier movimiento que haya ocurrido.
			tablero.imprimir_gemas();
			tablero.Check_Est();
			tablero.Chek_Comb();
			tablero.Re_Fill();
		}
		SDL_Flip(screen);
	}
}

int main(int argc, char **argv){
	srand(time(NULL));
	init();
	load_files();
	menu m;
	while(1){
		switch(m.disp_menu()){
			case 0:
				juego();
				break;
		}
	}
}
