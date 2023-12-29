#include <cs50.h>
#include <stdio.h>

int main(void)
{
    int height;
    do
    {
        height = get_int("Height: ");
    }
    while (height < 1 || height > 8);

    // rows
    int k = height - 1;
    for (int i = 0; i < height; i++)
    {
        for (int l = 0; l < k; l++)
        {
            printf(" ");
        }
        // columns
        for (int j = 0; j < height-k; j++)
        {
            printf("#");
        }

        k--;
        printf("\n");

    }
}