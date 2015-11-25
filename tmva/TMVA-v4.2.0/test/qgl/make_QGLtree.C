#include "TFile.h"
#include "TString.h"
#include "TSystem.h"
#include "TTree.h"
#include "MakeQGL.h"
#include "MakeQGL.C"
#include <iostream>
#include <fstream>
#include <sstream>
#include <stdio.h>
#include <string>
#include <stdlib.h>

using namespace std;

int main(int argc, char* argv[]){
		TString input_filename = std::string(argv[1]);
		TString output_dir = std::string(argv[2]);
		int set_type = atoi(argv[3]);
		
		TFile *f = TFile::Open(input_filename);
		if (f!=0){	
			MakeQGL	*c = new MakeQGL(0,input_filename);
			c->Loop(input_filename,output_dir,set_type);
		}
	return 0;
}
