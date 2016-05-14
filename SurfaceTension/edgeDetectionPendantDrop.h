#ifndef EDGEDETECTIONPENDANTDROP_H_
#define EDGEDETECTIONPENDANTDROP_H_
#include<string>
#include<iostream>
#include<vector>
#include "st_library.h"
vector<Point> readPendantDrop(string);
int findXBegP(bitmap_image);
int findXEndP(bitmap_image);
int findYEndP(bitmap_image);
void reverseVector(vector<Point>&);
#endif // EDGEDETECTIONPENDANTDROP_H_
