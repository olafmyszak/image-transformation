#ifndef IMAGE_PROCESSING_H
#define IMAGE_PROCESSING_H

#include <QImage>

void translate(double dx, double dy);

void rotate(double alphaDegrees);

void scale(double scaleFactor, QImage &dst);

void applyTransformations(const QImage &src, QImage &dst);

void clearTransformationMatrix();

#endif // IMAGE_PROCESSING_H
