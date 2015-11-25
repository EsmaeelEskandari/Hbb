#include "TFile.h"
#include "TString.h"
#include "TSystem.h"
#include "TTree.h"
#include "CreateTree_tmva_qgl.h"
#include "CreateTree_tmva_qgl.C"
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
		int sample_type = atoi(argv[3]);
		int set_type = atoi(argv[4]);
		
		TFile *f = TFile::Open(input_filename);
		if (f!=0){	
			CreateTree_tmva_qgl	*c = new CreateTree_tmva_qgl(0,input_filename);
			c->Loop(input_filename,output_dir,sample_type,set_type);
		}
	return 0;
}
