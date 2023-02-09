#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "funcionesLab2Grafos.h"

int main(){
	char nombreCons[31];
	char especiality[31];
	int capaMaxima;
	int pacientesTratandose;
	
	char datosCons[20]="Consultorios.in.txt";
	char datosGrafo[26]="DondeLlevarAlBulto.in.txt";
	FILE* consIn=abrirArchivo(datosCons);
	int cantidadCons;
	
	fscanf(consIn,"%d",&cantidadCons);
	if (fgetc(consIn) == '\r') {
		fgetc(consIn);
	}
	Grafo* grafo;
	grafo=crearGrafo(cantidadCons);
	
	int i;
	for(i=0; i<cantidadCons;i++){
		fscanf(consIn, " %s %s %d %d ", nombreCons, especiality, &capaMaxima, &pacientesTratandose);
		Consultorio consult= crearConsultorio(nombreCons, especiality, capaMaxima, pacientesTratandose);
		grafo->listaAdyacencia[i].consultorio=consult;
		if (fgetc(consIn) == '\r') {
		fgetc(consIn);
	}
	}
	char nombreConsult[31];
	char consultAdya[31];
	int tiempoViaje;
	
	FILE* consAdya= abrirArchivo(datosGrafo);
	int cantidadAdya;
	fscanf(consAdya, "%d", &cantidadAdya);
	if (fgetc(consAdya) == '\r') {
		fgetc(consAdya);
	}
	int j;
	for(j=0;j<cantidadAdya;j++){
		fscanf(consAdya, "%s %s %d", nombreConsult, consultAdya, &tiempoViaje);
		if (fgetc(consAdya) == '\r') {
			fgetc(consAdya);
		}
		int k;
		for(k=0; k<cantidadCons; k++){
			if(strcmp(grafo->listaAdyacencia[k].consultorio.nombre,nombreConsult)==0){
				int x;
				for(x=0; x<cantidadCons; x++){
					if(strcmp(grafo->listaAdyacencia[x].consultorio.nombre, consultAdya)==0){
						Vertice* verti=crearVertice(grafo->listaAdyacencia[x].consultorio, tiempoViaje);
						insertarEnLista(grafo->listaAdyacencia[k].listaAdyacentes, verti);
					}
				}
			}
		}
	}
	int accion;
	char salto;
	char* verificador;
	int marca=0;
	do{
		do{ if(marca==1){
				printf("Se han actualizado correctamente los datos\n");
			}
			printf("Para elegir la accion a realizar debe ingresar un numero:\n"
		        "1.- Ingresar un paciente.\n"
				"2.- Dar de alta un paciente.\n"
				"3.- Mostrar grafo.\n"
				"4.- Salir.\n");
			
		}while(((scanf("%d%c",&accion, &salto) != 2 || salto != '\n') && limpiarStdin()) || (accion < 1 || accion > 4));
		if(accion==1){
			printf("Ha elegido ingresar un paciente\n"
					"Ingrese nombre del Consultorio de inicio:\n");
			char consultorioIngreso[31];
			char especialidadBuscada[31];
			scanf("%s", consultorioIngreso);
			printf("Ingrese especialidad requerida:\n");
			scanf("%s", especialidadBuscada);
			
			int verificadorCons;
			int y;
			Consultorio auxiliarConsultorio;
			for(y=0;y<grafo->largo;y++){
				if(strcmp(grafo->listaAdyacencia[y].consultorio.nombre, consultorioIngreso)==0){
					auxiliarConsultorio=grafo->listaAdyacencia[y].consultorio;
					printf("%s\n", consultorioIngreso);
					printf("%s \n", auxiliarConsultorio.nombre);
					verificadorCons=10;
					y=grafo->largo;
					
				}
			}
			if(verificadorCons==10){
				Vertice* camino=(Vertice*)malloc(sizeof(Vertice)* grafo->largo);
				dijkstra(grafo,auxiliarConsultorio, camino);
				int verificadorEsp;
				int* coincidencias=(int*)malloc(sizeof(int)* grafo->largo);
				int cursorPosicion=0;
				int k;
				for(k=0;k<grafo->largo;k++){
					coincidencias[k]=-8;
					if(strcmp(grafo->listaAdyacencia[k].consultorio.especialidad, especialidadBuscada)==0){
						coincidencias[cursorPosicion]=k;
						verificadorEsp=10;
						cursorPosicion=cursorPosicion+1;
					}
				}
				if(verificadorEsp==10){
					int h;
					int menorTiempo=10000;
					for(h=0;h<grafo->largo;h++){
						if(coincidencias[h]==-8){
							h=grafo->largo;
						}
						else{
							if(grafo->listaAdyacencia[coincidencias[h]].consultorio.capacidadDisponible > 0){
								int l;
								for(l=0; l<grafo->largo;l++){
									if(strcmp(grafo->listaAdyacencia[coincidencias[h]].consultorio.nombre,camino[l].consultorio.nombre)==0){
										int tiempoActual=camino[l].distancia;
										if (tiempoActual<menorTiempo){
											menorTiempo=tiempoActual;
										}
									}	
								}
							}
						}
					}
					int posicionFinal=0;
					int x;
					for(x=0;x<grafo->largo;x++){
						if(camino[x].distancia==menorTiempo){
							int jk;
							for(jk=0;jk<grafo->largo;jk++){
								if(strcmp(camino[x].consultorio.nombre, grafo->listaAdyacencia[jk].consultorio.nombre)==0){
									posicionFinal=jk;
								}
							}
						}
					}
					if(menorTiempo==10000){
						printf("\n No tenemos cupos disponibles para esa especialidad. \n\n");
					}else{
						grafo->listaAdyacencia[posicionFinal].consultorio.pacientesActuales=grafo->listaAdyacencia[posicionFinal].consultorio.pacientesActuales+1;
						grafo->listaAdyacencia[posicionFinal].consultorio.capacidadDisponible=grafo->listaAdyacencia[posicionFinal].consultorio.capacidadDisponible -1;
						Vertice* caminoOrdenado=(Vertice*)malloc(sizeof(Vertice)*grafo->largo);
						imprimirCamino(camino,caminoOrdenado, menorTiempo, grafo->largo, 0);
						marca=1;
				
					}
				}else{
					printf("\n      Especialidad no encontrada\n"
					"   por favor revisa haber escrito correctamente la especialidad\n\n");
					
				}
			}else{
				printf("\n       Consultorio no encontrado\n"
				"   por favor revisa haber escrito correctamente el nombre.\n\n");
			}
			
			accion=0;
		}
		if(accion==2){
			printf("Ha elegido dar de alta un paciente.\n");
			char consultorioAlta[31];
			printf("Ingrese nombre del consultorio donde se encuentra el paciente:\n");
			scanf("%s", consultorioAlta);
			int g;
			for(g=0;g<grafo->largo;g++){
				if(strcmp(grafo->listaAdyacencia[g].consultorio.nombre,consultorioAlta)==0){
				}
			}
			darDeAlta(consultorioAlta,grafo);
			printf("Paciente dado de alta\n");
			marca=1;
			accion=0;
		}
		if(accion==3){
			imprimirGrafo(grafo);
			marca=0;
			accion=0;
			
		}
		if(accion==4){
			actualizarDatos(grafo);
		}
	
	}while(accion!=1 && accion!=2 && accion!=3 && accion!=4 && accion!=5);
	vaciarGrafo(grafo,cantidadCons);
	fclose(consIn);
	fclose(consAdya);
	return 0;
}
