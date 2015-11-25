#define MakeQGL_cxx
#include "MakeQGL.h"
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
#include "/afs/cern.ch/work/n/nchernya/Hbb/preselection_single_blike.C"
#include "/afs/cern.ch/work/n/nchernya/Hbb/preselection_double.C"


typedef std::map<double, int> JetList;

using namespace std;

const Int_t njets = 15;
typedef struct {
   Float_t pt[njets];
   Float_t eta[njets];
	Float_t ptd[njets];
	Float_t axis2[njets];
	Float_t mult[njets];
	Float_t qgl_1[njets];
	Float_t qgl_2[njets];
} TreeJets;

using namespace std;

void MakeQGL::Loop(TString input_filename, TString output_dir,int set_type)
{
   if (fChain == 0) return;
   Long64_t nentries = fChain->GetEntriesFast();
   Long64_t nbytes = 0, nb = 0;


	TString dataset_type[2] = {"_double","_single"};	

	TFile *input_file = TFile::Open(input_filename);
	TH1F*	Count = (TH1F*)input_file->Get("Count");
	TH1F*	CountPosWeight = (TH1F*)input_file->Get("CountPosWeight");
	TH1F*	CountNegWeight =(TH1F*)input_file->Get("CountNegWeight");
	TH1F*	CountWeighted =(TH1F*)input_file->Get("CountWeighted");

	Int_t presel=0;
	TreeJets TreeJet;
	Int_t loopJet_max;
	TFile *output = TFile::Open(output_dir+dataset_type[set_type]+".root","recreate");
	TTree *tree = fChain->CloneTree(0);
	TBranch *qgl_1 = tree->Branch("Jet_qgl1_VBF",TreeJet.qgl_1,"Jet_qgl1_VBF[nJet]/F");
	TBranch *qgl_2 = tree->Branch("Jet_qgl2_VBF",TreeJet.qgl_2,"Jet_qgl2_VBF[nJet]/F");
	TString weightfile_1 = "/shome/nchernya/Hbb/skim_trees/v14/qgd/weights/TMVAClassification_LikelihoodKDE"+dataset_type[set_type]+"_1";
	TString weightfile_2 = "/shome/nchernya/Hbb/skim_trees/v14/qgd/weights/TMVAClassification_LikelihoodKDE"+dataset_type[set_type]+"_2";
	weightfile_1.Append(".weights.xml");
	weightfile_2.Append(".weights.xml");
   TMVA::Reader *reader_1 = new TMVA::Reader("Silent");
   TMVA::Reader *reader_2 = new TMVA::Reader("Silent");
	float var1,var2,var3,var4,var12,var22,var32,var42,var5,var52;
	reader_1->AddVariable("Jet_pt",&var1);
	reader_1->AddVariable("Jet_eta",&var2);
	reader_1->AddVariable("Jet_ptd",&var3);
	reader_1->AddVariable("Jet_axis2",&var4);
	reader_1->AddVariable("Jet_mult",&var5);
	reader_1->BookMVA("LikelihoodKDE", weightfile_1);
	reader_2->AddVariable("Jet_pt",&var1);
	reader_2->AddVariable("Jet_eta",&var2);
	reader_2->AddVariable("Jet_ptd",&var3);
	reader_2->AddVariable("Jet_axis2",&var4);
	reader_2->AddVariable("Jet_mult",&var5);
	reader_2->BookMVA("LikelihoodKDE", weightfile_2);

	Int_t total_events=0;	

   for (Long64_t jentry=0; jentry<nentries;jentry++) {
      Long64_t ientry = LoadTree(jentry);
      nb = fChain->GetEntry(jentry);   nbytes += nb;

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
	

		for (int i=0;i<nJet;i++){
			TreeJet.qgl_1[i] = -2;
			TreeJet.qgl_2[i] = -2;
		}
	
		if (set_type==0) {
	//	 	if (preselection_single_blike(nJet, Jet_pt,Jet_eta, Jet_phi, Jet_mass, Jet_blike_VBF, Jet_id, btag_max1_number, btag_max2_number, pt_max1_number, pt_max2_number, HLT_BIT_HLT_QuadPFJet_SingleBTagCSV_VBF_Mqq460_v, Bjet1, Bjet2, Qjet1, Qjet2, qq) == 0) continue;
	//		if (preselection_double(nJet, Jet_pt,Jet_eta, Jet_phi, Jet_mass, Jet_btagCSV, Jet_id, btag_max1_number, btag_max2_number, pt_max1_number, pt_max2_number, HLT_BIT_HLT_QuadPFJet_DoubleBTagCSV_VBF_Mqq200_v, Bjet1, Bjet2, Qjet1, Qjet2, qq) != 0) continue;
			preselection_double(nJet, Jet_pt,Jet_eta, Jet_phi, Jet_mass, Jet_btagCSV, Jet_id, btag_max1_number, btag_max2_number, pt_max1_number, pt_max2_number, HLT_BIT_HLT_QuadPFJet_DoubleBTagCSV_VBF_Mqq200_v, Bjet1, Bjet2, Qjet1, Qjet2, qq);
			if (!( (btag_max1_number>=0)&&(btag_max2_number>=0)&&(pt_max1_number>=0)&&(pt_max2_number>=0))) continue;
		}
		else {
	//		if (set_type==1)	if (preselection_single_blike(nJet, Jet_pt,Jet_eta, Jet_phi, Jet_mass, Jet_blike_VBF, Jet_id, btag_max1_number, btag_max2_number, pt_max1_number, pt_max2_number, HLT_BIT_HLT_QuadPFJet_SingleBTagCSV_VBF_Mqq460_v, Bjet1, Bjet2, Qjet1, Qjet2, qq) != 0) continue;
	preselection_single_blike(nJet, Jet_pt,Jet_eta, Jet_phi, Jet_mass, Jet_blike_VBF, Jet_id, btag_max1_number, btag_max2_number, pt_max1_number, pt_max2_number, HLT_BIT_HLT_QuadPFJet_SingleBTagCSV_VBF_Mqq460_v, Bjet1, Bjet2, Qjet1, Qjet2, qq); 
			if (!( (btag_max1_number>=0)&&(btag_max2_number>=0)&&(pt_max1_number>=0)&&(pt_max2_number>=0))) continue;
		}


		presel+=TMath::Sign(1.,genWeight);

		int loopJet_max = 7;
		if (nJet<7) loopJet_max = nJet; 



	for (int i=0;i<loopJet_max;i++){
		   if(Jet_pt[i]<20) continue; 
			var1 = Jet_pt[i];
			var2 = TMath::Abs(Jet_eta[i]);
			var3 = Jet_ptd[i];
			var4 = Jet_axis2[i];
			var5 = Jet_mult[i];
			TreeJet.qgl_1[i] = reader_1->EvaluateMVA("LikelihoodKDE");
			TreeJet.qgl_2[i] = reader_2->EvaluateMVA("LikelihoodKDE");
		}

	//	total_events++;
	//	if (total_events>=200000) continue;
	

	tree->Fill();
	}
	cout<<presel<<endl;
	delete reader_1;
	delete reader_2;
	output->cd();
	tree->AutoSave();
	Count->Write();
	CountPosWeight->Write();
	CountNegWeight->Write();
	CountWeighted->Write();
	output->Close();
}

