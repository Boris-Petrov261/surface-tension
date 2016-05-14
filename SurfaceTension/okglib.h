#ifndef OKGLIB_H_INCLUDED
#define OKGLIB_H_INCLUDED

struct OKG_POINT
{
    float x;
    float y;
    float z;
} ;


bool running();
void init();
void finit();
float random(float a, float b);
float irandom(int a, int b);
void drawOxyz();


#endif // OKGLIB_H_INCLUDED
