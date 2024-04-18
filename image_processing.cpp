#include "image_processing.h"
#include "cmath"
#include "array"

std::array<std::array<double, 3>, 3> transformationMatrix = {{{{1.0, 0.0, 0.0}}, {{0.0, 1.0, 0.0}}, {{0.0, 0.0, 1.0}}}};

void clearTransformationMatrix()
{
	transformationMatrix = {{{{1.0, 0.0, 0.0}}, {{0.0, 1.0, 0.0}}, {{0.0, 0.0, 1.0}}}};
}

constexpr std::array<std::array<double, 3>, 3> multiply3x3Matrix(const std::array<std::array<double, 3>, 3> &a,
																 const std::array<std::array<double, 3>, 3> &b)
{
	std::array<std::array<double, 3>, 3> c{};

	constexpr double threshold = 1.0E-5;
	constexpr int size = 3;

	for (int j = 0; j < size; ++j)
	{
		for (int k = 0; k < size; ++k)
		{
			for (int i = 0; i < size; ++i)
			{
				c[i][j] += a[i][k] * b[k][j];

				if (std::abs(c[i][j]) < threshold)
				{
					c[i][j] = 0.0;
				}
			}
		}
	}

	return c;
}

void translate(double dx, double dy)
{
	const std::array<std::array<double, 3>, 3> translateMatrix = {{
																	  {{1.0, 0.0, -dx}},
																	  {{0.0, 1.0, -dy}},
																	  {{0.0, 0.0, 1.0}}
																  }};

	transformationMatrix = multiply3x3Matrix(translateMatrix, transformationMatrix);
}

constexpr double deg_to_rad = M_PI / 180.0;

void rotate(double alphaDegrees)
{
	const double alphaRadians = alphaDegrees * deg_to_rad;
	const double ca = std::cos(alphaRadians);
	const double sa = std::sin(alphaRadians);

	const std::array<std::array<double, 3>, 3> rotationMatrix = {{
																	 {{ca, sa, 0.0}},
																	 {{-sa, ca, 0.0}},
																	 {{0.0, 0.0, 1.0}}
																 }};

	transformationMatrix = multiply3x3Matrix(rotationMatrix, transformationMatrix);
}

void scale(double scaleFactor, QImage &dst)
{
	dst = QImage(dst.size() / scaleFactor, dst.format());

	const std::array<std::array<double, 3>, 3> scaleMatrix = {{
																  {{scaleFactor, 0.0, 0.0}},
																  {{0.0, scaleFactor, 0.0}},
																  {{0.0, 0.0, 1.0}}
															  }};

	transformationMatrix = multiply3x3Matrix(scaleMatrix, transformationMatrix);
}

void applyTransformations(const QImage &src, QImage &dst)
{
	const int dstHeight = dst.height();
	const int dstWidth = dst.width();
	const int srcHeight = src.height();
	const int srcWidth = src.width();

	for (int y = 0; y < dstHeight; ++y)
	{
		QRgb *rgb_dst = (QRgb *) dst.scanLine(y);

		for (int x = 0; x < dstWidth; ++x)
		{
			const double xp = transformationMatrix[0][0] * x + transformationMatrix[0][1] * y + transformationMatrix[0][2];
			const double yp = transformationMatrix[1][0] * x + transformationMatrix[1][1] * y + transformationMatrix[1][2];

			const int x0 = std::floor(xp);
			const int x1 = x0 + 1;
			const int y0 = std::floor(yp);
			const int y1 = y0 + 1;

			if (x0 >= 0 && y0 >= 0 && x1 < srcWidth && y1 < srcHeight)
			{
				const double tx = xp - x0;
				const double ty = yp - y0;
				const double w00 = (1.0 - tx) * (1.0 - ty);
				const double w01 = tx * (1.0 - ty);
				const double w10 = (1.0 - tx) * ty;
				const double w11 = tx * ty;

				const QRgb p00 = *((const QRgb * const) (src.scanLine(y0)) + x0);
				const QRgb p01 = *((const QRgb * const) (src.scanLine(y0)) + x1);
				const QRgb p10 = *((const QRgb * const) (src.scanLine(y1)) + x0);
				const QRgb p11 = *((const QRgb * const) (src.scanLine(y1)) + x1);

				const int red = w00 * qRed(p00) + w01 * qRed(p01) + w10 * qRed(p10) + w11 * qRed(p11);
				const int green = w00 * qGreen(p00) + w01 * qGreen(p01) + w10 * qGreen(p10) + w11 * qGreen(p11);
				const int blue = w00 * qBlue(p00) + w01 * qBlue(p01) + w10 * qBlue(p10) + w11 * qBlue(p11);

				rgb_dst[x] = qRgb(red, green, blue);
			}
			else
			{
				rgb_dst[x] = 0;
			}
		}
	}
}
