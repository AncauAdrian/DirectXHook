#pragma once


/*
https://stackoverflow.com/questions/217578/how-can-i-determine-whether-a-2d-point-is-within-a-polygon/2922778#2922778

Arguments
nvert: Number of vertices in the polygon. Whether to repeat the first vertex at the end has been discussed in the article referred above.
vertx, verty: Arrays containing the x- and y-coordinates of the polygon's vertices.
testx, testy: X- and y-coordinate of the test point.
It's both short and efficient and works both for convex and concave polygons. As suggested before, you should check the bounding rectangle first and treat polygon holes separately.

The idea behind this is pretty simple. The author describes it as follows:

I run a semi-infinite ray horizontally (increasing x, fixed y) out from the test point, and count how many edges it crosses. At each crossing, the ray switches between inside and outside. This is called the Jordan curve theorem.

The variable c is switching from 0 to 1 and 1 to 0 each time the horizontal ray crosses any edge. So basically it's keeping track of whether the number of edges crossed are even or odd. 0 means even and 1 means odd.
*/
int pnpoly(int nvert, float* vertx, float* verty, float testx, float testy)
{
    int i, j, c = 0;
    for (i = 0, j = nvert - 1; i < nvert; j = i++) {
        if (((verty[i] > testy) != (verty[j] > testy)) &&
            (testx < (vertx[j] - vertx[i]) * (testy - verty[i]) / (verty[j] - verty[i]) + vertx[i]))
            c = !c;
    }
    return c;
}