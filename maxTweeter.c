#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct
{
    char *name;
    int count;
} Tweeter;

void invalid()
{
    printf("Invalid Input Format\n");
    exit(-1);
}

int getNameCol()
{
    int c;
    // Boolean, indicate if now the stram pointer is in a quotation
    int inQuotation = 0;
    int correctCharPosInQuotation = 0;
    int charPosInQuotation = 0;
    int currentCol = 0;
    int counter = 0;
    while (1)
    {
        c = getchar();
        counter++;

        if (c == EOF || c == '\n')
            // We still cannot find 'name' until the end
            invalid();

        if (currentCol == 0 && c != '"')
            //first char is not "
            invalid();

        if (c == '"')
        {
            if (!inQuotation)
            {
                currentCol++;
                inQuotation = 1;
                correctCharPosInQuotation = 0;
                charPosInQuotation = 0;
            }
            else
            {
                if (correctCharPosInQuotation == 4 && charPosInQuotation == 4)
                {
                    // find target Col
                    // read and throw away remain chars in the line
                    do
                    {
                        c = getchar();
                        if (c == EOF)
                            // We cannot find eol until the end
                            invalid();
                    } while (c != '\n');
                    return currentCol;
                }
                inQuotation = 0;
            }
            continue;
        }

        if (inQuotation)
        {
            if ((charPosInQuotation == 0 && c == 'n') ||
                (charPosInQuotation == 1 && c == 'a') ||
                (charPosInQuotation == 2 && c == 'm') ||
                (charPosInQuotation == 3 && c == 'e'))
                correctCharPosInQuotation++;
            charPosInQuotation++;
        }
    }
}

int calcTweeterTweets(int col, Tweeter **edata)
{
    int dataCount = 20000;
    Tweeter *data = malloc(sizeof(Tweeter) * dataCount);
    int count = 0;
    int countCol = col * 2 - 1;
    int currentCountCol = 0;
    int c;
    int tempNameCount = 20;
    char *tempName = malloc(sizeof(char) * tempNameCount);
    int currentNameCount = 0;
    while (1)
    {
        c = getchar();

        if (c == EOF)
            break;
        if (c == '\n')
        {
            if (currentCountCol <= countCol)
                // Quotation mark not closed or early finished line
                invalid();
            else
            {
                // Reset Values
                currentCountCol = 0;
                tempNameCount = 20;
                tempName = malloc(sizeof(char) * tempNameCount);
                currentNameCount = 0;
            }
        }
        if (currentCountCol == countCol)
        {
            if (c != '"')
            {
                if (currentNameCount + 1 >= tempNameCount)
                {
                    // space not enough, double the space
                    tempNameCount *= 2;
                    tempName = realloc(tempName, sizeof(char) * tempNameCount);
                }
                tempName[currentNameCount] = c;
                currentNameCount++;
            }
            else
            {
                // add trailing zero and resize the space
                tempName[currentNameCount] = 0;
                //tempName = realloc(tempName, sizeof(char) * currentNameCount);

                // find the target tweeter
                // since performance is not critical, linear search is used
                int targetIndex = -1;
                for (int i = 0; i < count; i++)
                {
                    if (strcmp(tempName, data[i].name) == 0)
                    {
                        targetIndex = i;
                        break;
                    }
                }

                if (targetIndex == -1)
                {
                    // Nothing is found, create a new one
                    if (count >= dataCount)
                    {
                        // space not enough, double the space
                        dataCount *= 2;
                        data = realloc(data, sizeof(Tweeter) * dataCount);
                    }
                    data[count].name = malloc(sizeof(char) * (currentNameCount + 1));
                    strcpy(data[count].name, tempName);
                    data[count].count = 0;
                    targetIndex = count;
                    count++;
                }

                data[targetIndex].count++;
                free(tempName);
            }
        }
        if (c == '"')
            currentCountCol++;
    }

    // resize the space
    data = realloc(data, sizeof(Tweeter) * count);
    *edata = data;
    return count;
}

int compare(const void *a, const void *b)
{
    Tweeter *tweeterA = (Tweeter *)a;
    Tweeter *tweeterB = (Tweeter *)b;
    return (tweeterB->count - tweeterA->count);
}

int main(int argc, char **argv)
{
    if (argc != 2)
    {
        // Invalid argument number
        invalid();
    }
    // Read file
    if (freopen(argv[1], "r", stdin) == NULL)
    {
        // Invalid File Name
        invalid();
    }

    int col = getNameCol();
    Tweeter *data = NULL;
    int count = calcTweeterTweets(col, &data);
    if (count < 10)
        // less than 10 tweeters
        invalid();
    qsort(data, count, sizeof(Tweeter), compare);
    for (int i = 0; i < 10; i++)
    {
        printf("%s: %d\n", data[i].name, data[i].count);
    }
    return 0;
}