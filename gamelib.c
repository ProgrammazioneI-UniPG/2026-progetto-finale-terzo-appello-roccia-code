
#include "gamelib.h"
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

static void stampa_dati_giocatori();
static void genera_mappa();
static void inserisci_zona();
static void cancella_zona();
static void stampa_zona();
static void scelta_menu_mappa();
static void stampa_mappa();
static void chiudi_mappa();
static void avanza();
static void indietreggia();
static void cambia_mondo();
static void combatti();
static void raccogli_oggetto();
static void utilizza_oggetto();
static void passa();
static void nomezona();
static void nomenemico();
static void nomeoggetto();
static void stampa_zona_giocatore();
static void read_int();
static void freeListSinglePtr(struct Zona_mondoreale* firstmr, struct Zona_soprasotto* firstss);
static void stampa_menu_combattimento();



struct Giocatore giocatori[4];
struct Giocatore vincitori[3];
struct Zona_mondoreale* prima_zona_mondoreale;
struct Zona_mondoreale* ultima_zona_mondoreale;
struct Zona_mondoreale* scan_zona_mondoreale;
struct Zona_soprasotto* prima_zona_soprasotto;
struct Zona_soprasotto* ultima_zona_soprasotto;
struct Zona_soprasotto* scan_zona_soprasotto;
struct Zona_mondoreale* eddie;
struct Zona_soprasotto* hopper;
struct Zona_mondoreale* pos_bici_mr;
struct Zona_soprasotto* pos_bici_ss;

int n_giocatori=0;
int scelta_stat_modifier=0;
int scelta_map_modifier=0;
int dado=0;
int con_undici=0;
int contatore=0;
int map_generated=0;
int map_status=0;
int contademo=0;
int contaturno=1;
int scelta_giocatore=0;
int scelta_ogg=0;
int g_random=0;
int contavanza=0;
int contacombattimento=0;
int combattimento=0;
int contaoggetti=0;
int contaoggetti_stat=0;
int contaprimavolta=0;
int player_hp=25;
int nemico_hp=0;
int attacco_nemico=0;
int choice_combat=0;
int sceltazona=0;
int partita_vinta=0;
int cz=0;
int scelta_sicura=0;
int conta_zona_corrente=1;
int uso_schitarrata=0;


void clear_my_buffer(){
    int c;
    while ((c = getchar()) != '\n' && c != EOF) { }
}

static void read_int(int *num) {
    int ris;
    char c;
    while (1) {
        ris = scanf("%d", num);
        if (ris == 1) {
            c = getchar();
            if (c == '\n') {
                return; 
            } 
        } else {
            clear_my_buffer();
            printf("\nERRORE: Devi inserire un numero intero, ripsondi qui: ");
        }
    }
}

static void delplayer() {
    for (int i = g_random; i < n_giocatori; i++) {
        giocatori[i] = giocatori[i + 1];
    }
    n_giocatori--;
}


void menu_di_gioco(){
    int scelta=0;
    while (1){
        if (contaprimavolta==1){
                    printf("***************\t GIOCO IMPOSTATO CORRETTAMENTE \t***************\n");
        }
        else
            printf("***************\t BENVENUTO NEL GIOCO \t***************\n");
        printf("Selezionare da tastiera le seguenti opzioni:\n");
        printf("\t1) Imposta gioco\n");
        printf("\t2) Gioca (dopo aver impostato il gioco): la mappa deve contenere esattamente 15 zone e ci deve essere 1 solo demotorzone nel soprasotto\n");
        printf("\t3) Esci dal gioco\n");
        printf("\t4) Visualizza i crediti\n");
        printf("Risposta: ");
        read_int(&scelta);
        printf("Premere invio per continuare...");
        clear_my_buffer();
        system("clear");

        if (scelta<1) {
            scelta=0;
            printf("\nERRORE: INPUT NON VALIDO - SELEZIONARE DA TASTIERA SOLO I VALORI NUMERICI POSSIBILI.\n\n");
        }
        else if (scelta >= 1 && scelta <= 4){     
            if (scelta==2 && contaprimavolta ==0){
                printf("\nERRORE: INPUT NON VALIDO - DEVI PRIMA IMPOSTARE IL GIOCO.\n\n");
            } else
                break;       
        }
       
        else{
            printf("\nERRORE: INPUT NON VALIDO - SELEZIONARE DA TASTIERA SOLO I VALORI NUMERICI POSSIBILI.\n\n");
        }

    }
    system("clear");
     switch (scelta) {
        case 1:
            imposta_gioco();
            break;
        case 2:
            if(contatore == 15 && contademo ==1){
                gioca();
            } 
            else if (contatore !=15){
                if(contatore<15){
                    printf("ci sono in totale meno di 15 zone\n");
                }else {
                    printf("ci sono troppe zone (più di 15)\n");
                }
                printf("Verrai portato nel menù di impostazione mappa per modificarla...\n\n");
                scelta_menu_mappa();
            } else if(contademo !=1){
                if(contademo<1){
                    printf("Non ci sono demotorzoni, non si può iniziare il gioco\n"); 
                    --contademo;
                } else {
                    printf("ci sono troppi demotorzoni, cancella la zona in cui è presente un demotorzone (deve essere presente solamente 1)\n");
                }
                printf("Verrai portato nel menù di impostazione mappa per modificarla...\n\n");
                scelta_menu_mappa();
            }
            
        case 3:
            system("clear");
            termina_gioco();
        case 4:
            crediti();
            break;
        default:
            printf("\nERRORE: INPUT NON VALIDO - SELEZIONARE DA TASTIERA SOLO I VALORI NUMERICI POSSIBILI.\n\n");

    };
    return;
}

void imposta_gioco() {
    srand(time(NULL));
    do {
        system("clear");
        if(n_giocatori < 1){
            if(n_giocatori<0){
                printf("ERRORE: NUMERO NEGATIVO - VALORE NON AMMESSO\n");
            }
            else if(n_giocatori==0 && contaprimavolta!=0)
                printf("ERRORE: TROPPI POCHI GIOCATORI\n");
        } else if (n_giocatori >4)
                printf("ERRORE: TROPPI GIOCATORI\n");

        printf("***************\t IMPOSTA GIOCO \t***************\n");
        printf("***************\t AGGIUNTA GIOCATORI \t***************\n");
        printf("Ciao! Inserisci il numero di giocatori partecipanti(Max 4): ");
        read_int(&n_giocatori);
    }while (n_giocatori < 1 || n_giocatori > 4);
    printf("Premere invio per continuare...");
    clear_my_buffer();
    for (int i = 0; i < n_giocatori; i++) {
        system("clear");
        giocatori[i].mondo=0;
        giocatori[i].pos_mondoreale=prima_zona_mondoreale;
        printf("Benvenuto giocatore %d , inserisci il tuo nome: ", i+1);
        fgets(giocatori[i].nome, 25, stdin);
        giocatori[i].nome[strcspn(giocatori[i].nome, "\n")]=0;
        printf("statistiche assegnate! Premere invio per continuare...");
        dado = (rand()%20)+1;
        giocatori[i].attacco_psichico = dado;
        dado = (rand()%20)+1;
        giocatori[i].difesa_psichica = dado;
        dado = (rand()%20)+1;
        giocatori[i].fortuna = dado;
        clear_my_buffer();
        printf("Molto bene! le tue statistiche iniziali ti verranno mostrate qui sotto:\n");
        printf("\t Attacco psichico: %d\n", giocatori[i].attacco_psichico);
        printf("\t Difesa psichica: %d\n", giocatori[i].difesa_psichica);
        printf("\t Fortuna: %d\n", giocatori[i].fortuna);
        printf("Premere invio per continuare...");
        clear_my_buffer();
        if ((giocatori[i].attacco_psichico>=4 && giocatori[i].difesa_psichica>=4) && (giocatori[i].attacco_psichico<=17 && giocatori[i].difesa_psichica<=17)) {
            if (giocatori[i].attacco_psichico<=16 && giocatori[i].difesa_psichica<=16 && giocatori[i].fortuna<=13 &&con_undici==0) {
                printf("\nPuoi scegliere se diventare UndiciVirgolaCinque, ma avrai le seguenti statistiche:\n");
                printf("\t Attacco psichico: %d\n", giocatori[i].attacco_psichico+4);
                printf("\t Difesa psichica: %d\n", giocatori[i].difesa_psichica+4);
                printf("\t Fortuna: %d\n", giocatori[i].fortuna+7);
                printf("vuoi diventare?\n");
                printf("\t 1) Si\n");
                printf("\t 0) N0\n");
                printf("Risposta: ");
                read_int(&scelta_stat_modifier);
                if(scelta_stat_modifier==1){
                    giocatori[i].attacco_psichico +=4;
                    giocatori[i].difesa_psichica +=4;
                    giocatori[i].fortuna +=7;
                    strcpy(giocatori[i].nome, "UndiciVirgolaCinque");
                    con_undici=1;
                }
            }
            
            if (strcmp(giocatori[i].nome, "UndiciVirgolaCinque")){
                printf("Puoi scegliere se aumentare o diminuire il tuo attacco e difesa psichica prelevando fino a 3 punti da uno dei due campi. Vuoi modificare i campi?\n");
                printf("\t 1) Si\n");
                printf("\t 0) N0\n");
                printf("Risposta: ");
                read_int(&scelta_stat_modifier);
                if (scelta_stat_modifier==1) {
                    scelta_stat_modifier = 0;
                    printf("Bene! Quale vuoi modificare?:\n ");
                    printf("\t 1) Attacco psichico\n");
                    printf("\t 0) Difesa psichica\n");
                    printf("Risposta: ");
                    read_int(&scelta_stat_modifier);
                    printf("Premere invio per continuare...");
                    while (1) {
                        switch (scelta_stat_modifier) {
                            case 0:
                                giocatori[i].attacco_psichico -=3;
                                giocatori[i].difesa_psichica +=3;
                                break;

                            case 1:
                                giocatori[i].attacco_psichico +=3;
                                giocatori[i].difesa_psichica -=3;
                                break;

                            default:
                                printf("errore: inserire una delle due scelte possibili.");
                        }
                        if (scelta_stat_modifier==1 || scelta_stat_modifier==0)
                            break;
                    }
                }
            }
        }
    }
    contaprimavolta=1;
    system("clear");
    scelta_menu_mappa();  
}


static void scelta_menu_mappa(){
    int scegli=1;
    while(scegli){
        do{
            if(scelta_sicura<0 || scelta_sicura>1){
                system("clear");
                printf("Errore: inserire 1 o 0\n");
                scelta_sicura=0;
            } else if(scelta_sicura==0){
                system("clear");
            }
            printf("***************\t MENU' GENERAZIONE MAPPA \t***************\n\n");
            printf("Adesso il game master ha la possibilità di scegliere le seguenti opzioni per la creazione della mappa:\n");
            printf("Selezionare da tastiera le seguenti opzioni:\n");
            printf("\t1) Genera mappa -> Verranno create le 15 zone della mappa \n");
            printf("\t2) Inserisci zona -> Inserisci una nuova zona nella mappa\n");
            printf("\t3) Cancella zona -> Cancella una zona a piacere nella mappa\n");
            printf("\t4) Stampa mappa -> Stampa tutte le informazioni di tutte le zone create del Mondo Reale o del Soprasotto (a scelta)\n");
            printf("\t5) Stampa zona -> Stampa tutti i campi di una delle zone create del mondo reale o del Soprasotto (a scelta)\n");
            printf("\t6) Chiudi creazione mappa -> Una volta terminata la creazione della mappa si può chiudere e iniziare il gioco, ma solamente dopo averla creata.\n");
            printf("Risposta: ");
            read_int(&scelta_map_modifier);
            printf("Sei sicuro della tua scelta?:\n");
            printf("\t1) Si: continua\n");
            printf("\t0) No: cambia scelta\n");
            printf("Risposta: ");
            read_int(&scelta_sicura);
        }while(scelta_sicura==0);
        printf("Premere invio per continuare...");
        clear_my_buffer();
        system("clear");
        switch(scelta_map_modifier){
           case 1:
                genera_mappa();
                map_generated=1;
                printf("***************\t MAPPA GENERATA \t***************\n\n");
                break;
            case 2:
                if(map_generated){
                    inserisci_zona();
                    printf("\n\n***************\t ZONA INSERITA \t***************\n");
                    break;

                }
                else
                    printf("***************\t MAPPA NON GENERATA \t***************\nDevi generare la mappa prima di poter inserire una zona\n"); 

                scelta_map_modifier=0;
                break;
            case 3:
                if(map_generated){
                    cancella_zona();
                    printf("***************\t ZONA CANCELLATA \t***************\n");
                    break;
                }
                else
                    printf("***************\t MAPPA NON GENERATA \t***************\nDevi generare la mappa prima di cancellare una zona\n\n"); 

                scelta_map_modifier=0;
                break;
            case 4:
                if(map_generated){
                   stampa_mappa();
                   printf("\n\n");
                }
                else
                    printf("***************\t MAPPA NON GENERATA \t***************\nDevi generare la mappa prima di poterla stampare\n\n"); 
                scelta_map_modifier=0;
                break;
            case 5:
                if(map_generated){
                    stampa_zona();
                    break;

                }
                else
                    printf("***************\t MAPPA NON GENERATA \t***************\nDevi generare la mappa prima di poter stampare una zona\n\n"); 

                scelta_map_modifier=0;
                break;
            case 6:
                if(map_generated && contademo && contatore==15)
                    chiudi_mappa();
                else{
                    system("clear");
                    if(map_generated==0)
                        printf("ERRORE - MAPPA NON GENERATA\n\n");
                    else if(contademo==0)
                        printf("ERRORE - DEVE ESSERCI UN DEMOTORZONE\n\n");
                    else if(contatore>15)
                        printf("ERRORE - CI SONO TROPPE ZONE\n\n");
                    else if (contatore<15)
                        printf("ERRORE - NON CI SONO ABBASTANZA ZONE (ALMENO 15)\n\n");
                    break;
                }
                scegli=0;
                break;
            default:
                printf("ERRORE - INSERIRE I VALORI RICHIESTI\n\n");
        } 
    } 
}

static void freeListSinglePtr(struct Zona_mondoreale* firstmr, struct Zona_soprasotto* firstss) {
    struct Zona_mondoreale* tempmr;
    struct Zona_soprasotto* tempss;

    while (firstmr != NULL) {
        tempmr = firstmr;
        firstmr = firstmr->avanti; 
        free(tempmr);
    }

    while (firstss != NULL) {
        tempss = firstss;
        firstss = firstss->avanti;  
        free(tempss);         
    }

    prima_zona_mondoreale = NULL;
    prima_zona_soprasotto = NULL;
    ultima_zona_mondoreale = NULL;
    ultima_zona_soprasotto = NULL;
    
    contademo = 0; 
    contatore = 0;
    map_status = 0;

}

static void genera_mappa(){
    if (prima_zona_mondoreale != NULL || prima_zona_soprasotto != NULL) {
        freeListSinglePtr(prima_zona_mondoreale, prima_zona_soprasotto);
    }
    int tipozona = 0;
    contatore=0;
    for(int i=0;i<15; i++){
        contatore++;
        tipozona=rand()%10;
        struct Zona_mondoreale* nuova_z_mr = (struct Zona_mondoreale*)malloc(sizeof(struct Zona_mondoreale));
        nuova_z_mr->tipo = tipozona;
        nuova_z_mr->nemico = rand()%3;
        
        nuova_z_mr->oggetto = rand()%5;
        if(prima_zona_mondoreale==NULL){
            nuova_z_mr->indietro=NULL;
            nuova_z_mr->avanti=NULL;
            prima_zona_mondoreale=nuova_z_mr;
            ultima_zona_mondoreale=nuova_z_mr;
        } else{
            nuova_z_mr->indietro=ultima_zona_mondoreale;
            ultima_zona_mondoreale->avanti=nuova_z_mr;
            nuova_z_mr->avanti = NULL;
            ultima_zona_mondoreale=nuova_z_mr;
        }        

        struct Zona_soprasotto* nuova_z_ss = (struct Zona_soprasotto*)malloc(sizeof(struct Zona_soprasotto));
        nuova_z_ss->tipo = tipozona;
        nuova_z_ss->nemico = rand()%4;

        if(nuova_z_ss->nemico == 1){
            while(nuova_z_ss->nemico == 1){
                nuova_z_ss->nemico = rand()%4;

            }
        }

        if(nuova_z_ss->nemico==3){
            contademo++;
            if(contademo>1){
                nuova_z_ss->nemico = rand()%3;
                contademo--;
            }
        }

        if(prima_zona_soprasotto==NULL){
            nuova_z_ss->indietro=NULL;
            nuova_z_ss->avanti=NULL;
            prima_zona_soprasotto=nuova_z_ss;
            ultima_zona_soprasotto=nuova_z_ss;
        } else{
            nuova_z_ss->indietro=ultima_zona_soprasotto;
            ultima_zona_soprasotto->avanti=nuova_z_ss;
            nuova_z_ss->avanti = NULL;
            ultima_zona_soprasotto=nuova_z_ss;
        }
    }
}

static void inserisci_zona(){
    while(1){
        int sceltapos=0;
        int nemicomr=0;
        int nemicoss=0;
        int obj=0;

        struct Zona_mondoreale* nuova_z_mr = (struct Zona_mondoreale*)malloc(sizeof(struct Zona_mondoreale));
        struct Zona_soprasotto* nuova_z_ss = (struct Zona_soprasotto*)malloc(sizeof(struct Zona_soprasotto));
        nuova_z_mr->tipo = rand()%10;
        nuova_z_ss->tipo=nuova_z_mr->tipo;

        while(1){
            printf("Inserisci da tastiera il nemico che vuoi inserire(mondo reale)\n");
            printf("\t1)Nessun Nemico\t2)Billi\n\t3)Democane\nRisposta: ");
            read_int(&nemicomr);
            printf("Premere invio per continuare...");
            clear_my_buffer();
            nemicomr--;
            if(nemicomr>2||nemicomr<0){
                system("clear");
                printf("ERRORE - Scelta non valida\n");
            } else break;
        }

        while(1){
            printf("Inserisci da tastiera il nemico che vuoi inserire(soprasotto)\n");
            printf("\t1)Nessun Nemico\t2)Democane\n\t3)Demotorzone\nRisposta: ");
            read_int(&nemicoss);
            printf("Premere invio per continuare...");
            clear_my_buffer();
            if(nemicoss==1){
                nemicoss--;
                break;
            }
            else if(nemicoss>3||nemicoss<=0){
                printf("ERRORE - scelta non valida\n");
            } else break;
        }

        do{
            printf("Inserisci da tastiera l'oggetto che vuoi inserire:\n");
            printf("\t1)Nessun oggetto\t2)Bicicletta\n\t3)Maglia FuocoInferno\t4)Bussola\n\t5)Schitarrata MetallicA\nRisposta: ");
            read_int(&obj);
            obj--;
            if(obj<0 || obj>4)
                printf("ERRORE: scelta non valida\n");
        }while(obj<0 || obj>4);
        
        printf("Premere invio per continuare...");
        clear_my_buffer();

        nuova_z_mr->nemico=nemicomr;
        nuova_z_ss->nemico=nemicoss;
        nuova_z_mr->oggetto=obj;


        do{
            printf("Scegli la posizione in cui vuoi inserire la zona (numero da 1 a %d): ", contatore);
            read_int(&sceltapos); 
            if(sceltapos<0 || sceltapos>contatore){
                system("clear");
                printf("ERRORE: prova di nuovo\n\n");
            }
        }while(sceltapos<0 || sceltapos>contatore);
        printf("Premere invio per continuare...");
        clear_my_buffer();
        
        if(sceltapos==1){
            nuova_z_mr->indietro=NULL;
            nuova_z_mr->avanti=prima_zona_mondoreale;
            nuova_z_mr->avanti->indietro=nuova_z_mr;
            prima_zona_mondoreale=nuova_z_mr;

            nuova_z_ss->indietro=NULL;
            nuova_z_ss->avanti=prima_zona_soprasotto;
            nuova_z_ss->avanti->indietro=nuova_z_ss;
            prima_zona_soprasotto=nuova_z_ss;
            contatore++;

            break;
        } else if (sceltapos==contatore+1){
            nuova_z_mr->indietro=ultima_zona_mondoreale;
            ultima_zona_mondoreale->avanti=nuova_z_mr;
            nuova_z_mr->avanti = NULL;
            ultima_zona_mondoreale=nuova_z_mr;

            nuova_z_ss->indietro=ultima_zona_soprasotto;
            ultima_zona_soprasotto->avanti=nuova_z_ss;
            nuova_z_ss->avanti = NULL;
            ultima_zona_soprasotto=nuova_z_ss;
            contatore++;

            break;
        } else if (sceltapos>1 && sceltapos<contatore+1){
            struct Zona_mondoreale* pscanmr= prima_zona_mondoreale;
            struct Zona_soprasotto* pscanss= prima_zona_soprasotto;

            for(int i=1; i<sceltapos; i++){
                pscanmr=pscanmr->avanti;
                pscanss=pscanss->avanti;
            }

            nuova_z_mr->avanti=pscanmr;
            nuova_z_mr->indietro=pscanmr->indietro;
            pscanmr->indietro->avanti=nuova_z_mr;
            pscanmr->indietro=nuova_z_mr;

            nuova_z_ss->avanti=pscanss;
            nuova_z_ss->indietro=pscanss->indietro;
            pscanss->indietro->avanti=nuova_z_ss;
            pscanss->indietro=nuova_z_ss;
            contatore++;

            break;
        }
        else{
            printf("ERRORE - Posizione non valida\n");
        }
    }

}

static void cancella_zona(){
    if (contatore>0){
        int sz=0;
        while(1){
            do{
                printf("inserisci la posizione della zone che vuoi cancellare(da 1 a %d): ", contatore);
                read_int(&sz);
                printf("Premere invio per continuare...");
                clear_my_buffer();
                if(sz<=0||sz>contatore){
                    system("clear");
                    printf("ERRORE - Scelta non valida\n");
                }
            } while (sz<0 || sz>contatore);

            struct Zona_mondoreale* temp_pzmr = prima_zona_mondoreale;
            struct Zona_soprasotto* temp_pzss=prima_zona_soprasotto;
            if(sz==1){
                if(contatore>1)     
                    temp_pzmr=prima_zona_mondoreale->avanti;
                else
                    temp_pzmr=prima_zona_mondoreale;
                temp_pzmr->indietro=NULL;
                free(prima_zona_mondoreale);
                prima_zona_mondoreale=temp_pzmr;
                if(contatore>1)
                    temp_pzss=prima_zona_soprasotto->avanti;
                else 
                    temp_pzss=prima_zona_soprasotto;
                temp_pzss->indietro=NULL;
                if(prima_zona_soprasotto->nemico==3){
                    contademo=0;
                }
                free(prima_zona_soprasotto);
                prima_zona_soprasotto=temp_pzss;
                contatore--;
                break;
            } else if (sz==contatore){
                temp_pzmr=ultima_zona_mondoreale->indietro;
                temp_pzss=ultima_zona_soprasotto->indietro;
                free(ultima_zona_mondoreale);
                temp_pzmr->avanti=NULL;
                ultima_zona_mondoreale=temp_pzmr;
                if(ultima_zona_soprasotto->nemico==3){
                    contademo=0;
                }
                free(ultima_zona_soprasotto);
                temp_pzss->avanti=NULL;
                ultima_zona_soprasotto=temp_pzss;
                contatore--;
                break;
            } else{
                struct Zona_mondoreale* zona_interna_mr;
                struct Zona_soprasotto* zona_interna_ss;
                for(int i=1; i<sz-1; i++){
                    temp_pzmr=temp_pzmr->avanti;
                    temp_pzss=temp_pzss->avanti;
                }

                zona_interna_mr=temp_pzmr->avanti->avanti;
                free(zona_interna_mr->indietro);
                temp_pzmr->avanti=zona_interna_mr;
                zona_interna_mr->indietro=temp_pzmr;

                zona_interna_ss=temp_pzss->avanti->avanti;
                if(zona_interna_ss->indietro->nemico==3){
                    contademo=0;
                }
                free(zona_interna_ss->indietro);
                temp_pzss->avanti=zona_interna_ss;
                zona_interna_ss->indietro=temp_pzss;
                contatore--;
                break;
            }
        }
    } else
        printf("Non ci sono zone nella mappa!\n\n"), map_status=0;   
}

static void chiudi_mappa(){

    for(int  i=0; i<n_giocatori; i++){
        giocatori[i].pos_mondoreale=prima_zona_mondoreale;
        giocatori[i].pos_soprasotto=prima_zona_soprasotto;
        giocatori[i].mondo=0;
    }

    printf("***************\t CHIUSURA MAPPA EFFETTUATA \t***************\n"); 
    map_status=1;
    menu_di_gioco();
}


static void stampa_mappa(){
    system("clear");
    int scelta_stampa=1;
    cz=0;
    printf("scegli la mappa da stampare: \n");
    printf("\t1)  Mondo Reale\n");
    printf("\t0)  Soprasotto\n");
    printf("Risposta: ");
    read_int(&scelta_stampa);
    system("clear");
    if(scelta_stampa==1){
        scan_zona_mondoreale = prima_zona_mondoreale;
        printf("***************\t MONDO REALE \t***************\n");
        do{
            cz++;
            printf("\tZona %d\n", cz);
            nomezona(scan_zona_mondoreale->tipo);
            printf("\n");
            nomenemico(scan_zona_mondoreale->nemico);
            printf("\n");
            nomeoggetto(scan_zona_mondoreale->oggetto);
            printf("\n\n");
            scan_zona_mondoreale=scan_zona_mondoreale->avanti;
        }while(scan_zona_mondoreale!=NULL);
        printf("***********************************************\n\n");      
    }
    else{ 
        scan_zona_soprasotto = prima_zona_soprasotto;
        printf("***************\t SOPRASOTTO \t***************\n");
        do{
            cz++;
            printf("\tZona %d\n", cz);
            nomezona(scan_zona_soprasotto->tipo);
            printf("\n");
            nomenemico(scan_zona_soprasotto->nemico);
            printf("\n\n\n");
            scan_zona_soprasotto=scan_zona_soprasotto->avanti;
        }while(scan_zona_soprasotto!=NULL);
        printf("***********************************************\n\n");

    }
    printf("premere invio per continuare...");
    clear_my_buffer();
    system("clear");
   return;
}

static void stampa_zona(){
    do{
        system("clear");
        printf("scegli la zona che vuoi vedere (numero da 1 a 15): ");
        read_int(&sceltazona);
    }while(sceltazona<1 || sceltazona>15);
    
    scan_zona_mondoreale=prima_zona_mondoreale;
    scan_zona_soprasotto=prima_zona_soprasotto;
    for(int i=0; i<sceltazona-1; i++){
        scan_zona_mondoreale=scan_zona_mondoreale->avanti;
        scan_zona_soprasotto=scan_zona_soprasotto->avanti;
    }
    printf("Zona %d Mondo Reale:\n\n", sceltazona);
    nomezona(scan_zona_mondoreale->tipo);
    printf("\n");
    nomenemico(scan_zona_mondoreale->nemico);
    printf("\n");
    nomeoggetto(scan_zona_mondoreale->oggetto);
    printf("\n\n");
    printf("Zona %d Soprasotto\n\n", sceltazona);
    nomezona(scan_zona_soprasotto->tipo);
    printf("\n");
    nomenemico(scan_zona_soprasotto->nemico);
    printf("\n\n");
    printf("Premere invio per continuare...");
    clear_my_buffer();
}

static void stampa_zona_giocatore(struct Zona_mondoreale* zone_mr, struct Zona_soprasotto* zone_ss, int world){
     if(world==0){
        printf("info zona Mondo Reale:\n\n");
        printf("zona: %d\n", conta_zona_corrente);
        nomezona(zone_mr->tipo);
        printf("\n");
        nomenemico(zone_mr->nemico);
        printf("\n");
        nomeoggetto(zone_mr->oggetto);
        printf("\n\n");
    }
    else{
        printf("info zona Soprasotto\n\n");
        nomezona(zone_ss->tipo);
        printf("\n");
        nomenemico(zone_ss->nemico);
        printf("\n\n");
    }
}

static void stampa_dati_giocatore(){
    printf("\tNome giocatore: %s\n", giocatori[g_random].nome);
    printf("\tMondo corrente: ");
    if(giocatori[g_random].mondo==0){
            printf("mondo reale\n");
    }else{
            printf("Soprasotto\n");
    }
    printf("\tPunti vita: %d\n", player_hp);
    printf("\t Attacco psichico: %d\n", giocatori[g_random].attacco_psichico);
    printf("\t Difesa psichica: %d\n", giocatori[g_random].difesa_psichica);
    printf("\t Fortuna: %d\n", giocatori[g_random].fortuna);
    if(giocatori[g_random].zaino[0]==0){
        printf("\tZaino: vuoto\n\n");
    } else{
            printf("\toggetti contenuti nello zaino: \n");
            for(int c=0; c<3; c++){
                nomeoggetto(giocatori[g_random].zaino[c]);
                printf(": ");
                switch (giocatori[g_random].zaino[c]){
                    case 0:
                        printf("non fa niente\n");
                        break;
                    case 1:
                        printf("Puoi effettuare il viaggio rapido in qualsiasi zona nel tuo mondo\n");
                        break;
                    case 2:
                        printf("Se te la metti sei troppo figo; ricevi un bonus del 30 per cento in più di punti fortuna(se non sei UndiciVirgolaCinque)\n");
                        break;
                    case 3:
                        printf("E' una bussola! Ti dice le informazioni della prossima zona\n");
                        break;
                    case 4:
                        printf("ATTENZIONE, USA QUESTO OGGETTO A TUO RISCHIO E PERICOLO: verrai impossessato dallo spirito chitarristico dal signore supremo dei downstroke, GIACOMO ETTOCAMPO. \n\t questo ti darà una potenza sovrumana e diventerai il mastro delle barbie. E potrai sconfiggere ogni nemico in un solo colpo... Ma non prima di aver detto 228 volte la parola 'YEAH'.\n");
                        break;
                }
            }
        }
}

static void stampa_dati_giocatori(){
    printf("Stats giocatore:\n");
    for(int i=0; i<n_giocatori; i++){
        printf("\tNome giocatore: %s\n", giocatori[i].nome);
        printf("\tMondo corrente: ");
        if(giocatori[i].mondo==0){
            printf("mondo reale\n");
        }else{
            printf("Soprasotto\n");
        }
        printf("\t Attacco psichico: %d\n", giocatori[i].attacco_psichico);
        printf("\t Difesa psichica: %d\n", giocatori[i].difesa_psichica);
        printf("\t Fortuna: %d\n", giocatori[i].fortuna);
        if(giocatori[i].zaino[0]==0){
            printf("\tZaino: vuoto\n");
        } else{
            printf("\toggetti contenuti nello zaino: \n");
            for(int c=0; c<3; c++){
                nomeoggetto(giocatori[i].zaino[c]);
                printf(": ");
                switch (giocatori[i].zaino[c]){
                    case 0:
                        printf("Non ha alcun utilizzo speciale;\n");
                        break;
                    case 1:
                        printf("Puoi effettuare il viaggio rapido in qualsiasi zona nel tuo mondo;\n");
                        break;
                    case 2:
                        printf("Se te la metti sei troppo figo; ricevi un bonus del 30 per cento in più di punti fortuna(se non sei UndiciVirgolaCinque), ma puoi indossarla solo una volta;\n");
                        break;
                    case 3:
                        printf("E' una bussola! Ti dice le informazioni della prossima zona;\n");
                        break;
                    case 4:
                        printf("ATTENZIONE, USA QUESTO OGGETTO A TUO RISCHIO E PERICOLO: verrai impossessato dallo spirito chitarristico dal signore supremo dei downstroke, GIACOMO ETTOCAMPO. \n\t questo ti darà una potenza sovrumana e diventerai il mastro delle barbie. E potrai sconfiggere ogni nemico in un solo colpo... Ma non prima di aver detto 228 volte la parola 'YEAH'.\n");
                        break;
                }
            }
        }
    }
    printf("\n\n\n\n");
}

static void nomezona(int zona){
    printf("\t- ");
    switch(zona) {
        case 0:
            printf("Bosco");
            break;
        case 1:
            printf("Scuola");
            break;
        case 2:
            printf("Laboratorio");
            break;
        case 3:
            printf("Caverna");
            break;
        case 4:
            printf("Strada");
            break;
        case 5:
            printf("Giardino");
            break;
        case 6:
            printf("Supermercato");
            break;
        case 7:
            printf("Centrale Elettrica");
            break;
        case 8:
            printf("Deposito Abbandonato");
            break;
        case 9:
            printf("Stazione di Polizia");
            break;
    }
}

static void nomenemico(int nem){
    printf("\t- ");
    switch(nem) {
        case 0:
            printf("Nessun Nemico");
            break;
        case 1:
            printf("Billi");
            break;
        case 2:
            printf("Democane");
            break;
        case 3:
            printf("Demotorzone");
            break;
    }
}

static void nomeoggetto(int ogg){
    printf("\t- ");
    switch(ogg) {
        case 0:
            printf("Nessun oggetto");
            break;
        case 1:
            printf("Bicicletta");
            break;
        case 2:
            printf("Maglia FuocoInferno");
            break;
        case 3:
            printf("Bussola");
            break;
        case 4:
            printf("Schitarrata MetallicA");
            break;
    }
}


void gioca(){
    int gioca=1;
    int contamosse=0;
    contavanza=0;
    combattimento=0;

    while(gioca){
        while(1){
            g_random= rand()%n_giocatori;
            printf("***************\t TURNO %d \t***************\n\n", contaturno);
            printf("giocatore %d\n", g_random+1);
            printf("mossa %d\n", contamosse+1);
            stampa_dati_giocatore();            
            stampa_zona_giocatore(giocatori[g_random].pos_mondoreale, giocatori[g_random].pos_soprasotto, giocatori[g_random].mondo);
            printf("\n");
            printf("Benvenuto %s, ogni quattro mosse verrà cambiato turno scegli una delle seguenti opzioni:\n", giocatori[g_random].nome);
            printf("\t0) Torna al menu -> torna al menu di gioco per metterlo in pausa, chiuderlo o iniziarlo da capo;\n");
            printf("\t1) Avanza -> passa alla prossima zona; \n");
            printf("\t2) Indietreggia -> passa alla zona precedente;\n");
            if(giocatori[g_random].mondo==0)
                printf("\t3) Cambia mondo -> passa al soprasotto;\n");
            else
                printf("\t3) Cambia mondo -> passa al mondo reale;\n");
            printf("\t4) Raccogli oggetto (se presente nella zona);\n");
            printf("\t5) Utilizza oggetto (se presente nello zaino);\n");
            printf("\t6) Combatti -> inizia un combattimento se è presente un nemico;\n");
            printf("\t7) Passa il turno;\n");
            printf("Risposta: ");
            read_int(&scelta_giocatore);
            contamosse++;
            if(contamosse%3==0){
                contamosse=0;
                passa();
            }
            printf("Premere invio per continuare...");
            clear_my_buffer();
            system("clear");
            switch(scelta_giocatore){
                case 0:
                    menu_di_gioco();
                    break;
                case 1:
                    if(giocatori[g_random].mondo==0){
                        if(giocatori[g_random].pos_mondoreale->nemico ==0 ){
                            contavanza++;
                            conta_zona_corrente++;
                            avanza(); 
                        } 
                        else{
                            printf("Attenzione, è presente un nemico nella zona in cui ti trovi: sconfiggilo per poter andare avanti\n");
                            contamosse--;
                        }
                    } else{
                        if(giocatori[g_random].pos_soprasotto->nemico ==0 ){
                            contavanza++;
                            conta_zona_corrente++;
                            avanza(); 
                        } 
                        else{
                            printf("Attenzione, è presente un nemico nella zona in cui ti trovi: sconfiggilo per poter andare avanti\n");
                            contamosse--;
                        }
                    } 
                    
                    break;
                case 2:
                    if(giocatori[g_random].mondo==0){
                        if(giocatori[g_random].pos_mondoreale->nemico ==0 ){
                            conta_zona_corrente--;
                            indietreggia(); 
                        } 
                        else{
                            printf("Attenzione, è presente un nemico nella zona in cui ti trovi: sconfiggilo per poter andare indietro\n");
                            contamosse--;
                        }
                    } else{
                        if(giocatori[g_random].pos_soprasotto->nemico ==0 ){
                            conta_zona_corrente--;
                            indietreggia(); 
                        } 
                        else{
                            printf("Attenzione, è presente un nemico nella zona in cui ti trovi: sconfiggilo per poter andare indietro\n");
                            contamosse--;
                        }
                    } 
                    break;
                case 3:
                    if(giocatori[g_random].mondo==0){
                        if(giocatori[g_random].pos_mondoreale->nemico ==0 ){
                            conta_zona_corrente=1;
                            cambia_mondo();
                            break; 
                        } 
                        else{
                            printf("Attenzione, è presente un nemico nella zona in cui ti trovi: sconfiggilo per poter cambiare mondo\n");
                            contamosse--;
                        }
                    } else if (giocatori[g_random].mondo!=0){
                        if(giocatori[g_random].pos_soprasotto->nemico ==0 ){
                            conta_zona_corrente=1;
                            cambia_mondo();
                            break; 
                        } 
                        else{
                            printf("Attenzione, è presente un nemico nella zona in cui ti trovi: sconfiggilo per poter cambiare mondo\n");
                            contamosse--;
                        }
                    } 
                    else if(contavanza>0){
                        printf("hai già cambiato zona, quindi non puoi cambiare mondo\n");
                        contamosse--;
                        break;
                    } else if (combattimento==0){
                        printf("devi sconfiggere il nemico per cambiare mondo\n");
                        contamosse--;
                        break;
                    }
                    break;
                case 4:
                    if(giocatori[g_random].pos_mondoreale->oggetto==0){
                        printf("Nessun oggetto\n");
                    } 
                    
                    if(giocatori[g_random].mondo == 0){
                        if(giocatori[g_random].pos_mondoreale->nemico == 0){
                            contaoggetti_stat++;
                            raccogli_oggetto();
                            break;
                        } 
                        else
                            printf("attenzione è presente un nemico; sconfiggilo per poter prendere l'oggetto\n");

                    } else{
                        if(giocatori[g_random].pos_soprasotto->nemico == 0){
                            contaoggetti_stat++;
                            raccogli_oggetto();
                            break;
                        }
                        else
                            printf("attenzione è presente un nemico; sconfiggilo per poter prendere l'oggetto\n");
                    } 
                    break;
                case 5:
                    if(!(giocatori[g_random].zaino[0]==0 && giocatori[g_random].zaino[1]==0 && giocatori[g_random].zaino[2]==0)){
                        do{
                        printf("scegli l'oggetto da utilizzare:\n ");
                        for(int c=0; c<3; c++){
                            printf("\nScegli %d per usare: \n", c+1);
                            nomeoggetto(giocatori[g_random].zaino[c]);
                            printf("\n");

                        }
                        printf("\npremere 1, 2 o 3 per scegliere l'oggetto da usare: ");
                        read_int(&scelta_ogg);
                        scelta_ogg--;
                        }while(scelta_ogg<0 || scelta_ogg>2);
                        utilizza_oggetto(giocatori[g_random].zaino[scelta_ogg]);
                        break;
                    } else{
                        printf("Hai lo zaino vuoto\n");
                        break;
                    }
                case 6:
                    contacombattimento++;
                    combatti();
                    break;
                case 7:
                    passa();
                    break;
                default:
                    contamosse--;
                    printf("Errore - scegli una delle opzioni possibili\n\n");
            }
            if(contaturno==5*n_giocatori){
                break;
            }
        }
        gioca=0;      
    }

}


static void avanza(){
    if(giocatori[g_random].mondo==0){
        if(giocatori[g_random].pos_mondoreale->avanti!=NULL){
            giocatori[g_random].pos_mondoreale=giocatori[g_random].pos_mondoreale->avanti;
            printf("Sei andato avanti!\n");
        }
        else{
            printf("Sei all'ultima zona, non puoi andare avanti\n");
        }
    } else{
        if(giocatori[g_random].pos_soprasotto->avanti!=NULL){
            giocatori[g_random].pos_soprasotto=giocatori[g_random].pos_soprasotto->avanti;
            printf("Sei andato avanti!\n");

        }
        else{
            printf("Sei all'ultima zona\n");
        }
    }

}

static void indietreggia(){
    if(giocatori[g_random].mondo==0){
        if(giocatori[g_random].pos_mondoreale->indietro!=NULL){
            giocatori[g_random].pos_mondoreale=giocatori[g_random].pos_mondoreale->indietro;
            printf("Sei andato indietro!\n");
        }
        else{
            printf("Sei alla prima zona, non puoi andare indietro\n");
        }
    } else{
        if(giocatori[g_random].pos_soprasotto->indietro!=NULL){
            giocatori[g_random].pos_soprasotto=giocatori[g_random].pos_soprasotto->indietro;
            printf("Sei andato indietro!\n");
        }
        else{
            printf("Sei alla prima zona\n");
        }
    }
}

static void cambia_mondo(){
    dado=0;
    if(giocatori[g_random].mondo==0){
        giocatori[g_random].mondo=1;
    } else{
        giocatori[g_random].mondo=0;
    }
}

static void passa(){
    contaturno++;
}

static void raccogli_oggetto(){
    if(contaoggetti>2){
        printf("Hai lo zaino pieno, non puoi prendere altri oggetti.\n");
    } else{
        if(giocatori[g_random].mondo!=0){
            printf("non ci sono oggetti!\n");
        }
        if(giocatori[g_random].pos_mondoreale->oggetto==0){
            printf("non c'è alcun oggetto in questa zona\n");
        }
        else{
            giocatori[g_random].zaino[contaoggetti]=giocatori[g_random].pos_mondoreale->oggetto;
            printf("hai raccolto l'oggetto:\n");
            nomeoggetto(giocatori[g_random].pos_mondoreale->oggetto);
            printf("\n\n");
            giocatori[g_random].pos_mondoreale->oggetto=0;
            contaoggetti++;
        }
    }
}

static void utilizza_oggetto(int ogg){
    sceltazona=0;
    switch(ogg){
        case 1:
            do{
                system("clear");
                printf("Hai scelto la Bicicletta! Dove vuoi andare? (inserire la posizione della zona): ");
                read_int(&sceltazona);
            }while(sceltazona<1 || sceltazona>15);
            
            scan_zona_mondoreale=prima_zona_mondoreale;
            scan_zona_soprasotto=prima_zona_soprasotto;
            for(int i=0; i<sceltazona-1; i++){
                if(giocatori[g_random].mondo==0)
                    scan_zona_mondoreale=scan_zona_mondoreale->avanti;
                else
                    scan_zona_soprasotto=scan_zona_soprasotto->avanti;
            }
            giocatori[g_random].pos_mondoreale=scan_zona_mondoreale;
            giocatori[g_random].pos_soprasotto=scan_zona_soprasotto;
            break;
        case 2:
            giocatori[g_random].fortuna+=(giocatori[g_random].fortuna*30)/100;
            printf("hai indossato la maglietta FuocoInferno: ora hai %d punti fortuna\n ", giocatori[g_random].fortuna);
            for(int i=0; i<3; i++){
                if(giocatori[g_random].zaino[i]==ogg){
                    giocatori[g_random].zaino[i]=0;
                }
            }
            break;
        case 3:
            printf("Hai usato la bussola! ecco i dati della zona dopo la tua: \n");
            stampa_zona_giocatore(giocatori[g_random].pos_mondoreale->avanti, giocatori[g_random].pos_soprasotto->avanti, 0);
            break;
        case 4:
            printf("Hai scelto la schit- YEAH YEEEAH\n");
            for(int i=0; i<226; i++){
                printf("YEAH\t");
            }
            if(combattimento==1){
                nemico_hp-=1986;
            } else {
                printf("Giacomo ETTOCAMPO nel tuo corpo: ..per ora lasciamo stare, non ho nulla da CERCARE E DISTRUGGERE\n");
            }
            break;

    }
}

static void stampa_menu_combattimento(){
    printf("****************   MENU' COMBATTIMENTO   ***************\n");
    stampa_dati_giocatore();
    printf("Nemico:\n");
    printf("\tnome");
    if(giocatori[g_random].mondo==0)
        nomenemico(giocatori[g_random].pos_mondoreale->nemico);
    else
        nomenemico(giocatori[g_random].pos_soprasotto->nemico);
    printf("\n");
    printf("\tpunti vita: %d\n", nemico_hp);
    printf("\tpunti attacco: %d\n", attacco_nemico);
    printf("\n\n");
    printf("*******************************************************\n");
}

static void combatti(){
    system("clear");
    combattimento=1;
    if(giocatori[g_random].mondo==0){
        switch(giocatori[g_random].pos_mondoreale->nemico){
                case 0:
                    break;
                case 1:
                    nemico_hp=10;
                    attacco_nemico=4;
                    break;
                case 2:
                    nemico_hp=15;
                    attacco_nemico=10;
                    break;
                case 3:
                    break;
        }
    }
    
    if(giocatori[g_random].mondo==1){
        switch(giocatori[g_random].pos_soprasotto->nemico){
                    case 0:
                        break;
                    case 1:
                        break;
                    case 2:
                        nemico_hp=15;
                        attacco_nemico=10;
                        break;
                    case 3:
                        nemico_hp=20;
                        attacco_nemico=14;
                        break;
        }
    }

    while(combattimento==1){
        do{
            if(choice_combat<0 || choice_combat>1){
                system("clear");
                printf("ERRORE: scelata non valida\n");
                choice_combat=2;
                
            }

            stampa_menu_combattimento();
            printf("scegli una delle due opzioni:\n");
            printf("0) attacca\n");
            printf("1) difendi\n");
            printf("Risposta: ");
            read_int(&choice_combat);

        }while(choice_combat<0 || choice_combat>1);
        printf("Premere invio per continuare...");
        clear_my_buffer();
        if(choice_combat==1){
            if(giocatori[g_random].difesa_psichica>=attacco_nemico){
                system("clear");
                printf("Hai parato il colpo con successo\n");
            } else{
                if(giocatori[g_random].fortuna>=15){
                    if(rand()%5==0){
                        system("clear");
                        printf("che fortuna, il nemico ti ha mancato!\n");
                    }
                } else{
                    system("clear");
                    player_hp-=attacco_nemico;
                    printf("sei stato colpito, non avevi abbastanza difesa...\n");
                }
                if(player_hp<=0){
                    system("clear");
                    printf("%s SEI MORTO.\n", giocatori[g_random].nome);
                    delplayer();
                    if(n_giocatori==0){
                        system("clear");
                        printf("TUTTI I GIOCATORI SONO STATI ELIMINATI\n");
                        crediti();
                    }
                }
            }
        } 
        else{
            for(int i=0; i<3; i++){
                if(giocatori[g_random].zaino[i]==4){
                    printf("vuoi usare la schitarrata?: \n");
                    printf("0) NO\n1) SI\n risposta: ");
                    read_int(&choice_combat);
                    if(choice_combat==1){
                        utilizza_oggetto(4);
                        printf("hai sconfitto il tuo nemico effettuandogli 1986 di danno\n");
                        combattimento=0;
                        printf("premere invio per continuare...");
                        clear_my_buffer();
                        uso_schitarrata=1;
                    }     
                }
            }
            nemico_hp-=giocatori[g_random].attacco_psichico;
                if(nemico_hp<=0){
                    if(giocatori[g_random].mondo == 1 && giocatori[g_random].pos_soprasotto->nemico==3){
                        system("clear");
                        printf("Hai sconfitto il demotorzone... Hai vinto il gioco!!!!!\n\n");
                        printf("premere invio per continuare...");
                        clear_my_buffer();
                        vincitori[partita_vinta]=giocatori[g_random];
                        partita_vinta++;
                        crediti();
                    }
                    printf("Hai attaccato il nemico: gli hai effettuato %d punti danno\nOra la sua vita è pari a 0\n\n", (uso_schitarrata) ? 1986 : giocatori[g_random].attacco_psichico);
                    printf("hai vinto il combattimento!!\n");
                    printf("premere invio per continuare...");
                    clear_my_buffer();
                    if(giocatori[g_random].mondo==0)
                        giocatori[g_random].pos_mondoreale->nemico = 0;
                    else
                        giocatori[g_random].pos_soprasotto->nemico = 0;
                    system("clear");
                    combattimento=0;
 
                }   
                else if(nemico_hp>0){
                    system("clear");
                    printf("Hai attaccato il nemico: gli hai effettuato %d punti danno\nOra la sua vita è pari a %d\n", giocatori[g_random].attacco_psichico, nemico_hp);
                    if(rand()%3==0){
                        printf("Attenzione! il Nemico si è ripreso; ha contrattaccato!, ti ha tolto %d punti vita\n", attacco_nemico);
                        player_hp-=attacco_nemico;
                    }
                }
        }
    }
}

void termina_gioco(){
    printf("Il gioco è terminato...\n ");
    switch (n_giocatori){
        case 1:
            printf("Tanti saluti %s\n", giocatori[0].nome);
            break;
        case 2:
            printf("Tanti saluti %s e %s\n", giocatori[0].nome, giocatori[1].nome);
            break;
        case 3:
            printf("Tanti saluti %s, %s, e %s\n", giocatori[0].nome, giocatori[1].nome, giocatori[2].nome);
            break;
        case 4:
            printf("Tanti saluti %s, %s, %s, e %s\n", giocatori[0].nome, giocatori[1].nome, giocatori[2].nome, giocatori[3].nome);
            break;
        default:
            printf("Non ci sono giocatori.\n\n");
            break;
    }

    exit(0);
}

void crediti(){
    printf("***********CREDITI***********");
    printf("\nCreatore del gioco: Marco Zancan\n");
    if(n_giocatori>0){
        stampa_dati_giocatori();
        if(partita_vinta!=0){
            printf("\nVincitori delle precedenti partite: \n");
            for(int i=0; i<partita_vinta; i++){
            printf("\t- %s\n", vincitori[i].nome);
            }
        }
        
        printf("\nStatistiche Ultima Partita: \n");
        printf("\t- Numero combattimenti:  %d\n", contacombattimento);
        printf("\t- Niocatori totali: %d\n", n_giocatori);
        printf("\t- Nggetti usati: %d\n", contaoggetti_stat);
    }
    printf("Premi invio per tornare al menu'...");
    clear_my_buffer();
    menu_di_gioco();
}