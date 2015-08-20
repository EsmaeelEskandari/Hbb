#define CreateTree_qlike_cxx
#include "CreateTree_qlike.h"
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
typedef std::map<Float_t, Int_t> JetList;

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
		if (a[i]==value) {
			num = i;
			i=n;
		}
	}
	return num;
}
typedef struct {
   Float_t pt;
   Float_t eta;
	Float_t btagCSV;
   Int_t pt_idx;
   Int_t eta_idx;
	Int_t btagCSV_idx;
	Int_t btag_idx;
	Int_t type;
	Int_t ch_mult;
	Float_t ptd;
	Float_t leadTrPt;
	Float_t axis2;
	Float_t deltaEtaBQ;
	Int_t b_matched;
	Int_t q_matched;
	Int_t n_matched;
} TreeJets;

void CreateTree_qlike::Loop()
{
   if (fChain == 0) return;

   Long64_t nentries = fChain->GetEntriesFast();

   Long64_t nbytes = 0, nb = 0;

	TFile file("spring15_vbf_powheg_new_qlike6_125_08.root","recreate");
	TTree *tree0 = new TTree("Jet_tree_b","Jet_tree_b");
	TreeJets TreeJet;
	tree0->Branch("Jet_pt",&TreeJet.pt,"Jet_pt/F");
	tree0->Branch("Jet_eta",&TreeJet.eta,"Jet_eta/F");
	tree0->Branch("Jet_btagCSV",&TreeJet.btagCSV,"Jet_btagCSV/F");
	tree0->Branch("Jet_pt_idx",&TreeJet.pt_idx,"Jet_pt_idx/I");
	tree0->Branch("Jet_eta_idx",&TreeJet.eta_idx,"Jet_eta_idx/I");
	tree0->Branch("Jet_btagCSV_idx",&TreeJet.btagCSV_idx,"Jet_btagCSV_idx/I");
	tree0->Branch("Jet_btag_idx",&TreeJet.btag_idx,"Jet_btag_idx/I");
	tree0->Branch("Jet_chMult",&TreeJet.ch_mult,"Jet_chMult/I");
	tree0->Branch("Jet_ptd",&TreeJet.ptd,"Jet_ptd/F");
	tree0->Branch("Jet_leadTrackPt",&TreeJet.leadTrPt,"Jet_leadTrPt");
	tree0->Branch("Jet_axis2",&TreeJet.axis2, "Jet_axis2");
	tree0->Branch("Jet_deltaEtaBQ",&TreeJet.deltaEtaBQ, "Jet_deltaEtaBQ");
	tree0->Branch("Jet_b_matched",&TreeJet.b_matched,"Jet_b_matched/I");
	tree0->Branch("Jet_q_matched",&TreeJet.q_matched,"Jet_q_matched/I");
	tree0->Branch("Jet_n_matched",&TreeJet.n_matched,"Jet_n_matched/I");

	Float_t bb_efficiency_find=0.;
	Float_t bb_efficiency=0.;
	Float_t qq_efficiency_find=0.;
	Float_t qq_efficiency=0.;
	Float_t presel =0.;


   for (Long64_t jentry=0; jentry<nentries;jentry++) {
	   Long64_t ientry = LoadTree(jentry);
	   if (ientry < 0) break;
	   nb = fChain->GetEntry(jentry);   nbytes += nb;
	   JetList jetList_CSV, jetList_pt, jetList_eta; 
		Float_t jet_isMatched2 = 0;
		Float_t jet_isMatchedQ = 0;
	   




		if (!((Jet_pt[0]>92.)&&(Jet_pt[1]>76.)&&(Jet_pt[2]>64.)&&(Jet_pt[3]>30.))) continue;

		int loopJet_min = 6;
		if (nJet<6) loopJet_min=nJet;


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

		int loopJet_max = 5;
		if (nJet<5) loopJet_max = nJet; 

		for(int i=0; i<loopJet_max; i++){
		   if(Jet_pt[i]<20 || Jet_id[i] <0) continue;
			if ((i==btag_max1_number)||(i==btag_max2_number)) continue;
			if (Jet_btagCSV[i]==-10) Jet_btagCSV[i]=0; 
			if (Jet_btagCSV[i]>1) Jet_btagCSV[i]=1.;
		   jetList_CSV[Jet_btagCSV[i]]=i;
		   jetList_pt[Jet_pt[i]]=i;
         jetList_eta[TMath::Abs(Jet_eta[i])]=i;
	   }


	Float_t bb_found = 0.;
	Float_t qq_found = 0.;


		Float_t eta_sort[30];
		Float_t btag_sort[30];
		for (int i=0;i<loopJet_max;i++){
			if ((i==btag_max1_number)||(i==btag_max2_number)) continue;
			if ((Jet_pt[i]>20)&&(Jet_id[i]>0)) {
				TreeJet.eta = TMath::Abs(Jet_eta[i]);
				eta_sort[i] = TMath::Abs(Jet_eta[i]);
		   	TreeJet.btagCSV=Jet_btagCSV[i];
				btag_sort[i] = Jet_btagCSV[i];
			}
		}

		bubblesort(eta_sort,loopJet_max);
		bubblesort2(btag_sort,loopJet_max);
/*
		for (int i=0;i<loopJet_max;i++){
			cout<<btag_sort[i]<<"  "<<i<<endl;
		}*/
		int btag_0_num = 0;
	   for(int i=0; i<loopJet_max; i++){
		   if(Jet_pt[i]<20 || Jet_id[i] <0) continue;
			if ((i==btag_max1_number)||(i==btag_max2_number)) continue;
			TreeJet.b_matched=0;
			TreeJet.q_matched=0;
			TreeJet.n_matched=0;
		   TLorentzVector hJ0;
		   TreeJet.pt=Jet_pt[i];
		   TreeJet.eta=TMath::Abs(Jet_eta[i]); 
		   TreeJet.btagCSV=Jet_btagCSV[i];
			TreeJet.ptd=Jet_ptd[i];
			TreeJet.ch_mult=Jet_chMult[i];
			TreeJet.axis2=Jet_axis2[i];
			TreeJet.leadTrPt=Jet_leadTrackPt[i];
			TreeJet.pt_idx = i;
			TreeJet.eta_idx = find(eta_sort,TreeJet.eta,loopJet_max);
			TreeJet.btagCSV_idx = find(btag_sort,TreeJet.btagCSV,loopJet_max);
			if ((TreeJet.btagCSV==0)&&(btag_0_num==0)) {
				btag_0_num =  find(btag_sort,TreeJet.btagCSV,loopJet_max);
				TreeJet.btagCSV_idx = btag_0_num;
			} else 
			if ((TreeJet.btagCSV==0)&&(btag_0_num!=0)) {
				btag_0_num++;
				TreeJet.btagCSV_idx = btag_0_num;
			}
			TreeJet.btag_idx = Jet_btagIdx[i];
			TreeJet.deltaEtaBQ = TMath::Abs((Jet_eta[btag_max1_number]+Jet_eta[btag_max2_number])/2. - Jet_eta[i] );
			

		   hJ0.SetPtEtaPhiM(Jet_pt[i],Jet_eta[i],Jet_phi[i],Jet_mass[i]);	
		
		   for(int j =0; j<2; j++){
			   if(TMath::Abs(GenBQuarkFromH_pdgId[j])==5){
            	TLorentzVector hQQ;
               hQQ.SetPtEtaPhiM(GenBQuarkFromH_pt[j],GenBQuarkFromH_eta[j],GenBQuarkFromH_phi[j],GenBQuarkFromH_mass[j]);
              	if(hQQ.DeltaR(hJ0)<0.8){ TreeJet.b_matched =1; bb_found++;  }
            } 
		   }
		   for(int j =0; j<2; j++){
				TLorentzVector hQ;
				hQ.SetPtEtaPhiM(GenHiggsSisters_pt[j],GenHiggsSisters_eta[j],GenHiggsSisters_phi[j],GenHiggsSisters_mass[j]);
				if(hQ.DeltaR(hJ0)<0.8) {TreeJet.q_matched = 1; qq_found++;}
		   }



			if ((TreeJet.b_matched==0)&&(TreeJet.q_matched==0)) TreeJet.n_matched=1;
		 	tree0->Fill();
	   }
		
	int cont1=0; TLorentzVector hJ00, hJ11;	
	for (JetList::reverse_iterator iJet=jetList_CSV.rbegin(); iJet!=jetList_CSV.rend(); ++iJet)
	{

		int i = iJet->second;
		if(cont1==0) hJ00.SetPtEtaPhiM(Jet_pt[i],Jet_eta[i],Jet_phi[i],Jet_mass[i]);
		if(cont1==1) hJ11.SetPtEtaPhiM(Jet_pt[i],Jet_eta[i],Jet_phi[i],Jet_mass[i]);

		if (cont1>1) continue;
		cont1++;

	}
	for(int j =0; j<2; j++){
		if(TMath::Abs(GenBQuarkFromH_pdgId[j])==5)
		{
			TLorentzVector hQ;
			hQ.SetPtEtaPhiM(GenBQuarkFromH_pt[j],GenBQuarkFromH_eta[j],GenBQuarkFromH_phi[j],GenBQuarkFromH_mass[j]);
			if(hQ.DeltaR(hJ00)<0.8 || hQ.DeltaR(hJ11)<0.8) jet_isMatched2++;
		}
	}

	TLorentzVector hQ00, hQ11;
	hQ00.SetPtEtaPhiM(Jet_pt[pt_max1_number],Jet_eta[pt_max1_number],Jet_phi[pt_max1_number],Jet_mass[pt_max1_number]);
	hQ11.SetPtEtaPhiM(Jet_pt[pt_max2_number],Jet_eta[pt_max2_number],Jet_phi[pt_max2_number],Jet_mass[pt_max2_number]);
	for(int j =0; j<2; j++){
		TLorentzVector hQ;
		hQ.SetPtEtaPhiM(GenHiggsSisters_pt[j],GenHiggsSisters_eta[j],GenHiggsSisters_phi[j],GenHiggsSisters_mass[j]);
		if((hQ.DeltaR(hQ00)<0.8)||(hQ.DeltaR(hQ11)<0.8))  jet_isMatchedQ++;
	}




		if (jet_isMatched2==2) bb_efficiency+=TMath::Sign(1.,genWeight);
		if (jet_isMatchedQ==2) qq_efficiency+=TMath::Sign(1.,genWeight);
		if (bb_found>=2) bb_efficiency_find+=TMath::Sign(1.,genWeight);
		if (qq_found>=2) qq_efficiency_find+=TMath::Sign(1.,genWeight);
		presel+=TMath::Sign(1.,genWeight);
	}  

	ofstream out("spring15_powheg_125_08_qlike6.txt"); 
	out<<"bb efficiency to find b-jets = "<<bb_efficiency_find/presel<<endl;
	out<<"bb efficiency to match b-jets = "<<bb_efficiency/presel<<endl;
	out<<"qq efficiency to find q-jets = "<<qq_efficiency_find/presel<<endl;
	out<<"qq efficiency to match q-jets = "<<qq_efficiency/presel<<endl;
	cout<<"bb efficiency to find b-jets = "<<bb_efficiency_find/presel<<endl;
	cout<<"bb efficiency to match b-jets = "<<bb_efficiency/presel<<endl;
	cout<<"qq efficiency to find q-jets = "<<qq_efficiency_find/presel<<endl;
	cout<<"qq efficiency to match q-jets = "<<qq_efficiency/presel<<endl;
	file.Write();
	file.Close();

}

