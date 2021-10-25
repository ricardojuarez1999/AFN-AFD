#include <stdio.h>
#include <conio.h>
#include <iostream>
#include <locale.h>
#include <stdlib.h>
#include <string>
#include <string.h>
#include <fstream>

using namespace std; 

string direccion = "";
int l =0;

string datos[4][100];
string caminos[50][100];

string estados_afd[30][100];
string tabla_afd[30][10];

string datos_afd[4][100];
string caminos_afd[50][100];

string ef_afd[100];

int obtener_n_caminos();
bool validacion();
void conversion_afn_afd();
void imprimir_datos();
void error_conjunto(int linea,int columna);
void imprimir_caminos();
void error();
void error_datos(int linea,int columna);
bool es_espacio(int i, string linea);
void error_caracter(int linea,int columna);
void error_unico(int linea,int columna);
void error_general(int linea,int columna);
void agregar_epsilon(int estado, string estados[][100]);
bool dato_repetido(string dato, int posicion);
bool dato_repetido_conjunto(string conjunto_pre[][100], string dato);
void imprimir_estados_afd();
void imprimir_tabla_afd();
void ordenar_conjunto(int estado);
string verificacion_de_conjunto(int estado, string alfa);
bool pre_existe(string conjunto_pre[][100]);
int longitud_conjunto(string conjunto[][100], int dato);
string ingresar_conjunto(string conjunto[][100]);
bool pre_vacio(string conjunto_pre[][100]);
string obtener_letra(string conjunto[][100]);
int elementos_tabla();
void crear_AFDTXT();
string obtener_ruta_nueva();
string afd_S();
string afd_S0();
string afd_T();
string afd_A();
string afd_F();
void cadenas();
string validar_cadena(string cadena);
int obtener_posicion(char cadena);
int obtener_indice_de_letra(string actual);


main(){
	system("color 0A");
	cout<<endl<<"CONVERSOR DE AFN A AFD "<<endl<<endl;
	if(validacion()){
		cout<<"Presione la tecla Enter para continuar..."<<endl;
		getch();
		system("cls");
		try{
			conversion_afn_afd();
			crear_AFDTXT();
		}catch(int e){
			cout<<"Ha ocurrido un error al tratar de transformar el archivo a un AFD\nSaliendo...\nPresione una tecla para continuar..."<<endl;
			getch();
			exit(0);
		}
		imprimir_estados_afd();
		imprimir_tabla_afd();
		cadenas();
	}
}

void conversion_afn_afd(){
	int i = 0;
	int j = 0;
	estados_afd[0][0] = "A";
	estados_afd[0][1] = datos[1][1];
	agregar_epsilon(0,estados_afd);
	ordenar_conjunto(0);
	tabla_afd[0][0] = "A";
	
	while(estados_afd[j][0] != ""){
		
		i = 1;
		while(datos[3][i] != ""){
			tabla_afd[j][i] = verificacion_de_conjunto(j,datos[3][i]);
			i++;	
		}
		j++;
	}
	cout<<endl<<"Conversion Exitosa"<<endl;
}

bool validacion(){
	int adelante = 0;
	int atras = 0;
	int x;
	string ruta;
	string line;
	int cont_line = 0;
	int cont_col = 0;
	bool titulo_linea;
	int nombre_linea;
	bool estados = false;
	bool aceptacion = false;
	bool inicial = false;
	bool alfabeto = false;
	int n_caminos = 0;
	int n_datos_conjunto = 0;
	bool igual_linea = false;
	int conjunto_abierto = 0;
	string cadena = "";
	bool unico_dato = false;
	int f_abierto = 0;
	bool dato_valido = false;
	bool epsilon = false;
	int datos_f = 0;
	cout<<"Ingresar la ruta del archivo (ejemplo:  C:\\Directorio\\Directorio\\Directorio\\nombre_archivo.txt )"<<endl<<endl<<"Ingresar:";
	getline(cin,ruta);
	direccion = ruta;
	ifstream archivo(ruta.c_str());
	
	if(archivo == NULL){
		cout<<"Error de apertura del archivo"<<endl;
		system ("pause");
		exit(0);
	}else{		
		while (archivo.good()){
			cont_line++;
			getline(archivo,line);
			titulo_linea = false;
			unico_dato = false;
			n_datos_conjunto = 0;
			nombre_linea = 0;
			igual_linea = false;
			unico_dato = false;
			conjunto_abierto = 0;
			f_abierto = 0;
			dato_valido = false;
			epsilon = false;
			datos_f = 0;
			for(int i = 0 ; i < line.length() ; i++ ){
				cont_col++;
				if(!titulo_linea){
					if(line[i] == 'S'){
						if(line[i] == 'S' && line[i+1] == '0'){
							if(inicial){
								error_conjunto(cont_line,i);
							}else{
								i++;
								datos[1][0] = "S0";
								nombre_linea= 2;
								titulo_linea = true;
								n_datos_conjunto++;
								inicial = true;
							}
						}else{
							if(estados){
								error_conjunto(cont_line,i);
							}else{
								datos[0][0] = "S";
								nombre_linea= 1;
								titulo_linea = true;
								n_datos_conjunto++;
								estados = true;
							}
						}
					}else if(line[i] == 'T'){
						if(aceptacion){
							error_conjunto(cont_line,i);
						}else{
							datos[2][0] = "T";
							nombre_linea= 3;
							titulo_linea = true;
							n_datos_conjunto++;
							aceptacion = true;
						}
					}else if(line[i] == 'A'){
						if(alfabeto){
							error_conjunto(cont_line,i);
						}else{
							datos[3][0] = "A";
							nombre_linea= 4;
							titulo_linea = true;
							n_datos_conjunto++;
							alfabeto = true;
						}
					}else if(line[i] == 'F'){
						titulo_linea = true;
						caminos[n_caminos][0] = "F";
						nombre_linea = 5;
						n_datos_conjunto++;
					}else if(line[i] == ' ' || line[i] == '	'){
						
					}else{
						error_datos(cont_line,i);
					}
				}else{
					switch(nombre_linea){
						case 1:
							if(!es_espacio(i,line)){
								if(line[i] == '=' && igual_linea == false){
									igual_linea = true;
								}else if(igual_linea && line[i] == '{' && conjunto_abierto == 0){
									conjunto_abierto++;
								}else if(conjunto_abierto == 1 && line[i] != ',' && ((line[i]>='a' && line[i]<='z') || (line[i]>='A' && line[i]<='Z') || (line[i]>='0' && line[i]<='9')) ){
									cadena = cadena + line[i];
									dato_valido = true;
								}else if(line[i] == ',' && dato_valido){
									datos[0][n_datos_conjunto] = cadena;
									n_datos_conjunto++;
									cadena = "";
									dato_valido = false;
								}else if(conjunto_abierto == 1 && line[i] == '}' && dato_valido){
									datos[0][n_datos_conjunto] = cadena;
									conjunto_abierto++;
									n_datos_conjunto++;
									cadena = "";
									dato_valido = false;
								}else{
									error_caracter(cont_line,i);
								}
								if((i+1) == line.length() && conjunto_abierto != 2){
									error_caracter(cont_line,i);
								}
							}else if(es_espacio && conjunto_abierto == 1){
								adelante = 0;
								atras = 0;
								x = i;
								while(adelante == 0){
									if(!es_espacio(x,line) && ((line[x]>='a' && line[x]<='z') || (line[x]>='A' && line[x]<='Z') || (line[x]>='0' && line[x]<='9'))){
										adelante = 1;
									}else if(!es_espacio(x,line) && line[x] == ','){
										adelante = 2;
									}
									x++;
								}
								x = i;
								while(atras == 0){
									if(!es_espacio(x,line) && ((line[x]>='a' && line[x]<='z') || (line[x]>='A' && line[x]<='Z') || (line[x]>='0' && line[x]<='9'))){
										atras = 1;
									}else if(!es_espacio(x,line) && line[x] == ','){
										atras = 2;
									}
									x--;
								}
								if(adelante == 1 && atras == 1){
									cadena = cadena + line[i];
									dato_valido = true;
								}
							}
							break;
						case 2:
							if(!es_espacio(i,line) && unico_dato == false){
								if(line[i] == '=' && igual_linea == false){
									igual_linea = true;
								}else if(igual_linea && line[i] == '{' && conjunto_abierto == 0){
									conjunto_abierto++;
								}else if(conjunto_abierto == 1 && line[i] != ',' && ((line[i]>='a' && line[i]<='z') || (line[i]>='A' && line[i]<='Z') || (line[i]>='0' && line[i]<='9')) ){
									cadena = cadena + line[i];
									dato_valido = true;
								}else if(conjunto_abierto == 1 && line[i] == '}' && dato_valido){
									datos[1][n_datos_conjunto] = cadena;
									conjunto_abierto++;
									n_datos_conjunto++;
									cadena = "";
									unico_dato = true;
									dato_valido = false;
								}else if(line[i] == ','){
									error_unico(cont_line,i);
								}else{
									error_caracter(cont_line,i);
								}
								if((i+1) == line.length() && conjunto_abierto != 2){
									error_caracter(cont_line,i);
								}
							}else if(es_espacio && conjunto_abierto == 1){
								adelante = 0;
								atras = 0;
								x = i;
								while(adelante == 0){
									if(!es_espacio(x,line) && ((line[x]>='a' && line[x]<='z') || (line[x]>='A' && line[x]<='Z') || (line[x]>='0' && line[x]<='9'))){
										adelante = 1;
									}else if(!es_espacio(x,line) && line[x] == ','){
										adelante = 2;
									}
									x++;
								}
								x = i;
								while(atras == 0){
									if(!es_espacio(x,line) && ((line[x]>='a' && line[x]<='z') || (line[x]>='A' && line[x]<='Z') || (line[x]>='0' && line[x]<='9'))){
										atras = 1;
									}else if(!es_espacio(x,line) && line[x] == ','){
										atras = 2;
									}
									x--;
								}
								if(adelante == 1 && atras == 1){
									cadena = cadena + line[i];
								}
							}
							break;
						case 3:
							if(!es_espacio(i,line)){
								if(line[i] == '=' && igual_linea == false){
									igual_linea = true;
								}else if(igual_linea && line[i] == '{' && conjunto_abierto == 0){
									conjunto_abierto++;
								}else if(conjunto_abierto == 1 && line[i] != ',' && ((line[i]>='a' && line[i]<='z') || (line[i]>='A' && line[i]<='Z') || (line[i]>='0' && line[i]<='9')) ){
									cadena = cadena + line[i];
									dato_valido = true;
								}else if(line[i] == ',' && dato_valido){
									datos[2][n_datos_conjunto] = cadena;
									n_datos_conjunto++;
									cadena = "";
									dato_valido = false;
								}else if(conjunto_abierto == 1 && line[i] == '}' && dato_valido){
									datos[2][n_datos_conjunto] = cadena;
									conjunto_abierto++;
									n_datos_conjunto++;
									cadena = "";
									unico_dato = true;
									dato_valido = false;
								}else{
									error_caracter(cont_line,i);
								}
								if((i+1) == line.length() && conjunto_abierto != 2){
									error_caracter(cont_line,i);
								}
							}else if(es_espacio && conjunto_abierto == 1){
								adelante = 0;
								atras = 0;
								x = i;
								while(adelante == 0){
									if(!es_espacio(x,line) && ((line[x]>='a' && line[x]<='z') || (line[x]>='A' && line[x]<='Z') || (line[x]>='0' && line[x]<='9'))){
										adelante = 1;
									}else if(!es_espacio(x,line) && line[x] == ','){
										adelante = 2;
									}
									x++;
								}
								x = i;
								while(atras == 0){
									if(!es_espacio(x,line) && ((line[x]>='a' && line[x]<='z') || (line[x]>='A' && line[x]<='Z') || (line[x]>='0' && line[x]<='9'))){
										atras = 1;
									}else if(!es_espacio(x,line) && line[x] == ','){
										atras = 2;
									}
									x--;
								}
								if(adelante == 1 && atras == 1){
									cadena = cadena + line[i];
								}
							}
							break;
						case 4:
							if(!es_espacio(i,line)){
								if(line[i] == '=' && igual_linea == false){
									igual_linea = true;
								}else if(igual_linea && line[i] == '{' && conjunto_abierto == 0){
									conjunto_abierto++;
								}else if(conjunto_abierto == 1 && line[i] != ',' && ((line[i]>='a' && line[i]<='z') || (line[i]>='A' && line[i]<='Z') || (line[i]>='0' && line[i]<='9')) ){
									cadena = cadena + line[i];
									dato_valido = true;
								}else if(line[i] == ',' && dato_valido){
									datos[3][n_datos_conjunto] = cadena;
									n_datos_conjunto++;
									cadena = "";
									dato_valido = false;
								}else if(conjunto_abierto == 1 && line[i] == '}' && dato_valido){
									datos[3][n_datos_conjunto] = cadena;
									conjunto_abierto++;
									n_datos_conjunto++;
									cadena = "";
									dato_valido = false;
								}else{
									error_caracter(cont_line,i);
								}
								if((i+1) == line.length() && conjunto_abierto != 2){
									error_caracter(cont_line,i);
								}
							}else if(es_espacio && conjunto_abierto == 1){
								adelante = 0;
								atras = 0;
								x = i;
								while(adelante == 0){
									if(!es_espacio(x,line) && ((line[x]>='a' && line[x]<='z') || (line[x]>='A' && line[x]<='Z') || (line[x]>='0' && line[x]<='9'))){
										adelante = 1;
									}else if(!es_espacio(x,line) && line[x] == ','){
										adelante = 2;
									}
									x++;
								}
								x = i;
								while(atras == 0){
									if(!es_espacio(x,line) && ((line[x]>='a' && line[x]<='z') || (line[x]>='A' && line[x]<='Z') || (line[x]>='0' && line[x]<='9'))){
										atras = 1;
									}else if(!es_espacio(x,line) && line[x] == ','){
										atras = 2;
									}
									x--;
								}
								if(adelante == 1 && atras == 1){
									cadena = cadena + line[i];
								}
							}
							break;
						case 5:
							if(!es_espacio(i,line) && datos_f < 2){
								if(line[i] == '('){
									f_abierto++;
								}else if(line[i] == '$'){
									epsilon = true;
									dato_valido = true;
								}else if(f_abierto == 1 && line[i] != ',' && ((line[i]>='a' && line[i]<='z') || (line[i]>='A' && line[i]<='Z') || (line[i]>='0' && line[i]<='9')) && !epsilon){
									cadena = cadena + line[i];
									dato_valido = true;
								}else if(line[i] == ',' && dato_valido && !epsilon){
									caminos[n_caminos][n_datos_conjunto] = cadena;
									datos_f++;
									n_datos_conjunto++;
									cadena = "";
									dato_valido = false;
								}else if(line[i] == ',' && dato_valido && epsilon){
									caminos[n_caminos][n_datos_conjunto] = "$";
									datos_f++;
									n_datos_conjunto++;
									cadena = "";
									dato_valido = false;
								}else if(line[i] == ')' && epsilon){
									caminos[n_caminos][n_datos_conjunto] = "$";
									datos_f++;
									n_datos_conjunto++;
									cadena = "";
									dato_valido = false;
									f_abierto++;
								}else if(line[i] == ')' && !epsilon){
									caminos[n_caminos][n_datos_conjunto] = cadena;
									datos_f++;
									n_datos_conjunto++;
									cadena = "";
									dato_valido = false;
									f_abierto++;
								}else{
									error_caracter(cont_line,i);
								}
							}else if(es_espacio && f_abierto == 1){
								adelante = 0;
								atras = 0;
								x = i;
								while(adelante == 0){
									if(!es_espacio(x,line) && ((line[x]>='a' && line[x]<='z') || (line[x]>='A' && line[x]<='Z') || (line[x]>='0' && line[x]<='9'))){
										adelante = 1;
									}else if(!es_espacio(x,line) && line[x] == ','){
										adelante = 2;
									}
									x++;
								}
								x = i;
								while(atras == 0){
									if(!es_espacio(x,line) && ((line[x]>='a' && line[x]<='z') || (line[x]>='A' && line[x]<='Z') || (line[x]>='0' && line[x]<='9'))){
										atras = 1;
									}else if(!es_espacio(x,line) && line[x] == ','){
										atras = 2;
									}
									x--;
								}
								if(adelante == 1 && atras == 1){
									cadena = cadena + line[i];
								}
							}else if(!es_espacio(i,line) && datos_f == 2){
								if(line[i] == '=' && igual_linea == false){
									igual_linea = true;
								}else if(igual_linea && line[i] == '{' && conjunto_abierto == 0){
									conjunto_abierto++;
								}else if(conjunto_abierto == 1 && line[i] != ',' && ((line[i]>='a' && line[i]<='z') || (line[i]>='A' && line[i]<='Z') || (line[i]>='0' && line[i]<='9')) ){
									cadena = cadena + line[i];
									dato_valido = true;
								}else if(line[i] == ',' && dato_valido){
									caminos[n_caminos][n_datos_conjunto] = cadena;
									n_datos_conjunto++;
									cadena = "";
									dato_valido = false;
								}else if(conjunto_abierto == 1 && line[i] == '}' && dato_valido){
									caminos[n_caminos][n_datos_conjunto] = cadena;
									conjunto_abierto++;
									n_datos_conjunto++;
									cadena = "";
									n_caminos++;
									dato_valido = false;
								}else{
									error_caracter(cont_line,i);
								}
								if((i+1) == line.length() && conjunto_abierto != 2){
									error_caracter(cont_line,i);
								}
							}else if(es_espacio && conjunto_abierto == 1){
								adelante = 0;
								atras = 0;
								x = i;
								while(adelante == 0){
									if(!es_espacio(x,line) && ((line[x]>='a' && line[x]<='z') || (line[x]>='A' && line[x]<='Z') || (line[x]>='0' && line[x]<='9'))){
										adelante = 1;
									}else if(!es_espacio(x,line) && line[x] == ','){
										adelante = 2;
									}
									x++;
								}
								x = i;
								while(atras == 0){
									if(!es_espacio(x,line) && ((line[x]>='a' && line[x]<='z') || (line[x]>='A' && line[x]<='Z') || (line[x]>='0' && line[x]<='9'))){
										atras = 1;
									}else if(!es_espacio(x,line) && line[x] == ','){
										atras = 2;
									}
									x--;
								}
								if(adelante == 1 && atras == 1){
									cadena = cadena + line[i];
									dato_valido = true;
								}
							}
							break;
						default:
							error_general(cont_line,i);
							break;	
					}
				}
			}
		}		
	}
	archivo.close();
	cout<<endl<<"Archivo valido para conversion."<<endl<<endl;
	return true;
}

void error_general(int linea,int columna){
	cout<<"Error en la linea " << linea <<" y columna " << columna + 1 <<endl;
	getch();
	exit(0);
}

void error(){
	cout<<"Error... "<<endl;
	getch();
	exit(0);
}

void error_conjunto(int linea,int columna){
	cout<<"Error en la linea " << linea <<" y columna " << columna + 1 << " Este conjunto ya ha sido ingresado"<<endl;
	getch();
	exit(0);
}

void error_datos(int linea,int columna){
	cout<<"Error en la linea " << linea <<" y columna " << columna + 1 << " Nombre de conjunto no valido"<<endl;
	getch();
	exit(0);
}

void error_caracter(int linea,int columna){
	cout<<"Caracter no valido encontrado en linea " << linea <<" y columna " << columna + 1 << "."<<endl;
	getch();
	exit(0);
}

void error_unico(int linea,int columna){
	cout<<"Error en la linea " << linea <<" y columna " << columna + 1 << " en este conjunto solo se acepta un dato."<<endl;
	getch();
	exit(0);
}

void imprimir_datos(){
	int j;
	for(int i = 0; i<4;i++){
		cout<<datos[i][0];
		j = 1;
		while(datos[i][j] != ""){
			cout<<","<<datos[i][j];
			j++;
		}
		cout<<endl;
	}
}

void imprimir_caminos(){
	int j;
	for(int i = 0; i<50;i++){
		if(caminos[i][0] == "F"){
			cout<<caminos[i][0];
			j = 1;
			while(caminos[i][j] != ""){
				cout<<","<<caminos[i][j];
				j++;
			}
			cout<<endl;
		}
	}
}

bool es_espacio(int i, string linea){
	if(linea[i] == ' ' || linea[i] == '	'){
		return true;
	}else{
		return false;
	}
}

int obtener_n_caminos(){
	int x = 0;
	for(int i = 0; i<50;i++){
		if(caminos[i][0] == "F"){
			x++;
		}
	}
	return x;
}

void agregar_epsilon(int estado, string estados[][100]){
	int x,cont = 0,agregado = 0;
	while(estados[estado][cont] != ""){
		cont++;
	}
	agregado = cont;
	for(int i = 1; i<cont;i++){
		for(int j = 0; j<obtener_n_caminos();j++){
			if((caminos[j][1] == estados[estado][i]) && (caminos[j][2] == "$")){
				x = 3;
				while(caminos[j][x] != ""){
					if(!dato_repetido(caminos[j][x],estado)){
						estados[estado][agregado] = caminos[j][x];
						agregado++;
						agregar_epsilon(estado, estados);
					}
					x++;
				}
			}
		}
	}
}

bool dato_repetido(string dato, int posicion){
	int i = 1;
	while(estados_afd[posicion][i] != ""){
		if(dato == estados_afd[posicion][i]){
			return true;
		}
		i++;
	}
	return false;
}

bool dato_repetido_conjunto(string conjunto_pre[][100], string dato){
	int i = 1;
	while(conjunto_pre[0][i] != ""){
		if(dato == conjunto_pre[0][i]){
			return true;
		}
		i++;
	}
	return false;
}

void imprimir_estados_afd(){
	int j;
	for(int i = 0; i<30;i++){
		if(estados_afd[i][0] != ""){
			cout<<estados_afd[i][0];
			j = 1;
			while(estados_afd[i][j] != ""){
				cout<<","<<estados_afd[i][j];
				j++;
			}
			cout<<endl;
		}
		
	}
}

void imprimir_tabla_afd(){
	int j;
	for(int i = 0; i<30;i++){
		if(tabla_afd[i][0] != ""){
			cout<<tabla_afd[i][0];
			j = 1;
			while(tabla_afd[i][j] != ""){
				cout<<","<<tabla_afd[i][j];
				j++;
			}
			cout<<endl;
		}
		
	}
}

void ordenar_conjunto(int estado){
	string temp;
	int datos_conjunto = 1;
	while(estados_afd[estado][datos_conjunto] != ""){
		datos_conjunto++;
	}
	for(int i = 1; i < datos_conjunto;i++){
		for(int j = i; j < datos_conjunto;j++){
			if(estados_afd[estado][i] > estados_afd[estado][j]){
				temp = estados_afd[estado][i];
				estados_afd[estado][i] = estados_afd[estado][j];
				estados_afd[estado][j] = temp;
			}
		}
	}
}

int longitud_conjunto(string conjunto[][100], int dato){
	int i = 0;
	while(conjunto[dato][i] != ""){
		i++;
	}
	return i;
}

bool pre_existe(string conjunto_pre[][100]){
	int i  = 0;
	int j  = 0;
	int x = 0;
	while(estados_afd[i][0] != ""){
		x = 0;
		if(longitud_conjunto(conjunto_pre,0) == ((longitud_conjunto(estados_afd,i)-1))){
			j = 0;
			while(j < longitud_conjunto(conjunto_pre,0)){
				if(conjunto_pre[0][j] == estados_afd[i][j+1]){
					x++;
					if(x == longitud_conjunto(conjunto_pre,0)){
						return true;
					}
				}
				j++;
			}
		}
		i++;
	}
	return false;
}

string obtener_letra(string conjunto[][100]){
	int i  = 0;
	int j  = 0;
	int x = 0;
	while(estados_afd[i][0] != ""){
		x = 0;
		if(longitud_conjunto(conjunto,0) == ((longitud_conjunto(estados_afd,i)-1))){
			j = 0;
			while(j < longitud_conjunto(conjunto,0)){
				if(conjunto[0][j] == estados_afd[i][j+1]){
					x++;
					if(x == longitud_conjunto(conjunto,0)){
						return estados_afd[i][0];
					}
				}
				j++;
			}	
		}
		i++;
	}
	return "X1X";	
}

string ingresar_conjunto(string conjunto[][100]){
	int i = 0;
	int j = 0;
	int ascii = 65;
	
	while(estados_afd[i][0] != ""){
		i++;
	}
	ascii= ascii + i;
	string a;
	a = ascii;
	estados_afd[i][0] = a;
	while(conjunto[0][j] != ""){
		estados_afd[i][j+1] = conjunto[0][j];
		j++;
	}
	return a;
}

bool pre_vacio(string conjunto_pre[][100]){
	bool validador = true;
	int i = 0;
	while(conjunto_pre[0][i] != ""){
		validador = false;
		i++;
	}
	return validador;
}

int elementos_tabla(){
	int i = 0;
	while(tabla_afd[i][0] != ""){
		i++;
	}
	return i;
}

string verificacion_de_conjunto(int estado, string alfa){
	int x = 0;
	int cont = 0;
	int agregado = 0;
	string temp;
	string conjunto_pre[1][100];
	
	while(estados_afd[estado][cont] != ""){
		cont++;
	}
	
	for(int i = 1; i<cont;i++){
		for(int j = 0; j<obtener_n_caminos();j++){
			if((caminos[j][1] == estados_afd[estado][i]) && (caminos[j][2] == alfa)){
				x = 3;
				while(caminos[j][x] != ""){
					if(!dato_repetido_conjunto(conjunto_pre,caminos[estado][x])){
						conjunto_pre[0][agregado] = caminos[j][x];
						agregado++;
					}
					x++;
				}
			}
		}
	}
	
	
	epsilon:
	x = 0;
	cont = 0;
	agregado = 0;
	while(conjunto_pre[0][cont] != ""){
		cont++;
	}
	agregado = cont;
	for(int i = 0; i<cont;i++){
		for(int j = 0; j<obtener_n_caminos();j++){
			if((caminos[j][1] == conjunto_pre[0][i]) && (caminos[j][2] == "$")){
				x = 3;
				while(caminos[j][x] != ""){
					if(!dato_repetido_conjunto(conjunto_pre,caminos[j][x])){
						conjunto_pre[0][agregado] = caminos[j][x];
						agregado++;
						goto epsilon;
					}
					x++;
				}
			}
		}
	}
	
	agregado = longitud_conjunto(conjunto_pre, 0);
	int datos_conjunto = 0;
	while(conjunto_pre[0][datos_conjunto] != ""){
		datos_conjunto++;
	}
	for(int i = 0; i < datos_conjunto;i++){
		for(int j = i; j < datos_conjunto;j++){
			if(conjunto_pre[0][i] > conjunto_pre[0][j]){
				temp = conjunto_pre[0][i];
				conjunto_pre[0][i] = conjunto_pre[0][j];
				conjunto_pre[0][j] = temp;
			}
		}
	}
	if(pre_vacio(conjunto_pre)){
		return "-";
	}else if(!pre_existe(conjunto_pre)){
		string letra = ingresar_conjunto(conjunto_pre);
		tabla_afd[elementos_tabla()][0] = letra;
		return letra;
	}else{
		return obtener_letra(conjunto_pre); 
	}
}

string afd_S(){
	string S = "S		= {";
	int i = 0;
	while(estados_afd[i][0] != ""){
		S = S + estados_afd[i][0];
		if(estados_afd[i+1][0] != ""){
			S = S + ",";
		}else{
			S = S + "}";
		}
		i++;
	}
	return S;
}

string afd_S0(){
	string S0 = "S0		= {";
	S0 = S0 + estados_afd[0][0] + "}";
	return S0;
}

string afd_T(){
	string T = "T		= {";
	bool primero = false;
	int i = 0;
	int j = 1;
	int k = 1;
	int num_finales = 0;
	
	while(datos[2][k] != ""){
		i = 0;
		while(estados_afd[i][0] != ""){
			j = 1;
			while(estados_afd[i][j] != ""){
				if(!primero && (estados_afd[i][j] == datos[2][k])){
					primero = true;
					T = T + estados_afd[i][0];
					ef_afd[num_finales] = estados_afd[i][0];
					num_finales++;
				}else if(primero && (estados_afd[i][j] == datos[2][k])){
					T = T + "," + estados_afd[i][0];
					ef_afd[num_finales] = estados_afd[i][0];
					num_finales++;
				}
				j++;
			}
			i++;
		}
		k++;
	}
	T = T + "}";
	return T;
}

string afd_A(){
	string A = "A		= {";
	int i = 1;
	bool primero = false;
	while(datos[3][i] != ""){
		if(!primero){
			A = A + datos[3][i];
			primero = true;
		}else{
			A = A + "," + datos[3][i];
		}
		i++;
	}
	A = A + "}";
	return A;
}

string afd_F(){
	string F = "";
	int i = 0;
	int j = 1;
	while(tabla_afd[i][0] != ""){
		j = 1;
		while(datos[3][j] != ""){
			if(tabla_afd[i][j] != "-"){
				F = F + "F(" + tabla_afd[i][0] + "," + datos[3][j] + ")		" + "= {" + tabla_afd[i][j] + "}\n"; 
			}
			j++;
		}
		i++;
	}
	return F;
}

string obtener_ruta_nueva(){
	string ruta = "";
	int i = 0;
	int j = 0;
	int ascii;
	int cont = 0;
	int n_directorios = 0;
	while(i < direccion.length()){
		ascii = direccion[i];
		if(ascii == 92){
			n_directorios++;
		}
		i++;
	}
	while(j < direccion.length()){
		ascii = direccion[j];
		if(ascii == 92){
			cont++;
		}
		ruta = ruta + direccion[j];
		if(cont == n_directorios){
			ruta = ruta + "AFD.txt";
			j = direccion.length();
		}
		
		j++;
	}
	return ruta;
}

void crear_AFDTXT(){
	ofstream file;
	string ruta = obtener_ruta_nueva();
  	file.open(ruta.c_str());
  	file<<afd_S()<<"\n";
  	file<<afd_S0()<<"\n";
  	file<<afd_T()<<"\n";
  	file<<afd_A()<<"\n";
  	file<<afd_F()<<"\n";
  	file.close();
}

int obtener_posicion(char cadena){
	int i = 1;
	while(cadena != datos[3][i][0]){
		i++;
	}
	return i;
}

int obtener_indice_de_letra(string actual){
	int i = 0;
	string letra = "";
	while(tabla_afd[i][0] != ""){
		if(actual == tabla_afd[i][0]){
			return i;
		}
		i++;
	}
	return 100000;
}

string validar_cadena(string cadena){
	int j = 0;
	bool valida = false;
	// conjunto de estados finales ef_afd[10]
	int longitud = cadena.length();
	int posicion;
	int anterior;
	int x = 0;
	int y = 0;
	string inicial = tabla_afd[x][y];
	string actual = tabla_afd[x][y];
	for(int i = 0; i<longitud;i++){
		posicion = obtener_posicion(cadena[i]); 
		x = obtener_indice_de_letra(actual);
		anterior = x;
		actual = tabla_afd[anterior][posicion];
		if(actual == "-"){
			valida = false;
			goto fin;
		}
	}
	while(ef_afd[j] != ""){
		if(actual == ef_afd[j]){
			valida = true;
		}
		j++;
	}
	fin:
	if(valida){
		return "La cadena ingreada es valida en el AFD.";
	}else{
		return "La cadena ingreada no es valida en el AFD.";
	}
}

void cadenas(){
	string cadena;
	int x = 0;
	int i = 1;
	char enter = 13;
	char esc = 27;
	char del = 8;
	int z;
	cout<<endl<<"Ingrese una cadena para verificar si se reconoce en el AFD creado o presione Esc para salir."<<endl<<endl;
	while(x != esc){
		x = getch();
		i = 1;
		while(datos[3][i] != ""){
			z = (char)datos[3][i][0];
			if(x == z){
				cadena = cadena + (char)x;	
				cout<<endl<<endl;
				cout<<"\e[A"<<"\e[A";;
				cout<<cadena;
			}
			i++;
		}
		if(x == enter){
			if(cadena != ""){
				cout<<endl<<endl<<validar_cadena(cadena)<<endl;
				cadena = "";
				cout<<endl<<"Ingrese una cadena para verificar si se reconoce en el AFD creado o presione Esc para salir."<<endl<<endl;
			}else{
				cout<<endl<<"Debe ingresar una cadena o presionar Esc para salir."<<endl<<endl;
			}
		}else if(x == del){
			if(cadena != ""){
				cadena.erase(cadena.length()-1);
				cout<<endl<<endl;
				cout<<"\e[A"<<"\e[A";
				cout<<"                                                                                "<<endl;
				cout<<"\e[A";
			}
			cout<<cadena;
		}
	}
	cout<<endl<<endl<<"Ha presionado ESC, Terminando programa..."<<endl;
	exit(0);
}


