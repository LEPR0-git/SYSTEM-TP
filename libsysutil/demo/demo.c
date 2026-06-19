#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "llist.h"

/* ─── structure Contact ──────────────── */

typedef struct {
    char nom[64];
    char telephone[15];   
    char ville[32];
} Contact;

/* ─── callbacks ───────────────────────────────────────────── */

/* libération d'un Contact alloué sur le tas */
static void free_contact(void *data) {
    free(data);
}

/* affichage d'un contact — passé à ll_foreach */
static void afficher_contact(void *data, size_t idx) {
    Contact *c = (Contact *)data;
    printf("  [%zu] %-20s  %-15s  %s\n", idx, c->nom, c->telephone, c->ville);
}

/* comparaison par nom — passé à ll_remove */
static int cmp_nom(const void *a, const void *b) {
    return strcmp(((Contact *)a)->nom, (const char *)b);
}

/* ─── helper : créer un contact ───────────────────────────── */

static Contact *nouveau_contact(const char *nom,
                                const char *telephone,
                                const char *ville) {
    Contact *c = malloc(sizeof(Contact));
    if (c == NULL) {
        fprintf(stderr, "malloc échoué\n");
        exit(EXIT_FAILURE);
    }
    strncpy(c->nom,       nom,       sizeof c->nom       - 1);
    strncpy(c->telephone, telephone, sizeof c->telephone - 1);
    strncpy(c->ville,     ville,     sizeof c->ville     - 1);
    c->nom[sizeof c->nom - 1]             = '\0';
    c->telephone[sizeof c->telephone - 1] = '\0';
    c->ville[sizeof c->ville - 1]         = '\0';
    return c;
}

/* ─── main ────────────────────────────────────────────────── */

int main(void) {

    /* 1. Créer la liste */
    LList *repertoire = ll_create(free_contact);
    if (repertoire == NULL) {
        fprintf(stderr, "ll_create échoué\n");
        return EXIT_FAILURE;
    }

    // 2. Ajouter 5 contacts 
    ll_push_back(repertoire, nouveau_contact("Jean Mbarga",    "+237690001111", "Yaoundé"));
    ll_push_back(repertoire, nouveau_contact("Marie Ngo",      "+237670002222", "Douala"));
    ll_push_back(repertoire, nouveau_contact("Paul Feudjio",   "+237650003333", "Bafoussam"));
    ll_push_back(repertoire, nouveau_contact("Aminatou Bello", "+237680004444", "Garoua"));
    ll_push_back(repertoire, nouveau_contact("Pierre Manga",   "+237699005555", "Bertoua"));

    /* 3. Afficher tous les contacts */
    puts("=== Répertoire initial ===");
    printf("  %-4s %-20s  %-15s  %s\n", "N°", "Nom", "Téléphone", "Ville");
    puts("  -------------------------------------------------------");
    ll_foreach(repertoire, afficher_contact);
    printf("  → %zu contact(s)\n\n", repertoire->length);

    /* 4. Supprimer un contact par nom */
    const char *cible = "Marie Ngo";
    printf("Suppression de '%s'...\n", cible);
    int r = ll_remove(repertoire, (void *)cible, cmp_nom);
    if (r == 0)
        printf("  Contact supprimé avec succès.\n\n");
    else
        printf("  Contact introuvable.\n\n");

    /* 5. Afficher le résultat final */
    puts("=== Répertoire après suppression ===");
    printf("  %-4s %-20s  %-15s  %s\n", "N°", "Nom", "Téléphone", "Ville");
    puts("  -------------------------------------------------------");
    ll_foreach(repertoire, afficher_contact);
    printf("  → %zu contact(s)\n\n", repertoire->length);

    /* 6. Détruire proprement la liste */
    ll_destroy(&repertoire);
    printf("Liste détruite. repertoire = %s\n",
           repertoire == NULL ? "NULL ✓" : "non NULL ✗");

    return EXIT_SUCCESS;
}