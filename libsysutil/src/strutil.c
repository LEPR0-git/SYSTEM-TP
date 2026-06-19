#include "strutil.h"

size_t su_strlcpy(char *dst, const char *src, size_t dstsize){
    // evaluons la taille de la source
    char *src_temp = src;
    int src_taille = 0;
    while(*src_temp != '\0')
        {
            src_taille++;
            src_temp +=1;
        }
    // maintenant on copy du plus petit des 2 taille
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
    char ch_cat[dstsize];
    int src_taille = su_strlcpy(ch_cat, src, dstsize);
    int rst_taille = src_taille + dstsize;
    char rst[rst_taille];
    int i=0;
    while( *ch_cat != '\0'){
        rst[i] = dst[i];
        rst[dstsize + i ] = ch_cat[i];
        i++;
    }
    rst[rst_taille] = '\0';
    return  rst_taille;
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
    // decoupage de la chaine avec le delimiteur delim
    // on va compte le nombre de block 
    char *str_temp = str;
    int count=0;
    while (*str_temp != '\0' )
    {
        count +=1;
        
        do{str_temp +=1;}while(*str_temp != delim && *str_temp!= '0');
    }
    count = (*str == delim)? count + 1:count;
    // on va alouer de l'espace a chaque bloc tout en lisant son contenue et le copier
    saveptr = malloc(sizeof(char*)*count);
    int i=0;
    str_temp = str;
    char *str_temp2 = str;
    for(int j=0; j<count;j++){
    while(*str_temp != '\0')
    { i=0;
        do{
        if(*str_temp != delim){
            i++;
            break;
        }
        *str_temp +=1;
    }while(*str_temp != '\0');

    saveptr[j] = malloc(sizeof(char)*i);
    for(int k=0; k<i;k++){
        saveptr[j][k] = str_temp2++;
    }
    saveptr[j][i] = '\0';
    // on deplace du delimiteur 
    str_temp2++;

    }
    }
    return *saveptr;
}

char su_trim(char *str){
    // on va trimmer la chaine str

}

bool su_startswith(const char *str, const char *prefix); // verifie si str commence par prefix
