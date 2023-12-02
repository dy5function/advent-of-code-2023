/**
 * @brief Solution to Advent of Code Day 1: Trebuchet?!
 *
 * @details The task for day 1 can be found here:
 * <https://adventofcode.com/2023/day/1>
 *
 * @copyright 2023 Christoph Groß
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <https://www.gnu.org/licenses/>.
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <errno.h>
#include <string.h>

/**
 * @brief Print usage message to stdout
 *
 * @param program Name of the program to use in the usage message.
*/
void usage(const char* program)
{
    printf("Usage:\n%s <input-path>\n\n", program);
}

/**
 * @brief Main function
 * @details This will read the provided input file line by line and extract the
 * first and last digit of each line, append them to a two-digit number and add
 * those numbers extracted from each line up to get the final result.
 *
 * @param argc Number of commandline arguments provided. Should be 2,
 * otherwhise the programm will abort.
 * @param argv Array of commandline arguments provided as char arrays. This
 * program expects argv[1] to be a path to the input file to read.
 * @returns 0 if program finished successfully and 1 on error.
*/
int main(int argc, char** argv)
{
    FILE* fd;
    char* line = NULL;
    size_t len = 0;
    ssize_t read = 0;

    // Check input parameters
    if (argc != 2)
    {
        fprintf(stderr, "This program expects a commandline argument!\n");
        usage(argv[0]);
        exit(1);
    }

    // Open input file
    fd = fopen(argv[1], "r");
    if (fd == NULL)
    {
        fprintf(stderr, "Error opening file %s: %s\n", argv[1], strerror(errno));
        exit(1);
    }

    uint8_t digits[2] = {};
    uint8_t got_digit;
    uint32_t sum = 0;

    // Read line by line
    while ((read = getline(&line, &len, fd)) != -1)
    {
        got_digit = 0;
        for (uint8_t i = 0; i < read; i++)
        {
            // We have a number
            if (line[i] >= '0' && line[i] <= '9')
            {
                // Is this the first time we got one?
                if (!got_digit)
                {
                    digits[0] = line[i] - '0';
                    got_digit = 1;
                }
                // Always update second digit to take care of the case where
                // there is only one number in the line
                digits[1] = line[i] - '0';
            }
        }

#ifdef DEBUG
        printf("%d%d <- [%ld]: %s", digits[0], digits[1], read, line);
#endif

        // Combine digits into number and add up
        sum += 10 * digits[0] + digits[1];
    }

    printf("Sum of calibration values: %d\n", sum);

    fclose(fd);
    if (line)
    {
        free(line);
    }

    return 0;
}
