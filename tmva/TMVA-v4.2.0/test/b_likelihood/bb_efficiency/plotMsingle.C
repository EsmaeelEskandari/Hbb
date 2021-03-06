#define plotMsingle_cxx
#include "plotMsingle.h"
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
#include "TROOT.h"
#include "TLatex.h"
#include "TLegend.h"
#include "/afs/cern.ch/work/n/nchernya/Hbb/preselection_single.C"


typedef std::map<double, int> JetList;
void plotMsingle::Loop()
{

gROOT->ProcessLine(".x /afs/cern.ch/work/n/nchernya/setTDRStyle.C");
TH1F * h_mass_lik = new TH1F("h_mass_lik","h_mass_lik", 100, 0, 200);
TH1F * h_mass_ref = new TH1F("h_mass_ref","h_mass_ref", 100, 0, 200);
TH1F * h_mass_csv = new TH1F("h_mass_csv","h_mass_csv", 100, 0, 200);
TH1F * h_mass_lik_pt100 = new TH1F("h_mass_lik_pt100","h_mass_lik_pt100", 100, 0, 200);
TH1F * h_mass_ref_pt100 = new TH1F("h_mass_ref_pt100","h_mass_ref_pt100", 100, 0, 200);
TH1F * h_mass_csv_pt100 = new TH1F("h_mass_csv_pt100","h_mass_csv_pt100", 100, 0, 200);

TH1F * h_blik  = new TH1F("h_blik","h_blik",100, -1, 1);
TH1F * h_blik_match  = new TH1F("h_blik_match","h_blik_match",100, -1, 1);
Float_t presel = 0.;
Float_t bb_efficiency_find = 0;
	Int_t loopJet_max;
   if (fChain == 0) return;

	int events_wrong_blike[1000]={};
	int wrong_blike_counter=0;
	
   Long64_t nentries = fChain->GetEntriesFast();

   float cont_1m=0,cont_2m=0, cont_1m_=0, cont_2m_=0, nevent=0;
   float cont_1q=0,cont_2q=0, cont_1q_=0, cont_2q_=0;
   Long64_t nbytes = 0, nb = 0;
   for (Long64_t jentry=0; jentry<nentries;jentry++) {
      Long64_t ientry = LoadTree(jentry);
      if (ientry < 0) break;
      nb = fChain->GetEntry(jentry);   nbytes += nb;
      
		JetList jetList_CSV, jetList_bl, jetList_ql;
     	float jet_isMatched2=0, jet_isMatched=0;
      float jet_isMatched2Q=0, jet_isMatchedQ=0;

		int btag_max1_number = -1;
		int btag_max2_number = -1;
		int pt_max1_number = -1;
		int pt_max2_number = -1;
		TLorentzVector Bjet1;
		TLorentzVector Bjet2;
		TLorentzVector Qjet1;
		TLorentzVector Qjet2;
		TLorentzVector qq;	

		if (preselection_single(nJet, Jet_pt,Jet_eta, Jet_phi, Jet_mass, Jet_btagCSV, Jet_id, btag_max1_number, btag_max2_number, pt_max1_number, pt_max2_number, HLT_BIT_HLT_QuadPFJet_SingleBTagCSV_VBF_Mqq460_v, Bjet1, Bjet2, Qjet1, Qjet2, qq) != 0) continue;


		presel+=TMath::Sign(1.,genWeight);

		int loopJet_max;
		if (nJet<7) loopJet_max = nJet; 
		else loopJet_max = 7; 
	  
 
        for(int i=0; i<loopJet_max; i++){
                   if(Jet_pt[i]<20 /*|| Jet_id[i] <=0*/) continue;
                   jetList_CSV[Jet_btagCSV[i]]=i;
                   jetList_bl[Jet_blikelihood_b[i]]=i;
                 //  jetList_ql[Jet_blikelihood_q[i]]=i;
		 				 h_blik->Fill(Jet_blikelihood_b[i]);
			}
		int found_array[15] = {};
		int found_idx=0;
		Float_t bb_found = 0;
        for(int i=0; i<loopJet_max; i++){
                   if(Jet_pt[i]<20/* || Jet_id[i] <=0*/) continue;
							TLorentzVector hJall;
							hJall.SetPtEtaPhiM(Jet_pt[i],Jet_eta[i],Jet_phi[i],Jet_mass[i]);
						for(int j =0; j<2; j++){
							if(TMath::Abs(GenBQuarkFromH_pdgId[j])==5){
								TLorentzVector hJet;
								hJet.SetPtEtaPhiM(GenBQuarkFromH_pt[j],GenBQuarkFromH_eta[j],GenBQuarkFromH_phi[j],GenBQuarkFromH_mass[j]);
								if(hJet.DeltaR(hJall)<0.8) {found_array[found_idx] = i; found_idx++; bb_found++;};
							}
						}
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
		h_blik->Fill(iJet->first);
	}while (cont<2);
	cont=0;
	TLorentzVector hQ0, hQ1;
	JetList::reverse_iterator iJet0=jetList_ql.rbegin();
/*	do{
 		int i = iJet0->second;
		if(cont==0) hQ0.SetPtEtaPhiM(Jet_pt[i],Jet_eta[i],Jet_phi[i],Jet_mass[i]);
		if(cont==1) hQ1.SetPtEtaPhiM(Jet_pt[i],Jet_eta[i],Jet_phi[i],Jet_mass[i]);
		cont++;
		iJet0++;
	}while (cont<2);*/



	for(int j =0; j<2; j++){
		if(TMath::Abs(GenBQuarkFromH_pdgId[j])==5)
		{
			TLorentzVector hQ;
			hQ.SetPtEtaPhiM(GenBQuarkFromH_pt[j],GenBQuarkFromH_eta[j],GenBQuarkFromH_phi[j],GenBQuarkFromH_mass[j]);
			if((hQ.DeltaR(hJ1)<0.8 || hQ.DeltaR(hJ0)<0.8)) jet_isMatched++;
		}
	}
	if (bb_found>=2) bb_efficiency_find+=TMath::Sign(1.,genWeight);
/*	for(int j =0; j<2; j++){
		TLorentzVector hQ;
		hQ.SetPtEtaPhiM(GenHiggsSisters_pt[j],GenHiggsSisters_eta[j],GenHiggsSisters_phi[j],GenHiggsSisters_mass[j]);
		if((hQ.DeltaR(hQ0)<0.8)||(hQ.DeltaR(hQ1)<0.8))  jet_isMatchedQ++;
	}*/








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

/*
	TLorentzVector hQ00, hQ11;
	hQ00.SetPtEtaPhiM(Jet_pt[pt_max1_number],Jet_eta[pt_max1_number],Jet_phi[pt_max1_number],Jet_mass[pt_max1_number]);
	hQ11.SetPtEtaPhiM(Jet_pt[pt_max2_number],Jet_eta[pt_max2_number],Jet_phi[pt_max2_number],Jet_mass[pt_max2_number]);
	for(int j =0; j<2; j++){
		TLorentzVector hQ;
		hQ.SetPtEtaPhiM(GenHiggsSisters_pt[j],GenHiggsSisters_eta[j],GenHiggsSisters_phi[j],GenHiggsSisters_mass[j]);
		if((hQ.DeltaR(hQ00)<0.8)||(hQ.DeltaR(hQ11)<0.8))  jet_isMatched2Q++;
	}

*/
	if(jet_isMatched==1) cont_1m+=TMath::Sign(1.,genWeight);
	if(jet_isMatched==2) cont_2m+=TMath::Sign(1.,genWeight);
	if (jet_isMatched2!=2) {
			for (int k=0;k<found_idx;k++){
				//	 cout<<"event number = "<< jentry<<  " not matched = " <<found_array[k]<<"   ,  ";
				}
//			cout<<endl;
	}
	if(jet_isMatched2==1) cont_1m_+=TMath::Sign(1.,genWeight);
	if(jet_isMatched2==2) cont_2m_+=TMath::Sign(1.,genWeight);
	//if(jet_isMatchedQ==1) cont_1q+=TMath::Sign(1.,genWeight);
	//if(jet_isMatchedQ==2) cont_2q+=TMath::Sign(1.,genWeight);
	//if(jet_isMatched2Q==1) cont_1q_+=TMath::Sign(1.,genWeight);
	//if(jet_isMatched2Q==2) cont_2q_+=TMath::Sign(1.,genWeight);




	nevent++;	
	h_mass_lik->Fill((hJ0+hJ1).M());
	h_mass_csv->Fill((hJ00+hJ11).M());
	h_mass_ref->Fill(H_mass);
	if(V_pt>100) {h_mass_lik_pt100->Fill((hJ0+hJ1).M());
         h_mass_csv_pt100->Fill((hJ00+hJ11).M());
        h_mass_ref_pt100->Fill(H_mass);
		}

   }

	ofstream out;
	out.open("efficiency_v14_cmssw76_h21b.txt");
   out<<" preselection purity b-lik "<<cont_1m/presel<< " 1/2 "<<cont_2m/presel <<std::endl;
   out<<" preselection purity csv "<<cont_1m_/presel<< " 1/2 "<<cont_2m_/presel << " , presel = "<< presel<<std::endl;
	out<<"bb efficiency to find   = " << bb_efficiency_find/presel<<endl;
	out.close();

  // std::cout<<" purity b-lik "<<cont_1m/nevent<< " 1/2 "<<cont_2m/nevent <<std::endl;
 //  std::cout<<" purity csv "<<cont_1m_/nevent<< " 1/2 "<<cont_2m_/nevent <<std::endl;
   std::cout<<" preselection purity b-lik "<<cont_1m/presel<< " 1/2 "<<cont_2m/presel <<std::endl;
   std::cout<<" preselection purity csv "<<cont_1m_/presel<< " 1/2 "<<cont_2m_/presel << " , presel = "<< presel<<std::endl;
  // std::cout<<" preselection purity q-lik "<<cont_1q/presel<< " 1/2 "<<cont_2q/presel <<std::endl;
 //  std::cout<<" preselection purity q_pt "<<cont_1q_/presel<< " 1/2 "<<cont_2q_/presel << " , presel = "<< presel<<std::endl;
	std::cout<<"bb efficiency to find   = " << bb_efficiency_find/presel<<endl;
	std::cout<<"after preselection number of events and jets witj blike ==-2 : "<<wrong_blike_counter<<endl;
   gStyle->SetOptStat(0);
   TCanvas *c = new TCanvas();
	c->SetBottomMargin(.12);
   c->cd();
   h_mass_lik->SetLineColor(4);
   h_mass_lik->GetXaxis()->SetTitle("m_{H} (GeV)");
   h_mass_lik->GetYaxis()->SetTitle("Events / 2 GeV");
   h_mass_lik->SetTitle("");
   h_mass_lik->SetLineWidth(2);
   h_mass_lik->Draw("");
   h_mass_csv->SetLineColor(2);
   h_mass_csv->SetLineWidth(2);
   h_mass_csv->SetTitle("");
   h_mass_csv->Draw("same");
	TLegend *leg = new TLegend(0.68,0.7,0.9,0.93);
	leg->SetFillColor(0);
	leg->SetBorderSize(0);
	leg->SetTextSize(0.04);
	leg->AddEntry(h_mass_csv,"btag CSV order","L");
	leg->AddEntry(h_mass_lik,"b-likelihood order","L");
	leg->Draw("same");
	TLatex* tex = new TLatex(0.95,0.95,"13 TeV, PU = 20, bx = 25 ns, 1 pb^{-1}");
   tex->SetNDC();
	tex->SetTextAlign(35);
   tex->SetTextFont(42);
   tex->SetTextSize(0.04);
   tex->SetLineWidth(2);
   TLatex *tex1 = new TLatex(0.21,0.83,"CMS");
   tex1->SetNDC();
   tex1->SetTextAlign(20);
   tex1->SetTextFont(61);
   tex1->SetTextSize(0.06);
   tex1->SetLineWidth(2);
   TLatex* tex2 = new TLatex(0.28,0.77,"Work in progress");
   tex2->SetNDC();
   tex2->SetTextAlign(20);
   tex2->SetTextFont(52);
   tex2->SetTextSize(0.04);
  	tex2->SetLineWidth(2);	
	TLatex* tex_file = new TLatex(0.31,0.95,"SingleBtag");
   tex_file->SetNDC();
	tex_file->SetTextAlign(35);
   tex_file->SetTextFont(42);
   tex_file->SetTextSize(0.04);
   tex_file->SetLineWidth(2);	
	tex->Draw();
	tex1->Draw();
	tex2->Draw();
	tex_file->Draw();
	c->Print("plots/blike_mh_v14_cmssw76_h21b.png");


   TCanvas *d =  new TCanvas("d","d");	
	d->cd();
	d->SetLogy();
	h_blik->SetLineWidth(2);
   h_blik->GetXaxis()->SetTitle("b-likelihood");
   h_blik->GetYaxis()->SetTitle("Events / 0.02");
	h_blik->SetLineColor(4);
	h_blik->Draw();
	tex->Draw();
	tex1->Draw();
	tex2->Draw();
	tex_file->Draw();
	d->Print("plots/blike_v14_cmssw76_h21b.png");




}
