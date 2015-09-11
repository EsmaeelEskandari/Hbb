#define MakeB_cxx
#include "MakeB.h"
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

typedef std::map<double, int> JetList;

const Int_t njets = 15;
typedef struct {
   Float_t pt[njets];
   Float_t eta[njets];
	Float_t btagCSV[njets];
   Int_t pt_idx[njets];
   Int_t eta_idx[njets];
	Int_t btagCSV_idx[njets];
	Int_t type[njets];
	Float_t blike_b[njets];
	Float_t blike_q[njets];
	Int_t ch_mult[njets];
	Float_t ptd[njets];
} TreeJets;

using namespace std;

void MakeB::Loop()
{
   if (fChain == 0) return;
   Long64_t nentries = fChain->GetEntriesFast();
   Long64_t nbytes = 0, nb = 0;

	TreeJets TreeJet;
	Int_t loopJet_max;
	TFile *output = new TFile("b_likelihood_speedup_04_caterinaidx_b.root","recreate");
	TTree *tree = fChain->CloneTree();
	TBranch *blike_b = tree->Branch("Jet_blikelihood_b",TreeJet.blike_b,"Jet_blikelihood_b[15]/F");
//	TBranch *blike_q = tree->Branch("Jet_blikelihood_q",TreeJet.blike_q,"Jet_blikelihood_q[15]/F");
	TString weightfile_b = "weights/TMVAClassification_BDTG_bjet_04_catidx_b.weights.xml";
//	TString weightfile_q = "weights/TMVAClassification_BDTG_qjet_04_catidx.weights.xml";
   TMVA::Reader *reader_b = new TMVA::Reader("Silent");
	float var1,var2,var3,var4,var5,var6,var7,var8;
//	reader_b->AddVariable("Jet_pt",&var1);
	reader_b->AddVariable("Jet_eta",&var2);
	reader_b->AddVariable("Jet_btagCSV",&var3);
//	reader_b->AddVariable("Jet_pt_idx",&var4);
	reader_b->AddVariable("Jet_eta_idx",&var5);
//	reader_b->AddVariable("Jet_btagCSV_idx",&var6);
   reader_b->AddVariable( "Jet_chMult", &var7 );
   reader_b->AddVariable( "Jet_ptd", &var8 );
	reader_b->BookMVA("BDTG", weightfile_b);	/*
   TMVA::Reader *reader_q = new TMVA::Reader("Silent");
	reader_q->AddVariable("Jet_pt",&var1);
	reader_q->AddVariable("Jet_eta",&var2);
	reader_q->AddVariable("Jet_btagCSV",&var3);
//	reader_q->AddVariable("Jet_pt_idx",&var4);
	reader_q->AddVariable("Jet_eta_idx",&var5);
//	reader_q->AddVariable("Jet_btagCSV_idx",&var6);
	reader_q->BookMVA("BDTG", weightfile_q);	*/

   for (Long64_t jentry=0; jentry<nentries;jentry++) {
      Long64_t ientry = LoadTree(jentry);
      nb = fChain->GetEntry(jentry);   nbytes += nb;

	   JetList jetList_CSV, jetList_pt, jetList_eta; 


		int loopJet_max;
		if (nJet<6) loopJet_max = nJet; 
		else loopJet_max = 6; 

	   for(int i=0; i<nJet; i++){
		   if(Jet_pt[i]<20 || Jet_id[i] <0) continue; 
			if (Jet_btagCSV[i]==-10) Jet_btagCSV[i]=0; 
		   jetList_CSV[Jet_btagCSV[i]]=i;
		   jetList_pt[Jet_pt[i]]=i;
         jetList_eta[TMath::Abs(Jet_eta[i])]=i;
	   }

		for (int i=0;i<nJet;i++){
			TreeJet.blike_b[i] = -2;
		//	TreeJet.blike_q[i] = -2;
		   TreeJet.pt[i]=Jet_pt[i];
		   TreeJet.eta[i]=Jet_eta[i]; 
		   TreeJet.btagCSV[i]=Jet_btagCSV[i]; 
			TreeJet.ch_mult[i]=Jet_chMult[i];
			TreeJet.ptd[i]=Jet_ptd[i];
		

		   for (JetList::reverse_iterator iJet=jetList_CSV.rbegin(); iJet!=jetList_CSV.rend(); ++iJet){
			   if(iJet->first==Jet_btagCSV[i]) { TreeJet.btagCSV_idx[i] = iJet->second; }
		   } 	
		
		   for (JetList::reverse_iterator iJet=jetList_pt.rbegin(); iJet!=jetList_pt.rend(); ++iJet){
         	if(iJet->first==Jet_pt[i]) { TreeJet.pt_idx[i] = iJet->second; }
         }	

		   for (JetList::reverse_iterator iJet=jetList_eta.rbegin(); iJet!=jetList_eta.rend(); ++iJet){
         	if(iJet->first==TMath::Abs(Jet_eta[i])) { TreeJet.eta_idx[i] = iJet->second; }
         }	

		}




	for (int i=0;i<nJet;i++){
		   if(Jet_pt[i]<20 || Jet_id[i] <0) continue; 
		//	var1 = TreeJet.pt[i];
			var2 = TreeJet.eta[i];
			var3 = TreeJet.btagCSV[i];
		//	var4 = TreeJet.pt_idx[i];
			var5 = TreeJet.eta_idx[i];
		//	var6 = TreeJet.btagCSV_idx[i];
			var7 = TreeJet.ch_mult[i];
			var8 = TreeJet.ptd[i];
			TreeJet.blike_b[i] = reader_b->EvaluateMVA("BDTG");
/*
			var1 = TreeJet.pt[i];
			var2 = TreeJet.eta[i];
			var3 = TreeJet.btagCSV[i];
			var4 = TreeJet.pt_idx[i];
			var5 = TreeJet.eta_idx[i];
			var6 = TreeJet.btagCSV_idx[i];
			TreeJet.blike_q[i] = reader_q->EvaluateMVA("BDTG");*/
	}
	blike_b->Fill();
//	blike_q->Fill();
	}
	delete reader_b;
//	delete reader_q;
	output->cd();
	tree->AutoSave();
	output->Close();
}
