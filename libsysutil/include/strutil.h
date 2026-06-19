#ifndef STRUTIL_H
#define STRUTIL_H
#include <stddef.h>
#include <stdbool.h>


size_t su_strlcpy(char *dst, const char *src, size_t dstsize); // copy securise garantit final \0

size_t su_strlcat(char *dst, const char *src, size_t dstsize); // concatenation securise

int su_itoa(int val, char *buf, size_t bufsize, int base); // convertion entier -> chaine sans snprintf

char *su_strtok_r(char *str, char delim, char **saveptr); // decouper la chaine par delimiteur (thread-safe)

char su_trim(char *str); // retire les espace en debut et en fin 

bool su_startswith(const char *str, const char *prefix); // verifie si str commence par prefix

#endif