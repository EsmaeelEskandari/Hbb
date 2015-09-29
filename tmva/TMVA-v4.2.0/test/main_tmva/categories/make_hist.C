#include <ctype.h>
#include <iomanip>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <cstdlib>
#include "TString.h"
#include "TH1F.h"

using namespace std;


void make_hist(TString type, TString physics_type, TString sample_str){
	
	const int num_samples = 3;
	int sample = atoi(sample_str);
	
	TFile *file = new TFile("../application/main_tmva_4_"+physics_type+type+".root");
   TTree *tree = (TTree*)file->Get("tree");
	TH1F *countPos = (TH1F*)file->Get("CountPosWeight");
	TH1F *countNeg = (TH1F*)file->Get("CountNegWeight");
	Int_t events_generated = countPos->GetEntries()-countNeg->GetEntries();
	
	Float_t xsec[num_samples] = {2.16, 3.67E05, 2.94E04}; //vbf 300-500 500-700

	float BDT;
	float genweight;

	tree->SetBranchAddress("BDT_VBF",&BDT);
	tree->SetBranchAddress("genWeight",&genweight);

	TH1F *BDT_output = new TH1F("BDT_output","",400,-1.,1.);
	
	int nentries = tree->GetEntries();
	for (int entry=0;entry<nentries;entry++){
		tree->GetEntry(entry);
		genweight/=events_generated/(xsec[sample]*10000); //10 fb^-1 normalization
		BDT_output->Fill(BDT,genweight);
	}

	TFile *f_output = new TFile("BDT_hist_"+physics_type+type+".root","recreate");
	f_output->cd();
   BDT_output->Write();
   f_output->Write();
}
