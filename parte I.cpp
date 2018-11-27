#include <iostream>
using namespace std;
#include <time.h>

// Esta clase nos permite condensar la memoria y tiempo consumidos
// con cada ordenación.
class Benchmark {
	private:
		// Variables de tiempo y memoria.
		int time, time_mcaso, time_pcaso;
		float memory, memory_mcaso, memory_pcaso;
	public:
		// Constructor, declaramos todo en 0 para poder sumar.
		Benchmark(){
			time=memory=memory_mcaso=memory_pcaso=time_mcaso=time_pcaso=0;
		}
		~Benchmark(){}
		// Getters para las variables privadas.
		int get_time(){
			return time;
		}
		int get_memory(){
			return memory;
		}
		// Sobrecargamos los operadores para sumar tiempo y memoria.
		void operator +(int time){
			this->time+=time;
		}
		void operator +(float memory){
			this->memory+=memory;
		}
};

class Semilla {
	private:
		//Nuestro vector de ordenamiento y su tamaño respectivo.
		double *vector, *peor_caso, *mejor_caso, size;
	public:
		Semilla() {
			size = 0;
			// Ciclo infinito que permita tener (al menos) 10,000 elementos para ordenar.
			do{
				cout << "Introduzca el tamaño del vector (mas de 10,000) -> ";
				cin >> size;
			} while (size < 10000);
			// Reservamos la memoria para el vector.
			vector = peor_caso = mejor_caso = new double[(int)size];
			// Construímos el vector.
			for(int i=0, j=size; i<size; i++, j--){
				vector[i] = rand()%RAND_MAX;
				peor_caso[i] = j;
				mejor_caso[i] = i;
			}
			system("cls");
		}
		double *get_vector(){
			return vector;
		}
};

class Ordenamientos {
	
};

// • Inserción
// • Selección	
// • Intercalación (burbuja)
// • Mergesort
// • Shellsort
// • Quicksort
// • Heapsort

int main() {
	srand(time(NULL));
	Semilla s;
	int op;
	do{
		system("pause");
		system("cls");
		cout << "M E T O D O S    D E    O R D E N A M I E N T O";
		cout << "\n1. Prueba individual";
		cout << "\n2. Prueba en grupo";
		cout << "\n3. Salir";
		cout << "\n -> ";
		cin >> op;
		switch(op){
			case 1:
				cout << endl << "Individual";
				break;
			case 2:
				cout << endl << "Grupo";
				break;
			case 3:
				cout << endl << "Saliendo";
				break;
			default:
				cout << endl << "Opcion incorrecta!";
				break;
		}
		cout << endl;
	} while (op!=3);
}
