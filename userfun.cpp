//
// Created by chris on 11/5/17.
//

#include <cmath>
#include <cstdlib>

int isqrt(unsigned x)
{
    unsigned r, nr, m;

    r= 0;
    m= 0x40000000;

    do
    {
        nr= r + m;
        if (nr<=x)
        {
            x-= nr;
            r= nr + m;
        }
        r>>= 1;
        m>>= 2;
    }
    while (m!=0);

    if (x>r) r+= 1;
    return r;
}

constexpr int ROUGHATAN_FIXED = 1 << 16;
constexpr float firstdiv = (((double)0x33333333) / (double)ROUGHATAN_FIXED);
static inline float roughAtan(float x) {
    //no fuckin' clue

    x *= ROUGHATAN_FIXED;

    float y = x / 2.0f;
    double z = y;
    y = ((int *) &z)[1];
    float result = (y / firstdiv) / (float)ROUGHATAN_FIXED;

    if(fabsf(x) < ROUGHATAN_FIXED) {
        result -= (x < 0 ? -1 : 1);
    }
    return result;
}

static inline int coercei(float f) {
    return *(int*)&f;
}

constexpr unsigned findval = 0;

int userfun(int x, int y) {
    //char bytes[4];
    //*(int*)&bytes = x;

    if(!y || (y == -1 && x == (int) 0x80000000))
        return 0;
    return x % y == 0;//(unsigned)x / (unsigned)0x7060504;//(bytes[0] == findval) | (bytes[1] == findval) | (bytes[2] == findval) | (bytes[3] == findval);
}
