/*************************************************************
* proto_tdd_v1 -  récepteur                                  *
* TRANSFERT DE DONNEES  v1                                   *
*                                                            *
* Protocole sans contrôle de flux, sans reprise sur erreurs  *
*                                                            *
* Jefrey SANCHEZ REYES - Univ. de Toulouse III-Paul Sabatier *
**************************************************************/

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

    init_reseau(RECEPTION);

    printf("[TRP] Initialisation reseau : OK.\n");
    printf("[TRP] Debut execution protocole transport V1 côté récepteur.\n");

    /* Tant qu'on reçoit des paquets*/
    while(!fin){
        /* On récupere le paquet de la couche réseau */
        printf("Réception d'un paquet\n");
        de_reseau(&paquet_recu);
        /* On verifie qu'il n'y a pas d'erreur */
        printf("Verification de somme de controle...\n");
        if(generer_somme_controle(&paquet_recu) == paquet_recu.somme_ctrl){
            /* paquet est correct, on renvoie un ACK et on remet vers l'application*/
            printf("Somme de controle bonne! On renvoie un ACK\n");
            construire_paquet(&pack,ACK,0,0,0);

            /* Extraction de données reçues*/
            for(int i = 0; i < paquet_recu.lg_info; i++){
                message[i] = paquet_recu.info[i];
            }
            /* Remets le données vers la couche application */
            fin = vers_application(message,paquet_recu.lg_info);
        } else{
            printf("Erreur détectée, on renvoie un NACK!\n");
            /* Erreur détecté, on renvoie un NACK */
            construire_paquet(&pack,NACK,0,0,0);
        }
        /* On renvoie un acquittement ACK/NACK */
        vers_reseau(&pack);
    }
    return 0;
}