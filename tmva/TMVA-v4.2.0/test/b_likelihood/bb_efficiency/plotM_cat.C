#define plotM_cxx
#include "plotM.h"
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
typedef std::map<double, int> JetList;
void plotM::Loop()
{

TH1F * h_mass_lik = new TH1F("h_mass_lik","h_mass_lik", 100, 0, 200);
TH1F * h_mass_ref = new TH1F("h_mass_ref","h_mass_ref", 100, 0, 200);
TH1F * h_mass_csv = new TH1F("h_mass_csv","h_mass_csv", 100, 0, 200);
TH1F * h_mass_lik_pt100 = new TH1F("h_mass_lik_pt100","h_mass_lik_pt100", 100, 0, 200);
TH1F * h_mass_ref_pt100 = new TH1F("h_mass_ref_pt100","h_mass_ref_pt100", 100, 0, 200);
TH1F * h_mass_csv_pt100 = new TH1F("h_mass_csv_pt100","h_mass_csv_pt100", 100, 0, 200);

TH1F * h_blik  = new TH1F("h_blik","h_blik",100, -1, 1);
TH1F * h_blik_match  = new TH1F("h_blik_match","h_blik_match",100, -1, 1);
//   In a ROOT session, you can do:
//      Root > .L plotM.C
//      Root > plotM t
//      Root > t.GetEntry(12); // Fill t data members with entry number 12
//      Root > t.Show();       // Show values of entry 12
//      Root > t.Show(16);     // Read and show values of entry 16
//      Root > t.Loop();       // Loop on all entries
//

//     This is the loop skeleton where:
//    jentry is the global entry number in the chain
//    ientry is the entry number in the current Tree
//  Note that the argument to GetEntry must be:
//    jentry for TChain::GetEntry
//    ientry for TTree::GetEntry and TBranch::GetEntry
//
//       To read only selected branches, Insert statements like:
// METHOD1:
//    fChain->SetBranchStatus("*",0);  // disable all branches
//    fChain->SetBranchStatus("branchname",1);  // activate branchname
// METHOD2: replace line
//    fChain->GetEntry(jentry);       //read all branches
//by  b_branchname->GetEntry(ientry); //read only this branch
   if (fChain == 0) return;

   Long64_t nentries = fChain->GetEntriesFast();
  float cont_1m=0,cont_2m=0, cont_1m_=0, cont_2m_=0, nevent=0;
   Long64_t nbytes = 0, nb = 0;
   for (Long64_t jentry=0; jentry<nentries;jentry++) {
      Long64_t ientry = LoadTree(jentry);
      if (ientry < 0) break;
      nb = fChain->GetEntry(jentry);   nbytes += nb;
      // if (Cut(ientry) < 0) continue;
      JetList jetList_CSV, jetList_bl;
         float jet_isMatched2=0, jet_isMatched=0;
	  
 
        for(int i=0; i<nJet; i++){
                   if(Jet_pt[i]<20 || Jet_id[i] <0 || Jet_btagCSV[i]>10) continue;
                   jetList_CSV[Jet_btagCSV[i]]=i;
                   jetList_bl[Jet_blikelihood_b[i]]=i;
		 				 h_blik->Fill(Jet_blikelihood_b[i]);	
           }


                   /*hJ0.SetPtEtaPhiE(Jet_pt[i],Jet_eta[i],Jet_phi[i],Jet_mass[i]);
                   jet_et=hJ0.Et();
                   jet_e = hJ0.M();
                   jet_genPt=Jet_mcPt[i];
                   jet_flavour=Jet_mcFlavour[i];
                   jet_csv=Jet_btagCSV[i];
                   jet_isMatched =0;
                  */ 


	int cont=0;
	TLorentzVector hJ0, hJ1;
	for (JetList::reverse_iterator iJet=jetList_bl.rbegin(); iJet!=jetList_bl.rend(); ++iJet)
	{

		int i = iJet->second;
		if(cont==0) hJ0.SetPtEtaPhiM(Jet_pt[i],Jet_eta[i],Jet_phi[i],Jet_mass[i]);
		if(cont==1) hJ1.SetPtEtaPhiM(Jet_pt[i],Jet_eta[i],Jet_phi[i],Jet_mass[i]);
		if (cont>1) continue;
		cont++;
//		h_blik->Fill(iJet->first);

	}




	for(int j =0; j<2; j++){
		if(TMath::Abs(GenBQuarkFromH_pdgId[j])==5)
		{
			TLorentzVector hQ;
			hQ.SetPtEtaPhiM(GenBQuarkFromH_pt[j],GenBQuarkFromH_eta[j],GenBQuarkFromH_phi[j],GenBQuarkFromH_mass[j]);
			if((hQ.DeltaR(hJ1)<0.4 || hQ.DeltaR(hJ0)<0.4)) jet_isMatched++;
			
		}
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
			if(hQ.DeltaR(hJ00)<0.4 || hQ.DeltaR(hJ11)<0.4) jet_isMatched2++;
		}



	}



	if(jet_isMatched==1) cont_1m++;
	if(jet_isMatched==2) cont_2m++;
	if(jet_isMatched2==1) cont_1m_++;
	if(jet_isMatched2==2) cont_2m_++;



	nevent++;	
	h_mass_lik->Fill((hJ0+hJ1).M());
	h_mass_csv->Fill((hJ00+hJ11).M());
	h_mass_ref->Fill(H_mass);
	if(V_pt>100) {h_mass_lik_pt100->Fill((hJ0+hJ1).M());
         h_mass_csv_pt100->Fill((hJ00+hJ11).M());
        h_mass_ref_pt100->Fill(H_mass);
		}

   }

   std::cout<<" purity b-lik "<<cont_1m/nevent<< " 1/2 "<<cont_2m/nevent <<std::endl;
   std::cout<<" purity csv "<<cont_1m_/nevent<< " 1/2 "<<cont_2m_/nevent <<std::endl;
   gStyle->SetOptStat(0);
   TCanvas *c = new TCanvas("c","c",700,700);
   c->cd();
   h_mass_lik->SetLineColor(4);
   h_mass_lik->GetXaxis()->SetTitle("m_{H} (GeV)");
   h_mass_lik->SetTitle("b-likelihood order");
   h_mass_lik->Draw("");
   h_mass_csv->SetLineColor(2);
   h_mass_csv->SetTitle("csv order");
   h_mass_csv->Draw("same");
   h_mass_ref->SetTitle("highest dijet");
   h_mass_ref->Draw("same");
	c->Print("c.png");

   TCanvas *c1 = new TCanvas("c1","c1",700,700);
   c1->cd();
   h_mass_ref_pt100->GetXaxis()->SetTitle("m_{H} (GeV)");
   h_mass_lik_pt100->SetTitle("b-likelihood order");
   h_mass_csv_pt100->SetTitle("csv order");
   h_mass_ref_pt100->SetTitle("csv order");
	h_mass_lik_pt100->SetLineColor(4);
   h_mass_lik_pt100->Draw("");   
   h_mass_ref_pt100->Draw("same");
   h_mass_csv_pt100->SetLineColor(2);
   h_mass_csv_pt100->Draw("same");

	c1->Print("c1.png");

   TCanvas *d =  new TCanvas("d","d");	
	d->cd();
	d->SetLogy();
	h_blik->Draw();
	d->Print("d.png");


}
