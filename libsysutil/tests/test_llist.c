/*
 tests/test_llist.c — Tests complets du module liste chaînée générique

 Couvre : ll_create, ll_push_back, ll_push_front, ll_pop_front,
          ll_get, ll_remove, ll_foreach, ll_destroy

 Compilation :
   gcc -Wall -Wextra -Werror -std=c99 -g -I../include ../src/llist.c test_llist.c -o ../bin/test_llist
 Exécution :
   ../bin/test_llist

*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "llist.h"


static int tests_total  = 0;
static int tests_passed = 0;
static int tests_failed = 0;

#define CHECK(condition, description)                                        \
    do {                                                                     \
        tests_total++;                                                       \
        if (condition) {                                                     \
            tests_passed++;                                                  \
            printf("  \033[32m[PASS]\033[0m %s\n", description);           \
        } else {                                                             \
            tests_failed++;                                                  \
            printf("  \033[31m[FAIL]\033[0m %s\n", description);           \
            printf("         → ligne %d dans %s\n", __LINE__, __FILE__);   \
        }                                                                    \
    } while (0)

static void section(const char *titre) {
    printf("\n  ┌─────────────────────────────────────────────────┐\n");
    printf("  │  %-47s│\n", titre);
    printf("  └─────────────────────────────────────────────────┘\n");
}

// Contact 
typedef struct {
    char nom[64];
    char telephone[15];
    char ville[32];
} Contact;

// Constructeur de Contact alloué sur le tas 
static Contact *make_contact(const char *nom, const char *tel, const char *ville)
{
    Contact *c = malloc(sizeof(Contact));
    if (c == NULL) return NULL;
    strncpy(c->nom,       nom,   sizeof(c->nom)       - 1); c->nom[63]       = '\0';
    strncpy(c->telephone, tel,   sizeof(c->telephone) - 1); c->telephone[14] = '\0';
    strncpy(c->ville,     ville, sizeof(c->ville)     - 1); c->ville[31]     = '\0';
    return c;
}

// Callback d'affichage pour ll_foreach (Contact*) 
static void afficher_contact(void *data, size_t idx)
{
    Contact *c = (Contact *)data;
    printf("         [%zu] %-20s %-15s %s\n", idx, c->nom, c->telephone, c->ville);
}

// Comparateur par nom de contact (pour ll_remove) 
static int cmp_contact_nom(const void *a, const void *b)
{
    return strcmp(((const Contact *)a)->nom, (const char *)b);
}

// Comparateur d'entiers (pour les tests avec int*) 
static int cmp_int(const void *a, const void *b)
{
    return *(const int *)a - *(const int *)b;
}

// SUITE 1 — ll_create
static void test_suite_create(void)
{
    section("SUITE 1 : ll_create");

    LList *l = ll_create(NULL);
    CHECK(l != NULL,           "ll_create(NULL) retourne un pointeur non-NULL");
    CHECK(l->head == NULL,     "ll_create : head initialisé à NULL");
    CHECK(l->tail == NULL,     "ll_create : tail initialisé à NULL");
    CHECK(l->length == 0,      "ll_create : length initialisé à 0");
    CHECK(l->free_data == NULL,"ll_create(NULL) : free_data est NULL");
    ll_destroy(&l);

    LList *l2 = ll_create(free);
    CHECK(l2 != NULL,          "ll_create(free) retourne un pointeur non-NULL");
    CHECK(l2->free_data == free,"ll_create(free) : free_data == free");
    ll_destroy(&l2);
}

// SUITE 2 — ll_push_back
static void test_suite_push_back(void)
{
    section("SUITE 2 : ll_push_back");

    LList *l = ll_create(NULL);

    // Insertion dans une liste vide 
    int a = 10;
    int ret = ll_push_back(l, &a);
    CHECK(ret == 0,             "push_back sur liste vide : retourne 0");
    CHECK(l->length == 1,       "push_back liste vide : length = 1");
    CHECK(l->head != NULL,      "push_back liste vide : head != NULL");
    CHECK(l->tail != NULL,      "push_back liste vide : tail != NULL");
    CHECK(l->head == l->tail,   "push_back liste vide : head == tail (nœud unique)");
    CHECK(l->head->data == &a,  "push_back liste vide : head->data = &a");

    // Insertion dans une liste non vide 
    int b = 20, c = 30;
    ll_push_back(l, &b);
    ll_push_back(l, &c);
    CHECK(l->length == 3,       "push_back x3 : length = 3");
    CHECK(l->head->data == &a,  "push_back x3 : head inchangé (toujours &a)");
    CHECK(l->tail->data == &c,  "push_back x3 : tail mis à jour vers &c");

    // Vérification de l'ordre FIFO 
    CHECK(*(int *)ll_get(l, 0) == 10, "push_back : ordre [0] = 10");
    CHECK(*(int *)ll_get(l, 1) == 20, "push_back : ordre [1] = 20");
    CHECK(*(int *)ll_get(l, 2) == 30, "push_back : ordre [2] = 30");

    // push_back sur liste NULL 
    CHECK(ll_push_back(NULL, &a) == -1, "push_back(NULL, data) retourne -1");

    ll_destroy(&l);
}

// SUITE 3 — ll_push_front
static void test_suite_push_front(void)
{
    section("SUITE 3 : ll_push_front");

    LList *l = ll_create(NULL);

    int a = 1;
    int ret = ll_push_front(l, &a);
    CHECK(ret == 0,             "push_front sur liste vide : retourne 0");
    CHECK(l->length == 1,       "push_front liste vide : length = 1");
    CHECK(l->head == l->tail,   "push_front liste vide : head == tail");

    int b = 2, c = 3;
    ll_push_front(l, &b);
    ll_push_front(l, &c);
    CHECK(l->length == 3,       "push_front x3 : length = 3");
    CHECK(*(int *)ll_get(l, 0) == 3, "push_front : [0] = 3 (dernier inséré)");
    CHECK(*(int *)ll_get(l, 1) == 2, "push_front : [1] = 2");
    CHECK(*(int *)ll_get(l, 2) == 1, "push_front : [2] = 1 (premier inséré)");
    CHECK(l->head->data == &c,  "push_front : head pointe vers &c");
    CHECK(l->tail->data == &a,  "push_front : tail pointe vers &a (inchangé)");

    // push_front sur liste NULL 
    CHECK(ll_push_front(NULL, &a) == -1, "push_front(NULL, data) retourne -1");

    ll_destroy(&l);
}

// SUITE 4 — ll_pop_front
static void test_suite_pop_front(void)
{
    section("SUITE 4 : ll_pop_front");

    // pop sur liste vide 
    LList *vide = ll_create(NULL);
    CHECK(ll_pop_front(vide) == NULL, "pop_front sur liste vide → NULL");
    CHECK(vide->length == 0,          "pop_front liste vide : length reste 0");
    ll_destroy(&vide);

    // pop sur liste NULL 
    CHECK(ll_pop_front(NULL) == NULL, "pop_front(NULL) → NULL");

    // pop sur liste à 1 nœud → liste doit redevenir vide 
    LList *l1 = ll_create(NULL);
    int x = 99;
    ll_push_back(l1, &x);
    void *data = ll_pop_front(l1);
    CHECK(data == &x,              "pop_front liste à 1 nœud : retourne &x");
    CHECK(l1->length == 0,         "pop_front liste à 1 nœud : length = 0");
    CHECK(l1->head == NULL,        "pop_front liste à 1 nœud : head = NULL");
    CHECK(l1->tail == NULL,        "pop_front liste à 1 nœud : tail = NULL (crucial !)");
    ll_destroy(&l1);

    // pop successifs sur liste à 3 nœuds 
    LList *l3 = ll_create(NULL);
    int v1 = 100, v2 = 200, v3 = 300;
    ll_push_back(l3, &v1);
    ll_push_back(l3, &v2);
    ll_push_back(l3, &v3);

    CHECK(*(int *)ll_pop_front(l3) == 100, "pop_front [1/3] : retourne 100");
    CHECK(l3->length == 2,                 "pop_front [1/3] : length = 2");
    CHECK(*(int *)ll_pop_front(l3) == 200, "pop_front [2/3] : retourne 200");
    CHECK(l3->length == 1,                 "pop_front [2/3] : length = 1");
    CHECK(*(int *)ll_pop_front(l3) == 300, "pop_front [3/3] : retourne 300");
    CHECK(l3->length == 0,                 "pop_front [3/3] : length = 0");
    CHECK(l3->head == NULL,                "pop_front [3/3] : head = NULL");
    CHECK(l3->tail == NULL,                "pop_front [3/3] : tail = NULL");
    ll_destroy(&l3);
}

// SUITE 5 — ll_get
static void test_suite_get(void)
{
    section("SUITE 5 : ll_get");

    LList *l = ll_create(NULL);
    int vals[5] = {10, 20, 30, 40, 50};
    for (int i = 0; i < 5; i++)
        ll_push_back(l, &vals[i]);

    CHECK(*(int *)ll_get(l, 0) == 10, "ll_get index 0 = 10");
    CHECK(*(int *)ll_get(l, 2) == 30, "ll_get index 2 = 30");
    CHECK(*(int *)ll_get(l, 4) == 50, "ll_get index 4 = 50 (dernier)");

    // Cas limites 
    CHECK(ll_get(l, 5)  == NULL, "ll_get index = length → NULL");
    CHECK(ll_get(l, 99) == NULL, "ll_get index >> length → NULL");
    CHECK(ll_get(NULL, 0) == NULL, "ll_get(NULL, 0) → NULL");

    ll_destroy(&l);

    // ll_get sur liste vide 
    LList *vide = ll_create(NULL);
    CHECK(ll_get(vide, 0) == NULL, "ll_get sur liste vide → NULL");
    ll_destroy(&vide);
}

// SUITE 6 — ll_remove
static void test_suite_remove(void)
{
    section("SUITE 6 : ll_remove");

    LList *l = ll_create(NULL);
    int v1 = 10, v2 = 20, v3 = 30, v4 = 40;
    ll_push_back(l, &v1);
    ll_push_back(l, &v2);
    ll_push_back(l, &v3);
    ll_push_back(l, &v4);
    // Liste : 10 → 20 → 30 → 40 

    // Suppression d'un élément intermédiaire 
    int ret = ll_remove(l, &v2, cmp_int);
    CHECK(ret == 0,             "ll_remove élément intermédiaire (20) : retourne 0");
    CHECK(l->length == 3,       "ll_remove intermédiaire : length = 3");
    CHECK(*(int *)ll_get(l, 0) == 10, "ll_remove intermédiaire : [0] = 10");
    CHECK(*(int *)ll_get(l, 1) == 30, "ll_remove intermédiaire : [1] = 30 (décalé)");
    // Liste : 10 → 30 → 40 

    // Suppression du premier nœud (head) 
    ret = ll_remove(l, &v1, cmp_int);
    CHECK(ret == 0,             "ll_remove premier nœud (10) : retourne 0");
    CHECK(l->length == 2,       "ll_remove premier nœud : length = 2");
    CHECK(*(int *)ll_get(l, 0) == 30, "ll_remove premier nœud : nouveau head = 30");
    // Liste : 30 → 40 

    // Suppression du dernier nœud (tail) 
    ret = ll_remove(l, &v4, cmp_int);
    CHECK(ret == 0,             "ll_remove dernier nœud (40) : retourne 0");
    CHECK(l->length == 1,       "ll_remove dernier nœud : length = 1");
    CHECK(l->tail->data == &v3, "ll_remove dernier nœud : tail mis à jour vers &v3");
    // Liste : 30 

    // Suppression d'un élément absent 
    int absent = 999;
    ret = ll_remove(l, &absent, cmp_int);
    CHECK(ret == -1,            "ll_remove élément absent : retourne -1");
    CHECK(l->length == 1,       "ll_remove élément absent : length inchangé");

    // Appels invalides 
    CHECK(ll_remove(NULL, &v3, cmp_int) == -1, "ll_remove(NULL, ...) retourne -1");
    CHECK(ll_remove(l, &v3, NULL)       == -1, "ll_remove(..., NULL) retourne -1");

    ll_destroy(&l);

    // Suppression du seul nœud → liste vide 
    LList *l1 = ll_create(NULL);
    int u = 42;
    ll_push_back(l1, &u);
    ll_remove(l1, &u, cmp_int);
    CHECK(l1->length == 0,       "ll_remove seul nœud : length = 0");
    CHECK(l1->head == NULL,      "ll_remove seul nœud : head = NULL");
    CHECK(l1->tail == NULL,      "ll_remove seul nœud : tail = NULL");
    ll_destroy(&l1);
}

// SUITE 7 — ll_foreach

static size_t foreach_count = 0;
static int    foreach_sum   = 0;

static void cb_compter(void *data, size_t idx)
{
    (void)idx;
    foreach_count++;
    foreach_sum += *(int *)data;
}

static void test_suite_foreach(void)
{
    section("SUITE 7 : ll_foreach");

    LList *l = ll_create(NULL);
    int vals[4] = {5, 10, 15, 20};
    for (int i = 0; i < 4; i++)
        ll_push_back(l, &vals[i]);

    foreach_count = 0;
    foreach_sum   = 0;
    ll_foreach(l, cb_compter);

    CHECK(foreach_count == 4,    "ll_foreach : callback appelé 4 fois");
    CHECK(foreach_sum   == 50,   "ll_foreach : somme accumulée = 50");

    // Appels invalides — ne doivent pas planter 
    ll_foreach(NULL, cb_compter);
    ll_foreach(l, NULL);
    CHECK(1, "ll_foreach(NULL, fn) et ll_foreach(l, NULL) : pas de crash");

    // ll_foreach sur liste vide 
    LList *vide = ll_create(NULL);
    foreach_count = 0;
    ll_foreach(vide, cb_compter);
    CHECK(foreach_count == 0, "ll_foreach sur liste vide : 0 appels");
    ll_destroy(&vide);

    ll_destroy(&l);
}

// SUITE 8 — ll_destroy
static void test_suite_destroy(void)
{
    section("SUITE 8 : ll_destroy");

    // destroy avec free_data = NULL 
    LList *l = ll_create(NULL);
    int a = 1, b = 2;
    ll_push_back(l, &a);
    ll_push_back(l, &b);
    ll_destroy(&l);
    CHECK(l == NULL, "ll_destroy : *l mis à NULL (free_data = NULL)");

    // destroy avec free_data = free (données sur le tas) 
    LList *lm = ll_create(free);
    for (int i = 0; i < 5; i++) {
        int *p = malloc(sizeof(int));
        *p = i * 10;
        ll_push_back(lm, p);
    }
    ll_destroy(&lm);
    CHECK(lm == NULL, "ll_destroy : *l mis à NULL (free_data = free, 5 nœuds)");

    // destroy sur liste vide 
    LList *vide = ll_create(NULL);
    ll_destroy(&vide);
    CHECK(vide == NULL, "ll_destroy sur liste vide : *l mis à NULL");

    // destroy avec pointeur NULL — ne doit pas planter 
    ll_destroy(NULL);
    CHECK(1, "ll_destroy(NULL) : pas de crash");
}

// SUITE 9 — test intégration : répertoire de contacts camerounais
static void test_suite_contacts(void)
{
    section("SUITE 9 : Intégration — répertoire de contacts camerounais");

    LList *annuaire = ll_create(free);   /* free_data = free car malloc */

    // Ajout de 5 contacts 
    ll_push_back(annuaire, make_contact("Manga Paul",    "+237699000001", "Yaoundé"));
    ll_push_back(annuaire, make_contact("Bello Fatima",  "+237677000002", "Douala"));
    ll_push_back(annuaire, make_contact("Nguemo Eric",   "+237655000003", "Bafoussam"));
    ll_push_back(annuaire, make_contact("Oumarou Sali",  "+237690000004", "Garoua"));
    ll_push_back(annuaire, make_contact("Abena Marie",   "+237670000005", "Bertoua"));

    CHECK(annuaire->length == 5, "Insertion de 5 contacts : length = 5");

    printf("\n  Affichage avec ll_foreach :\n");
    ll_foreach(annuaire, afficher_contact);

    // Vérification du 3e contact 
    Contact *c3 = (Contact *)ll_get(annuaire, 2);
    CHECK(c3 != NULL,                            "ll_get(2) : Nguemo Eric non-NULL");
    CHECK(strcmp(c3->ville, "Bafoussam") == 0,   "ll_get(2) : ville = Bafoussam");

    // Suppression par nom 
    const char *cible = "Bello Fatima";
    int ret = ll_remove(annuaire, (void *)cible, cmp_contact_nom);
    CHECK(ret == 0,                 "ll_remove Bello Fatima : retourne 0");
    CHECK(annuaire->length == 4,    "ll_remove Bello Fatima : length = 4");

    // Vérifier que Bello Fatima n'est plus là 
    int trouve = 0;
    for (size_t i = 0; i < annuaire->length; i++) {
        Contact *c = (Contact *)ll_get(annuaire, i);
        if (strcmp(c->nom, "Bello Fatima") == 0) { trouve = 1; break; }
    }
    CHECK(trouve == 0, "Après suppression : Bello Fatima introuvable");

    printf("\n  Répertoire après suppression :\n");
    ll_foreach(annuaire, afficher_contact);

    // Suppression d'un contact inexistant 
    ret = ll_remove(annuaire, (void *)"Inconnu Dupont", cmp_contact_nom);
    CHECK(ret == -1, "ll_remove contact inexistant : retourne -1");

    ll_destroy(&annuaire);
    CHECK(annuaire == NULL, "ll_destroy annuaire : *annuaire = NULL");
}

// SUITE 10 — cas limites et robustesse
static void test_suite_robustesse(void)
{
    section("SUITE 10 : Cas limites et robustesse");

    // Insertion massive 
    LList *l = ll_create(free);
    for (int i = 0; i < 1000; i++) {
        int *p = malloc(sizeof(int));
        *p = i;
        ll_push_back(l, p);
    }
    CHECK(l->length == 1000, "1000 push_back successifs : length = 1000");
    CHECK(*(int *)ll_get(l, 0)   == 0,   "Masse : premier élément = 0");
    CHECK(*(int *)ll_get(l, 999) == 999, "Masse : dernier élément = 999");
    ll_destroy(&l);
    CHECK(l == NULL, "Après 1000 nœuds : ll_destroy propre");

    // Mélange push_front / push_back 
    LList *l2 = ll_create(NULL);
    int a = 1, b = 2, c = 3, d = 4;
    ll_push_back(l2,  &b);   // → [2]       
    ll_push_front(l2, &a);   // → [1, 2]    
    ll_push_back(l2,  &c);   // → [1, 2, 3] 
    ll_push_front(l2, &d);   // → [4, 1, 2, 3] 

    CHECK(*(int *)ll_get(l2, 0) == 4, "Mélange front/back : [0] = 4");
    CHECK(*(int *)ll_get(l2, 1) == 1, "Mélange front/back : [1] = 1");
    CHECK(*(int *)ll_get(l2, 2) == 2, "Mélange front/back : [2] = 2");
    CHECK(*(int *)ll_get(l2, 3) == 3, "Mélange front/back : [3] = 3");
    ll_destroy(&l2);

    // pop_front jusqu'à vide puis push_back → liste doit rester cohérente 
    LList *l3 = ll_create(NULL);
    int x = 10, y = 20;
    ll_push_back(l3, &x);
    ll_pop_front(l3);                  // liste vide 
    ll_push_back(l3, &y);              // réinsertion 
    CHECK(l3->length == 1,             "Pop jusqu'à vide puis push : length = 1");
    CHECK(l3->head == l3->tail,        "Pop jusqu'à vide puis push : head == tail");
    CHECK(*(int *)ll_get(l3, 0) == 20, "Pop jusqu'à vide puis push : [0] = 20");
    ll_destroy(&l3);
}


int main(void)
{
    printf("\n");
    printf("  ══════════════════════════════════════════════════════════\n");
    printf("     TESTS — Module llist (liste chaînée générique)        \n");
    printf("  ══════════════════════════════════════════════════════════\n");

    test_suite_create();
    test_suite_push_back();
    test_suite_push_front();
    test_suite_pop_front();
    test_suite_get();
    test_suite_remove();
    test_suite_foreach();
    test_suite_destroy();
    test_suite_contacts();
    test_suite_robustesse();

    // Bilan final 
    printf("\n  ══════════════════════════════════════════════════════════\n");
    printf("  BILAN : %d test(s) exécuté(s)  |  ", tests_total);
    printf("\033[32m%d PASS\033[0m  |  ", tests_passed);
    if (tests_failed > 0)
        printf("\033[31m%d FAIL\033[0m\n", tests_failed);
    else
        printf("\033[32m%d FAIL\033[0m\n", tests_failed);
    printf("  ══════════════════════════════════════════════════════════\n\n");

    return (tests_failed == 0) ? 0 : 1;
}