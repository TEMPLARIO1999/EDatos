#include <iostream>
using namespace std;
#include <time.h>

class Semilla {
	private:
		//Nuestros vectores de ordenamiento, random, peor y mejor caso, asi como su tamaño
		int *vector, *peor_caso, *mejor_caso, size;
	public:
		Semilla() {
			size = 0;
			//Permitimos al usuario elegir el tamaño de los vectores mayor que 1000(recomendado) 
			cout << "Introduzca el tamaño del vector (mas de 1,000) -> ";
			cin >> size;
			// Reservamos la memoria para cada vector.
			vector = new int [size]; peor_caso = new int[size]; mejor_caso = new int[size];
			// Construímos cada uno de los vectores.
			for(int i=0, j=size; i<size; i++, j--){
				vector[i] = (rand()%100)+1;
				peor_caso[i] = j;
				mejor_caso[i] = i+1;
			}
			system("cls");
		}
		void regenerar_vectores(){
			for(int i=0, j=size; i<size; i++, j--){
				vector[i] = (rand()%100)+1;
				peor_caso[i] = j;
				mejor_caso[i] = i+1;
			}
		}
		int *get_vector(){
			return vector;
		}
		int *get_peor(){
			return peor_caso;
		}
		int *get_mejor(){
			return mejor_caso;
		}
		int get_size(){
			return size;
		}
		void imp_vec(int *a,int n){
			for (int i=0;i<n;i++) cout<<"["<<a[i]<<"]";
		}
};

//Con esta clase condensamos los datos de cada metodo 
class Benchmark {
	public:
		string names[7] = {"MergeSort ","QuickSort ","ShellSort ","HeapSort  ","Seleccion ","Inserccion","Burbuja   "};
		string vec[3] = {"CASO NORMAL","MEJOR CASO","PEOR CASO"};
		// Variables de condiciones y permutaciones.
		int condiciones[7][3], cambios[7][3]; //Cambios xd
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
		void PrintOne(int n){
			cout<< "METODO" << "VECTOR" << "CONDICIONES" << "PERMUTACIONES" <<endl<<endl;
			for (int i=0;i<3;i++)
				cout<< names[n] << vec[i] << condiciones[n][i] << cambios[n][i] <<endl;
		}
		void PrintAll(){
			cout<< "METODO" << "VECTOR" << "CONDICIONES" << "PERMUTACIONES" <<endl<<endl;
			for (int j=0;j<7;j++){
				for (int i=0;i<3;i++)
					cout<< names[j] << vec[i] << condiciones[j][i] << cambios[j][i] <<endl;
				cout<<endl;
			}
		}
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
		void Check_All(){
			Semilla s;
			for (int i=0;i<7;i++){
				Check_One(s.get_vector(),s.get_mejor(),s.get_peor(),s.get_size(),i);
				s.regenerar_vectores();
			}
		}
		void Clean(){
			for (int i = 0; i < 7 ; i++ ) 
				for (int j = 0; j < 3; j++ ){
					condiciones[i][j]=0;
					cambios[i][j]=0;
				}
		}
		
		void Merge(int *arr1,int *arr2, int *array, int n,int cast) {  		//Funcion auxiliar del metodo mergesort
			int i=0, j=0, k=0;
			condiciones[0][cast]++;
			while (i < n/2 && j < n - n/2) {
				condiciones[0][cast]++;
		  		if (arr1[i] < arr2[j]) { 
		  			array[k] = arr1 [i]; 
		  			i++; k++;
		 	    } else { 
		  			array[k] = arr2[j]; 
		  			j++; k++;
		  		} 
			}
			condiciones[0][cast]++;
			while (i < n/2) { 
				array[k] = arr1 [i]; 
				i++; k++;
			} 
			condiciones[0][cast]++;
		 	while (j < n-n/2) { 
		  		array[k] = arr2 [j]; 
		  		j++; k++;
		 	}  
		} 
		int * MergeSort (int *array, int n,int cast) { 		//Metodo de ordenamiento MergeSort
			condiciones[0][cast]++;
			if(n>1) {
				int arr1[n/2],arr2[n-n/2];
				condiciones[0][cast]++;
				for	(int i=0,j=0;i<n;i++){
					condiciones[0][cast]++;
					if(i<n/2) arr1[i]=array[i];
					else{ 
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
		  			aux=array[i];
		  			array[i]=array[pivote];
		  			array[pivote]=aux;
		  		}
		 	}
		 	aux=array[begin];
		 	array[begin]=array[pivote];
		 	array[pivote]=aux;
			return pivote;
		} 
		int * Quicksort(int * array, int begin, int end,int cast){		//Metodo de ordenamiento QuickSort
			int pivote;
			condiciones[1][cast]++;
			if(begin < end){
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
		            int aux = array[i],j;
					condiciones[2][cast]++;     
		            for (j = i; j >= k && array[j - k] > aux; j -= k) array[j] = array[j - k]; 
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
		    if (l < n && array[l] > array[max]) max = l; 
		    condiciones[3][cast]++;
		    if (r < n && array[r] > array[max]) max = r;
		    condiciones[3][cast]++;
		    if (max != i) { 
		        swap(array[i], array[max]); 
		        heap(array, n, max,cast); 
		    } 
		} 
		int * heapSort(int *array, int n,int cast) {			//Metodo de HeapSort
			condiciones[3][cast]++;
		    for (int i = n / 2 - 1; i >= 0; i--) heap(array, n, i,cast);
		    condiciones[3][cast]++;
		    for (int i=n-1; i>=0; i--) { 
		        swap(array[0], array[i]); 
		        heap(array, i, 0,cast); 
		    }
		    return array;
		}
		
		int * seleccion(int * array,int n,int cast){		//Metodo de seleccion
			condiciones[4][cast]++;
			for (int i = 0; i < n; i++) {
			    int min = i;
			    condiciones[4][cast]++;
	   			for (int j = i + 1; j < n; j++){
	   				condiciones[4][cast]++;
					if (array[min] > array[j]) min = j;
				}
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
	 		    aux = array[i];
	 		    condiciones[5][cast]++;
	    		for (i; i > 0 && array[i-1] > aux; i--) {
	        		array[i] = array[i-1];
	    		}
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
		Semilla s;
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
					if(cin>>op) break;						//Espera a que elija un metodo
				}while(1);
				if(op!=0) m.Check_One(s.get_vector(),s.get_mejor(),s.get_peor(),s.get_size(),op-1);	//Analiza el metodo seleccionado y lo imprime 
				m.PrintOne(op-1);
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
