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
	
	const int num_samples = 12;
	int sample = atoi(sample_str);
	
	TFile *file = new TFile("../application/v13/main_tmva_v13_Data_4_"+physics_type+type+".root");
   TTree *tree = (TTree*)file->Get("tree");
	Int_t events_generated;
	TH1F *countPos;
	TH1F *countNeg;
	if (sample!=11){
		countPos = (TH1F*)file->Get("CountPosWeight");
 		countNeg = (TH1F*)file->Get("CountNegWeight");
 		events_generated = countPos->GetEntries()-countNeg->GetEntries();
	} else events_generated = 1;

///	TString file_names[nfiles] = {"QCD_HT100to200", "QCD_HT200to300", "QCD_HT300to500","QCD_HT500to700", "QCD_HT700to1000", "QCD_HT1000to1500", "QCD_HT1500to2000", "QCD_HT2000toInf", "VBFHToBB_M-125_13TeV_powheg", "VBFHToBB_M-130_13TeV_powheg", "VBFHToBB_M125_13TeV_amcatnlo","data"};
	Float_t xsec[num_samples] = { 2.75E07,  1.74E06,  3.67E05, 2.94E04, 6.52E03,1.064E03,   121.5,  2.54E01,2.16 ,1.96,2.16,1./15.47};

	float lumi=10000; //in pb
	float BDT;
	float genweight=1.;

	tree->SetBranchAddress("BDT_VBF",&BDT);
	tree->SetBranchAddress("genWeight",&genweight);

	TH1F *BDT_output = new TH1F("BDT_output","",400,-1.,1.);
	
	int nentries = tree->GetEntries();
	for (int entry=0;entry<nentries;entry++){
		tree->GetEntry(entry);
		genweight=TMath::Sign(1.,genweight);
		genweight/=events_generated/(xsec[sample]*lumi); //10 fb^-1 normalization
		BDT_output->Fill(BDT,genweight);
	}

	TFile *f_output = new TFile("output_hist/v13/BDT_hist_"+physics_type+type+".root","recreate");
	f_output->cd();
   BDT_output->Write();
   f_output->Write();
}
