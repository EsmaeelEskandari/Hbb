#define CreateTree_tmva_single_cxx
#include "CreateTree_tmva_single.h"
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
#include "/afs/cern.ch/work/n/nchernya/Hbb/preselection_single.C"
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
	Float_t DeltaPhiQQ;
	Int_t SoftN5;
	Float_t HTsoft;
	Float_t DeltaEtaQB1;
	Float_t DeltaEtaQB2;
	Float_t cosOqqbb;
	Float_t qgl1;
	Float_t qgl2;
	Float_t x1;
	Float_t x2;
	Float_t VB1;
	Float_t VB2;
	Float_t Jet5_pt;
	Float_t Etot;
}TMVAstruct;




void CreateTree_tmva_single::Loop(TString input_filename,TString output_dir,  int sample_type)
{
   if (fChain == 0) return;

   Long64_t nentries = fChain->GetEntriesFast();

   Long64_t nbytes = 0, nb = 0;
	TMVAstruct TMVA;


	int events_saved=0;
	
	TFile *file_initial = new TFile(input_filename);
	TH1F *countPos = (TH1F*)file_initial->Get("CountPosWeight");
	TH1F *countNeg = (TH1F*)file_initial->Get("CountNegWeight");
	Int_t events_generated = countPos->GetEntries()-countNeg->GetEntries();
//	genWeight/=events_generated/xsec[sample_type];
 
	TFile file("main_tmva_tree_"+output_dir+"_v13_single.root","recreate");
	TTree *tree0 = new TTree("TMVA","TMVA");
	tree0->Branch("CSV1",&TMVA.CSV1,"CSV1/F");
	tree0->Branch("CSV2",&TMVA.CSV2,"CSV2/F");
	tree0->Branch("Mqq",&TMVA.Mqq,"Mqq/F");
	tree0->Branch("DeltaEtaQQ",&TMVA.DeltaEtaQQ,"DeltaEtaQQ/F");
	tree0->Branch("DeltaPhiQQ",&TMVA.DeltaPhiQQ,"DeltaPhiQQ/F");
	tree0->Branch("SoftN5",&TMVA.SoftN5,"SoftN5/I");
	tree0->Branch("HTsoft",&TMVA.HTsoft,"HTsoft/F");
	tree0->Branch("DeltaEtaQB1",&TMVA.DeltaEtaQB1,"DeltaEtaQB1/F");
	tree0->Branch("DeltaEtaQB2",&TMVA.DeltaEtaQB2,"DeltaEtaQB2/F");
	tree0->Branch("cosOqqbb",&TMVA.cosOqqbb,"cosOqqbb/F");
	tree0->Branch("qgl1",&TMVA.qgl1,"qgl1/F");
	tree0->Branch("qgl2",&TMVA.qgl2,"qgl2/F");
	tree0->Branch("Etot",&TMVA.Etot,"Etot/F");
	tree0->Branch("Jet5_pt",&TMVA.Jet5_pt,"Jet5_pt/F");
	tree0->Branch("x1",&TMVA.x1,"x1/F");
	tree0->Branch("x2",&TMVA.x2,"x2/F");
	tree0->Branch("VB1",&TMVA.VB1,"VB1/F");
	tree0->Branch("VB2",&TMVA.VB2,"VB2/F");
	



   for (Long64_t jentry=0; jentry<nentries;jentry++) {
	   Long64_t ientry = LoadTree(jentry);
	   if (ientry < 0) break;
	   nb = fChain->GetEntry(jentry);   nbytes += nb;
	  
		if (genWeight<0) continue;
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

		if (preselection_single(nJet, Jet_pt,Jet_eta, Jet_phi, Jet_mass, Jet_btagCSV, Jet_id, btag_max1_number, btag_max2_number, pt_max1_number, pt_max2_number, HLT_BIT_HLT_QuadPFJet_SingleBTagCSV_VBF_Mqq460_v, Bjet1, Bjet2, Qjet1, Qjet2, qq) !=0) continue;
		
		Float_t Mqq = qq.M();
		Float_t bbDeltaPhi = TMath::Abs(Bjet1.DeltaPhi(Bjet2));
		Double_t qqDeltaPhi = TMath::Abs(Qjet1.DeltaPhi(Qjet2));
		Float_t qqDeltaEta = TMath::Abs(Qjet1.Eta()-Qjet2.Eta());
		TLorentzVector bb;
		bb = Bjet1+Bjet2;
		Float_t Mbb = bb.M();
		TLorentzVector bbqq;
		bbqq = Bjet1 + Bjet2 + Qjet1 + Qjet2;
		Float_t cosOqqbb =TMath::Cos( ( ( Bjet1.Vect() ).Cross(Bjet2.Vect()) ).Angle( ( Qjet1.Vect() ).Cross(Qjet2.Vect()) ) );	



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
		

		
		Float_t Etot = Bjet1.E()+Bjet2.E()+Qjet1.E()+Qjet2.E();
		Float_t PzTot = Bjet1.Pz()+Bjet2.Pz()+Qjet1.Pz()+Qjet2.Pz();
		Float_t PxTot = Bjet1.Px()+Bjet2.Px()+Qjet1.Px()+Qjet2.Px();
		Float_t PyTot = Bjet1.Py()+Bjet2.Py()+Qjet1.Py()+Qjet2.Py();
	
		Float_t x1 = 0.;
		Float_t x2 = 0.;
		x1 = (Etot + PzTot)/2./13000.;
		x2 = (Etot - PzTot)/2./13000.;

		TLorentzVector q1,q2,q1_after,q2_after, VB1, VB2;
		q1.SetPxPyPzE(0.,0.,13000./2.*x1,13000./2.*x1);
		q2.SetPxPyPzE(0.,0.,-13000./2.*x2,13000./2.*x2);
		q1_after.SetPxPyPzE(Qjet1.Px()/Qjet1.Beta(),Qjet1.Py()/Qjet1.Beta(),Qjet1.Pz()/Qjet1.Beta(),Qjet1.E());
		q2_after.SetPxPyPzE(Qjet2.Px()/Qjet2.Beta(),Qjet2.Py()/Qjet2.Beta(),Qjet2.Pz()/Qjet2.Beta(),Qjet2.E());
		if (q1_after.Eta()>=0.) {
			VB1 = -q1_after+q1;
			VB2 = -q2_after+q2;
		} else {
			VB1 = -q2_after+q1;
			VB2 = -q1_after+q2;
		} 
		Float_t VB1_mass, VB2_mass;
		VB1_mass = TMath::Abs(VB1.M());
		VB2_mass = TMath::Abs(VB2.M());

		for (int i=0;i<nJet;i++){
			if (Jet_btagCSV[i]>1) Jet_btagCSV[i]=1.;
			if (Jet_btagCSV[i]<0) Jet_btagCSV[i]=0.;
		}

		TMVA.Mqq = Mqq;
		TMVA.CSV1 = Jet_btagCSV[btag_max1_number];	
		TMVA.CSV2 = Jet_btagCSV[btag_max2_number];	
		TMVA.DeltaEtaQQ = qqDeltaEta;
		TMVA.DeltaPhiQQ = qqDeltaPhi;
		TMVA.SoftN5 = softActivity_njets5;
		TMVA.HTsoft = softActivity_HT;
		TMVA.DeltaEtaQB1 = EtaBQ1;
		TMVA.DeltaEtaQB2 = EtaBQ2;
		TMVA.cosOqqbb = cosOqqbb;
		TMVA.qgl1 = Jet_qgl[pt_max1_number];
		TMVA.qgl2 = Jet_qgl[pt_max2_number];
		TMVA.Jet5_pt = Jet_pt[4];
		TMVA.Etot = Etot;
		TMVA.x1 = x1;
		TMVA.x2 = x2;
		TMVA.VB1 = VB1_mass;
		TMVA.VB2 = VB2_mass;

		tree0->Fill();		
		
		events_saved++;		

//		if (sample_type==2) 
//			if (events_saved>=6422) break;

	}  

	file.Write();
	file.Close();

}
