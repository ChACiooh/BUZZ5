#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <ctime>
#include <algorithm>
#include <utility>
#include <cmath>

#define TEST_CASE   100000
#define HAV_LOTTERYS  7
#define NUM_OF_EACH_LOTTERY   5
#define KIND_WINS   6
#define NUM_BNDRY   24
#define SQ(a)   ((a) * (a))

using namespace std;

int myBingo[HAV_LOTTERYS][NUM_OF_EACH_LOTTERY];
int randBingo[HAV_LOTTERYS][NUM_OF_EACH_LOTTERY];
int buzz[KIND_WINS] = {1, 5, 30, 100, 150, 300};
int valuesM[TEST_CASE];
int valuesR[TEST_CASE];
int lottos[TEST_CASE][NUM_OF_EACH_LOTTERY];

void generateBingos()
{
    int k = 1;
    int passive = NUM_BNDRY / NUM_OF_EACH_LOTTERY + (NUM_BNDRY % NUM_OF_EACH_LOTTERY > 0 ? 1 : 0);
    for(int i = 0; i < passive; ++i)
    {
        int visit[NUM_BNDRY + 1] = {0};
        for(int j = 0; j < NUM_OF_EACH_LOTTERY; ++j)
        {
            myBingo[i][j] = k++;
        }
        for(int j = 0; j < NUM_OF_EACH_LOTTERY;)
        {
            int tmp = rand() % NUM_BNDRY + 1;
            if(visit[tmp] == 0)
            {
                visit[tmp] = 1;
                randBingo[i][j++] = tmp;
            }
        }
    }

    int latestIndex = NUM_BNDRY / NUM_OF_EACH_LOTTERY;
    int counted = NUM_BNDRY - (NUM_BNDRY % NUM_OF_EACH_LOTTERY);
    for(int i = 1; i <= NUM_BNDRY % NUM_OF_EACH_LOTTERY; ++i)
    {
        myBingo[latestIndex][NUM_OF_EACH_LOTTERY] = rand() % counted + 1;
    }
    for(int i = latestIndex + 1; i < HAV_LOTTERYS; ++i)
    {
        int visit[NUM_BNDRY + 1] = {0}, visit2[NUM_BNDRY + 1] = {0};
        for(int j = 0; j < NUM_OF_EACH_LOTTERY;)
        {
            int tmp = rand() % NUM_BNDRY + 1;
            while(visit[tmp] == 1)  tmp = rand() % NUM_BNDRY + 1;
            myBingo[i][j] = tmp;
            visit[tmp] = 1;
            tmp = rand() % NUM_BNDRY + 1;
            while(visit2[tmp] == 1) tmp = rand() % NUM_BNDRY + 1;
            randBingo[i][j++] = tmp;
            visit2[tmp] = 1;
        }
    }

    for(int i = 0; i < HAV_LOTTERYS; ++i)
    {
        sort(myBingo[i], myBingo[i] + NUM_OF_EACH_LOTTERY);
        sort(randBingo[i], randBingo[i] + NUM_OF_EACH_LOTTERY);
    }
}

bool bs(int i, int f, const int* _list_, const int& val)
{
    int mid = (i + f) / 2;
    if(mid == f)    return false;
    if(_list_[mid] < val)   return bs(mid+1, f, _list_, val);
    else if(_list_[mid] > val)  return bs(i, mid, _list_, val);
    return true;
}

pair<int, int> match(const int* lotto)
{
    int resultBuzzM = 0, resultBuzzR = 0;
    for(int i = 0; i < HAV_LOTTERYS; ++i)
    {
        int matchedM = 0, matchedR = 0;
        for(int j = 0; j < NUM_OF_EACH_LOTTERY; ++j)
        {
            int now = lotto[j];
            if(bs(0, NUM_OF_EACH_LOTTERY, myBingo[i], now))  matchedM += 1;
            if(bs(0, NUM_OF_EACH_LOTTERY, randBingo[i], now))    matchedR += 1;
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
        int lotto[NUM_OF_EACH_LOTTERY];
        int visit[NUM_BNDRY + 1] = {0};
        for(int i = 0; i < NUM_OF_EACH_LOTTERY;)
        {
            int tmp = rand() % NUM_BNDRY + 1;
            if(visit[tmp] == 0)
            {
                visit[tmp] = 1;
                lotto[i++] = tmp;
            }
        }

        sort(lotto, lotto + NUM_OF_EACH_LOTTERY);
        for(int i = 0; i < NUM_OF_EACH_LOTTERY; ++i)
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
    fprintf(ofp, "¥ò : (M)%.3lf (R)%.3lf\n", sigmaM, sigmaR);
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
