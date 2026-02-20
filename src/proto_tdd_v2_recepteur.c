/*********************************************************************
* proto_tdd_v2 - récepteur                                           *
* TRANSFERT DE DONNEES  v2                                           *
*                                                                    *
* Protocole avec contrôle de flux, avec retransmissions sur erreurs  *
* et pertes                                                          *
* Jefrey SANCHEZ REYES - Univ. de Toulouse III-Paul Sabatier         *
**********************************************************************/

#include <stdio.h>
#include "application.h"
#include "couche_transport.h"
#include "services_reseau.h"

/* =============================== */
/* Programme principal - récepteur */
/* =============================== */
int main (int argc, char* argv[]){
    /* Données */
    unsigned char message[MAX_INFO]; //Message de l'application
    paquet_t paquet_recu; //Paquet reçu par le protocole
    paquet_t pack; //Paquet d'acquittements
    uint8_t fin = 0;// condition d'arret de la boucle
    int num_seq = 0;//Numéro de séquence du paquet attendu

    init_reseau(RECEPTION);

    printf("[TRP] Initialisation reseau : OK.\n");
    printf("[TRP] Debut execution protocole transport V1 côté récepteur.\n");

    /* Tant qu'on reçoit des paquets*/
    while(!fin){
        /* On récupere le paquet de la couche réseau */
        de_reseau(&paquet_recu);
        printf("Réception d'un paquet\n");
        /* On verifie qu'il n'y a pas d'erreur */
        printf("Verification de somme de controle...\n");
        if(generer_somme_controle(&paquet_recu) == paquet_recu.somme_ctrl){
            if(paquet_recu.num_seq == num_seq){
                /* paquet, num_seq correct, on renvoie un ACK et on remet vers l'application
                puis on incremente le num_seq */
                printf("Somme de controle bonne! On renvoie un ACK\n");
                construire_paquet(&pack,ACK,0,0,num_seq);
                /* Extraction de données reçues*/
                for(int i = 0; i < paquet_recu.lg_info; i++){
                    message[i] = paquet_recu.info[i];
                }
                /* Remets le données vers la couche application */
                fin = vers_application(message,paquet_recu.lg_info);
                num_seq = inc(num_seq,2);
            }
            /* On renvoie un acquittement ACK */
            vers_reseau(&pack);
        }
        /* Sinon on fait rien, le timerout indique qu'il y a une erreur ou une perte */
        
        
    }
    printf("[TRP] Fin execution protocole transfert de donnees V1 (TDD).\n");
    return 0;
}