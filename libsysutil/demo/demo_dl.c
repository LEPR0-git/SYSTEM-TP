#include <stdio.h>
#include <stdlib.h>
#include <dlfcn.h>        /* dlopen, dlsym, dlclose, dlerror */
#include "llist.h"

/* ═══════════════════════════════════════════════════════════════
   demo_dl.c — Tâche 3.3 INF362
   Charge lib/libsysutil.so dynamiquement et appelle :
     - ll_create    via dlsym
     - ll_push_back via dlsym
     - ll_foreach   via dlsym
   ═══════════════════════════════════════════════════════════════ */

/* ─── types des fonctions chargées dynamiquement ─────────────── */

typedef LList * (*fn_ll_create)   (void (*free_data)(void *));
typedef int     (*fn_ll_push_back)(LList *l, void *data);
typedef void    (*fn_ll_foreach)  (const LList *l,
                                   void (*fn)(void *data, size_t idx));
typedef void    (*fn_ll_destroy)  (LList **l);

/* ─── callback d'affichage (défini ici, pas dans la .so) ─────── */

static void print_item(void *data, size_t idx) {
    printf("  [%zu] %s\n", idx, (char *)data);
}

/* ─── utilitaire : charge un symbole ou quitte ───────────────── */

static void *load_sym(void *handle, const char *name) {
    dlerror();                          /* vide l'erreur précédente  */
    void *sym = dlsym(handle, name);
    const char *err = dlerror();
    if (err != NULL) {
        fprintf(stderr, "[dlsym] %s : %s\n", name, err);
        dlclose(handle);
        exit(EXIT_FAILURE);
    }
    return sym;
}

/* ─── main ───────────────────────────────────────────────────── */

int main(void) {

    /* 1. Ouvrir la bibliothèque dynamique */
    void *handle = dlopen("lib/libsysutil.so", RTLD_LAZY);
    if (handle == NULL) {
        fprintf(stderr, "[dlopen] %s\n", dlerror());
        return EXIT_FAILURE;
    }
    printf("[dlopen]  lib/libsysutil.so chargée avec succès.\n");

    /* 2. Résoudre les symboles via dlsym */
    fn_ll_create    my_ll_create    = (fn_ll_create)    load_sym(handle, "ll_create");
    fn_ll_push_back my_ll_push_back = (fn_ll_push_back) load_sym(handle, "ll_push_back");
    fn_ll_foreach   my_ll_foreach   = (fn_ll_foreach)   load_sym(handle, "ll_foreach");
    fn_ll_destroy   my_ll_destroy   = (fn_ll_destroy)   load_sym(handle, "ll_destroy");

    printf("[dlsym]   ll_create    -> %p\n", (void *)my_ll_create);
    printf("[dlsym]   ll_push_back -> %p\n", (void *)my_ll_push_back);
    printf("[dlsym]   ll_foreach   -> %p\n", (void *)my_ll_foreach);
    printf("[dlsym]   ll_destroy   -> %p\n\n", (void *)my_ll_destroy);

    /* 3. Utiliser la bibliothèque chargée dynamiquement */
    LList *liste = my_ll_create(NULL);
    if (liste == NULL) {
        fprintf(stderr, "ll_create a retourné NULL\n");
        dlclose(handle);
        return EXIT_FAILURE;
    }

    my_ll_push_back(liste, "Yaoundé");
    my_ll_push_back(liste, "Douala");
    my_ll_push_back(liste, "Bafoussam");

    puts("=== Contenu de la liste (via dlopen) ===");
    my_ll_foreach(liste, print_item);
    printf("  → %zu élément(s)\n\n", liste->length);

    /* 4. Détruire la liste et fermer la bibliothèque */
    my_ll_destroy(&liste);
    printf("[ll_destroy] liste = %s\n", liste == NULL ? "NULL ✓" : "non NULL ✗");

    dlclose(handle);
    printf("[dlclose]  lib/libsysutil.so déchargée.\n");

    return EXIT_SUCCESS;
}