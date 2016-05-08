#include <iostream>
#include <fstream>
#include <algorithm>
#include <TH1F.h>
#include <TTree.h>
#include <TStyle.h>
#include <TCanvas.h>
#include "TLorentzVector.h"
#include <iomanip>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "TFile.h"
#include "TROOT.h"
#include "TLatex.h"
#include "TLegend.h"
#include "TPaveText.h"

using namespace std;

//int JECR_unc(int type_int, int type_jec){
int main(int argc, char* argv[]){

	int type_int = atoi(argv[1]);
	int type_jec = atoi(argv[2]);
	int sig_num = atoi(argv[3]);   //0 -vbf , 1 - gf
	gROOT->ProcessLine(".x /afs/cern.ch/work/n/nchernya/setTDRStyle.C");


//	int type_int; //double  =0; single=1
	
	TString type;
	if (type_int==0) type = "_double";
	if (type_int==1) type = "_single";
	TString text_type;
	if (type_int==0) text_type = "DoubleB";
	if (type_int==1) text_type = "SingleB";
	TString jec;
	if (type_jec==0) jec="JER";
	if (type_jec==1) jec = "JEC";

	int NCATS;
	Float_t cats[10]; 
	Float_t Nevt_up[10];
	Float_t Nevt_nom[10];
	Float_t Nevt_down[10];
	Float_t SysUnc[10];
	Float_t SysUncUp[10];
	Float_t SysUncDown[10];
	TString cats_names[10];
	if (type_int==0) NCATS=3+1;
	else if (type_int==1) NCATS=4+1;
	if (type_int==1) {
		Float_t cats_new[10]={-1.,0.28  , 0.72  , 0.87 , 0.93    , 0.9825};
		memcpy(cats, cats_new, sizeof cats_new);
	}
	if (type_int==0) {
		Float_t cats_new[10]={-1.,0.36 ,  0.76  , 0.89  ,0.9725};
		memcpy(cats, cats_new, sizeof cats_new);
	}
	for (int i=0;i<NCATS;i++){
		TString tmp;
		tmp.Form("%1d",i);
		cats_names[i] = "CAT";
		cats_names[i].Append(tmp);
	} 
//		Float_t cats[10] = {-1.,0.12,0.44,0.57,0.6725};

	Float_t lumi = 2320.;
	Float_t xsec[2] =  {2.20, 25.69};

	const int num_ss = 2;	

//	TString s_names[num_ss] = {"VBFHToBB_M-125_13TeV_powheg","GF"};
	TString s_names[num_ss] = {"vbf_jecr_v21","gf_jecr_v21"};
//	TString s_names[num_ss] = {"vbf_76"};
	TString tex_s_names[num_ss] = {"VBF", "GF"};
		


	TCanvas *c0 = new TCanvas();
	TFile *file_nom =  new TFile("/mnt/t3nfs01/data01/shome/nchernya/Hbb/tmva/main_mva/SysUnc/v21/main_mva_v21_"+s_names[sig_num]+type+ToLower(jec)+"nom_v21.root");
	TH1F*	CountPosWeight_nom = (TH1F*)file_nom->Get("CountPosWeight");
	TH1F*	CountNegWeight_nom =(TH1F*)file_nom->Get("CountNegWeight");
	float events_generated_nom = CountPosWeight_nom->GetEntries()-CountNegWeight_nom->GetEntries();	
	TTree *tree_bdt_JEnom = (TTree*)file_nom->Get(ToLower(jec)+"nom");
	TH1F *hist_bdt_JEnom= new TH1F("hist_bdt_"+jec+"_nom","",NCATS,cats);
	float scale;
	scale=lumi*xsec[sig_num]/events_generated_nom;
	hist_bdt_JEnom->GetXaxis()->SetTitle("BDT output");
	TH1F *hist_mbb_JEnom= new TH1F("hist_mbb_"+jec+"_nom","",60,50,200);
	hist_mbb_JEnom->GetXaxis()->SetTitle("m_{bb} (GeV)");
	char cmd[50];
	char cmd2[50];
	char cmd3[50];
	sprintf(cmd,"\%s>>\%s",("BDT_VBF_"+jec+"_nom").Data(),("hist_bdt_"+jec+"_nom").Data());	
	sprintf(cmd3,"\%s>>\%s",("Mbb_VBF_"+jec+"_nom").Data(),("hist_mbb_"+jec+"_nom").Data());	
	sprintf(cmd2,"trigWeight*puWeight*bTagWeight");	
	tree_bdt_JEnom->Draw(cmd,cmd2);
	tree_bdt_JEnom->Draw(cmd3,cmd2);
	hist_mbb_JEnom->Scale(scale);
	hist_bdt_JEnom->Scale(scale);
	for (int i=0;i<NCATS;i++)
		Nevt_nom[i] = hist_bdt_JEnom->GetBinContent(i+1);

	TCanvas *c3 = new TCanvas();
	TFile *file_down =  new TFile("/mnt/t3nfs01/data01/shome/nchernya/Hbb/tmva/main_mva/SysUnc/v21/main_mva_v21_"+s_names[sig_num]+type+ToLower(jec)+"down_v21.root");
	TH1F*	CountPosWeight_down = (TH1F*)file_down->Get("CountPosWeight");
	TH1F*	CountNegWeight_down =(TH1F*)file_down->Get("CountNegWeight");
	float events_generated_down = CountPosWeight_down->GetEntries()-CountNegWeight_down->GetEntries();	
	scale=lumi*xsec[sig_num]/events_generated_down;
	TTree *tree_bdt_JEdown = (TTree*)file_down->Get(ToLower(jec)+"down");
	TH1F *hist_bdt_JEdown= new TH1F("hist_bdt_"+jec+"_down","",NCATS,cats);
	hist_bdt_JEdown->GetXaxis()->SetTitle("BDT output"); 
	TH1F *hist_mbb_JEdown= new TH1F("hist_mbb_"+jec+"_down","",60,50,200);
	hist_mbb_JEdown->GetXaxis()->SetTitle("m_{bb} (GeV)");
	sprintf(cmd,"\%s>>\%s",("BDT_VBF_"+jec+"_down").Data(),("hist_bdt_"+jec+"_down").Data());	
	sprintf(cmd3,"\%s>>\%s",("Mbb_VBF_"+jec+"_down").Data(),("hist_mbb_"+jec+"_down").Data());	
	tree_bdt_JEdown->Draw(cmd,cmd2);
	tree_bdt_JEdown->Draw(cmd3,cmd2);
	hist_mbb_JEdown->Scale(scale);
	hist_bdt_JEdown->Scale(scale);
	for (int i=0;i<NCATS;i++)
		Nevt_down[i] = hist_bdt_JEdown->GetBinContent(i+1);


	TCanvas *c2 = new TCanvas();
	TFile *file_up =  new TFile("/mnt/t3nfs01/data01/shome/nchernya/Hbb/tmva/main_mva/SysUnc/v21/main_mva_v21_"+s_names[sig_num]+type+ToLower(jec)+"up_v21.root");
	TH1F*	CountPosWeight_up = (TH1F*)file_up->Get("CountPosWeight");
	TH1F*	CountNegWeight_up =(TH1F*)file_up->Get("CountNegWeight");
	float events_generated_up = CountPosWeight_up->GetEntries()-CountNegWeight_up->GetEntries();	
	scale=lumi*xsec[sig_num]/events_generated_up;
	TTree *tree_bdt_JEup= (TTree*)file_up->Get(ToLower(jec)+"up");
	TH1F * hist_bdt_JEup= new TH1F("hist_bdt_"+jec+"_up","",NCATS,cats);
	hist_bdt_JEup->GetXaxis()->SetTitle("BDT output");
	TH1F *hist_mbb_JEup= new TH1F("hist_mbb_"+jec+"_up","",60,50,200);
	hist_mbb_JEup->GetXaxis()->SetTitle("m_{bb} (GeV)");
	sprintf(cmd,"\%s>>\%s",("BDT_VBF_"+jec+"_up").Data(),("hist_bdt_"+jec+"_up").Data());	
	sprintf(cmd3,"\%s>>\%s",("Mbb_VBF_"+jec+"_up").Data(),("hist_mbb_"+jec+"_up").Data());	
	tree_bdt_JEup->Draw(cmd,cmd2);
	tree_bdt_JEup->Draw(cmd3,cmd2);
	hist_mbb_JEup->Scale(scale);
	hist_bdt_JEup->Scale(scale);
	for (int i=0;i<NCATS;i++)
		Nevt_up[i] = hist_bdt_JEup->GetBinContent(i+1);

	
	for (int i=0;i<NCATS;i++){
		SysUncUp[i] = TMath::Abs(Nevt_up[i] - Nevt_nom[i])/ Nevt_nom[i];
		SysUncDown[i] = TMath::Abs(Nevt_nom[i] - Nevt_down[i]) / Nevt_nom[i];
	}


	ofstream out;
	out.open("output_txt/v21/UpDownsysUnc_"+s_names[sig_num]+type+ToLower(jec)+".txt");
	out <<"Systematic uncertainties for "<<jec<<"  "<<s_names[sig_num]<<"  " <<type<<endl;
	for (int i=0;i<NCATS;i++)
		out<<cats_names[i]<<"\t";
	out<<endl;	
	for (int i=0;i<NCATS;i++)
		out<<std::fixed<<std::setprecision(3)<<1+SysUncUp[i]<<"\t";
	out<<endl;	
	for (int i=0;i<NCATS;i++)
		out<<std::fixed<<std::setprecision(3)<<1-SysUncDown[i]<<"\t";
	out<<endl;	
	out.close();




 	TCanvas* c1 = new TCanvas("bdt","",800,800) ;
	c1->cd();
	TH1F *frame2 = new TH1F("frame2","",50,-1.,1.);
	frame2->SetMinimum(0.);
	frame2->GetXaxis()->SetNdivisions(505);
   frame2->SetMaximum(hist_bdt_JEnom->GetMaximum()*1.7);
//	if (type_int==1)  frame2->SetMaximum(0.005);
   frame2->SetStats(0);
	frame2->SetXTitle(hist_bdt_JEnom->GetXaxis()->GetTitle());
	frame2->SetYTitle("Events");
	frame2->Draw();

	TString tmp;
	float right   = gStyle->GetPadRightMargin();
	float top = gStyle->GetPadTopMargin();
	float left =  gStyle->GetPadLeftMargin();
	float bottom = gStyle->GetPadBottomMargin();
	TPaveText pave(0.65,0.65,1.-right-top*0.5333,1.-top*1.666,"NDC");
	pave.SetTextAlign(11);
	pave.SetFillStyle(-1);
	pave.SetBorderSize(0);
	pave.SetTextFont(42);
	pave.SetTextSize(top*0.5);
	pave.SetTextColor(kBlue+1);

	pave.AddText(tmp.Format("\%s",tex_s_names[sig_num].Data()));
	pave.AddText("M_{H} = 125 GeV");
	pave.AddText(tmp.Format("\%s selection",text_type.Data()));
	pave.AddText(tmp.Format("%s",jec.Data()));
	pave.AddText(tmp);
	pave.Draw("same");

	TLegend *leg = new TLegend(0.2,0.76,0.5,1.-top*1.666);
	leg->SetFillColor(0);
	leg->SetFillStyle(-1);
	leg->SetBorderSize(0);
	leg->SetTextFont(42);
	leg->SetTextColor(kBlue+1);
	leg->SetTextSize(top*0.5);
	leg->AddEntry(hist_bdt_JEup,"Up","L");
	leg->AddEntry(hist_bdt_JEnom,"Central","L");
	leg->AddEntry(hist_bdt_JEdown,"Down","L");
	leg->Draw("same");

// CMS info

	float left2 = gStyle->GetPadLeftMargin();
	float right2 = gStyle->GetPadRightMargin();
	float top2 = gStyle->GetPadTopMargin();
	float bottom2 = gStyle->GetPadBottomMargin();

	TPaveText pCMS1(left2,1.-top2,0.4,1.,"NDC");
	pCMS1.SetTextFont(62);
	pCMS1.SetTextSize(top2*0.75);
	pCMS1.SetTextAlign(12);
	pCMS1.SetFillStyle(-1);
	pCMS1.SetBorderSize(0);
	pCMS1.AddText("CMS");
	TPaveText pCMS2(0.5,1.-top2,1.-right2*0.5,1.,"NDC");
	pCMS2.SetTextFont(42);
	pCMS2.SetTextSize(top2*0.75);
	pCMS2.SetTextAlign(32);
	pCMS2.SetFillStyle(-1);
	pCMS2.SetBorderSize(0);
	pCMS2.AddText("L = 2.32 fb^{-1} (13 TeV)");
	gPad->Update();
	pCMS1.Draw("same");
	pCMS2.Draw("same");
	gPad->Update();
	pave.Draw("same");
	pCMS1.Draw("same");
	pCMS2.Draw("same");

	hist_bdt_JEnom->SetLineColor(4);
	hist_bdt_JEnom->SetLineWidth(2);
	hist_bdt_JEup->SetLineColor(2);
	hist_bdt_JEup->SetLineWidth(2);
	hist_bdt_JEdown->SetLineColor(8);
	hist_bdt_JEdown->SetLineWidth(2);
	hist_bdt_JEnom->Draw("same");
	hist_bdt_JEup->Draw("same");
	hist_bdt_JEdown->Draw("same");

////////

	c1->Print("plots/v21/v21_vbf_"+s_names[sig_num]+type+"_"+jec+".png");
	

	
/*
	TCanvas *c11 = new TCanvas();
	c11->SetBottomMargin(.15);
	c11->SetRightMargin(.25);
	c11->cd();
	TH1F *frame3 = new TH1F("frame3","",15,50.,200.);
	frame3->SetMinimum(0.);
   frame3->SetMaximum(hist_mbb_JEnom->GetMaximum()*1.6);
   frame3->SetStats(0);
	frame3->SetYTitle("Events");
	frame3->SetXTitle(hist_mbb_JEnom->GetXaxis()->GetTitle());
	frame3->GetYaxis()->SetNdivisions(505);
	frame3->GetXaxis()->SetLabelSize(0.0);
  	frame3->GetXaxis()->SetTitleSize(0.05);
  	frame3->GetXaxis()->SetLabelSize(0.04);
	frame3->Draw();
//	tex_file->Draw();
	hist_mbb_JEnom->SetLineColor(4);
	hist_mbb_JEnom->SetLineWidth(2);
	hist_mbb_JEup->SetLineColor(2);
	hist_mbb_JEup->SetLineWidth(2);
	hist_mbb_JEdown->SetLineColor(8);
	hist_mbb_JEdown->SetLineWidth(2);
	hist_mbb_JEnom->Draw("same");
	hist_mbb_JEup->Draw("same");
	hist_mbb_JEdown->Draw("same");
	leg->Draw("same");
	c11->Print("plots/v21/v21_vbf_"+s_names[sig_num]+type+"_"+jec+"_mbb.png");*/




	return 0;
}

