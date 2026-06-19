<<<<<<< HEAD
#ifndef LLIST_H
=======
 #include "llist.h"
 #include <stdlib.h>   
 

LList *ll_create(void (*free_data)(void *)){
    LList *l = malloc(sizeof(LList));
    if (l == NULL)
        return NULL;
 
    l->head      = NULL;
    l->tail      = NULL;
    l->length    = 0;
    l->free_data = free_data;
 
    return l;
}
 
 /* 
  ll_push_back
  Insère data en queue de liste.
  Retourne 0 en cas de succès, -1 si malloc échoue ou si l est NULL 
*/
int ll_push_back(LList *l, void *data){
    if (l == NULL)
        return -1;
 
    LLNode *node = malloc(sizeof(LLNode));
    if (node == NULL)
        return -1;
 
    node->data = data;
    node->next = NULL;
 
    if (l->tail == NULL) {
        // Liste vide : head et tail pointent tous les deux vers le nouveau nœud 
        l->head = node;
        l->tail = node;
    } else {
        // Liste non vide : on chaîne après l'actuel tail, puis on avance tail 
        l->tail->next = node;
        l->tail       = node;
    }
 
    l->length++;
    return 0;
}
 
 /* 
  ll_push_front
  Insère data en tête de liste.
  Retourne 0 en cas de succès, -1 si malloc échoue ou si l est NULL.
 */
int ll_push_front(LList *l, void *data){
    if (l == NULL)
         return -1;
 
    LLNode *node = malloc(sizeof(LLNode));
    if (node == NULL)
        return -1;
 
    node->data = data;
    node->next = l->head;   // le nouveau nœud pointe vers l'ancien head 
 
    if (l->head == NULL) {
        // Liste vide : tail doit aussi pointer vers ce nœud unique 
        l->tail = node;
    }
 
    l->head = node;
    l->length++;
    return 0;
}
 
 /* 
  ll_pop_front
  Retire et retourne la donnée du premier nœud.
  Le nœud LLNode est libéré, mais PAS la donnée (responsabilité de l'appelant).
  Retourne NULL si la liste est vide ou NULL.
 */
void *ll_pop_front(LList *l){
    if (l == NULL || l->head == NULL)
        return NULL;
 
    LLNode *old_head = l->head;
    void   *data     = old_head->data;
 
    l->head = old_head->next;
 
    // Si la liste devient vide après le retrait, tail doit aussi être NULL 
    if (l->head == NULL)
        l->tail = NULL;
 
    free(old_head);
    l->length--;
 
    return data;
}
 
 /* 
  ll_get
  Accès en O(n) par index.
  Retourne la donnée à la position index, ou NULL si index >= length ou l NULL.
 */
void *ll_get(const LList *l, size_t index){
    if (l == NULL || index >= l->length)
        return NULL;
 
    LLNode *cur = l->head;
    for (size_t i = 0; i < index; i++)
        cur = cur->next;
 
    return cur->data;
}
 
 /* 
  ll_remove
  Cherche le premier nœud dont la donnée vérifie cmp(data, data_cible) == 0.
  Supprime ce nœud, appelle free_data sur la donnée si le callback est défini.
  Retourne  0 si un nœud a été supprimé.
  Retourne -1 si rien trouvé, ou si l / cmp est NULL.
 */
int ll_remove(LList *l, void *data, int (*cmp)(const void *, const void *)){
    if (l == NULL || cmp == NULL || l->head == NULL)
        return -1;
 
    LLNode *prev = NULL;
    LLNode *cur  = l->head;
 
    while (cur != NULL) {
        if (cmp(cur->data, data) == 0) {
            // Nœud trouvé — on le décroche de la chaîne 
 
            if (prev == NULL) {
                // C'est le premier nœud 
                l->head = cur->next;
            } else {
                prev->next = cur->next;
            }
 
            // Si c'était le dernier nœud, mettre tail à jour 
            if (cur->next == NULL)
                l->tail = prev;
 
            // Libérer la donnée si le callback est fourni 
            if (l->free_data != NULL)
                l->free_data(cur->data);
            
            free(cur);
            l->length--;
            return 0;
        }
        prev = cur;
        cur  = cur->next;
    }
    return -1;   // aucun nœud correspondant trouvé 
 }
 
 /* 
  ll_foreach
  Itère sur tous les nœuds et appelle fn(data, idx) pour chacun.
  Ne fait rien si l ou fn est NULL.
 */
void ll_foreach(const LList *l, void (*fn)(void *data, size_t idx)){
    if (l == NULL || fn == NULL)
        return;
 
    LLNode *cur = l->head;
    size_t  idx = 0;
 
    while (cur != NULL) {
        fn(cur->data, idx);
        cur = cur->next;
        idx++;
    }
}
 
 /* 
  ll_destroy
  Libère tous les nœuds et les données (via free_data si non NULL).
  Met *l à NULL pour éviter les dangling pointers chez l'appelant.
  Ne fait rien si l ou *l est NULL.
 */
void ll_destroy(LList **l){
    if (l == NULL || *l == NULL)
        return;
 
    LLNode *cur = (*l)->head;
 
    while (cur != NULL) {
        LLNode *next = cur->next;   // sauvegarder next avant de libérer cur 
 
        if ((*l)->free_data != NULL)
            (*l)->free_data(cur->data);
 
        free(cur);
        cur = next;
    }
 
    free(*l);
    *l = NULL;   // invalide le pointeur de l'appelant — anti dangling pointer 
}
>>>>>>> 47c8fcf991530f15da7192efd4144b66c0e7ff9f
