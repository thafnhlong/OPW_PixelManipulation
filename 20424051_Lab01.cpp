#include "Header.h"
#include "ColorTransformer.h"
#include "Converter.h"

void showInfo() {
	cout << "Cu phap su dung <ProgramName.exe> <Command> <InputPath> <CommandArguments>";
}


void rgb2grayAction(String InputPath) {
	//doc anh 3 kenh mau
	Mat input = imread(InputPath, IMREAD_COLOR);
	Mat output;

	Converter ct = Converter();

	if (ct.Convert(input, output, 0)) {
		imshow("Anh ban dau", input);
		imshow("Anh bien doi", output);
	}
	else {
		cout << "Khong the chuyen doi sang grayscale";
	}
}

void rgb2hsvAction(String InputPath) {
	//doc anh 3 kenh mau
	Mat input = imread(InputPath, IMREAD_COLOR);
	Mat output;

	Converter ct = Converter();

	if (ct.Convert(input, output, 1)) {
		imshow("Anh ban dau", input);
		imshow("Anh bien doi", output);
	}
	else {
		cout << "Khong the chuyen doi sang hsv";
	}
}

void brightAction(String InputPath, string CommandArguments) {
	// lay do sang b
	short b = 0;
	try {
		b = stoi(CommandArguments);
		if (to_string(b) != CommandArguments) {
			throw exception();
		}
	}
	catch (...) {
		cout << "Do sang khong hop le";
		return;
	}

	//doc anh bang bat ky mac sac nao co san
	Mat input = imread(InputPath, IMREAD_ANYCOLOR);
	Mat output;

	ColorTransformer ct = ColorTransformer();

	if (ct.ChangeBrighness(input, output, b)) {
		imshow("Anh ban dau", input);
		imshow("Anh bien doi", output);
	}
	else {
		cout << "Khong the thay doi do sang anh";
	}
}

void contrastAction(String InputPath, string CommandArguments) {
	// lay do tuong phan c
	float c = 0;
	try {
		c = stof(CommandArguments);
	}
	catch (...) {
		cout << "Do tuong phan khong hop le";
		return;
	}

	//doc anh bang bat ky mac sac nao co san
	Mat input = imread(InputPath, IMREAD_ANYCOLOR);
	Mat output;

	ColorTransformer ct = ColorTransformer();

	if (ct.ChangeContrast(input, output, c)) {
		imshow("Anh ban dau", input);
		imshow("Anh bien doi", output);
	}
	else {
		cout << "Khong the thay doi do tuong phan anh";
	}
}

void histAction(String InputPath) {
	//doc anh bang bat ky mac sac nao co san
	Mat input = imread(InputPath, IMREAD_ANYCOLOR);
	Mat output;

	ColorTransformer ct = ColorTransformer();

	if (ct.CalcHistogram(input, output)) {
		cout << "Tinh histogram anh thanh cong";

		Mat histImage;
		int ret = ct.DrawHistogram(output, histImage);
		if (ret) {
			imshow("Luoc do", histImage);
		}
	}
	else {
		cout << "Khong the tinh histogram anh";
	}
}

void equalhistAction(String InputPath) {
	//doc anh bang bat ky mac sac nao co san
	Mat input = imread(InputPath, IMREAD_ANYCOLOR);
	Mat output;

	ColorTransformer ct = ColorTransformer();

	if (ct.HistogramEqualization(input, output)) {
		cout << "Can bang histogram anh thanh cong";

		Mat histMatrix, histImage;
		ct.CalcHistogram(input, histMatrix);
		ct.DrawHistogram(histMatrix, histImage);

		imshow("Luoc do goc", histImage);
		imshow("Luoc do can bang", output);

	}
	else {
		cout << "Khong the Can bang histogram anh";
	}
}

void drawhistAction(String InputPath) {
	//doc anh bang bat ky mac sac nao co san
	Mat input = imread(InputPath, IMREAD_ANYCOLOR);
	Mat histMatrix;

	ColorTransformer ct = ColorTransformer();

	int ret = ct.CalcHistogram(input, histMatrix);
	if (ret) {
		Mat histImage;
		ret = ct.DrawHistogram(histMatrix, histImage);
		if (ret) {

			int nChannels = histMatrix.rows;
			string title;
			if (nChannels > 1) {
				cout << "Ve luoc do anh mau";
				title = "Histogram anh mau";
			}
			else {
				cout << "Ve luoc do anh xam";
				title = "Histogram anh xam";
			}

			imshow(title, histImage);

			return;
		}
	}

	cout << "Khong the ve histogram";
}

bool process(string Command, string InputPath, string CommandArguments) {

	if (Command == "--rgb2gray") {
		rgb2grayAction(InputPath);
	}
	else if (Command == "--rgb2hsv") {
		rgb2hsvAction(InputPath);
	}
	else if (Command == "--bright") {
		brightAction(InputPath, CommandArguments);
	}
	else if (Command == "--contrast") {
		contrastAction(InputPath, CommandArguments);
	}
	else if (Command == "--hist") {
		histAction(InputPath);
	}
	else if (Command == "--equalhist") {
		equalhistAction(InputPath);
	}
	else if (Command == "--drawhist") {
		drawhistAction(InputPath);
	}
	else {
		return false;
	}

	return true;
}

int main(int argc, char* argv[]) {
	// kiem tra so luong tham so hop le
	if (argc > 2) {
		string Command, InputPath, CommandArguments = "";
		Command = argv[1];
		InputPath = argv[2];

		// truong hop co tham so cau lenh
		if (argc > 3)
			CommandArguments = argv[3];

		if (!process(Command, InputPath, CommandArguments)) {
			cout << "Khong tim thay Command tuong ung";
			return 1;
		}
	}
	else
	{
		cout << "So luong tham so thieu\n";
		showInfo();
		return 1;
	}

	waitKey(0);
	return 0;
}