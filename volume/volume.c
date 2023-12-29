// Modifies the volume of an audio file

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

// Number of bytes in .wav header
const int HEADER_SIZE = 44;

int main(int argc, char *argv[])
{
    // Check command-line arguments
    if (argc != 4)
    {
        printf("Usage: ./volume input.wav output.wav factor\n");
        return 1;
    }

    // Open files and determine scaling factor
    FILE *input = fopen(argv[1], "r");
    if (input == NULL)
    {
        printf("Could not open file.\n");
        return 1;
    }

    FILE *output = fopen(argv[2], "w");
    if (output == NULL)
    {
        printf("Could not open file.\n");
        return 1;
    }

    float factor = atof(argv[3]);

    // TODO: Copy header from input file to output file

    uint8_t header[HEADER_SIZE];
    fread(header, HEADER_SIZE, 1, input);
    fwrite(header, HEADER_SIZE, 1, output);

    // TODO: Read samples from input file and write updated data to output file

    int16_t buffer;
    //while-loop: wenn fread das Ende der Datei erreicht, gibt es 0 aus --> false, while-loop bricht ab!
    while (fread(&buffer, sizeof(int16_t), 1, input)) // &buffer=Addresse davon, wo hin geschrieben werden soll; sizeof(int16_t)=jedes sample hat größe int16_t..; input=was soll gelesen werden
    {
        buffer = buffer * factor;   //im buffer befindet sich nun je ein sample. Dies wird entsprechend dem Faktor angepasst.
        fwrite(&buffer, sizeof(int16_t), 1, output);    //&buffer=Adresse woraus gelesen werden soll,
    }


    // Close files
    fclose(input);
    fclose(output);
}
