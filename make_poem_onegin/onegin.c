#include <locale.h>
#include <io.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <ctype.h>

void strsort (char *lineptr[], int left, int right);
void rev (char buf[], int right);  // rev (buf + left);
int scmp (char *s1, char *s2);
void swap1(char**a, char**b);      // swap (char* *a, char* *b);
void writelines ( char *lineptr[], FILE *fileout, int number);
int fillbuf (char buffer[], int bsize);
void fillptr (char *lineptr[], char buffer[], int number);
char lower (char c);
int getfsize ( FILE *filein );

int main()
    {
    FILE *filein = fopen ("fscanf1.txt", "r");
    FILE *fileout = fopen ("fprintf1.txt", "w");
    assert (filein); assert (fileout);

    int fsize = getfsize (filein);//размер файла

    char *buffer = (char*) calloc (fsize+1, sizeof (char));//создание буффера, содержащего текст целиком
    int number = fillbuf (buffer, fread (buffer, 1, fsize, filein));//количество строк
    char **lineptr = (char**) calloc (number, sizeof (char *));//массив указателей на начала строк

    fillptr (lineptr, buffer, number);//заполнение lineptr
    strsort (lineptr, 0, number-1);//сортировка строк
    writelines (lineptr, fileout, number);//запись отсортированных строк в файл

    fclose (filein);
    fclose (fileout);
    free (buffer);
    free (lineptr);
    return 0;
    }


void rev (char buf[], int right)
    {
     for (int left = 0; left < right; left++, right--)
        {
         char c = buf[left];
         buf[left] = buf[right];
         buf[right] = c;

        }
    }

void strsort (char * lineptr[], int left, int right)
    {
    if (left >= right)
        return;
    swap1 (lineptr + left, lineptr + (left+ right)/2);
    int last = left;
    for (int i = left+1; i <= right; i++)
        if (scmp (lineptr[i], lineptr[left]) < 0)
            {
            ++last;
            swap1 (lineptr+last, lineptr +i);
            }
    swap1 (lineptr + left, lineptr + last);
    strsort (lineptr, left, last-1);
    strsort (lineptr, last+1, right);
    }

void swap1 (char **a, char **b)
    {
    char *temp;
    temp = *a;
    *a = *b;
    *b = temp;
    }

int scmp (char *s1, char *s2)
    {
    //setlocale(LC_ALL, "Russian");
    int i = 0;
    int j = 0;
    do
        {
        if (s1[i] == '\0')
            return 0;
        while ((tolower (s1[i]) < 'а' || tolower (s1[i]) > 'я') && s1[i] != '\0')
            i++;
        while ((tolower (s2[j]) < 'а' || tolower (s2[j]) > 'я') && s2[j] != '\0')
            j++;
        }
    while (tolower (s1[i++]) == tolower (s2[j++]));

    return tolower (s1[--i]) - tolower (s2[--j]);
    }


void writelines (char *lineptr[], FILE *fileout, int number)
    {
    for (int i = 0; i < number; i++)
        {
        rev (lineptr[i], strlen (lineptr[i])-1);
        if ((lineptr[i][0] == '\t')) //сделано так исключительно для данной версии текста
            {
            fputs (lineptr[i], fileout);
            fputs ("\n", fileout);
            }
        }
    }

int fillbuf (char *buffer, int bsize)
    {
    int number = 0;

    if (buffer[bsize-1] == '\n')
        rev (buffer, bsize - 2);
    else
        rev (buffer, bsize - 1);

    for (int i = 0; i <= bsize; i++)
        {
        if (buffer[i] == '\n')
            {
            ++number;
            buffer[i] = '\0';
            }
        }

    if (buffer[bsize-1] != '\n')
       ++number;

    buffer[bsize] = '\0';

    return number;
    }

void fillptr (char *lineptr[], char buffer[], int number)
    {
    int j = 0;
    for (int i = 0; i < number; i++)
        {
        lineptr[i] = &buffer[j];

        while ( buffer[j++] != '\0');
        }
    }

int getfsize (FILE *filein )
    {
    fseek (filein, 0, SEEK_END);
    int fsize = ftell (filein);//размер файла
    fseek (filein, 0, SEEK_SET);
    assert (fsize != -1);
    return fsize;
    }
/*char lower (char c)
    {
    setlocale(LC_ALL, "Rus");
    if (c >= 'А' && c <= 'П')
        return с + 'а' - 'А';
    else if (c >= 'Р' && c <= 'Я')
        return с + 'р' - 'Р';
    else if (c == 'Ё')
        return 'ё';
    else return c;
    }*/
