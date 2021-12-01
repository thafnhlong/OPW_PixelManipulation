#include "ColorTransformer.h"

int ColorTransformer::ChangeBrighness(const Mat& sourceImage, Mat& destinationImage, short b)
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

	// tao anh destinationImage
	destinationImage = sourceImage.clone();

	// lay con tro data destinationImage
	uchar* pDataDest = (uchar*)destinationImage.data;

	for (int y = 0; y < rows; y++, pDataSrc += rowStepSrc, pDataDest += rowStepSrc) {
		uchar* pRowSrc = pDataSrc;
		uchar* pRowDest = pDataDest;
		for (int x = 0; x < cols; x++, pRowSrc += colStepSrc, pRowDest += colStepSrc) {

			// loc qua tung kenh mau
			for (int i = 0; i < colStepSrc; ++i)
			{
				// su dung int tranh tran so
				int newPixel = b + pRowSrc[i];
				if (newPixel > 255) {
					pRowDest[i] = 255;
				}
				else if (newPixel < 0) {
					pRowDest[i] = 0;
				}
				else {
					pRowDest[i] = newPixel;
				}
			}
		}
	}

	return 1;
}

int ColorTransformer::ChangeContrast(const Mat& sourceImage, Mat& destinationImage, float c)
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
	destinationImage = sourceImage.clone();

	// lay con tro data destinationImage
	uchar* pDataDest = (uchar*)destinationImage.data;

	for (int y = 0; y < rows; y++, pDataSrc += rowStepSrc, pDataDest += rowStepSrc) {
		uchar* pRowSrc = pDataSrc;
		uchar* pRowDest = pDataDest;
		for (int x = 0; x < cols; x++, pRowSrc += colStepSrc, pRowDest += colStepSrc) {

			// loc qua tung kenh mau
			for (int i = 0; i < colStepSrc; ++i)
			{
				// su dung int tranh tran so
				int newPixel = c * pRowSrc[i];
				if (newPixel > 255) {
					pRowDest[i] = 255;
				}
				else if (newPixel < 0) {
					pRowDest[i] = 0;
				}
				else {
					pRowDest[i] = newPixel;
				}
			}
		}
	}

	return 1;
}

int ColorTransformer::CalcHistogram(const Mat& sourceImage, Mat& histMatrix)
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

	//Khoi tao ma tran histogram toan bo so 0 voi kich thuoc channels x 256, kieu so thuc
	histMatrix = Mat::zeros(colStepSrc, 256, CV_32FC1);

	// lay chieu dai cua 1 dong ma tran histogram
	auto widthStepHistMatrix = histMatrix.step[0];

	// lay con tro data ma tran histogram
	uchar* pDataHistMatrix = (uchar*)histMatrix.data;

	// khoi tao mang con tro chua cac con tro tro toi tung row
	float** pChannel = new float* [colStepSrc];

	// gan tung con tro tro toi tung dong cua histogram matrix vao con tro pChannel
	for (int i = 0; i < colStepSrc; ++i, pDataHistMatrix += widthStepHistMatrix) {
		pChannel[i] = (float*)pDataHistMatrix;
	}

	for (int y = 0; y < rows; y++, pDataSrc += rowStepSrc) {
		uchar* pRowSrc = pDataSrc;
		for (int x = 0; x < cols; x++, pRowSrc += colStepSrc) {

			// loc qua tung kenh mau
			for (int i = 0; i < colStepSrc; ++i)
			{
				// tang so luong mau i len 1
				uchar indexColor = pRowSrc[i];
				pChannel[i][indexColor] += 1;
			}
		}
	}
	return 1;
}

int ColorTransformer::HistogramEqualization(const Mat& sourceImage, Mat& destinationImage)
{
	if (sourceImage.empty()) {
		return 0;
	}

	// ma tran histogram
	Mat histMatrix;

	// tinh histogram cho anh
	int result = this->CalcHistogram(sourceImage, histMatrix);

	if (!result) {
		return 0;
	}

	// lay so kenh mau
	int nChannels = histMatrix.rows;

	// tim tong so pixel tren anh
	int totalPixel = 0;

	// con tro dong so 0
	auto pRow = histMatrix.ptr<float>(0);
	for (int x = 0; x < 256; ++x, pRow++) {
		totalPixel += *pRow;
	}


	// tao bang phan phoi tich luy va thay doi gia tri
	Mat convertMatrix = histMatrix.clone();

	// cong thuc: https://www.math.uci.edu/icamp/courses/math77c/demos/hist_eq.pdf

	// chay qua tung kenh mau
	for (int y = 0; y < nChannels; ++y) {
		// tong tich luy
		float sumValue = 0;

		// con tro dong y
		auto pRowCdf = convertMatrix.ptr<float>(y);
		for (int x = 0; x < 256; ++x, pRowCdf++) {
			// sum tich luy so luong mau i
			sumValue += *pRowCdf;
			// gia tri moi = P (tong tich luy) * ( L - 1 ) 
			// L = 256
			*pRowCdf = floor(sumValue / totalPixel * 255);
		}
	}

	// tao mot ma tran histogram va gan gia tri = 0
	Mat equalizeMatrix = histMatrix.clone();
	equalizeMatrix.setTo(0);

	// anh xa lai ma tran histogram
	for (int y = 0; y < nChannels; ++y) {
		// loc qua tung kenh mau
		auto pRowOrigin = histMatrix.ptr<float>(y);
		auto pRowCdf = convertMatrix.ptr<float>(y);
		auto pRowEqualize = equalizeMatrix.ptr<float>(y);

		// voi moi gia tri mau x, ta tim mau tuong ung sau khi can bang, tiep theo cong vao gia tri dang co
		for (int x = 0; x < 256; ++x) {

			// mau tuong ung voi mau x
			int convertColor = pRowCdf[x];

			// so luong mau x co trong anh
			int freqColor = pRowOrigin[x];

			// cap nhat lai so luong mau bien doi
			pRowEqualize[convertColor] += freqColor;
		
		}
	}

	// thuc hien luu anh vao ma tran destinationImage
	this->DrawHistogram(equalizeMatrix, destinationImage);

	return 1;
}

int ColorTransformer::DrawHistogram(const Mat& histMatrix, Mat& histImage)
{
	if (histMatrix.empty()) {
		return 0;
	}

	int nChannels = histMatrix.rows;

	//chuan hoa lai luoc do theo chieu cao co san

	// tao mang luu gia tri max cua cac Channel
	int* maxCount = new int[nChannels] {};
	for (int i = 0; i < nChannels; ++i) {
		maxCount[0] = 0;
	}

	// thuc hien tim max
	for (int y = 0; y < nChannels; ++y) {
		// con tro dong y
		auto pRow = histMatrix.ptr<float>(y);
		for (int x = 0; x < 256; ++x, pRow++) {
			if (maxCount[y] < *pRow) {
				maxCount[y] = *pRow;
			}
		}
	}

	// sao chep ma tran histogram
	Mat copyHistMatrix = histMatrix.clone();

	// buoc chuan hoa theo chieu cao max va heightSize
	for (int y = 0; y < nChannels; ++y) {
		// con tro dong y
		auto pRowCopy = copyHistMatrix.ptr<float>(y);
		for (int x = 0; x < 256; ++x, pRowCopy++) {
			// gia tri moi = gia tri cu / max * chieu cao luoc do
			*pRowCopy = *pRowCopy / maxCount[y] * HEIGHT_SIZE;
		}
	}

	// chieu rong moi gia tri mau
	int widthColSize;

	// chieu cao cua cua so
	int heightWindow = PADDING_SIZE * 2 + HEIGHT_SIZE;

	// toa do truc y khoi dau 
	int yaxixPixel = PADDING_SIZE + HEIGHT_SIZE;

	// mau co so
	Scalar baseColor[3];

	if (nChannels > 1) {
		// 3 channels
		widthColSize = WIDTH_COL_SIZE_3C;
		baseColor[0] = Scalar(0, 0, 255); // red
		baseColor[1] = Scalar(0, 255, 0); // green
		baseColor[2] = Scalar(255, 0, 0); // blue
	}
	else {
		// gray scale
		widthColSize = WIDTH_COL_SIZE_1C;
		baseColor[0] = Scalar(128, 128, 128); // gray
	}

	// chieu rong cua cua so
	int widthWindow = (PADDING_SIZE + 256 * widthColSize) * nChannels + PADDING_SIZE;

	// khoi tao anh histogram co mau den
	histImage = Mat::zeros(heightWindow, widthWindow, CV_8UC3);

	// lap qua tung kenh mau ( tung dong ma tran histogram )
	for (int y = 0; y < nChannels; ++y) {

		// con tro dong y
		auto pRowCopy = copyHistMatrix.ptr<float>(y);

		// chieu cao cua mau thu 0 trong kenh mau y
		float heightPixel = *pRowCopy;

		// tao ra 1 diem co toa do mau dau tien cua kenh
		Point prePoint = Point(PADDING_SIZE + (PADDING_SIZE + 256 * widthColSize) * y,
			yaxixPixel - round(heightPixel));

		// di chuyen con tro toi mau tiep theo
		pRowCopy++;

		for (int x = 1; x < 256; ++x, pRowCopy++) {

			// tao ra diem ket thuc cua duong thang
			Point nextPoint = prePoint;
			nextPoint.x += widthColSize;
			nextPoint.y = yaxixPixel - round(*pRowCopy);

			// ve duong thang
			line(histImage, prePoint, nextPoint, baseColor[y]);

			// set lai diem
			prePoint = nextPoint;
		}
	}

	return 1;
}

ColorTransformer::ColorTransformer()
{
}

ColorTransformer::~ColorTransformer()
{
}
