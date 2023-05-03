#include <QCoreApplication>
#include <iostream>
#include "flowdatasource.h"
using namespace std;

int main()
{
    //cout << "Hello World!\n";
    FlowDataSource test (10, 10, 10);
    test.createData(0);
    cout << test.getDataValue(3, 3, 3, 0) << "\n";
    cout << test.getDataValue(3, 3, 3, 1) << "\n";
    cout << test.getDataValue(3, 3, 3, 2) << "\n";
    cout << "\n";
    cout << test.getDataValue(3, 3, 4, 0) << "\n";
    cout << test.getDataValue(3, 3, 4, 1) << "\n";
    cout << test.getDataValue(3, 3, 4, 2) << "\n";
}
