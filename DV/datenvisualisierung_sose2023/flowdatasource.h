#ifndef FLOWDATASOURCE_H
#define FLOWDATASOURCE_H


class FlowDataSource
{
public:
    FlowDataSource(int xsize, int ysize, int zsize);
    FlowDataSource();
    void createData(int time);
    float getDataValue(int iz, int iy, int ix, int ic);
    void printValuesOfHorizontalSlice(int z, int c);
    int * getDimensions();
    int getXDimension();
    int getYDimension();
    int getZDimension();

private:
    float * cartesianDataGrid;
    int xs, ys, zs;
};

#endif // FLOWDATASOURCE_H
