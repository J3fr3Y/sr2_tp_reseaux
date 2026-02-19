/*************************************************************
* proto_tdd_v1 -  émetteur                                   *
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
/* Programme principal - émetteur  */
/* =============================== */
int main (int argc, char* argv[]){
    /* Données */
    unsigned char message[MAX_INFO]; //Message de l'application
    unsigned int taille_msg; //taille du message
    paquet_t paquet; //Paquet a envoyer par le protocole
    paquet_t pack; //Paquet d'acquittements
    
    init_reseau(EMISSION);

    printf("[TRP] Initialisation reseau : OK.\n");
    printf("[TRP] Debut execution protocole transport V1.\n");

    /* On récupere le mmessage de la couche application*/
    de_application(message,&taille_msg);

    /* Tant que l'émetteur a des données à envoyer */
    while(taille_msg != 0){
        /* Construction du paquet a transmettre 
            La somme de controle est géneré dans cette construction de paquet
            grace à l'appel de la fonction generer_somme_controle
        */
        printf("Construction du paquet...\n");
        construire_paquet(&paquet, DATA,taille_msg, message, 0);//num_seq pour la V1
        printf("Paquet construit!\n");

        /* On remet le paquet vers la couche réseaux
            TANT QUE pas de NACK reçu, sinon on retransmet le paquet
        */
        do{
            /* On envoie le paquet de données */
            printf("Envoie du paquet...\n");
            vers_reseau(&paquet);
            printf("Paquet envoyé\n");
            /* réception dácquittemens ACK/NACK */
            de_reseau(&pack);
            printf("Acquittement reçu\n");
        } while(pack.type != NACK);

        /* On lit les données suivantes */
        de_application(message,&taille_msg);

        printf("[TRP] Fin execution protocole transfert de donnees V1 (TDD).\n");
        return 0;
    }

    




}