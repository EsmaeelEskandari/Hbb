#define skim_trees_single_cxx
#include "skim_trees_single.h"
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


void skim_trees_single::Loop(TString filename, TString input_filename)
{
   if (fChain == 0) return;

   Long64_t nentries = fChain->GetEntriesFast();

   Long64_t nbytes = 0, nb = 0;

	TFile *output = new TFile(filename,"recreate");
	TTree *tree = fChain->CloneTree(0);
	TFile *input_file = TFile::Open(input_filename);
	TH1F*	Count = (TH1F*)input_file->Get("Count");
	TH1F*	CountPosWeight = (TH1F*)input_file->Get("CountPosWeight");
	TH1F*	CountNegWeight =(TH1F*)input_file->Get("CountNegWeight");
	
	Float_t looser_cuts_coef = 0.95;	

   for (Long64_t jentry=0; jentry<nentries;jentry++) {
	   Long64_t ientry = LoadTree(jentry);
	   if (ientry < 0) break;
	   nb = fChain->GetEntry(jentry);   nbytes += nb;


		if (nJet<4) continue;
	   
		if (!((Jet_pt[0]>92.*looser_cuts_coef)&&(Jet_pt[1]>76.*looser_cuts_coef)&&(Jet_pt[2]>64.*looser_cuts_coef)&&(Jet_pt[3]>30.*looser_cuts_coef))) continue;
	
 
		int loopJet_min = 4;


		Double_t btag_max = 0.7;
		int btag_max1_number = -1;
		int btag_max2_number = -1;
		for (int i=0;i<loopJet_min;i++){
			if ((Jet_btagCSV[i]>btag_max)&&(Jet_id[i]>0)){
				btag_max=Jet_btagCSV[i];
				btag_max1_number=i;
			}
		}
		if (!((btag_max1_number>=0))) continue;
		TLorentzVector Bjet1;
		Bjet1.SetPtEtaPhiM(Jet_pt[btag_max1_number],Jet_eta[btag_max1_number],Jet_phi[btag_max1_number],Jet_mass[btag_max1_number]);


		int pt_max1_number = -1;
		int pt_max2_number = -1;

		TLorentzVector js[3];
		int jcount = 0;
		int j_num[3] = {};
		for (int i=0;i<4;i++){
			if ((i!=btag_max1_number)&&(Jet_id[i]>0)) {
				js[jcount].SetPtEtaPhiM(Jet_pt[i], Jet_eta[i], Jet_phi[i], Jet_mass[i]);
				j_num[jcount] = i;
				jcount++;
			}
		}	
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
		
		pt_max1_number = j_num[ eta_num[max_deltaEta_num][0]];
		pt_max2_number = j_num[ eta_num[max_deltaEta_num][1]];

		if (!((pt_max1_number>=0)&&(pt_max2_number>=0))) continue;
	
		TLorentzVector Qjet1;
		Qjet1.SetPtEtaPhiM(Jet_pt[pt_max1_number] ,Jet_eta[pt_max1_number],Jet_phi[pt_max1_number],Jet_mass[pt_max1_number]);
	
		TLorentzVector Qjet2;
		Qjet2.SetPtEtaPhiM(Jet_pt[pt_max2_number],Jet_eta[pt_max2_number],Jet_phi[pt_max2_number],Jet_mass[pt_max2_number]);

		for (int i=0;i<4;i++){
			if ( (i!=btag_max1_number)&&(i!=pt_max1_number)&&(i!=pt_max2_number)&&(Jet_id[i]>0)) btag_max2_number=i;
		}

		if (!((btag_max2_number>=0))) continue;

		TLorentzVector Bjet2;
		Bjet2.SetPtEtaPhiM(Jet_pt[btag_max2_number],Jet_eta[btag_max2_number],Jet_phi[btag_max2_number],Jet_mass[btag_max2_number]);

		TLorentzVector qq;
		qq = Qjet1+Qjet2;
		Double_t Mqq = qq.M();
		Double_t bbDeltaPhi = TMath::Abs(Bjet1.DeltaPhi(Bjet2));
		Double_t qqDeltaEta = TMath::Abs(Qjet1.Eta()-Qjet2.Eta());
		if (!((Mqq>460*looser_cuts_coef)&&(qqDeltaEta>4.1)&&(bbDeltaPhi<1.6))) continue;

		if (HLT_BIT_HLT_QuadPFJet_SingleBTagCSV_VBF_Mqq460_v!=1) continue;

		tree->Fill();		

	}  
	output->cd();
	tree->AutoSave();
	Count->Write();
	CountPosWeight->Write();
	CountNegWeight->Write();
	output->Close();
	delete output;
	
}
