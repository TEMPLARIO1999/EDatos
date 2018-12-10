#include "graficos.h"
#include <time.h>

struct Trecord{			//Estructura utilizada para guardar los records
	int puntuacion;
	int nivel;
	string alias;
};

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
		string alias;
		int nivel;
		int puntuacion;
		Jugador(string alias){
			this->alias=alias;
			nivel=1;
			puntuacion=0;
		}
		int GetPuntuacion(){
			return puntuacion;
		}
};

class Tablero{
	private:
		Gema tab[8][8];
		int vx,vy;
	protected:
		Jugador *Player;
		Trecord rec[11];
	public:
		Tablero(string alias){
			vx=0; vy=0;
			Player = new Jugador(alias);
			for(int i=0;i<8;i++){
				for(int j=0;j<8;j++){
					tab[i][j]=Gema();
				}
			}
			while(Check_Est() || Chek_Comb()){
				Re_Fill();
				for(int i=0;i<8;i++) for(int j=0;j<8;j++) if(tab[i][j].tipo!=1) tab[i][j]=Gema();
			}
			Player->puntuacion=0;
		}
		int get_nv () {
			return this->Player->nivel;
		}
		void imprimir_gemas(){
			int posx = 60, posy = 120;
			string auxiliar;
			for(int i=0; i<8; i++, posy+=60){
				for(int j=0; j<8; j++, posx+=60){
					apply_surface(posx, posy, gemas[tab[i][j].color-1][tab[i][j].tipo-1], screen);
				}
				posx=60;
			}
			apply_surface(0, 0, status, screen);
			message = TTF_RenderText_Solid( status_font, this->Player->alias.c_str(), black );
			apply_surface(930, 150, message, screen);
			auxiliar = to_string(this->Player->nivel);
			message = TTF_RenderText_Solid(status_font, auxiliar.c_str(), black);
			apply_surface(945, 390, message, screen);
			auxiliar = to_string(this->Player->puntuacion) + " / " + to_string((int)PUNTUACION_PARA_PASAR_DE_NIVEL);
			message = TTF_RenderText_Solid(status_font, auxiliar.c_str(), black);
			apply_surface(900, 565, message, screen);
			imprimir_rectangulo(screen, black, this->Player->puntuacion);
		}
		bool B_trio(int dir,int y,int x){    //Direccion 0=Horizontal 1=vertical busca trios
			if(dir==0){
				if(y-2>=0 && x>=0 && x<=7){
					if(tab[y][x].color!=0 && tab[y][x].color==tab[y-1][x].color && tab[y-1][x].color==tab[y-2][x].color){
						vx=x; vy=y;
						powers(tab[y-1][x],y-1,x); powers(tab[y-2][x],y-2,x);
						tab[y-1][x].color=0; tab[y-2][x].color=0;
						return true;
					}else return false;
				}else return false;
			}else if(dir==1){
				if(y+2<=7 && x<=7 && x>=0){
					if(tab[y][x].color!=0 && tab[y][x].color==tab[y+1][x].color && tab[y+1][x].color==tab[y+2][x].color){
						vx=x; vy=y;
						powers(tab[y+1][x],y+1,x); powers(tab[y+2][x],y+2,x);
						tab[y+1][x].color=0; tab[y+2][x].color=0;
						return true;
					}else return false;
				}else return false;
			}
		}
		bool Check_Est(){					//Comprueba si hay combinaciones estelares
			for(int i=0;i<8;i++){
				for(int j=0;j<8;j++){
					if(tab[i][j].color!=0 && tab[i][j].color==tab[i][j+1].color && tab[i][j+1].color==tab[i][j+2].color){
						if(j+2<=7) 
						if(B_trio(0,i,j) || B_trio(1,i,j) || B_trio(0,i,j+1) || B_trio(1,i,j+1) || B_trio(0,i,j+2) || B_trio(1,i,j+2)){
							int aux=tab[vy][vx].color;
							Player->puntuacion+=300;
							powers(tab[i][j+1],i,j+1); powers(tab[i][j+2],i,j+2); powers(tab[i][j],i,j);
							tab[i][j+1].color=0; tab[i][j+2].color=0; tab[i][j].color=0;
							tab[vy][vx].color=aux;
							tab[vy][vx].tipo=2;
							return true;
						}
					}
					else if(tab[i][j].color!=0 && tab[i][j].color==tab[i+1][j].color && tab[i+1][j].color==tab[i+2][j].color){
						if(i+2<=7){
							if(tab[i+1][j].color!=0 && tab[i+1][j].color==tab[i+1][j+1].color && tab[i+1][j+1].color==tab[i+1][j+2].color){
								if(j+2<=7){
									Player->puntuacion+=300;
									powers(tab[i][j],i,j); powers(tab[i+2][j],i+2,j); powers(tab[i+1][j+1],i+1,j+1); powers(tab[i+1][j+2],i+1,j+2);
									tab[i+1][j].tipo=2;
									tab[i][j].color=0; tab[i+2][j].color=0;
									tab[i+1][j+1].color=0; tab[i+1][j+2].color=0;
									return true;
								}
							}else if(tab[i+1][j].color!=0 && tab[i+1][j].color==tab[i+1][j-1].color && tab[i+1][j-1].color==tab[i+1][j-2].color){
								if(j-2>=0){
									Player->puntuacion+=300;
									powers(tab[i][j],i,j); powers(tab[i+2][j],i+2,j); powers(tab[i+1][j-1],i+1,j-1); powers(tab[i+1][j-2],i+1,j-2);
									tab[i+1][j].tipo=2;
									tab[i][j].color=0; tab[i+2][j].color=0;
									tab[i+1][j-1].color=0; tab[i+1][j-2].color=0;
									return true;
								}
							}
						}
					}
				}
			}
			return false;
		}
		bool Comb(int cont,int y,int x,int dir){			//Busca casillas adyacentes 
			if(dir){
				if(x+1<=7 && tab[y][x].color!=0 && tab[y][x].color==tab[y][x+1].color) Comb(cont+1,y,x+1,1);
				else{
					if(cont>=3) {
						vx=cont;
						powers(tab[y][x],y,x);
						tab[y][x].color=0;
						Player->puntuacion+=50;
						return true;
					}else return false;
				}
				if(vx>=3) {
					powers(tab[y][x],y,x);
					tab[y][x].color=0;
						Player->puntuacion+=50;
					return true;
				}return false;
			}else{
				if(y+1<=7 && tab[y][x].color!=0 && tab[y][x].color==tab[y+1][x].color) Comb(cont+1,y+1,x,0);
				else{
					if(cont>=3) {
						vx=cont;
						powers(tab[y][x],y,x);
						tab[y][x].color=0;
						Player->puntuacion+=50;
						return true;
					}else return false;
				}
				if(vx>=3){
					powers(tab[y][x],y,x);
					tab[y][x].color=0;
					Player->puntuacion+=50;
					return true;
				}return false;
			}
		}
		bool Chek_Comb(){   //Comprueba si hay combinaciones de 3 4 o 5 gemas :3
			for(int i=0;i<8;i++){
				for(int j=0;j<8;j++){
					vx=0; vy=tab[i][j].color;
					if(Comb(1,i,j,1)){
						if(vx==3){
							powers(tab[i][j],i,j);
							tab[i][j].color=0;
							Player->puntuacion+=50;
						}else if(vx==4) tab[i][j].tipo=3;
						else if(vx>=5) tab[i][j].tipo=4;
						if(vx>3) tab[i][j].color=vy;
						return true;
					}else if(Comb(1,i,j,0)){
						if(vx==3){
							powers(tab[i][j],i,j);
							tab[i][j].color=0;
							Player->puntuacion+=50;
						}else if(vx==4) tab[i][j].tipo=3;
						else if(vx>=5) tab[i][j].tipo=4; 
						if(vx>3) tab[i][j].color=vy;
						return true;
					}
				}
			}
			return false;
		}
		void powers(Gema gem,int y,int x){
			this->Re_Fill();
			if(gem.tipo==1) return;
			else if(gem.tipo==2){
				tab[y][x].color=0;
				Player->puntuacion+=100;
				for(int i=0;i<8;i++) {
					if(tab[i][x].tipo!=gem.tipo && tab[i][x].color!=gem.color){
						tab[i][x].color=0;
						Player->puntuacion+=50;
						powers(tab[i][x],i,x);
					}
				}
				for(int j=0;j<8;j++){
					if(tab[y][j].tipo!=gem.tipo && tab[y][j].color!=gem.color){
						tab[y][j].color=0;
						Player->puntuacion+=50;
						powers(tab[y][j],y,j);
					}
				}
			}else if(gem.tipo==3){
				tab[y][x].color=0;
				Player->puntuacion+=150;
				Player->puntuacion+=400;
				if(y-1>=0 && x-1>=0) {
					powers(tab[y-1][x-1],y-1,x-1); tab[y-1][x-1].color=0;
				} if(y-1>=0){
					powers(tab[y-1][x],y-1,x); tab[y-1][x].color=0;
				} if(y-1>=0 && x+1<8){
					powers(tab[y-1][x+1],y-1,x+1); tab[y-1][x+1].color=0;
				} if(x-1>=0) {
					powers(tab[y][x-1],y,x-1); tab[y][x-1].color=0;
				} if(x+1<8) {
					powers(tab[y][x+1],y,x+1); tab[y][x+1].color=0;
				} if(y+1<8 && x-1>=0){
					powers(tab[y+1][x-1],y+1,x-1); tab[y+1][x-1].color=0;
				} if(y+1<8){
					powers(tab[y+1][x],y+1,x); tab[y+1][x].color=0;
				} if(y+1<8 && x+1<8){
					powers(tab[y+1][x+1],y+1,x+1); tab[y+1][x+1].color=0;	
				}
			}else if(gem.tipo==4){
				vx=tab[y][x].color;
				Player->puntuacion+=200;
				tab[y][x].color=0;
				for (int i=0;i<8;i++){
					for(int j=0;j<8;j++){
						if(tab[i][j].color==vx){
							powers(tab[i][j],i,j);
							tab[i][j].color=0;
							Player->puntuacion+=50;
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
		// Se moverán las gemas de acuerdo al movimiento del usuario.
		// Por ello se necesita el sentido y la casilla que se movió.
		// Siempre se llama en casos dentro del tablero, por ello posx y posy no necesitan comprobarse.
		void movimiento_gemas(int sentido, int posx, int posy){
			// Tomamos las posiciones en píxeles de x y y para animar el intercambio.
			int posx_px = (posx*60)+60, posy_px = (posy*60)+120;
			// i dará el incremento o decremento de píxeles.
			for(int i=1; i <= 60; i++){
				// Aplicamos cuadro en blanco al tablero, para hacer movimiento limpio.
				apply_surface(60, 120, tablero_img, screen);
				this->imprimir_gemas();
				apply_surface(posx_px, posy_px, pos_blanca, screen);
				switch(sentido){
					case 1:
						apply_surface(posx_px, posy_px-60, pos_blanca, screen);
						apply_surface(posx_px, posy_px-i, gemas[tab[posy][posx].color-1][tab[posy][posx].tipo-1], screen);
						apply_surface(posx_px, posy_px-60+i, gemas[tab[posy-1][posx].color-1][tab[posy-1][posx].tipo-1], screen);
						break;
					case 2:
						apply_surface(posx_px+60, posy_px, pos_blanca, screen);
						apply_surface(posx_px+i, posy_px, gemas[tab[posy][posx].color-1][tab[posy][posx].tipo-1], screen);
						apply_surface(posx_px+60-i, posy_px, gemas[tab[posy][posx+1].color-1][tab[posy][posx+1].tipo-1], screen);
						break;
					case 3:
						apply_surface(posx_px, posy_px+60, pos_blanca, screen);
						apply_surface(posx_px, posy_px+i, gemas[tab[posy][posx].color-1][tab[posy][posx].tipo-1], screen);
						apply_surface(posx_px, posy_px+60-i, gemas[tab[posy+1][posx].color-1][tab[posy+1][posx].tipo-1], screen);
						break;
					case 4:
						apply_surface(posx_px-60, posy_px, pos_blanca, screen);
						apply_surface(posx_px-i, posy_px, gemas[tab[posy][posx].color-1][tab[posy][posx].tipo-1], screen);
						apply_surface(posx_px-60+i, posy_px, gemas[tab[posy][posx-1].color-1][tab[posy][posx-1].tipo-1], screen);
						break;
				}
				SDL_Flip(screen);
				SDL_Delay(1);
			}
			if(tab[posy][posx].tipo==4){
				if(sentido == 1 and posy > 0) tab[posy][posx].color=tab[posy-1][posx].color;
				if(sentido == 2 and posx < 7) tab[posy][posx].color=tab[posy][posx+1].color;
				if(sentido == 3 and posy < 7) tab[posy][posx].color=tab[posy+1][posx].color;
				if(sentido == 4 and posx > 0) tab[posy][posx].color=tab[posy][posx-1].color; 
				powers(tab[posy][posx],posy,posx);
			}else{
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
				if(!Check_Est() && !Chek_Comb()){
					Gema aux = tab[posy][posx];
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
			}
			// Checamos si el usuario será cambiado de nivel.
			this->check_lvl_change();
		}
		void Change_Tab(){
			for(int i=0;i<8;i++){
				for(int j=0;j<8;j++){
					if(tab[i][j].tipo==1) tab[i][j]=Gema();
				}
			}
		}
		void check_lvl_change () {
			int id = (this->Player->nivel-1)%5;
			SDL_Surface *ant = mundos[id];
			SDL_Surface *sig = mundos[(id+1)%5];
			if(this->Player->puntuacion > PUNTUACION_PARA_PASAR_DE_NIVEL){
				++(this->Player->nivel);
				PUNTUACION_PARA_PASAR_DE_NIVEL*=(AUMENTO_PUNTUACION+1);
				this->Change_Tab();
				for(int i=1, j=-1280; i<=1280; i+=5, j+=5){
					apply_surface(i, 0, ant, screen);
					apply_surface(j, 0, sig, screen);
					this->imprimir_gemas();
					SDL_Flip(screen);
				}
			}
		}
		void show(){
			for(int i=0;i<8;i++){
				for(int j=0;j<8;j++){
					cout<<tab[i][j].color;
				}
				cout<<endl;
			}
		}
		bool GameOver(){
			for(int i=0;i<8;i++){
				for (int j=0;j<8;j++){
					if(j+3<8){
						if(tab[i][j].color==tab[i][j+1].color && tab[i][j+1].color==tab[i][j+3].color) return true;
						if(tab[i][j].color==tab[i][j+2].color && tab[i][j+2].color==tab[i][j+3].color) return true;
					}
					if(j+2<8){
						if(i-1>=0) {
							if(tab[i][j].color==tab[i][j+1].color && tab[i][j+1].color==tab[i-1][j+2].color) return true;
							if(tab[i][j].color==tab[i+1][j+1].color && tab[i+1][j+1].color==tab[i+1][j+2].color) return true;
							if(tab[i][j].color==tab[i-1][j+1].color && tab[i-1][j+1].color==tab[i][j+2].color) return true;
						}
						if(i+1<8) {
							if(tab[i][j].color==tab[i][j+1].color && tab[i][j+1].color==tab[i+1][j+2].color) return true;
							if(tab[i][j].color==tab[i-1][j+1].color && tab[i-1][j+1].color==tab[i-1][j+2].color) return true;
							if(tab[i][j].color==tab[i+1][j+1].color && tab[i+1][j+1].color==tab[i][j+2].color) return true;
						}
					}
					if(i+3<8){
						if(tab[i][j].color==tab[i+1][j].color && tab[i+1][j].color==tab[i+3][j].color) return true;
						if(tab[i][j].color==tab[i+2][j].color && tab[i+2][j].color==tab[i+3][j].color) return true;
					}
					if(i+2<8){
						if(j-1>=0) {
							if(tab[i][j].color==tab[i+1][j-1].color && tab[i+1][j-1].color==tab[i+2][j-1].color) return true;
							if(tab[i][j].color==tab[i+1][j].color && tab[i+1][j].color==tab[i+2][j-1].color) return true;
							if(tab[i][j].color==tab[i+1][j-1].color && tab[i+1][j-1].color==tab[i+2][j].color) return true;
						}
						if(j+1<8) {
							if(tab[i][j].color==tab[i+1][j+1].color && tab[i+1][j+1].color==tab[i+2][j+1].color) return true;
							if(tab[i][j].color==tab[i+1][j].color && tab[i+1][j].color==tab[i+2][j+1].color) return true;
							if(tab[i][j].color==tab[i+1][j+1].color && tab[i+1][j+1].color==tab[i+2][j].color) return true;
						}
					}
				}
			}
			return false;
		}
		void Limp_Rec(){
			for(int i=0;i<11;i++){
				rec[i].puntuacion=0;
				rec[i].nivel=0;
				rec[i].alias="";
			}
		}
		void Save_Rec(){
			Trecord aux;
			this->Limp_Rec();
			int i=1;
			if((archivo=fopen("Records.dat","ab+"))==NULL) exit(0);
			while(i<11) {
				fread(&rec[i],sizeof(Trecord),1,archivo);
				i++;
			}
			rec[0].puntuacion=Player->puntuacion; rec[0].nivel=Player->nivel; rec[0].alias=Player->alias;
			fclose(archivo);
			for(int i=0;i<11;i++)
				for(int j=i;j<11;j++)
					if(rec[i].puntuacion>rec[j].puntuacion && rec[j].puntuacion!=0 ){
						aux=rec[i];
						rec[i]=rec[j];
						rec[j]=aux;
					}
			if((archivo=fopen("Records.dat","wb"))==NULL) exit(0);
			for(int i=0;i<10;i++){
				fwrite(&rec[i],sizeof(Trecord),1,archivo);
			}
			fclose(archivo);
			Limp_Rec();
		}
		friend void juego();
		friend void Show_Records();
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
		// x_ant, y_ant coordenadas donde iniciará el arrastre del mouse.
		int x, y, x_ant, y_ant;
	public:
		// x_vec, y_vec representación de x y para la gema de la matriz seleccionada.
		// publicas para evitar la fatiga siempre que se da click. xD
		int x_vec, y_vec;
		// Se hace en forma de clase para no tener que declarar muchos eventos.
		SDL_Event event;
		EventManager(){}
		~EventManager(){}
		// Identificamos donde está el ratón para aplicar un destello de fondo
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
		// un movimiento válido.
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
		// Esta función está a la espera de pulsaciones de tecla para mostrarlas en pantalla.
		string handle_nickname_event() {
			bool salir = false;
			string aux, nc;
			apply_surface(0, 0, nombre_jugador, screen);
			SDL_Flip(screen);
			// Mientras el usuario no presione Enter
			while( 1 and !salir ) {
				// Checamos un nuevo evento
				if( SDL_PollEvent( &event ) ) {
					// Si se pulsa una tecla
					if( event.type == SDL_KEYDOWN ) {
						// Obtenemos el código de la tecla
						aux = string(SDL_GetKeyName(event.key.keysym.sym));
						// Si la tecla presionada está en el abecedario
						if(aux.length()==1 and ((aux[0]>=65 and aux[0]<=90) or (aux[0]>=95 and aux[0]<=122)) and nc.length()<15) {
							// Concatenamos al nickname
							nc += aux;
						} else if (event.key.keysym.sym == SDLK_BACKSPACE) {
							// Si presionamos backspace eliminamos el ultimo caracter. Si la cadena no está vacía
							if(!nc.empty()) nc.erase(nc.end()-1);
						} else if(event.key.keysym.sym == SDLK_RETURN) {
							// Si presionamos enter regresamos la cadena.
							if(!nc.empty()) return nc;
						}
						// Volvemos a dibujar, porque cuando se elimina podría sobreponerse.
						apply_surface(0, 0, nombre_jugador, screen);
						message = TTF_RenderText_Solid( font_big, nc.c_str(), white );
						apply_surface(230, 306, message, screen);
						SDL_Flip(screen);
					}
				}
			}
		}
};

void juego(){
	// movimiento, cuando se da click identificamos hacia donde se mueve
	// 1 arriba, 2 derecha, 3 abajo, 4 izquierda. Sentido del reloj.
	int mov; bool inicio=true;
	// Objeto que nos permitirá el manejo de eventos a lo largo del juego.
	EventManager manager;
	string nc = manager.handle_nickname_event();
	// Inicializamos el tablero.
	Tablero tablero(nc);
	// Eliminamos cualquier combinación mayor a 3 o especial antes de iniciar.
	tablero.Check_Est();
	tablero.Chek_Comb();
	tablero.Re_Fill();
	while(tablero.GameOver() && inicio){
		// Siempre se va a sobre escribir el tablero a la espera de nuevas instrucciones.
		if( SDL_PollEvent( &manager.event )){
			if( manager.event.type == SDL_KEYDOWN ) 
				if(manager.event.key.keysym.sym == SDLK_ESCAPE){
					inicio = false;
					break;
				} 
			apply_surface(0, 0, mundos[(tablero.get_nv()-1)%5], screen);
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
				mov = manager.mouse_upclick_ev(); // Se calcula la dirección del arrastre para movimiento de gemas.
				if(mov){
					// Casos para el movimiento, 1 arriba, 2 dcha, 3 abajo, 4 izqda
					tablero.movimiento_gemas(mov, manager.x_vec, manager.y_vec);
				}
			}
			// Se imprimen las gemas, para cualquier movimiento que haya ocurrido.
			tablero.Re_Fill();
			tablero.imprimir_gemas();
			tablero.Check_Est();
			tablero.Chek_Comb();
		}
		SDL_Flip(screen);
	}
	tablero.Save_Rec();
	PUNTUACION_PARA_PASAR_DE_NIVEL=2500;
}

void Show_Records(){
	Trecord aux;
	string recplay;
	int x=200,y=100;
	if((archivo=fopen("Records.dat","rb"))==NULL) exit(0);
	while(1){
		if(SDL_PollEvent (&event)){
			if(event.type==SDL_KEYDOWN) 
				if(event.key.keysym.sym == SDLK_ESCAPE) break;
			y=100;
			rewind(archivo);
			apply_surface(0, 0, record, screen);
			for(int i=0;i<10;i++){
				fread(&aux,sizeof(Trecord),1,archivo);
				if(aux.nivel!=0){
					recplay = aux.alias+"   "+to_string(aux.nivel)+"   "+to_string(aux.puntuacion);
					recjugador=TTF_RenderText_Solid( font_big, recplay.c_str(), white );
					apply_surface(y,x, recjugador, screen);
					y+=50;
				}
			}
		}
		SDL_Flip(screen);
	}
	fclose(archivo);
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
			case 1:
				Show_Records();
				break;
			case 2:
				exit(0);
				break;
		}
	}
}
