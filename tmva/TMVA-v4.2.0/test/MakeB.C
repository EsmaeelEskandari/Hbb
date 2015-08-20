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
#define SWAP(A, B) { Float_t t = A; A = B; B = t; }
void bubblesort(Float_t *a, int n){
  int i, j;
  for (i = n - 1; i >= 0; i--){
    for (j = 0; j < i; j++){
      if (a[j] > a[j + 1]){
        SWAP( a[j], a[j + 1] );
		}
    }
  }
}
void bubblesort2(Float_t *a, int n){
  int i, j;
  for (i = n - 1; i >= 0; i--){
    for (j = 0; j < i; j++){
      if (a[j] < a[j + 1]){
        SWAP( a[j], a[j + 1] );
		}
    }
  }
}

int find(Float_t *a, Float_t value, int n){
	int num=-1;
	for (int i =0;i<n;i++){
		if (a[i]==value){
			 num = i;
			i=n;
		}
	}
	return num;
}

const Int_t njets = 15;
typedef struct {
   Float_t pt[njets];
   Float_t eta[njets];
	Float_t btagCSV[njets];
   Int_t pt_idx[njets];
   Int_t eta_idx[njets];
	Int_t btagCSV_idx[njets];
	Int_t btag_idx[njets];
	Int_t type[njets];
	Float_t blike_b[njets];
	Float_t blike_q[njets];
	Int_t ch_mult[njets];
	Float_t leadTrPt[njets];
	Float_t axis2[njets];
	Float_t ptd[njets];
	Int_t b_matched[njets];
	Int_t q_matched[njets];
	Int_t bb_chosen;
} TreeJets;

using namespace std;

void MakeB::Loop()
{
   if (fChain == 0) return;
   Long64_t nentries = fChain->GetEntriesFast();
   Long64_t nbytes = 0, nb = 0;

	Int_t presel=0;
	TreeJets TreeJet;
	Int_t loopJet_max;
	TFile *output = new TFile("b_likelihood_powheg_6var_bbqq_125_08.root","recreate");
	TTree *tree = fChain->CloneTree(0);
	TBranch *blike_b = tree->Branch("Jet_blikelihood_b",TreeJet.blike_b,"Jet_blikelihood_b[nJet]/F");
	TBranch *blike_q = tree->Branch("Jet_blikelihood_q",TreeJet.blike_q,"Jet_blikelihood_q[nJet]/F");
	TBranch *b_matched = tree->Branch("Jet_b_matched",TreeJet.b_matched,"Jet_b_matched[nJet]/I");
	TBranch *q_matched = tree->Branch("Jet_q_matched",TreeJet.q_matched,"Jet_q_matched[nJet]/I");
	TBranch *bb_chosen = tree->Branch("Jet_bb_chosen",&TreeJet.bb_chosen,"Jet_bb_chosen/I");
	TString weightfile_b = "weights/TMVAClassification_BDTG_6var_bb_125_08.weights.xml";
	TString weightfile_q = "weights/TMVAClassification_BDTG_6var_qq_125_08.weights.xml";
   TMVA::Reader *reader_b = new TMVA::Reader("Silent");
	float var1,var2,var3,var4,var5,var6,var7,var8,var9,var10, var11;
	reader_b->AddVariable("Jet_pt",&var1);
	reader_b->AddVariable("Jet_eta",&var2);
	reader_b->AddVariable("Jet_btagCSV",&var3);
	reader_b->AddVariable("Jet_pt_idx",&var4);
	reader_b->AddVariable("Jet_eta_idx",&var5);
	reader_b->AddVariable("Jet_btagCSV_idx",&var6);
//   reader_b->AddVariable( "Jet_chMult", &var7 );
//  reader_b->AddVariable( "Jet_ptd", &var8 );
  // reader_b->AddVariable( "Jet_axis2", &var9 );
 //  reader_b->AddVariable( "Jet_leadTrPt", &var10 );
  // 	reader_b->AddVariable("Jet_btag_idx",&var11);
	reader_b->BookMVA("BDTG", weightfile_b);
	
   TMVA::Reader *reader_q = new TMVA::Reader("Silent");
	reader_q->AddVariable("Jet_pt",&var1);
	reader_q->AddVariable("Jet_eta",&var2);
	reader_q->AddVariable("Jet_btagCSV",&var3);
	reader_q->AddVariable("Jet_pt_idx",&var4);
	reader_q->AddVariable("Jet_eta_idx",&var5);
	reader_q->AddVariable("Jet_btagCSV_idx",&var6);
	reader_q->BookMVA("BDTG", weightfile_q);	

   for (Long64_t jentry=0; jentry<nentries;jentry++) {
      Long64_t ientry = LoadTree(jentry);
      nb = fChain->GetEntry(jentry);   nbytes += nb;

	   JetList jetList_CSV, jetList_pt, jetList_eta , jetList_bl; 

		int loopJet_min = 6;
		if (nJet<6) loopJet_min=nJet;
		if (!((Jet_pt[0]>92.)&&(Jet_pt[1]>76.)&&(Jet_pt[2]>64.)&&(Jet_pt[3]>30.))) continue;

		for (int i=0;i<nJet;i++){
			TreeJet.blike_b[i] = -2;
			TreeJet.blike_q[i] = -2;
			TreeJet.b_matched[i] = 0.;	
			TreeJet.q_matched[i] = 0.;	
			TreeJet.bb_chosen = 0.;	
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

		presel+=TMath::Sign(1.,genWeight);

		int loopJet_max = 7;
		if (nJet<7) loopJet_max = nJet; 

		for(int i=0; i<loopJet_max; i++){
		   if(Jet_pt[i]<20 || Jet_id[i] <0) continue;
			if (Jet_btagCSV[i]==-10) Jet_btagCSV[i]=0; 
			if (Jet_btagCSV[i]>1.) Jet_btagCSV[i]=1.; 
		   jetList_CSV[Jet_btagCSV[i]]=i;
		   jetList_pt[Jet_pt[i]]=i;
         jetList_eta[TMath::Abs(Jet_eta[i])]=i;
	   }


		Float_t eta_sort[30];
		Float_t btag_sort[30];
		for (int i=0;i<loopJet_max;i++){
			if ((Jet_pt[i]>20)&&(Jet_id[i]>0)) {
				TreeJet.eta[i] = TMath::Abs(Jet_eta[i]);
				eta_sort[i] = TMath::Abs(Jet_eta[i]);
		   	TreeJet.btagCSV[i]=Jet_btagCSV[i];
				btag_sort[i] = Jet_btagCSV[i];
			}
		}

		bubblesort(eta_sort,loopJet_max);
		bubblesort2(btag_sort,loopJet_max);
		int btag_0_num = 0;

	   for(int i=0; i<loopJet_max; i++){
		   if(Jet_pt[i]<20 || Jet_id[i] <0) continue;
		   TreeJet.pt[i]=Jet_pt[i];
		   TreeJet.eta[i]=TMath::Abs(Jet_eta[i]); 
		   TreeJet.btagCSV[i]=Jet_btagCSV[i];
			TreeJet.ptd[i]=Jet_ptd[i];
			TreeJet.ch_mult[i]=Jet_chMult[i];
			TreeJet.axis2[i]=Jet_axis2[i];
			TreeJet.leadTrPt[i]=Jet_leadTrackPt[i];
			TreeJet.pt_idx[i] = i;
			TreeJet.eta_idx[i] = find(eta_sort,TreeJet.eta[i],loopJet_max);
			TreeJet.btagCSV_idx[i] = find(btag_sort,TreeJet.btagCSV[i],loopJet_max);
			if ((TreeJet.btagCSV[i]==0)&&(btag_0_num==0)) {
				btag_0_num =  find(btag_sort,TreeJet.btagCSV[i],loopJet_max);
				TreeJet.btagCSV_idx[i] = btag_0_num;
			} else 
			if ((TreeJet.btagCSV[i]==0)&&(btag_0_num!=0)) {
				btag_0_num++;
				TreeJet.btagCSV_idx[i] = btag_0_num;
			}
			TreeJet.btag_idx[i] = Jet_btagIdx[i];
		   TLorentzVector hJ0;
		   hJ0.SetPtEtaPhiM(Jet_pt[i],Jet_eta[i],Jet_phi[i],Jet_mass[i]);	
		   for(int j =0; j<2; j++){
			   if(TMath::Abs(GenBQuarkFromH_pdgId[j])==5){
            	TLorentzVector hQQ;
               hQQ.SetPtEtaPhiM(GenBQuarkFromH_pt[j],GenBQuarkFromH_eta[j],GenBQuarkFromH_phi[j],GenBQuarkFromH_mass[j]);
              	if(hQQ.DeltaR(hJ0)<0.8){ TreeJet.b_matched[i] =1.;  }
            } 
		   }
		   for(int j =0; j<2; j++){
				TLorentzVector hQ;
				hQ.SetPtEtaPhiM(GenHiggsSisters_pt[j],GenHiggsSisters_eta[j],GenHiggsSisters_phi[j],GenHiggsSisters_mass[j]);
				if(hQ.DeltaR(hJ0)<0.8) {TreeJet.q_matched[i] = 1.;}
		   }

		}



	for (int i=0;i<loopJet_max;i++){
		   if(Jet_pt[i]<20 || Jet_id[i] <0) continue; 
			var1 = TreeJet.pt[i];
			var2 = TreeJet.eta[i];
			var3 = TreeJet.btagCSV[i];
			var4 = TreeJet.pt_idx[i];
			var5 = TreeJet.eta_idx[i];
			var6 = TreeJet.btagCSV_idx[i];
		//	var7 = TreeJet.ch_mult[i];
		//	var8 = TreeJet.ptd[i];
	//		var11 = TreeJet.btag_idx[i];
			TreeJet.blike_b[i] = reader_b->EvaluateMVA("BDTG");
	//		cout<<var3<<"   "<<var6<< "  "<<i<< "   "<<TreeJet.blike_b[i]<<  endl;

		//	var1 = TreeJet.pt[i];
		//	var2 = TreeJet.eta[i];
		//	var3 = TreeJet.btagCSV[i];
	//		var4 = TreeJet.pt_idx[i];
	//		var5 = TreeJet.eta_idx[i];
	//		var6 = TreeJet.btagCSV_idx[i];
			TreeJet.blike_q[i] = reader_q->EvaluateMVA("BDTG");
	}

	
        for(int i=0; i<loopJet_max; i++){
                   if(Jet_pt[i]<20 || Jet_id[i] <0) continue;
                   jetList_bl[TreeJet.blike_b[i]]=i;
			}
	int cont=0;
	TLorentzVector hJ0, hJ1;
	JetList::reverse_iterator iJet=jetList_bl.rbegin();
	do{
 		int i = iJet->second;
		if(cont==0) hJ0.SetPtEtaPhiM(Jet_pt[i],Jet_eta[i],Jet_phi[i],Jet_mass[i]);
		if(cont==1) hJ1.SetPtEtaPhiM(Jet_pt[i],Jet_eta[i],Jet_phi[i],Jet_mass[i]);
		cont++;
		iJet++;
	}while (cont<2);
	int jet_isMatched=0;
	for(int j =0; j<2; j++){
		if(TMath::Abs(GenBQuarkFromH_pdgId[j])==5)
		{
			TLorentzVector hQ;
			hQ.SetPtEtaPhiM(GenBQuarkFromH_pt[j],GenBQuarkFromH_eta[j],GenBQuarkFromH_phi[j],GenBQuarkFromH_mass[j]);
			if((hQ.DeltaR(hJ1)<0.8 || hQ.DeltaR(hJ0)<0.8)) jet_isMatched++; 
		}
	}
	if (jet_isMatched==2) 	TreeJet.bb_chosen=1;

	tree->Fill();
	}
	cout<<presel<<endl;
	delete reader_b;
	delete reader_q;
	output->cd();
	tree->AutoSave();
	output->Close();
}
