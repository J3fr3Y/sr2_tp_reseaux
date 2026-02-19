#include <stdio.h>
#include "couche_transport.h"
#include "services_reseau.h"
#include "application.h"

/* ************************************************************************** */
/* *************** Fonctions utilitaires couche transport ******************* */
/* ************************************************************************** */

// RAJOUTER VOS FONCTIONS DANS CE FICHIER...



/*--------------------------------------*/
/* Fonction d'inclusion dans la fenetre */
/*--------------------------------------*/
int dans_fenetre(unsigned int inf, unsigned int pointeur, int taille) {

    unsigned int sup = (inf+taille-1) % SEQ_NUM_SIZE;

    return
        /* inf <= pointeur <= sup */
        ( inf <= sup && pointeur >= inf && pointeur <= sup ) ||
        /* sup < inf <= pointeur */
        ( sup < inf && pointeur >= inf) ||
        /* pointeur <= sup < inf */
        ( sup < inf && pointeur <= sup);
}

/* Mes fonctions */

/*--------------------------------------*
* Fonction somme de contrôle            *
*--------------------------------------*/

uint8_t somme_controle(paquet_t * paquet){
    uint8_t somme_ctrl = paquet->type ^ paquet->num_seq ^ paquet->lg_info;
    for (int i = 0; i < paquet->lg_info; i++){
        somme_ctrl ^= paquet->info[i];
    }
    return somme_ctrl;
     
}