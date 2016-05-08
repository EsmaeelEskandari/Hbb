#define skim_trees_triggers_cxx
#include "skim_trees_triggers.h"
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


void skim_trees_triggers::Loop(TString filename, TString input_filename, int trigger, int current_sample, int data)
{
	
	TString denominator_trigger_names[3] = { "HLT_BIT_HLT_PFJet60_v"};
	if (data==1) denominator_trigger_names[0] = {"HLT_BIT_HLT_DiPFJetAve60_v"};

   if (fChain == 0) return;

   Long64_t nentries = fChain->GetEntriesFast();

   Long64_t nbytes = 0, nb = 0;

	TFile *output = new TFile(filename+"_"+denominator_trigger_names[trigger]+".root","recreate");
	fChain->SetBranchStatus("*",0);
	fChain->SetBranchStatus("nJet",1);
	fChain->SetBranchStatus("Jet_pt",1);
	fChain->SetBranchStatus("Jet_pt*",1);
	fChain->SetBranchStatus("Jet_eta",1);
	fChain->SetBranchStatus("Jet_phi",1);
	fChain->SetBranchStatus("Jet_mass",1);
	fChain->SetBranchStatus("Jet_btagCSV",1);
	fChain->SetBranchStatus("*id",1);
	fChain->SetBranchStatus("*blike*",1);
	fChain->SetBranchStatus("*type",1);
   fChain->SetBranchStatus("*eight",1);
   fChain->SetBranchStatus("json",1);
	fChain->SetBranchStatus("HLT_*VBF*",1);
	fChain->SetBranchStatus("HLT_*PFJet*",1);
	
	TTree *tree = fChain->CloneTree(0);
	TFile *input_file = TFile::Open(input_filename);
	TH1F*	Count = (TH1F*)input_file->Get("Count");
	TH1F*	CountPosWeight = (TH1F*)input_file->Get("CountPosWeight");
	TH1F*	CountNegWeight =(TH1F*)input_file->Get("CountNegWeight");
	TH1F* CountWeighted = (TH1F*)input_file->Get("CountWeighted");


   for (Long64_t jentry=0; jentry<nentries;jentry++) {
	   Long64_t ientry = LoadTree(jentry);
	   if (ientry < 0) break;
	   nb = fChain->GetEntry(jentry);   nbytes += nb;
		Float_t denominator_trigger[4] = { HLT_BIT_HLT_PFJet60_v};
		if (data==1) denominator_trigger[0] = {HLT_BIT_HLT_DiPFJetAve60_v};
	 
		if (denominator_trigger[trigger]!=1) continue;

		tree->Fill();		

	}  
	output->cd();
	tree->AutoSave();
	Count->Write();
	CountPosWeight->Write();
	CountNegWeight->Write();
	CountWeighted->Write();
	output->Close();
	delete output;

}

