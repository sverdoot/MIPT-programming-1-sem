#include "hash functions.h"
#include <string.h>
#include <assert.h>

int HF_const_0 (char* str)
    {
    return 0;
    }

int HF_first_letter (char* str)
    {
    assert (str);
    return (*str)*2;
    }

int HF_strlen (char* str)
    {
    return strlen (str);
    }

int HF_sum (char* str)
    {
    int len = strlen (str);
    int sum = 0;
    for (int i = 0; i < len; i++)
        {
        sum += *str;
        str++;
        }
    return sum;
    }

int HF_avg (char* str)
    {
    return HF_sum (str) / strlen (str);
    }

