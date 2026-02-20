/*********************************************************************
* proto_tdd_v2 -  émetteur                                           *
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
#define MAX_TIME 20 //temps max temporisateur

/* =============================== */
/* Programme principal - émetteur  */
/* =============================== */
int main (int argc, char* argv[]){
    /* Données */
    unsigned char message[MAX_INFO]; //Message de l'application
    int taille_msg; //taille du message
    paquet_t paquet; //Paquets a envoyer par le protocole
    paquet_t pack; //Paquets d'acquittements
    int num_seq = 0; //Numéro de séquence du prochain paquet a envoyer
    int evt; //Timeout ou paquet reçu
    

    init_reseau(EMISSION);

    printf("[TRP] Initialisation reseau : OK.\n");
    printf("[TRP] Debut execution protocole transport V2 côté émetteur.\n");

    /* On récupere le mmessage de la couche application*/
    de_application(message,&taille_msg);

    /* Tant que l'émetteur a des données à envoyer */
    while(taille_msg != 0){
        /* Construction du paquet a transmettre 
            La somme de controle est géneré dans cette construction de paquet
            grace à l'appel de la fonction generer_somme_controle
        */
       printf("Contructions du paquet...\n");
       construire_paquet(&paquet,DATA,taille_msg,message,num_seq);
       printf("Paquet construit");

       /* Tant qu'il y a un timeout rentrasment le paquet*/
       do {
            printf("On envoie le paquet\n");
            vers_reseau(&paquet);
            printf("Paquet envoyé!\n");
            printf("Timer démarre");
            depart_temporisateur(MAX_TIME);
            /* Récupère le paquet d'ack*/
            //de_reseau(&pack);
            evt = attendre();
       } while(evt >= 0);

        /* On reçoit un ACK, on arrete le temp 
        incremente le num_seq */
        de_reseau(&pack);
        arret_temporisateur();
        num_seq = inc(num_seq,2);

        /* On preleve le prochain paquet */
        de_application(message,&taille_msg);
        printf("[TRP] Fin execution protocole transfert de donnees V2 (TDD).\n");
    }
    return 0;
}
