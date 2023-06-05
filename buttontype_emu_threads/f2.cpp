#include <iostream>
#include <fstream>
#include <string>
#include <chrono>
#include <thread>
#include <time.h>
#include <stdlib.h>
#define CLICK_LOWER 25
#define CLICK_UPPER 80
#include <windows.h>
#define NUMBER 1
#define KEY 0x32
using namespace std;
int basedrand(double maxtime) {

	int lower_bound = maxtime * 2000 / 5;
	int upper_bound = maxtime * 1000;

	return rand() % (upper_bound - lower_bound + 1) + lower_bound;
}
void CLICK(INPUT* in, int ct) {
	SendInput(1, in++, sizeof(INPUT));
	this_thread::sleep_for(chrono::milliseconds(ct));
	SendInput(1, in, sizeof(INPUT));
}
void menu(int paused) {

	string status = (paused) ? "STATUS:PAUSED" : "STATUS:UNPAUSED";
	cout << status << endl;
}
void dispatch(double f_time) {
	bool paused = 1;
	bool state = 1;
	int timer;
	int CT = 25;
	INPUT Inputs[2] = { 0 };
	Inputs[0].type = INPUT_KEYBOARD;
	Inputs[0].ki.wVk = KEY;
	Inputs[1].type = INPUT_KEYBOARD;
	Inputs[1].ki.dwFlags = KEYEVENTF_KEYUP;
	Inputs[1].ki.wVk = KEY;
	while (state) {
		/*if (GetAsyncKeyState(0x20)) {
			paused = (paused) ? 0 : 1;
			this_thread::sleep_for(chrono::milliseconds(500));
		}
		if (GetAsyncKeyState(0x23)) {
			return;
		}


		if (paused)
			continue;*/
		timer = basedrand(f_time);
		CT = rand() % (CLICK_UPPER - CLICK_LOWER + 1) + CLICK_LOWER;
		cout << "FL2: " << timer << "\tCLICK_TIME:" << CT;
		CLICK(Inputs, CT);
		this_thread::sleep_for(chrono::milliseconds(timer - CT));
		cout << "\r";
	}
}
int main() {
	srand(time(NULL));
	double f_time = 0;
	ifstream f1(".\\timings.binary", std::fstream::binary);

	if (f1.good()) {
		f1.seekg(NUMBER * sizeof(double));
		f1.read((char*)&f_time, sizeof(double));
		dispatch(f_time);
	}
	f1.close();
	return 0;
}

