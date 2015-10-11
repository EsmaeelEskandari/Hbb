#define TMVAapply_all_cxx
#include "TMVAapply_all.h"
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>
#include "TLorentzVector.h"
#include <iomanip>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "TMVA/Factory.h"
#include "TMVA/Tools.h"
#include "TMVA/Reader.h"
#include "/afs/cern.ch/work/n/nchernya/Hbb/preselection_single.C"
#include "/afs/cern.ch/work/n/nchernya/Hbb/preselection_double.C"

const Int_t njets = 15;

using namespace std;

void TMVAapply_all::Loop(TString inputfile, TString output_dir,  int set_type)
{
   if (fChain == 0) return;
   Long64_t nentries = fChain->GetEntriesFast();
   Long64_t nbytes = 0, nb = 0;

	Int_t presel=0;
	Int_t loopJet_max;
	TFile *output = new TFile("v13/main_tmva_v13_Data_4_"+output_dir+".root","recreate");
	TTree *tree = fChain->CloneTree(0);
	float BDT_VBF;
	TBranch *branchBDT_VBF = tree->Branch("BDT_VBF",&BDT_VBF,"BDT_VBF/F");
	TString weightfile[2]= {"../weights/TMVAClassification_BDTG_v13_Data_double_all_4.weights.xml","../weights/TMVAClassification_BDTG_v13_Data_single_all_4.weights.xml"}
   TMVA::Reader *reader = new TMVA::Reader("Silent");
	float var1,var2,var3,var4,var5,var6,var7,var8,var9,var10, var11, var12;
	reader->AddVariable("Mqq",&var1);
	reader->AddVariable("DeltaEtaQQ",&var2);
	reader->AddVariable("DeltaPhiQQ",&var3);
	reader->AddVariable("SoftN5",&var4);
	reader->AddVariable("HTsoft",&var5);
	reader->AddVariable("CSV1",&var6);
   reader->AddVariable( "CSV2", &var7 );
	reader->AddVariable( "cosOqqbb", &var8 );
   reader->AddVariable( "DeltaEtaQB1", &var9 );
   reader->AddVariable( "DeltaEtaQB2", &var10 );
  	reader->AddVariable("qgl1",&var11);
   reader->AddVariable("qgl2",&var12);
	reader->BookMVA("BDTG", weightfile[set_type]);
	
	TFile *input_file = new TFile(inputfile);
	TH1F*	Count = (TH1F*)input_file->Get("Count");
	TH1F*	CountPosWeight = (TH1F*)input_file->Get("CountPosWeight");
	TH1F*	CountNegWeight =(TH1F*)input_file->Get("CountNegWeight");

	int events_saved=0;
   for (Long64_t jentry=0; jentry<nentries;jentry++) {
      Long64_t ientry = LoadTree(jentry);
      nb = fChain->GetEntry(jentry);   nbytes += nb;
		
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
			if (preselection_double(nJet, Jet_pt,Jet_eta, Jet_phi, Jet_mass, Jet_btagCSV, Jet_id, btag_max1_number, btag_max2_number, pt_max1_number, pt_max2_number, HLT_BIT_HLT_QuadPFJet_DoubleBTagCSV_VBF_Mqq200_v, Bjet1, Bjet2, Qjet1, Qjet2, qq) == -1) continue;
		}
		else {
			if (set_type==1) if (preselection_single(nJet, Jet_pt,Jet_eta, Jet_phi, Jet_mass, Jet_btagCSV, Jet_id, btag_max1_number, btag_max2_number, pt_max1_number, pt_max2_number, HLT_BIT_HLT_QuadPFJet_SingleBTagCSV_VBF_Mqq460_v, Bjet1, Bjet2, Qjet1, Qjet2, qq) == -1) continue;
		}


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

		var1= Mqq;
		var6= Jet_btagCSV[btag_max1_number];	
		var7= Jet_btagCSV[btag_max2_number];	
		var2= qqDeltaEta;
		var3= qqDeltaPhi;
		var4= softActivity_njets5;
		var5= softActivity_HT;
		var9= EtaBQ1;
		var10= EtaBQ2;
		var8= cosOqqbb;
		var11=Jet_qgl[pt_max1_number];
		var12 = Jet_qgl[pt_max2_number];
	
		BDT_VBF = reader->EvaluateMVA("BDTG");


	tree->Fill();
	//	events_saved++;		
//		if (sample_type==2) 
//			if (events_saved>=108767) break; //for 500-700

	}
	delete reader;
	output->cd();
	tree->AutoSave();
	Count->Write();
	CountPosWeight->Write();
	CountNegWeight->Write();
	output->Close();
}
