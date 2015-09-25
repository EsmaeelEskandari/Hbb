#include "TROOT.h"
#include "TSystem.h"
#include "TApplication.h"
#include <iostream>
int main(int argc, char* argv[]){
//	char* variable_name = (argv[1]);	
//	char* type = (argv[2]);
	char cmd[50];
	sprintf(cmd,".x TMVAClassification_main.C(\"%s\",\"%s\")",argv[1],argv[2]);	
	gROOT->ProcessLine(cmd);
	return 0;
}
