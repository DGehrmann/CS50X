#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

typedef uint8_t BYTE;

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        printf("Usage: ./recover IMAGE\n");
        return 1;
    }

    FILE *raw_file = fopen(argv[1], "r");   //opens whatever is stored in argv[1]
    if(raw_file == NULL)
    {
        printf("Could not open %s!\n", argv[1]);
        return 1;
    }

    //read from memory-card (raw_file) into buffer-array in blocks of 512 bytes
    //create buffer with size 512 bytes
    int jpgcounter = 0;
    int headercounter = 0;
    char *filename = malloc(8); //Platz schaffen in memory für filename
    FILE *img = NULL;
    BYTE buffer[512];

    while (fread(&buffer, 1, 512, raw_file) == 512)
    {
        //check if it's a jpeg-file
        if ((buffer[0] == 0xff) && (buffer[1] == 0xd8) && (buffer[2] == 0xff) && ((buffer[3] & 0xf0) == 0xe0))
        {
            //Check, ob vorher schon ein jpeg gefunden wurde. Wenn ja, schließe aktuelles file

            if (headercounter == (jpgcounter + 1))
            {
                fclose(img);
                jpgcounter++;
            }

            //make new jpeg file once found the first header:
            sprintf(filename, "%03i.jpg", jpgcounter);
            img = fopen(filename, "w");
            fwrite(&buffer, 1, 512, img);
            headercounter++;
        }

        else if (headercounter > 0)
        {
            fwrite(&buffer, 1, 512, img);
        }
    }
    fclose(img);
    fclose(raw_file);
    free(filename);
}