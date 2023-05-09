#include "horizontalslicerenderer.h"
#include "horizontalslicetoimagemapper.h"

HorizontalSliceRenderer::HorizontalSliceRenderer()
{

}

void HorizontalSliceRenderer::setMapper(HorizontalSliceToImageMapper* mpr)
{
    mapper = *mpr;
}

void HorizontalSliceRenderer::drawImage() {
    QImage image = mapper.mapSliceToImage();
}
