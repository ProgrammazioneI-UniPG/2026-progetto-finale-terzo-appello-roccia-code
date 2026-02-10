enum tipo_oggetto{nessun_oggetto, bicicletta, maglietta_fuocoinferno, bussola, schitarrata_metallica};
enum tipo_zona{bosco, scuola, laboratorio, caverna, strada, giardino, supermercato, centrale_elettrica, deposito_abbandonato, stazione_polizia};
enum tipo_nemico{nessun_nemico, billi, democane, demotorzone};

void clear_my_buffer();
void imposta_gioco();
void gioca();
void termina_gioco();
void crediti();
void menu_di_gioco();

struct Giocatore {
    char nome[50];
    int mondo;
    struct Zona_mondoreale* pos_mondoreale;
    struct Zona_soprasotto* pos_soprasotto;
    int attacco_psichico;
    int difesa_psichica;
    int fortuna;
    enum tipo_oggetto zaino[3];
};

struct Zona_mondoreale {
    enum tipo_nemico nemico;
    enum tipo_oggetto oggetto;
    enum tipo_zona tipo;
    struct Zona_mondoreale* avanti;
    struct Zona_mondoreale* indietro;
    struct Zona_soprasotto* link_soprasotto;
};

struct Zona_soprasotto {
    enum tipo_zona tipo;
    enum tipo_nemico nemico;
    struct Zona_soprasotto* avanti;
    struct Zona_soprasotto* indietro;
    struct Zona_mondoreale* link_mondoreale;
};