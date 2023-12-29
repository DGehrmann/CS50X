#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

bool only_digits(string s);
char rotate(char c, int m);

// argc = argument count: counts how many words are being typed after the command
// argv = argument vector: stores the words typed after the command in an array
int main(int argc, string argv[])
{
    if (argc == 1 || argc > 2)
    {
        printf("Usage: ./caesar key\n");
        return 1;
    }

    else if (only_digits(argv[1]) == false)
    {
        printf("Usage: ./caesar key\n");
        return 1;
    }

    //return 0;

    int key = atoi(argv[1]);
    //printf("Key = %i\n", key);

    string plain = get_string("Plaintext:  ");
    //printf("%s\n", plain);

    printf("Ciphertext: ");

    for (int i = 0, n = strlen(plain); i < n; i++)
    {
        printf("%c", rotate(plain[i], key));
    }
    printf("\n");
}

bool only_digits(string s)
{

    for (int i = 0, n = strlen(s); i < n; i++)
    {
        if (!isdigit(s[i]))
        {
            return false;
        }
    }

    return true;
}

char rotate(char c, int m)
{
    if (isupper(c))
    {
        int value = (char)c - 65;
        int new_value = value + (m % 26);
        char new_c = ((int)new_value % 26) + 65;
        return new_c;
    }

    else if (islower(c))
    {
        int value = (char)c - 97;
        int new_value = value + (m % 26);
        char new_c = ((int)new_value % 26) + 97;
        return new_c;
    }

    else
    {
        int new_c = c;
        return new_c;
    }
}