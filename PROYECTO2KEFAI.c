#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define minFirmas 3000
#define MaxCantidadArticulos 100

struct Estado {
    struct Ciudadanos *ciudadanos;
    struct CongresoNacional *congreso;
    struct Presidente *presidente;
};

struct Persona {
    char *nombreApellido;
    char *rut;
    int edad;
    int haFirmado;
};

struct Articulo {
    int num;
    char *descripcion;
};

struct nodoPersona { // Lista doblemente enlazada circular
    struct Persona *datosPersona;
    struct nodoPersona *sig, *ant;
};

struct Parlamentario {
    struct Persona *datos;
    struct nodoVoto *votos;
    int idParlamentario;
    char *periodo;
    char *partidoPolitico;
    char *especialidad;
};

struct nodoParlamentario { // Lista doblemente enlazada
    struct Parlamentario *datosParlamentario;
    struct nodoParlamentario *sig, *ant;
};

struct Presidente {
    struct Persona *datosPresidente;
    struct nodoProyectoLey *proyectoLey;
};

struct ProyectoLey {
    struct CongresoNacional *congreso;
    struct Comision *comision;
    int id;
    int urgencia;
    char *tipo;
    char *nombreProyecto;
    char *descripcion;
    char *camaraOrigen;
    struct Articulo **articulos;
    int votado;
    char *fecha;
    char *fechaEntradaVigencia;
};

struct nodoProyectoLey { // Es un ABB
    struct ProyectoLey *datosProyecto;
    struct nodoProyectoLey *izq, *der;
};

struct CongresoNacional {
    struct nodoProyectoLey *ley;
    struct nodoParlamentario *diputados;
    struct nodoParlamentario *senadores;
    int numDiputados;
    int numSenadores;
};

struct Comision {
    struct nodoParlamentario *miembros;
    int idComision;
    char *tipo; // Si es de hacienda, educacion, etc
    char *tipoComision; // Si es mixta o no
};

struct Ciudadanos {
    struct nodoPersona *personas;
};

struct Voto {
    struct ProyectoLey *proyecto;
    int votado;
};

struct nodoVoto {
    struct Voto *datosVoto;
    struct nodoVoto *izq, *der;
};

struct Grafo {
    int numVertices; // vértices
    int **adyacencia; // m. de adyacencia
};

void ordenamientoShell(int numArticulos, struct Articulo **articulos) {
    struct Articulo *temp;
    int i, j, k;
    
    // iniciar salto
    k = numArticulos / 2;
    
    while (k >= 1) {
        for (i = k; i < numArticulos; i++) { // recorrer los elementos desde el salto k
            temp = articulos[i];
            j = i - k;
            
            while (j >= 0 && articulos[j]->num > temp->num) { // mover elementos mayores que temp a la derecha
                articulos[j + k] = articulos[j];
                j = j - k;
            }
            
            articulos[j + k] = temp; // insertar elemento en su posición correcta
        } 
        
        k = k / 2; // reducir el salto
    }
}

struct Persona *crearPersona(char *nombre,char *Rut,int edad, int firma) {
    struct Persona *nuevaPersona;
    nuevaPersona = (struct Persona *) malloc(sizeof(struct Persona));

    nuevaPersona->nombreApellido = (char *) malloc(strlen(nombre)*sizeof(char));
    strcpy(nuevaPersona->nombreApellido, nombre);

    nuevaPersona->rut =(char *) malloc(strlen(Rut)*sizeof(char));

    strcpy(nuevaPersona->rut,Rut);
    nuevaPersona->edad = edad;
    nuevaPersona->haFirmado = firma;

    return nuevaPersona;
}

struct nodoPersona *crearNodoPersona(struct Persona *datosPersona) {
    struct nodoPersona *nuevoNodo;

    nuevoNodo = (struct nodoPersona *) malloc(sizeof(struct nodoPersona));
    nuevoNodo->datosPersona = datosPersona;
    nuevoNodo->ant = NULL;
    nuevoNodo->sig = NULL;

    return nuevoNodo;
}
int agregarNodoPersona(struct nodoPersona **head, struct Persona *nueva) {
    struct nodoPersona *nuevoNodo, *rec;
    if(nueva==NULL) {
        return 0;
    }

    nuevoNodo = crearNodoPersona(nueva);

    if (*head == NULL) {
        *head = nuevoNodo;
        nuevoNodo->sig = nuevoNodo;
        nuevoNodo->ant = nuevoNodo;
    }else {
        rec = *head;
        do{
            rec=rec->sig;

        }while (rec->sig != *head);

        rec->sig = nuevoNodo;
        nuevoNodo->ant = rec;
        nuevoNodo->sig = *head;
        (*head)->ant = nuevoNodo;
    }
    return 1;
}


struct Parlamentario *crearParlamentario(struct Persona *datos, int id, char *periodo ,char *partido ,char *especialidad){
    struct Parlamentario *nuevoParlamentario;
    nuevoParlamentario =(struct Parlamentario *) malloc(sizeof(struct Parlamentario));

    nuevoParlamentario->datos = datos;
    nuevoParlamentario->idParlamentario = id;
    nuevoParlamentario->periodo =(char *) malloc(strlen(periodo)*sizeof(char));
    strcpy(nuevoParlamentario->periodo,periodo);

    nuevoParlamentario->partidoPolitico = (char *) malloc(strlen(partido)*sizeof(char));
    strcpy(nuevoParlamentario->partidoPolitico,partido);

    nuevoParlamentario->especialidad = (char *) malloc(strlen(especialidad)*sizeof(char));
    strcpy(nuevoParlamentario->especialidad,especialidad);
    nuevoParlamentario->votos = NULL;

    return nuevoParlamentario;
}

struct nodoParlamentario *crearNodoParlamentario(struct Parlamentario *datosParlamentario) {
    struct nodoParlamentario *nuevoNodo;

    nuevoNodo = (struct nodoParlamentario *) malloc(sizeof(struct nodoParlamentario));
    nuevoNodo->datosParlamentario = datosParlamentario;
    nuevoNodo->ant = NULL;
    nuevoNodo->sig = NULL;

    return nuevoNodo;
}

int agregarParlamentario(struct nodoParlamentario **head, struct Parlamentario *nuevo,int *numParlamentario) {
    struct nodoParlamentario *nuevoNodo ;
    struct nodoParlamentario *rec ;
    if(nuevo == NULL) {
        return 0;
    }
    nuevoNodo= crearNodoParlamentario(nuevo);
    if (*head == NULL) {
        *head = nuevoNodo;
        (*numParlamentario)++;
        return 1;
    }

    rec = *head;
    while (rec->sig != NULL) {
         if (rec->datosParlamentario->idParlamentario == nuevo->idParlamentario || strcmp(rec->datosParlamentario->datos->rut, nuevo->datos->rut) == 0) {
             return 0;
         }
        rec = rec->sig;
    }

    rec->sig = nuevoNodo;
    nuevoNodo->ant = rec;
    (*numParlamentario)++;
    return 1;
}

struct ProyectoLey *crearProyectoLey(int id, char *tipo, int urgencia, char *nombre, char *descripcion, struct Articulo **articulos,char *fechaIngreso) {

    struct ProyectoLey *nuevoProyecto ;
    nuevoProyecto = (struct ProyectoLey *) malloc(sizeof(struct ProyectoLey));

    nuevoProyecto->id = id;

    nuevoProyecto->tipo = (char *)malloc((strlen(tipo) + 1) * sizeof(char));
    strcpy(nuevoProyecto->tipo, tipo);

    nuevoProyecto->nombreProyecto = (char *)malloc((strlen(nombre) + 1) * sizeof(char));
    strcpy(nuevoProyecto->nombreProyecto, nombre);

    nuevoProyecto->urgencia = urgencia;

    nuevoProyecto->descripcion = (char *)malloc((strlen(descripcion) + 1) * sizeof(char));
    strcpy(nuevoProyecto->descripcion, descripcion);

    nuevoProyecto->camaraOrigen = (char *)malloc(2*sizeof(char));
    strcpy(nuevoProyecto->camaraOrigen , "N"); // Se le asigna N como Forma de Decir que no Tiene Camara de Origen Todavia

    nuevoProyecto->fecha = (char *) malloc(strlen(fechaIngreso)*sizeof(char));
    strcpy(nuevoProyecto->fecha,fechaIngreso);

    nuevoProyecto->articulos = articulos;

    nuevoProyecto->comision = NULL;

    return nuevoProyecto;
}


struct Articulo *crearArticulo(int numero,char *descripcion) {
    struct Articulo *nuevoArticulo;

    nuevoArticulo =(struct Articulo *) malloc(sizeof(struct Articulo));
    nuevoArticulo->num = numero;
    nuevoArticulo->descripcion = (char *) malloc(strlen(descripcion)*sizeof(char));
    strcpy(nuevoArticulo->descripcion,descripcion);

    return nuevoArticulo;
}

int buscarParlamentario(struct nodoParlamentario *head,char *rutPersona,int idParlamentario) {

    struct nodoParlamentario *rec;

    if(head!=NULL) {

        rec = head;

        while(rec!=NULL) {

            if(rec->datosParlamentario->idParlamentario == idParlamentario && strcmp(rec->datosParlamentario->datos->rut,rutPersona)== 0)
                return 1;

        rec=rec->sig;

        }
    }
    return 0;

}

struct nodoProyectoLey *buscarProyectoLey(struct nodoProyectoLey *arbol,struct nodoProyectoLey **ant,int idBuscado) {

    int enc=0;

    while(enc==0 && arbol!=NULL) {

        if(arbol->datosProyecto->id==idBuscado)
            enc++;

        else {

            *ant = arbol;

            if(idBuscado<arbol->datosProyecto->id)
                arbol= arbol->izq;

            else
                arbol= arbol->der;
        }
    }
    return arbol;
}



int agregarProyectoLey(struct nodoProyectoLey **head,struct ProyectoLey *nuevoProyecto) {

    struct nodoProyectoLey *nuevo=NULL,*ant=NULL,*ptr=NULL;
    ptr = buscarProyectoLey(*head,&ant,nuevoProyecto->id);
    if(ptr==NULL) {
        nuevo =(struct nodoProyectoLey *) malloc(sizeof(struct nodoProyectoLey));
        nuevo->datosProyecto = nuevoProyecto;

        if(ant==NULL) {
            *head = nuevo;
        }else {
            if(nuevoProyecto->id<ant->datosProyecto->id) {
                ant->izq = nuevo;
            }else{
                ant->der = nuevo;

            }
        }
        return 1;
    }
    return 0;
}

int verificarSiProyectoExiste(struct nodoProyectoLey *raiz, int idBuscado) {
    struct nodoProyectoLey *actual;

    if(raiz != NULL) {
        actual = raiz;
        while (actual != NULL) {
            if (actual->datosProyecto->id == idBuscado){
                return 1;
            }
            else{
                if (idBuscado < actual->datosProyecto->id){
                    actual = actual->izq;
                }
                else{
                    actual = actual->der;
                }
            }
        }
    }
    return 0;
}

int agregarSectorProyectoLey(char *sector,int opcion) {
    char sectores[10][27] = {"Educacion", "Salud", "Reforma", "Impuesto", "Defensa",
        "Ministerio Publico", "Seguridad Social", "Presupuesto",
        "Poder Judicial", "Cooperacion Internacional"};
    int numSectores = 10;
    if(opcion >0 && opcion <=numSectores) {
        strcpy(sector, sectores[opcion - 1]);
        return 1;
    }
    return 0;
}
int contarFirmasPersonas(struct nodoPersona *head) {
    int contadorFirmas = 0; // arreglar
    struct nodoPersona *rec; // arreglar

    if(head == NULL)
        return 0;
    rec = head; // se arregla lo de arriba

    do {
        printf("El Ciudadano '%s' Firmo?\n",rec->datosPersona->nombreApellido); // se arregla con lo de arriba
        do {
            printf("1:si --- 0: no");

            scanf("%d",&rec->datosPersona->haFirmado);

        }while(rec->datosPersona->haFirmado<0 ||rec->datosPersona->haFirmado<1);
        if(rec->datosPersona->haFirmado == 1)
            contadorFirmas++;
        rec = rec->sig;

    } while (rec!=head);

    return contadorFirmas;
}

int agregarProyectoPersona(struct nodoProyectoLey **proyectos,struct ProyectoLey *nuevo, struct nodoPersona *head) {
    int totalFirmas; //arreglar
    if(head == NULL)
        return 0;

    totalFirmas = contarFirmasPersonas(head);
    if(totalFirmas >= minFirmas) {
        if(agregarProyectoLey(proyectos, nuevo) == 1)
            return 1;
        return 0;
    }
    return 0;
}

void ingresarProyectoLey(struct Estado *es, struct nodoProyectoLey **head) {
    int idProyectoley, opcion = 1, salida, i;
    int year, dia, mes;
    int urgencia;
    char fechaIngreso[13]="";
    char sector[27], nombreProyecto[70], descripcionProyecto[200], numero[6];
    struct Articulo *Articulos[MaxCantidadArticulos];
    char ArticuloDescripcion[500];
    struct ProyectoLey *nuevoProyecto;

    do {

        printf("Menu de Ingreso Proyecto de Ley\n");
        printf("Opcion 1: Salir al Menu Principal\n");
        printf("Opcion 2: Agregar nuevo Proyecto De Ley\n");
        printf("Opcion 3: Ver Proyectos de Ley\n");

        scanf("%d", &opcion);

        switch (opcion) {

            case 1: {
                printf("Saliendo Del Menu de Ingreso de Proyectos de Ley\n\n");
                break;
            }

            case 2: {

                do {

                    salida = 0;
                    printf("Ingrese el ID del Nuevo Proyecto de Ley (digito mayor a 0):\n");
                    scanf("%d", &idProyectoley);

                    if (verificarSiProyectoExiste(*head, idProyectoley) == 1) {
                        printf("El proyecto que está introduciendo ya existe\n");
                        printf("Si quiere salir al menú anterior, ingrese 1: ");
                        scanf("%d", &salida);
                        printf("\n");
                    }
                }while(verificarSiProyectoExiste(*head, idProyectoley) == 1);

                if (salida == 1) {
                    break;
                }
                printf("Ingrese el Nombre del Proyecto de Ley:\n");
                scanf(" %[^\n]", nombreProyecto);

                do {

                    printf("Ingrese de que Sector es el Proyecto de Ley \n");
                    printf("Ingrese el numero Correspondiente\n");
                    printf("1:Educacion---2:Salud---3:Reforma---4:Impuesto---5:Defensa---06:Ministerio Publico \n");
                    printf("7:Seguridad Social---8:Presupuesto---9:Poder Judicial---10:Coperacion Internacional\n");
                    scanf("%d",&salida);
                    if(agregarSectorProyectoLey(sector,salida)==0) {
                        salida = 0;
                        printf("Ingrese un Numero Valido\n");
                    }

                }while(salida==0);

                printf("Ingrese la Descripcion del Proyecto de Ley:\n");
                scanf(" %[^\n]", descripcionProyecto);

                printf("Ingrese el nivel de urgencia (1 - baja, 2 - media, 3 - alta):\n");  // Nueva solicitud para urgencia

                do {

                    scanf("%d", &urgencia);

                } while(urgencia!= 1 && urgencia!= 2 && urgencia!=3);
                salida=0;
                printf("Ingrese los Articulos que tiene el Proyecto de Ley:\n");
                for (i = 0; i < MaxCantidadArticulos && salida == 0; i++) {
                    printf("Articulo [%d]: ", i+1);
                    scanf(" %[^\n]", ArticuloDescripcion);
                    Articulos[i] = crearArticulo(i+1,ArticuloDescripcion);
                    printf("Si desea salir, ingrese 1 si no, ingrese 0: ");
                    ordenamientoShell(i, Articulos);
                    scanf("%d", &salida);

                }

                salida = 0;
                do {

                    printf("Ingrese la Fecha de ingreso del Proyecto (year-Mes-Dia)\n");
                    printf("year:");
                    scanf("%d", &year);
                    printf("Mes:");
                    scanf("%d", &mes);
                    printf("Dia:");
                    scanf("%d", &dia);

                    if (year > 0 && (mes > 0 && mes < 13) && (dia > 0 && dia <= 31)) {
                        salida = 1;
                        sprintf(numero, "%d-", year);
                        strcat(fechaIngreso, numero);
                        sprintf(numero, "%02d-", mes);
                        strcat(fechaIngreso, numero);
                        sprintf(numero, "%02d", dia);
                        strcat(fechaIngreso, numero);
                    }
                    else{
                        printf("La fecha ingresada no es valida\n");
                    }

                } while (salida == 0);
                nuevoProyecto = crearProyectoLey(idProyectoley, sector, urgencia, nombreProyecto, descripcionProyecto, Articulos,fechaIngreso);
                printf("Ingrese de Quien Propuso el proyecto De ley\n");
                printf("1:El Presidente ---- 2:La Ciudadania --- 3:Los Parlamentarios\n");
                do {
                    scanf("%d",&salida);
                    if(salida<1 || salida>3) {
                        printf("Ingrese un numero Valido\n\n");
                    }
                }while(salida<1 || salida>3);
                if(salida == 2) {
                    printf("Comprobando si tiene Suficientes Firmas");
                    if(agregarProyectoPersona(&es->congreso->ley,nuevoProyecto,es->ciudadanos->personas)) {
                        printf("Cumple con las firmas\n");
                    }else {
                        printf("No Cumple con las firmas\n\n");
                    }
                }else {
                    if(salida==1) {
                        agregarProyectoLey(&es->congreso->ley,nuevoProyecto);
                        agregarProyectoLey(&es->presidente->proyectoLey,nuevoProyecto);
                        printf("Se agrego el Proyecto De Ley\n\n");
                    }else {
                        agregarProyectoLey(&es->congreso->ley,nuevoProyecto);
                        printf("Se agrego el Proyecto De Ley\n\n");
                    }
                }
                break;
            }
            default:
                printf("Ingrese una opción válida\n\n");
        }
    } while (opcion != 1);
}


void asignarCamaraOrigen(struct ProyectoLey *proyecto) {
    if(strcmp(proyecto->tipo, "Presupuesto") == 0 || strcmp(proyecto->tipo, "Impuesto")==0 || strcmp(proyecto->tipo, "Seguridad Social") == 0
        || strcmp(proyecto->tipo, "Salud")== 0 || strcmp(proyecto->tipo, "Educacion")==0 ){
        strcpy(proyecto->camaraOrigen ,"D"); //Asignar la camara de Diputados
    }else {
        strcpy(proyecto->camaraOrigen ,"S");//Asignar la camara de Senadores
    }
}

struct nodoVoto *crearNodoVoto(struct Voto *datos) {

    struct nodoVoto *nuevo;
    nuevo = (struct nodoVoto *) malloc(sizeof(struct nodoVoto));
    nuevo->datosVoto = datos;
    nuevo->izq = nuevo->der = NULL;
    return nuevo;

}

struct Voto *crearVoto(struct ProyectoLey * proyecto,int decision) {

    struct Voto *nuevo;
    nuevo =(struct Voto *) malloc(sizeof(struct Voto ));
    nuevo->proyecto = proyecto;
    nuevo->votado = decision;
    return nuevo;

}

int buscarVoto(struct nodoVoto *votos,struct ProyectoLey *proyecto) {

    while(votos!=NULL) {

        if(votos->datosVoto->proyecto->id == proyecto->id)
            return votos->datosVoto->votado; // Retorna la decision del voto si lo encientra

        else {
            if(proyecto->id < votos->datosVoto->proyecto->id)
                votos = votos->izq; // Sigue a la izquierda si el ID del proyecto es menor
            else
                votos = votos->der; //Sigue a la derecha si el ID del proyecto es mayor

        }
    }
    return -1; // No se encontro el voto

}

int votosCamara(struct ProyectoLey *proyecto,struct nodoParlamentario *head) {

    struct nodoParlamentario *rec;
    int contarVotosPositivos = 0;

    if(head!=NULL) {

        rec= head;
        while(rec!=NULL){
            if(buscarVoto(rec->datosParlamentario->votos,proyecto)==1) {
                contarVotosPositivos++;
            }
            rec=rec->sig;
        }
    }
    return contarVotosPositivos;

}

int agregarVotoParlamentario(struct Parlamentario *parlamentario,struct ProyectoLey * proyecto,int decision) {

    struct nodoVoto *nuevo,*rec;
    struct nodoVoto *padre=NULL;

    nuevo = crearNodoVoto(crearVoto(proyecto,decision));

    if(parlamentario->votos==NULL)
        parlamentario->votos = nuevo;

    else {
        rec = parlamentario->votos;

        while(rec!=NULL) {

            padre = rec;
            if(rec->datosVoto->proyecto == proyecto) {
                // ya existe un voto para este proyecto, actualiza la decision
                rec->datosVoto->votado = decision;
                return 1;

            } else {

                if(proyecto->id < rec->datosVoto->proyecto->id)
                    rec= rec->izq;

                else
                    rec= rec->der;
            }
        }
        // insertar el nuevo voto en el lugar correcto

        if(padre!=NULL) {

            if(proyecto->id < padre->datosVoto->proyecto->id) {
                padre->izq = nuevo;
                return 1;
            }

            else {
                padre->der = nuevo;
                return 1;
            }

        }

    }
    return 0;
}

void seleccionarParlamentarioMixto(struct nodoParlamentario *head,struct Comision *comision) {
    struct nodoParlamentario *ultimo;
    struct nodoParlamentario *nuevoNodo,*rec;
    int i;
    rec = head;
    for(i=0; i<6 && rec->datosParlamentario!=NULL ;i++) {
        nuevoNodo = crearNodoParlamentario(rec->datosParlamentario);
        if(comision->miembros==NULL) {
            comision->miembros = nuevoNodo;

        }else {
            ultimo = comision->miembros;
            while(ultimo->sig!=NULL) {
                ultimo = ultimo->sig;
            }
            ultimo->sig = nuevoNodo;
            nuevoNodo->ant = ultimo;
        }
        printf("EL nuevo miembro '%s' con ID '%d'\n",nuevoNodo->datosParlamentario->datos->nombreApellido,nuevoNodo->datosParlamentario->idParlamentario);
        printf("Se unio a la comision Mixta\n");
        rec->sig = rec;
    }
}


void formarComisionMixta(struct ProyectoLey *proyecto,struct CongresoNacional *congreso) {
    struct Comision *comisionMixta;
    comisionMixta = (struct Comision *) malloc(sizeof(struct Comision));
    comisionMixta->tipoComision = "Mixta";
    comisionMixta->tipo = proyecto->tipo; //Asignar el tipo Segun el proyecto de ley
    comisionMixta->miembros = NULL;

    //Seleccion de parlamentarios
    seleccionarParlamentarioMixto(congreso->diputados,comisionMixta);
    seleccionarParlamentarioMixto(congreso->senadores,comisionMixta);
    proyecto->comision = comisionMixta;
    printf("Se ha formado una Comision Mixta para el proyecto '%s'.\n",proyecto->nombreProyecto);

}

void asignarComision(struct ProyectoLey *proyecto,struct nodoParlamentario *head,struct Comision *comision) {
    struct nodoParlamentario *ultimo;
    struct nodoParlamentario *nuevoNodo,*rec;

    rec = head;
    while(rec!=NULL){
        if(rec->datosParlamentario->especialidad !=NULL && strcmp(rec->datosParlamentario->especialidad,proyecto->tipo) ==0) {
            nuevoNodo = crearNodoParlamentario(rec->datosParlamentario);
            if(comision->miembros == NULL) {
                comision->miembros = nuevoNodo;
            }else {
                ultimo= comision->miembros;
                while(ultimo->sig!=NULL) {
                    ultimo = ultimo->sig;
                }
                ultimo->sig = nuevoNodo;
                nuevoNodo->ant = ultimo;
            }
            printf("EL nuevo miembro es '%s' con ID '%d'\n",nuevoNodo->datosParlamentario->datos->nombreApellido,nuevoNodo->datosParlamentario->idParlamentario);
        }
        rec= rec->sig;
    }
}

void listarComision(struct Comision *comision) {
struct nodoParlamentario *miembro;
    if (comision == NULL) {
        printf("No hay comisión registrada.\n");
        return;
    }
    printf("Comisión Tipo: %s, Tipo de Comisión: %s\n", 
        comision->tipo, 
        comision->tipoComision);
    miembro = comision->miembros;
    printf("Miembros:\n");
    while (miembro != NULL) {
        printf(" - %s (ID: %d)\n", 
            miembro->datosParlamentario->datos->nombreApellido, 
            miembro->datosParlamentario->idParlamentario);
        miembro = miembro->sig;
    }
}

void eliminarComision(struct ProyectoLey *proyecto) {
    if (proyecto->comision == NULL) {
        printf("El proyecto no tiene una comisión asignada.\n");
        return;
    }
    free(proyecto->comision->tipo);
    free(proyecto->comision->tipoComision);
    free(proyecto->comision);
    proyecto->comision = NULL;
    printf("Comisión eliminada exitosamente.\n");
}

void agregarComision(struct ProyectoLey *proyecto, struct nodoParlamentario *head) {
    struct Comision *Nueva;
    int opcion, salir = 0;

    do {
        printf("Menu comisión\n"); 
        printf("1: Agregar Comisión\n");
        printf("2: Ver Comisión Proyecto\n");
        printf("3: Eliminar Comisión\n");
        printf("4: Salir\n");
        scanf("%d", &opcion);

        switch (opcion) {
            case 1: {
                if(head!=NULL) {
                    Nueva = (struct Comision *) malloc(sizeof (struct Comision));
                    Nueva->miembros = NULL;
                    Nueva->tipo = (char *) malloc(strlen(proyecto->tipo)*sizeof(char));
                    strcpy(Nueva->tipo,proyecto->tipo);
                    printf("El proyecto es derivado a una Comision permanente");
                    asignarComision(proyecto,head,Nueva);
                }
            }
            case 2: {
                listarComision(Nueva);
                break;
            }
            case 3: {
                eliminarComision(proyecto);
                break;
            }
            case 4: {
                printf("Saliendo del menu...\n");
                salir = 1;
                break;
            }
            default:
                printf("Ingrese una opción válida\n");
        }
    } while (salir != 1);
}

void votacionEnProceso(struct ProyectoLey *proyecto,struct nodoParlamentario *head,char *camara) {
    struct nodoParlamentario *rec;
    int decision;
    printf("Comienza la Votacion del Proyecto De Ley '%s' en la Camara de '%s'\n",proyecto->nombreProyecto,camara);
    rec = head;
    while(rec!=NULL) {
        if(strcmp(camara,"Senadores")==0) {
            printf("Senador '%s' ingrese su decision acerca del Proyecto de Ley '%s'\n",rec->datosParlamentario->datos->nombreApellido,proyecto->nombreProyecto);
        }else {
            printf("Diputado '%s' ingrese su decision acerca del Proyecto de Ley '%s'\n",rec->datosParlamentario->datos->nombreApellido,proyecto->nombreProyecto);
        }
        do {
            printf("1:A favor --- 0:abstencion --- -1:en contra\n");
            scanf("%d",&decision);
        }while(decision<-1 || decision>1);
        if(agregarVotoParlamentario(rec->datosParlamentario,proyecto,decision)==1) {
            printf("Su voto se ha guardado");
        }
        rec = rec->sig;
    }
}

int votarArticulo(struct nodoParlamentario *head) {
    struct nodoParlamentario *rec;
    int votos=0,voto;
    rec = head;
    do {
        printf("Sr(a) '%s', Ingrese su Voto 1:Aprovado----2:Rechazo\n",rec->datosParlamentario->datos->nombreApellido);
        do {
            scanf("%d",&voto);
        }while(voto<1 || voto>2);
        if(voto==1) {
            votos++;
        }
        rec = rec->sig;
    }while(rec!=NULL);

    return votos;

}

int modificarArticulo(struct ProyectoLey *proyecto,struct Articulo *articulo) {
    char nuevaDescripcion[500];
    int i;
    printf("Ingrese el Articulo modificado\n");
    scanf("%[^\n]",nuevaDescripcion);
    for(i=0;i<MaxCantidadArticulos && proyecto->articulos[i]!=NULL;i++) {
        if(proyecto->articulos[i]==articulo) {
            proyecto->articulos[i]->descripcion = (char *) malloc(strlen(nuevaDescripcion)*sizeof(char));
            return 1;
        }
    }
    return 0;
}

void eliminarArticulo(struct ProyectoLey *proyecto,int numBuscado) {
    int encontrado =0,i,k;
    for(i=0;i<MaxCantidadArticulos && proyecto->articulos[i]!=NULL;i++) {
        if(proyecto->articulos[i]->num == numBuscado) {
            encontrado =1;
            for(k=i;k<MaxCantidadArticulos-1 && proyecto->articulos[k]!=NULL;k++) {
                proyecto->articulos[k] = proyecto->articulos[k+1];
            }
            printf("EL Articulo fue Elminado\n");
        }
    }
    if(encontrado==0) {
        printf("El Articulo No pudo ser Eliminado\n");
    }
}

void votacionPorArticuloModificacion(struct ProyectoLey *proyecto,struct nodoParlamentario *head,int NumParlamentario) {
    struct Articulo *Articulos[MaxCantidadArticulos];
    int cantVotos,eleccion;
    int i,k=0;
    printf("Comienza La Votacion Por Articulo\n");
    for(i=0;i<MaxCantidadArticulos && proyecto->articulos[i]!=NULL;i++) {
        printf("El '%d'Articulo: %s",i+1,proyecto->articulos[i]->descripcion);
        cantVotos = votarArticulo(head);
        if((( 2*NumParlamentario) / 3) <=cantVotos) {
            printf("El '%d'Articulo Fue Aprobado\n",i+1);
        }else {
            printf("El '%d'Articulo Fue Rechazado\n",i+1);
            Articulos[k]= proyecto->articulos[i];
            k++;
        }
    }
    for(i=0;i<MaxCantidadArticulos && Articulos[i]!=NULL;i++) {
        printf("El '%d'Articulo Fue Rechazado Que quieren hacer con el?\n",Articulos[i]->num);
        printf("1:Modificarlo----2:Eliminarlo\n");
        do {
            scanf("%d",&eleccion);
        }while(eleccion<1 || eleccion>2);
        if(eleccion==1) {
            if(modificarArticulo(proyecto,Articulos[i])==1) {
                printf("El Articulo Se modifico correctamente\n");
            }else {
                printf("El Articulo no pudo ser modificado\n");
            }
        }else {
            eliminarArticulo(proyecto,Articulos[i]->num);
        }


    }

}



void votarProyecto(struct ProyectoLey *proyecto,struct CongresoNacional *congreso) {
    char camara[10];
    // Determina si el proyecto se aprueba segun la camara de origen
    if(strcmp(proyecto->camaraOrigen,"S")==0){

        strcpy(camara,"Senadores");
        agregarComision(proyecto,congreso->senadores); // menu comision
        votacionEnProceso(proyecto,congreso->senadores,camara);

        if((( 2*congreso->numSenadores ) / 3) <= votosCamara(proyecto,congreso->senadores)){ // Votos de la Camara de Senadores

            printf("La idea de legislar fue aprobada en el Senado.\n");
            votacionPorArticuloModificacion(proyecto,congreso->senadores,congreso->numSenadores);

            // Votacion en Camara Revisora(Diputados)
            strcpy(camara,"Diputados");
            votacionEnProceso(proyecto,congreso->diputados,camara);
            if((( 2*congreso->numDiputados) / 3) <=votosCamara(proyecto,congreso->diputados)) { // Votos de la Camara de Diputados
                printf("El proyecto '%s' ha sido aprobado por ambas camaras.\n",proyecto->nombreProyecto);
                proyecto->votado = 1; // Marca el proyecto como votado y aprobado
            }else {
                printf("El proyecto '%s' no alcanzo el quorum en la Camara de Diputado.\n",proyecto->nombreProyecto);


            }

        }else {
            printf("El proyecto '%s' no alcanzo el qiorum en el Senado.\n",proyecto->nombreProyecto);
        }
    }else {

        strcpy(camara,"Diputados");
        agregarComision(proyecto,congreso->diputados); // menu comision
        votacionEnProceso(proyecto,congreso->diputados,camara);

        if(((2*congreso->numDiputados ) /3) <= votosCamara(proyecto,congreso->diputados)) { // Votos de la Camara de Diputados

            printf("La idea de legislar fue aprobada en la camara de diputados");
            votacionPorArticuloModificacion(proyecto,congreso->senadores,congreso->numSenadores);

            strcpy(camara,"Senadores");
            votacionEnProceso(proyecto,congreso->senadores,camara);

            if(((2*congreso->numSenadores )/3) <= votosCamara(proyecto,congreso->senadores)){ // Votos de la Camara de Senadores
                printf("El pryecto '%s' ha sido aprobado por ambas camaras.\n",proyecto->nombreProyecto);
                proyecto->votado = 1; //Marca el proyecto como votado y aprobado
            }else {
                printf("El proyecto '%s' no alcanzo el quorum en el Senado.\n",proyecto->nombreProyecto);
            }
        }else {
            printf("El proyecto '%s' no alcanzo el quorum en la camara de Diputado.\n",proyecto->nombreProyecto);
        }
    }
}



//Funcion extra 1
void recorrerABB(struct nodoProyectoLey *nodo, struct nodoParlamentario *head, struct nodoProyectoLey **masVotado, int *maxVotos) {
    int votosActual;
    if (nodo == NULL)
        return;

    recorrerABB(nodo->izq, head, masVotado, maxVotos);  // In-Orden
    votosActual = votosCamara(nodo->datosProyecto,head);

    if (votosActual > *maxVotos) {
        *maxVotos = votosActual;
        *masVotado = nodo;
    }

    recorrerABB(nodo->der, head, masVotado, maxVotos);
}

struct ProyectoLey *getProyectoMasVotado(struct nodoParlamentario *head, struct nodoProyectoLey *raiz) {
    int maxVotos = -1;
    struct nodoProyectoLey *masVotado = NULL;
    struct nodoProyectoLey *rec;

    if (head == NULL || raiz == NULL)
        return NULL;
    printf("!\n");
    rec = raiz;
    recorrerABB(rec, head, &masVotado, &maxVotos);

    if (masVotado!=NULL) {
        return masVotado->datosProyecto;
    }
    return NULL;
}
//Fin funcion extra

//Funcion extra 2
int eliminarParlamentario(struct nodoParlamentario **head, int idParlamentario) {

     struct nodoParlamentario *rec = *head;

    if (*head == NULL)
        return 0;


    while(rec!=NULL &&  rec->datosParlamentario->idParlamentario != idParlamentario)
        rec = rec->sig;


    if(rec == NULL)
        return 0;

    if(rec == *head) {
        *head = rec->sig;

        if(*head != NULL)
            (*head)->ant = NULL;
    }

    else {

        if (rec->ant != NULL)
            rec->ant->sig = rec->sig;
        if (rec->sig != NULL)
            rec->sig->ant = rec->ant;
    }

    return 1;
}

struct ProyectoLey * proyectoVotando(struct nodoProyectoLey *raiz,int idBuscado){
    struct nodoProyectoLey *actual;
    actual = raiz;
    while (actual != NULL) {
        if (actual->datosProyecto->id == idBuscado){
            return actual->datosProyecto;
        }
        else{
            if (idBuscado < actual->datosProyecto->id){
                actual = actual->izq;
            }
            else{
                actual = actual->der;
            }
        }
    }
    return NULL;
}

void mostrarProyecto(struct ProyectoLey *proyecto) {
    int i;
    printf("'%s'",proyecto->nombreProyecto);
    printf("tipo: '%s'\n",proyecto->tipo);
    for(i=0;i<MaxCantidadArticulos && proyecto->articulos[i]!=NULL;i++) {
        printf("Articulo[%d]: '%s' \n",i+1,proyecto->articulos[i]->descripcion);
    }
}

void recorrerArbolProyectos(struct nodoProyectoLey *raiz) {

    if (raiz != NULL) {
        // Recorrido en orden (in-order): Izquierda, Raíz, Derecha
        recorrerArbolProyectos(raiz->izq);
        mostrarProyecto(raiz->datosProyecto);
        recorrerArbolProyectos(raiz->der);
    }
    return;
}

void comienzoVotacionCongreso(struct Estado *es){
    int opcion,idProyectoVotar,salida=0;
    struct ProyectoLey *proyectoVotado,*proyectoMasVotado;
    int year,dia,mes;
    char fechaEnVigencia[13]="";
    char numero[6];

    do{
        printf("Menu de Comienzo de votaciones Congreso Nacional\n");
        printf("Opcion 1: Salir al menu Principal\n");
        printf("Opcion 2: Comenzar La votacion \n");
        printf("Opcion 3: Mostrar Los Proyectos en el Congreso\n");
        printf("Opcion 4: Mostrar Proyecto de Ley con mas Votos positivos\n");
        scanf("%d",&opcion);
        switch (opcion){
            case 1:{
                printf("Saliendo del Menu Comienzo de votaciones Congreso Nacional\n");
                break;
            }
            case 2:{
                if(es->congreso->numDiputados!=155 && es->congreso->numSenadores!=50) {
                    printf("No puede comenzar una votacion porque alguna de las camaras no esta llena\n\n");

                }else {
                    do{
                        printf("Ingrese el ID del Proyecto De Ley a votar\n");
                        scanf("%d",&idProyectoVotar);
                        if(verificarSiProyectoExiste(es->congreso->ley,idProyectoVotar)==0){
                            printf("No se encontro el Proyecto de Ley\n");
                            printf("Si quiere Salir al menu Anterior ingrese 2 si quiere seguir ingresando un ID Ingrese 0 \n");
                            scanf("%d",&salida);
                        }else{
                            break;
                        }
                    }while(salida!=2);
                    if(salida==2){
                        break;
                    }
                    proyectoVotado = proyectoVotando(es->congreso->ley,idProyectoVotar);
                    printf("Comienza La Votacion del Proyecto con el ID : '%d'\n",idProyectoVotar);
                    votarProyecto(proyectoVotado,es->congreso);
                    if(proyectoVotado->votado==1){
                        salida = 0;
                        printf("El Proyecto de Ley es enviado al presidente de la republica '%s'\n",es->presidente->datosPresidente->nombreApellido);
                        printf("El proyecto '%s' Fue Aprobado o Vetado por el Presidente?\n",proyectoVotado->nombreProyecto);
                        printf("Ingrese el numero correspondiente\n");
                        printf("1:Aprobado---2:Vetado---3:Veto Parcial \n");
                        do {
                            scanf("%d",&salida);
                        }while(salida<1 || salida>3);
                        if(salida==1) {
                            salida= 0;
                            do {
                                printf("Ingrese la Fecha de entrada en vigencia del Proyecto (year-Mes-Dia)\n");
                                printf("year:");
                                scanf("%d",&year);
                                printf("\nMes:");
                                scanf("%d",&mes);
                                printf("\nDia:");
                                scanf("%d",&dia);
                                if(year>0 && (mes>0 && mes < 13) && (dia>0 && dia <= 31)) {
                                    salida = 1;
                                    sprintf(numero,"%d-",year);
                                    strcat(fechaEnVigencia,numero );
                                    sprintf(numero,"%02d-",mes);
                                    strcat(fechaEnVigencia, numero);
                                    sprintf(numero,"%02d",dia);
                                    strcat(fechaEnVigencia, numero);
                                }else {
                                    printf("La fecha Ingresada no es valida\n");
                                }
                            }while(salida==0);

                            proyectoVotado->fechaEntradaVigencia = (char *) malloc(strlen(fechaEnVigencia)*sizeof(char));
                            strcpy(proyectoVotado->fechaEntradaVigencia,fechaEnVigencia);

                            printf("El proyecto fue enviado al diario Oficial para su publicacion \n");
                            printf("Diario Oficial\n\n");
                            mostrarProyecto(proyectoVotado);
                            printf("Fecha de entrada en Vigencia: '%s'\n",proyectoVotado->fechaEntradaVigencia);
                        }else {
                            if(salida==2) {
                                printf("El proyecto de ley '%s' fue Vetado por el presidente de la republica",proyectoVotado->nombreProyecto);
                            }else {

                            }
                        }
                    }
                }
                break;

            }
            case 3: {
                recorrerArbolProyectos(es->congreso->ley);
                break;
            }
            case 4:{
                printf("Camara Del Senado\n");
                proyectoMasVotado = getProyectoMasVotado(es->congreso->senadores,es->congreso->ley);
                if(proyectoMasVotado!=NULL) {
                    printf("Nombre Proyecto de Ley:'%s'",proyectoMasVotado->nombreProyecto);
                }else {
                    printf("No hay Proyectos\n");
                }
                printf("\nCamara Del Senado\n");
                proyectoMasVotado = getProyectoMasVotado(es->congreso->diputados,es->congreso->ley);
                if(proyectoMasVotado!=NULL) {
                    printf("Nombre Proyecto de Ley:'%s'",proyectoMasVotado->nombreProyecto);
                }else {
                    printf("No hay Proyectos\n");
                }
                printf("\n\n");
                break;
            }
            default: {
                printf("Ingrese una opcion valida");
                break;
            }
        }
    }while(opcion!=1);
}
int buscarPersonaCongreso(struct nodoParlamentario *head,char *RutBuscado) {
    struct nodoParlamentario * rec;
    if(head==NULL) {
        return 0;
    }
    rec =head;
    while(rec!=NULL) {
        if(strcmp(rec->datosParlamentario->datos->rut,RutBuscado)==0) {
            return 1;
        }
        rec = rec->sig;
    }
    return 0;
}

struct Persona *datosPersona(struct Estado *es) {
    struct Persona *nueva;
    int edad, salida = 0;
    char nombre[60], Rut[14];

    // Solicitar el nombre
    printf("Ingrese el nombre: ");
    scanf(" %[^\n]", nombre);  // Se corrige la lectura sin usar & en una cadena

    // Solicitar y validar el Rut
    printf("Ingrese el Rut de la persona (Formato x.xxx.xxx-x): ");
    do {
        scanf("%s", Rut);  // Leer el Rut

        // Validar si el Rut ya existe en las listas del Congreso
        if (buscarPersonaCongreso(es->congreso->senadores, Rut) == 0 && 
            buscarPersonaCongreso(es->congreso->diputados, Rut) == 0) {
            salida = 1;  // Rut es válido y no está duplicado
        } else {
            printf("Ese Rut ya existe. ");
            printf("Ingrese 0 si quiere salir o cualquier número para reintentar: ");
            scanf("%d", &salida);
            if (salida == 0) {
                return NULL;  // Salir si el usuario elige
            }
        }
    } while (salida != 1);

    // Solicitar la edad
    printf("Ingrese la Edad: ");
    do {
        if (scanf("%d", &edad) != 1 || edad <= 0) {
            printf("Edad no válida. Intente de nuevo: ");
            while (getchar() != '\n');  // Limpiar el buffer
        } else {
            break;
        }
    } while (1);

    // Crear la nueva persona con los datos ingresados
    nueva = crearPersona(nombre, Rut, edad, 0);
    return nueva;
}


struct Parlamentario *DatosNuevoParlamentario(struct Estado *es,struct nodoParlamentario *head,int ID) {
    int salida=0,IdParlamentario;
    char partido[50],especialidad[27],periodo[50];
    struct Persona *datos;
    struct Parlamentario *parlamentario;

    datos = datosPersona(es);

    if(datos!=NULL) {
        printf("Ingrese el ID:");
        do {
            scanf("%d",&IdParlamentario);
            if(buscarParlamentario(head,datos->rut,ID)==0) {
                salida=1;
            }else {
                printf("El ID Del Parlamentario Ya existe\n");
            }
        }while(salida!=1);

        printf("Ingrese el partido Politico: ");
        scanf(" %[^\n]",partido);

        printf("Ingrese el numero Correspondiente a la Especialidad del parlamentario\n");
        do{
            printf("1:Educacion---2:Salud---3:Reforma---4:Impuesto---5:Defensa---06:Ministerio Publico \n");
            printf("7:Seguridad Social---8:Presupuesto---9:Poder Judicial---10:Coperacion Internacional\n");
            scanf("%d",&salida);
            if(agregarSectorProyectoLey(especialidad,salida)==0) {
                salida = 0;
                printf("Ingrese un Numero Valido\n");
            }

        }while(salida==0);
        printf("Ingrese el periodo: ");
        scanf(" %[^\n]",periodo);
        parlamentario = crearParlamentario(datos,IdParlamentario,periodo,partido,especialidad);
        return parlamentario;

    }
    printf("hubo un error\n");
    return NULL;
}

void agregarCambioParlamentario(struct Estado *es,struct nodoParlamentario *head,int idCambio){
    struct nodoParlamentario *rec;
    struct Parlamentario *nuevoParlamentario;
    rec=head;
    while(rec!=NULL) {
        if(rec->datosParlamentario->idParlamentario == idCambio) {
            printf("Ingrese Los datos del Nuevo Parlamentario\n");
            nuevoParlamentario = DatosNuevoParlamentario(es,head,idCambio);

            rec->datosParlamentario = nuevoParlamentario;
            printf("Se agrego el Parlamentario\n");
        }

        rec = rec->sig;
        }

    }


void cambiarParlamentario(struct Estado *es,struct nodoParlamentario *head) {
    int idCambio,salida=0;
    char rutBuscado[14];
    if(head==NULL) {
        printf("La Camara esta Vacia\n");
    }else {
        printf("Ingrese el ID del Parlamentario Que desea Cambiar y Tambien el Rut(x.xxx.xxx-x)\n");
        do {
            printf("ID: ");
            scanf("%d",&idCambio);
            printf("Rut: ");
            scanf("%s",rutBuscado);
            if(buscarParlamentario(head,rutBuscado,idCambio)==1) {
                printf("Se encontro\n");
                agregarCambioParlamentario(es,head,idCambio);
                salida=1;
            }else {
                printf("No se encontro el ID \n");
                printf("Ingrese 0 si quiere seguir o 1 si quiere salir\n");
                scanf("%d",&salida);
            }
        }while(salida != 1);
    }

}
void mostrarParlamentarios(struct CongresoNacional *congreso) {
    struct nodoParlamentario *rec;

    printf("Listado de Parlamentarios:\n");

    // Mostrar Diputados
    printf("\nCamara de Diputados:\n");
    if(congreso->diputados==NULL) {
        printf("La camara de Diputados esta Vacia\n");
    }else {
        rec = congreso->diputados;
        while (rec!= NULL) {
            printf("ID: %d, Nombre: %s\n", rec->datosParlamentario->idParlamentario, rec->datosParlamentario->datos->nombreApellido);
            rec = rec->sig;
        }
    }

    // Mostrar Senadores
    printf("\nCamara del Senado:\n");
    if(congreso->senadores==NULL) {
        printf("La camara del Senado esta Vacia\n");
    }else {
        rec = congreso->senadores;
        while (rec!= NULL) {
            printf("ID: %d, Nombre: %s\n", rec->datosParlamentario->idParlamentario, rec->datosParlamentario->datos->nombreApellido);
            rec = rec->sig;
        }
    }
}

struct Grafo *crearGrafo(int vertices) {
    int i;
    struct Grafo *grafo = (struct Grafo *)malloc(sizeof(struct Grafo));
    grafo->numVertices = vertices;

    grafo->adyacencia = (int **)malloc(vertices * sizeof(int *));
    for (i = 0; i < vertices; i++) {
        grafo->adyacencia[i] = (int *)calloc(vertices, sizeof(int)); // iniciar con ceros
    }

    return grafo;
}


void agregarArista(struct Grafo *grafo, int origen, int destino) {
    if (origen >= 0 && origen < grafo->numVertices && destino >= 0 && destino < grafo->numVertices) {
        grafo->adyacencia[origen][destino] = 1; // origen -> destino
    } else {
        printf("Error: índices fuera de rango.\n");
    }
}

void imprimirGrafoGlobal(struct Grafo *grafo, struct Parlamentario **parlamentarios) {
    int i, j;
    printf("Adyacencia Parlamentarios:\n");
    for (i = 0; i < grafo->numVertices; i++) {
        printf("Parlamentario %s (ID: %d): ", 
               parlamentarios[i]->datos->nombreApellido, 
               parlamentarios[i]->idParlamentario);

        for (j = 0; j < grafo->numVertices; j++) {
            if (grafo->adyacencia[i][j]) {
                printf("-> %s (ID: %d) ", 
                       parlamentarios[j]->datos->nombreApellido, 
                       parlamentarios[j]->idParlamentario);
            }
        }
        printf("\n");
    }
}

/*auxiliares*/

struct ProyectoLey *buscarProyectoPorID(struct nodoProyectoLey *arbol, int id) {
    while (arbol != NULL) {
        if (arbol->datosProyecto->id == id) {
            return arbol->datosProyecto; // proyecto encontrado
        } else if (id < arbol->datosProyecto->id) {
            arbol = arbol->izq;
        } else {
            arbol = arbol->der; 
        }
    }
    return NULL; // proyecto no encontrado
}

int buscarProyectoEnABB (struct nodoVoto *nodo, int idProyecto) { // busca el proyecto dentro de los votos del parlamentario
    while (nodo != NULL) {
        if (nodo->datosVoto->proyecto->id == idProyecto) {
            return 1; // proyecto encontrado
        } else if (idProyecto < nodo->datosVoto->proyecto->id) {
            nodo = nodo->izq;
        } else {
            nodo = nodo->der;
        }
    }
    return 0; // Proyecto no encontrado
}

void mostrarProyectos(struct nodoProyectoLey *arbol) { // mostrar proyectos de ley para seleccionar
    if (arbol == NULL) {
        return;
    }
    mostrarProyectos(arbol->izq);
    printf("ID: %d, Nombre: %s\n", arbol->datosProyecto->id, arbol->datosProyecto->nombreProyecto);
    mostrarProyectos(arbol->der);
}

/*fin auxiliares*/

void crearGrafoPorProyecto(struct CongresoNacional *congreso, int idProyecto) { // crea el grafo para un proyecto de ley
    struct Parlamentario *parlamentarios[205]; // máximo 155 diputados + 50 senadores
    struct nodoParlamentario *rec;
    struct Grafo *grafo;
    int totalParlamentarios = 0;
    struct ProyectoLey *proyecto = buscarProyectoPorID(congreso->ley, idProyecto);
    int i, j;
    if (proyecto == NULL) {
        printf("No se encontró un proyecto con el ID %d.\n", idProyecto);
        return;
    }

    printf("Proyecto seleccionado: %s\n", proyecto->nombreProyecto);

    for (rec = congreso->diputados; rec != NULL; rec = rec->sig) {
        if (buscarProyectoEnABB(rec->datosParlamentario->votos, proyecto->id)) {
            parlamentarios[totalParlamentarios++] = rec->datosParlamentario; // agregar diputado al arreglo
        }
    }
    for (rec = congreso->senadores; rec != NULL; rec = rec->sig) {
        if (buscarProyectoEnABB(rec->datosParlamentario->votos, proyecto->id)) {
            parlamentarios[totalParlamentarios++] = rec->datosParlamentario; // agregar senador al arreglo
        }
    }

    if (totalParlamentarios == 0) {
        printf("No se encontraron parlamentarios relacionados con este proyecto.\n");
        return;
    }

    grafo = crearGrafo(totalParlamentarios); // crea el grafo con los parlamenarios relacionados

    for (i = 0; i < totalParlamentarios; i++) {
        for (j = i + 1; j < totalParlamentarios; j++) {
            agregarArista(grafo, i, j); // agrega aristas entre los parlamentarios encontrados
        }
    }

    printf("Parlamentarios relacionados al Proyecto de Ley ID %d (%s):\n", idProyecto, proyecto->nombreProyecto);
    imprimirGrafoGlobal(grafo, parlamentarios);
}

void menuGrafoPorProyecto(struct CongresoNacional *congreso) {
    int idProyecto;

    printf("Proyectos de Ley Disponibles:\n"); // muestra los proyectos de ley para seleccionar
    mostrarProyectos(congreso->ley);

    printf("Ingrese el ID del proyecto para ver los parlamentarios relacionados: ");
    scanf("%d", &idProyecto);

    crearGrafoPorProyecto(congreso, idProyecto); // crea el grafo para el proyecto seleccionado
}

void agregarDatosParlamentario(struct Estado *es){
    int opcion, salida = 0;
    int idFalso = -22;
    struct Parlamentario *nuevoParlamentario;

    do {
        printf("Menu de Comienzo de votaciones Congreso Nacional\n");
        printf("Opcion 1: Salir al menu Principal\n");
        printf("Opcion 2: Modificar Parlamentarios\n");
        printf("Opcion 3: Mostrar los Parlamentarios\n");
        printf("Opcion 4: Mostrar parlamentarios relacionados a un proyecto\n");
        scanf("%d", &opcion);
        switch (opcion) {
            case 1: {
                printf("Saliendo del Menu de Agregacion de Parlamentarios\n");
                break;
            }
            case 2: {
                printf("Ingreso a Modificar Parlamentario\n");
                do {

                    printf("Ingrese que camara quiere modificar\n");
                    printf("1:Diputado ---- 2:Senado\n");
                    do {
                        scanf("%d", &salida);
                        if (salida != 1 && salida != 2) {
                            printf("Ingrese un Numero Valido\n");
                        }
                    } while (salida != 1 && salida != 2);

                    if (salida == 1) {
                        printf("Ingreso a la Camara de Diputados\n");
                        printf("1:Modificar Diputados --- 2: Agregar Diputados\n");
                        do {
                            scanf("%d", &salida);
                            if (salida != 1 && salida != 2) {
                                printf("Ingrese un Numero Valido\n");
                            }
                        } while (salida != 1 && salida != 2);

                        if (salida == 1) {
                            cambiarParlamentario(es, es->congreso->diputados);

                        } else {
                            if(es->congreso->numDiputados<155) {
                                nuevoParlamentario = DatosNuevoParlamentario(es, es->congreso->diputados, idFalso);
                                agregarParlamentario(&(es->congreso->diputados), nuevoParlamentario, &(es->congreso->numDiputados));
                            }else {
                                printf("Esta Llena la Camara de Diputados\n");
                            }
                        }


                    } else {
                        printf("Ingreso a la Camara del Senado\n");
                        printf("1:Modificar Senado --- 2: Agregar Senador\n");
                        do {
                            scanf("%d", &salida);
                            if (salida != 1 && salida != 2) {
                                printf("Ingrese un Numero Valido\n");
                            }
                        } while (salida != 1 && salida != 2);

                        if (salida == 1) {
                            cambiarParlamentario(es, es->congreso->senadores);

                        } else {
                            if(es->congreso->numSenadores<50) {
                                nuevoParlamentario = DatosNuevoParlamentario(es, es->congreso->senadores, idFalso);
                                agregarParlamentario(&(es->congreso->senadores), nuevoParlamentario,&(es->congreso->numSenadores));
                            }else {
                                printf("Esta Llena la Camara del Senado");
                            }
                        }
                    }

                    printf("Ingrese 1 si quiere Salir al menu anterior o Ingrese 0 si quiere seguir agregando Parlamentarios :\n");
                    scanf("%d",&salida);

                }while(salida!=1);
                break;
            }
            case 3: {
                printf("Ingreso a Mostrar Parlamentario\n");
                mostrarParlamentarios(es->congreso);
                printf("\n\n");
                break;

            }

            case 4: {
                menuGrafoPorProyecto(es->congreso);
                break;
            }

            default: {
                printf("Ingrese una opcion valida\n");
                break;
            }
        }
    } while (opcion != 1);
}
int eliminarCiudadano(struct nodoPersona **head, char *rut) {
    struct nodoPersona *rec;

    // Verificar si la lista está vacía
    if (*head == NULL) {
        return 0;
    }

    rec = *head;

    // Recorrer la lista circular
    do {
        if (strcmp(rec->datosPersona->rut, rut) == 0) {  // Si encontramos el nodo a eliminar
            if (rec->sig == rec) {  // Único nodo en la lista
                *head = NULL;  // Lista queda vacía
            } else {
                if (rec == *head) {  // El nodo a eliminar es el primer nodo
                    *head = rec->sig;  // Cambiar el puntero de la cabeza
                }
                rec->ant->sig = rec->sig;  // Saltar el nodo actual
                rec->sig->ant = rec->ant;  // Conectar los nodos adyacentes
            }

            // Liberar memoria del nodo eliminado
            free(rec->datosPersona->nombreApellido);
            free(rec->datosPersona->rut);
            free(rec->datosPersona);
            free(rec);

            return 1;  // Éxito
        }
        rec = rec->sig;
    } while (rec != *head);

    return 0;  // Rut no encontrado
}

void mostrarCiudadanos(struct nodoPersona *head) {
    struct nodoPersona *rec ;
    if (head == NULL) {
        printf("No hay ciudadanos registrados.\n\n");
        return;
    }
    rec =head;
    do {
        printf("Nombre: %s, RUT: %s, Edad: %d\n", rec->datosPersona->nombreApellido, rec->datosPersona->rut, rec->datosPersona->edad);
        rec = rec->sig;
    } while (rec != head);  // Asumiendo que es una lista circular

    printf("\n\n");
}

struct Articulo *buscarArticulo(struct Articulo **articulos, int numArticulos, int numeroArticulo) {
    // Buscar un artículo por su número en el arreglo de artículos
    int i;
    for (i = 0; i < numArticulos; i++) {
        if (articulos[i] != NULL && articulos[i]->num == numeroArticulo) {
            return articulos[i];  // Retorna el artículo si se encuentra
        }
    }
    return NULL;  // Retorna NULL si no se encuentra
}

void eliminarPresidente(struct Estado *es) {
    if (es->presidente->datosPresidente == NULL) {
        printf("No hay presidente registrado.\n");
        return;
    }
    free(es->presidente->datosPresidente->nombreApellido);
    free(es->presidente->datosPresidente->rut);
    free(es->presidente->datosPresidente);
    es->presidente->datosPresidente = NULL;
    printf("Presidente eliminado exitosamente.\n");
}

void listarPresidente(struct Estado *es) {
    if (es->presidente->datosPresidente == NULL) {
        printf("No hay presidente registrado.\n");
        return;
    }
    printf("Presidente: %s, RUT: %s, Edad: %d\n", 
        es->presidente->datosPresidente->nombreApellido, 
        es->presidente->datosPresidente->rut, 
        es->presidente->datosPresidente->edad);
}

void agregarDatosPresidente(struct Estado *es) {
    struct Presidente *nuevoPresidente;
    char periodo[50];
    int salida = 0, opcion;

    do {
        printf("Menu de Presidente:\n");
        printf("1. Agregar presidente\n");
        printf("2. Eliminar presidente\n");
        printf("3. Mostrar presidente\n");
        printf("4. Salir\n");
        printf("Seleccione una opcion:");
        scanf("%d", &opcion);

        switch (opcion) {
            case 1: {
                printf("Agregar un nuevo presidente:\n");

                // crear el presidente solicitando datos al usuario
                if (es->presidente->datosPresidente != NULL) {
                    printf("Ya hay un presidente registrado.\n");
                    break;
                }
                es->presidente = (struct Presidente *)malloc(sizeof(struct Presidente));
                nuevoPresidente->datosPresidente = datosPersona(es);
                nuevoPresidente->proyectoLey = NULL;
                
                es->presidente = nuevoPresidente;
                es->presidente->proyectoLey = NULL;

                if (nuevoPresidente != NULL) {
                    // Agregar la nueva persona a la lista de ciudadanos

                    if (agregarNodoPersona(&es->ciudadanos->personas, nuevoPresidente->datosPresidente)==1) {
                        printf("Presidente agregado exitosamente.\n");

                    } else {
                        printf("No se pudo agregar el presidente.\n");
                    }
                }
                break;
            }

            case 2: {
                eliminarPresidente(es);
                break;
            }

            case 3: {
                listarPresidente(es);
                break;
            }

            case 4: {
                salida = 1;
                printf("Saliendo del menu de presidente.\n\n");
                break;
            }

            default: {
                printf("Opción no válida. Intente de nuevo.\n");
                break;
            }
        } 
    } while (salida != 1);
}

void agregarDatosCiudadanos(struct Estado *es) {
    struct Persona *nuevaPersona;
    int salida =0,opcion;
    char rutEliminar[14];

    do {
        printf("Menu de Ciudadanos:\n");
        printf("1. Agregar ciudadano\n");
        printf("2. Eliminar ciudadano\n");
        printf("3. Mostrar Ciudadanos\n");
        printf("4. Salir\n");
        printf("Seleccione una opcion:");
        scanf("%d", &opcion);

        switch (opcion) {
            case 1: {
                printf("Agregar un nuevo ciudadano:\n");

                // Crear una nueva persona solicitando datos al usuario
                nuevaPersona = datosPersona(es);
                if (nuevaPersona != NULL) {
                    // Agregar la nueva persona a la lista de ciudadanos

                    if (agregarNodoPersona(&es->ciudadanos->personas, nuevaPersona)==1) {
                        printf("Ciudadano agregado exitosamente.\n");

                    } else {
                        printf("No se pudo agregar el ciudadano.\n");
                    }
                }
                break;
            }
            case 2: {
                printf("Ingrese el RUT del ciudadano a eliminar (Formato x.xxx.xxx-x):");
                scanf("%s", rutEliminar);

                if (eliminarCiudadano(&es->ciudadanos->personas, rutEliminar)) {
                    printf("Ciudadano eliminado exitosamente.\n\n");
                } else {
                    printf("No se encontro el ciudadano con el RUT proporcionado.\n");
                }
                break;
            }
            case 3: {
                printf("Mostrando todos los ciudadanos:\n");
                mostrarCiudadanos(es->ciudadanos->personas);
                break;
            }
            case 4: {
                salida = 1;
                printf("Saliendo del menu de ciudadanos.\n\n");
                break;
            }
            default: {
                printf("Opción no válida. Intente de nuevo.\n");
                break;
            }
        }

    } while (salida == 0);
}

int main(){
    int opcion, articuloBuscado, numeroArticulo;
    struct Estado *es;
    struct ProyectoLey *proyecto;
    struct Articulo *articulo;
    int idProyecto;
    es= (struct Estado*)malloc(sizeof(struct Estado));
    es->congreso= (struct CongresoNacional *) malloc(sizeof(struct CongresoNacional));
    es->congreso->ley = NULL;
    es->congreso->senadores = es->congreso->diputados =  NULL;
    es->congreso->numSenadores = es->congreso->numDiputados = 0;
    es->ciudadanos = (struct Ciudadanos *) malloc(sizeof(struct Ciudadanos));
    es->ciudadanos->personas = NULL;
    es->presidente= (struct Presidente *) malloc(sizeof(struct Presidente));
    es->presidente->datosPresidente =NULL;
    es->presidente->proyectoLey = NULL;

    do {
        printf("1. Menu Agregar Proyecto de Ley\n");
        printf("2. Menu Parlamentario\n");
        printf("3. Menu Agregar Ciudadano\n");
        printf("4. Menu Clientes\n");
        printf("5. Menu Votacion Proyecto De Ley\n");
        printf("6. Menu Presidente\n");
        printf("7. Buscar Articulo\n");
        printf("0. Salir\n");
        printf("Ingrese opcion: \n");
        scanf("%d", &opcion);
        switch (opcion) {
            case 0:{
                printf("Saliendo...\n");
                break;
            }
            case 1: {
                printf("\nSeleccionaste la opcion 1 Menu Agregar Proyecto de Ley\n\n");
                ingresarProyectoLey(es,&es->congreso->ley);
                break;
            }

            case 2: {
                printf("\nSeleccionaste la opcion 2 Menu Parlamentario\n\n");
                agregarDatosParlamentario(es);
                break;
            }

            case 3:{
                printf("\nSeleccionaste la opcion 3 Menu Agregar Ciudadano\n\n");
                agregarDatosCiudadanos(es);
                break;
            }

            case 4: {
                printf("\nSeleccionaste la opcion 4 Menu Clientes\n\n");
                agregarDatosCiudadanos(es);
                break;
            }
            case 5: {
                printf("\nSeleccionaste la opcion 5 Menu Votacion Proyecto De Ley\n\n");
                comienzoVotacionCongreso(es);
                break;
            }
            case 6: {
                printf("\nSeleccionaste la opcion 6 Menu Presidente\n\n");
                agregarDatosPresidente(es);
                break;
            }
            case 7: {
                printf("\nSeleccionaste la opción 7 Buscar Artículo en Proyecto de Ley\n\n");
                printf("Ingrese el ID del Proyecto de Ley:\n");
                scanf("%d", &idProyecto);

                // Verificar si el proyecto existe
                proyecto = proyectoVotando(es->congreso->ley, idProyecto);
                if (proyecto == NULL) {
                    printf("El proyecto de ley con ID %d no existe.\n", idProyecto);
                } else {
                    printf("Ingrese el número del artículo a buscar:\n");
                    scanf("%d", &numeroArticulo);

                    // Buscar el artículo en el proyecto

                    articulo = buscarArticulo(proyecto->articulos, MaxCantidadArticulos, numeroArticulo);
                    if (articulo != NULL) {
                        printf("Artículo encontrado:\n");
                        printf("Número: %d\nDescripción: %s\n", articulo->num, articulo->descripcion);
                    } else {
                        printf("Artículo con número %d no encontrado en el proyecto de ley.\n", numeroArticulo);
                    }
                }
                printf("\n\n");
                break;
            }
            default: {
                printf("Ingrese opcion valida\n\n");
                break;
            }

        }

    }while(opcion!=0);

    return 0;
}
