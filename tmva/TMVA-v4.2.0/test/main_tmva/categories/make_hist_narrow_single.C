#include <ctype.h>
#include <iomanip>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <cstdlib>
#include "TString.h"
#include "TH1F.h"
#include "TFile.h"
#include "TTree.h"
#include "TLorentzVector.h"
#include "/afs/cern.ch/work/n/nchernya/Hbb/preselection_single.C"

using namespace std;


const int njets=300;
typedef struct {
   Float_t eta[njets];
   Float_t pt[njets];
   Float_t phi[njets];
	Float_t mass[njets];
	Float_t btag[njets];
	Int_t id[njets];
} Jets;


int main(int argc, char* argv[]){
//void make_hist_narrow_single(TString type, TString physics_type, TString sample_str){
	
	TString type = std::string(argv[1]);
	TString physics_type = std::string(argv[2]);
	TString sample_str=std::string(argv[3]);

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
   Jets Jet;
   Int_t nJets;
	Float_t HLT_QuadPFJet_SingleBTag_CSV_VBF_Mqq460;

	tree->SetBranchAddress("BDT_VBF",&BDT);
	tree->SetBranchAddress("genWeight",&genweight);
   tree->SetBranchAddress("nJet",&nJets);
   tree->SetBranchAddress("Jet_pt",Jet.pt);
   tree->SetBranchAddress("Jet_eta",Jet.eta);
   tree->SetBranchAddress("Jet_phi",Jet.phi);
	tree->SetBranchAddress("Jet_mass",Jet.mass);
	tree->SetBranchAddress("Jet_btagCSV",Jet.btag);
	tree->SetBranchAddress("Jet_id",Jet.id);	
 	tree->SetBranchAddress("HLT_BIT_HLT_QuadPFJet_SingleBTagCSV_VBF_Mqq460_v",&HLT_QuadPFJet_SingleBTag_CSV_VBF_Mqq460);

	TH1F *BDT_output = new TH1F("BDT_output","",400,-1.,1.);
	
	int nentries = tree->GetEntries();
	for (int entry=0;entry<nentries;entry++){
		tree->GetEntry(entry);
		genweight=TMath::Sign(1.,genweight);
		genweight/=events_generated/(xsec[sample]*10000); //10 fb^-1 normalization

		int btag_max1_number = -1;
		int btag_max2_number = -1;
		int pt_max1_number = -1;
		int pt_max2_number = -1;
		TLorentzVector Bjet1;
		TLorentzVector Bjet2;
		TLorentzVector Qjet1;
		TLorentzVector Qjet2;
		TLorentzVector qq;

		if (preselection_single(nJet, Jet_pt,Jet_eta, Jet_phi, Jet_mass, Jet_btagCSV, Jet_id, btag_max1_number, btag_max2_number, pt_max1_number, pt_max2_number, HLT_BIT_HLT_QuadPFJet_SingleBTagCSV_VBF_Mqq460_v, Bjet1, Bjet2, Qjet1, Qjet2, qq) == -1) continue;

		TLorentzVector bb;
		bb = Bjet1+Bjet2;
		Float_t Mbb = bb.M();

		if ((Mbb<140)&&(Mbb>110)) BDT_output->Fill(BDT,genweight);
	}

	TFile *f_output = new TFile("output_hist/BDT_hist_narrow_"+physics_type+type+".root","recreate");
	f_output->cd();
   BDT_output->Write();
   f_output->Write();


	return 0;
}
