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

const char digit_strings[10][6] = {
    "zero",
    "one",
    "two",
    "three",
    "four",
    "five",
    "six",
    "seven",
    "eight",
    "nine",
};

/**
 * @brief Print usage message to stdout
 *
 * @param program Name of the program to use in the usage message.
 */
void usage(const char *program)
{
    printf("Usage:\n%s <input-path>\n\n", program);
}

/**
 * @brief Check if the input represents a number
 * @details Take the input character and check if this represents a number.
 * This function assumes that it is called repeatedly on individual characters
 * of a string and detects both actual numerals 0-9 and spelled out digits
 * "zero" to "nine". It does so by remembering the inputs given in the past
 * and maintaining matching indices for the digit strings.
 *
 * @post Function internal matching indices for digit strings are stored and
 * will impact results for the next time this function is called. When a null
 * byte is passed, all matching indices are reset.
 *
 * @param character Character to check if it represents a number.
 * @param number Pointer to store a number in if one is identified.
 * @returns 0 if no number is identified in the input, 1 if a number is found.
 */
int is_digit(const char character, uint8_t *number)
{
    // Matching indices for digit strings. Whenever we match a character of a
    // digit string, we increment the repsective index so that we check the
    // next character when we are called the next time. Whenever the current
    // character of a digit string is not matched by the input, we reset that
    // index.
    static int matching_index[10] = {0};
    int found_number = 0;
    int reset = 0;

    if (character == '\0')
    {
        reset = 1;
    }
    else if (character >= '0' && character <= '9')
    {
        *number = character - '0';
        found_number = 1;
        // A numeral cannot be part of a digit string. Resetz all matching
        // indices to restart matching from the beginning on the next call.
        reset = 1;
    }
    else
    {
        // Check each digit word if we match the current character
        for (uint8_t i = 0; i < 10; i++)
        {
            // Retry loop to take care of the case that our current index does
            // not match but the beginning of the digit word does!
            for (uint8_t j = 0; j < 2; j++)
            {
                // Match -> Next time, we need to match the next character of
                // that word
                if (character == digit_strings[i][matching_index[i]])
                {
                    ++matching_index[i];
                    // Did we match to full word? If yes, we found our next
                    // number!
                    if (digit_strings[i][matching_index[i]] == '\0')
                    {
                        *number = i;
                        found_number = 1;
                        matching_index[i] = 0;
                    }
                    else
                    {
                        // No retry if we incremented the matching index and
                        // did not reach the end of a word. We do not want to
                        // double-count!
                        break;
                    }
                }
                // No match -> Start from the beginning of this word
                else
                {
                    matching_index[i] = 0;
                }
            }
        }
    }

    // Reset all indices so we start matching from the beginning next time we are called
    if (reset)
    {
        for (uint8_t i = 0; i < 10; i++)
        {
            matching_index[i] = 0;
        }
    }

    if (found_number)
        return 1;

    return 0;
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
int main(int argc, char **argv)
{
    FILE *fd;
    char *line = NULL;
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

    uint8_t number;
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
            if (is_digit(line[i], &number))
            {
                // Is this the first time we got one?
                if (!got_digit)
                {
                    digits[0] = number;
                    got_digit = 1;
                }
                // Always update second digit to take care of the case where
                // there is only one number in the line
                digits[1] = number;
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
