#include <ctype.h>
#include <iomanip>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <cstdlib>
#include "TString.h"
#include "TH1F.h"

using namespace std;


void make_hist(TString type, TString physics_type){
	

	const int num_samples = 3;
	Float_t xsec[num_samples] = {2.16, 3.67E05, 2.94E04};

	TFile *file = new TFile("../application/main_tmva_4_"+physics_type+type+".root");
   TTree *tree = (TTree*)file->Get("tree");
	TH1F *countPos = (TH1F*)file->Get("CountPosWeight");
	TH1F *countNeg = (TH1F*)file->Get("CountNegWeight");
//	Int_t events_generated_s = countPos_s->GetEntries()-countNeg_s->GetEntries();
	/*
	TFile *file_b = new TFile("../application/main_tmva_4_BG"+type+".root");
   TTree *tree_b = (TTree*)file_b->Get("tree");
	TH1F *countPos_b = (TH1F*)file_b->Get("CountPosWeight");
	TH1F *countNeg_b = (TH1F*)file_b->Get("CountNegWeight");
//	Int_t events_generated_b = countPos_b->GetEntries()-countNeg_b->GetEntries();
*/
	float BDT, BDT_b;
	float weight = 5, weight_b;

	tree->SetBranchAddress("BDT_VBF",&BDT);
	tree->SetBranchAddress("genWeight",&weight);
//	tree_b->SetBranchAddress("BDT_VBF",&BDT_b);
//	tree_b->SetBranchAddress("genWeight",&weight_b);

	TH1F *BDT_output = new TH1F("BDT_output","",400,-1.,1.);
//	TH1F *BDT_output_b = new TH1F("BDT_output_b","",400,-1.,1.);

	TFile *f_output = new TFile("BDT_hist_"+physics_type+type+".root","new");
	f_output->cd();
   tree->Draw("BDT_VBF>>BDT_output(400)","genWeight*(1>0)");
	BDT_output->Scale(xsec[0]);
   f_output->Write();
/*
	TFile *f_output_b = new TFile("BDT_hist_bg"+type+".root","new");
	f_output_b->cd();
   tree_b->Draw("BDT_VBF>>BDT_output_b(400)","weight_b*(1>0)");
   f_output_b->Write();

*/
}
