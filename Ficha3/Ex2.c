#include <stdlib.h>
#include <string.h>
#include <stdio.h>


unsigned hash(char *str){
    unsigned hash = 5381;
    int c;
    
    while (c = *str++)
        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */
    
    return hash;
}

#define Size 10
#define Free 0
#define Used 1
#define Del  2

typedef struct bucket {
    int status; // Free | Used | Del
    char *chave; int ocorr;
} THash [Size];

int where (char *s, THash t)
{
    return hash(s)%Size;
}

void initEmpty (THash t)
{
    int i = Size;

    while (i)
    {
        t[--i].status = Free;
        t[i].chave = NULL;
        t[i].ocorr = 0;
    }
}

void add (char *s, THash t)
{
    int indx = where(s,t);
    
    int verified = 0;
    while (verified<=Size)
    {
        if (t[indx].status == Free)
        {
            t[indx].status = Used;
            t[indx].chave = strdup(s);
            t[indx].ocorr = 1;
            return;
        }

        if (!strcmp(t[indx].chave, s)) 
        {
            t[indx].ocorr++;
            return;
        }

        verified++;
        indx = (indx+1)%Size;
    }
    printf("Couldn't add word <%s>\n", s);
}

int lookup (char *s, THash t)
{
    int indx = where(s,t);

    int verified = 0;
    while (verified<=Size)
    {
        if (t[indx].status==Free) return -1;

        if (!strcmp(t[indx].chave, s)) return t[indx].ocorr;

        verified++;
        indx = (indx+1)%Size;
    }
    return -1;
}

int Remove (char *s, THash t)
{
    int indx = where(s,t);

    int verified = 0;
    while (verified<=Size)
    {
        if (t[indx].status==Free) return -1; 

        if ((t[indx].status==Used) && !strcmp(t[indx].chave, s)) 
        {
            if (!--t[indx].ocorr)
            {
                t[indx].status = Del;
                t[indx].chave  = NULL;
            }
            
            return 0;
        }

        verified++;
        indx = (indx+1)%Size;
    }
    return -1;
}

int garb_collection (THash t)
{   
    for (int i = 0 ; i<Size ; i++)
        if (t[i].status == Del)
            t[i].status = Free;
    return 0;
}

int main() {
  
    THash tab;
  
    initEmpty(tab);
  
    add((char*)"Ola", tab);
    add((char*)"Ola", tab);
    add((char*)"Adeus", tab);
    add((char*)"d", tab);
    add((char*)"M", tab);
    add((char*)"e", tab);
    add((char*)"s", tab);
    add((char*)"a", tab);
    add((char*)"gfs", tab);
    add((char*)"popo", tab);
    add((char*)"puta", tab);
    add((char*)"afasf", tab);

    printf("%d\n", Remove((char*)"Ola", tab));
    printf("%d\n", Remove((char*)"Ola", tab));
    printf("%d\n", Remove((char*)"Ola", tab));

    garb_collection(tab);

    add((char*)"afasf", tab);

    return 0;
}