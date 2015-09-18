#include "TFile.h"
#include "TString.h"
#include "TSystem.h"
#include "TTree.h"
#include "skim_trees_double.h"
#include "skim_trees_double.C"
#include <iostream>
#include <fstream>
#include <sstream>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

using namespace std;

int main(int argc, char* argv[]){
		TString file_name = std::string(argv[1]);
		TString tree_number = std::string(argv[2]); 	
		TString output_dir = std::string(argv[3]);
	
		TFile *f = TFile::Open(file_name);
		if (f!=0){	
			skim_trees_double *c = new skim_trees_double(0,file_name);
			c->Loop(output_dir+"/skimmed_tree_"+tree_number+".root", file_name);
		}
	return 0;
}
