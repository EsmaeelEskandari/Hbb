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
#include "/afs/cern.ch/work/n/nchernya/Hbb/preselection_double.C"
#include "/afs/cern.ch/work/n/nchernya/Hbb/preselection_single.C"
#include "/afs/cern.ch/work/n/nchernya/Hbb/preselection_single_blike.C"


using namespace std;

const int njets=300;
typedef struct {
   Float_t eta[njets];
   Float_t pt[njets];
   Float_t phi[njets];
	Float_t mass[njets];
	Float_t btag[njets];
	Int_t id[njets];
   Float_t blike_VBF[njets];
   Float_t pt_regVBF[njets];
} Jets;


int main(int argc, char* argv[]){
//void make_hist_narrow_double(TString type, TString physics_type, TString sample_str){
	
	TString type[2] = {"_double","_single"};
	int sample =atoi(argv[1]);
	int set_type=atoi(argv[2]);

	const int nfiles = 20;
	TString file_names[nfiles] = {"QCD_HT100to200", "QCD_HT200to300", "QCD_HT300to500","QCD_HT500to700", "QCD_HT700to1000", "QCD_HT1000to1500", "QCD_HT1500to2000", "QCD_HT2000toInf", "VBFHToBB_M-125_13TeV_powheg", "GF", "BTagCSV","TTbar","DYtoQQ","ST_tW","ttHtobb","ttHtoNbb", "DYtoLL"};
	Double_t xsec[nfiles] = { 2.75E07, 1.74E06,  3.67E05, 2.94E04, 6.52E03,1.064E03,   121.5,  2.54E01,2.16 ,25.17,1.,816.,1461., 71.7,0.2934,  0.2151,  6025.2  };
	Double_t data[nfiles] = {0.,0.,0.,0,0,0,0,0,0,0,1,0,0,0,0,0,0.};

//	TFile *file = TFile::Open("../application/v13/main_tmva_v13_Data_4_"+physics_type+type+".root");
	TFile *file = TFile::Open("dcap://t3se01.psi.ch:22125//pnfs/psi.ch/cms/trivcat///store/user/nchernya/Hbb/v14/main_tmva/main_mva_v14_"+file_names[sample]+type[set_type]+".root");
   TTree *tree = (TTree*)file->Get("tree");
	Int_t events_generated;
	TH1F *countPos;
	TH1F *countNeg;
	if (data[sample]!=1){
		countPos = (TH1F*)file->Get("CountPosWeight");
 		countNeg = (TH1F*)file->Get("CountNegWeight");
 		events_generated = countPos->GetEntries()-countNeg->GetEntries();
	} else events_generated = 1;

	float lumi=1280.23; //in pb
	float BDT;
	float genweight=1.;
	float puWeight=1;
   Jets Jet;
   Int_t nJets;
	Float_t HLT_QuadPFJet_DoubleBTagCSV_VBF_Mqq200;
	Float_t HLT_QuadPFJet_SingleBTagCSV_VBF_Mqq460;
	Float_t json=1.;

	tree->SetBranchAddress("BDT_VBF",&BDT);
	tree->SetBranchAddress("genWeight",&genweight);
	tree->SetBranchAddress("puWeight",&puWeight);
	tree->SetBranchAddress("json",&json);
   tree->SetBranchAddress("nJet",&nJets);
   tree->SetBranchAddress("Jet_pt",Jet.pt);
   tree->SetBranchAddress("Jet_eta",Jet.eta);
   tree->SetBranchAddress("Jet_phi",Jet.phi);
	tree->SetBranchAddress("Jet_mass",Jet.mass);
	tree->SetBranchAddress("Jet_btagCSV",Jet.btag);
	tree->SetBranchAddress("Jet_id",Jet.id);	
	tree->SetBranchAddress("Jet_blike_VBF",Jet.blike_VBF);
	tree->SetBranchAddress("Jet_pt_regVBF",Jet.pt_regVBF);
 	tree->SetBranchAddress("HLT_BIT_HLT_QuadPFJet_DoubleBTagCSV_VBF_Mqq200_v",&HLT_QuadPFJet_DoubleBTagCSV_VBF_Mqq200);
 	tree->SetBranchAddress("HLT_BIT_HLT_QuadPFJet_SingleBTagCSV_VBF_Mqq460_v",&HLT_QuadPFJet_SingleBTagCSV_VBF_Mqq460);

	TH1F *BDT_output = new TH1F("BDT_output","",400,-1.,1.);
	BDT_output->GetXaxis()->SetTitle("BDT");
	TH1F *BDT_output_narrow = new TH1F("BDT_output_narrow","",400,-1.,1.);
	BDT_output_narrow->GetXaxis()->SetTitle("BDT, 110 < m(H) < 140 GeV");
	
	int nentries = tree->GetEntries();
	for (int entry=0;entry<nentries;entry++){
		tree->GetEntry(entry);
		genweight=TMath::Sign(1.,genweight);
		if (data[sample]!=1) {
			genweight/=events_generated/(xsec[sample]*lumi);
			genweight*=puWeight;
		}
		else genweight=1.; 
		if (json!=1) continue;	

		int btag_max1_number = -1;
		int btag_max2_number = -1;
		int pt_max1_number = -1;
		int pt_max2_number = -1;
		TLorentzVector Bjet1;
		TLorentzVector Bjet2;
		TLorentzVector Qjet1;
		TLorentzVector Qjet2;
		TLorentzVector qq;

		if (set_type==0) {
		 if (preselection_single_blike(nJets, Jet.pt,Jet.eta, Jet.phi, Jet.mass, Jet.blike_VBF, Jet.id, btag_max1_number, btag_max2_number, pt_max1_number, pt_max2_number, HLT_QuadPFJet_SingleBTagCSV_VBF_Mqq460, Bjet1, Bjet2, Qjet1, Qjet2, qq) == 0) continue;

			if (preselection_double(nJets, Jet.pt,Jet.eta, Jet.phi, Jet.mass, Jet.btag, Jet.id, btag_max1_number, btag_max2_number, pt_max1_number, pt_max2_number, HLT_QuadPFJet_DoubleBTagCSV_VBF_Mqq200, Bjet1, Bjet2, Qjet1, Qjet2, qq) != 0) continue;
		}
		else {
			if (set_type==1) if (preselection_single_blike(nJets, Jet.pt,Jet.eta, Jet.phi, Jet.mass, Jet.blike_VBF, Jet.id, btag_max1_number, btag_max2_number, pt_max1_number, pt_max2_number, HLT_QuadPFJet_SingleBTagCSV_VBF_Mqq460, Bjet1, Bjet2, Qjet1, Qjet2, qq) != 0) continue;
		}


		Float_t alpha_bjet1_reg = 1; 
		Float_t alpha_bjet2_reg = 1 ;
		if (Jet.pt_regVBF[btag_max1_number]>0) alpha_bjet1_reg = Jet.pt_regVBF[btag_max1_number]/Jet.pt[btag_max1_number];
  		if (Jet.pt_regVBF[btag_max2_number]>0) alpha_bjet2_reg = Jet.pt_regVBF[btag_max2_number]/Jet.pt[btag_max2_number];
		Bjet1*=alpha_bjet1_reg;
		Bjet2*=alpha_bjet2_reg;

		TLorentzVector bb;
		bb = Bjet1+Bjet2;
		Float_t Mbb = bb.M();

		if ((Mbb<140)&&(Mbb>110)) BDT_output_narrow->Fill(BDT,genweight);
		BDT_output->Fill(BDT,genweight);
	}

	TFile *f_output = TFile::Open("output_hist/v14/BDT_hist_"+file_names[sample]+type[set_type]+".root","recreate");
	f_output->cd();
   BDT_output->Write();
   BDT_output_narrow->Write();
   f_output->Write();

	return 0;
}
