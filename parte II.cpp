#include "graficos.h"
#include <time.h>
#include <string.h>

struct Trecord{			//Estructura utilizada para guardar los records
	int puntuacion;
	int nivel;
	char alias[30];
};

class Gema{
	public:
		int color;  			 //morado amarillo naranja verde azul rojo blanco 
		int tipo;				 //1=Normal 2=Estelar 3=Explosiva 4=Cubo
		Gema(){					 //Cuando se llama al constructor se inicializa con un color random y tipo normal
			color=(rand()%7)+1;
			tipo=1;
		}
};

//Clase donde se guardan los datos del jugador, asi como su nombre el nivel que lleva y su puntuacion
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
		int GetPuntuacion(){		//retorna la puntuacion 
			return puntuacion;
		}
};

//Clase donde se guardan las variables principales del tablero, ademas de los metodos necesarios para hacer
//Todos las funciones del juego de manera logica
class Tablero{
	private:
		Gema tab[8][8]; //Utilizamos una matriz de tipo gema simulando un tablero de 8x8 con una gema por posicion
		int vx,vy;		//Variables auxiliares
	protected:
		Jugador *Player;	//El tablero contiene un objeto de tipo jugador(el que esta jugando) de esta manera
							//se accede a todos los datos mas facil
	public:
		Tablero(string alias){	
			vx=0; vy=0;
			Player = new Jugador(alias);	//Inicia al jugador mandando su alias
			for(int i=0;i<8;i++){			//llenamos el tablero de gemas
				for(int j=0;j<8;j++){
					tab[i][j]=Gema();
				}
			}
			while(Check_Est() || Chek_Comb()){	//Eliminamos las combinaciones basura antes de iniciar partida
				Re_Fill();
				for(int i=0;i<8;i++) for(int j=0;j<8;j++) if(tab[i][j].tipo!=1) tab[i][j]=Gema();
			}
			Player->puntuacion=0;
		}
		int get_nv () {		//Retorna el nivel del jugador
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
		
		//Direccion 0=Horizontal 1=vertical 
		//Esta funcion se encarga de buscar una secuencia de tres gemas en cualquier direccion a partir de 
		//una casilla o gema que se encuentra en las coordenadas x,y recibidas
		//regresa true siencuentra algun trio
		bool B_trio(int dir,int y,int x){    
			if(dir==0){	
				//busca horizontalmente si los x y y no se salen del rango
				if(y-2>=0 && x>=0 && x<=7){
					if(tab[y][x].color!=0 && tab[y][x].color==tab[y-1][x].color && tab[y-1][x].color==tab[y-2][x].color){
						vx=x; vy=y;	//guarda la posicion si existe un grupo de tres
						powers(tab[y-1][x],y-1,x); powers(tab[y-2][x],y-2,x); //Activa la funcion poderes para cada gema 
						//													en el grupo exepto la que se mando como coordenada
						tab[y-1][x].color=0; tab[y-2][x].color=0; //Destruye las gemas
						return true;	
					}else return false;
				}else return false;
			}else if(dir==1){
				//Lo mismo ocurre para la direccion vertical 
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
		
		//Comprueba si hay combinaciones estelares
		//Este metodo se apoya de la funcion B_trio y lo qeu hace es buscar alguna combinacion estelar 
		// es decir una T o una L en todo el tablero
		//Regresa un true si encuentra alguna combinacion y si no es asi regresa un false
		bool Check_Est(){
			for(int i=0;i<8;i++){
				for(int j=0;j<8;j++){ //Con estos dos for busca en cada gema de toda la matriz
					//Si encuentra un trio hacia adelante y no se sale del rango de el tablero
					if(tab[i][j].color!=0 && tab[i][j].color==tab[i][j+1].color && tab[i][j+1].color==tab[i][j+2].color){
						if(j+2<=7) 
						//entonces busca un trio hacia arriba y hacia abajo en cada una de las tres gemas 
						if(B_trio(0,i,j) || B_trio(1,i,j) || B_trio(0,i,j+1) || B_trio(1,i,j+1) || B_trio(0,i,j+2) || B_trio(1,i,j+2)){
							//si encuentra uno de estos quiere decir que hay una combinacion estelar entonces
							//guarda el color de la gema de interseccion aumenta la puntuacion llama a los poderes
							//de las tres primeras gemas y destruye estas luego regresa el color de la gema de interseccion
							//y cambia su tipo a 2 (gema estelar)
							int aux=tab[vy][vx].color;
							Player->puntuacion+=300;
							powers(tab[i][j+1],i,j+1); powers(tab[i][j+2],i,j+2); powers(tab[i][j],i,j);
							tab[i][j+1].color=0; tab[i][j+2].color=0; tab[i][j].color=0;
							tab[vy][vx].color=aux;
							tab[vy][vx].tipo=2;
							return true;
						}
					}
					//si no encuentra en el primer caso buscamos dos casos especiales 
					else if(tab[i][j].color!=0 && tab[i][j].color==tab[i+1][j].color && tab[i+1][j].color==tab[i+2][j].color){
						//mientras o se salga del rango del tablero buscara una T acostada horizontalmenete
						//si encuentra una T en cualquiera de los casos aumenta puntuacion, llama a los poderes
						//cambia el tipo de la gema interseccion y destruye las demas
						if(i+2<=7){
							//puede ser apuntando a la izquierda
							if(tab[i+1][j].color!=0 && tab[i+1][j].color==tab[i+1][j+1].color && tab[i+1][j+1].color==tab[i+1][j+2].color){
								if(j+2<=7){
									Player->puntuacion+=300;
									powers(tab[i][j],i,j); powers(tab[i+2][j],i+2,j); powers(tab[i+1][j+1],i+1,j+1); powers(tab[i+1][j+2],i+1,j+2);
									tab[i+1][j].tipo=2;
									tab[i][j].color=0; tab[i+2][j].color=0;
									tab[i+1][j+1].color=0; tab[i+1][j+2].color=0;
									return true;
								}
							//o apuntando a la derecha
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
		
		//Busca casillas adyacentes
		//si la direccion es 1 busca horizontal si es 0 busca vertical
		//regresa true si encuentra tres o mas gemas adyacentes y false si no es asi
		bool Comb(int cont,int y,int x,int dir){ 
			if(dir){
				//Si no se sale del rango del tablero busca si existe una gema de su mismo color adyacente a ella
				//si es asi se llama recursivamente buscando otra gema adyacente
				//Si no hay una gema adyacente verifica si encontro mas de tres, si es asi guarda el numero de gemas adyacentes
				//activa los poderes de la ultima gema y la destruye aumenta puntuacion y regresa
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
				//cuando regresa recursivamente activa los poderes y destruye la gema actual 
				//ademas aumenta puntuacion todo esto si se encontraron 3 o mas gemas adyacentes
				if(vx>=3) {
					powers(tab[y][x],y,x);
					tab[y][x].color=0;
						Player->puntuacion+=50;
					return true;
				}return false;
			}else{
				//Lo mismo que se realiza en la direccion horizontal se realiza para la vertical
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
		
		//Comprueba si hay combinaciones de 3 4 o 5 gemas 
		//Este metodo se auxilia del metodo comb y lo llama para que se aplique recursivamente
		//Regresa true si se encuentra alguna combinacion de gemas en cualquier direccion y false en caso contrario
		bool Chek_Comb(){   
			for(int i=0;i<8;i++){
				for(int j=0;j<8;j++){  //Con estos dos for recorremos toda la matriz que representa el tablero
					vx=0; vy=tab[i][j].color;	//Guardamos siempre el color de la gema actual en una variable auxiliar
					if(Comb(1,i,j,1)){
						//Si encuentra una combinacion de gemas de manera horizontal, verifica si fue de tres
						//cuatro o cinco gemas 
						if(vx==3){ //si es de tres activa los poderes de la gema la detruye y aumenta puntuacion
							powers(tab[i][j],i,j);
							tab[i][j].color=0;
							Player->puntuacion+=50;
						//si es de cuatro o de cinco cambia el tipo de la gema ya que fue combinacion especial
						}else if(vx==4) tab[i][j].tipo=3;
						else if(vx>=5) tab[i][j].tipo=4;
						if(vx>3) tab[i][j].color=vy; //regresa el color de la gema actual si la combinacion fue especial
						return true;
					}else if(Comb(1,i,j,0)){
						//Realiza los mismos pasos pero ahora busca combinaciones de gemas de manera vertical
						//busca combinacion de tres cuatro cinco auxiliandose de comb
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
		
		//Esta funcion permite activar los poderes de la gema enviada como parametro y las coordenadas x,y 
		//del tablero donde se encuentra dicha gema, no tiene valor de retorno
		void powers(Gema gem,int y,int x){
			this->Re_Fill(); //Rellena espacios vacios si los hay para evitar errores
			if(gem.tipo==1) return; //si la gema es tipo 1 no tiene poder
			else if(gem.tipo==2){
				//Si la gema es tipo dos destruye todas las gemas que estan en la misma fila y en la misma
				//columna que la gema enviada despues de destruir la gema enviada y aumentar puntuacion
				//conforme va destruyendo gemas va activando poderes para cualquier reaccion en cadena
				//y aumentando la puntuacion
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
				//Si la gema es de tipo tres destruye la gema actual y hace un aumento de puntuacion
				tab[y][x].color=0;
				Player->puntuacion+=150;
				Player->puntuacion+=400;
				//Mientras no se salga del rango del tablero, va destruyendo y activando los poderes de las 
				//gemas que estan al rededor de la gema enviada
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
				//si la gema es de tipo cuatro guarda el color de la gema enviada y aumenta la puntuacion
				vx=tab[y][x].color;
				Player->puntuacion+=200;
				tab[y][x].color=0;
				//despues destruye la gema y busca por todo el tablero a las gemas que contengan el mismo color
				//que la gema recibida 
				for (int i=0;i<8;i++){
					for(int j=0;j<8;j++){
						if(tab[i][j].color==vx){
							//si las gemas tienen el mismo color estas son destruidas y se llaman a sus poderes
							//ademas se aumenta la puntuacion por cada gema destruida
							powers(tab[i][j],i,j);
							tab[i][j].color=0;
							Player->puntuacion+=50;
						}
					}
				}
			}
		}
		
		//Esta funcion se encarga de rellenar los espacios vacios en el tablero
		void Re_Fill(){
			for(int i=7;i>=0;i--){ //con estos dos for busca de abajo hacia arriba y de derecha a izquierda
								   //una gema destruida
				for(int j=0;j<8;j++){
					if(tab[i][j].color==0){
						//Si se encuentra una gema destruida busca de donde esta la gema hacia arriba una gema
						//que no este destruida y la intercambia por la gema que encontro destruida
						//quebramos el ultimo ciclo para seguir buscando
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
			//al final rellenamos de abajo hacia arriba dando una impresion de que las gemas aparecen arriba
			//y caen hacia abajo
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
		}
		
		//Con esta funcion lo que hacemos cambiar todas las fichas que no sean especiales del tablero
		//hacemos esto despues de cada nivel
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
		
		//Con esta funcion se comrueba para cada gema cualquier caso en el que pueda tener una combinacion
		//regrese un true si hay combinaciones posibles y false si no las hay 
		//utilizamos este metodo para comprobar si el juego termina por falta de combinaciones
		bool GameOver(){
			//con los dos for recorremos todo el tablero siempre cuidando que las comparaciones no se salgan del
			//rango de la matriz que representa al tablero
			for(int i=0;i<8;i++){
				for (int j=0;j<8;j++){
					//busca posibles combinaciones horizontales hacia la izquierda
					if(j+3<8){
						if(tab[i][j].color==tab[i][j+1].color && tab[i][j+1].color==tab[i][j+3].color) return true;
						if(tab[i][j].color==tab[i][j+2].color && tab[i][j+2].color==tab[i][j+3].color) return true;
					}
					//busca casos donde dos gemas horizontales se combinan con una que esta abajo o arriba vertical
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
					//busca posibles combinaciones verticales hacia abajo 
					if(i+3<8){
						if(tab[i][j].color==tab[i+1][j].color && tab[i+1][j].color==tab[i+3][j].color) return true;
						if(tab[i][j].color==tab[i+2][j].color && tab[i+2][j].color==tab[i+3][j].color) return true;
					}
					//busca casos donde dos gemas verticales pueden ser combinadas con una que este a la 
					//izquierda o a la derecha horizontalmente
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
		void Save_Rec(){
			int i=0;
			// Booleano que determinará si el jugador ha sacado récord.
			bool existe_record = false;
			// Definimos un vector de records de 20 posiciones para poder comparar.
			Trecord records[20], aux;
			// Abrimos nuestro archivo de records. Si no existe cierra la ejecucion.
			FILE *archivo = fopen("Records.dat", "rb+");
			if(archivo==NULL) {
				exit(1);
			}
			// Recuperamos todos los records. De no existir los ponemos a 0.
			if(fread(&records[i], sizeof(Trecord), 1, archivo)) {
				while(!feof(archivo)) {
					i++;
					fread(&records[i], sizeof(Trecord), 1, archivo);
				}
			} else {
				rewind(archivo); //regresamos al inicio en el archivo
				for(int i=0; i<20; i++){ 
					strcpy(records[i].alias, ("Jugador " + to_string(i+1)).c_str());
					records[i].nivel = 1;
					records[i].puntuacion = 0;
				}
			}
			Trecord anterior;
			// Hacemos las comparaciones de puntuaciones. La condicion esta dada por:
			// - Si el record esta en ceros, significa que puede modificarse por estar vacio.
			// - Si el puntaje es menor a alguno de los records.
			for(i=0; i<20; i++) {
				if(Player->GetPuntuacion()<records[i].puntuacion or !records[i].puntuacion) {
					// Cuando un record es generado, los demas se desplazaran una posicion abajo
					// desde la posicion del nuevo record.
					// Primeramente guardamos en la posición actual el nuevo récord. 
					// Guardamos la posición que había antes para recorrerla.
					anterior = records[i];
					records[i].puntuacion = Player->GetPuntuacion();
					strcpy(records[i].alias, Player->alias.c_str());
					records[i].nivel = Player->nivel;
					for(int j=i+1; j<10; j++) {
						aux = records[j];
						records[j].puntuacion=anterior.puntuacion;
						strcpy(records[j].alias, anterior.alias);
						records[j].nivel = anterior.nivel;
						anterior = aux;
					}
					existe_record = true;
					break;
				}
			}
			rewind(archivo);
			// Escribimos los 10 records al archivo para que se guarden.
			for(int i=0; i<20; i++)
				fwrite(&records[i], sizeof(Trecord), 1, archivo);
			// Felicitamos al usuario. En caso de existir récord se aplica otra imagen.
			if(existe_record)
				apply_surface(0, 0, ganador_con_record, screen);
			else
				apply_surface(0, 0, ganador, screen);
			SDL_Flip(screen);
			bool salir = false;
			while(!salir)
				while(SDL_PollEvent( &event ))
					if(event.type == SDL_KEYDOWN)
						if (event.key.keysym.sym == SDLK_ESCAPE) salir = true;
			// Cerramos el archivo para evitar perdida de datos.
			fclose(archivo);
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
			// Checamos si el usuario será cambiado de nivel.
			tablero.check_lvl_change();
		}
		SDL_Flip(screen);
	}
	tablero.Save_Rec();
	PUNTUACION_PARA_PASAR_DE_NIVEL=2500;
}

//Esta funcion se encarga de mostrar los records cuando en el menu se selecciona la opcion records
void Show_Records(){
	bool salir = false;		//booleano que nos dice cuando salir de la pantalla
	Trecord aux;			//tomamos una variable auxiliar de trecord
	FILE * archivo=NULL;	//y una variable para abrir el archivo
	string recplay;			//en este string guardamos lo que se imprimira para cada juagdor
	int x=250,i=0,y;		//x,y para coordenadas
	if((archivo=fopen("Records.dat","rb"))==NULL) exit(0);		//abrimos el archivo
	apply_surface(0, 0, record, screen);		//se imprime el fondo
	y=180;
	recplay =  "No               Nickname               Nivel               Puntuación";	//encabezado
	recjugador=TTF_RenderText_Solid( font, recplay.c_str(), white );	//Guardamos como surface el texto
	apply_surface(x,y, recjugador, screen);		//Lo imprimimos en pantalla
	while((fread(&aux,sizeof(Trecord),1,archivo)) && i<20){	//recuperamos cada record guardado y lo imprimimos
		if(aux.nivel>0){									//mientras su nivel no sea 0
			y+=20;
			recplay =  to_string(i+1) + "                    " + string(aux.alias)+"                    "+to_string(aux.nivel)+"                   "+to_string(aux.puntuacion);
			recjugador=TTF_RenderText_Solid( font, recplay.c_str(), white );
			apply_surface(x,y, recjugador, screen);
		}
		i++;
	}
	SDL_Flip(screen);
	rewind(archivo);//regresamos y cerramos el archivo
	fclose(archivo);
	while(!salir) {   //mientras no teclee esc salir sera false y continuara mostrando records
		while(SDL_PollEvent( &event )) {
			if(event.type == SDL_KEYDOWN) {
				if (event.key.keysym.sym == SDLK_ESCAPE) salir = true;
			}
		}
	}
}

//Main principal 
int main(int argc, char **argv){
	srand(time(NULL));
	init();	//Iniciamos la libreria grafica
	load_files();	//Cargamos los archivos necesarios
	menu m;	//Instanciamos un objeto de tipo menu
	while(1){
		switch(m.disp_menu()){		//De acuerdo a lo que nos regrese el metodo desp_menu es lo que hara el juego
			case 0:
				juego();	//jugar
				break;
			case 1:
				Show_Records();	//ver records
				break;
			case 2:
				exit(0);	//salir
				break;
		}
	}
}
