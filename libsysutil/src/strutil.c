#include "strutil.h"

size_t su_strlcpy(char *dst, const char *src, size_t dstsize){
    // evaluons la taille de la source
   const char *src_temp = src;
    int src_taille = 0;
    while(*src_temp != '\0')
        {
            src_taille++;
            src_temp +=1;
        }
    // maintenant on copy du plus petit des 2 taille
    if (dstsize <= 0) return src_taille;
    int count = (src_taille < dstsize -1)?src_taille:dstsize-1;
    for (size_t i = 0; i < count; i++)
    {
        dst[i] = src[i];
    }
    dst[count] = '\0';


    return src_taille;
}


size_t su_strlcat(char *dst, const char *src, size_t dstsize){
    // fonction de concatenation , recuperons d'abord la chaine a concatene
    int dest_taille = 0, src_taille = 0;

    // on compte le nombre d'element dans la destination
    const char * dest_temp = dst;
        while(*dest_temp != '\0'){
            dest_taille++;
            dest_temp+=1;
        }
    // on compte la taille de la src
    dest_temp = src;
        while(*dest_temp != '\0'){
            src_taille++;
            dest_temp+=1;
        }
        int i=0;
        if (dstsize <=0) return (dest_taille + src_taille);
        int count = (dstsize-1 < (dest_taille + src_taille))?dstsize-1: (dest_taille + src_taille);
    while(dest_taille + i < count){
        dst[dest_taille + i] = src[i];
        i++;
    }
    dst[i+dest_taille] = '\0';
    return  dest_taille + src_taille;
}


int su_itoa(int val, char *buf, size_t bufsize, int base){
    
    // verifion la taille du buffer
    if(bufsize <= 0)
        return -1;

    char convert[] = {'0','1','2','3','4','5','6','7','8','9','a','b','c','d','e','f','\0'};
    // verifion si la valeur est negatif ou positif
    if(val < 0 || base == 10){
        // negatif vers positif
        int j=0;
        if(val<0){
            buf[j] = '-';
            j++;
        }
        val = (val>0)?val : -1 *val;
        // si c'est un chiffre dans [0, 9] on le convertit facilement
        if(j>bufsize -1)
            return -1;
        if(val <=9)
            {
                buf[j] = convert[val];
                return 1;
            }

        // on va se servir de la decomposition des nombre pour la convertion en chaine
        // calculons d'abord l'unite max
        int val_temp=val, divi = 10;
        int nrb_ele=1;
        do{
            nrb_ele ++;
            divi *=10;
        }
        while ((val / divi) >= 1);
            
       
        // decomposition du nombre
        
        for(int i=divi/10; j<=nrb_ele ; i/=10 , j++){
            int count = 0;
            if(i==1 && j < bufsize -1){
                
                buf[j] = convert[val];
                break;
            }

            else{
                
            do{
            val_temp = val - i;
            
            if(val_temp < i)
                break;
            count++;
            val = val_temp;
            }
            while((val_temp / i) != 0);
            if(val > i){
                count++;
                val = val-i;
            }
         
            if(j < bufsize -1){
            buf[j] = convert[count];
            count = 0;
        }
        else{
            //buf[j] = '\0';
            return -1;
        }
        }
         
    }

    return nrb_ele;
    }
    




    int rst;
    int i=0;
    char rst_c[bufsize];
    while(val !=0){
        rst = val % base;
        val = val / base;
        if(i<bufsize-1){
            rst_c[i] = convert[rst];
            i++;
        }
        else{
            
            for(int l=i;l<=0;l--){
                buf[i-l] = rst_c[l]; 
                printf(" valeur convertit %s ",buf);
                
        }
        buf[i] = '\0';
        return -1;
        }
        
            
    }
    for(int l=i-1;l>=0;l--)
                buf[i-l-1] = rst_c[l]; 
            printf("voici la derniere valeur %d\n",i);
                buf[i] = '\0';
return i;
}

char *su_strtok_r(char *str, char delim, char **saveptr){
    //on verifie si le dernier token etais null
    // on va verifier que la chainne n'est pas NULL
    if(str != NULL)
        *saveptr = str;
    // on evalue si on est a un delimiteur ou a la fin de la chaine 
    if(*saveptr == NULL || **saveptr == '\0') // fin de lecture
        return NULL;
    while(**saveptr == delim)
            (*saveptr)++;
    if(**saveptr == '\0')
        return NULL;
        
char *token = *saveptr;
    // on positionne ferme le prochain delim ou a la fin
    while(**saveptr != delim && **saveptr != '\0'){
            (*saveptr)++;   

    }
    if(**saveptr == delim){
    **saveptr = '\0';
    (*saveptr)++; 
    }
    return token;
}

char *su_trim(char *str){
// on va chercher la position du premier et dernier element non vide
int posi_first = -1, posi_last = 0;


int i=0;
while(*(str+i) != '\0'){
    if(*(str+i) != ' ' && posi_first == -1){
        posi_first = i; 
        posi_last = i;
    }
    if(posi_first != -1 && *(str+i) != ' ')
        posi_last = i;
i++;
}

// maintenant on trim
if(posi_first==posi_last && posi_first==0)
    return str;
if(posi_first==posi_last && posi_first!=0)
    {
        *(str +posi_last +1) = '\0';
    }
if(posi_first != posi_last){
    for(i=0;i<=posi_last;i++){
        *(str+i) = *(str+i+posi_first);
    }
    *(str+posi_last - posi_first +1) = '\0';
}

return str;
}

bool su_startswith(const char *str, const char *prefix){
    // on verifie si str commence par le prefix
    bool meme = true;
    if(str == NULL || prefix == NULL)
        return false;
        int i=0;
    while(true){
        if(*(str + i) != *(prefix +i)){
            return false;
        }
        if(*(prefix + i + 1) == '\0' && ( *(str +i + 1) == '\0' || *(str +i + 1) != '\0')){
            return meme;
        }
        
        i++;
    }
}
