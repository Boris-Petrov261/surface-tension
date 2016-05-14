#include "edgeDetectionRotatedDrop.h"
#include <fstream>
using namespace std;
vector<Point> readRotatedDrop(string FILENAME_INPUT)
{
    vector <Point> coords;
    unsigned char red , green , blue;
    string EXP_DIRECTORY = "res\\pics\\EXP_PROFILES\\";
    string DER_DIRECTORY =  "res\\pics\\DER_PROFILES\\";
    bitmap_image image(EXP_DIRECTORY + FILENAME_INPUT);
    //horizontal and vertical extrema
    int xBeg , xEnd, yBeg , yEnd;
    //find horizontal and vertical extrema
    xBeg = findXBegR(image);
    xEnd = findXEndR(image);
    yBeg = findYBegR(image);
    yEnd = findYEndR(image);
    //find coordinates
    for(int w = xBeg ; w <= xEnd ; w ++)
    {
        for(int h = yBeg ; h <= ((yEnd + yBeg)/2) ; h ++)
        {
            image.get_pixel(w , h , red , green , blue);
            if((int)red <= 100 && (int)green <= 100 && (int)blue <= 100)
            {
                image.set_pixel(w ,h , 255 , 0 , 0);
                Point tmp(w , h);//creates a temporary point to be inserted in the vector
                coords.push_back(tmp);//temporary point is pushed in the back of the vector
                break;
            }
        }
    }
    image.save_image(DER_DIRECTORY + FILENAME_INPUT);
    //resize the coordinates
    for(auto& it : coords)
    {
        it.x = (it.x - xBeg)/ ((yEnd - yBeg)/2)*(1.0);
        it.y = (abs( (yEnd + yBeg)/2 - it.y))/((yEnd - yBeg)/2)*(1.0);
    }
    //flip the graphic
    for(auto& it : coords)
        swap(it.x , it.y);
    coords.insert(coords.begin() , Point(0,0));
    return coords;
}
int findYBegR(bitmap_image image)
{
    unsigned char red , green , blue;
    for(int h = 1 ; h < image.height() ; h ++)
        for(int w = 1 ; w < image.width() ; w ++)
        {
            image.get_pixel(w , h , red , green , blue);
            if((int)red <= 100 && (int)green <= 100 && (int)blue <= 100)
                return h;
        }
    return -1;
}
int findYEndR(bitmap_image image)
{
    unsigned char red , green , blue;
    for(int h = image.height() - 1 ; h > 0 ; h --)
        for(int w = 1 ; w < image.width() ; w ++)
        {
            image.get_pixel(w , h , red , green , blue);
            if((int)red <= 100 && (int)green <= 100 && (int)blue <= 100)
                return h;
        }
    return -1;
}
int findXBegR(bitmap_image image)
{
    unsigned char red , green , blue;
    for(int w = 1 ; w < image.width() ; w ++)
        for(int h = 1 ; h < image.height() ; h ++)
        {
            image.get_pixel(w , h , red , green , blue);
            if((int)red <= 100 && (int)green <= 100 && (int)blue <= 100)
                return w;
        }
    return -1;
}
int findXEndR(bitmap_image image)
{
    unsigned char red , green , blue;
    for(int w = image.width() - 1 ; w > 0 ; w --)
        for(int h = 1 ; h < image.height() ; h ++)
        {
            image.get_pixel(w , h , red , green , blue);
            if((int)red <= 100 && (int)green <= 100 && (int)blue <= 100)
                return w;
        }
    return -1;
}

