#define plotMpresel_cxx
#include "plotMpresel.h"
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

void plotMpresel::Loop()
{

TH1F * h_mass_lik = new TH1F("h_mass_lik","h_mass_lik", 100, 0, 200);
TH1F * h_mass_ref = new TH1F("h_mass_ref","h_mass_ref", 100, 0, 200);
TH1F * h_mass_csv = new TH1F("h_mass_csv","h_mass_csv", 100, 0, 200);
TH1F * h_mass_lik_pt100 = new TH1F("h_mass_lik_pt100","h_mass_lik_pt100", 100, 0, 200);
TH1F * h_mass_ref_pt100 = new TH1F("h_mass_ref_pt100","h_mass_ref_pt100", 100, 0, 200);
TH1F * h_mass_csv_pt100 = new TH1F("h_mass_csv_pt100","h_mass_csv_pt100", 100, 0, 200);

TH1F * h_blik  = new TH1F("h_blik","h_blik",100, -1, 1);
TH1F * h_blik_match  = new TH1F("h_blik_match","h_blik_match",100, -1, 1);
Float_t presel = 0.;
	Int_t loopJet_max;
   if (fChain == 0) return;

   Long64_t nentries = fChain->GetEntriesFast();
  float cont_1m=0,cont_2m=0, cont_1m_=0, cont_2m_=0, nevent=0;
   Long64_t nbytes = 0, nb = 0;
   for (Long64_t jentry=0; jentry<nentries;jentry++) {
      Long64_t ientry = LoadTree(jentry);
      if (ientry < 0) break;
      nb = fChain->GetEntry(jentry);   nbytes += nb;
      JetList jetList_CSV, jetList_bl;
         float jet_isMatched2=0, jet_isMatched=0;
	  
		int loopJet_max;
		if (nJet<7) loopJet_max = nJet; 
		else loopJet_max = 7; 
 
        for(int i=0; i<loopJet_max; i++){
                   if(Jet_pt[i]<20 || Jet_id[i] <0 || Jet_btagCSV[i]>10) continue;
                   jetList_CSV[Jet_btagCSV[i]]=i;
                   jetList_bl[Jet_blikelihood_b[i]]=i;
		 				 h_blik->Fill(Jet_blikelihood_b[i]);	
           }


		if (!((Jet_pt[0]>92.)&&(Jet_pt[1]>76.)&&(Jet_pt[2]>64.)&&(Jet_pt[3]>30.))) continue;

		Double_t btag_max = 0.4;
		int btag_max1_number = -1;
		int btag_max2_number = -1;
		for (int i=0;i<6;i++){
			if ((Jet_btagCSV[i]>btag_max)&&(Jet_id[i]>0)){
				btag_max=Jet_btagCSV[i];
				btag_max1_number=i;
			}
		}
		btag_max = 0.4;
		for (int i=0;i<6;i++){
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
		for (int i=0;i<6;i++){
			if ((Jet_pt[i]>pt_max)&&(i!=btag_max1_number)&&(i!=btag_max2_number)&&(Jet_id[i]>0)) {
				pt_max=Jet_pt[i];
				pt_max1_number=i;	
			}
		}
		pt_max = 20.;
		for (int i=0;i<6;i++){
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
		//presel+=TMath::Sign(1.,genWeight);
		presel++;

	int cont=0;
	TLorentzVector hJ0, hJ1;
	for (JetList::reverse_iterator iJet=jetList_bl.rbegin(); iJet!=jetList_bl.rend(); ++iJet)
	{

		int i = iJet->second;
		if(cont==0) hJ0.SetPtEtaPhiM(Jet_pt[i],Jet_eta[i],Jet_phi[i],Jet_mass[i]);
		if(cont==1) hJ1.SetPtEtaPhiM(Jet_pt[i],Jet_eta[i],Jet_phi[i],Jet_mass[i]);
		if (cont>1) continue;
		cont++;

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

  // std::cout<<" purity b-lik "<<cont_1m/nevent<< " 1/2 "<<cont_2m/nevent <<std::endl;
 //  std::cout<<" purity csv "<<cont_1m_/nevent<< " 1/2 "<<cont_2m_/nevent <<std::endl;
   std::cout<<" preselection purity b-lik "<<cont_1m/presel<< " 1/2 "<<cont_2m/presel <<std::endl;
   std::cout<<" preselection purity csv "<<cont_1m_/presel<< " 1/2 "<<cont_2m_/presel <<std::endl;
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
