#include "flowdatasource.h"
#include <math.h>
#include <iostream>
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

int * FlowDataSource::getDimensions() {
    int dimArray[3];
    dimArray[0] = xs;
    dimArray[1] = ys;
    dimArray[2] = zs;
    return dimArray;
}

/*
 * xs, ys, zs: groesse des Gatters
 * time:Zeitpunkt im Tornado
 * tornado: referenz auf vektor
 */
void FlowDataSource::gen_tornado(int xs, int ys, int zs, int time, float *tornado)
{
    /*
     *  Gen_Tornado creates a vector field of dimension [xs,ys,zs,3] from
     *  a proceedural function. By passing in different time arguements,
     *  a slightly different and rotating field is created.
     *
     *  The magnitude of the vector field is highest at some funnel shape
     *  and values range from 0.0 to around 0.4 (I think).
     *
     *  I just wrote these comments, 8 years after I wrote the function.
     *
     * Developed by Roger A. Crawfis, The Ohio State University
     *
     */
    {
      float x, y, z;
      int ix, iy, iz;
      float r, xc, yc, scale, temp, z0;
      float r2 = 8;
      float SMALL = 0.00000000001;
      float xdelta = 1.0 / (xs-1.0);
      float ydelta = 1.0 / (ys-1.0);
      float zdelta = 1.0 / (zs-1.0);

      for( iz = 0; iz < zs; iz++ )
      {
         z = iz * zdelta;                        // map z to 0->1
         xc = 0.5 + 0.1*sin(0.04*time+10.0*z);   // For each z-slice, determine the spiral circle.
         yc = 0.5 + 0.1*cos(0.03*time+3.0*z);    //    (xc,yc) determine the center of the circle.
         r = 0.1 + 0.4 * z*z + 0.1 * z * sin(8.0*z); //  The radius also changes at each z-slice.
         r2 = 0.2 + 0.1*z;                           //    r is the center radius, r2 is for damping
         for( iy = 0; iy < ys; iy++ )
         {
            y = iy * ydelta;
            for( ix = 0; ix < xs; ix++ )
            {
                x = ix * xdelta;
                temp = sqrt( (y-yc)*(y-yc) + (x-xc)*(x-xc) );
                scale = fabs( r - temp );
    /*
     *  I do not like this next line. It produces a discontinuity
     *  in the magnitude. Fix it later.
     *
     */
               if ( scale > r2 )
                  scale = 0.8 - scale;
               else
                  scale = 1.0;
                z0 = 0.1 * (0.1 - temp*z );
               if ( z0 < 0.0 )  z0 = 0.0;
               temp = sqrt( temp*temp + z0*z0 );
                scale = (r + r2 - temp) * scale / (temp + SMALL);
                scale = scale / (1+z);
               //*tornado++ = ... heisst effektiv: der naechste Eintrag im Array wird ueberschrieben
               *tornado++ = scale * (y-yc) + 0.1*(x-xc); //ic=0  x-Komponente des Vektors
               *tornado++ = scale * -(x-xc) + 0.1*(y-yc); //ic=1  y-Komponente des Vektors
               *tornado++ = scale * z0; //ic=2  x-Komponente des Vektors
            }
         }
      }
    }
}

