#ifndef HORIZONTALSLICERENDERER_H
#define HORIZONTALSLICERENDERER_H
#include "horizontalslicetoimagemapper.h"

class HorizontalSliceRenderer
{
public:
    HorizontalSliceRenderer();
    void setMapper(HorizontalSliceToImageMapper* mapper);
    void drawImage();
    HorizontalSliceToImageMapper mapper;
};

#endif // HORIZONTALSLICERENDERER_H
