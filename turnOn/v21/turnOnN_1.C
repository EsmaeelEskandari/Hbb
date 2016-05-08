#include <ctype.h>
#include <iomanip>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "TFile.h"
#include "TH1.h"
#include "TH1F.h"
#include "TH2.h"
#include "TH2F.h"
#include "TTree.h"
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
//#include "/afs/cern.ch/work/n/nchernya/Hbb/preselection_double_n_4.C"
//#include "/afs/cern.ch/work/n/nchernya/Hbb/preselection_single_n_4.C"
#include "preselection_double_n_4.C"
#include "preselection_single_n_4.C"


Double_t erf( Double_t *x, Double_t *par){
  return par[0]/2.*(1.+TMath::Erf((x[0]-par[1])/par[2]));
}

const int njets = 300;

typedef struct {
   Float_t eta[njets];
   Float_t pt[njets];
   Float_t phi[njets];
	Float_t mass[njets];
	Float_t btag[njets];
	Float_t blike_VBF[njets];
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
} Jets;

using namespace std;

//void turnOnN_1(int files, int files_max, int set_type, int trigger){
int main(int argc, char* argv[]){  

int files=atoi(argv[1]);
int files_max=atoi(argv[2]);
int set_type = atoi(argv[3]);
int trigger=atoi(argv[4]);
 
//set_type = 0 double, set_type = 1 single
TString type[2] = {"_double","_single"};


const int nfiles  = 20;
int data=0;

TString path = "dcap://t3se01.psi.ch:22125//pnfs/psi.ch/cms/trivcat///store/user/nchernya/Hbb/v21/skimmed/trig/";
TString v20="_v20";
   
//// {"QCD_HT100to200", "QCD_HT200to300", "QCD_HT300to500","QCD_HT500to700", "QCD_HT700to1000", "QCD_HT1000to1500", "QCD_HT1500to2000", "QCD_HT2000toInf", "VBFHToBB_M-125_13TeV_powheg", "VBFHToBB_M-130_13TeV_powheg", "VBFHToBB_M125_13TeV_amcatnlo", "data_BTagCSV"};
Double_t xsec[nfiles] = {1.,/*1.,1.,*/ 2.75E07, 1.74E06,  3.67E05, 2.94E04, 6.52E03,1.064E03,   121.5,  2.54E01};

TString file_names[nfiles] = {/*"JetHT_C",*/"JetHT_D"/*,"JetHT_D"*/,"QCD_HT100to200", "QCD_HT200to300", "QCD_HT300to500","QCD_HT500to700", "QCD_HT700to1000", "QCD_HT1000to1500", "QCD_HT1500to2000", "QCD_HT2000toInf"};
do {

	data=0;
	if (files<1) data=1;
	
	Float_t presel=0;
	Float_t cut1 = 0;
	Float_t cut2 = 0;
	Float_t cut3 = 0;
	Float_t genweight=1.;
	Float_t BTagWeight=1.;
	Int_t global_counter = 0;
	Int_t HLT_QuadPFJet_DoubleBTag_CSV_VBF_Mqq200;
	Int_t HLT_QuadPFJet_SingleBTag_CSV_VBF_Mqq460;

	Int_t HLT_DiPFJetAve60;
	Int_t HLT_DiPFJetAve40;
	Int_t HLT_PFJet60;
	Int_t HLT_L1_TripleJet_VBF;
	Int_t HLT_QuadPFJet_VBF;

	TString denominator_trigger_names[4] = { "HLT_BIT_HLT_QuadPFJet_VBF_v","HLT_BIT_HLT_DiPFJetAve60_v","HLT_BIT_HLT_DiPFJetAve40_v","HLT_BIT_HLT_DiPFJetAve40or60_v"};

	TFile *file_initial;
	file_initial = TFile::Open(path+file_names[files]+"_single_v21_trig.root");

//	if (data==1)file_initial = TFile::Open(path+file_names[files]+".root");
//	if (data!=1)file_initial = TFile::Open(path+file_names[files]+"_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.root");
   TTree *tree_initial = (TTree*)file_initial->Get("tree");
	TH1F *countPos;
	TH1F *countNeg;
	Float_t events_generated;
	if (data!=1){
		countPos = (TH1F*)file_initial->Get("CountPosWeight");
 		countNeg = (TH1F*)file_initial->Get("CountNegWeight");
 		events_generated = countPos->GetEntries()-countNeg->GetEntries();
	} else events_generated = 1;
    Jets Jet;
   Int_t nJets;
	float counter = 0.;
	float counter_tr1=0.;
	Float_t JSON = 0;


	

    tree_initial->SetBranchAddress("nJet",&nJets);
    tree_initial->SetBranchAddress("genWeight",&genweight);
    tree_initial->SetBranchAddress("bTagWeight",&BTagWeight);
    tree_initial->SetBranchAddress("Jet_pt",Jet.pt);
    tree_initial->SetBranchAddress("Jet_eta",Jet.eta);
    tree_initial->SetBranchAddress("Jet_phi",Jet.phi);
	tree_initial->SetBranchAddress("Jet_mass",Jet.mass);
	tree_initial->SetBranchAddress("Jet_btagCSV",Jet.btag);
	tree_initial->SetBranchAddress("Jet_blike_VBF",Jet.blike_VBF);
	tree_initial->SetBranchAddress("Jet_id",Jet.id);	
	tree_initial->SetBranchAddress("Jet_puId",Jet.puId);	
	tree_initial->SetBranchAddress("json",&JSON);

 	tree_initial->SetBranchAddress("HLT_BIT_HLT_QuadPFJet_DoubleBTagCSV_VBF_Mqq200_v",&HLT_QuadPFJet_DoubleBTag_CSV_VBF_Mqq200);
 	tree_initial->SetBranchAddress("HLT_BIT_HLT_QuadPFJet_SingleBTagCSV_VBF_Mqq460_v",&HLT_QuadPFJet_SingleBTag_CSV_VBF_Mqq460);

	tree_initial->SetBranchAddress("HLT_BIT_HLT_DiPFJetAve60_v",&HLT_DiPFJetAve60);
	tree_initial->SetBranchAddress("HLT_BIT_HLT_DiPFJetAve40_v",&HLT_DiPFJetAve40);
	tree_initial->SetBranchAddress("HLT_BIT_HLT_L1_TripleJet_VBF_v",&HLT_L1_TripleJet_VBF);
	tree_initial->SetBranchAddress("HLT_BIT_HLT_QuadPFJet_VBF_v",&HLT_QuadPFJet_VBF);




	Float_t jet1pt_binx[30] = {0,20,40,60,80,100,120,140,160,180,200,240,300,550};   
	Float_t jet2pt_binx[30] = {0,20,40,60,80,100,120,140,160,200,250,330,550};   
	Float_t jet3pt_binx[30] = {0,20,40,60,80,100,120,150,220,390,550};   
	Float_t jet4pt_binx[30] = {0,20,40,60,80,100,200};	
	Float_t mqq_bins[40] = {0,30,60,90,120,150,180,210,240,270,300,330,360,390,420,450,480,510,540,570,600,650,720,800,900,1100,1500,2600};   


 
    TH1F *hJet1_pt = new TH1F("hJet1_pt", "", 13, jet1pt_binx);
    hJet1_pt->GetXaxis()->SetTitle("1^{st} Jet p_{T} (GeV)");
    TH1F *hJet2_pt = new TH1F("hJet2_pt", "", 12, jet2pt_binx);
    hJet2_pt->GetXaxis()->SetTitle("2^{nd} Jet p_{T} (GeV)");
    TH1F *hJet3_pt = new TH1F("hJet3_pt", "", 9, jet3pt_binx);
    hJet3_pt->GetXaxis()->SetTitle("3^{rd} Jet p_{T} (GeV)");
    TH1F *hJet4_pt = new TH1F("hJet4_pt", "", 6, jet4pt_binx);
    hJet4_pt->GetXaxis()->SetTitle("4^{th} Jet p_{T} (GeV)");
	 TH1F *hMqq = new TH1F("hMqq","",27, mqq_bins);
	hMqq->GetXaxis()->SetTitle("m_{qq} (GeV)");
	TH1F *hbtag = new TH1F("hbtag","",10,0.,1.);
	hbtag->GetXaxis()->SetTitle("CSV 1^{st} b-jet"); 
	TH1F *hbtag2 = new TH1F("hbtag2","",10,0.,1.);
	hbtag2->GetXaxis()->SetTitle("CSV 2^{nd} b-jet");
	TH1F *hDeltaEta = new TH1F("hDeltaEta","",80.,0.,8.);
	hDeltaEta->GetXaxis()->SetTitle("|#Delta#eta_{qq}|");
	TH1F *hDeltaPhi = new TH1F("hDeltaPhi","",32,0.,3.2);
	hDeltaPhi->GetXaxis()->SetTitle("|#Delta#phi_{bb}|");
	hbtag2->GetXaxis()->SetTitle("CSV 2^{nd} b-jet");
	TH2F *hCSV = new TH2F("hCSV","",10,0.,10.,10.,0.,10);
	hCSV->GetXaxis()->SetTitle("-log(1-CSV) of 1^{st} b-jet");
	hCSV->GetYaxis()->SetTitle("-log(1-CSV) of 2^{nd} b-jet");
	TH2F *hCSV2 = new TH2F("hCSV2","",10,0.,1.,10.,0.,1);
	hCSV2->GetXaxis()->SetTitle("CSV of 1^{st} b-jet");
	hCSV2->GetYaxis()->SetTitle("CSV of 2^{nd} b-jet");
	TH1F *hbtag_log = new TH1F("hbtag_log","",20,0.,10.);
	hbtag_log->GetXaxis()->SetTitle("-log(1-CSV) 1^{st} b-jet"); 
	TH1F *hbtag2_log = new TH1F("hbtag2_log","",20,0.,10.);
	hbtag2_log->GetXaxis()->SetTitle("-log(1-CSV) 2^{nd} b-jet");


    TH1F *hJet1_pt_trigger = new TH1F("hJet1_pt_trigger", "", 13, jet1pt_binx);
    hJet1_pt_trigger->GetXaxis()->SetTitle("1^{st} Jet p_{T} (GeV)");
    TH1F *hJet2_pt_trigger = new TH1F("hJet2_pt_trigger", "",12, jet2pt_binx);
    hJet2_pt_trigger->GetXaxis()->SetTitle("2^{nd} Jet p_{T} (GeV)");
    TH1F *hJet3_pt_trigger = new TH1F("hJet3_pt_trigger", "",9, jet3pt_binx);
    hJet3_pt_trigger->GetXaxis()->SetTitle("3^{rd} Jet p_{T} (GeV)");
    TH1F *hJet4_pt_trigger = new TH1F("hJet4_pt_trigger", "", 6, jet4pt_binx);
    hJet4_pt_trigger->GetXaxis()->SetTitle("4^{th} Jet p_{T} (GeV)");
	 TH1F *hMqq_trigger = new TH1F("hMqq_trigger","",27, mqq_bins);
	hMqq_trigger->GetXaxis()->SetTitle("m_{qq} (GeV)");
	TH1F *hbtag_trigger = new TH1F("hbtag_trigger","",10,0.,1.);
	hbtag_trigger->GetXaxis()->SetTitle("CSV 1^{st} b-jet"); 
	TH1F *hbtag2_trigger = new TH1F("hbtag2_trigger","",10,0.,1.);
	hbtag2_trigger->GetXaxis()->SetTitle("CSV 2^{nd} b-jet");
	TH1F *hDeltaEta_trigger= new TH1F("hDeltaEta_trigger","",80.,0.,8.);
	hDeltaEta_trigger->GetXaxis()->SetTitle("|#Delta#eta_{qq}|");
	TH1F *hDeltaPhi_trigger = new TH1F("hDeltaPhi_trigger","",32,0.,3.2);
	hDeltaPhi_trigger->GetXaxis()->SetTitle("|#Delta#phi_{bb}|");
	TH2F *hCSV_trigger = new TH2F("hCSV_trigger","",10,0.,10.,10.,0.,10);
	hCSV_trigger->GetXaxis()->SetTitle("-log(1-CSV) of 1^{st} b-jet");
	hCSV_trigger->GetYaxis()->SetTitle("-log(1-CSV) of 2^{nd} b-jet");
	TH2F *hCSV_trigger2 = new TH2F("hCSV_trigger2","",10,0.,1.,10.,0.,1);
	hCSV_trigger2->GetXaxis()->SetTitle("CSV of 1^{st} b-jet");
	hCSV_trigger2->GetYaxis()->SetTitle("CSV of 2^{nd} b-jet");
	TH1F *hbtag_log_trigger = new TH1F("hbtag_log_trigger","",20,0.,10.);
	hbtag_log_trigger->GetXaxis()->SetTitle("-log(1-CSV) 1^{st} b-jet"); 
	TH1F *hbtag2_log_trigger = new TH1F("hbtag2_log_trigger","",20,0.,10.);
	hbtag2_log_trigger->GetXaxis()->SetTitle("-log(1-CSV) 2^{nd} b-jet");


 
   		const int numArray= 11;
			const int numArray_notrigger = numArray;
   		TH1F* histArray[numArray] = {hJet1_pt_trigger,hJet2_pt_trigger,hJet3_pt_trigger,hJet4_pt_trigger, hMqq_trigger, hbtag_trigger, hbtag2_trigger, hDeltaEta_trigger, hDeltaPhi_trigger, hbtag_log_trigger,  hbtag2_log_trigger};
			TH1F *histArray_notrigger[numArray_notrigger] = {hJet1_pt,hJet2_pt,hJet3_pt,hJet4_pt, hMqq, hbtag, hbtag2, hDeltaEta, hDeltaPhi, hbtag_log, hbtag2_log};
			TH2F *histArray_2d[10] = {hCSV,hCSV2};
			TH2F *histArray_notrigger_2d[10] = {hCSV_trigger, hCSV_trigger2};
	

		for (int i=0;i<numArray;i++){	
			histArray[i]->Sumw2();
			histArray_notrigger[i]->Sumw2();
		}
		histArray_2d[0]->Sumw2();
		histArray_notrigger_2d[0]->Sumw2();
		histArray_2d[1]->Sumw2();
		histArray_notrigger_2d[1]->Sumw2();


		TF1 *func_r = new TF1("erffunc",erf,0.,1000.,3);

	int nentries = tree_initial->GetEntries() ;
    for (int entry= 0; entry<nentries;++entry){
//    for (int entry= 0; entry<6;++entry){
    	tree_initial->GetEntry(entry);

		Int_t nominator_trigger[2] = {HLT_QuadPFJet_DoubleBTag_CSV_VBF_Mqq200,HLT_QuadPFJet_SingleBTag_CSV_VBF_Mqq460};
		Int_t denominator_trigger[4] = {HLT_QuadPFJet_VBF, HLT_DiPFJetAve60, HLT_DiPFJetAve40,HLT_DiPFJetAve60};
	
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


		genweight=TMath::Abs(genweight)/genweight*BTagWeight;
		if (data!=1) genweight/=events_generated/xsec[files]; 

		int btag_max1_number = -1;
		int btag_max2_number = -1;
		int pt_max1_number = -1;
		int pt_max2_number = -1;

		TLorentzVector Bjet1;
		TLorentzVector Bjet2;
		TLorentzVector Qjet1;
		TLorentzVector Qjet2;
		TLorentzVector qq;

		int preselection_pass=-100;
		int preselection_single_pass=-100;
		int cut_count=0;
		if (set_type==0) {
			preselection_single_pass = preselection_single(nJets, Jet.pt,Jet.eta, Jet.phi, Jet.mass, Jet.blike_VBF, Jet.id,Jet.puId, btag_max1_number, btag_max2_number, pt_max1_number, pt_max2_number, HLT_QuadPFJet_SingleBTag_CSV_VBF_Mqq460, Bjet1, Bjet2, Qjet1, Qjet2, qq,cut_count) ;//if it does pass the Single presel+trigger
			preselection_pass= preselection_double(nJets, Jet.pt,Jet.eta, Jet.phi, Jet.mass, Jet.btag, Jet.id,Jet.puId, btag_max1_number, btag_max2_number, pt_max1_number, pt_max2_number,1., Bjet1, Bjet2, Qjet1, Qjet2, qq,cut_count);
			if (preselection_single_pass==0) preselection_pass=-1;//if it does pass the Single presel+trigger then it does NOT pass Double
			if (!((btag_max1_number>=0)&&(btag_max2_number>=0)&&(pt_max1_number>=0)&&(pt_max2_number>=0))) continue;	
		}
		else {
			if (set_type==1){
 				preselection_pass=preselection_single(nJets, Jet.pt,Jet.eta, Jet.phi, Jet.mass, Jet.blike_VBF, Jet.id,Jet.puId, btag_max1_number, btag_max2_number, pt_max1_number, pt_max2_number, 1., Bjet1, Bjet2, Qjet1, Qjet2, qq, cut_count);
				if (!((btag_max1_number>=0)&&(btag_max2_number>=0)&&(pt_max1_number>=0)&&(pt_max2_number>=0))) continue;	
			}
		}
	
		Float_t Mqq = qq.M();
		Float_t bbDeltaPhi = TMath::Abs(Bjet1.DeltaPhi(Bjet2));
		Float_t qqDeltaEta = TMath::Abs(Qjet1.Eta()-Qjet2.Eta());
 	

//		if ((denominator_trigger[trigger]==1)|| (denominator_trigger[trigger-1]==1)){  //only for ave 40 or ave 60
		if (denominator_trigger[trigger]==1){ 
			if (preselection_pass==0)counter+=genweight;
		
	/*		if ((preselection_pass==0)||(preselection_pass==-2)||(preselection_pass==-3)||(preselection_pass==-4)||(preselection_pass==-5)) hJet1_pt->Fill(Jet.pt[0],genweight);
			if ((preselection_pass==0)||(preselection_pass==-2)||(preselection_pass==-3)||(preselection_pass==-4)||(preselection_pass==-5)) hJet2_pt->Fill(Jet.pt[1],genweight);
			if ((preselection_pass==0)||(preselection_pass==-2)||(preselection_pass==-3)||(preselection_pass==-4)||(preselection_pass==-5)) hJet3_pt->Fill(Jet.pt[2],genweight);
			if ((preselection_pass==0)||(preselection_pass==-2)||(preselection_pass==-3)||(preselection_pass==-4)||(preselection_pass==-5)) hJet4_pt->Fill(Jet.pt[3],genweight);*/
			if ((preselection_pass==0)||((preselection_pass==-2)&&(cut_count==1))) hJet1_pt->Fill(jets_pv[0].Pt(),genweight);
			if ((preselection_pass==0)||((preselection_pass==-3)&&(cut_count==1))) hJet2_pt->Fill(jets_pv[1].Pt(),genweight);
			if ((preselection_pass==0)||((preselection_pass==-4)&&(cut_count==1))) hJet3_pt->Fill(jets_pv[2].Pt(),genweight);
			if ((preselection_pass==0)||((preselection_pass==-5)&&(cut_count==1))) hJet4_pt->Fill(jets_pv[3].Pt(),genweight);

			if ((preselection_pass==0)||((preselection_pass==-7)&&(cut_count==1))) hDeltaEta->Fill(qqDeltaEta,genweight);
			if ((preselection_pass==0)||((preselection_pass==-8)&&(cut_count==1))) hDeltaPhi->Fill(bbDeltaPhi,genweight);
//			if ((preselection_pass==0)) hbtag->Fill(Jet.btag[btag_max1_number],genweight);
	//		if ((preselection_pass==0)) hbtag2->Fill(Jet.btag[btag_max2_number],genweight);
			if (set_type==0){
				if ((preselection_pass==0)||((preselection_pass==-10)&&(cut_count==1))) {hbtag->Fill(Jet.btag[btag_max1_number],genweight); hbtag_log->Fill(-1.*TMath::Log(1-Jet.btag[btag_max1_number]),genweight);}
			} else if ((preselection_pass==0)) {hbtag->Fill(Jet.btag[btag_max1_number],genweight); hbtag_log->Fill(-1.*TMath::Log(1-Jet.btag[btag_max1_number]),genweight);}
			if (set_type==0){
				if ((preselection_pass==0)||((preselection_pass==-11)&&(cut_count==1))) {hbtag2->Fill(Jet.btag[btag_max2_number],genweight); hbtag2_log->Fill(-1.*TMath::Log(1-Jet.btag[btag_max2_number]),genweight);}

				if ((preselection_pass==0)||((preselection_pass==-10)&&(cut_count==1))||(preselection_pass==-11)&&(cut_count==1)||((preselection_pass==-11)&&(cut_count==2)&&(Jet.btag[btag_max1_number]<0.5))) {

					hCSV->Fill(-1.*TMath::Log(1.-Jet.btag[btag_max1_number]),-1.*TMath::Log(1.-Jet.btag[btag_max2_number]),genweight);
					hCSV2->Fill(Jet.btag[btag_max1_number],Jet.btag[btag_max2_number],genweight);
				}

			} else if ((preselection_pass==0)) {
				hbtag2->Fill(Jet.btag[btag_max2_number],genweight);
				hbtag2_log->Fill(-1.*TMath::Log(1-Jet.btag[btag_max2_number]),genweight);
				hCSV->Fill(-1.*TMath::Log(1.-Jet.btag[btag_max1_number]),-1.*TMath::Log(1.-Jet.btag[btag_max2_number]),genweight);
				hCSV2->Fill(Jet.btag[btag_max1_number],Jet.btag[btag_max2_number],genweight);
			}

		if (set_type==0) {
			if ((preselection_pass==0)||((preselection_pass==-6)&&(cut_count==1))) hMqq->Fill(Mqq, genweight);
		}
		if (set_type==1){
			if ((preselection_pass==0)||((preselection_pass==-6)&&(cut_count==1))||((preselection_pass==-7)&&(Mqq<=460.)&&(cut_count==2))||((preselection_pass==-7)&&(cut_count==1)) ) hMqq->Fill(Mqq, genweight);
		}

		}


		float trigWeight = 1.;

		float trigCor1_nom_single[10] = {8.24458e+01,7.95354e+01 ,5.29763e+01};
		float trigCor2_nom_single[10] = { 2.90000e+01,1.45915e+01 , 1.62510e+01};

		float trigCor1_nom_double[10] = {7.60656e+01, 7.14527e+01,  3.78975e+01  };
		float trigCor2_nom_double[10] = {2.90000e+01,1.68348e+01, 3.71160e+01 };
		func_r->FixParameter(0,1.);
		if ((set_type==0)&&(data!=1)) {
			func_r->FixParameter(1,trigCor1_nom_double[0] );
			func_r->FixParameter(2,trigCor2_nom_double[0] );
			trigWeight*=func_r->Eval(jets_pv[0].Pt());
		}
		if ((set_type==0)&&(data!=1)) {
			func_r->FixParameter(1,trigCor1_nom_double[1] );
			func_r->FixParameter(2,trigCor2_nom_double[1] );
			trigWeight*=func_r->Eval(jets_pv[1].Pt());
		}
		if ((set_type==0)&&(data!=1)) {
			func_r->FixParameter(1,trigCor1_nom_double[2] );
			func_r->FixParameter(2,trigCor2_nom_double[2] );
			trigWeight*=func_r->Eval(jets_pv[2].Pt());
		}
		if ((set_type==1)&&(data!=1)) {
			func_r->FixParameter(1,trigCor1_nom_single[0] );
			func_r->FixParameter(2,trigCor2_nom_single[0] );
			trigWeight*=func_r->Eval(jets_pv[0].Pt());
		}
		if ((set_type==1)&&(data!=1)) {
			func_r->FixParameter(1,trigCor1_nom_single[1] );
			func_r->FixParameter(2,trigCor2_nom_single[1] );
			trigWeight*=func_r->Eval(jets_pv[1].Pt());
		}
		if ((set_type==1)&&(data!=1)) {
			func_r->FixParameter(1,trigCor1_nom_single[2] );
			func_r->FixParameter(2,trigCor2_nom_single[2] );
			trigWeight*=func_r->Eval(jets_pv[2].Pt());
		}



//		if ((nominator_trigger[set_type]==1)&&((denominator_trigger[trigger]==1)|| (denominator_trigger[trigger-1]==1)) ){ 
		if ((nominator_trigger[set_type]==1)&&(denominator_trigger[trigger]==1)){
			genweight*=trigWeight; 
			if (preselection_pass==0) counter_tr1+=genweight; 
	/*		if ((preselection_pass==0)||(preselection_pass==-2)||(preselection_pass==-3)||(preselection_pass==-4)||(preselection_pass==-5)) hJet1_pt_trigger->Fill(Jet.pt[0],genweight);
			if ((preselection_pass==0)||(preselection_pass==-2)||(preselection_pass==-3)||(preselection_pass==-4)||(preselection_pass==-5)) hJet2_pt_trigger->Fill(Jet.pt[1],genweight);
			if ((preselection_pass==0)||(preselection_pass==-2)||(preselection_pass==-3)||(preselection_pass==-4)||(preselection_pass==-5)) hJet3_pt_trigger->Fill(Jet.pt[2],genweight);
			if ((preselection_pass==0)||(preselection_pass==-2)||(preselection_pass==-3)||(preselection_pass==-4)||(preselection_pass==-5)) hJet4_pt_trigger->Fill(Jet.pt[3],genweight);*/

			if ((preselection_pass==0)||((preselection_pass==-2)&&(cut_count==1))) hJet1_pt_trigger->Fill(jets_pv[0].Pt(),genweight);
			if ((preselection_pass==0)||((preselection_pass==-3)&&(cut_count==1))) hJet2_pt_trigger->Fill(jets_pv[1].Pt(),genweight);
			if ((preselection_pass==0)||((preselection_pass==-4)&&(cut_count==1))) hJet3_pt_trigger->Fill(jets_pv[2].Pt(),genweight);
			if ((preselection_pass==0)||((preselection_pass==-5)&&(cut_count==1))) hJet4_pt_trigger->Fill(jets_pv[3].Pt(),genweight);

			if ((preselection_pass==0)||((preselection_pass==-7)&&(cut_count==1))) hDeltaEta_trigger->Fill(qqDeltaEta,genweight);
			if ((preselection_pass==0)||((preselection_pass==-8)&&(cut_count==1))) hDeltaPhi_trigger->Fill(bbDeltaPhi,genweight);
			if (set_type==0){
				if ((preselection_pass==0)||((preselection_pass==-10)&&(cut_count==1))) {hbtag_trigger->Fill(Jet.btag[btag_max1_number],genweight);hbtag_log_trigger->Fill(-1.*TMath::Log(1-Jet.btag[btag_max1_number]),genweight);}
			} else if ((preselection_pass==0)) {hbtag_trigger->Fill(Jet.btag[btag_max1_number],genweight);hbtag_log_trigger->Fill(-1.*TMath::Log(1-Jet.btag[btag_max1_number]),genweight);}
			if (set_type==0){
				if ((preselection_pass==0)||((preselection_pass==-11)&&(cut_count==1))) {hbtag2_trigger->Fill(Jet.btag[btag_max2_number],genweight);hbtag2_log_trigger->Fill(-1.*TMath::Log(1-Jet.btag[btag_max2_number]),genweight);}

				if ((preselection_pass==0)||((preselection_pass==-10)&&(cut_count==1))||(preselection_pass==-11)&&(cut_count==1)||((preselection_pass==-11)&&(cut_count==2)&&(Jet.btag[btag_max1_number]<0.5))) {
				hCSV_trigger->Fill(-1.*TMath::Log(1.-Jet.btag[btag_max1_number]),-1.*TMath::Log(1.-Jet.btag[btag_max2_number]),genweight);
				hCSV_trigger2->Fill(Jet.btag[btag_max1_number],Jet.btag[btag_max2_number],genweight);}

			} else if ((preselection_pass==0)) {
				hbtag2_trigger->Fill(Jet.btag[btag_max2_number],genweight);
				hbtag2_log_trigger->Fill(-1.*TMath::Log(1-Jet.btag[btag_max2_number]),genweight);
				hCSV_trigger->Fill(-1.*TMath::Log(1.-Jet.btag[btag_max1_number]),-1.*TMath::Log(1.-Jet.btag[btag_max2_number]),genweight);
				hCSV_trigger2->Fill(Jet.btag[btag_max1_number],Jet.btag[btag_max2_number],genweight);
			}
			 
			if (set_type==0) {
				if ((preselection_pass==0)||((preselection_pass==-6)&&(cut_count==1))) hMqq_trigger->Fill(Mqq, genweight);
			}
			if (set_type==1){
				if ((preselection_pass==0)||((preselection_pass==-6)&&(cut_count==1))||((preselection_pass==-7)&&(Mqq<=460.)&&(cut_count==2))||((preselection_pass==-7)&&(cut_count==1)) ) hMqq_trigger->Fill(Mqq, genweight);
			}

		}

		

}


	
	TFile *f_output = new TFile("output_hist/"+file_names[files]+type[set_type]+denominator_trigger_names[trigger]+"_btagW_unc.root","recreate");
	f_output->cd();
	for (int i=0;i<numArray;i++){
   	histArray[i]->Write();
   	histArray_notrigger[i]->Write();
	}
   histArray_2d[0]->Write();
   histArray_notrigger_2d[0]->Write();
   histArray_2d[1]->Write();
   histArray_notrigger_2d[1]->Write();
   f_output->Write();
	f_output->Close();
	
	ofstream out;
	out.open("output_txt/"+file_names[files]+type[set_type]+denominator_trigger_names[trigger]+"_btagW_unc.txt");	
	out<<counter_tr1<<"\t"<<counter<<endl;
	out.close();


	files++;
} while (files<files_max);


return 0; 

    
}
