#define CreateTree_tmva_all_cxx
#include "CreateTree_tmva_all.h"
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
#include "/afs/cern.ch/work/n/nchernya/Hbb/preselection_double.C"
#include "/afs/cern.ch/work/n/nchernya/Hbb/preselection_single_blike.C"


const int njets = 300;

typedef struct {
	Float_t CSV1;
	Float_t CSV2;
	Float_t Mqq;
	Float_t Mbb;
	Float_t DeltaEtaQQ;
	Float_t DeltaPhiQQ;
	Int_t SoftN5;
	Float_t HTsoft;
	Float_t DeltaEtaQB1;
	Float_t DeltaEtaQB2;
	Float_t cosOqqbb;
	Float_t qgl1_VBF;
	Float_t qgl2_VBF;
	Float_t x1;
	Float_t x2;
	Float_t VB1;
	Float_t VB2;
	Float_t Jet5_pt;
	Float_t Etot;
	Float_t blike1_VBF;
	Float_t blike2_VBF;
}TMVAstruct;


using namespace std;

void CreateTree_tmva_all::Loop(TString input_filename,TString output_dir, int data, int set_type)
{
   if (fChain == 0) return;

   Long64_t nentries = fChain->GetEntriesFast();

   Long64_t nbytes = 0, nb = 0;
	TMVAstruct TMVA;

	TString dataset_type[2] = {"_double","_single"};	

	int events_saved=0;
	
 

	TFile file("main_tmva_tree_"+output_dir+"_v14"+dataset_type[set_type]+".root","recreate");
	TTree *tree0 = new TTree("TMVA","TMVA");
	tree0->Branch("CSV1",&TMVA.CSV1,"CSV1/F");
	tree0->Branch("CSV2",&TMVA.CSV2,"CSV2/F");
	tree0->Branch("Mqq",&TMVA.Mqq,"Mqq/F");
	tree0->Branch("Mbb",&TMVA.Mbb,"Mbb/F");
	tree0->Branch("DeltaEtaQQ",&TMVA.DeltaEtaQQ,"DeltaEtaQQ/F");
	tree0->Branch("DeltaPhiQQ",&TMVA.DeltaPhiQQ,"DeltaPhiQQ/F");
	tree0->Branch("SoftN5",&TMVA.SoftN5,"SoftN5/I");
	tree0->Branch("HTsoft",&TMVA.HTsoft,"HTsoft/F");
	tree0->Branch("DeltaEtaQB1",&TMVA.DeltaEtaQB1,"DeltaEtaQB1/F");
	tree0->Branch("DeltaEtaQB2",&TMVA.DeltaEtaQB2,"DeltaEtaQB2/F");
	tree0->Branch("cosOqqbb",&TMVA.cosOqqbb,"cosOqqbb/F");
	tree0->Branch("qgl1_VBF",&TMVA.qgl1_VBF,"qgl1_VBF/F");
	tree0->Branch("qgl2_VBF",&TMVA.qgl2_VBF,"qgl2_VBF/F");
	tree0->Branch("Etot",&TMVA.Etot,"Etot/F");
	tree0->Branch("Jet5_pt",&TMVA.Jet5_pt,"Jet5_pt/F");
	tree0->Branch("x1",&TMVA.x1,"x1/F");
	tree0->Branch("x2",&TMVA.x2,"x2/F");
	tree0->Branch("VB1",&TMVA.VB1,"VB1/F");
	tree0->Branch("VB2",&TMVA.VB2,"VB2/F");
	tree0->Branch("blike1_VBF",&TMVA.blike1_VBF,"blike1_VBF/F");
	tree0->Branch("blike2_VBF",&TMVA.blike2_VBF,"blike2_VBF/F");



   for (Long64_t jentry=0; jentry<nentries;jentry++) {
	   Long64_t ientry = LoadTree(jentry);
	   if (ientry < 0) break;
	   nb = fChain->GetEntry(jentry);   nbytes += nb;
			   
		if (data==1) genWeight = 1.;
		if (genWeight <0) continue;
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
			if (preselection_single_blike(nJet, Jet_pt,Jet_eta, Jet_phi, Jet_mass, Jet_blike_VBF, Jet_id, btag_max1_number, btag_max2_number, pt_max1_number, pt_max2_number, HLT_BIT_HLT_QuadPFJet_SingleBTagCSV_VBF_Mqq460_v, Bjet1, Bjet2, Qjet1, Qjet2, qq) ==0) continue;

			if (preselection_double(nJet, Jet_pt,Jet_eta, Jet_phi, Jet_mass, Jet_btagCSV, Jet_id, btag_max1_number, btag_max2_number, pt_max1_number, pt_max2_number, HLT_BIT_HLT_QuadPFJet_DoubleBTagCSV_VBF_Mqq200_v, Bjet1, Bjet2, Qjet1, Qjet2, qq) !=0 ) continue;
		}
		else {
			if (set_type==1) if (preselection_single_blike(nJet, Jet_pt,Jet_eta, Jet_phi, Jet_mass, Jet_blike_VBF, Jet_id, btag_max1_number, btag_max2_number, pt_max1_number, pt_max2_number, HLT_BIT_HLT_QuadPFJet_SingleBTagCSV_VBF_Mqq460_v, Bjet1, Bjet2, Qjet1, Qjet2, qq) !=0) continue;
		}

		Float_t alpha_bjet1_reg = 1; 
		Float_t alpha_bjet2_reg = 1 ;
		if (Jet_pt_regVBF[btag_max1_number]>0) alpha_bjet1_reg = Jet_pt_regVBF[btag_max1_number]/Jet_pt[btag_max1_number];
  		if (Jet_pt_regVBF[btag_max2_number]>0) alpha_bjet2_reg = Jet_pt_regVBF[btag_max2_number]/Jet_pt[btag_max2_number];
		Bjet1*=alpha_bjet1_reg;
		Bjet2*=alpha_bjet2_reg;

		
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
		TMVA.qgl1_VBF = Jet_qgl1_VBF[pt_max1_number];
		TMVA.qgl2_VBF = Jet_qgl2_VBF[pt_max2_number];
		TMVA.Jet5_pt = Jet_pt[4];
		TMVA.Etot = Etot;
		TMVA.x1 = x1;
		TMVA.x2 = x2;
		TMVA.VB1 = VB1_mass;
		TMVA.VB2 = VB2_mass;
		TMVA.blike1_VBF = Jet_blike_VBF[btag_max1_number];
		TMVA.blike2_VBF = Jet_blike_VBF[btag_max2_number];
		TMVA.Mbb = Mbb;

		tree0->Fill();	
		events_saved++;		
		if (events_saved>=200000) break;

	}  

	file.Write();
	file.Close();

}

