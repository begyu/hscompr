#include <ctype.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#define MAXW 96

#define FALSE false
#define TRUE  true

char *hwords[MAXW] = { " a "," az", " be", " de", " el", " és ", " ha", " is",
 " jó", " ki", " le", " mi", " új", "akar", "aki", "akkor", "ala", "áll",
 "amely", "ami", "arr", "bár", "csak", "ebbe", "egy", "egész", "ember",
 "ennek", "épp", "erre", "eset", "ezer", "ezért", "ezt", "fel", "fog", "for",
 "fél", "hely", "hisz", "hogy", "idő", "igen", "így", "ilyen", "itt",
 "kap", "kell", "kép", "kér", "két", "köz", "lát", "lehet", "mag", "majd",
 "meg", "mert", "min", "mit", "mond", "most", "még", "már", "más", "nagy",
 "nap", "neki", "nem", "olyan", "össze", "ott", "rész", "sem", "sok", "sor",
 "szer", "szét", "szám", "száz", "szó", "tart", "tel", "tesz", "tud",
 "több", "tör", "úgy", "ugyan", "után", "vagy", "val", "van", "vesz",
 "vis", "volt" };


void strextr(const char *s, char *dst)
{
    register int i, j, k;
    unsigned int d,t,z;
    unsigned char c;

    i = j = 0;
    while (i < strlen(s))
    {
        c = s[i];
        d = (unsigned char)s[i+1]-32;
        if (c < 6)
        {
            if (c == 2)
                d += 224;
            else if (c == 3)
                d += 448;
            else if (c == 4)
                d += 672;
            else if (c == 5)
                d += 896;
            z = (int)(d / 100);
            t = (int)((d-(100*z)) / 10);
            dst[j] = z + '0';
            dst[j+1] = t + '0';
            dst[j+2] = (d % 10) + '0';
            i += 2;
            j += 3;
        }
        else if (c != 6)
        {
            if (c == 7)
            {
                for (k=0; k<d; k++)
                {
                    dst[j] = ' ';
                    j++;
                }
                i += 2;
                continue;
            }
            else
            {
                dst[j] = s[i];
                i++; j++;
                continue;
            }
        }
        else
        {
            for (k=0; k<strlen(hwords[d]); k++)
            {
                dst[j] = hwords[d][k];
                j++;
            }
            i += 2;
        }
    }
    dst[j] = '\0';
}

void strcompr(const char *str, char *cpr)
{
    register int i, j, k;
    int n, m;
    unsigned int x;
    int lim = strlen(str)-3;

    j =0;
    for (i=0; i<lim; i++)
    {
        if ((isdigit(str[i]) && isdigit(str[i+1]) && isdigit(str[i+2])))
        {
            x = 100 * (str[i] - '0');
            x += 10 * (str[i+1] - '0');
            x += (str[i+2] - '0');
            if (x < 224)
            {
                cpr[j] = 1;
                cpr[j+1] = x+32;
            }
            else if (x < 448)
            {
                cpr[j] = 2;
                cpr[j+1] = x-192;
            }
            else if (x < 672)
            {
                cpr[j] = 3;
                cpr[j+1] = x-416;
            }
            else if (x < 896)
            {
                cpr[j] = 4;
                cpr[j+1] = x-640;
            }
            else
            {
                cpr[j] = 5;
                cpr[j+1] = x-864;
            }
            i += 2;
            j += 2;
        }
        else
        {
            x = 0;
            for (k=0; k<MAXW; k++)
            {
                if (str[i] == hwords[k][0])
                {
                    n = i;
                    m = 0;
                    while (str[n] == hwords[k][m])
                    {
                        n++; m++;
                    }
                    if (hwords[k][m] == '\0')
                    {
                        cpr[j] = 6;
                        cpr[j+1] = (unsigned char)(k + 32);
                        i += m;
                        i--;
                        j += 2;
                        x = 1;
                    }
                }
                if (x == 1)
                    break;
            }
            if (x == 0)
            {
                n = i;
                k = 0;
                while (str[n] == ' ' && k < MAXW)
                {
                    k++;
                    n++;
                }
                if (k > 1)
                {
                    cpr[j] = 7;
                    cpr[j+1] = (unsigned char)(k + 32);
                    i += k;
                    i--;
                    j += 2;
                }
                else
                {
                    cpr[j] = str[i];
                    j++;
                }
            }
        }
    }
    lim += 3;
    while (i < lim)
    {
        cpr[j] = str[i];
        i++;
        j++;
    }
    cpr[j] = '\0';
}

void compress(char *src, char *dst, bool rev)
{
    if (rev)
       	strextr(src, dst);
    else
       	strcompr(src, dst);
}


#ifdef TEST
#define MAXS 1024
int main(int ac, char **av)
{
    char s1[MAXS+1];
    char s2[MAXS+1];

    if (ac > 1)
    {
        if (av[1][0] == 'c')
        {
            while (gets(s1))
            {
                compress(s1, s2, FALSE);
                puts(s2);
            }
        }
        else if (av[1][0] == 'd')
        {
            while (gets(s1))
            {
                compress(s1, s2, TRUE);
                puts(s2);
            }
        }
    }
    else
        printf("\n%s <c|d>",av[0]);
    
    return 0;
}
#else
#ifdef TEST2
#define MAXS 1024
int main(int ac, char **av)
{
    char s1[MAXS+1];
    char s2[MAXS+1];
    int l1, l2;
    float p1, p2;

    if (ac > 1)
    {
        l1 = strlen(av[1]);
        compress(av[1], s1, FALSE);
        l2 = strlen(s1);
        printf("\norig length=%d", l1);
        p1 = (float)l1/100;
        p2 = (float)l2/p1;
        printf("\n compressed=\"%s\"", s1);
        printf("\ncomp.length=%d", l2);
        printf(" (%02.02f%%)", p2);
        compress(s1, s2, TRUE);
        printf("\ndecoded str=\"%s\"", s2);
    }
    return 0;
}
#endif
#endif

