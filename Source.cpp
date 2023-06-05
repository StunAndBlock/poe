
#include <windows.h>
#include <iostream>
#include <fstream>
#include <string>
#include <chrono>
#include <thread>
#include <time.h>
#include <stdlib.h>
#define D_DEF ".\\opt.cfg"
#define F_DEF ".\\def.flask"
using namespace std;

STARTUPINFO si[5];
PROCESS_INFORMATION pi[5];

 bool isExist(const std::string& name,ifstream* cfg) {
	cfg->open(name);
	return cfg->good();
}
 void onLostCfgStart() {
	 ofstream newcfg(D_DEF, std::ofstream::out);
	 newcfg << "FLASK_FILE:def.flask\nPAUSE_BUTTON:0x20";
	 newcfg.close();
	
 }

 void onLostDef(string FLASK_FILE) {
	 ofstream newflask(F_DEF, std::ofstream::out);
	 newflask << "5\n1.0\n1.0\n1.1\n1.0\n1.0";
	 newflask.close();
 }
 void comma(int msec) {
	 cout << "#";
	this_thread::sleep_for(chrono::milliseconds(msec));
	 
 }

  void parseCfgFile(string* FLASK_FILE,string* PAUSE_BUTTON) {
	  string temp;
	  if (FLASK_FILE->c_str()[10] == ':') {
		  FLASK_FILE->insert(11, ".\\\\");
		  temp = FLASK_FILE->substr(11, FLASK_FILE->size());
		  *FLASK_FILE = temp;

		  if (PAUSE_BUTTON->c_str()[12] == ':') {
			  temp = PAUSE_BUTTON->substr(13, PAUSE_BUTTON->size());
			  *PAUSE_BUTTON = temp;
		  }
	  }
	  
	 
 }




 void readCfgMessage(ifstream* cfg,string* FLASK_FILE,string* PAUSE_BUTTON) {
	 cout << "READING <opt.cfg> FILE\n[";
	 comma(500);
	 if (cfg->peek() == EOF) {
		 system("cls");
		 cout << "EMPTY FILE";
		 onLostCfgStart();
		 cfg->close();
		 return;
	 }
	 comma(150);
	 (*cfg) >> *FLASK_FILE;
	 (*cfg) >> *PAUSE_BUTTON;
	 comma(100);
	 parseCfgFile(FLASK_FILE,PAUSE_BUTTON);
	 comma(350);
	 comma(900);
	 cout << "]\n";
	 system("cls");
 }
 double* readFlaskFile(string FLASK_FILE, double* FLASKARR,int* n) {
	
	 ifstream flask;
	 bool state = 1;
	 while(state)
	 if (isExist(FLASK_FILE, &flask)) {
		 flask >> *n;
		 FLASKARR = new double[*n];
		 for (int i = 0; i < *n; i++) {
			 flask >> FLASKARR[i];
			
		 }
		 flask.close();
		 state = 0;
	 }
	 else {
		 onLostDef(FLASK_FILE);
	 }
	 return FLASKARR;
}

 void menu(string PAUSE_BUTTON,double* FLASKARR,int paused,int n) {
	 string status;
	 status = (paused) ? "STATUS:PAUSED" : "STATUS:UNPAUSED";
	 cout << status << endl;
	 for (int i = 0; i < n; i++) {
		 cout << "FLASK N" << i + 1 << " TIME:\t"<< FLASKARR[i] << endl;
	 }
	 cout << "PAUSE_BUTTON:\t" << PAUSE_BUTTON << "\n-----------------------------\n";
	 }

 int basedrand(double maxtime){
	
	 int lower_bound = maxtime * 50;
	 int upper_bound = maxtime * 1000;

	 return rand() % upper_bound + lower_bound;
 }
 void STARTMAINFRAME(int n) {
	 wchar_t fname[9] = L".\\f0.exe";
	 for (int i = 0; i < n; i++) {
		 fname[3] = (wchar_t)(i+1+'0');
		 //cout << fname;
		 ZeroMemory(&si[i], sizeof(si[i]));
		 si[i].cb = sizeof(si[i]);
		 ZeroMemory(&pi[i], sizeof(pi[i]));
		 if (!CreateProcess(fname,   // No module name (use command line)
			 NULL,        // Command line
			 NULL,           // Process handle not inheritable
			 NULL,           // Thread handle not inheritable
			 FALSE,          // Set handle inheritance to FALSE
			 0,              // No creation flags
			 NULL,           // Use parent's environment block
			 NULL,           // Use parent's starting directory 
			 &si[i],            // Pointer to STARTUPINFO structure
			 &pi[i])           // Pointer to PROCESS_INFORMATION structure
			 )
		 {
			 cout << "CreateProcess failed (%d)." << GetLastError();
		 }
	 }
	// WaitForSingleObject(pi.hProcess, INFINITE);
 }
 void dispatch(string PAUSE_BUTTON, double* FLASKARR, int n) {
	 bool paused = 1;
	 menu(PAUSE_BUTTON, FLASKARR, paused,n);
	 bool state = 1;
	 ofstream binary("timings.binary", std::ofstream::out | std::ofstream::binary | std::ofstream::trunc);
	 for (int i = 0; i < n; i++) {
		 binary.write((char*)&FLASKARR[i], sizeof(double));
	 }
	 binary.close();

	 STARTMAINFRAME(n);
	 for (int i = 0; i < n; i++)
		 SuspendThread(pi[i].hThread);
	 while (state) {
		 if (GetAsyncKeyState(0x2D)) {
			 paused = (paused) ? 0 : 1;
			 if (paused) {
				 for (int i = 0; i < n; i++)
					 SuspendThread(pi[i].hThread);
			 }
			 else {
				 for (int i = 0; i < n; i++)
				 while (ResumeThread(pi[i].hThread) > 0);
			 }
			 this_thread::sleep_for(chrono::milliseconds(500));
			 system("cls");
			 menu(PAUSE_BUTTON, FLASKARR, paused,n);
			 
		 }
		 if (GetAsyncKeyState(0x23)) {
			 for (int i = 0; i < n; i++) {
				 TerminateProcess(pi[i].hProcess, 1);
				 state = 0;
			 }
		 }

	 }

 }
int main(int argc,char* argv[]) {
	ifstream cfg;
	
	srand(time(NULL));
	string FLASK_FILE;
	string PAUSE_BUTTON;
	double* FLASKARR = NULL;
	int n;
	bool state = 1;
	char option;
	while (state) {
		if (isExist(D_DEF, &cfg)) {
			readCfgMessage(&cfg,&FLASK_FILE,&PAUSE_BUTTON);
			FLASKARR=readFlaskFile(FLASK_FILE, FLASKARR,&n);
			dispatch(PAUSE_BUTTON, FLASKARR,n);
			state = 0;
		
		} else {
			cout << "<opt.cfg> is missing,\n[Y - default file will be created, proceed\\N - exit] : ";
			cin >> option;
			switch (option) {
			case 89:
			case 121:
				onLostCfgStart();
				break;
			case 78:
			case 110:
				state = 0;
				break;
			}
			
		}
	}
		
	

	delete FLASKARR;
	return 0;
}