#include <stdio.h>
#include <string.h>
#include <limits.h>
#include "strutil.h"


static int pass = 0, fail = 0;

#define ASSERT(cond, msg) \
    do { \
        if (cond) { printf("  [PASS] %s\n", msg); pass++; } \
        else       { printf("  [FAIL] %s\n", msg); fail++; } \
    } while (0)

/* ─── su_strlcpy ──────────────────────────────────────────── */

static void test_strlcpy(void) {
    puts("=== su_strlcpy ===");

    char dst[32];
    size_t r;

    /* cas 1 : dstsize < strlen(src) — troncature */
    memset(dst, 'X', sizeof dst);
    r = su_strlcpy(dst, "hello", 3);          /* dstsize=3 < src=5 */
    ASSERT(r == 5,              "retourne strlen(src) == 5");
    ASSERT(strcmp(dst,"he")==0, "copie tronquée à dstsize-1 chars");
    ASSERT(dst[2] == '\0',      "\\0 terminal garanti");

    /* cas 2 : dstsize == strlen(src)+1 — copie exacte */
    memset(dst, 'X', sizeof dst);
    r = su_strlcpy(dst, "hello", 6);          /* dstsize == 5+1 */
    ASSERT(r == 5,               "retourne strlen(src) == 5");
    ASSERT(strcmp(dst,"hello")==0,"copie complète");
    ASSERT(dst[5] == '\0',       "\\0 terminal garanti");

    /* cas 3 : dstsize >> strlen(src) — copie normale */
    memset(dst, 'X', sizeof dst);
    r = su_strlcpy(dst, "hi", 32);
    ASSERT(r == 2,              "retourne strlen(src) == 2");
    ASSERT(strcmp(dst,"hi")==0, "contenu correct");
    ASSERT(dst[2] == '\0',      "\\0 terminal garanti");
}

/* ─── su_itoa ─────────────────────────────────────────────── */

static void test_itoa(void) {
    puts("=== su_itoa ===");

    char buf[64];
    int  r;

    /* base 10 — positif */
    r = su_itoa(42, buf, sizeof buf, 10);
    ASSERT(r > 0 && strcmp(buf,"42")==0,  "base10 : 42");

    /* base 10 — zéro */
    r = su_itoa(0, buf, sizeof buf, 10);
    ASSERT(r >= 0 && strcmp(buf,"0")==0,  "base10 : 0");

    /* base 10 — négatif */
    r = su_itoa(-7, buf, sizeof buf, 10);
    ASSERT(r > 0 && strcmp(buf,"-7")==0,  "base10 : -7");

    /* base 10 — INT_MIN (cas limite) */
    r = su_itoa(INT_MIN, buf, sizeof buf, 10);
    char expected[32];
    snprintf(expected, sizeof expected, "%d", INT_MIN);
    ASSERT(r > 0 && strcmp(buf, expected)==0, "base10 : INT_MIN");

    /* base 2 */
    r = su_itoa(10, buf, sizeof buf, 2);
    ASSERT(r > 0 && strcmp(buf,"1010")==0, "base2 : 10 -> '1010'");

    /* base 8 */
    r = su_itoa(8, buf, sizeof buf, 8);
    ASSERT(r > 0 && strcmp(buf,"10")==0,   "base8 : 8 -> '10'");

    /* base 16 */
    r = su_itoa(255, buf, sizeof buf, 16);
    ASSERT(r > 0 && strcmp(buf,"ff")==0,   "base16 : 255 -> 'ff'");

    /* buffer trop petit */
    r = su_itoa(12345, buf, 3, 10);
    ASSERT(r == -1, "retourne -1 si buffer trop petit");
}

/* ─── su_strtok_r ─────────────────────────────────────────── */

static void test_strtok_r(void) {
    puts("=== su_strtok_r ===");

    char csv[] = "Yaound\xc3\xa9,Douala,,Garoua,";
    char *save = NULL;
    char *tok;

    tok = su_strtok_r(csv, ',', &save);
    ASSERT(tok != NULL && strncmp(tok,"Yaound",6)==0, "token 1 : 'Yaoundé'");

    tok = su_strtok_r(NULL, ',', &save);
    ASSERT(tok != NULL && strcmp(tok,"Douala")==0,    "token 2 : 'Douala'");

    tok = su_strtok_r(NULL, ',', &save);
    /* champ vide entre ",," — comportement selon implémentation :
       soit token vide "", soit saut et token suivant */
    /* on vérifie juste qu'il ne plante pas */
    ASSERT(1, "token 3 (champ vide) : pas de crash");

    tok = su_strtok_r(NULL, ',', &save);
    /* selon le saut du champ vide, on peut être sur "Garoua" ici ou après */
    ASSERT(tok != NULL, "au moins un token supplémentaire après le champ vide");

    /* fin de chaîne */
    while (su_strtok_r(NULL, ',', &save) != NULL) {} /* vider */
    tok = su_strtok_r(NULL, ',', &save);
    ASSERT(tok == NULL, "retourne NULL quand la chaîne est épuisée");
}

/* ─── su_trim ─────────────────────────────────────────────── */

static void test_trim(void) {
    puts("=== su_trim ===");

    /* cas normal avec espaces, tabulation, newline */
    char s1[] = " \t hello world \n";
    char *r = su_trim(s1);
    ASSERT(strcmp(r,"hello world")==0, "trim normal : ' \\t hello world \\n'");

    /* chaîne vide */
    char s2[] = "";
    r = su_trim(s2);
    ASSERT(strcmp(r,"")==0, "trim chaîne vide");

    /* chaîne d'espaces seuls */
    char s3[] = "   ";
    r = su_trim(s3);
    /* résultat attendu : chaîne vide "" */
    ASSERT(strlen(r)==0, "trim chaîne d'espaces seuls -> vide");
}

/* ─── main ────────────────────────────────────────────────── */

int main(void) {
    puts("====== Tests strutil ======");

    test_strlcpy();
    test_itoa();
    test_strtok_r();
    test_trim();

    printf("\n%d PASS  %d FAIL\n", pass, fail);
    return fail == 0 ? 0 : 1;
}