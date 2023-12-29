#include "helpers.h"
#include <math.h>

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            //calculate average of r, g and b values
            RGBTRIPLE value = image[i][j];
            // float average = (value.rgbtBlue + value.rgbtGreen + value.rgbtRed) / 3.0;
            // int rd_average = round(average);
            int rd_average = round((value.rgbtBlue + value.rgbtGreen + value.rgbtRed) / 3.0);

            //set r, g and b values of image[height][width] to that average value
            RGBTRIPLE new_value;
            new_value.rgbtBlue = rd_average;
            new_value.rgbtGreen = rd_average;
            new_value.rgbtRed = rd_average;

            image[i][j] = new_value;
        }
    }

    return;
}

// Convert image to sepia
void sepia(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            RGBTRIPLE value = image[i][j];

            //new values according to given formulae
            BYTE sepiaRed = fmin(round(0.393 * value.rgbtRed + 0.769 * value.rgbtGreen + 0.189 * value.rgbtBlue), 255);
            BYTE sepiaGreen = fmin(round(0.349 * value.rgbtRed + 0.686 * value.rgbtGreen + 0.168 * value.rgbtBlue), 255);
            BYTE sepiaBlue = fmin(round(0.272 * value.rgbtRed + 0.534 * value.rgbtGreen + 0.131 * value.rgbtBlue), 255);

            RGBTRIPLE new_value;
            new_value.rgbtBlue = sepiaBlue;
            new_value.rgbtGreen = sepiaGreen;
            new_value.rgbtRed = sepiaRed;

            image[i][j] = new_value;
        }
    }

    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width / 2; j++)
        {
            RGBTRIPLE tmp = image[i][j];
            image[i][j] = image[i][width - 1 - j];//-1*(j-(width-1))
            image[i][width - 1 - j] = tmp;
        }
    }

    return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE copy[height][width];
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            copy[i][j] = image[i][j];
        }
    }

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            if (i == 0) //erste Reihe
            {
                if (j == 0) //... und erste Spalte
                {
                    image[i][j].rgbtBlue = round((copy[i][j].rgbtBlue + copy[i][j+1].rgbtBlue + copy[i+1][j].rgbtBlue + copy[i+1][j+1].rgbtBlue) / 4.0);
                    image[i][j].rgbtGreen = round((copy[i][j].rgbtGreen + copy[i][j+1].rgbtGreen + copy[i+1][j].rgbtGreen + copy[i+1][j+1].rgbtGreen) / 4.0);
                    image[i][j].rgbtRed = round((copy[i][j].rgbtRed + copy[i][j+1].rgbtRed + copy[i+1][j].rgbtRed + copy[i+1][j+1].rgbtRed) / 4.0);
                }

                else if (j == (width - 1)) //... und letzte Spalte
                {
                    image[i][j].rgbtBlue = round((copy[i][j].rgbtBlue + copy[i][j-1].rgbtBlue + copy[i+1][j].rgbtBlue + copy[i+1][j-1].rgbtBlue) / 4.0);
                    image[i][j].rgbtGreen = round((copy[i][j].rgbtGreen + copy[i][j-1].rgbtGreen + copy[i+1][j].rgbtGreen + copy[i+1][j-1].rgbtGreen) / 4.0);
                    image[i][j].rgbtRed = round((copy[i][j].rgbtRed + copy[i][j-1].rgbtRed + copy[i+1][j].rgbtRed + copy[i+1][j-1].rgbtRed) / 4.0);
                }

                //Rest in erster Reihe:
                else
                {
                    image[i][j].rgbtBlue = round((copy[i][j-1].rgbtBlue + copy[i][j].rgbtBlue + copy[i][j+1].rgbtBlue + copy[i+1][j-1].rgbtBlue + copy[i+1][j].rgbtBlue + copy[i+1][j+1].rgbtBlue) / 6.0);
                    image[i][j].rgbtGreen = round((copy[i][j-1].rgbtGreen + copy[i][j].rgbtGreen + copy[i][j+1].rgbtGreen + copy[i+1][j-1].rgbtGreen + copy[i+1][j].rgbtGreen + copy[i+1][j+1].rgbtGreen) / 6.0);
                    image[i][j].rgbtRed = round((copy[i][j-1].rgbtRed + copy[i][j].rgbtRed + copy[i][j+1].rgbtRed + copy[i+1][j-1].rgbtRed + copy[i+1][j].rgbtRed + copy[i+1][j+1].rgbtRed) / 6.0);
                }
            }

            else if (i == (height - 1)) //letzte Reihe
            {
                if (j == 0) //... und erste Spalte
                {
                    image[i][j].rgbtBlue = round((copy[i][j].rgbtBlue + copy[i][j+1].rgbtBlue + copy[i-1][j].rgbtBlue + copy[i-1][j+1].rgbtBlue) / 4.0);
                    image[i][j].rgbtGreen = round((copy[i][j].rgbtGreen + copy[i][j+1].rgbtGreen + copy[i-1][j].rgbtGreen + copy[i-1][j+1].rgbtGreen) / 4.0);
                    image[i][j].rgbtRed = round((copy[i][j].rgbtRed + copy[i][j+1].rgbtRed + copy[i-1][j].rgbtRed + copy[i-1][j+1].rgbtRed) / 4.0);
                }

                else if (j == (width - 1)) //... und letzte Spalte
                {
                    image[i][j].rgbtBlue = round((copy[i][j].rgbtBlue + copy[i][j-1].rgbtBlue + copy[i-1][j].rgbtBlue + copy[i-1][j-1].rgbtBlue) / 4.0);
                    image[i][j].rgbtGreen = round((copy[i][j].rgbtGreen + copy[i][j-1].rgbtGreen + copy[i-1][j].rgbtGreen + copy[i-1][j-1].rgbtGreen) / 4.0);
                    image[i][j].rgbtRed = round((copy[i][j].rgbtRed + copy[i][j-1].rgbtRed + copy[i-1][j].rgbtRed + copy[i-1][j-1].rgbtRed) / 4.0);
                }

                //Rest in letzter Reihe:
                else
                {
                    image[i][j].rgbtBlue = round((copy[i][j-1].rgbtBlue + copy[i][j].rgbtBlue + copy[i][j+1].rgbtBlue + copy[i-1][j-1].rgbtBlue + copy[i-1][j].rgbtBlue + copy[i-1][j+1].rgbtBlue) / 6.0);
                    image[i][j].rgbtGreen = round((copy[i][j-1].rgbtGreen + copy[i][j].rgbtGreen + copy[i][j+1].rgbtGreen + copy[i-1][j-1].rgbtGreen + copy[i-1][j].rgbtGreen + copy[i-1][j+1].rgbtGreen) / 6.0);
                    image[i][j].rgbtRed = round((copy[i][j-1].rgbtRed + copy[i][j].rgbtRed + copy[i][j+1].rgbtRed + copy[i-1][j-1].rgbtRed + copy[i-1][j].rgbtRed + copy[i-1][j+1].rgbtRed) / 6.0);
                }
            }

            else if (j == 0 && (i != 0 && i != (height - 1))) //erste Spalte ohne Ecken
            {
                image[i][j].rgbtBlue = round((copy[i-1][j].rgbtBlue + copy[i-1][j+1].rgbtBlue + copy[i][j].rgbtBlue + copy[i][j+1].rgbtBlue + copy[i+1][j].rgbtBlue + copy[i+1][j+1].rgbtBlue) / 6.0);
                image[i][j].rgbtGreen = round((copy[i-1][j].rgbtGreen + copy[i-1][j+1].rgbtGreen + copy[i][j].rgbtGreen + copy[i][j+1].rgbtGreen + copy[i+1][j].rgbtGreen + copy[i+1][j+1].rgbtGreen) / 6.0);
                image[i][j].rgbtRed = round((copy[i-1][j].rgbtRed + copy[i-1][j+1].rgbtRed + copy[i][j].rgbtRed + copy[i][j+1].rgbtRed + copy[i+1][j].rgbtRed + copy[i+1][j+1].rgbtRed) / 6.0);
            }

            else if (j == (width - 1) && (i != 0 && i != (height - 1))) //letzte Spalte ohne Ecken
            {
                image[i][j].rgbtBlue = round((copy[i-1][j].rgbtBlue + copy[i-1][j-1].rgbtBlue + copy[i][j-1].rgbtBlue + copy[i][j].rgbtBlue + copy[i+1][j].rgbtBlue + copy[i+1][j-1].rgbtBlue) / 6.0);
                image[i][j].rgbtGreen = round((copy[i-1][j].rgbtGreen + copy[i-1][j-1].rgbtGreen + copy[i][j-1].rgbtGreen + copy[i][j].rgbtGreen + copy[i+1][j].rgbtGreen + copy[i+1][j-1].rgbtGreen) / 6.0);
                image[i][j].rgbtRed = round((copy[i-1][j].rgbtRed + copy[i-1][j-1].rgbtRed + copy[i][j-1].rgbtRed + copy[i][j].rgbtRed + copy[i+1][j].rgbtRed + copy[i+1][j-1].rgbtRed) / 6.0);
            }

            //alle anderen, keine corner cases
            else
            {
                image[i][j].rgbtBlue = round((copy[i-1][j-1].rgbtBlue + copy[i-1][j].rgbtBlue + copy[i-1][j+1].rgbtBlue + copy[i][j-1].rgbtBlue + copy[i][j].rgbtBlue + copy[i][j+1].rgbtBlue + copy[i+1][j-1].rgbtBlue + copy[i+1][j].rgbtBlue + copy[i+1][j+1].rgbtBlue) / 9.0);
                image[i][j].rgbtGreen = round((copy[i-1][j-1].rgbtGreen + copy[i-1][j].rgbtGreen + copy[i-1][j+1].rgbtGreen + copy[i][j-1].rgbtGreen + copy[i][j].rgbtGreen + copy[i][j+1].rgbtGreen + copy[i+1][j-1].rgbtGreen + copy[i+1][j].rgbtGreen + copy[i+1][j+1].rgbtGreen) / 9.0);
                image[i][j].rgbtRed = round((copy[i-1][j-1].rgbtRed + copy[i-1][j].rgbtRed + copy[i-1][j+1].rgbtRed + copy[i][j-1].rgbtRed + copy[i][j].rgbtRed + copy[i][j+1].rgbtRed + copy[i+1][j-1].rgbtRed + copy[i+1][j].rgbtRed + copy[i+1][j+1].rgbtRed) / 9.0);
            }
        }
    }

    return;
}
