#include <ctype.h>
#include <iomanip>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "TFile.h"
#include "TH1.h"
#include "TH1F.h"
#include "TH2.h"
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


const int njets = 300;

typedef struct {
   Float_t eta[njets];
   Float_t pt[njets];
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
	Float_t HTsoft;			
} Jets;

using namespace std;

void turnOnBtag(int files, int files_max){
    
const int nfiles  = 5;

TString file_names[nfiles] = {"/afs/cern.ch/work/n/nchernya/Hbb/data40pb/tree_HTMHT.root", "/afs/cern.ch/work/n/nchernya/Hbb/MC_new/VBFHToBB_M-125_13TeV_powheg_pythia8/tree.root","/afs/cern.ch/work/n/nchernya/Hbb/data40pb/tree_JetHT.root","/afs/cern.ch/work/n/nchernya/Hbb/MC_new/VBFHToBB_M-130_13TeV_powheg_pythia8__RunIISpring15DR74-Asympt25ns_MCRUN2_74_V9-v2/tree.root","/afs/cern.ch/work/n/nchernya/Hbb/MC_new/VBFHToBB_M125_13TeV_amcatnlo_pythia8__RunIISpring15DR74-Asympt25ns_MCRUN2_74_V9-v1/tree.root"};
   
TString type[nfiles] = {"Data40pb_HTMHT_","Powheg125_","Data40pb_JetHT_","Powheg130_","amc125_"};

do {
	
	Float_t presel=0;
	Float_t cut1 = 0;
	Float_t cut2 = 0;
	Float_t cut3 = 0;
	Float_t genweight=1.;
	Int_t global_counter = 0;
	Bool_t HLT_QuadPFJet_DoubleBTag_CSV_VBF_Mqq200;
	Bool_t HLT_QuadPFJet_SingleBTag_CSV_VBF_Mqq460;
//	Float_t HLT_QuadPFJet_DoubleBTag_CSV_VBF_Mqq200;
//	Float_t HLT_QuadPFJet_SingleBTag_CSV_VBF_Mqq460;
	Float_t HLT_QuadPFJet_VBF;
	Float_t HLT_L1_TripleJet_VBF;


	TFile *file_initial = new TFile(file_names[files]);
   TTree *tree_initial = (TTree*)file_initial->Get("tree");
    Jets Jet;
    Float_t v_type;
    Float_t wrong_type=0.;
    Int_t nJets;
	Int_t counter = 0;
	Int_t counter_tr2 = 0;
	Int_t counter_tr1=0;
	Int_t JSON = 0;


	
	if ((files==0)||(files==2)){
    tree_initial->SetBranchAddress("offJet_num",&nJets);
    tree_initial->SetBranchAddress("offJet_pt",Jet.pt);
    tree_initial->SetBranchAddress("offJet_eta",Jet.eta);
    tree_initial->SetBranchAddress("offJet_phi",Jet.phi);
	tree_initial->SetBranchAddress("offJet_mass",Jet.mass);
	tree_initial->SetBranchAddress("offJet_csv",Jet.btag);
 	tree_initial->SetBranchAddress("HLT_QuadPFJet_DoubleBTagCSV_VBF_Mqq200_v2",&HLT_QuadPFJet_DoubleBTag_CSV_VBF_Mqq200);
 	tree_initial->SetBranchAddress("HLT_QuadPFJet_SingleBTagCSV_VBF_Mqq460_v2",&HLT_QuadPFJet_SingleBTag_CSV_VBF_Mqq460);
	tree_initial->SetBranchAddress("JSON",&JSON);
 } 
	if ((files!=0)&&(files!=2)){


    tree_initial->SetBranchAddress("nJet",&nJets);
    tree_initial->SetBranchAddress("genWeight",&genweight);
    tree_initial->SetBranchAddress("Jet_pt",Jet.pt);
    tree_initial->SetBranchAddress("Jet_eta",Jet.eta);
    tree_initial->SetBranchAddress("Jet_phi",Jet.phi);
	tree_initial->SetBranchAddress("Jet_mass",Jet.mass);
	tree_initial->SetBranchAddress("Jet_btagCSV",Jet.btag);
 	tree_initial->SetBranchAddress("HLT_BIT_HLT_QuadPFJet_DoubleBTagCSV_VBF_Mqq200_v",&HLT_QuadPFJet_DoubleBTag_CSV_VBF_Mqq200);
 	tree_initial->SetBranchAddress("HLT_BIT_HLT_QuadPFJet_SingleBTagCSV_VBF_Mqq460_v",&HLT_QuadPFJet_SingleBTag_CSV_VBF_Mqq460);
 	tree_initial->SetBranchAddress("HLT_BIT_HLT_QuadPFJet_VBF_v",&HLT_QuadPFJet_VBF);
 	tree_initial->SetBranchAddress("HLT_BIT_HLT_L1_TripleJet_VBF_v",&HLT_L1_TripleJet_VBF);

	}

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


    TH1F *hJet1_pt_2 = new TH1F("hJet1_pt_2", "", 13, jet1pt_binx);
    hJet1_pt_2->GetXaxis()->SetTitle("1^{st} Jet p_{T} (GeV)");
    TH1F *hJet2_pt_2 = new TH1F("hJet2_pt_2", "", 12, jet2pt_binx);
    hJet2_pt_2->GetXaxis()->SetTitle("2^{nd} Jet p_{T} (GeV)");
    TH1F *hJet3_pt_2 = new TH1F("hJet3_pt_2", "", 9, jet3pt_binx);
    hJet3_pt_2->GetXaxis()->SetTitle("3^{rd} Jet p_{T} (GeV)");
    TH1F *hJet4_pt_2 = new TH1F("hJet4_pt_2", "", 6, jet4pt_binx);
    hJet4_pt_2->GetXaxis()->SetTitle("4^{th} Jet p_{T} (GeV)");
	 TH1F *hMqq_2 = new TH1F("hMqq_2","",27, mqq_bins);
	hMqq_2->GetXaxis()->SetTitle("m_{qq} (GeV)");
	TH1F *hbtag_2 = new TH1F("hbtag_2","",10,0.,1.);
	hbtag_2->GetXaxis()->SetTitle("CSV 1^{st} b-jet"); 
	TH1F *hbtag2_2 = new TH1F("hbtag2_2","",10,0.,1.);

    
    TH1F *hJet1_pt_trigger2 = new TH1F("hJet1_pt_trigger2", "", 13, jet1pt_binx);
    hJet1_pt_trigger2->GetXaxis()->SetTitle("1^{st} Jet p_{T} (GeV)");
    TH1F *hJet2_pt_trigger2 = new TH1F("hJet2_pt_trigger2", "",12, jet2pt_binx);
    hJet2_pt_trigger2->GetXaxis()->SetTitle("2^{nd} Jet p_{T} (GeV)");
    TH1F *hJet3_pt_trigger2 = new TH1F("hJet3_pt_trigger2", "", 9, jet3pt_binx);
    hJet3_pt_trigger2->GetXaxis()->SetTitle("3^{rd} Jet p_{T} (GeV)");
    TH1F *hJet4_pt_trigger2 = new TH1F("hJet4_pt_trigger2", "", 6, jet4pt_binx);
    hJet4_pt_trigger2->GetXaxis()->SetTitle("4^{th} Jet p_{T} (GeV)");
	 TH1F *hMqq_trigger2 = new TH1F("hMqq_trigger2","",27, mqq_bins);
	hMqq_trigger2->GetXaxis()->SetTitle("m_{qq} (GeV)");
	TH1F *hbtag_trigger2 = new TH1F("hbtag_trigger2","",10,0.,1.);
	hbtag_trigger2->GetXaxis()->SetTitle("CSV 1^{st} b-jet"); 
	TH1F *hbtag2_trigger2 = new TH1F("hbtag2_trigger2","",10,0.,1.);
	hbtag2_trigger2->GetXaxis()->SetTitle("CSV 2^{nd} b-jet");
	
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


	TProfile *hprof = new TProfile("hprof","",100,0,3000,0,1.);





   		const int numArray= 7;
   		TH1F* histArray[numArray] = {hJet1_pt_trigger,hJet2_pt_trigger,hJet3_pt_trigger,hJet4_pt_trigger, hMqq_trigger, hbtag_trigger, hbtag2_trigger};
			TH1F* histArray_2[numArray] = {hJet1_pt_trigger2,hJet2_pt_trigger2,hJet3_pt_trigger2,hJet4_pt_trigger2, hMqq_trigger2, hbtag_trigger2, hbtag2_trigger2 };
			const int numArray_notrigger = 7;
			TH1F *histArray_notrigger[numArray_notrigger] = {hJet1_pt,hJet2_pt,hJet3_pt,hJet4_pt, hMqq, hbtag, hbtag2};
			TH1F *histArray_notrigger_2[numArray_notrigger] = {hJet1_pt_2,hJet2_pt_2,hJet3_pt_2,hJet4_pt_2, hMqq_2, hbtag_2, hbtag2_2};
			TString hist_names[numArray_notrigger] = {"Jet1_pt","Jet2_pt","Jet3_pt","Jet4_pt", "Mqq", "btag", "btag2"};
			TString output_names[numArray_notrigger];
			for (int i=0;i<numArray_notrigger;i++){
				output_names[i] = hist_names[i];
				output_names[i].Prepend(type[files]+"plots"+"_eta/");
				output_names[i].Append(".png");
			}
	//		Float_t hist_min[numArray_notrigger] = {1e-04,1e-04,1e-04,1e-04,1e-04,1e-04,1e-04};
	//		Float_t hist_max[numArray_notrigger] = {0.07,0.08,0.16,0.18,0.05,0.1,0.21};
	//		Float_t hist_min[numArray_notrigger] = {1e-04,1e-04,1e-04,1e-04,1e-04,1e-04,1e-04};
	//		Float_t hist_max[numArray_notrigger] = {.5,.7,1.,1.,.2,0.14,0.18};


			Float_t hist_min[numArray_notrigger] = {1e-04,1e-04,1e-04,1e-04,1e-04,1e-04,1e-04};
			Float_t hist_max[numArray_notrigger] = {.1,0.16,.3,.3,0.08,0.2,0.4}; //HTMHT
	//		Float_t hist_min[numArray_notrigger] = {1e-04,1e-04,1e-04,1e-04,1e-04,1e-04,1e-04};
	//		Float_t hist_max[numArray_notrigger] = {.7,.9,1.,1.,0.2,0.2,0.25};
	//		Float_t hist_max[numArray_notrigger] = {1.,1.,1.,1.,0.8,1.,1.};
 

		for (int i=0;i<numArray;i++){	
			histArray[i]->Sumw2();
			histArray_2[i]->Sumw2();
			histArray_notrigger[i]->Sumw2();
			histArray_notrigger_2[i]->Sumw2();
		}



	int nentries = tree_initial->GetEntries() ;
    for (int entry=0; entry<nentries;++entry){
        tree_initial->GetEntry(entry);


	//	if (!HLT_QuadPFJet_VBF) continue;
//		if (!HLT_L1_TripleJet_VBF) continue;
	
		if ((files==0)||(files==2)) {
			if (JSON!=1) {
				continue;
			}
		}	
	
		int loopJet_min = 6;
		if (nJets<6) loopJet_min=nJets;

		Float_t btag_max = 0.0;
		int btag_max1_number = -1;
		int btag_max2_number = -1;
		for (int i=0;i<loopJet_min;i++){
			if ((Jet.btag[i]>btag_max)){
				btag_max=Jet.btag[i];
				btag_max1_number=i;
			}
		}
		btag_max = 0.0;
		for (int i=0;i<loopJet_min;i++){
			if ((Jet.btag[i]>btag_max)&&(i!=btag_max1_number)) {
				btag_max=Jet.btag[i];
				btag_max2_number=i;
			} 
		}
		if (!((btag_max1_number>=0)&&(btag_max2_number>=0))) continue;
		TLorentzVector Bjet1;
		Bjet1.SetPtEtaPhiM(Jet.pt[btag_max1_number],Jet.eta[btag_max1_number],Jet.phi[btag_max1_number],Jet.mass[btag_max1_number]);
		
		TLorentzVector Bjet2;
		Bjet2.SetPtEtaPhiM(Jet.pt[btag_max2_number],Jet.eta[btag_max2_number],Jet.phi[btag_max2_number],Jet.mass[btag_max2_number]);


		Float_t pt_max = 0.;
		int pt_max1_number = -1;
		int pt_max2_number = -1;
		for (int i=0;i<loopJet_min;i++){
			if ((Jet.pt[i]>pt_max)&&(i!=btag_max1_number)&&(i!=btag_max2_number)) {
				pt_max=Jet.pt[i];
				pt_max1_number=i;	
			}
		}
		pt_max = 0.;
		for (int i=0;i<loopJet_min;i++){
			if ((Jet.pt[i]>pt_max)&&(i!=btag_max1_number)&&(i!=btag_max2_number)&&(i!=pt_max1_number)) {
				pt_max=Jet.pt[i];
				pt_max2_number=i;	
			}
		}
		
		if (!((pt_max1_number>=0)&&(pt_max2_number>=0))) continue;
			
		TLorentzVector Qjet1;
		Qjet1.SetPtEtaPhiM(Jet.pt[pt_max1_number],Jet.eta[pt_max1_number],Jet.phi[pt_max1_number],Jet.mass[pt_max1_number]);
		
		TLorentzVector Qjet2;
		Qjet2.SetPtEtaPhiM(Jet.pt[pt_max2_number],Jet.eta[pt_max2_number],Jet.phi[pt_max2_number],Jet.mass[pt_max2_number]);




		TLorentzVector qq;
		qq = Qjet1+Qjet2;
		Float_t Mqq = qq.M();


		Float_t bbDeltaPhi = TMath::Abs(Bjet1.DeltaPhi(Bjet2));
		Float_t qqDeltaEta = TMath::Abs(Qjet1.Eta()-Qjet2.Eta());
	if ((qqDeltaEta>1.2)/*&&(bbDeltaPhi<2.4)&&((Jet.pt[0]>92.)&&(Jet.pt[1]>76.)&&(Jet.pt[2]>64.)&&(Jet.pt[3]>30.))*/){	
		hJet1_pt_2->Fill(Jet.pt[0],genweight);
		hJet2_pt_2->Fill(Jet.pt[1],genweight);
		hJet3_pt_2->Fill(Jet.pt[2],genweight);
		hJet4_pt_2->Fill(Jet.pt[3],genweight);
		hMqq_2->Fill(Mqq, genweight);
		hbtag_2->Fill(Jet.btag[btag_max1_number],genweight);
		hbtag2_2->Fill(Jet.btag[btag_max2_number],genweight);
		
		counter++;	
		if (HLT_QuadPFJet_DoubleBTag_CSV_VBF_Mqq200==1){
			counter_tr2++; 
			hJet1_pt_trigger2->Fill(Jet.pt[0],genweight);
			hJet2_pt_trigger2->Fill(Jet.pt[1],genweight);
			hJet3_pt_trigger2->Fill(Jet.pt[2],genweight);
			hJet4_pt_trigger2->Fill(Jet.pt[3],genweight);
			hMqq_trigger2->Fill(Mqq, genweight);
			hbtag_trigger2->Fill(Jet.btag[btag_max1_number],genweight);
			hbtag2_trigger2->Fill(Jet.btag[btag_max2_number],genweight);
		}

	}
//////////////////////////////////////////////Single BTag trigger/////////////////////////////////////////////////////	
		loopJet_min = 4;
		if (nJets<4) loopJet_min=nJets;

		btag_max = 0.;
		int btag_max1_number_single = -1;
		int btag_max2_number_single = -1;
		for (int i=0;i<loopJet_min;i++){
			if ((Jet.btag[i]>btag_max)){
				btag_max=Jet.btag[i];
				btag_max1_number_single=i;
			}
		}
		if (!((btag_max1_number_single>=0))) continue;
		TLorentzVector Bjet1_single;
		Bjet1_single.SetPtEtaPhiM(Jet.pt[btag_max1_number_single],Jet.eta[btag_max1_number_single],Jet.phi[btag_max1_number_single],Jet.mass[btag_max1_number_single]);


		int pt_max1_number_single = -1;
		int pt_max2_number_single = -1;

		TLorentzVector js[3];
		int jcount = 0;
		int j_num[3] = {};
		for (int i=0;i<4;i++){
			if ((i!=btag_max1_number_single)) {
				js[jcount].SetPtEtaPhiM(Jet.pt[i], Jet.eta[i], Jet.phi[i], Jet.mass[i]);
				j_num[jcount] = i;
				jcount++;
			}
		}	
		if (!(jcount==3)) continue;	
		Float_t deltaEtaJets[3] = {TMath::Abs(js[0].Eta()-js[1].Eta()),TMath::Abs(js[1].Eta()-js[2].Eta()), TMath::Abs(js[0].Eta()-js[2].Eta())};
		int eta_num[3][2] = {{0,1}, {1,2} ,{0,2}};
		Float_t max_deltaEta = 0.;
		int max_deltaEta_num = -1;
		for (int i=0;i<3;i++){
			if (deltaEtaJets[i]>max_deltaEta) {
				max_deltaEta = deltaEtaJets[i];
				max_deltaEta_num = i;
			}
		}
		
		pt_max1_number_single = j_num[ eta_num[max_deltaEta_num][0]];
		pt_max2_number_single = j_num[ eta_num[max_deltaEta_num][1]];

		if (!((pt_max1_number_single>=0)&&(pt_max2_number_single>=0))) continue;
	
		TLorentzVector Qjet1_single;
		Qjet1_single.SetPtEtaPhiM(Jet.pt[pt_max1_number_single] ,Jet.eta[pt_max1_number_single],Jet.phi[pt_max1_number_single],Jet.mass[pt_max1_number_single]);
	
		TLorentzVector Qjet2_single;
		Qjet2_single.SetPtEtaPhiM(Jet.pt[pt_max2_number_single],Jet.eta[pt_max2_number_single],Jet.phi[pt_max2_number_single],Jet.mass[pt_max2_number_single]);

		for (int i=0;i<4;i++){
			if ( (i!=btag_max1_number_single)&&(i!=pt_max1_number_single)&&(i!=pt_max2_number_single)) btag_max2_number_single=i;
		}

		if (!((btag_max2_number_single>=0))) continue;

		


		TLorentzVector Bjet2_single;
		Bjet2_single.SetPtEtaPhiM(Jet.pt[btag_max2_number_single],Jet.eta[btag_max2_number_single],Jet.phi[btag_max2_number_single],Jet.mass[btag_max2_number_single]);

		TLorentzVector qq_single;
		qq_single = Qjet1_single+Qjet2_single;
		Double_t Mqq_single = qq_single.M();

			
		Float_t qqDeltaEta_single = TMath::Abs(Qjet1_single.Eta()-Qjet2_single.Eta());
		Float_t bbDeltaPhi_single = TMath::Abs(Bjet1_single.DeltaPhi(Bjet2_single));
				
		hprof->Fill(Mqq_single,Jet.btag[btag_max1_number_single],genweight);

		if ((qqDeltaEta_single>4.1)/*&&(bbDeltaPhi_single<1.6)&&((Jet.pt[0]>92.)&&(Jet.pt[1]>76.)&&(Jet.pt[2]>64.)&&(Jet.pt[3]>30.))*/) {


		hJet1_pt->Fill(Jet.pt[0],genweight);
		hJet2_pt->Fill(Jet.pt[1],genweight);
		hJet3_pt->Fill(Jet.pt[2],genweight);
		hJet4_pt->Fill(Jet.pt[3],genweight);
		hMqq->Fill(Mqq_single, genweight);
		hbtag->Fill(Jet.btag[btag_max1_number_single],genweight);
		hbtag2->Fill(Jet.btag[btag_max2_number_single],genweight);


		if (HLT_QuadPFJet_SingleBTag_CSV_VBF_Mqq460==1){ 
			counter_tr1++; 
			hJet1_pt_trigger->Fill(Jet.pt[0],genweight);
			hJet2_pt_trigger->Fill(Jet.pt[1],genweight);
			hJet3_pt_trigger->Fill(Jet.pt[2],genweight);
			hJet4_pt_trigger->Fill(Jet.pt[3],genweight);
			hMqq_trigger->Fill(Mqq_single, genweight);
			hbtag_trigger->Fill(Jet.btag[btag_max1_number_single],genweight);
			hbtag2_trigger->Fill(Jet.btag[btag_max2_number_single],genweight);
		}
		

      }
}

		for (int i=0;i<7;i++){
			histArray[i]->Divide(histArray_notrigger[i]);
			histArray_2[i]->Divide(histArray_notrigger_2[i]);
		}

	
    
		for (int i=0;i<7;++i){
				TLatex* tex = new TLatex(0.90,0.92,"13 TeV, bx = 50 ns, 40 pb^{-1}"); /// need to be change for data/MC 25-50
     			tex->SetNDC();
				tex->SetTextAlign(35);
      		tex->SetTextFont(42);
      		tex->SetTextSize(0.04);
      		tex->SetLineWidth(2);
     			TLatex *tex1 = new TLatex(0.13,0.83,"CMS");
     			tex1->SetNDC();
      		tex1->SetTextAlign(20);
      		tex1->SetTextFont(61);
      		tex1->SetTextSize(0.06);
      		tex1->SetLineWidth(2);
      		TLatex* tex2 = new TLatex(0.22,0.77,"Work in progress");
      		tex2->SetNDC();
      		tex2->SetTextAlign(20);
      		tex2->SetTextFont(52);
      		tex2->SetTextSize(0.04);
      		tex2->SetLineWidth(2);
				TLatex* tex_file = new TLatex(0.28,0.92,"H(bb) triggers");
    		  	tex_file->SetNDC();
				tex_file->SetTextAlign(35);
     			tex_file->SetTextFont(42);
     			tex_file->SetTextSize(0.04);
      		tex_file->SetLineWidth(2);	
				TCanvas *c1 = new TCanvas();
				c1->SetBottomMargin(.12);
				c1->cd();
				Double_t xmin = histArray[i]->GetBinCenter(0);
				Double_t xmax = (histArray[i]->GetBinCenter(histArray[i]->GetNbinsX()))*1.3;
				TH1F *frame = new TH1F("frame","",1,xmin,xmax);
				frame->SetMinimum(hist_min[i]);
     			frame->SetMaximum(hist_max[i]);
     			frame->GetYaxis()->SetTitleOffset(0.9);
     			frame->GetXaxis()->SetTitleOffset(0.91);
     			frame->SetStats(0);
     			frame->SetTitleFont(42,"x");
				frame->SetTitleFont(42,"y");
      		frame->SetTitleSize(0.05, "XYZ");
				frame->SetXTitle(histArray[i]->GetXaxis()->GetTitle());
    	   	frame->SetYTitle("Efficiency");
				frame->Draw();
				TLegend *leg = new TLegend(0.7,0.75,0.85,0.85);
				leg->SetBorderSize(0);
				leg->SetTextSize(0.04);
    	    	histArray[i]->SetLineWidth(2);
    	    	histArray[i]->SetMarkerStyle(20);
    	    	histArray_2[i]->SetMarkerStyle(20);
    	    	histArray_2[i]->SetLineWidth(2);
        		histArray[i]->SetLineColor(kBlue);
        		histArray[i]->SetMarkerColor(kBlue);
        		histArray[i]->Draw("PEsame");
        		histArray_2[i]->SetLineColor(kPink);
        		histArray_2[i]->SetMarkerColor(kPink);
        		histArray_2[i]->Draw("PEsame");
				leg->AddEntry(histArray_2[i],"DoubleBtag","LP");
				leg->AddEntry(histArray[i],"SingleBtag","LP");
				tex->Draw();
				tex1->Draw();
				tex2->Draw();
				tex_file->Draw();
				leg->Draw();
				c1->Print(output_names[i]);
				c1->Delete();
				leg->Delete();
   		} 
//	TCanvas *c2 = new TCanvas("c2","",200,10,700,500);
//	hprof->Draw();
	files++;
	ofstream out(type[files]+".txt");
	out<<"2 = "<<counter_tr2<<"   "<<counter<< "    " <<(float) counter_tr2/counter<<endl;
	out<<"1 = "<<counter_tr1<<"   "<<counter<< "    "<<(float) counter_tr1/counter<<endl;
	cout<<"2 = "<<counter_tr2<<"   "<<counter<< "    " <<(float) counter_tr2/counter<<endl;
	cout<<"1 = "<<counter_tr1<<"   "<<counter<< "    "<<(float) counter_tr1/counter<<endl;
	out.close();
} while (files<files_max); 

    
}
