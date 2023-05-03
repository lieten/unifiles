#ifndef FLOWDATASOURCE_H
#define FLOWDATASOURCE_H


class FlowDataSource
{
public:
    FlowDataSource(int xsize, int ysize, int zsize);
    void createData(int time);
    float getDataValue(int iz, int iy, int ix, int ic);
    void printValuesOfHorizontalSlice(int z, int c);

private:
    void gen_tornado(int xs, int ys, int zs, int time, float *tornado);
    float * cartesianDataGrid;
    int xs, ys, zs;
};

#endif // FLOWDATASOURCE_H
