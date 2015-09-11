#define CreateTree_tmva_double_cxx
#include "CreateTree_tmva_double.h"
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>
#include "TLorentzVector.h"
#include <iostream>
#include <fstream>
#include <iomanip>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
typedef std::map<Float_t, Int_t> JetList;

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

typedef struct {
	Float_t CSV1;
	Float_t CSV2;
	Float_t Mqq;
	Float_t DeltaEtaQQ;
	Int_t SoftN5;
	Float_t HTsoft;
	Float_t DeltaEtaQB1;
	Float_t DeltaEtaQB2;
	Float_t DeltaPhiBB;
}TMVAstruct;




void CreateTree_tmva_double::Loop(int sample_type)
{
   if (fChain == 0) return;

   Long64_t nentries = fChain->GetEntriesFast();

   Long64_t nbytes = 0, nb = 0;
	TMVAstruct TMVA;

	TString sample_path[2] = {"/afs/cern.ch/work/n/nchernya/Hbb/MC_new/VBFHToBB_M-125_13TeV_powheg_pythia8/tree.root","/afs/cern.ch/work/n/nchernya/Hbb/MC_new/QCD_HT300to500_TuneCUETP8M1_13TeV-madgraphMLM-pythia8__RunIISpring15DR74-Asympt25ns_MCRUN2_74_V9-v2/tree.root"};	
	TString sample_type_name[2] = {"VBF_powheg_125","QCD_300_500"};
	Float_t xsec[2] = {2.16,3.67E05};

	TFile *file_initial = new TFile(sample_path[sample_type]);
	TH1F *countPos = (TH1F*)file_initial->Get("CountPosWeight");
	TH1F *countNeg = (TH1F*)file_initial->Get("CountNegWeight");
	Int_t events_generated = countPos->GetEntries()-countNeg->GetEntries();
	genWeight/=events_generated/xsec[sample_type];
 
	TFile file("main_tmva_tree_"+sample_type_name[sample_type]+".root","recreate");
	TTree *tree0 = new TTree("TMVA","TMVA");
	tree0->Branch("CSV1",&TMVA.CSV1,"CSV1/F");
	tree0->Branch("CSV2",&TMVA.CSV2,"CSV2/F");
	tree0->Branch("Mqq",&TMVA.Mqq,"Mqq/F");
	tree0->Branch("DeltaQQ",&TMVA.DeltaEtaQQ,"DeltaQQ/F");
	tree0->Branch("SoftN5",&TMVA.SoftN5,"SoftN5/I");
	tree0->Branch("HTsoft",&TMVA.HTsoft,"HTsoft/F");
	tree0->Branch("DeltaEtaQB1",&TMVA.DeltaEtaQB1,"DeltaEtaQB1/F");
	tree0->Branch("DeltaEtaQB2",&TMVA.DeltaEtaQB2,"DeltaEtaQB2/F");
	



   for (Long64_t jentry=0; jentry<nentries;jentry++) {
	   Long64_t ientry = LoadTree(jentry);
	   if (ientry < 0) break;
	   nb = fChain->GetEntry(jentry);   nbytes += nb;
	   
		if (!((Jet_pt[0]>92.)&&(Jet_pt[1]>76.)&&(Jet_pt[2]>64.)&&(Jet_pt[3]>30.))) continue;

		int loopJet_min = 6;
		if (nJet<6) loopJet_min=nJet;

		for (int i=0;i<loopJet_min;i++){
			if (Jet_btagCSV[i]>1) Jet_btagCSV[i]=1.;
		}

		Double_t btag_max = 0.4;
		int btag_max1_number = -1;
		int btag_max2_number = -1;
		for (int i=0;i<loopJet_min;i++){
			if ((Jet_btagCSV[i]>btag_max)&&(Jet_id[i]>0)){
				btag_max=Jet_btagCSV[i];
				btag_max1_number=i;
			}
		}
		btag_max = 0.4;
		for (int i=0;i<loopJet_min;i++){
			if ((Jet_btagCSV[i]>btag_max)&&(i!=btag_max1_number)&&(Jet_id[i]>0)) {
				btag_max=Jet_btagCSV[i];
				btag_max2_number=i;
			} 
		}
		if (!((btag_max1_number>=0)&&(btag_max2_number>=0))) continue;
		TLorentzVector Bjet1;
		Bjet1.SetPtEtaPhiM(Jet_pt[btag_max1_number],Jet_eta[btag_max1_number],Jet_phi[btag_max1_number],Jet_mass[btag_max1_number]);
		
		TLorentzVector Bjet2;
		Bjet2.SetPtEtaPhiM(Jet_pt[btag_max2_number],Jet_eta[btag_max2_number],Jet_phi[btag_max2_number],Jet_mass[btag_max2_number]);


		Double_t pt_max = 20.;
		int pt_max1_number = -1;
		int pt_max2_number = -1;
		for (int i=0;i<loopJet_min;i++){
			if ((Jet_pt[i]>pt_max)&&(i!=btag_max1_number)&&(i!=btag_max2_number)&&(Jet_id[i]>0)) {
				pt_max=Jet_pt[i];
				pt_max1_number=i;	
			}
		}
		pt_max = 20.;
		for (int i=0;i<loopJet_min;i++){
			if ((Jet_pt[i]>pt_max)&&(i!=btag_max1_number)&&(i!=btag_max2_number)&&(i!=pt_max1_number)&&(Jet_id[i]>0)) {
				pt_max=Jet_pt[i];
				pt_max2_number=i;	
			}
		}
		
		if (!((pt_max1_number>=0)&&(pt_max2_number>=0))) continue;
			
		TLorentzVector Qjet1;
		Qjet1.SetPtEtaPhiM(Jet_pt[pt_max1_number],Jet_eta[pt_max1_number],Jet_phi[pt_max1_number],Jet_mass[pt_max1_number]);
	
		TLorentzVector Qjet2;
		Qjet2.SetPtEtaPhiM(Jet_pt[pt_max2_number],Jet_eta[pt_max2_number],Jet_phi[pt_max2_number],Jet_mass[pt_max2_number]);


		TLorentzVector qq;
		qq = Qjet1+Qjet2;
		Double_t Mqq = qq.M();
		Double_t bbDeltaPhi = TMath::Abs(Bjet1.DeltaPhi(Bjet2));
		Double_t qqDeltaEta = TMath::Abs(Qjet1.Eta()-Qjet2.Eta());
		if (!((Mqq>200)&&(qqDeltaEta>1.2)&&(bbDeltaPhi<2.4))) continue;

		if (HLT_BIT_HLT_QuadPFJet_DoubleBTagCSV_VBF_Mqq200_v!=1) continue;


		Float_t EtaBQ1;
	 	Float_t EtaBQ2;
		Float_t PhiBQ1; 	
		Float_t PhiBQ2;
		 if (Qjet1.Eta() >= Qjet2.Eta()) {
			if (Bjet1.Eta() >= Bjet2.Eta())  {
				EtaBQ1 = Qjet1.Eta()-Bjet1.Eta();
				PhiBQ1 = TMath::Abs(Bjet1.DeltaPhi(Qjet1));		
			}
			else {
				EtaBQ1 = Qjet1.Eta()-Bjet2.Eta();
				PhiBQ1 = TMath::Abs(Bjet2.DeltaPhi(Qjet1));	
			}	
		} else if (Bjet1.Eta() >= Bjet2.Eta()) {
				EtaBQ1 = Qjet2.Eta()-Bjet1.Eta();
				PhiBQ1 = TMath::Abs(Bjet1.DeltaPhi(Qjet2));	
				
				}
			else {
				EtaBQ1 = Qjet2.Eta()-Bjet2.Eta();
				PhiBQ1 = TMath::Abs(Bjet2.DeltaPhi(Qjet2));	
			}


		 if (Qjet1.Eta() <= Qjet2.Eta()) {
			if (Bjet1.Eta() <= Bjet2.Eta())  {
				EtaBQ2 = Qjet1.Eta()-Bjet1.Eta();
				PhiBQ2 = TMath::Abs(Bjet1.DeltaPhi(Qjet1));		
			}
			else {
				EtaBQ2 = Qjet1.Eta()-Bjet2.Eta();
				PhiBQ2 = TMath::Abs(Bjet2.DeltaPhi(Qjet1));	
			}	
		} else if (Bjet1.Eta() <= Bjet2.Eta()) {
				EtaBQ2 = Qjet2.Eta()-Bjet1.Eta();
				PhiBQ2 = TMath::Abs(Bjet1.DeltaPhi(Qjet2));	
				
				}
			else {
				EtaBQ2 = Qjet2.Eta()-Bjet2.Eta();
				PhiBQ2 = TMath::Abs(Bjet2.DeltaPhi(Qjet2));	
			}
		


		TMVA.Mqq = Mqq;
		TMVA.CSV1 = Jet_btagCSV[btag_max1_number];	
		TMVA.CSV2 = Jet_btagCSV[btag_max2_number];	
		TMVA.DeltaEtaQQ = qqDeltaEta;
		TMVA.DeltaPhiBB = bbDeltaPhi;
		TMVA.SoftN5 = softActivity_njets5;
		TMVA.HTsoft = softActivity_HT;
		TMVA.DeltaEtaQB1 = EtaBQ1;
		TMVA.DeltaEtaQB2 = EtaBQ2;

		tree0->SetWeight(genWeight);
		tree0->Fill();		

	}  

	file.Write();
	file.Close();

}

