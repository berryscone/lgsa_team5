#include <iostream>
using namespace std;

#include "sample0_VideoCapture.h"


int main()
{
	while (true) {
		int sample_no = -1;
		cout << endl << "Sample No >> ";
		cin >> sample_no;

		switch (sample_no) {
		case 0:
			run_sample0();
			break;
		default:
			cerr << "Invalid Sample No - " << sample_no << endl;
			break;
		}
	}
}
