#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

typedef struct consultorio Consultorio;
typedef struct vertice Vertice;
typedef struct lista Lista;
typedef struct listaAdya ListaAdya;
typedef struct grafo Grafo;
typedef struct ruta Ruta;
FILE* abrirArchivo(char* nombre);
FILE* prepararArchivo();
Consultorio crearConsultorio(char* nombreCons, char* especiality, int capaMax, int capaUse);
Vertice* crearVertice(Consultorio consul, int tiempo);
Lista* crearLista();
void insertarEnLista(Lista* lista, Vertice* vertice);
Grafo* crearGrafo(int cantidad);
void vaciarGrafo(Grafo* grafo, int cantidadRegistros);
void vaciarLista(Lista* lista);
void imprimirGrafo(Grafo* grafo);
void darDeAlta(char* nombreConsultorio, Grafo* grafo);
Vertice crearVertice2(Consultorio consul);
void insertarEnArreglo(Vertice* arreglo, Vertice vertice, int tamanoGrafo);
Vertice obtenerVerticeMasCercano(Vertice* vertices, int largo);
void dijkstra(Grafo* grafo, Consultorio consultorio, Vertice* camino);
void imprimirCamino(Vertice* camino,Vertice* caminoOrdenado, int tiempo, int largo, int cursor);
void actualizarDatos(Grafo* grafo);
int limpiarStdin();

typedef struct consultorio{
	char nombre[31];
	char especialidad[31];
	int capacidadMaxima;
	int pacientesActuales;
	int capacidadDisponible;
}Consultorio;

typedef struct vertice{
	Consultorio consultorio;
	int tiempoDemora;
	int distancia;
	int anterior;
	int marca;
	int visitado;
	Vertice* siguiente;
}Vertice;

typedef struct lista{
	Vertice* inicio;
	Vertice* fin;
	int largo;
}Lista;

typedef struct listaAdya{
	Lista* listaAdyacentes;
	Consultorio consultorio;
}ListaAdya;

typedef struct grafo{
	ListaAdya* listaAdyacencia;
	int largo;
}Grafo;

//Funcion que abre un archivo en formato .txt para su lectura.
//Entrada:ruta (nombre) del archivo txt    Salida: archivo
FILE* abrirArchivo(char* nombre){
	FILE* archivo;
	archivo= fopen(nombre,"r");
	if(archivo==NULL){
		printf("No se pudo abrir el archivo.\n");
	}
	return archivo;
}
//Funcion que crea o vacia un archivo en formato .txt para su posterior escritura.
//Entrada:      Salida: archivo.
FILE* prepararArchivo(){
	FILE* archiv;
	char text[15]="WiiuuWiiuu.out"; 
	archiv=fopen(text, "w");
	return archiv;
}
//Funcion que crea un Consultorio con todos sus datos.
//Entrada: nombre del consultorio, especialidad, capac maxima, pacientes en tratamiento     Salida: Consultorio.
Consultorio crearConsultorio(char* nombreCons, char* especiality, int capaMax, int capaUse){
	Consultorio consul;
	strcpy(consul.nombre, nombreCons);
	strcpy(consul.especialidad, especiality);
	consul.capacidadMaxima=capaMax;
	consul.pacientesActuales=capaUse;
	consul.capacidadDisponible=capaMax-capaUse;
	return consul;
}
//Funcion que crea un vertice con todos sus datos.
//Entrada: Consultorio, tiempo de viaje   Salida:Vertice.
Vertice* crearVertice(Consultorio consul, int tiempo){
	Vertice* vertice;
	vertice=(Vertice*) malloc(sizeof(Vertice));
	if(vertice==NULL){
		printf("No se pudo asignar memoria al vertice.\n");
	}
	vertice->consultorio=consul;
	vertice->tiempoDemora=tiempo;
	vertice->distancia=0;
	vertice->anterior=-1;
	vertice->marca=0;
	vertice->visitado=0;
	vertice->siguiente=NULL;
	return vertice;
}
//Funcion que crea una lista vacia.
//Entrada:        Salida: Lista.
Lista* crearLista(){
	Lista* lista;
	lista=(Lista*)malloc(sizeof(Lista));
	if(lista==NULL){
		printf("No se pudo asignar memoria a la lista.\n");
		return NULL;
	}
	lista->inicio=NULL;
	lista->fin=NULL;
	lista->largo=0;
	return lista;
}
//Funcion que inserta un vertice en la ultima posicion de la lista.
//Entrada: Lista, Vertice    Salida:
void insertarEnLista(Lista* lista, Vertice* vertice){
	if(lista->inicio==NULL){
		lista->inicio=vertice;
		lista->fin=vertice;
		lista->fin->siguiente=NULL;
		lista->largo=(lista->largo +1);
	}else{
		lista->fin->siguiente=vertice;
		lista->fin=vertice;
		vertice->siguiente=NULL;
		lista->largo=(lista->largo +1);
		}
}
//Funcion que crea el grafo.
//Entrada: cantidad de registros   Salida: Grafo.
Grafo* crearGrafo(int cantidad){
	Grafo* grafo= (Grafo*)malloc(sizeof(Grafo)*cantidad);
	if (grafo==NULL){
		printf("No se pudo asignar memoria al grafo.\n");
	}
	grafo->listaAdyacencia=(ListaAdya*)malloc(sizeof(ListaAdya)*cantidad);
	if(grafo->listaAdyacencia==NULL){
		printf("No se pudo asignar memoria a la lista adyacente.\n");		
	}
	int i;
	for(i=0; i<cantidad; i++){
		grafo->listaAdyacencia[i].listaAdyacentes=crearLista();
	}
	grafo->largo=cantidad;
	return grafo;
}
//Funcion que libera la memoria usada en el grafo.
//Entrada: Grafo, cantida de registros    Salida: 
void vaciarGrafo(Grafo* grafo, int cantidadRegistros){
	int i;
	for(i=0;i<cantidadRegistros;i++){
		vaciarLista(grafo->listaAdyacencia[i].listaAdyacentes);
	}
	free(grafo->listaAdyacencia);
	free(grafo);
}
//Funcion que libera la memoria de una lista.
//Entrada: Lista     Salida:
void vaciarLista(Lista* lista){
	Vertice* auxiliar;
	auxiliar=lista->inicio;
	while(auxiliar!=NULL){
	    Vertice* aux2=auxiliar; 
		auxiliar=auxiliar->siguiente;
		free(aux2);
	}
	free(lista);
}
//Funcion que imprime el grafo completo (como lista de adyacencia).
//Entrada: Grafo   Salida:
void imprimirGrafo(Grafo* grafo){
	Vertice* aux;
	int i;
	for(i=0;i<grafo->largo;i++){
		printf("|%s| |%d| ",grafo->listaAdyacencia[i].consultorio.nombre, grafo->listaAdyacencia[i].consultorio.pacientesActuales);
		aux=grafo->listaAdyacencia[i].listaAdyacentes->inicio;
		while(aux!=NULL){
			printf("%s->%s  ",grafo->listaAdyacencia[i].consultorio.nombre, aux->consultorio.nombre);
			aux=aux->siguiente;
		}
		printf("\n");
	}
	free(aux);
}
//Funcion que busca y da de alta un paciente a partir de un consultorio dado.
//Entrada: nombreConsultorio, grafo    Salida: 
void darDeAlta(char* nombreConsultorio, Grafo* grafo){
	int cantidadDeConsultorios= grafo->largo;
	int x;
	for(x=0;x<cantidadDeConsultorios;x++){
		if(strcmp(grafo->listaAdyacencia[x].consultorio.nombre,nombreConsultorio)==0){
			grafo->listaAdyacencia[x].consultorio.pacientesActuales--;
			grafo->listaAdyacencia[x].consultorio.capacidadDisponible++;
		}
	}
}
//Funcion que crea un vertice a partir de un consultorio.
//Entrada: consultorio     Salida: vertice.
Vertice crearVertice2(Consultorio consul){
	Vertice vertice;
	vertice.consultorio=consul;
	vertice.tiempoDemora=0;
	vertice.distancia=10000;
	vertice.anterior=-1;
	vertice.marca=0;
	vertice.visitado=0;
	vertice.siguiente=NULL;
	return vertice;
}
//Funcion que recorre un arreglo de vertices en busca de un espacio vacio en el cual colocar un vertice nuevo y lo marca como espacio ocupado.
//Entrada: arreglo (de vertices), vertice (el cual se busca agregar), tamaño del grafo (cantidad de vertices)   Salida:void-> se agrega un vertice nada al arreglo
void insertarEnArreglo(Vertice* arreglo, Vertice vertice, int tamanoGrafo){
	int a;
	for(a=0; a<tamanoGrafo;a++){
		if(arreglo[a].marca==0){
			arreglo[a]=vertice;
			arreglo[a].marca=1;
			a=tamanoGrafo;
		}
	}
}
//Funcion que busca el vertice con la menor distancia en tiempo desde un arreglo de vertices.
//Entrada: vertices (Arreglo de vertices), largo (cantidad total de vertices).  Salida: Vertice con la distancia menor. 
Vertice obtenerVerticeMasCercano(Vertice* vertices, int largo){
	int distanciaMinima= 100000;
	Vertice retorno;
	int posicion;
	int k;
	for(k=0; k<largo; k++){
		if(vertices[k].visitado==0){
			if(vertices[k].distancia<distanciaMinima){
				distanciaMinima=vertices[k].distancia;
				posicion=k;
			}
		}	
	}
	vertices[posicion].visitado=1;
	retorno=vertices[posicion];
	return retorno;
}
//Funcion que calcula la distancia en tiempo desde un consultorio de partida hasta todos los demas del grafo.
//Entradas: grafo completo, consultorio de partida, camino (arreglo de vertices)   Salida: void-> solo se agregan vertices al arreglo con las nuevas distancias (tiempos)
void dijkstra(Grafo* grafo, Consultorio consultorio, Vertice* camino){
	int x;
	int largoGrafo= grafo->largo;
	Vertice* vertices=(Vertice*)malloc(sizeof(Vertice)* largoGrafo);
	if(vertices==NULL){
		printf("No se pudo asignar memoria al arreglo de vertices\n");
	}
	for(x=0; x<largoGrafo;x++){
		vertices[x]=crearVertice2(grafo->listaAdyacencia[x].consultorio);
		if(strcmp(vertices[x].consultorio.nombre,consultorio.nombre)==0){
			vertices[x].distancia=0;
		}
	}
	Vertice* cola=(Vertice*)malloc(sizeof(Vertice)* largoGrafo);
	int y=0;
	while (y<largoGrafo){
		cola[y]=vertices[y];
		y++;
	}
	int z;
	for(z=0;z<largoGrafo;z++){
		Vertice u=obtenerVerticeMasCercano(cola, largoGrafo);
		insertarEnArreglo(camino, u, largoGrafo);
		char* nombreU= u.consultorio.nombre;
		int largoLista=0;
		int posicionConsultorioCamino;
		int c;
		for(c=0; c<largoGrafo;c++){
			if(camino[c].marca==1){
				if(strcmp(camino[c].consultorio.nombre, nombreU)==0){
					posicionConsultorioCamino=c;
				}
			}	
		}
		int posicionConsultorioGrafo;
		int j;
		for(j=0; j<largoGrafo;j++){
			if(strcmp(grafo->listaAdyacencia[j].consultorio.nombre, nombreU)==0){
				largoLista= grafo->listaAdyacencia[j].listaAdyacentes->largo;
				posicionConsultorioGrafo=j;
				j=largoGrafo;
			}
		}
		Vertice* auxiliar= grafo->listaAdyacencia[posicionConsultorioGrafo].listaAdyacentes->inicio;
		int d;
		for(d=0;d<largoLista;d++){
			int peso= auxiliar->tiempoDemora;
			char* nombreVerticeActual = auxiliar->consultorio.nombre;
			int posicionVerticeCola=0;
			int e;
			for(e=0;e<largoGrafo;e++){
				if(strcmp(cola[e].consultorio.nombre,nombreVerticeActual)==0){
					posicionVerticeCola=e;
					e=largoGrafo;
				}
			}
			if(cola[posicionVerticeCola].distancia>(u.distancia + peso)){
				cola[posicionVerticeCola].distancia=u.distancia + peso;
				cola[posicionVerticeCola].anterior=posicionConsultorioCamino;
			}
			auxiliar=auxiliar->siguiente;
		}
	}
	free(cola);
	free(vertices);
}
//Funcion que imprime el camino por pantalla en el orden correcto de inicio a fin.
//Entradas: camino (retorno de dijkstra), caminoOrdenado (arreglo vacio de vertices), tiempo(menor tiempo de viaje del grafo), largo(cantidad de vertices), cursor (numero con el cual controlo la entrada en el arreglo)
//Salida: void->imprime por pantalla.
void imprimirCamino(Vertice* camino,Vertice* caminoOrdenado, int tiempo, int largo, int cursor){
	int i;
	int tiempoTotal=tiempo;
	for(i=0;i<largo;i++){
		if(camino[i].distancia==tiempo){
			caminoOrdenado[cursor]=camino[i];
			cursor=cursor+1;
			if(camino[camino[i].anterior].anterior==-1){
				caminoOrdenado[cursor]=camino[camino[i].anterior];
				cursor=cursor+1;
				i=largo;
			}else{
			tiempo=camino[camino[i].anterior].distancia;
			i=0;
			}
		}
	}
	FILE* archivoSalida=prepararArchivo();
	while(cursor >1){
		fprintf(archivoSalida,"%s ",caminoOrdenado[cursor-1].consultorio.nombre);
		fprintf(archivoSalida,"%s ", caminoOrdenado[cursor-2].consultorio.nombre);
		fprintf(archivoSalida,"%d", caminoOrdenado[cursor-2].distancia-caminoOrdenado[cursor-1].distancia);
		fprintf(archivoSalida,"\r\n");
		cursor=cursor-1;
	}
	fprintf(archivoSalida,"\r\n");
	fprintf(archivoSalida,"Tiempo total: %d minutos.\r\n", tiempoTotal);
	fclose(archivoSalida);
}
//Funcion que actualiza la informacion presente en los consultorios y reescribe el archivo txt de entrada.
//Entrada: grafo        Salida:
void actualizarDatos(Grafo* grafo){
	int x;
	FILE* archivoActualizar=fopen("Consultorios.in.txt","w");
	fprintf(archivoActualizar,"%d", grafo->largo);
	fprintf(archivoActualizar,"\r\n");
	for(x=0;x<grafo->largo; x++){
		fprintf(archivoActualizar,"%s ",grafo->listaAdyacencia[x].consultorio.nombre);
		fprintf(archivoActualizar,"%s ",grafo->listaAdyacencia[x].consultorio.especialidad);
		fprintf(archivoActualizar,"%d ",grafo->listaAdyacencia[x].consultorio.capacidadMaxima);
		fprintf(archivoActualizar,"%d",grafo->listaAdyacencia[x].consultorio.pacientesActuales);
		fprintf(archivoActualizar,"\r\n");
	}
	fclose(archivoActualizar);
}
//Funcion que se encarga de correr el cursor de lectura.
//Entrada:              Salida: 1 
int limpiarStdin(){
    while (getchar()!='\n');
    return 1;
}

