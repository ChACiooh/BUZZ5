#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <ctime>
#include <algorithm>
#include <utility>
#include <cmath>

#define TEST_CASE   100000
#define SQ(a)   ((a) * (a))

using namespace std;

int myBingo[7][5];
int randBingo[7][5];
int buzz[6] = {1, 30, 100, 150, 300};
int valuesM[TEST_CASE];
int valuesR[TEST_CASE];
int lottos[TEST_CASE][5];

void generateBingos()
{
    int k = 1;
    for(int i = 0; i < 5; ++i)
    {
        int visit[25] = {0};
        for(int j = 0; j < 5; ++j)
        {
            myBingo[i][j] = k++;
        }
        for(int j = 0; j < 5;)
        {
            int tmp = rand() % 24 + 1;
            if(visit[tmp] == 0)
            {
                visit[tmp] = 1;
                randBingo[i][j++] = tmp;
            }
        }
    }
    myBingo[4][4] = rand() % 20 + 1;
    for(int i = 5; i < 7; ++i)
    {
        int visit[25] = {0}, visit2[25] = {0};
        for(int j = 0; j < 5;)
        {
            int tmp = rand() % 24 + 1;
            while(visit[tmp] == 1)  tmp = rand() % 24 + 1;
            myBingo[i][j] = tmp;
            visit[tmp] = 1;
            tmp = rand() % 24 + 1;
            while(visit2[tmp] == 1) tmp = rand() % 24 + 1;
            randBingo[i][j++] = tmp;
            visit2[tmp] = 1;
        }
    }

    for(int i = 0; i < 7; ++i)
    {
        sort(myBingo[i], myBingo[i] + 5);
        sort(randBingo[i], randBingo[i] + 5);
    }
}

pair<int, int> match(const int* lotto)
{
    int resultBuzzM = 0, resultBuzzR = 0;
    for(int i = 0; i < 7; ++i)
    {
        int matchedM = 0, matchedR = 0;
        for(int j = 0; j < 5; ++j)
        {
            int now = lotto[j];
            for(int k = 0; k < 5; ++k)
            {
                if(now == myBingo[i][k])
                {
                    matchedM += 1;
                }
                if(now == randBingo[i][k])
                {
                    matchedR += 1;
                }
            }
        }
        resultBuzzM += buzz[matchedM];
        resultBuzzR += buzz[matchedR];
    }
    return make_pair(resultBuzzM, resultBuzzR);
}

int main()
{
    srand(time(NULL));
    generateBingos();

    double avgM, avgR;
    int sumM, sumR;
    FILE *ofp = fopen("output.txt", "w");
    sumM = sumR = 0;

    for(int t = 1; t <= TEST_CASE; ++t)
    {
        int lotto[5];
        int visit[25] = {0};
        for(int i = 0; i < 5;)
        {
            int tmp = rand() % 24 + 1;
            if(visit[tmp] == 0)
            {
                visit[tmp] = 1;
                lotto[i++] = tmp;
            }
        }

        sort(lotto, lotto + 5);
        for(int i = 0; i < 5; ++i)
            lottos[t-1][i] = lotto[i];
        pair<int, int> result = match(lotto);

        sumM += result.first;
        sumR += result.second;
        valuesM[t-1] = result.first;
        valuesR[t-1] = result.second;
    }
    avgM = (double)sumM / TEST_CASE;
    avgR = (double)sumR / TEST_CASE;

    double valanceM, valanceR;
    valanceM = valanceR = 0.0;
    int maxM, minM, maxR, minR;
    maxM = minM = valuesM[0];
    maxR = minR = valuesR[0];
    for(int i = 0; i < TEST_CASE; ++i)
    {
        valanceM += SQ(valuesM[i] - avgM) / (TEST_CASE-1);
        valanceR += SQ(valuesR[i] - avgR) / (TEST_CASE-1);
        if(maxM < valuesM[i])   maxM = valuesM[i];
        if(minM > valuesM[i])   minM = valuesM[i];
        if(maxR < valuesR[i])   maxR = valuesR[i];
        if(minR > valuesR[i])   minR = valuesR[i];
    }

    double sigmaM, sigmaR;
    sigmaM = sqrt(valanceM);
    sigmaR = sqrt(valanceR);

    fprintf(ofp, "avg: (M)%.3lf (R)%.3lf\n", avgM, avgR);
    fprintf(ofp, "¥ò: (M)%.3lf (R)%.3lf\n", sigmaM, sigmaR);
    fprintf(ofp, "M/m: (M)%3d/%3d (R)%3d/%3d\n", maxM, minM, maxR, minR);
    for(int t = 1; t <= TEST_CASE; ++t)
    {
        fprintf(ofp, "Case #%6d: (M)%3d (R)%3d ", t, valuesM[t-1], valuesR[t-1]);
        fprintf(ofp, "(L) ");
        for(int i = 0; i < 5; ++i)
            fprintf(ofp, "%2d ", lottos[t-1][i]);
        fprintf(ofp, "\n");
    }
    fclose(ofp);
    return 0;
}
