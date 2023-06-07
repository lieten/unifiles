#include "flowdatasource.h"
#include <math.h>
#include <iostream>
extern "C" {
    #include "tornadoSrc.h"
}

using namespace std;

FlowDataSource::FlowDataSource(int xsize, int ysize, int zsize)
{
    xs = xsize;
    ys = ysize;
    zs = zsize;
    int size = xs * ys * zs * 3;
    cartesianDataGrid = new (nothrow) float[size];
    createData(0);
}

FlowDataSource::FlowDataSource() {};

void FlowDataSource::createData(int time)
{
    gen_tornado(xs, ys, zs, time, cartesianDataGrid);
}

float FlowDataSource::getDataValue(int iz, int iy, int ix, int ic)
{
    return cartesianDataGrid[3*xs*ys*iz + 3*xs*iy + 3*ix + ic]; //springe zum richtigen z-block, dann y-block, dann x-block, dann Vektorkomponente
}

/* Beispeilarray: 2*2*2*3
 * [ ((((x,y,z), (x,y,z)), ((x,y,z), (x,y,z))), (((x,y,z), (x,y,z)), ((x,y,z), (x,y,z)))), ((((x,y,z), (x,y,z)), ((x,y,z), (x,y,z))), (((x,y,z), (x,y,z)), ((x,y,z), (x,y,z))))
 * x           1                   2                     1                   2                       1                   2                     1                   2
 * y                     1                                         2                                           1                                         2
 * z                                          1                                                                                     2
 */
void FlowDataSource::printValuesOfHorizontalSlice(int z, int c)
{
    for(int y = 0; y < ys; y++) {
        for(int x = 0; x < xs; x++) {
            cout << getDataValue(z, y, x, c) << " ";
        }
    }
}

int FlowDataSource::getXDimension() {
    return xs;
}

int FlowDataSource::getYDimension() {
    return ys;
}

int FlowDataSource::getZDimension() {
    return zs;
}
