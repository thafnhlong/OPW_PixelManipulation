#include "Converter.h"

int Converter::RGB2GrayScale(const Mat& sourceImage, Mat& destinationImage)
{
	if (sourceImage.empty()) {
		return 0;
	}

	// lay hang, cot cua anh goc
	int rows = sourceImage.rows;
	int cols = sourceImage.cols;

	// lay chieu dai cua 1 dong anh goc
	auto rowStepSrc = sourceImage.step[0];

	// lay chieu rong cua 1 pixel anh goc
	auto colStepSrc = sourceImage.step[1];

	// lay con tro data sourceImage
	uchar* pDataSrc = (uchar*)sourceImage.data;

	// tao anh xam
	destinationImage = Mat(rows, cols, CV_8UC1);

	// lay chieu dai cua 1 dong anh xam
	auto rowStepDest = destinationImage.step[0];

	// lay chieu rong cua 1 pixel anh xam
	auto colStepDest = destinationImage.step[1];

	// lay con tro data destinationImage
	uchar* pDataDest = (uchar*)destinationImage.data;

	for (int y = 0; y < rows; y++, pDataSrc += rowStepSrc, pDataDest += rowStepDest) {
		uchar* pRowSrc = pDataSrc;
		uchar* pRowDest = pDataDest;
		for (int x = 0; x < cols; x++, pRowSrc += colStepSrc, pRowDest += colStepDest) {
			uchar R = pRowSrc[0];
			uchar G = pRowSrc[1];
			uchar B = pRowSrc[2];

			// cong thuc :https://docs.opencv.org/2.4/modules/imgproc/doc/miscellaneous_transformations.html#void%20cvtColor%28InputArray%20src,%20OutputArray%20dst,%20int%20code,%20int%20dstCn%29

			uchar pixel = 0.299f * R + 0.587f * G + 0.114f * B;
			*pRowDest = pixel;
		}
	}
	return 1;
}

int Converter::RGB2HSV(const Mat& sourceImage, Mat& destinationImage)
{
	if (sourceImage.empty()) {
		return 0;
	}

	// lay hang, cot cua anh goc
	int rows = sourceImage.rows;
	int cols = sourceImage.cols;

	// lay chieu dai cua 1 dong anh goc
	auto rowStepSrc = sourceImage.step[0];

	// lay chieu rong cua 1 pixel anh goc
	auto colStepSrc = sourceImage.step[1];

	// lay con tro data sourceImage
	uchar* pDataSrc = (uchar*)sourceImage.data;

	// tao anh dich
	destinationImage = Mat(rows, cols, CV_8UC3);

	// lay chieu dai cua 1 dong anh dich
	auto rowStepDest = destinationImage.step[0];

	// lay chieu rong cua 1 pixel anh dich
	auto colStepDest = destinationImage.step[1];

	// lay con tro data destinationImage
	uchar* pDataDest = (uchar*)destinationImage.data;

	for (int y = 0; y < rows; y++, pDataSrc += rowStepSrc, pDataDest += rowStepDest) {
		uchar* pRowSrc = pDataSrc;
		uchar* pRowDest = pDataDest;
		for (int x = 0; x < cols; x++, pRowSrc += colStepSrc, pRowDest += colStepDest) {

			// cong thuc :https://docs.opencv.org/2.4/modules/imgproc/doc/miscellaneous_transformations.html#void%20cvtColor%28InputArray%20src,%20OutputArray%20dst,%20int%20code,%20int%20dstCn%29

			// chuyen ti le 0-1
			float R = pRowSrc[0] / 255.f;
			float G = pRowSrc[1] / 255.f;
			float B = pRowSrc[2] / 255.f;


			float H = 0;
			float S = 0;
			float V = Util::max(Util::max(R, G), B);

			float minRGB = Util::min(Util::min(R, G), B);
			float VSubMin = V - minRGB;

			if (VSubMin > 0) {

				S = VSubMin / V;

				if (V == R) {
					H = 60 * (G - B) / VSubMin;
				}
				else if (V == G) {
					H = 120 + 60 * (B - R) / VSubMin;
				}
				else if (V == B) {
					H = 240 + 60 * (R - G) / VSubMin;
				}
				if (H < 0)
				{
					H += 360;
				}
			}

			pRowDest[0] = H / 2.f;
			pRowDest[1] = 255.f * S;
			pRowDest[2] = 255.f * V;
		}
	}
	return 1;
}

int Converter::Convert(Mat& sourceImage, Mat& destinationImage, int type)
{
	if (type == 0) {
		//chuyen rgb sang grayscale
		return this->RGB2GrayScale(sourceImage, destinationImage);
	}
	else if (type == 1) {
		//chuyen rgb sang hsv
		return this->RGB2HSV(sourceImage, destinationImage);
	}
	else
		return 0;
}

Converter::Converter()
{
}

Converter::~Converter()
{
}
