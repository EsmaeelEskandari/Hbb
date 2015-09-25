#include "TFile.h"
#include "TString.h"
#include "TSystem.h"
#include "TTree.h"
#include "CreateTree_tmva_double.h"
#include "CreateTree_tmva_double.C"
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
		
		TFile *f = TFile::Open(input_filename);
		if (f!=0){	
			CreateTree_tmva_double	*c = new CreateTree_tmva_double(0,input_filename);
			c->Loop(input_filename,output_dir,sample_type);
		}
	return 0;
}
