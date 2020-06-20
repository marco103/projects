#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define epsilon 0.00000000000000011102			//Definisco qui come epsilon la precisione di macchina del calcolatore, da usare in confronti tra numeri reali

//Definizione delle strutture dati costituenti le matrici elaborate dal programma

struct colonne{                 //Lista costituente gli elementi non nulli su una riga della matrice

        long double elemento;
        struct colonne *nextcolumn;
        int colonna;

};

struct righe{                   //Lista di liste,costituente le righe della matrice. Ad ogni riga è connessa tramite puntatore una struttura di tipo colonne, contenente
                                //gli elementi della riga
        struct colonne *column;
        struct righe *nextrow;
        int riga;

};


struct caratteristiche{     //Struttura che contiene le caratteristiche di una matrice, compresi i suoi elementi e le loro posizioni

        struct righe *row;
        char nome[11];
        int rows;
        int cols;

};


struct matrice{                 //Struttura contenente un puntatore a righe (a loro volta connesse a colonne) e il carattere che costituisce il "nome" della matrice.

        struct caratteristiche car;
        struct matrice *nextmatrix;

};

int CreaModifica_Nodo (struct matrice *matrix, int rig, int col,long double val){	//Crea un nodo per la matrice "matrix" nel caso in cui esso non esista, sostituisce il valore al suo interno nel caso in cui esso esista.
											//Se il nodo non viene creato per memoria insufficiente la funzione restituisce come valore di ritorno 1, altrimenti 0.
	if (val==0)
		return 0;

	if (matrix->car.row==NULL){

		struct righe *head=NULL;
                struct colonne *head2=NULL;

                head=(struct righe*)malloc(sizeof(struct righe));
                head2=(struct colonne*)malloc(sizeof(struct colonne));

		if (head==NULL || head2==NULL)
                        return 1;



		matrix->car.row=head;
                matrix->car.row->column=head2;
                matrix->car.row->riga=rig;
                matrix->car.row->column->colonna=col;
                matrix->car.row->column->elemento=val;
                matrix->car.row->nextrow=NULL;
                matrix->car.row->column->nextcolumn=NULL;
                return 0;

	}



        struct righe *s=matrix->car.row;

        while (s!=NULL && s->riga<rig)
        	s=s->nextrow;

        if (s==NULL || s->riga!=rig){

                struct righe *newrow;
                struct colonne *newcol;
                struct righe *p=matrix->car.row;

                newrow=(struct righe*)malloc(sizeof(struct righe));
                newcol=(struct colonne*)malloc(sizeof(struct colonne));

		if (newrow==NULL || newcol==NULL)
			return 1;

                if (s==matrix->car.row){

                        newrow->nextrow=s;
                        matrix->car.row=newrow;

                }

		else {

			while (p->nextrow!=s)
				 p=p->nextrow;

                        p->nextrow=newrow;
                        newrow->nextrow=s;

		}

                newrow->column=newcol;
		newrow->column->nextcolumn=NULL;

                newrow->riga=rig;
                newcol->colonna=col;
                newcol->elemento=val;

		return 0;
	}

        else {

                struct colonne *newcol;
                struct colonne *q;
                newcol=(struct colonne*)malloc(sizeof(struct colonne));

		if (newcol==NULL)
			return 1;

                q=s->column;

                while (q!=NULL && q->colonna<col)
                         q=q->nextcolumn;

                if (q==NULL || q->colonna!=col){

                        if (q==s->column){

                                newcol->nextcolumn=s->column;
                                s->column=newcol;

                        }

                        else {

                                struct colonne *p=s->column;

                                while (p!=NULL && p->nextcolumn!=q)
                                        p=p->nextcolumn;

                                p->nextcolumn=newcol;
                                newcol->nextcolumn=q;

                        }

                	newcol->colonna=col;
                	newcol->elemento=val;

			return 0;

                }

                if (q!=NULL && q->colonna==col){

                        q->elemento=val;
			return 0;
        	}
	}
}

void RiempiMatrice (struct matrice *matrix,int rig,int col){            //Riempie la matrice date come parametri la matrice stessa e le sue dimensioni. Crea tutti i nodi inseriti dall'utente (non nulli)

        printf("\n\nInserisci gli elementi non nulli della matrice, collocandoli in una riga o colonna. Collocare in una posizione già riempita equivale a sovrascrivere.");
        printf("\n\nScrivere 0 in qualsiasi campo farà interrompere l'inserimento al termine dell'operazione in corso.\n\n");

        int newrow=rig+1,newcol=col+1;
        double val=1;

	int ch;

	while (newrow!=0 && newcol!=0 && val!=0){

		printf("Riga: ");
		scanf("%d",&newrow);

		while((ch=getchar())!='\n' && ch!=EOF);


		printf("Colonna: ");
		scanf("%d",&newcol);

		while((ch=getchar())!='\n' && ch!=EOF);


                printf("Valore: ");
		scanf("%lf",&val);

		while((ch=getchar())!='\n' && ch!=EOF);


                if (newrow>rig || newcol>col)
			printf("\nIl nodo non può essere inserito, la riga o la colonna selezionate non esistono\n\n");

		else{

			if (newrow!=0 && newcol!=0 && val!=0)
				CreaModifica_Nodo(matrix,newrow,newcol,val);
                }
	}
}

int CancellaNewLine (char stringa[]){		//Cancella il carattere \n lasciato da fgets() per sostituirlo con \0 (carattere di terminazione) e comunica, nel caso in cui \n è assente (val di ritorno 1), che il buffer deve essere
						//svuotato in quanto l'utente ha inserito troppi caratteri
	int lung=strlen(stringa);

	if (stringa[lung-1]=='\n'){

		stringa[lung-1]='\0';
		return 0;

	}

	else
		return 1;

}

void GiveName (struct matrice *matrix){		//Dà un nome alla matrice passata come parametro

	if (matrix==NULL)
		return;

	printf("Come si chiama la nuova matrice?\n\nInserisci un nome (max 10 caratteri presi): ");

	char nome[11];
	int ch;

	strcpy(nome,"stop");		//Evita l'allocazione di una stringa casuale e fa entrare il programma nel ciclo

        while (strcmp(nome,"stop")==0){

                fgets(nome,11,stdin);
		if (CancellaNewLine(nome)==1)
			while((ch=getchar())!='\n' && ch!=EOF);

		if (strcmp(nome,"stop")==0)
			printf("\n\nNon puoi assegnare 'stop' come nome ad una matrice, esso è un comando utilizzato all'interno del programma.\n\nInserire un altro nome (max 10 caratteri acquisiti): ");
                else
                        strcpy(matrix->car.nome,nome);
        }
}


struct matrice* NuovaMatrice (){   //Genera una nuova matrice inserita dall'utente, e può anche creare la prima matrice, che si pone a testa della lista di matrici contenute in memoria durante l'esecuzione del programma.
				   //Alloca dunque la memoria per una struct matrice e procede poi al riempimento della matrice con allocazione di memoria per i nodi non nulli e con sostituzioni eventuali di valori già inseriti.
	struct matrice *M=NULL;
	M=(struct matrice*)malloc(sizeof(struct matrice));

	int ch;

	if (M!=NULL){

		M->nextmatrix=NULL;
		M->car.row=NULL;

		printf("Numero di righe: ");
		scanf("%d",&(M->car.rows));

		while((ch=getchar())!='\n' && ch!=EOF);

		printf("\n\nNumero di colonne: ");
		scanf("%d",&(M->car.cols));

		while((ch=getchar())!='\n' && ch!=EOF);

		RiempiMatrice(M,M->car.rows,M->car.cols);

	}

	return M;
}

void LiberaMemoria(struct matrice *matrix){		//Libera la memoria saturata dai dati appartenenti alla matrice che occupa l'area di memoria puntata.

	struct righe *canc_r=matrix->car.row;
	struct colonne *canc_c=NULL;

	if (canc_r==NULL){

		free(matrix);
		return;

	}

	else {

		while (canc_r!=NULL){

			if (canc_r->column!=NULL){

				canc_c=canc_r->column;

				while (canc_c!=NULL){

					canc_r->column=canc_c->nextcolumn;
					free(canc_c);
					canc_c=canc_r->column;

				}

			}

			matrix->car.row=canc_r->nextrow;
			free(canc_r);
			canc_r=matrix->car.row;

		}

		free(matrix);

		return;

	}
}

int CancellaMatrice(struct matrice **list,char name[]){		//Cancella una matrice da una lista di matrici e libera l'area di memoria da essa saturata.

	struct matrice *prec,*succ;
	prec=succ=*list;

	while (succ!=NULL && strcmp(succ->car.nome,name)){

		prec=succ;
		succ=succ->nextmatrix;

	}

	if (succ==NULL)
		return 1;

	else {


		struct righe *p=succ->car.row,*canc_r=succ->car.row;
		struct colonne *q=NULL,*canc_c=NULL;

		while (p!=NULL){

			if (p->column!=NULL){

				canc_c=p->column;
				q=p->column;

				while (q!=NULL){

					q=q->nextcolumn;
					free(canc_c);
					canc_c=q;

				}

			}

			p=p->nextrow;
			succ->car.row=succ->car.row->nextrow;
			free(canc_r);
			canc_r=p;

		}

		if (succ==(*list)){

			*list=(*list)->nextmatrix;
			free(succ);

		}

		else {

			prec->nextmatrix=succ->nextmatrix;
			free(succ);

		}

		return 0;
	}
}

void CancellaElemento (struct matrice *matrix,int rig, int col){		//Cancella un nodo all'interno della matrice, se il nodo non esiste non succede nulla e la funzione esce.

        struct righe *prec_r=matrix->car.row,*succ_r=matrix->car.row;
        struct colonne *prec_c=NULL,*succ_c=NULL;

        while (succ_r!=NULL && succ_r->riga<rig){

                prec_r=succ_r;
                succ_r=succ_r->nextrow;

        }

        if (succ_r==NULL || succ_r->riga!=rig)
                return;

        succ_c=prec_c=succ_r->column;

        while (succ_c!=NULL && succ_c->colonna<col){

                prec_c=succ_c;
                succ_c=succ_c->nextcolumn;

        }

        if (succ_c==NULL || succ_c->colonna!=col)
                return;

        if (succ_c==succ_r->column){

                succ_r->column=succ_c->nextcolumn;
                free(succ_c);

        }

        else {
                prec_c->nextcolumn=succ_c->nextcolumn;

                free(succ_c);

        }

        if (succ_r->column==NULL){

                if (succ_r==matrix->car.row){

                        matrix->car.row=succ_r->nextrow;
                        free(succ_r);

                }

                else {

                        prec_r->nextrow=succ_r->nextrow;
                        free(succ_r);
                }
        }
}

void PulisciMatrice (struct matrice *matrix){		//Elimina tutti gli elementi di una matrice lasciandone però la struttura intatta (utile nelle copie di matrice o per evitare che rimangano in memoria elementi che non vengono
							//utilizzati (esempio se la matrice acquista un numero di colonne o righe minore in seguito ad un prodotto)
	struct righe *r=matrix->car.row;
	struct colonne *c=NULL;

	while (matrix->car.row!=NULL){

		c=r->column;

		while (r->column!=NULL){

			r->column=c->nextcolumn;
			free(c);
			c=r->column;
		}

		matrix->car.row=r->nextrow;
		free(r);
		r=matrix->car.row;

	}

}


void CopiaMatrice (struct matrice *dest,struct matrice *source){		//Copia i dati di una matrice (escluso il nome) nell'area di memoria puntata da dest, per cui si ottiene in dest una matrice identica a quella contenuta
										//nell'area di memoria puntata da source
	dest->car.rows=source->car.rows;
        dest->car.cols=source->car.cols;

	int i,j;

	PulisciMatrice(dest);

	if (source->car.row==NULL){

		for (i=1;i<=source->car.rows;i++){

			for (j=1;j<=source->car.cols;j++)
                                CancellaElemento(dest,i,j);

		}

                return;

	}

        struct righe *p=source->car.row;
        struct colonne *colp=p->column;

        for (i=1;i<=source->car.rows;i++){

		if (p!=NULL && p->riga==i){

			colp=p->column;

       	        	for (j=1;j<=source->car.cols;j++){

				if (colp!=NULL && colp->colonna==j){

               	                	CreaModifica_Nodo(dest,i,j,colp->elemento);
                       	        	colp=colp->nextcolumn;

				}

				else if (colp==NULL || colp->colonna!=j)
					CancellaElemento(dest,i,j);

			}

			p=p->nextrow;

		}

		else if (p==NULL || p->riga!=i){

			for (j=1;j<=source->car.cols;j++)
				CancellaElemento(dest,i,j);

		}

	}
}



void print (struct matrice *matrix){			//Stampa a schermo il contenuto di una matrice. Fondamentale sia per il debugging che per il feedback con l'utente del programma.

	struct righe *r=matrix->car.row;
	struct colonne *c=NULL;
	int i,j;

	for (i=1;i<=matrix->car.rows;i++){

		printf("\t");

		if (r!=NULL && r->riga==i){

			c=r->column;

			for (j=1;j<=matrix->car.cols;j++){

				if (c!=NULL && c->colonna==j){
					printf("%.4Lf\t",c->elemento);
					c=c->nextcolumn;
				}

				else
					printf("0.0000\t");
			}

			r=r->nextrow;

		}

		else {

			for (j=1;j<=matrix->car.cols;j++)
				printf("0.0000\t");

		}

		printf("\n\n");
	}
}

void StampaRaccolta(struct matrice *list){		//Funzione che stampa l'intero contenuto di una lista di matrici

	while (list!=NULL){

		printf("\n\n%s\n--------------------------------------------------",list->car.nome);
		printf("\n\n");
		print(list);
		list=list->nextmatrix;

	}
}

float Seek (struct matrice *matrix,int rig,int col){    //Funzione di ricerca e lettura di un singolo nodo,restituisce il valore contenuto nel nodo
                                                        //selezionato della matrice
        struct righe *p=matrix->car.row;

	if (matrix->car.row==NULL)		//Se la matrice è vuota, la funzione restituisce direttamente 0, in quanto il nodo cercato ovviamente non esiste
		return 0;

        while (p!=NULL && p->riga<rig)
                p=p->nextrow;

        if (p!=NULL && p->riga==rig){

                struct colonne *q=p->column;

                while (q!=NULL && q->colonna<col)
                        q=q->nextcolumn;

		if (q==NULL || q->colonna!=col)
			return 0;		 //Se non esiste il nodo vuol dire che l'elemento è nullo, per cui la funzione restituisce il valore 0
		else
			return q->elemento;	 //Se l'elemento viene trovato esso viene restituito come valore di ritorno dalla funzione

        }

	else
		return 0;	 //Se non esiste il nodo vuol dire che l'elemento è nullo, per cui la funzione restituisce il valore 0
}

//STRUTTURE SOMMA-PRODOTTO

struct sumprod {			//Si definisce una pila di nomi di matrici, affinchè le matrici possano essere bersagliate da altre funzioni in base al loro nome.

        char name[11];
        struct sumprod *nextname;

};


void *AggiungiOperando(struct sumprod **stack,char nome[]){		//Aggiunge alla pila di nomi il nome di una matrice (nel main si faranno delle verifiche per accertare che il nome sia contenuto nella pila prima di
									//invocare la funzione)
        struct sumprod *newop=NULL;
        newop=(struct sumprod*)malloc(sizeof(struct sumprod));

	newop->nextname=NULL;

        strcpy(newop->name,nome);

        if (*stack==NULL){

                *stack=newop;
                (*stack)->nextname=NULL;

        }

        else {

                newop->nextname=*stack;
                *stack=newop;

        }

}

void Pop(struct sumprod **stack,char *nome){		//Estrae il nome in testa alla pila e libera l'area di memoria occupata dal nodo.

	struct sumprod *p;

	p=(*stack);

	strcpy(nome,p->name);

	*stack=(*stack)->nextname;

	free(p);
}

struct sumprod *EstraiTesta (struct sumprod **stack){		//Rimuove la testa da una pila.

	struct sumprod *p=NULL;

	p=(*stack);
	*stack=(*stack)->nextname;

	return p;
}

struct sumprod *ListaProdotto(struct sumprod *stack){		//A partire da una pila ne crea un'altra in cui gli elementi sono posti nell'ordine esattamente opposto (serve per mantenere corretto l'ordine all'esecuzione della
								//funzione prodotto, in cui conta l'ordine (il prodotto matriciale non è commutativo)
	struct sumprod *newstack=NULL;
	struct sumprod *p=NULL;

	newstack=EstraiTesta(&stack);
	newstack->nextname=NULL;

	while (stack!=NULL){

		p=EstraiTesta(&stack);
		p->nextname=newstack;
		newstack=p;

	}

	return newstack;

}

void printnames(struct sumprod *list){			//Funzione utilizzata solo per il debugging

	while(list!=NULL){
		printf("%s",list->name);
		list=list->nextname;
	}
}

void CancellaPila(struct sumprod **namestack){

	struct sumprod *p=NULL;

	while ((*namestack)!=NULL){

		p=(*namestack);
		*namestack=(*namestack)->nextname;
		free(p);

	}
}

struct matrice *AggiungiMatriceInTesta (struct matrice **list,struct matrice *newheadmatrix){    //Funzione che aggiunge in testa alla lista passata come parametro
												//la matrice passata come secondo parametro
	if ((*list)==NULL)
		*list=newheadmatrix;

	else {

		newheadmatrix->nextmatrix=*list;
		*list=newheadmatrix;

	}
}

struct matrice *SelezionaMatrice (struct matrice *list,char name[]){            //Seleziona da una lista di matrici la matrice associata alla stringa "name[]" e fa
                                                                                //puntare l'area di memoria ad essa destinata ad un puntatore a struct matrice
        while (list!=NULL && strcmp(list->car.nome,name)!=0)
                list=list->nextmatrix;

        return list;
}

long double AbsVal (long double x){

        if (x<0)
                x=-x;

        return x;
}

struct matrice *Somma (struct matrice *list,struct sumprod **namestack){		//Somma tra loro le matrici contenute in "list" i cui nomi corrispondono a quelli contenuti nella pila "namestack".

        struct matrice *memory=NULL;
        memory=(struct matrice*)malloc(sizeof(struct matrice));

	memory->car.row=NULL;
	memory->nextmatrix=NULL;

        struct matrice *first=NULL,*second=NULL;

	char matrixname[11];

        int i,j,rig,col;
        char nome[11];

        long double somma;

	int start=0;

        while ((*namestack)!=NULL){

		if (start==0){

			Pop(namestack,matrixname);
			first=SelezionaMatrice(list,matrixname);
			Pop(namestack,matrixname);
			second=SelezionaMatrice(list,matrixname);

		}

		else {

			first=memory;
			Pop(namestack,matrixname);
			second=SelezionaMatrice(list,matrixname);

		}

		if (first->car.rows!=second->car.rows || first->car.cols!=second->car.cols){

        	        LiberaMemoria(memory);
			memory=NULL;
			while (*namestack!=NULL)
				Pop(namestack,matrixname);
                	return memory;

	        }

		start=1;

		rig=first->car.rows;
		col=first->car.cols;

                for (i=1;i<=rig;i++){

                        for (j=1;j<=col;j++){

                                somma=Seek(first,i,j)+Seek(second,i,j);

                                if (AbsVal(somma)>=epsilon)
					CreaModifica_Nodo(memory,i,j,somma);

				if (AbsVal(somma)<epsilon)
					CancellaElemento(memory,i,j);

                        }

                }

                memory->car.rows=rig;
		memory->car.cols=col;

        }

        return memory;
}

struct matrice *Prodotto (struct matrice *list,struct sumprod **namestack){   		//Moltiplica tra loro le matrici contenute in "list" i cui nomi corrispondono a quelli contenuti nella pila "namestack".

        struct matrice *memory=NULL;
        memory=(struct matrice*)malloc(sizeof(struct matrice));

	memory->car.row=NULL;
	memory->nextmatrix=NULL;

	struct matrice *result=NULL;
	result=(struct matrice*)malloc(sizeof(struct matrice));

	result->car.row=NULL;
	result->nextmatrix=NULL;

        struct matrice *first=NULL,*second=NULL;

	char matrixname[11];

        int i=1,j=1,scorrimento;
        int rig,col;
        long double num=0;
        char nome[11];

	int start=0;

        while ((*namestack)!=NULL){

		if (start==0){

			Pop(namestack,matrixname);
        		first=SelezionaMatrice(list,matrixname);
			Pop(namestack,matrixname);
        		second=SelezionaMatrice(list,matrixname);

		}

		else {

			CopiaMatrice(result,memory);
                        first=result;
                        Pop(namestack,matrixname);
                        second=SelezionaMatrice(list,matrixname);
			PulisciMatrice(memory);

		}

		if (first->car.cols!=second->car.rows){

                	LiberaMemoria(result);
                	LiberaMemoria(memory);
			result=NULL;
			memory=NULL;
			while (*namestack!=NULL)
				Pop(namestack,matrixname);
                	return memory;

        	}


		start=1;

		rig=first->car.rows;
                col=second->car.cols;

                for (i=1;i<=rig;i++){

                        for (j=1;j<=col;j++){

                                for (scorrimento=1;scorrimento<=first->car.cols;scorrimento++)
                                        num+=Seek(first,i,scorrimento)*Seek(second,scorrimento,j);

                                if (AbsVal(num)>=epsilon)
					CreaModifica_Nodo(memory,i,j,num);

				if (AbsVal(num)<epsilon)
					CancellaElemento(memory,i,j);

                                num=0;

                        }

                }

		memory->car.rows=rig;
		memory->car.cols=col;

        }

	LiberaMemoria(result);
	result=NULL;

	return memory;
}

//OPERAZIONI SU SINGOLA MATRICE

void ScambiaRighe (struct matrice *matrix,int riga1, int riga2){	//Scambia due righe della matrice, i parametri di tipo int devono essere legati dalla relazione riga1>riga2

	if (matrix->car.row==NULL)
		return;

        struct righe *r1=matrix->car.row,*r2=matrix->car.row;

        if (r1->riga==riga1){

                while (r2->nextrow!=NULL && r2->nextrow->riga<riga2)
                        r2=r2->nextrow;

		if (r2->nextrow!=NULL && r2->nextrow->riga==riga2){

                        matrix->car.row=r2->nextrow;
                        r2->nextrow=r2->nextrow->nextrow;

			if (r1==r2)
				matrix->car.row->nextrow=r1;
			else {

				matrix->car.row->nextrow=r1->nextrow;

				r1->nextrow=r2->nextrow;
				r2->nextrow=r1;

			}

			matrix->car.row->riga=riga1;
			r1->riga=riga2;

		}

		else {

			if (r1==r2)
				r1->riga=riga2;

			else {

				r1->nextrow=r2->nextrow;
				r2->nextrow=r1;
				matrix->car.row=r2;
				r1->riga=riga2;
			}

		}

        }

        else if (r1->riga!=riga1){

		while (r1->nextrow!=NULL && r1->nextrow->riga<riga1)
                        r1=r1->nextrow;

                while (r2->nextrow!=NULL && r2->nextrow->riga<riga2)
                        r2=r2->nextrow;


		if (riga1 < matrix->car.row->riga){

			if (r2==matrix->car.row && matrix->car.row->riga==riga2){

                                r2->riga=riga1;
                                return;

                        }

                        if (r2==matrix->car.row && r2->nextrow->riga==riga2){

                                r1=r2->nextrow;
                                r2->nextrow=r1->nextrow;
                                matrix->car.row=r1;
                                r1->nextrow=r2;
				r1->riga=riga1;
                                return;

                        }

                        matrix->car.row=r2->nextrow;
			r2->nextrow=r2->nextrow->nextrow;
			matrix->car.row->nextrow=r1;
			matrix->car.row->riga=riga1;
			return;


		}

		else {

			if (r1==r2){

				r1->nextrow->riga=riga1;
				return;

			}

			if ((r1->nextrow==NULL || r1->nextrow->riga!=riga1) && (r2->nextrow!=NULL && r2->nextrow->riga==riga2)){

				r1->nextrow=r2->nextrow;
				r2->nextrow=r2->nextrow->nextrow;
				r1->nextrow->nextrow=r2;
				r1->nextrow->riga=riga1;
				return;

			}

			if ((r1->nextrow!=NULL && r1->nextrow->riga==riga1) && (r2->nextrow!=NULL && r2->nextrow->riga==riga2)){

				int j,valtemp;
				struct colonne *c1=r1->nextrow->column,*c2=r2->nextrow->column;

				for (j=1;j<=matrix->car.cols;j++){

					if ((c1!=NULL && c1->colonna==j) && (c2!=NULL && c2->colonna==j)){

						valtemp=c1->elemento;
						c1->elemento=c2->elemento;
						c2->elemento=valtemp;
						c1=c1->nextcolumn;
						c2=c2->nextcolumn;

					}

					else if ((c1==NULL || c1->colonna!=j) && (c2!=NULL && c2->colonna==j)){

						CreaModifica_Nodo(matrix,riga1,j,c2->elemento);
						c2=c2->nextcolumn;
						CancellaElemento(matrix,riga2,j);

					}

					else if ((c1!=NULL && c1->colonna==j) && (c2==NULL || c2->colonna!=j)){

						CreaModifica_Nodo(matrix,riga2,j,c1->elemento);
                                                c1=c1->nextcolumn;
                                                CancellaElemento(matrix,riga1,j);

                                        }

				}

				return;

			}
		}
	}
}

long double *SottraiRighe (struct matrice *matrix,int pivot,int rigamod,char modo[]){		//Funzione che permette di sottrarre le due righe scelte della matrice in modo tale da creare pivot nella matrice ed effettuare passo passo
												//la riduzione di Gauss. Inserire "m", che sta per "memorizza", permette alla funzione di memorizzare nel vettore divris il risultato della
        struct righe *rig1=matrix->car.row,*rig2=matrix->car.row;				//divisione tra l'elemento della seconda riga che si trova nella colonna del pivot e il pivot stesso (divris[0]) e un valore tra 0 e 1
        struct colonne *c1=NULL,*c2=NULL;							//che indica se le righe sono state scambiate (divris[1]). Per non memorizzare tali risultati si inserisce una qualunque stringa. Per
												//convenzione nel codice si userà "nm".
	long double *divris=NULL;

	if (strcmp(modo,"m")==0){

		divris=(long double*)malloc(2*sizeof(long double));
		divris[0]=0;
		divris[1]=0;

	}

        while (rig1!=NULL && rig1->riga<pivot)
                rig1=rig1->nextrow;

        while (rig2!=NULL && rig2->riga<rigamod)
                rig2=rig2->nextrow;

        if ((rig1==NULL || rig1->riga!=pivot) && (rig2==NULL || rig2->riga!=rigamod))
                return divris;

        if ((rig1!=NULL && rig1->riga==pivot) && (rig2!=NULL && rig2->riga==rigamod)){

                c1=rig1->column;
                c2=rig2->column;

                while (c2!=NULL && c2->colonna<pivot)
                        c2=c2->nextcolumn;
                while (c1!=NULL && c1->colonna<pivot)
                        c1=c1->nextcolumn;

                if ((c2!=NULL && c2->colonna==pivot) && (c1!=NULL && c1->colonna==pivot)){

                        long double div;

                        div=(c2->elemento)/(c1->elemento);
                        c1=c1->nextcolumn;
                        c2=c2->nextcolumn;
                        CancellaElemento(matrix,rigamod,pivot);

                        int j=pivot+1;

                        while (j<=matrix->car.cols){

                                if ((c2!=NULL && c2->colonna==j) && (c1!=NULL && c1->colonna==j)){

                                        if (AbsVal((c2->elemento)-(c1->elemento)*div)<epsilon){
                                                c2=c2->nextcolumn;
						CancellaElemento(matrix,rigamod,j);
                                                c1=c1->nextcolumn;
                                        }

                                        else {

                                                c2->elemento=(c2->elemento)-(c1->elemento)*div;
                                                c1=c1->nextcolumn;
                                                c2=c2->nextcolumn;
                                        }

                                }

                                if ((c2==NULL || c2->colonna!=j) && (c1!=NULL && c1->colonna==j)){

				        CreaModifica_Nodo(matrix,rigamod,j,(-(c1->elemento)*div));
					CreaModifica_Nodo(matrix,rigamod,j,(-(c1->elemento)*div));
                                        c1=c1->nextcolumn;

                                }

                                if ((c2!=NULL && c2->colonna==j) && (c1==NULL || c1->colonna!=j))
                                        c2=c2->nextcolumn;

                                j++;

                        }

			if (divris!=NULL){
				divris[0]=div;
				divris[1]=0;
			}

                        return divris;


                }

                if ((c2!=NULL && c2->colonna==pivot) && (c1==NULL || c1->colonna!=pivot)){

                        ScambiaRighe(matrix,pivot,rigamod);

			if (divris!=NULL){

				divris[0]=0;
				divris[1]=1;

			}

                        return divris;
                }
        }

        if ((rig1==NULL || rig1->riga!=pivot) && (rig2!=NULL && rig2->riga==rigamod)){

		c1=rig2->column;

                while (c1!=NULL && c1->colonna<pivot)
                        c1=c1->nextcolumn;

                if (c1!=NULL && c1->colonna==pivot){

                        ScambiaRighe(matrix,pivot,rigamod);

			if (divris!=NULL){

				divris[0]=0;
				divris[1]=1;

			}

		}

                return divris;

        }

	return divris;

}

struct matrice *Gauss(struct matrice *matrix){		//Implementazione della riduzione per righe di una matrice con metodo di Gauss

        struct matrice *copy=NULL;
        copy=(struct matrice*)malloc(sizeof(struct matrice));

	copy->car.row=NULL;
	copy->nextmatrix=NULL;

        CopiaMatrice(copy,matrix);

        int i;
	int pivot;

        for (pivot=1;pivot < matrix->car.rows;pivot++){

                for (i=pivot+1;i<=matrix->car.rows;i++){
                        SottraiRighe(copy,pivot,i,"nm");
		}

        }

        return copy;

}

long double det(struct matrice *matrix){		//Funzione che restituisce il determinante della matrice copy, copia della matrice di partenza che viene sottoposta a riduzione di Gauss

        struct matrice *copy=Gauss(matrix);

        int i;
        long double determinante=1;

        for (i=1;i<=copy->car.rows;i++)
                determinante*=Seek(copy,i,i);

	LiberaMemoria(copy);

        return determinante;
}

int CheckZeroes (struct matrice *matrix){		//Controlla quante righe nulle ha la matrice, restituendo il risultato basandosi su quante righe ha la matrice e quante sono non nulle

        struct righe *check=matrix->car.row;
        int counter=0;

        while (check!=NULL){

                check=check->nextrow;
                counter++;

        }

        return (matrix->car.rows)-counter;
}

int rank(struct matrice *matrix){		//Calcola il rango della matrice utilizzando una copia sottoposta a riduzione di Gauss

	struct matrice *copy=NULL;

        copy=Gauss(matrix);

        int partenza,i;
        int contaprop=0;

        contaprop=CheckZeroes(copy);

	LiberaMemoria(copy);

        return ((matrix->car.rows)-contaprop);
}

struct matrice *Transpose (struct matrice *matrix){	//Crea la matrice trasposta

	struct matrice *tr;

	tr=(struct matrice*)malloc(sizeof(struct matrice));

	tr->car.rows=matrix->car.cols;
	tr->car.cols=matrix->car.rows;
	tr->car.row=NULL;
	tr->nextmatrix=NULL;

	int i,j;

	for (i=1;i<=matrix->car.rows;i++){

		for (j=1;j<=matrix->car.cols;j++)
			CreaModifica_Nodo(tr,j,i,Seek(matrix,i,j));

	}

	return tr;

}

void SideRedux(struct matrice *identita,int rigafissa,int rigamod,long double *div){		//Implementazione del metodo di Gauss-Jordan per le inverse, mentre la divisione per righe della matrice di cui si calcola l'inversa
												//ha luogo, questa funzione applica le stesse operazioni anche alla matrice identità, che si trasformerà in matrice inversa
	if (div[1]==1){

		ScambiaRighe(identita,rigafissa,rigamod);
		return;

	}

	if (div[0]==0)
		return;


        struct righe *r1=identita->car.row,*r2=identita->car.row;

        while (r1!=NULL && r1->riga<rigafissa)
                r1=r1->nextrow;
        while (r1!=NULL && r2->riga<rigamod)
                r2=r2->nextrow;

        if ((r1!=NULL && r1->riga==rigafissa) && (r2!=NULL && r2->riga==rigamod)){

                struct colonne *c1=r1->column,*c2=r2->column;
		int j;

                for (j=1;j<=identita->car.cols;j++){

                        if ((c1==NULL || c1->colonna!=j) && (c2!=NULL && c2->colonna==j))
                                c2=c2->nextcolumn;

                        if ((c1!=NULL && c1->colonna==j) && (c2==NULL || c2->colonna!=j)){
                                CreaModifica_Nodo(identita,rigamod,j,-(c1->elemento)*(div[0]));
                                c1=c1->nextcolumn;
                        }

                        if ((c1!=NULL && c1->colonna==j) && (c2!=NULL && c2->colonna==j)){

                                if (AbsVal((c2->elemento)-(c1->elemento)*(div[0]))<epsilon){
                                        c2=c2->nextcolumn;
                                        CancellaElemento(identita,rigamod,j);
                                        c1=c1->nextcolumn;
                                }

                                else {
                                        c2->elemento=(c2->elemento)-(c1->elemento)*(div[0]);
                                        c1=c1->nextcolumn;
                                        c2=c2->nextcolumn;
                                }
                        }
                }

		return;

        }

        if ((r1!=NULL && r1->riga==rigafissa) && (r2==NULL || r2->riga!=rigamod)){

                struct colonne *c1=r1->column;
		int j;

                for (j=1;j<=identita->car.rows;j++){
                        if (c1->colonna==j){
                                CreaModifica_Nodo(identita,rigamod,j,-(c1->elemento)*(div[0]));
                                c1=c1->nextcolumn;
                        }
                }

		return;
	}
}

struct matrice *inv(struct matrice *matrix){		//Calcola l'inversa con Gauss-Jordan, riducendo la matrice di partenza a matrice identità, mentre l'identità si trasforma nell'inversa

	struct matrice *copy=NULL;
	copy=(struct matrice*)malloc(sizeof(struct matrice));

	copy->car.row=NULL;
	copy->nextmatrix=NULL;

	CopiaMatrice(copy,matrix);

        struct matrice *identita=NULL;
	identita=(struct matrice*)malloc(sizeof(struct matrice));

	identita->car.row=NULL;
	identita->nextmatrix=NULL;

        int i,pivot;
        long double *div=NULL;

        for (i=1;i<=matrix->car.rows;i++)
                CreaModifica_Nodo(identita,i,i,1);

        identita->car.rows=identita->car.cols=matrix->car.rows;

        for (pivot=1;pivot < matrix->car.rows;pivot++){

                for (i=pivot+1;i<=matrix->car.rows;i++){
                        div=SottraiRighe(copy,pivot,i,"m");
                        SideRedux(identita,pivot,i,div);
			free(div);
                }

        }

        for (pivot=matrix->car.rows;pivot>1;pivot--){

                for (i=pivot-1;i>=1;i--){
                        div=SottraiRighe(copy,pivot,i,"m");
                        SideRedux(identita,pivot,i,div);
			free(div);
                }
        }

	struct righe *rmatrix=copy->car.row,*rid=identita->car.row;
	struct colonne *cmatrix=NULL,*cid=NULL;

	for (i=1;i<=copy->car.rows;i++){

		cmatrix=rmatrix->column;

		while (rid!=NULL && rid->riga<i)
			rid=rid->nextrow;

		if (rid!=NULL && rid->riga==i){

			cid=rid->column;

			while (cid!=NULL){

				(cid->elemento)=((cid->elemento)/(cmatrix->elemento));
				cid=cid->nextcolumn;

			}
		}

		rmatrix=rmatrix->nextrow;

	}

	LiberaMemoria(copy);

        return identita;
}

int ContaElementi (struct matrice *matrix){		//Conta gli elementi della matrice (utile per salvataggio su file)

	if (matrix->car.row==NULL)
		return 0;

	struct righe *r=matrix->car.row;
	struct colonne *c=NULL;
	int numelem=0;

	while (r!=NULL){

		c=r->column;

		while (c!=NULL){

			numelem++;
			c=c->nextcolumn;

		}

		r=r->nextrow;

	}

	return numelem;

}

void PrintOnFile (struct matrice *matrix,FILE *savefile){		//Scrive una matrice su file

	int numelem=ContaElementi(matrix);

	fprintf(savefile,"%s\n%d\n%d\n%d\n",matrix->car.nome,numelem,matrix->car.rows,matrix->car.cols);

	if (matrix->car.row==NULL)
		return;

	else {

		struct righe *print_r=matrix->car.row;
		struct colonne *print_c=NULL;

		while (print_r!=NULL){

			print_c=print_r->column;

			while (print_c!=NULL){

				fprintf(savefile,"%d  %d   %.4Lf\n",print_r->riga,print_c->colonna,print_c->elemento);
				print_c=print_c->nextcolumn;

			}

			print_r=print_r->nextrow;

		}

	}
}


int SalvaRaccoltaSuFile (struct matrice *matrixlist){		//Salva l'intera raccolta di matrici nel file di autosalvataggio al termine dell'esecuzione del programma

	FILE *savefile=NULL;

	savefile=fopen("autosave.rm","w");

	if (savefile==NULL)
		return 1;

	while (matrixlist!=NULL){

		PrintOnFile(matrixlist,savefile);
		matrixlist=matrixlist->nextmatrix;

	}

	if (fclose(savefile))
		return 2;

}

struct matrice *CreaMatriceDaFile (FILE *loadfile,int *errcode){	//Alloca in memoria una matrice caricata da file

	struct matrice *matrix=NULL;
	matrix=(struct matrice*)malloc(sizeof(struct matrice));
	matrix->car.row=NULL;
	matrix->nextmatrix=NULL;

	int numelem;

	if (fscanf(loadfile,"%s\n",matrix->car.nome)==EOF || fscanf(loadfile,"%d\n",&numelem)==EOF || fscanf(loadfile,"%d\n",&(matrix->car.rows))==EOF || fscanf(loadfile,"%d\n",&(matrix->car.cols))==EOF){

		*errcode=1;
		free(matrix);
		matrix=NULL;
		return matrix;

	}

	if (numelem==0)
		return matrix;

	else {

		int elem=0;
		int newcol,newrow;
		long double val;

		for (elem=0;elem<numelem;elem++){

			if(fscanf(loadfile,"%d  %d   %Lf\n",&newrow,&newcol,&val)==EOF){
				*errcode=2;
				LiberaMemoria(matrix);
				matrix=NULL;
				return matrix;
			}

			CreaModifica_Nodo(matrix,newrow,newcol,val);

		}

	        if (ContaElementi(matrix)<numelem){

               		*errcode=3;
                	LiberaMemoria(matrix);
                	matrix=NULL;
                	return matrix;

		}

       		return matrix;

	}
}

int CaricaRaccoltaDaFile (struct matrice **matrixlist){		//Carica il file di autosalvataggio

	FILE *loadfile=NULL;

	loadfile=fopen("autosave.rm","r");

        if (loadfile==NULL)
                return 1;

	int errcode=0;
	int matrice_eliminata=0;
	struct matrice *newmatrix=NULL;

	while (errcode==0 || errcode==3){

		newmatrix=CreaMatriceDaFile(loadfile,&errcode);

		if (errcode==2 || errcode==3)
			matrice_eliminata=1;

		if (newmatrix!=NULL)
			AggiungiMatriceInTesta(matrixlist,newmatrix);

	}

	fclose(loadfile);

	if (matrice_eliminata==1)
		return 2;

	return 0;
}

void Combine (struct matrice *coefficienti, struct matrice *termini_noti){	//Combina le matrici dei coefficienti e dei termini noti (in sistemi lineari)

	int i;

	long double val;

	(coefficienti->car.cols)++;

	for (i=1;i<=coefficienti->car.rows;i++){

		if ((AbsVal(val=Seek(termini_noti,i,1)))>=epsilon)
			CreaModifica_Nodo(coefficienti,i,coefficienti->car.cols,val);

	}

	return;

}

void Detach (struct matrice *coefficienti){		//Stacca le due matrici restituendo nuovamente la matrice dei coefficienti

	int i;

	for (i=1;i<=coefficienti->car.rows;i++)
		CancellaElemento(coefficienti,i,coefficienti->car.cols);

	(coefficienti->car.cols)--;

	return;

}

void CancellaRaccolta (struct matrice **matrixlist){		//Cancella una lista di matrici

	struct matrice *M=(*matrixlist);

	while ((*matrixlist)!=NULL){

		(*matrixlist)=(*matrixlist)->nextmatrix;
		LiberaMemoria(M);
		M=(*matrixlist);

	}
}

struct matrice *InvertiLista (struct matrice *matrixlist){	//Inverte la lista di matrici trattandola come una pila

	struct matrice *newlist=NULL;
	struct matrice *p=NULL;

	newlist=matrixlist;
	matrixlist=matrixlist->nextmatrix;
	newlist->nextmatrix=NULL;

	while (matrixlist!=NULL){

		p=matrixlist;
		matrixlist=matrixlist->nextmatrix;
		p->nextmatrix=newlist;
		newlist=p;

	}

	return newlist;
}

int SalvaMatriceSuFile (struct matrice *matrix){		//Salva una matrice su file .m

	FILE *savefile=NULL;
	char filename[13];
	strcpy(filename,matrix->car.nome);
	strcat(filename,".m");

	savefile=fopen(filename,"w");

	if (savefile==NULL)
		return 1;

	PrintOnFile(matrix,savefile);

	if (fclose(savefile))
		return 2;

	return 0;

}

int CaricaMatriceDaFile (struct matrice **matrixlist,char name[]){	//Carica una matrice da file .m

	FILE *loadfile=NULL;
	char filename[13];

	strcpy(filename,name);

	strcat(filename,".m");

	loadfile=fopen(filename,"r");

	if (loadfile==NULL)
		return 1;

	int errcode=0;
	struct matrice *matrix=NULL,*sovrascrivi=NULL;

	matrix=CreaMatriceDaFile(loadfile,&errcode);

	fclose(loadfile);

	if (matrix!=NULL){

		if ((sovrascrivi=SelezionaMatrice(*matrixlist,matrix->car.nome))!=NULL){

			CopiaMatrice(sovrascrivi,matrix);
			LiberaMemoria(matrix);
			matrix=NULL;

		}

		else
			AggiungiMatriceInTesta(matrixlist,matrix);

		return 0;

	}

	else
		return 2;
}

void Menu(){

	system ("clear");

	printf("***********************************************************************************************************MATRIX CALCULATOR******************************************************************************************************************");

	printf("\n                                                                                          Progetto per Fondamenti di Informatica AA 2019/2020");

	printf("\n**********************************************************************************************************************************************************************************************************************************************\n**********************************************************************************************************************************************************************************************************************************************");

	printf("\n                                                                                                            MENU' PRINCIPALE");

	printf("\n                                                                                            Scegli un comando tramite numero identificativo");

	printf("\n\n\n1)Aggiungi una nuova matrice alla raccolta\n2)Cancella una matrice dalla raccolta\n3)Visualizza la raccolta\n4)Operazioni su più matrici\n5)Operazioni su una matrice\n6)Calcolo di sistemi lineari\n7)Gestione file\n0)Esci dal programma");

	printf("\n\n\nInserisci un comando: ");

}

void SubMenuOPM(){

	system("clear");

	printf("\n\n1)Somma più matrici");

	printf("\n\n2)Moltiplica più matrici");

	printf("\n\n0)Torna al menù precedente");

	printf("\n\nInserisci un comando: ");

}

void SubMenuOPS(){

	system("clear");

	printf("\n\n1)Calcola il determinante di una matrice\n2)Calcola il rango di una matrice\n3)Calcola la trasposta di una matrice\n4)Calcola l'inversa di una matrice\n0)Torna al menù precedente");

	printf("\n\nInserisci un comando...");

}

void LinSysMenu(){

	printf("**********************************************************************************************************************************************************************************************************************************");

	printf("\n                                                                                 	TOOL PER LA RISOLUZIONE DI SISTEMI LINEARI\n");

	printf("********************************************************************************************************************************************************************************************************************************");

}

void FileMenu(){

	printf("                                                                                                      Gestione File\n");
	printf("											  Salva o carica le tue matrici da file .m\n\n\n");
	printf("1)Salva matrice su file\n2)Carica matrice da file\n0)Torna al menù principale\n\nInserire scelta: ");

}




int main(){

	int scelta=0,ch;
	int fileerror=0;
	int invertibile=0;
	char nome[11];
	struct matrice *matrixlist=NULL,*newmatrix=NULL,*sovrascrivi=NULL;
	struct sumprod *namestack=NULL;

	struct matrice *linearsystem=NULL,*matrix=NULL;		//Dichiarazioni di variabili utilizzate nel tool per sistemi lineari
	int equazioni,incognite;
	int i;

	int rankA,rankC;

	long double val;

	system("clear");

	printf("\n\n\n\n\n\n\n");

	printf("Vuoi caricare dal file di autosalvataggio di Matrix Calculator?\n\n1)Sì\n2)No\n\nInserire Scelta: ");

	scanf("%d",&scelta);
	while (ch=getchar()!='\n' && ch!=EOF);

	if (scelta==1){

		if((fileerror=CaricaRaccoltaDaFile(&matrixlist))==1)
			printf("\n\nImpossibile accedere al file di autosalvataggio. Possibili cause:\n\n1)Primo accesso al programma (il file non è mai stato creato)\n\n2)Il file non viene aperto correttamente, è corrotto, oppure è stato modificato dall'utente in modo improprio\n\n");
		if(fileerror==2)
			printf("\n\nI dati di alcune matrici sono stati corrotti o modificati in modo improprio dall'utente\n\n");
		if (fileerror==0 && matrixlist!=NULL)
			printf("\n\nIl file di autosalvataggio è stato aperto correttamente! Le matrici sono state tutte salvate in raccolta!\n\n");
		if (fileerror==0 && matrixlist==NULL)
			printf("\n\nIl file di autosalvataggio è vuoto.\n\n");
		if (matrixlist!=NULL)
			matrixlist=InvertiLista(matrixlist);

	}

	printf("\n\n\n\nPremi Invio per avviare il programma...");
	getchar();

	for(;;){
		Menu();
		scanf("%d",&scelta);

		while (ch=getchar()!='\n' && ch!=EOF);

		if (scelta==0){

			system("clear");

			if (matrixlist!=NULL){

				fileerror=SalvaRaccoltaSuFile(matrixlist);

				if (fileerror==0)
					printf("\n\n\n\n\n\n\n\n\n\n\n\nIl file di autosalvataggio è stato salvato correttamente!");
				if (fileerror==1)
					printf("\n\n\n\n\n\n\n\n\n\n\n\nIl file di autosalvataggio non è stato aperto correttamente");
				if (fileerror==2)
					printf("\n\n\n\n\n\n\n\n\n\n\n\nIl file di autosalvataggio non è stato chiuso correttamente");


			}

			printf("\n\nPremi Invio per uscire dal programma...");
			getchar();
			CancellaRaccolta(&matrixlist);
			exit(0);

		}

		if (scelta==1){

			system("clear");
			printf("\n\n");
			newmatrix=NuovaMatrice();
			GiveName(newmatrix);
			if ((sovrascrivi=SelezionaMatrice(matrixlist,newmatrix->car.nome))!=NULL){

				scelta=0;

				while (scelta!=1 && scelta!=2){
					printf("\n\nEsiste già una matrice con questo nome, vuoi sovrascriverla?\n\n1)Sì\n2)No\n\nScelta: ");
					scanf("%d",&scelta);

					while ((ch=getchar())!='\n' && ch!=EOF);

					if (scelta==1){

						CopiaMatrice(sovrascrivi,newmatrix);
						LiberaMemoria(newmatrix);

					}
				}

			}

			else
				AggiungiMatriceInTesta(&matrixlist,newmatrix);

			newmatrix=NULL;
			printf("Premi Invio per continuare...");
			getchar();

		}

		if (scelta==2){

			system("clear");
			printf("\n\nInserisci il nome della matrice che vuoi cancellare dalla raccolta: ");

			fgets(nome,11,stdin);
			if (CancellaNewLine(nome)==1)
                        	while((ch=getchar())!='\n' && ch!=EOF);


			if (CancellaMatrice(&matrixlist,nome)==1)
				printf("\n\nLa matrice da te selezionata non è in raccolta\n\n");

			printf("Premi Invio per continuare...");
	                while((ch=getchar())!='\n' && ch!=EOF);

		}

		if (scelta==3){

			system("clear");

			if (matrixlist==NULL)
				printf("\n\nNon ci sono matrici in raccolta, inserisci una nuova matrice usando il comando 1) o caricala da file\n\n");
			else
				StampaRaccolta(matrixlist);
			printf("Premi Invio per continuare...");
	                getchar();

		}

		if (scelta==4){

			for (;;){
				SubMenuOPM();
				scanf("%d",&scelta);

				while (ch=getchar()!='\n' && ch!=EOF);

				strcpy(nome," ");		//Evita l'allocazione di una stringa casuale e fa entrare il programma nel ciclo

				if (scelta==1){

					printf("\n\nInserisci i nomi delle matrici che vuoi sommare, se vuoi fermarti inserisci il comando 'stop'\n\n");

					while (strcmp(nome,"stop")){

						printf("Nome: ");

						fgets(nome,11,stdin);
						if (CancellaNewLine(nome)==1)
							while((ch=getchar())!='\n' && ch!=EOF);

						if ((newmatrix=SelezionaMatrice(matrixlist,nome))!=NULL)
							AggiungiOperando(&namestack,nome);
						else{

							if (strcmp(nome,"stop")!=0)
								printf("\n\nLa matrice selezionata non è in raccolta\n\n");

						}
					}

					if (namestack==NULL){
						printf("\n\nNon hai inserito nessuna matrice da sommare,impossibile eseguire la somma\n\n");
						scelta=-1;
					}

					else if (namestack!=NULL && namestack->nextname==NULL){
						printf("\n\nHai inserito una sola matrice, impossibile eseguire la somma\n\n");
						Pop(&namestack,nome);
						scelta=-1;
					}
					else {
						newmatrix=Somma(matrixlist,&namestack);

						if (newmatrix==NULL)
							printf("\n\nLe dimensioni delle matrici non sono uguali, impossibile calcolare la somma di queste matrici.");

						if (newmatrix!=NULL){

							printf("\n\n");

							print(newmatrix);

							printf("\n\nVuoi inserire la nuova matrice in lista?\n1)Sì\n2)No\n\nInserire scelta: ");

							scelta=0;

							while (scelta!=1 && scelta!=2){

								scanf("%d",&scelta);
								while (ch=getchar()!='\n' && ch!=EOF);

							}

							if (scelta==1){

								GiveName(newmatrix);

								if ((sovrascrivi=SelezionaMatrice(matrixlist,newmatrix->car.nome))!=NULL){

                                					scelta=0;

                                					while (scelta!=1 && scelta!=2){

                                        					printf("\n\nEsiste già una matrice con questo nome, vuoi sovrascriverla?\n\n1)Sì\n2)No\n\nScelta: ");
                                        					scanf("%d",&scelta);

										while (ch=getchar()!='\n' && ch!=EOF);

                                        					if (scelta==1){

                                                					CopiaMatrice(sovrascrivi,newmatrix);
											LiberaMemoria(newmatrix);

										}

										if (scelta==2)
											LiberaMemoria(newmatrix);

                                					}

                        					}

								else
									AggiungiMatriceInTesta(&matrixlist,newmatrix);

							}

							else if (scelta==2)
								LiberaMemoria(newmatrix);
						}

					}

					newmatrix=NULL;
					scelta=-1;

					printf("\n\nPremi Invio per continuare...");
					getchar();

				}

				if (scelta==2){

                                        printf("\n\nInserisci i nomi delle matrici che vuoi moltiplicare, se vuoi fermarti inserisci il comando 'stop'\n\n");

                                        while (strcmp(nome,"stop")){

                                                printf("Nome: ");

                                                fgets(nome,11,stdin);
						if (CancellaNewLine(nome)==1)
							while((ch=getchar())!='\n' && ch!=EOF);

                                                if ((newmatrix=SelezionaMatrice(matrixlist,nome))!=NULL)
                                                        AggiungiOperando(&namestack,nome);
                                                else{
                                                        if (strcmp(nome,"stop")!=0)
                                                                printf("\n\nLa matrice selezionata non è in raccolta\n\n");
						}
                                        }

                                        if (namestack==NULL){
                                                printf("\n\nNon hai inserito nessuna matrice da moltiplicare,impossibile eseguire il prodotto\n\n");
                                                scelta=-1;
                                        }

                                        else if (namestack!=NULL && namestack->nextname==NULL){
                                                printf("\n\nHai inserito una sola matrice, impossibile eseguire il prodotto\n\n");
						Pop(&namestack,nome);
                                                scelta=-1;
                                        }
                                        else {

						namestack=ListaProdotto(namestack);

                                                newmatrix=Prodotto(matrixlist,&namestack);

						if (newmatrix==NULL)
							printf("\n\nLe dimensioni delle matrici non rispettano le condizioni di uguaglianza interna, impossibile calcolare il prodotto tra queste matrici.");

						if (newmatrix!=NULL){

                                                	printf("\n\n");

                                                	print(newmatrix);

							scelta=0;

							while (scelta!=1 && scelta!=2){

                                                		printf("\n\nVuoi inserire la nuova matrice in lista?\n1)Sì\n2)No\n\nInserire scelta: ");
                                                		scanf("%d",&scelta);
								while ((ch=getchar())!='\n' && ch!=EOF);

							}

                                                	if (scelta==1){
								GiveName(newmatrix);

                                                        	if ((sovrascrivi=SelezionaMatrice(matrixlist,newmatrix->car.nome))!=NULL){

                                                                	scelta=0;

                                                                	while (scelta!=1 && scelta!=2){

                                                                        	printf("\n\nEsiste già una matrice con questo nome, vuoi sovrascriverla?\n\n1)Sì\n2)No\n\nScelta: ");
                                                                        	scanf("%d",&scelta);

                                                                        	while (ch=getchar()!='\n' && ch!=EOF);

                                                                        	if (scelta==1){

                                                                                	CopiaMatrice(sovrascrivi,newmatrix);
											LiberaMemoria(newmatrix);

										}

                                                                        	if (scelta==2)
                                                                                	LiberaMemoria(newmatrix);

                                                                	}

                                                        	}

                                                        	else
                                                                	AggiungiMatriceInTesta(&matrixlist,newmatrix);

                                                	}



                                                	else if (scelta==2)
                                                        	LiberaMemoria(newmatrix);
                                        	}

					}

					scelta=-1;

					newmatrix=NULL;

                                        printf("\n\nPremi Invio per continuare...");
                                        getchar();

                                }



				if (scelta==0){
					scelta=-1;
					break;
				}
			}
		}

		if (scelta==5){

			for(;;){

				SubMenuOPS();
				scanf("%d",&scelta);

				while (ch=getchar()!='\n' && ch!=EOF);

				if (scelta==1){

					system("clear");

					printf("\n\nInserisci il nome della matrice di cui vuoi calcolare il determinante: ");

					fgets(nome,11,stdin);
					if (CancellaNewLine(nome)==1)
						while((ch=getchar())!='\n' && ch!=EOF);

					if ((newmatrix=SelezionaMatrice(matrixlist,nome))!=NULL){

						if (newmatrix->car.rows==newmatrix->car.cols){
							printf("\n\n");
							print(newmatrix);
							printf("\n\nIl determinante della matrice è %.4Lf",det(newmatrix));
						}

						else
							printf("\n\nLa matrice scelta non è quadrata!");

						newmatrix=NULL;

					}

					else

						printf("\n\nLa matrice selezionata non esiste");

					printf("\n\nPremi Invio per continuare...");
					getchar();

					scelta=-1;

				}

				if (scelta==2){

					system("clear");

					printf("\n\nInserisci il nome della matrice di cui vuoi calcolare il rango: ");

                                        fgets(nome,11,stdin);
                                        if (CancellaNewLine(nome)==1)
						while((ch=getchar())!='\n' && ch!=EOF);

                                	if ((newmatrix=SelezionaMatrice(matrixlist,nome))!=NULL){

                                	        printf("\n\n");
                                	        print(newmatrix);
                                	        printf("\n\nIl rango della matrice è %d",rank(newmatrix));

						newmatrix=NULL;

                                	}

                                	else

                                        	printf("\n\nLa matrice selezionata non esiste");

                                	printf("\n\nPremi Invio per continuare...");
					getchar();

					scelta=-1;

				}

				if (scelta==3){

					system("clear");

					printf("\n\nInserisci il nome della matrice di cui vuoi calcolare la trasposta: ");

                                        fgets(nome,11,stdin);
                                        if (CancellaNewLine(nome)==1)
						while((ch=getchar())!='\n' && ch!=EOF);

					if ((newmatrix=SelezionaMatrice(matrixlist,nome))!=NULL){

						printf("\n\n");

						print(newmatrix);

						newmatrix=Transpose(newmatrix);

						printf("\n\nLa trasposta della matrice selezionata è: \n\n");

						print(newmatrix);

						while (scelta!=1 && scelta!=2){

                                                        printf("\n\nVuoi inserire la nuova matrice in lista?\n1)Sì\n2)No\n\nInserire scelta: ");
                                                        scanf("%d",&scelta);
							while ((ch=getchar())!='\n' && ch!=EOF);

                                                }

                                                if (scelta==1){
                                                        GiveName(newmatrix);

                                                        if ((sovrascrivi=SelezionaMatrice(matrixlist,newmatrix->car.nome))!=NULL){

                                                                scelta=0;

                                                                while (scelta!=1 && scelta!=2){

                                                                        printf("\n\nEsiste già una matrice con questo nome, vuoi sovrascriverla?\n\n1)Sì\n2)No\n\nScelta: ");
                                                                        scanf("%d",&scelta);

                                                                        while (ch=getchar()!='\n' && ch!=EOF);

                                                                        if (scelta==1){

                                                                                CopiaMatrice(sovrascrivi,newmatrix);
										LiberaMemoria(newmatrix);

									}

                                                                        if (scelta==2)
                                                                                LiberaMemoria(newmatrix);

                                                                }

                                                        }

                                                        else
                                                                AggiungiMatriceInTesta(&matrixlist,newmatrix);

                                                }

                                                else if (scelta==2)
                                                        LiberaMemoria(newmatrix);
                                        }

					else
						printf("\n\nLa matrice selezionata non esiste.");

					newmatrix=NULL;

					printf("\n\nPremi Invio per continuare...");
					getchar();
				}

				if (scelta==4){

					system("clear");

					printf("\n\nInserisci il nome della matrice di cui vuoi calcolare l'inversa: ");

					fgets(nome,11,stdin);
                                        if (CancellaNewLine(nome)==1)
						while((ch=getchar())!='\n' && ch!=EOF);

					if ((newmatrix=SelezionaMatrice(matrixlist,nome))!=NULL){

						if (newmatrix->car.rows==newmatrix->car.cols){

							if (AbsVal(det(newmatrix))>=epsilon){

								printf("\n\n");
								print(newmatrix);
								newmatrix=inv(newmatrix);
								printf("L'inversa della matrice selezionata è:\n\n");
								print(newmatrix);
								invertibile=1;

							}

							else
								printf("\n\nLa matrice selezionata non è invertibile");

						}

						else
							printf("\n\nLa matrice selezionata non è invertibile!");

						if (invertibile==1){

							while (scelta!=1 && scelta!=2){

                                                        	printf("\n\nVuoi inserire la nuova matrice in lista?\n1)Sì\n2)No\n\nInserire scelta: ");
                                                        	scanf("%d",&scelta);
								while (ch=getchar()!='\n' && ch!=EOF);

							}

                                                	if (scelta==1){
                                                        	GiveName(newmatrix);

                                                        	if ((sovrascrivi=SelezionaMatrice(matrixlist,newmatrix->car.nome))!=NULL){

                                                                	scelta=0;

                                                                	while (scelta!=1 && scelta!=2){

                                                                        	printf("\n\nEsiste già una matrice con questo nome, vuoi sovrascriverla?\n\n1)Sì\n2)No\n\nScelta: ");
                                                                        	scanf("%d",&scelta);

                                                                        	while (ch=getchar()!='\n' && ch!=EOF);

                                                                        	if (scelta==1){

                                                                                	CopiaMatrice(sovrascrivi,newmatrix);
											LiberaMemoria(newmatrix);

										}

                                                                        	if (scelta==2)
                                                                                	LiberaMemoria(newmatrix);

                                                                	}

                                                        	}

                                                        	else
                                                                	AggiungiMatriceInTesta(&matrixlist,newmatrix);

                                                	}

                                                	else if (scelta==2)
                                                        	LiberaMemoria(newmatrix);

							invertibile=0;
                                        	}
					}

					else
						printf("\n\nLa matrice selezionata non esiste.\n\n");

					newmatrix=NULL;

					scelta=-1;

					printf("Premi Invio per continuare...");
					getchar();

				}

				if (scelta==0){
					scelta=-1;
					break;
				}
			}
		}

		if (scelta==6){

			system("clear");
                        LinSysMenu();
                        printf("\n\nInserisci la matrice A dei coefficienti (le righe indicano le equazioni e le colonne indicano i coefficienti delle incognite)...\n\n");
                        newmatrix=NuovaMatrice();
                        equazioni=newmatrix->car.rows;
                        incognite=newmatrix->car.cols;

                        if (equazioni>incognite){

				printf("Il sistema è sovradeterminato, non ammette soluzioni comuni a tutte le equazioni");
                                LiberaMemoria(newmatrix);
                                newmatrix=NULL;

                        }

                        else {
                                strcpy(newmatrix->car.nome,"A");

                                AggiungiMatriceInTesta(&linearsystem,newmatrix);

                                newmatrix=NULL;
                                newmatrix=(struct matrice*)malloc(sizeof(struct matrice));

                                newmatrix->car.row=NULL;
                                newmatrix->nextmatrix=NULL;
                                newmatrix->car.rows=equazioni;
                                newmatrix->car.cols=1;
                                printf("\n\nInserire la matrice b dei termini noti (il numero di righe è pari al numero di equazioni)...\n\n");

                                for (i=1;i<=equazioni;i++){

                                	printf("Colonna %d: ",i);

                                        scanf("%Lf",&val);
                                        while ((ch=getchar())!='\n' && ch!=EOF);

                                        if (AbsVal(val)>=epsilon)
                                        	CreaModifica_Nodo(newmatrix,i,1,val);

                                        printf("\n\n");

				}

                                strcpy(newmatrix->car.nome,"b");

                                AggiungiMatriceInTesta(&linearsystem,newmatrix);

                                newmatrix=SelezionaMatrice(linearsystem,"A");
                                rankA=rank(newmatrix);

                                matrix=SelezionaMatrice(linearsystem,"b");

                                Combine(newmatrix,matrix);

                                rankC=rank(newmatrix);

                                Detach(newmatrix);

                                if (rankA<rankC)
                                	printf("\n\nIl sistema non ammette soluzioni.");
                                else if (rankA==rankC){

                                	if (rankA<incognite)
                                        	printf("\n\nIl sistema ammette infinite soluzioni");
                                        else if (rankA==incognite){

                                        	newmatrix=inv(newmatrix);
                                                strcpy(newmatrix->car.nome,"inv(A)");
                                                AggiungiMatriceInTesta(&linearsystem,newmatrix);

                                                AggiungiOperando(&namestack,"b");
                                                AggiungiOperando(&namestack,"inv(A)");

                                                newmatrix=Prodotto(linearsystem,&namestack);

						printf("\n\nLa soluzione è unica ed è:\n\n");

						print(newmatrix);

						LiberaMemoria(newmatrix);

                                        }

                                }

                                CancellaRaccolta(&linearsystem);
                        }

			printf("\n\nPremi Invio per continuare...");
                	getchar();

                }

		if (scelta==7){

			for (;;){

				system("clear");
				FileMenu();

				scanf("%d",&scelta);
				while((ch=getchar())!='\n' && ch!=EOF);

				if (scelta==1){

					system("clear");
					printf("Inserisci il nome della matrice da salvare su file (se un file con lo stesso nome esiste già verrà sovrascritto): ");

					fgets(nome,11,stdin);
					if (CancellaNewLine(nome)==1)
						while((ch=getchar())!='\n' && ch!=EOF);

					if ((newmatrix=SelezionaMatrice(matrixlist,nome))!=NULL)
						SalvaMatriceSuFile(newmatrix);

					newmatrix=NULL;

					scelta=-1;
				}

				if (scelta==2){

					system("clear");
					printf("Inserisci il nome della matrice da caricare da file: ");

					fgets(nome,11,stdin);
					if (CancellaNewLine(nome)==1)
						while((ch=getchar())!='\n' && ch!=EOF);

					fileerror=CaricaMatriceDaFile(&matrixlist,nome);

					if (fileerror==0)
						printf("\n\nLa matrice è stata caricata correttamente!");
					if (fileerror==1)
						printf("\n\nIl file non è stato aperto correttamente.");
					if (fileerror==2)
						printf("\n\nLa matrice non è stata salvata (causa corruzione o modifica impropria al file da parte dell'utente).");

					printf("\n\nPremi Invio per continuare");
					getchar();

					scelta=-1;

				}

				if (scelta==0)
					break;
			}

		}

	}
}


