#define CreateTree_tmva_qgl_cxx
#include "CreateTree_tmva_qgl.h"
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
#include <vector>
#include "/afs/cern.ch/work/n/nchernya/Hbb/preselection_double.C"
#include "/afs/cern.ch/work/n/nchernya/Hbb/preselection_single_blike.C"



const int njets = 300;
typedef struct {
   Float_t pt;
   Float_t eta;
   Float_t ptd;
   Float_t axis2;	
  	Int_t mult;
} TreeJets;


using namespace std;

void CreateTree_tmva_qgl::Loop(TString input_filename,TString output_dir,  int sample_type, int set_type)
{
   if (fChain == 0) return;

   Long64_t nentries = fChain->GetEntriesFast();

   Long64_t nbytes = 0, nb = 0;


	TString dataset_type[2] = {"_double","_single"};	

	int events_saved=0;
	
	TFile *file_initial = new TFile(input_filename);
	TH1F *countPos = (TH1F*)file_initial->Get("CountPosWeight");
	TH1F *countNeg = (TH1F*)file_initial->Get("CountNegWeight");
	Int_t events_generated = countPos->GetEntries()-countNeg->GetEntries();
 

	TFile file("qgl_tmva_tree_"+output_dir+"_v14"+dataset_type[set_type]+"_2jets.root","recreate");
	TTree *tree1 = new TTree("QGL_1","QGL_2");
	TTree *tree2 = new TTree("QGL_2","QGL_2");
	TreeJets TreeJet;
	tree1->Branch("Jet_pt",&TreeJet.pt,"Jet_pt/F");
	tree1->Branch("Jet_eta",&TreeJet.eta,"Jet_eta/F");
	tree1->Branch("Jet_ptd",&TreeJet.ptd,"Jet_ptd/F");
	tree1->Branch("Jet_axis2",&TreeJet.axis2,"Jet_axis2/F");
	tree1->Branch("Jet_mult",&TreeJet.mult,"Jet_mult/I");

	tree2->Branch("Jet_pt",&TreeJet.pt,"Jet_pt/F");
	tree2->Branch("Jet_eta",&TreeJet.eta,"Jet_eta/F");
	tree2->Branch("Jet_ptd",&TreeJet.ptd,"Jet_ptd/F");
	tree2->Branch("Jet_axis2",&TreeJet.axis2,"Jet_axis2/F");
	tree2->Branch("Jet_mult",&TreeJet.mult,"Jet_mult/I");
	
	


   for (Long64_t jentry=0; jentry<nentries;jentry++) {
	   Long64_t ientry = LoadTree(jentry);
	   if (ientry < 0) break;
	   nb = fChain->GetEntry(jentry);   nbytes += nb;
		  
		if (sample_type==1) genWeight=1.;
 
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
		 	if (preselection_single_blike(nJet, Jet_pt,Jet_eta, Jet_phi, Jet_mass, Jet_blike_VBF, Jet_id, btag_max1_number, btag_max2_number, pt_max1_number, pt_max2_number, HLT_BIT_HLT_QuadPFJet_SingleBTagCSV_VBF_Mqq460_v, Bjet1, Bjet2, Qjet1, Qjet2, qq) == 0) continue;
			if (preselection_double(nJet, Jet_pt,Jet_eta, Jet_phi, Jet_mass, Jet_btagCSV, Jet_id, btag_max1_number, btag_max2_number, pt_max1_number, pt_max2_number, HLT_BIT_HLT_QuadPFJet_DoubleBTagCSV_VBF_Mqq200_v, Bjet1, Bjet2, Qjet1, Qjet2, qq) != 0) continue;
		}
		else {
			if (set_type==1)	if (preselection_single_blike(nJet, Jet_pt,Jet_eta, Jet_phi, Jet_mass, Jet_blike_VBF, Jet_id, btag_max1_number, btag_max2_number, pt_max1_number, pt_max2_number, HLT_BIT_HLT_QuadPFJet_SingleBTagCSV_VBF_Mqq460_v, Bjet1, Bjet2, Qjet1, Qjet2, qq) != 0) continue;
		}
			

		
		TreeJet.pt=Jet_pt[pt_max1_number];
		TreeJet.eta=TMath::Abs(Jet_eta[pt_max1_number]); 
		TreeJet.ptd=Jet_ptd[pt_max1_number];
		TreeJet.axis2=Jet_axis2[pt_max1_number];
		TreeJet.mult=Jet_mult[pt_max1_number];
	
		if (sample_type==0) {
			TLorentzVector GenHiggsSisters[2];
			if (GenHiggsSisters_pt[0]>GenHiggsSisters_pt[1]) {
				GenHiggsSisters[0].SetPtEtaPhiM(GenHiggsSisters_pt[0],GenHiggsSisters_eta[0],GenHiggsSisters_phi[0],GenHiggsSisters_mass[0]) ;
				GenHiggsSisters[1].SetPtEtaPhiM(GenHiggsSisters_pt[1],GenHiggsSisters_eta[1],GenHiggsSisters_phi[1],GenHiggsSisters_mass[1]) ;
			}
			else {
				GenHiggsSisters[0].SetPtEtaPhiM(GenHiggsSisters_pt[1],GenHiggsSisters_eta[1],GenHiggsSisters_phi[1],GenHiggsSisters_mass[1]) ;
				GenHiggsSisters[1].SetPtEtaPhiM(GenHiggsSisters_pt[0],GenHiggsSisters_eta[0],GenHiggsSisters_phi[0],GenHiggsSisters_mass[0]) ;
			}
	
			for (int j=0;j<2;j++){		
				if (Qjet1.DeltaR(GenHiggsSisters[j])<0.8) tree1->Fill();
			}
			TreeJet.pt=Jet_pt[pt_max2_number];
			TreeJet.eta=TMath::Abs(Jet_eta[pt_max2_number]); 
			TreeJet.ptd=Jet_ptd[pt_max2_number];
			TreeJet.axis2=Jet_axis2[pt_max2_number];
			TreeJet.mult=Jet_mult[pt_max2_number];
			for (int j=0;j<2;j++){		
				if (Qjet2.DeltaR(GenHiggsSisters[j])<0.8) tree2->Fill();
			}
		} else  {
			TreeJet.pt=Jet_pt[pt_max1_number];
			TreeJet.eta=TMath::Abs(Jet_eta[pt_max1_number]); 
			TreeJet.ptd=Jet_ptd[pt_max1_number];
			TreeJet.axis2=Jet_axis2[pt_max1_number];
			TreeJet.mult=Jet_mult[pt_max1_number];
	
			tree1->Fill();
	
			TreeJet.pt=Jet_pt[pt_max2_number];
			TreeJet.eta=TMath::Abs(Jet_eta[pt_max2_number]); 
			TreeJet.ptd=Jet_ptd[pt_max2_number];
			TreeJet.axis2=Jet_axis2[pt_max2_number];
			TreeJet.mult=Jet_mult[pt_max2_number];		

			tree2->Fill();	
		}


		events_saved++;
		if (events_saved>=200000	) break;	

	}  

	file.Write();
	file.Close();

}

