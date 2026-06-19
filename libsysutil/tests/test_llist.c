#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "llist.h"

/* ─── helpers ─────────────────────────────────────────────── */

static int pass = 0, fail = 0;

/* strdup n'est pas C99 standard — implémentation locale */
static char *my_strdup(const char *s) {
    size_t len = strlen(s) + 1;
    char *copy = malloc(len);
    if (copy) memcpy(copy, s, len);
    return copy;
}

#define ASSERT(cond, msg) \
    do { \
        if (cond) { printf("  [PASS] %s\n", msg); pass++; } \
        else       { printf("  [FAIL] %s\n", msg); fail++; } \
    } while (0)

static void free_str(void *data) { free(data); }

static int cmp_str(const void *a, const void *b) {
    return strcmp((const char *)a, (const char *)b);
}

static char collected[256];
static void collect(void *data, size_t idx) {
    if (idx > 0) strcat(collected, ",");
    strcat(collected, (char *)data);
}

/* ─── tests ───────────────────────────────────────────────── */

static void test_create(void) {
    puts("=== ll_create ===");

    LList *l = ll_create(NULL);
    ASSERT(l != NULL,       "create retourne un pointeur non-NULL");
    ASSERT(l->head == NULL, "head initialisé à NULL");
    ASSERT(l->tail == NULL, "tail initialisé à NULL");
    ASSERT(l->length == 0,  "length initialisé à 0");
    ll_destroy(&l);
    ASSERT(l == NULL,       "destroy met *l à NULL");
}

static void test_push(void) {
    puts("=== ll_push_back / ll_push_front ===");

    LList *l = ll_create(NULL);
    ll_push_back(l, "B");
    ll_push_back(l, "C");
    ll_push_front(l, "A");

    ASSERT(l->length == 3,                "length == 3 après 3 insertions");
    ASSERT(strcmp(ll_get(l, 0), "A") == 0,"index 0 == 'A' (push_front)");
    ASSERT(strcmp(ll_get(l, 1), "B") == 0,"index 1 == 'B'");
    ASSERT(strcmp(ll_get(l, 2), "C") == 0,"index 2 == 'C'");

    ll_destroy(&l);
}

static void test_pop_front(void) {
    puts("=== ll_pop_front ===");

    LList *l = ll_create(NULL);
    ll_push_back(l, "X");
    ll_push_back(l, "Y");

    void *d = ll_pop_front(l);
    ASSERT(strcmp((char *)d, "X") == 0,   "pop_front retourne 'X'");
    ASSERT(l->length == 1,                "length == 1 après pop");
    ASSERT(strcmp(ll_get(l, 0), "Y") == 0,"head devient 'Y'");

    ll_pop_front(l);
    ASSERT(l->head == NULL,               "head NULL sur liste vide");
    ASSERT(l->tail == NULL,               "tail NULL sur liste vide");
    ASSERT(ll_pop_front(l) == NULL,       "pop_front sur liste vide retourne NULL");

    ll_destroy(&l);
}

static void test_get(void) {
    puts("=== ll_get ===");

    LList *l = ll_create(NULL);
    ll_push_back(l, "un");
    ll_push_back(l, "deux");

    ASSERT(strcmp(ll_get(l, 0), "un")   == 0, "get(0) == 'un'");
    ASSERT(strcmp(ll_get(l, 1), "deux") == 0, "get(1) == 'deux'");
    ASSERT(ll_get(l, 2) == NULL,              "get(index >= length) == NULL");
    ASSERT(ll_get(NULL, 0) == NULL,           "get(NULL, 0) == NULL");

    ll_destroy(&l);
}

static void test_remove(void) {
    puts("=== ll_remove ===");

    LList *l = ll_create(free_str);

    char *a = my_strdup("Alice");
    char *b = my_strdup("Bob");
    char *c = my_strdup("Charlie");
    ll_push_back(l, a);
    ll_push_back(l, b);
    ll_push_back(l, c);

    /* suppression milieu */
    int r = ll_remove(l, "Bob", cmp_str);
    ASSERT(r == 0,          "remove retourne 0 si trouvé");
    ASSERT(l->length == 2,  "length == 2 après remove");

    /* suppression tête */
    ll_remove(l, "Alice", cmp_str);
    ASSERT(strcmp(ll_get(l, 0), "Charlie") == 0, "nouveau head == 'Charlie'");

    /* suppression queue — tail doit être mis à jour */
    ll_remove(l, "Charlie", cmp_str);
    ASSERT(l->head == NULL, "head NULL après suppression totale");
    ASSERT(l->tail == NULL, "tail NULL après suppression totale");

    /* élément absent */
    ll_push_back(l, my_strdup("X"));
    ASSERT(ll_remove(l, "Z", cmp_str) == -1, "remove retourne -1 si absent");

    ll_destroy(&l);
}

static void test_foreach(void) {
    puts("=== ll_foreach ===");

    LList *l = ll_create(NULL);
    ll_push_back(l, "un");
    ll_push_back(l, "deux");
    ll_push_back(l, "trois");

    collected[0] = '\0';
    ll_foreach(l, collect);
    ASSERT(strcmp(collected, "un,deux,trois") == 0,
           "foreach parcourt dans l'ordre d'insertion");

    ll_foreach(NULL, collect);
    ll_foreach(l, NULL);
    ASSERT(1, "foreach ne plante pas avec arguments NULL");

    ll_destroy(&l);
}

static void test_destroy(void) {
    puts("=== ll_destroy ===");

    LList *l = ll_create(free_str);
    ll_push_back(l, my_strdup("a"));
    ll_push_back(l, my_strdup("b"));

    ll_destroy(&l);
    ASSERT(l == NULL, "destroy met *l à NULL (anti dangling-pointer)");

    ll_destroy(&l);
    ASSERT(1, "double destroy sans crash");
}

/* ─── main ────────────────────────────────────────────────── */

int main(void) {
    puts("====== Tests llist ======");

    test_create();
    test_push();
    test_pop_front();
    test_get();
    test_remove();
    test_foreach();
    test_destroy();

    printf("\n%d PASS  %d FAIL\n", pass, fail);
    return fail == 0 ? 0 : 1;
}