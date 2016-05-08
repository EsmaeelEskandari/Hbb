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
#include "/afs/cern.ch/work/n/nchernya/Hbb/preselection_double.C"
#include "/afs/cern.ch/work/n/nchernya/Hbb/preselection_single_blike.C"

using namespace std;

void skim_trees_single::Loop(TString filename, TString input_filename)
{
   if (fChain == 0) return;

   Long64_t nentries = fChain->GetEntries();
	std::cout<<nentries<<endl;

   Long64_t nbytes = 0, nb = 0;



	fChain->SetBranchStatus("*",0);



	fChain->SetBranchStatus("nJet",1);
   fChain->SetBranchStatus("genWeight",1.);
   fChain->SetBranchStatus("bTagWeight",1.);
	fChain->SetBranchStatus("nTrueInt",1);
	fChain->SetBranchStatus("nPU0",1);
	fChain->SetBranchStatus("nPVs",1);
	fChain->SetBranchStatus("isData",1);
	fChain->SetBranchStatus("xsec",1);
   fChain->SetBranchStatus("Jet_pt",1.);
   fChain->SetBranchStatus("Jet_eta",1.);
   fChain->SetBranchStatus("Jet_phi",1.);
	fChain->SetBranchStatus("Jet_mass",1.);
	fChain->SetBranchStatus("Jet_btagCSV",1.);
	fChain->SetBranchStatus("Jet_blike_VBF",1.);
	fChain->SetBranchStatus("Jet_id",1.);
	fChain->SetBranchStatus("Jet_puId",1.);
   fChain->SetBranchStatus("Jet_qgl",1.);
	fChain->SetBranchStatus("json",1.);

 	fChain->SetBranchStatus("HLT_BIT_HLT_QuadPFJet_DoubleBTagCSV_VBF_Mqq200_v",1.);
 	fChain->SetBranchStatus("HLT_BIT_HLT_QuadPFJet_SingleBTagCSV_VBF_Mqq460_v",1.);

	fChain->SetBranchStatus("HLT_BIT_HLT_DiPFJetAve60_v",1.);
	fChain->SetBranchStatus("HLT_BIT_HLT_DiPFJetAve40_v",1.);
	fChain->SetBranchStatus("HLT_BIT_HLT_L1_TripleJet_VBF_v",1.);
	fChain->SetBranchStatus("HLT_BIT_HLT_QuadPFJet_VBF_v",1.);


///////////////////for normal workflow////////////////not trigger////
   //fChain->SetBranchStatus("*",1.);

	fChain->SetBranchStatus("nselLeptons",1.);
	fChain->SetBranchStatus("selLeptons_pdgId",1.);
	fChain->SetBranchStatus("selLeptons_charge",1.);
   fChain->SetBranchStatus("selLeptons_tightId",1.);
   fChain->SetBranchStatus("selLeptons_relIso03",1.);
   fChain->SetBranchStatus("selLeptons_chargedHadRelIso0",1.);
   fChain->SetBranchStatus("selLeptons_pfRelIso03",1.);
   fChain->SetBranchStatus("vLeptons_dz",1.);
   fChain->SetBranchStatus("vLeptons_ed",1.);
   fChain->SetBranchStatus("nGenHiggsSisters",1.);
   fChain->SetBranchStatus("GenHiggsSisters_pdgId",1.);
   fChain->SetBranchStatus("GenHiggsSisters_pt",1.);
   fChain->SetBranchStatus("GenHiggsSisters_eta",1.);
   fChain->SetBranchStatus("GenHiggsSisters_phi",1.);
   fChain->SetBranchStatus("GenHiggsSisters_mass",1.);
   fChain->SetBranchStatus("GenHiggsSisters_charge",1.);
   fChain->SetBranchStatus("Jet_corr*",1.);
   fChain->SetBranchStatus("Jet_leadTrackPt",1.);
   fChain->SetBranchStatus("Jet_pt*",1.);
   fChain->SetBranchStatus("bTagWeight*",1.);
   fChain->SetBranchStatus("Jet_ptd",1.);
   fChain->SetBranchStatus("Jet_axis2",1.);
   fChain->SetBranchStatus("Jet_mult",1.);
   fChain->SetBranchStatus("met_pt",1.);
   fChain->SetBranchStatus("*softActivity*",1.);
   fChain->SetBranchStatus("lumi",1.);
   fChain->SetBranchStatus("evt",1.);
   fChain->SetBranchStatus("run",1.);
   fChain->SetBranchStatus("puWeight*",1.);
   fChain->SetBranchStatus("json_silver",1.);
   fChain->SetBranchStatus("Vtype",1.);
	fChain->SetBranchStatus("rho*",1.);
	fChain->SetBranchStatus("GenBQuarkFromH_pdgId",1.);
	fChain->SetBranchStatus("GenBQuarkFromH_pt",1.);
	fChain->SetBranchStatus("GenBQuarkFromH_phi",1.);
	fChain->SetBranchStatus("GenBQuarkFromH_eta",1.);
	fChain->SetBranchStatus("GenBQuarkFromH_mass",1.);
	fChain->SetBranchStatus("GenBQuarkFromH_charge",1.);
	fChain->SetBranchStatus("GenBQuarkFromHafterISR_pdgId",1.);
	fChain->SetBranchStatus("GenBQuarkFromHafterISR_pt",1.);
	fChain->SetBranchStatus("GenBQuarkFromHafterISR_phi",1.);
	fChain->SetBranchStatus("GenBQuarkFromHafterISR_eta",1.);
	fChain->SetBranchStatus("GenBQuarkFromHafterISR_mass",1.);
	fChain->SetBranchStatus("GenBQuarkFromHafterISR_charge",1.);
	fChain->SetBranchStatus("nGenBQuarkFromH",1.);
	fChain->SetBranchStatus("nGenBQuarkFromHafterISR",1.);
	fChain->SetBranchStatus("Jet_hadronFlavour",1.);
	fChain->SetBranchStatus("LHE*",1.);
	fChain->SetBranchStatus("lhe*",1.);
	fChain->SetBranchStatus("nLHE*",1.);
//	fChain->SetBranchStatus(,1.);



/////////////////////////SingleElectron part///////////////////
/*
	fChain->SetBranchStatus("*",0);
	fChain->SetBranchStatus("nTrueInt",1);
	fChain->SetBranchStatus("nPU0",1);
	fChain->SetBranchStatus("isData",1);
	fChain->SetBranchStatus("xsec",1);
	fChain->SetBranchStatus("nJet",1);
	fChain->SetBranchStatus("Jet_pt",1);
	fChain->SetBranchStatus("Jet_pt*",1);
	fChain->SetBranchStatus("Jet_eta",1);
	fChain->SetBranchStatus("Jet_phi",1);
	fChain->SetBranchStatus("Jet_mass",1);
	fChain->SetBranchStatus("Jet_btagCSV",1);
	fChain->SetBranchStatus("Jet_id",1);
	fChain->SetBranchStatus("Jet_*",1);
	fChain->SetBranchStatus("met*",1);
	fChain->SetBranchStatus("nPVs",1);
   fChain->SetBranchStatus("*eight*",1);
   fChain->SetBranchStatus("*json*",1);
//	fChain->SetBranchStatus("*trgObjects*",1);
	 fChain->SetBranchStatus("HLT_*Mu*",1);
    fChain->SetBranchStatus("HLT_*Ele*",1);
    fChain->SetBranchStatus("HLT_*VBF*",1);
    fChain->SetBranchStatus("HLT_*Jet*",1);
	fChain->SetBranchStatus("*type",1);
	fChain->SetBranchStatus("*blike*",1);
	fChain->SetBranchStatus("*qgl*",1);
	fChain->SetBranchStatus("soft*",1);
	fChain->SetBranchStatus("GenHiggs*",1);
	fChain->SetBranchStatus("selLeptons*",1);

*/

/////////////////////////////////////////


	std::cout<<filename<<endl;
	TFile *output = TFile::Open(filename,"recreate");
	cout<<filename<<endl;
	TTree *tree = fChain->CloneTree(0);
	TFile *input_file = TFile::Open(input_filename);
	TH1F*	Count = (TH1F*)input_file->Get("Count");
	TH1F*	CountPosWeight = (TH1F*)input_file->Get("CountPosWeight");
	TH1F*	CountNegWeight =(TH1F*)input_file->Get("CountNegWeight");
	TH1F*	CountWeighted =(TH1F*)input_file->Get("CountWeighted");
	TH1F* CountFullWeighted  = (TH1F*)input_file->Get("CountFullWeighted");
	TH1F* CountWeightedLHEWeightScale  = (TH1F*)input_file->Get("CountWeightedLHEWeightScale");
	TH1F* CountWeightedLHEWeightPdf  = (TH1F*)input_file->Get("CountWeightedLHEWeightPdf");

	
	Float_t looser_cuts_coef = 0.9;	

   for (Long64_t jentry=0; jentry<nentries;jentry++) {
	   Long64_t ientry = LoadTree(jentry);
	 //  if (ientry < 0) break;
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
	
//		if (preselection_single(nJet, Jet_pt,Jet_eta, Jet_phi, Jet_mass, Jet_btagCSV, Jet_id, btag_max1_number, btag_max2_number, pt_max1_number, pt_max2_number, /*HLT_BIT_HLT_QuadPFJet_SingleBTagCSV_VBF_Mqq460_v*/1., Bjet1, Bjet2, Qjet1, Qjet2, qq, looser_cuts_coef) !=0) continue;
	

//that one
//	if (preselection_single_blike(nJet, Jet_pt,Jet_eta, Jet_phi, Jet_mass, Jet_blike_VBF, Jet_id, btag_max1_number, btag_max2_number, pt_max1_number, pt_max2_number, /*HLT_BIT_HLT_QuadPFJet_SingleBTagCSV_VBF_Mqq460_v*/1., Bjet1, Bjet2, Qjet1, Qjet2, qq, looser_cuts_coef) !=0) continue;
	

	if (!((HLT_BIT_HLT_QuadPFJet_SingleBTagCSV_VBF_Mqq460_v==1) || (HLT_BIT_HLT_QuadPFJet_DoubleBTagCSV_VBF_Mqq200_v==1))) continue; 
	if (!( (Jet_pt[0] > 82.) && (Jet_pt[1] > 66.) && (Jet_pt[2] > 54.)  && (Jet_pt[3] > 20.))) continue;
	
	if (nJet<4) continue;
//	if (!((HLT_BIT_HLT_QuadPFJet_VBF_v==1) || (HLT_BIT_HLT_DiPFJetAve60_v==1) || (HLT_BIT_HLT_DiPFJetAve40_v==1))) continue; 
//	if (HLT_BIT_HLT_QuadPFJet_VBF_v!=1) continue;

		//if (preselection_double(nJet, Jet_pt,Jet_eta, Jet_phi, Jet_mass, Jet_btagCSV, Jet_id, btag_max1_number, btag_max2_number, pt_max1_number, pt_max2_number, /*HLT_BIT_HLT_QuadPFJet_DoubleBTagCSV_VBF_Mqq200_v*/1., Bjet1, Bjet2, Qjet1, Qjet2, qq, looser_cuts_coef) !=0) continue;
		//
		
	//	if (nJet<4) continue;
//		if (Jet_pt[0]< 80) continue;
//		if (Jet_pt[1]< 60) continue;
//		if (Jet_pt[2]< 40) continue;
//		if (Jet_pt[3]< 10) continue;
				


		tree->Fill();		

	}  
	output->cd();
	tree->AutoSave();
	Count->Write();
	CountPosWeight->Write();
	CountNegWeight->Write();
	CountWeighted->Write();
	CountFullWeighted->Write();
	CountWeightedLHEWeightScale->Write();
	CountWeightedLHEWeightPdf->Write();
	output->Close();
	delete output;
	
}

