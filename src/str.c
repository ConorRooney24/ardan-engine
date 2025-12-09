
#include <stdlib.h>

static char *str_whitespace_remove(char *str)
{
        char *start = str;

        while (*start && (*start == ' ' || *start == '\t' || *start == '\r' || *start == '\n'))
        {
                start++;
        }       

        if (start != str)
        {
                memmove(str, start, strlen(start) + 1);
        }
        
        // trailing
        size_t len = strlen(str);

        while (len > 0 && (str[len-1] == ' ' || str[len-1] == '\t' || str[len-1] == '\r' || str[len-1] == '\n'))
        {
                str[len-1] = '\0';
                len--;
        }

        return str;
}