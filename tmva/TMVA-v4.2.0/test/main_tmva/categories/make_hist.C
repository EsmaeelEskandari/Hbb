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
	
	const int num_samples = 11;
	int sample = atoi(sample_str);
	
	TFile *file = new TFile("../application/main_tmva_4_"+physics_type+type+".root");
   TTree *tree = (TTree*)file->Get("tree");
	TH1F *countPos = (TH1F*)file->Get("CountPosWeight");
	TH1F *countNeg = (TH1F*)file->Get("CountNegWeight");
	Int_t events_generated = countPos->GetEntries()-countNeg->GetEntries();

///	TString file_names[nfiles] = {"QCD_HT100to200", "QCD_HT200to300", "QCD_HT300to500","QCD_HT500to700", "QCD_HT700to1000", "QCD_HT1000to1500", "QCD_HT1500to2000", "QCD_HT2000toInf", "VBFHToBB_M-125_13TeV_powheg", "VBFHToBB_M-130_13TeV_powheg", "VBFHToBB_M125_13TeV_amcatnlo"};
	Float_t xsec[num_samples] = { 2.75E07,  1.74E06,  3.67E05, 2.94E04, 6.52E03,1.064E03,   121.5,  2.54E01,2.16 ,1.96,2.16};

	float BDT;
	float genweight;

	tree->SetBranchAddress("BDT_VBF",&BDT);
	tree->SetBranchAddress("genWeight",&genweight);

	TH1F *BDT_output = new TH1F("BDT_output","",400,-1.,1.);
	
	int nentries = tree->GetEntries();
	for (int entry=0;entry<nentries;entry++){
		tree->GetEntry(entry);
		genweight=TMath::Sign(1.,genweight);
		genweight/=events_generated/(xsec[sample]*10000); //10 fb^-1 normalization
		BDT_output->Fill(BDT,genweight);
	}

	TFile *f_output = new TFile("BDT_hist_"+physics_type+type+".root","recreate");
	f_output->cd();
   BDT_output->Write();
   f_output->Write();
}
