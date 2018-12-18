#include <iostream>
using namespace std;
#include <time.h>

class Semilla {
	private:
		//Nuestros vectores de ordenamiento, random, peor y mejor caso, asi como su tamaño
		int *vector, *peor_caso, *mejor_caso, size;
		//Nuestros vectores originales para que no se modifiquen
		int *vectorO,*peor_casoO,*mejor_casoO;
	public:
		Semilla(int size) {
			this->size = size;
			// Reservamos la memoria para cada vector.
			vector = new int [size]; peor_caso = new int[size]; mejor_caso = new int[size];
			vectorO = new int [size]; peor_casoO = new int[size]; mejor_casoO = new int[size];
			// Construímos cada uno de los vectores.
			for(int i=0, j=size; i<size; i++, j--){
				vectorO[i] = (rand()%100)+1;
				peor_casoO[i] = j;
				mejor_casoO[i] = i+1;
				vector[i] = vectorO[i];
				peor_caso[i] = peor_casoO[i];
				mejor_caso[i] = mejor_casoO[i];
			}
			system("cls");
		}
		//con esta funcion copiamos los vectores originales en los copia
		void regenerar_vectores(){
			for(int i=0; i<size; i++){
				vector[i] = vectorO[i];
				peor_caso[i] = peor_casoO[i];
				mejor_caso[i] = mejor_casoO[i];
			}
		}
		//regresan cada uno de los vectores
		int *get_vector(){
			return vector;
		}
		int *get_peor(){
			return peor_caso;
		}
		int *get_mejor(){
			return mejor_caso;
		}
		//retorna el tamaño de los vectores
		int get_size(){
			return size;
		}
};

//Con esta clase condensamos los datos de cada metodo 
class Benchmark {
	public:
		string names[7] = {"MergeSort ","QuickSort ","ShellSort ","HeapSort  ","Seleccion ","Inserccion","Burbuja   "};
		string vec[3] = {"CASO NORMAL","MEJOR CASO","PEOR CASO"};
		// Variables de condiciones y cambios.
		int condiciones[7][3], cambios[7][3]; 
		/*
		Se manejara cada metodo con un indice en las matrices
		Indice 0 -> Metodo MergeSort
		Indice 1 -> Metodo QuickSort
		Indice 2 -> Metodo ShellSort
		Indice 3 -> Metodo HeapSort
		Indice 4 -> Metodo Seleccion
		Indice 5 -> Metodo Inserccion
		Indice 6 -> Metodo Burbuja
		Cada metodo con su normal, mejor y peor caso en cada variable
		Indice 0 -> Caso Normal
		Indice 1 -> Mejor Caso
		Indice 2 -> Peor Caso
		*/
		// Constructor, declaramos todo en 0 para poder sumar.
		Benchmark(){
			for (int i = 0; i < 7 ; i++ ) 
				for (int j = 0; j < 3; j++ ){
					condiciones[i][j]=0;
					cambios[i][j]=0;
				} 
		}
		//Imprime las variables de un metodo de acuerdo al numero que le mandamos
		void PrintOne(int n){
			cout<< "	METODO" << "		VECTOR" << "		CONDICIONES" << "	CAMBIOS" <<endl<<endl;
			for (int i=0;i<3;i++)
				cout<< "	"<< names[n] << "	"<< vec[i] <<  "	"<<condiciones[n][i] <<"		"<< cambios[n][i] <<endl;
		}
		//imprime una tabla con las variables de todos los metodos en una tabla comparativa
		void PrintAll(){
			cout<< "	METODO" << "		VECTOR" << "		CONDICIONES" << "	CAMBIOS" <<endl<<endl;
			for (int n=0;n<7;n++){
				for (int i=0;i<3;i++)
					cout<< "	"<< names[n] << "	"<< vec[i] <<  "	"<<condiciones[n][i] <<"		"<< cambios[n][i] <<endl;
				cout<<endl;
			}
		}
		//Manda a llamar al metodo seleccionado mediante op en sus tres casos
		void Check_One(int *array,int *best,int *worst, int n,int op){
			int *a;
			switch(op){
				case 0:
					MergeSort(array,n,0);
					MergeSort(best,n,1);
					MergeSort(worst,n,2);
					break;
				case 1:
					Quicksort(array,0,n-1,0);
					Quicksort(best,0,n-1,1);
					Quicksort(worst,0,n-1,2);
					break;
				case 2:
					shellSort(array,n,0);
					shellSort(best,n,1);
					shellSort(worst,n,2);
					break;
				case 3:
					heapSort(array,n,0);
					heapSort(best,n,1);
					heapSort(worst,n,2);
					break;
				case 4:
					seleccion(array,n,0);
					seleccion(best,n,1);
					seleccion(worst,n,2);
					break;
				case 5:
					insercion(array,n,0);
					insercion(best,n,1);
					insercion(worst,n,2);
					break;
				case 6:
					burbuja(array,n,0);
					burbuja(best,n,1);
					burbuja(worst,n,2);
					break;
				default:
					cout<<endl<<"ERROR INTERNO"<<endl;
			}
		}
		//dentro de un for manda a llamar a cada metodo utilizando la funcion check_one enviandole numeros
		//del 0 al 7 ademas de ir regenerando los vectores para que todos los metodos ordenen los mismos vectores
		void Check_All(){
			int size = 0;
			do{
				cout << "Introduzca el tamaño del vector (mas de 1,000) -> ";
				cin>>size;
			} while (size<1000);
			Semilla s(size);
			for (int i=0;i<7;i++){
				Check_One(s.get_vector(),s.get_mejor(),s.get_peor(),s.get_size(),i);
				s.regenerar_vectores();
			}
		}
		//limpia las variables que guardan las condiciones y los cambios de cada metodo
		void Clean(){
			for (int i = 0; i < 7 ; i++ ) 
				for (int j = 0; j < 3; j++ ){
					condiciones[i][j]=0;
					cambios[i][j]=0;
				}
		}
		//esta funcion es una funcion auxiliar que utiliza el metodo merge sort
		void Merge(int *arr1,int *arr2, int *array, int n,int cast) {  		//Funcion auxiliar del metodo mergesort
			int i=0, j=0, k=0;
			condiciones[0][cast]++;
			while (i < n/2 && j < n - n/2) {
				condiciones[0][cast]++;
		  		if (arr1[i] < arr2[j]) { 
		  			cambios[0][cast]++;
		  			array[k] = arr1 [i]; 
		  			i++; k++;
		 	    } else { 
		 	    	cambios[0][cast]++;
		  			array[k] = arr2[j]; 
		  			j++; k++;
		  		} 
			}
			condiciones[0][cast]++;
			while (i < n/2) { 
				cambios[0][cast]++;
				array[k] = arr1 [i]; 
				i++; k++;
			} 
			condiciones[0][cast]++;
		 	while (j < n-n/2) { 
		 		cambios[0][cast]++;
		  		array[k] = arr2 [j]; 
		  		j++; k++;
		 	}  
		}
		//el metodo merge sort lo que hace es ir partiendo el vector en dos vectores hasta llegar a ser un vector
		//de un elemento y despues los va intercalando para que quede ordenado
		int * MergeSort (int *array, int n,int cast) { 		//Metodo de ordenamiento MergeSort
			condiciones[0][cast]++;
			if(n>1) {
				int arr1[n/2],arr2[n-n/2];
				condiciones[0][cast]++;
				for	(int i=0,j=0;i<n;i++){
					condiciones[0][cast]++;
					if(i<n/2) {
						cambios[0][cast]++;
						arr1[i]=array[i];
					}else{
						cambios[0][cast]++;
						arr2[j]=array[i]; 
						j++;
					}
				}
	  		    MergeSort(arr1, n/2,cast);
	  			MergeSort(arr2, n-n/2,cast); 
	  			Merge(arr1,arr2,array, n,cast);
			}
			return array;
		}
		
		int Quick(int *array, int begin, int end,int cast){		//Funcion auxiliar del metodo quickSort
			int i, pivote, valor_pivote, aux;
			pivote = begin;
			valor_pivote = array[pivote];
			condiciones[1][cast]++;
			for (i=begin+1; i<=end; i++){
				condiciones[1][cast]++;
		  		if (array[i] < valor_pivote){
		  			pivote++; 
		  			cambios[1][cast]++;
		  			aux=array[i];
		  			array[i]=array[pivote];
		  			array[pivote]=aux;
		  		}
		 	}
		 	cambios[1][cast]++;
		 	aux=array[begin];
		 	array[begin]=array[pivote];
		 	array[pivote]=aux;
			return pivote;
		} 
		int * Quicksort(int * array, int begin, int end,int cast){		//Metodo de ordenamiento QuickSort
			int pivote;
			condiciones[1][cast]++;
			if(begin < end){
				cambios[1][cast]++;
				pivote=Quick(array, begin, end,cast);
				Quicksort(array, begin, pivote-1,cast);
				Quicksort(array, pivote+1, end,cast);
			}
			return array;
		}
		
		int * shellSort(int * array, int n,int cast) {		//Metodo de ShellSort
			condiciones[2][cast]++;
		    for (int k = n/2; k > 0; k /= 2) {
		    	condiciones[2][cast]++;
		        for (int i = k; i < n; i++ ) {
		        	cambios[2][cast]++;
		            int aux = array[i],j;
					condiciones[2][cast]++;     
		            for (j = i; j >= k && array[j - k] > aux; j -= k) {
		            	cambios[2][cast]++;
						array[j] = array[j - k];
					}
					cambios[2][cast]++;	
		            array[j] = aux; 
		        } 
		    } 
		    return array; 
		}
		
		void heap (int *array, int n, int i,int cast) {		//Funcion auxiliar del metodo de heapsort
		    int max = i; 
		    int l = 2*i + 1; 
		    int r = 2*i + 2;
		    condiciones[3][cast]++;
		    if (l < n && array[l] > array[max]) {
		    	cambios[3][cast]++;
				max = l; 
			}    
			condiciones[3][cast]++;
		    if (r < n && array[r] > array[max]) {
		    	cambios[3][cast]++;
				max = r;
			}    
			condiciones[3][cast]++;
		    if (max != i) {
		    	cambios[3][cast]++;
		        swap(array[i], array[max]); 
		        heap(array, n, max,cast); 
		    } 
		} 
		int * heapSort(int *array, int n,int cast) {			//Metodo de HeapSort
			condiciones[3][cast]++;
		    for (int i = n / 2 - 1; i >= 0; i--) heap(array, n, i,cast);
		    condiciones[3][cast]++;
		    for (int i=n-1; i>=0; i--) {
		    	cambios[3][cast]++;
		        swap(array[0], array[i]); 
		        heap(array, i, 0,cast); 
		    }
		    return array;
		}
		
		int * seleccion(int * array,int n,int cast){		//Metodo de seleccion
			condiciones[4][cast]++;
			for (int i = 0; i < n; i++) {
				cambios[4][cast]++;
			    int min = i;
			    condiciones[4][cast]++;
	   			for (int j = i + 1; j < n; j++){
	   				condiciones[4][cast]++;
					if (array[min] > array[j]) {
						cambios[4][cast]++;
						min = j;
					}
				}
				cambios[4][cast]++;
	   			int aux = array[i];
	   			array[i] = array[min];
	   			array[min] = aux;
			}
			return array;
		}
		
		int * insercion(int * array,int n,int cast){		//Metodo de inserccion
			int aux;
			condiciones[5][cast]++;
			for (int i = 0; i < n; i++) {
				cambios[5][cast]++;
	 		    aux = array[i];
	 		    condiciones[5][cast]++;
	    		for (i; i > 0 && array[i-1] > aux; i--) {
	    			cambios[5][cast]++;
	        		array[i] = array[i-1];
	    		}
	    		cambios[5][cast]++;
	    		array[i] = aux;
			}
			return array;
		}
		
		int * burbuja(int * array,int n,int cast){	//Metodo de intercalacion o burbuja
			int aux;
			condiciones[6][cast]++;
			for(int i=0;i<n;i++){
				condiciones[6][cast]++;
				for(int j=i+1;j<n;j++){
					condiciones[6][cast]++;
					if(array[j]<array[i]){
						cambios[6][cast]++;
						aux=array[j];
						array[j]=array[i];
						array[i]=aux;
					}
				}
			}
			return array;
		}
		~Benchmark(){}
};


int main() {
	srand(time(NULL));
	Benchmark m;
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
		int size = 0;
		//Permitimos al usuario elegir el tamaño de los vectores mayor que 1000(recomendado) 
		if(op==1) {
			do {
				cout << "Introduzca el tamaño del vector (mas de 1,000) -> ";
				cin >> size;
			} while (size < 1000);
		} 
		Semilla s(size);
		switch(op){
			case 1:
				do {										//Menu de metodos
					cout<<"1 -> Metodo MergeSort"<<endl;
					cout<<"2 -> Metodo QuickSort"<<endl;
					cout<<"3 -> Metodo ShellSort"<<endl;
					cout<<"4 -> Metodo HeapSort"<<endl;
					cout<<"5 -> Metodo de Seleccion"<<endl;
					cout<<"6 -> Metodo de Inserccion"<<endl;
					cout<<"7 -> Metodo de Burbuja"<<endl;
					cout<<"0 -> Salir"<<endl;
					cin >> op; //Espera a que elija un metodo
				}while(op!=0);
				if(op!=0){
					m.Check_One(s.get_vector(),s.get_mejor(),s.get_peor(),s.get_size(),op-1);	//Analiza el metodo seleccionado y lo imprime 
					m.PrintOne(op-1);
				}
				break;
			case 2:
				m.Check_All();//Analiza todos los metodos e imprime resultados
				m.PrintAll();
				break;
			case 3:
				cout << endl << "Saliendo";
				break;
			default:
				cout << endl << "Opcion incorrecta!";
				break;
		}
		cout << endl;
		m.Clean();           //Limpia los resultados para nuevos analisis
	} while (op!=3);
}
