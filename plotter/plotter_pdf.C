#include <ctype.h>
#include <iomanip>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <cstdlib>
#include "TFile.h"
#include "TH1.h"
#include "TH1F.h"
#include "TH2.h"
#include "TTree.h"
#include "TChain.h"
#include <vector>
#include <iostream>
#include <fstream>
#include <cstdio>
#include <TROOT.h>
#include <TString.h>
#include <TStyle.h>
#include <TSystem.h>
#include <TCanvas.h>
#include <TMarker.h>
#include <THStack.h>
#include <TLegend.h>
#include <TLatex.h>
#include <TCut.h>
#include <TGraph.h>
#include <TGraphErrors.h>
#include <TLorentzVector.h>
#include <TMath.h>
#include <TF1.h>
#include <TProfile.h>
#include "/afs/cern.ch/work/n/nchernya/Hbb/preselection_double.C"
#include "/afs/cern.ch/work/n/nchernya/Hbb/preselection_single_blike.C"
#include "/afs/cern.ch/work/n/nchernya/Hbb/trigger_maps3.C"
#include "math.h"


Double_t erf( Double_t *x, Double_t *par){
  return par[0]/2.*(1.+TMath::Erf((x[0]-par[1])/par[2]));
}
Double_t erf2( Double_t *x, Double_t *par){
  return par[0]/2.*(1.+TMath::Erf((x[0]-par[1])/par[2]))+ (1.-par[0]);
}
#define SWAP2(A, B) { TLorentzVector t = A; A = B; B = t; }
void SortByEta(std::vector<TLorentzVector> &jets){
  int i, j;
	int n=jets.size();
  for (i = n - 1; i >= 0; i--){
    for (j = 0; j < i; j++){
      if (jets[j].Eta() < jets[j + 1].Eta() ){
        SWAP2( jets[j], jets[j + 1] );
		}
    }
	}
}

typedef std::map<double, int> JetList;
const int njets = 300;

typedef struct {
   Float_t eta[njets];
   Float_t pt[njets];
   Float_t JEC_corr[njets];
   Float_t JEC_corr_up[njets];
   Float_t JEC_corr_down[njets];
   Float_t JER_corr[njets];
   Float_t JER_corr_up[njets];
   Float_t JER_corr_down[njets];
   Float_t phi[njets];
	Float_t mass[njets];
	Float_t btag[njets];
	Int_t nsoft;
	Float_t soft_pt[njets];
	Float_t soft_eta[njets];
	Float_t soft_mass[njets];
	Float_t qgl[njets];
	Int_t nsoft2;
	Int_t nsoft5;
	Int_t nsoft10;
	Int_t id[njets];
	Int_t puId[njets];
	Float_t HTsoft;
	Float_t pt_regVBF[njets];	
	Float_t ptd[njets];
	Float_t axis2[njets];
	Int_t mult[njets];
	Float_t leadTrackPt[njets];
	Float_t blike_VBF[njets];
	Float_t qgl1_VBF[njets];
	Float_t qgl2_VBF[njets];
} Jets;

using namespace std;


int main(int argc, char* argv[]){


TString file_name = std::string(argv[1]);
TString file_tag = std::string(argv[2]);
TString dataset_type = std::string(argv[3]);
TString region = std::string(argv[4]); 
int data = atoi(argv[5]);
int applyTrigWeight = atoi(argv[6]);
TString trigWeight_str = std::string(argv[7]);
TString heppyVersion = std::string(argv[8]);
TString postfix = std::string(argv[9]);
TString output = std::string(argv[10]);


std::map <TString, float> xsec;
xsec["BTagCSV"] = 1.;
xsec["VBFHToBB_M-125"] =  2.20;
xsec["GluGluHToBB_M-125"] = 25.69;
xsec["QCD_HT100to200"] = 2.75E07; 
xsec["QCD_HT200to300"] = 1.74E06 ; 
xsec["QCD_HT300to500"] = 3.67E05; 
xsec["QCD_HT500to700"] = 2.94E04; 
xsec["QCD_HT700to1000"] = 6.52E03;
xsec["QCD_HT1000to1500"] = 1.064E03;
xsec["QCD_HT1500to2000"] =  121.5;
xsec["QCD_HT2000toInf"] = 2.54E01;
xsec["TT"] = 831.76;
xsec["TT_madgraph"] = 831.76;
xsec["TT_powheg"] = 831.76;
xsec["ST_tW"] = 71.7 ;
xsec["ST_s-channel"] = 3.344 ;
xsec["DYJetsToQQ"] = 1461.02;
xsec["WJetsToQQ"] = 3539.25 ;
xsec["DYJetsToLL"] = 6025.2;
xsec["WJetsToLNu"]  = 61526.7;
xsec["ttHTobb_M125"]  = .295;
xsec["ttHToNonbb_M125"]  = .2117 ;
xsec["ZH_HToBB"] = 3.093083e-01; //to qq
xsec["WplusH_HToBB"] = 3.057600e-01; 
xsec["WminusH_HToBB"] = 1.940120e-01;
xsec["WW"] = 118.7 ;
xsec["ZZ"] = 16.523 ;
xsec["WZ"] = 47.13;
xsec["bbHSusy120"] = 0.284;
xsec["bbHToBB_yb2"] = 0.3104;
xsec["bbHToBB_ybyt"] = 0.0262;
xsec["VBFHToBB_M-125_amc"] = 2.20;
xsec["GluGluHToBB_M-125_amc"] = 25.69;



Int_t NCATS;
Float_t cats[10];

int set_type;
if (dataset_type.CompareTo("double")==0) set_type=0;
if (dataset_type.CompareTo("single")==0) set_type=1;
int whichTrigWeight;
if ((trigWeight_str.CompareTo("none")==0)||(trigWeight_str.CompareTo("nom")==0)) whichTrigWeight=0;
if (trigWeight_str.CompareTo("up")==0) whichTrigWeight=1;
if (trigWeight_str.CompareTo("down")==0) whichTrigWeight=2;
if (set_type==0) NCATS=4;  //3+1
else if (set_type==1) NCATS=5;//
if (set_type==1) {
	Float_t cats_new[10]={-1.,0.28  , 0.72 ,  0.87,  0.93    ,1. };
	memcpy(cats, cats_new, sizeof cats_new);
}
else if (set_type==0) { 
	Float_t cats_new[10]={-1.,0.36 ,  0.76,   0.89   ,  1. };
	memcpy(cats, cats_new, sizeof cats_new);
}
Float_t LHE_weights_pdf_wgt[103];
     


	
	Float_t presel=0;
	Float_t presel_vtype[10] = {0,0,0,0,0,0,0,0,0};
	Float_t pos_puweight=0;
	Float_t all_puweight=0.;
	Float_t puweight;
	Float_t PU=1.;
	Float_t genweight;
	Float_t bTagWeight;
	Float_t genweight0;
	float  trigWeight_tree;
	Int_t global_counter = 0;
	Int_t HLT_QuadPFJet_DoubleBTag_CSV_VBF_Mqq200;
	Int_t HLT_QuadPFJet_SingleBTag_CSV_VBF_Mqq460;
	TFile *file_initial;
	TChain *tree_initial;

//	TString path = "/shome/nchernya/Hbb/skim_trees/v14/";
//	file_initial = TFile::Open(path+file_names[files]+"_v14"+dataset_type[set_type]+"/"+file_names[files]+file_postfix[set_type]+dataset_type[set_type]+".root");

/////////////////qgd//////////////
	TString path ;
//	path = "~/Hbb/slim_trees/v14_slimmed_bdt/";
//	file_initial =TFile::Open(path+file_names[files]+dataset_type[set_type]+".root");
	
//	path = "dcap://t3se01.psi.ch:22125//pnfs/psi.ch/cms/trivcat///store/user/nchernya/Hbb/v14/main_tmva/v14_last/main_mva_v14_";
	//path = "dcap://t3se01.psi.ch:22125//pnfs/psi.ch/cms/trivcat///store/user/nchernya/Hbb/v14/main_tmva/new5jet_float_final/main_mva_v14_";
//	path= "dcap://t3se01.psi.ch:22125//pnfs/psi.ch/cms/trivcat///store/user/nchernya/Hbb/v14/qgd/";
//	if (set_type==0)	file_initial = TFile::Open(path+file_names[files]+dataset_type[set_type]+".root");
//	if (set_type==1) file_initial = TFile::Open(path+"qgd_"+file_names[files]+dataset_type[set_type]+"_2jets"+".root");
//	file_initial=TFile::Open(path+file_names[files]+dataset_type[set_type]+".root");
//	if ((files==9)||(files==8)) file_initial=TFile::Open("/shome/nchernya/Hbb/slim_trees/SysUnc/JECR/main_mva_v14_"+file_names[files]+dataset_type[set_type]+".root");



	//if (files==18) file_initial=TFile::Open("/shome/nchernya/Hbb/skim_trees/v14/vbf_76"+dataset_type[set_type]+"/"+file_names[files]+"_v14"+dataset_type[set_type]+".root");


//	path= "dcap://t3se01.psi.ch:22125//pnfs/psi.ch/cms/trivcat///store/user/nchernya/Hbb/v14/qgd/";
//	if (set_type==0)	file_initial = TFile::Open(path+file_names[files]+dataset_type[set_type]+".root");
//	if (set_type==1) file_initial = TFile::Open(path+"qgd_"+file_names[files]+dataset_type[set_type]+"_2jets"+".root");
////////////////////////////

	file_initial = TFile::Open(file_name);


	
	tree_initial = (TChain*)file_initial->Get("tree");
	Int_t events_generated;
	TH1F *countPos;
	TH1F *countNeg;
	TH1F *countLHEPdf;
	if ((data!=1)){
		countPos = (TH1F*)file_initial->Get("CountPosWeight");
 		countNeg = (TH1F*)file_initial->Get("CountNegWeight");
		countLHEPdf=	(TH1F*)file_initial->Get("CountWeightedLHEWeightPdf");
 		events_generated = countPos->GetEntries()-countNeg->GetEntries();
	} else events_generated = 1;
    Jets Jet;
    Float_t v_type;
    Float_t wrong_type=0.;
    Int_t nJets;
	Float_t JSON;	
	Float_t nPVs;	
	Float_t rho;	

	Float_t bdt;
	Float_t met_pt;

	Jets GenHiggsSisters;

	int pos_weight_presel=0;
 	Int_t selLeptons_tightId[20];
	Float_t selLeptons_relIso03[20] , selLeptons_chargedHadRelIso03[20], selLeptons_pfRelIso03[20];
	Float_t vLeptons_dz[20], vLeptons_edz[20];



    tree_initial->SetBranchAddress("Vtype",&v_type);
    tree_initial->SetBranchAddress("rho",&rho);
    tree_initial->SetBranchAddress("nJet",&nJets);
    tree_initial->SetBranchAddress("Jet_pt",Jet.pt);
    tree_initial->SetBranchAddress("Jet_corr_JECUp",Jet.JEC_corr_up);
    tree_initial->SetBranchAddress("Jet_corr_JECDown",Jet.JEC_corr_down);
    tree_initial->SetBranchAddress("Jet_corr",Jet.JEC_corr);
    tree_initial->SetBranchAddress("Jet_corr_JERUp",Jet.JER_corr_up);
    tree_initial->SetBranchAddress("Jet_corr_JERDown",Jet.JER_corr_down);
    tree_initial->SetBranchAddress("Jet_corr_JER",Jet.JER_corr);
    tree_initial->SetBranchAddress("Jet_eta",Jet.eta);
    tree_initial->SetBranchAddress("Jet_phi",Jet.phi);
	tree_initial->SetBranchAddress("Jet_mass",Jet.mass);
	tree_initial->SetBranchAddress("Jet_btagCSV",Jet.btag);
	tree_initial->SetBranchAddress("Jet_blike_VBF",Jet.blike_VBF);
	tree_initial->SetBranchAddress("Jet_id",Jet.id);	
	tree_initial->SetBranchAddress("Jet_puId",Jet.puId);
 	tree_initial->SetBranchAddress("Jet_leadTrackPt",Jet.leadTrackPt);
	tree_initial->SetBranchAddress("met_pt",&met_pt);
	
	tree_initial->SetBranchAddress("softActivityJets_pt",Jet.soft_pt);
	tree_initial->SetBranchAddress("softActivityJets_eta",Jet.soft_eta);
	tree_initial->SetBranchAddress("softActivityJets_mass",Jet.soft_mass);
	tree_initial->SetBranchAddress("softActivity_HT",&Jet.HTsoft);
	tree_initial->SetBranchAddress("softActivity_njets2",&Jet.nsoft2);
	tree_initial->SetBranchAddress("softActivity_njets5",&Jet.nsoft5);
	tree_initial->SetBranchAddress("softActivity_njets10",&Jet.nsoft10);
	tree_initial->SetBranchAddress("Jet_qgl",Jet.qgl);
	tree_initial->SetBranchAddress("genWeight",&genweight);
	tree_initial->SetBranchAddress("bTagWeight",&bTagWeight);
	tree_initial->SetBranchAddress("puWeight",&puweight);
	tree_initial->SetBranchAddress("nPVs",&nPVs);
	tree_initial->SetBranchAddress("Jet_ptd",Jet.ptd);
	tree_initial->SetBranchAddress("Jet_axis2",Jet.axis2);
	tree_initial->SetBranchAddress("Jet_mult",Jet.mult);
 	tree_initial->SetBranchAddress("HLT_BIT_HLT_QuadPFJet_DoubleBTagCSV_VBF_Mqq200_v",&HLT_QuadPFJet_DoubleBTag_CSV_VBF_Mqq200);
 	tree_initial->SetBranchAddress("HLT_BIT_HLT_QuadPFJet_SingleBTagCSV_VBF_Mqq460_v",&HLT_QuadPFJet_SingleBTag_CSV_VBF_Mqq460);
	tree_initial->SetBranchAddress("Jet_pt_regVBF",Jet.pt_regVBF);
	tree_initial->SetBranchAddress("json",&JSON);
    
	tree_initial->SetBranchAddress("GenHiggsSisters_pt",GenHiggsSisters.pt);
    tree_initial->SetBranchAddress("GenHiggsSisters_eta",GenHiggsSisters.eta);
    tree_initial->SetBranchAddress("GenHiggsSisters_phi",GenHiggsSisters.phi);
	tree_initial->SetBranchAddress("GenHiggsSisters_mass",GenHiggsSisters.mass);

	tree_initial->SetBranchAddress("selLeptons_tightId",selLeptons_tightId);
	tree_initial->SetBranchAddress("selLeptons_relIso03",selLeptons_relIso03);
	tree_initial->SetBranchAddress("selLeptons_chargedHadRelIso03",selLeptons_chargedHadRelIso03);
	tree_initial->SetBranchAddress("selLeptons_pfRelIso03",selLeptons_pfRelIso03);
	tree_initial->SetBranchAddress("vLeptons_dz",vLeptons_dz);
	tree_initial->SetBranchAddress("vLeptons_edz",vLeptons_edz);

	tree_initial->SetBranchAddress("Jet_qgl1_VBF",Jet.qgl1_VBF);
	tree_initial->SetBranchAddress("Jet_qgl2_VBF",Jet.qgl2_VBF);

	tree_initial->SetBranchAddress("BDT_VBF",&bdt);
	tree_initial->SetBranchAddress("trigWeight",&trigWeight_tree);
	tree_initial->SetBranchAddress("LHE_weights_pdf_wgt",LHE_weights_pdf_wgt);



	if (data==1){
		genweight = 1.;
		bTagWeight = 1.;
		puweight=1.;
	}

 	
	TH1F *hist_acceptance[10];
	float mean_acceptance[10];
	
	if ((file_tag.CompareTo("VBFHToBB_M-125")==0)&&(set_type==0)) {
		Float_t mean_acceptance_new[10]={31.,23,11,4,69. };
		memcpy(mean_acceptance, mean_acceptance_new, sizeof mean_acceptance_new);
	}
	if ((file_tag.CompareTo("GluGluHToBB_M-125")==0)&&(set_type==0)) {
		Float_t mean_acceptance_new[10]={121.,20,3,0.65 };
		memcpy(mean_acceptance, mean_acceptance_new, sizeof mean_acceptance_new);
	}
	if ((file_tag.CompareTo("VBFHToBB_M-125")==0)&&(set_type==1)) {
		Float_t mean_acceptance_new[10]={20,20,14,7,4 };
		memcpy(mean_acceptance, mean_acceptance_new, sizeof mean_acceptance_new);
	}
	if ((file_tag.CompareTo("GluGluHToBB_M-125")==0)&&(set_type==1)) {
		Float_t mean_acceptance_new[10]={12,5.79,1.85,0.64,0.22,20.71 };
		memcpy(mean_acceptance, mean_acceptance_new, sizeof mean_acceptance_new);
	}


	for (int i=0;i<NCATS;i++){
		TString tmp;
		hist_acceptance[i] = new TH1F("acceptance_cat"+tmp.Format("%d",i),"",60,mean_acceptance[i]*0.85,mean_acceptance[i]*1.15);
		hist_acceptance[i]->GetXaxis()->SetTitle("acceptance in CAT"+tmp.Format("%d",i));
	}

	
	float qq_matching = 0;
	float qq_matching_all = 0;
	

	int nentries = tree_initial->GetEntries() ;
	
//	int nentries30;
//	int tmp_int;
//	float tmp_fl = modf(0.3*nentries; &tmp_int);

	TF1 *func_r = new TF1("erffunc",erf,0.,1000.,3);


for (int  pdf_counter=-1;pdf_counter<100;pdf_counter++){
Float_t events_in_cats[10] = {0,0,0,0,0,0,0,0,0,0};
Float_t events_in_cats_er[10] = {0,0,0,0,0,0,0,0,0,0};


	for (int entry=0; entry<nentries;++entry){
	//for (int entry=0; entry<30;++entry){
        tree_initial->GetEntry(entry);
		

		if (JSON!=1) {
			continue;
		}

		int good_jets = 0;
		vector<TLorentzVector> jets_pv;
		for (int i=0;i<nJets;i++){
			TLorentzVector jet0;
			if (!((Jet.id[i]>2)&&(Jet.puId[i]>0))) continue;
			jet0.SetPtEtaPhiM(Jet.pt[i],Jet.eta[i],Jet.phi[i],Jet.mass[i]);
			jets_pv.push_back(jet0);
			good_jets++;
		}

		if (good_jets<4)continue;

		if (region.CompareTo("analysis")==0) {
			if (!((v_type==-1)||(v_type>3))) continue;
	
		} 
		else if (region.CompareTo("controlTop")==0) {
			if (!((v_type==2)||(v_type==3))) continue;
			if (met_pt<50) continue;
			if (!(selLeptons_relIso03[0]<0.08)) continue;
	//		if (!(selLeptons_chargedHadRelIso03[0]<0.05)) continue;
		}
		else if (region.CompareTo("controlDY")==0) {
			if (!((v_type==0)||(v_type==1))) continue;
			if (!((selLeptons_tightId[0]==1)||(selLeptons_tightId[0]==3))) continue;
			if (!((selLeptons_tightId[1]==1)||(selLeptons_tightId[1]==3))) continue;
		}
		

		if (data==1) PU=1.;
		else PU=puweight;
		genweight0 = genweight/TMath::Abs(genweight);
		genweight=genweight/TMath::Abs(genweight)*PU*bTagWeight;
		if (pdf_counter!=-1) genweight/=countLHEPdf->GetBinContent( countLHEPdf->FindBin(pdf_counter))/xsec[file_tag];
		else  genweight/=events_generated/xsec[file_tag];

		


		int btag_max1_number = -1;
		int btag_max2_number = -1;
		int pt_max1_number = -1;
		int pt_max2_number = -1;
		TLorentzVector Bjet1;
		TLorentzVector Bjet2;
		TLorentzVector Qjet1;
		TLorentzVector Qjet2;
		TLorentzVector qq;
		
		int flag=0;
		
		if (set_type==0) {
			if (preselection_single_blike(nJets, Jet.pt,Jet.eta, Jet.phi, Jet.mass, Jet.blike_VBF, Jet.id, Jet.puId, btag_max1_number, btag_max2_number, pt_max1_number, pt_max2_number, HLT_QuadPFJet_SingleBTag_CSV_VBF_Mqq460, Bjet1, Bjet2, Qjet1, Qjet2, qq) == 0) continue;
			if (preselection_double(nJets, Jet.pt,Jet.eta, Jet.phi, Jet.mass, Jet.btag, Jet.id, Jet.puId, btag_max1_number, btag_max2_number, pt_max1_number, pt_max2_number, HLT_QuadPFJet_DoubleBTag_CSV_VBF_Mqq200, Bjet1, Bjet2, Qjet1, Qjet2, qq) != 0) continue;
		}
		else {
			if (set_type==1) if (preselection_single_blike(nJets, Jet.pt,Jet.eta, Jet.phi, Jet.mass, Jet.blike_VBF, Jet.id, Jet.puId, btag_max1_number, btag_max2_number, pt_max1_number, pt_max2_number, HLT_QuadPFJet_SingleBTag_CSV_VBF_Mqq460, Bjet1, Bjet2, Qjet1, Qjet2, qq) != 0) continue;
		}

	

		int matched_qq = 0;
		for (int i=0;i<nJets;i++){
			if (!(((Jet.id[i]>0)||(TMath::Abs(Jet.eta[i])>3))&& (Jet.pt[i]>20) )) continue;
			if (!((Jet.id[i]>2)&&(Jet.puId[i]>0))) continue;
			TLorentzVector hJ0;
		   hJ0.SetPtEtaPhiM(Jet.pt[i],Jet.eta[i],Jet.phi[i],Jet.mass[i]);	
		   for(int j =0; j<2; j++){
				TLorentzVector hQ;
				hQ.SetPtEtaPhiM(GenHiggsSisters.pt[j],GenHiggsSisters.eta[j],GenHiggsSisters.phi[j],GenHiggsSisters.mass[j]);
				if(hQ.DeltaR(hJ0)<0.8) matched_qq++;
		   }	
		} 
		if (matched_qq>=2) qq_matching++;
		qq_matching_all++;	



		Float_t Mqq = qq.M();
		Float_t bbDeltaPhi = TMath::Abs(Bjet1.DeltaPhi(Bjet2));
		Float_t qqDeltaEta = TMath::Abs(Qjet1.Eta()-Qjet2.Eta());
		Float_t qqDeltaPhi = TMath::Abs(Qjet1.DeltaPhi(Qjet2));


////////////////////////////
/*
		vector<TLorentzVector> jets;
		vector<Float_t> jets_btag;
		for (int i=0;i<4;i++){
			TLorentzVector jet0;
			jet0.SetPtEtaPhiM(Jet.pt[i],Jet.eta[i],Jet.phi[i],Jet.mass[i]);
			jets.push_back(jet0);
		}
		SortByEta(jets);
		TLorentzVector Qjet1_eta = jets[0];	
		TLorentzVector Qjet2_eta = jets[3];
		jets.clear();
		Float_t Mqq_eta=(Qjet1_eta+Qjet2_eta).M();	
		Float_t Detaqq_eta = TMath::Abs(Qjet1_eta.Eta()-Qjet2_eta.Eta());
		for (int i=0;i<nJets;i++){
			if (Jet.pt[i]>30)	jets_btag.push_back(Jet.btag[i]);
		}
		std::sort(jets_btag.begin(), jets_btag.end()); 
		std::reverse(jets_btag.begin(), jets_btag.end()); 
		Float_t CSV1;
		Float_t CSV2;
		if (jets_btag[0]>1.) CSV1 = 1.;
		if (jets_btag[0]<0.) CSV1 = 0.;
		else CSV1=jets_btag[0];
		if (jets_btag[1]>1.) CSV2 = 1.;
		if (jets_btag[1]<0.) CSV2 = 0.;
		else CSV2=jets_btag[1];*/

////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////triggerWeightApplicationBegin///////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////
		



		float trigWeight = 1.;
/*
		float trigCor1_nom_single[10] = {8.24458e+01,8.08404e+01, 5.58251e+01};
		float trigCor2_nom_single[10] = { 2.90000e+01, 1.59732e+01, 2.90000e+01};
		float trigCor1_up_single[10] = {7.82700e+01,8.06390e+01 ,5.70699e+01};
		float trigCor2_up_single[10] = {3.48000e+01,1.91678e+01,3.48000e+01  };
		float trigCor1_down_single[10] = {8.71164e+01,8.13242e+01,5.68139e+01 };
		float trigCor2_down_single[10] = {2.75500e+01,1.27785e+01,2.32000e+01 };

		float trigCor1_nom_double[10] = {7.60656e+01, 7.21789e+01,4.73122e+01 };
		float trigCor2_nom_double[10] = {2.90000e+01,1.92332e+01 , 2.90000e+01};
		float trigCor1_up_double[10] = {6.88413e+01,7.06364e+01,4.56501e+01 };
		float trigCor2_up_double[10] = {3.48000e+01,2.30799e+01,3.48000e+01};
		float trigCor1_down_double[10] = {8.09108e+01,7.36682e+01,4.80169e+01 };
		float trigCor2_down_double[10] = {2.75500e+01,  1.53866e+01,2.75500e+01 };
*/

		float trigCor0_nom_single[10] = {1.,1.,1., 1.17774e+00  ,1.};
		float trigCor1_nom_single[10] = {8.24458e+01,7.95354e+01,5.29763e+01, -3.93424e+01 };
		float trigCor2_nom_single[10] = {2.90000e+01,1.45915e+01,1.62510e+01,9.49616e+00};
	//	float trigCor0_up_single[10] = {1.,1.,1., 1.33815e+00  ,1.};
		float trigCor0_up_single[10] = {1.,1.,1.,1.24684620  ,1.};
	//	float trigCor1_up_single[10] = {7.82700e+01,7.89997e+01,4.94856e+01, -4.68187e+01};
//		float trigCor2_up_single[10] = {3.48000e+01, 1.75098e+01,1.95012e+01 ,1.02166e+01   };
		float trigCor1_up_single[10] = {6.18344e+01, 7.15819e+01,4.30640e+01,                -4.68187e+01};
		float trigCor2_up_single[10] = {3.04500e+01, 1.53211e+01,1.70635e+01,                1.02166e+01   };
//		float trigCor0_down_single[10] = {1.,1.,1., 1.01732e+00  ,1. };
		float trigCor0_down_single[10] = {1.,1.,1.,1.1086338  ,1. };
//		float trigCor1_down_single[10] = {8.71164e+01,8.03130e+01,5.61926e+01,-3.95425e+01  };
//		float trigCor2_down_single[10] = {2.75500e+01,1.16732e+01,1.30008e+01,8.19539e+00 };
		float trigCor1_down_single[10] = {1.00420e+02,8.74889e+01,6.10755e+01        ,-3.95425e+01  };
		float trigCor2_down_single[10] = {2.75500e+01,1.16732e+01,1.30008e+01    ,8.19539e+00 };

		float trigCor0_nom_double[10] = {1.,1.,1., 1. , 1.08783e+00  };
		float trigCor1_nom_double[10] = {7.60656e+01,7.14527e+01,3.78975e+01 ,-2.94747e+00,-3.07239e+01 };
		float trigCor2_nom_double[10] = {2.90000e+01,1.68348e+01,3.71160e+01 ,1.00767e+00, 6.52856e+00 };
		float trigCor0_up_double[10] = {1.,1.,1.,1.,1.14223e+00   };
//		float trigCor1_up_double[10] = {6.88413e+01,6.98489e+01,3.45193e+01 ,  -3.53439e+00,-2.92504e+01  };
//		float trigCor2_up_double[10] = {3.48000e+01, 2.02018e+01 ,4.45392e+01, 1.06560e+00,6.89496e+00  };
		float trigCor1_up_double[10] = {5.70492e+01 ,6.43074e+01, 2.84231e+01,              -3.53439e+00,-2.92504e+01  };
		float trigCor2_up_double[10] = {3.04500e+01, 1.92630e+01, 3.89718e+01,              1.06560e+00,6.89496e+00  };
		float trigCor0_down_double[10] = {1.,1.,1., 1.,1.03344e+00   };
//		float trigCor1_down_double[10] = {8.09108e+01, 7.29042e+01,4.24637e+01,-2.81353e+00, -1.06603e+03  };
//		float trigCor2_down_double[10] = {2.75500e+01,1.34679e+01 ,2.96928e+01, 8.52894e-01, 5.34791e+00};
		float trigCor1_down_double[10] = {8.88824e+01,7.85980e+01,4.70174e+01,             -2.81353e+00, -1.06603e+03  };
		float trigCor2_down_double[10] = {2.75500e+01,1.34678e+01,3.52602e+01,             8.52894e-01, 5.34791e+00};
	
		func_r->FixParameter(0,1.);
		if ((set_type==0)&&(data!=1)) {
			if (whichTrigWeight==0) func_r->FixParameter(1,trigCor1_nom_double[0] );
			if (whichTrigWeight==0) func_r->FixParameter(2,trigCor2_nom_double[0] );
			if (whichTrigWeight==1) func_r->FixParameter(1,trigCor1_up_double[0] );
			if (whichTrigWeight==1) func_r->FixParameter(2,trigCor2_up_double[0] );
			if (whichTrigWeight==2) func_r->FixParameter(1,trigCor1_down_double[0] );
			if (whichTrigWeight==2) func_r->FixParameter(2,trigCor2_down_double[0] );
			trigWeight*=func_r->Eval(jets_pv[0].Pt());
		}
		if ((set_type==0)&&(data!=1)) {
			if (whichTrigWeight==0) func_r->FixParameter(1,trigCor1_nom_double[1] );
			if (whichTrigWeight==0) func_r->FixParameter(2,trigCor2_nom_double[1] );
			if (whichTrigWeight==1) func_r->FixParameter(1,trigCor1_up_double[1] );
			if (whichTrigWeight==1) func_r->FixParameter(2,trigCor2_up_double[1] );
			if (whichTrigWeight==2) func_r->FixParameter(1,trigCor1_down_double[1] );
			if (whichTrigWeight==2) func_r->FixParameter(2,trigCor2_down_double[1] );
			trigWeight*=func_r->Eval(jets_pv[1].Pt());
		}
		if ((set_type==0)&&(data!=1)) {
			if (whichTrigWeight==0) func_r->FixParameter(1,trigCor1_nom_double[2] );
			if (whichTrigWeight==0) func_r->FixParameter(2,trigCor2_nom_double[2] );
			if (whichTrigWeight==1) func_r->FixParameter(1,trigCor1_up_double[2] );
			if (whichTrigWeight==1) func_r->FixParameter(2,trigCor2_up_double[2] );
			if (whichTrigWeight==2) func_r->FixParameter(1,trigCor1_down_double[2] );
			if (whichTrigWeight==2) func_r->FixParameter(2,trigCor2_down_double[2] );
			trigWeight*=func_r->Eval(jets_pv[2].Pt());
		}
		if ((set_type==0)&&(data!=1)){
			if (whichTrigWeight==0) func_r->FixParameter(1,trigCor1_nom_double[3] );
			if (whichTrigWeight==0) func_r->FixParameter(2,trigCor2_nom_double[3] );
			if (whichTrigWeight==1) func_r->FixParameter(1,trigCor1_up_double[3] );
			if (whichTrigWeight==1) func_r->FixParameter(2,trigCor2_up_double[3] );
			if (whichTrigWeight==2) func_r->FixParameter(1,trigCor1_down_double[3] );
			if (whichTrigWeight==2) func_r->FixParameter(2,trigCor2_down_double[3] );
			trigWeight*=func_r->Eval(-1.*TMath::Log(1.-Jet.btag[btag_max1_number]));
		}
		if ((set_type==0)&&(data!=1)){
			if (whichTrigWeight==0) func_r->FixParameter(0,trigCor0_nom_double[4] );
			if (whichTrigWeight==0) func_r->FixParameter(1,trigCor1_nom_double[4] );
			if (whichTrigWeight==0) func_r->FixParameter(2,trigCor2_nom_double[4] );
			if (whichTrigWeight==1) func_r->FixParameter(0,trigCor0_up_double[4] );
			if (whichTrigWeight==1) func_r->FixParameter(1,trigCor1_up_double[4] );
			if (whichTrigWeight==1) func_r->FixParameter(2,trigCor2_up_double[4] );
			if (whichTrigWeight==2) func_r->FixParameter(0,trigCor0_down_double[4] );
			if (whichTrigWeight==2) func_r->FixParameter(1,trigCor1_down_double[4] );
			if (whichTrigWeight==2) func_r->FixParameter(2,trigCor2_down_double[4] );
			trigWeight*=func_r->Eval(-1.*TMath::Log(1.-Jet.btag[btag_max2_number]));
		}
//////////////////////

		func_r->FixParameter(0,1.);

		if ((set_type==1)&&(data!=1)){
			if (whichTrigWeight==0) func_r->FixParameter(1,trigCor1_nom_single[0] );
			if (whichTrigWeight==0) func_r->FixParameter(2,trigCor2_nom_single[0] );
			if (whichTrigWeight==1) func_r->FixParameter(1,trigCor1_up_single[0] );
			if (whichTrigWeight==1) func_r->FixParameter(2,trigCor2_up_single[0] );
			if (whichTrigWeight==2) func_r->FixParameter(1,trigCor1_down_single[0] );
			if (whichTrigWeight==2) func_r->FixParameter(2,trigCor2_down_single[0] );
			trigWeight*=func_r->Eval(jets_pv[0].Pt());
		}
		if ((set_type==1)&&(data!=1)){
			if (whichTrigWeight==0) func_r->FixParameter(1,trigCor1_nom_single[1] );
			if (whichTrigWeight==0) func_r->FixParameter(2,trigCor2_nom_single[1] );
			if (whichTrigWeight==1) func_r->FixParameter(1,trigCor1_up_single[1] );
			if (whichTrigWeight==1) func_r->FixParameter(2,trigCor2_up_single[1] );
			if (whichTrigWeight==2) func_r->FixParameter(1,trigCor1_down_single[1] );
			if (whichTrigWeight==2) func_r->FixParameter(2,trigCor2_down_single[1] );
			trigWeight*=func_r->Eval(jets_pv[1].Pt());
		}
		if ((set_type==1)&&(data!=1)){
			if (whichTrigWeight==0) func_r->FixParameter(1,trigCor1_nom_single[2] );
			if (whichTrigWeight==0) func_r->FixParameter(2,trigCor2_nom_single[2] );
			if (whichTrigWeight==1) func_r->FixParameter(1,trigCor1_up_single[2] );
			if (whichTrigWeight==1) func_r->FixParameter(2,trigCor2_up_single[2] );
			if (whichTrigWeight==2) func_r->FixParameter(1,trigCor1_down_single[2] );
			if (whichTrigWeight==2) func_r->FixParameter(2,trigCor2_down_single[2] );
			trigWeight*=func_r->Eval(jets_pv[2].Pt());
		} 
		if ((set_type==1)&&(data!=1)){
			if (whichTrigWeight==0) func_r->FixParameter(0,trigCor0_nom_single[3] );
			if (whichTrigWeight==0) func_r->FixParameter(1,trigCor1_nom_single[3] );
			if (whichTrigWeight==0) func_r->FixParameter(2,trigCor2_nom_single[3] );
			if (whichTrigWeight==1) func_r->FixParameter(0,trigCor0_up_single[3] );
			if (whichTrigWeight==1) func_r->FixParameter(1,trigCor1_up_single[3] );
			if (whichTrigWeight==1) func_r->FixParameter(2,trigCor2_up_single[3] );
			if (whichTrigWeight==2) func_r->FixParameter(0,trigCor0_down_single[3] );
			if (whichTrigWeight==2) func_r->FixParameter(1,trigCor1_down_single[3] );
			if (whichTrigWeight==2) func_r->FixParameter(2,trigCor2_down_single[3] );
			trigWeight*=func_r->Eval(-1.*TMath::Log(1.-Jet.btag[btag_max1_number]));
		} 
//////////////////////
//////////////////////
//		TF1 *func_r_2 = new TF1("erffunc2",erf2,0.,1000.,3);
//		if ((set_type==1)&&(data!=1)){
//			func_r_2->FixParameter(0,4.45514e-01);
//			func_r_2->FixParameter(1,2.12454e+00);
//			func_r_2->FixParameter(2,6.29383e-01);
//			trigWeight*=func_r_2->Eval(-1.*TMath::Log(1.-Jet.btag[btag_max1_number]));
//		}

	//	if (trigWeight!=trigWeight_tree) cout<<trigWeight <<  "     "<<trigWeight_tree<<endl;  
		if (applyTrigWeight==1) genweight*=trigWeight;

		if (pdf_counter!=-1) genweight*=LHE_weights_pdf_wgt[pdf_counter];
		

////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////triggerWeightApplicationEnd/////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////

		presel+=genweight;

		presel_vtype[(int)(v_type+1)]+=genweight;
	

		for (int i=0;i<NCATS;i++){
			if  ((bdt>cats[i])&&(bdt<=cats[i+1])) events_in_cats[i] += genweight;
			if  ((bdt>cats[i])&&(bdt<=cats[i+1])) events_in_cats_er[i] += genweight0;
		}

				
   }



		for (int i=0;i<NCATS;i++){
			if (data!=1) events_in_cats[i]*=2320.;
		}
		for (int i=0;i<NCATS;i++){	
			hist_acceptance[i]->Fill(events_in_cats[i]);
		}

}

	
		TFile file(output+"/"+file_tag+"_"+region+"_"+dataset_type+"_trig"+trigWeight_str+"_"+heppyVersion+"_"+postfix+"_pdf_unc.root","recreate");
		for (int i=0;i<NCATS;++i){
    	    	hist_acceptance[i]->SetLineWidth(2);
    	   	hist_acceptance[i]->GetYaxis()->SetTitle("N_{events}");
       		hist_acceptance[i]->GetYaxis()->SetTitleFont(42);
       		hist_acceptance[i]->GetYaxis()->SetTitleSize(0.060);
        		hist_acceptance[i]->GetYaxis()->SetTitleOffset(0.8);
        		hist_acceptance[i]->SetLineColor(kBlue);
        		hist_acceptance[i]->Draw();
        		hist_acceptance[i]->Write();
   		} 

    		file.Write();
    		file.Close();



return 0;
    
}
