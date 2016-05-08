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

//int bTagWeight_unc(int type_int, int type_unc_int){
int main(int argc, char* argv[]){
	gROOT->ProcessLine(".x /afs/cern.ch/work/n/nchernya/setTDRStyle.C");
	int type_int = atoi(argv[1]);
	int sig_num = atoi(argv[2]);   //0 -vbf , 1 - gf

//	int type_int; //double  =0; single=1
	
	TString type_unc = "puWeight";
	TString up_str = "Up";
	TString down_str = "Down";
	TString type;
	if (type_int==0) type = "_double";
	if (type_int==1) type = "_single";
	TString text_type;
	if (type_int==0) text_type = "DoubleB";
	if (type_int==1) text_type = "SingleB";

	int NCATS;
	Float_t cats[10]; 
	Float_t Nevt_up[10];
	Float_t Nevt_nom[10];
	Float_t Nevt_down[10];
	Float_t SysUnc[10];
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
	TFile *file_nom =  new TFile("/mnt/t3nfs01/data01/shome/nchernya/Hbb/tmva/main_mva/SysUnc/v21/main_mva_v21_"+s_names[sig_num]+type+"jecnom_v21.root");
	TH1F*	CountPosWeight_nom = (TH1F*)file_nom->Get("CountPosWeight");
	TH1F*	CountNegWeight_nom =(TH1F*)file_nom->Get("CountNegWeight");
	float events_generated_nom = CountPosWeight_nom->GetEntries()-CountNegWeight_nom->GetEntries();	
	float scale;
	scale=lumi*xsec[sig_num]/events_generated_nom;
	TTree *tree_bdt_JEnom = (TTree*)file_nom->Get("jecnom");
	TH1F *hist_bdt_JEnom= new TH1F("hist_bdt_nom","",NCATS,cats);
	hist_bdt_JEnom->GetXaxis()->SetTitle("BDT output");
	TH1F *hist_mbb_JEnom= new TH1F("hist_mbb_nom","",60,50,200);
	hist_mbb_JEnom->GetXaxis()->SetTitle("m_{bb} (GeV)");
	char cmd[50];
	char cmd2[50];
	char cmd3[50];
	sprintf(cmd,"\%s>>\%s",("BDT_VBF_JEC_nom"),("hist_bdt_nom"));	
	sprintf(cmd3,"\%s>>\%s",("Mbb_VBF_JEC_nom"),("hist_mbb_nom"));	
	sprintf(cmd2,"trigWeight*puWeight*bTagWeight");	
	tree_bdt_JEnom->Draw(cmd,cmd2);
	tree_bdt_JEnom->Draw(cmd3,cmd2);
	hist_bdt_JEnom->Scale(scale);
	hist_mbb_JEnom->Scale(scale);
	for (int i=0;i<NCATS;i++)
		Nevt_nom[i] = hist_bdt_JEnom->GetBinContent(i+1);

	TCanvas *c3 = new TCanvas();
	TH1F *hist_bdt_JEdown= new TH1F("hist_bdt_down","",NCATS,cats);
	hist_bdt_JEdown->GetXaxis()->SetTitle("BDT output"); 
	TH1F *hist_mbb_JEdown= new TH1F("hist_mbb_down","",60,50,200);
	hist_mbb_JEdown->GetXaxis()->SetTitle("m_{bb} (GeV)");
	sprintf(cmd,"\%s>>\%s",("BDT_VBF_JEC_nom"),("hist_bdt_down"));	
	sprintf(cmd3,"\%s>>\%s",("Mbb_VBF_JEC_nom"),("hist_mbb_down"));	
	sprintf(cmd2,"trigWeight*bTagWeight*\%s\%s",(type_unc.Data()),("Down"));	
	tree_bdt_JEnom->Draw(cmd,cmd2);
	tree_bdt_JEnom->Draw(cmd3,cmd2);
	hist_bdt_JEdown->Scale(scale);
	hist_mbb_JEdown->Scale(scale);
	for (int i=0;i<NCATS;i++)
		Nevt_down[i] = hist_bdt_JEdown->GetBinContent(i+1);


	TCanvas *c2 = new TCanvas();
	TH1F * hist_bdt_JEup= new TH1F("hist_bdt_up","",NCATS,cats);
	hist_bdt_JEup->GetXaxis()->SetTitle("BDT output");
	TH1F *hist_mbb_JEup= new TH1F("hist_mbb_up","",60,50,200);
	hist_mbb_JEup->GetXaxis()->SetTitle("m_{bb} (GeV)");
	sprintf(cmd,"\%s>>\%s",("BDT_VBF_JEC_nom"),("hist_bdt_up"));	
	sprintf(cmd3,"\%s>>\%s",("Mbb_VBF_JEC_nom"),("hist_mbb_up"));	
	sprintf(cmd2,"trigWeight*bTagWeight*\%s\%s",type_unc.Data(),"Up");	
	tree_bdt_JEnom->Draw(cmd,cmd2);
	tree_bdt_JEnom->Draw(cmd3,cmd2);
	hist_bdt_JEup->Scale(scale);
	hist_mbb_JEup->Scale(scale);
	for (int i=0;i<NCATS;i++)
		Nevt_up[i] = hist_bdt_JEup->GetBinContent(i+1);

	
	for (int i=0;i<NCATS;i++)
	//	SysUnc[i] = max( TMath::Abs((Nevt_up[i] - Nevt_nom[i])), TMath::Abs((Nevt_nom[i] - Nevt_down[i])) ) / Nevt_nom[i];
		SysUnc[i] = (Nevt_up[i] - Nevt_down[i])/(Nevt_up[i] + Nevt_down[i]);


	ofstream out;
	out.open("output_txt_pu/v21/sysUnc_"+s_names[sig_num]+type+type_unc+".txt");
	out <<"Systematic uncertainties for "<<type<<"  "<<type_unc<<"  "<<s_names[sig_num]<<"  " <<endl;
	for (int i=0;i<NCATS;i++)
		out<<cats_names[i]<<"\t";
	out<<endl;	
	for (int i=0;i<NCATS;i++)
		out<<std::fixed<<std::setprecision(3)<<1+SysUnc[i]<<"\t";	
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
	pave.AddText(tmp.Format("\%s","puWeight "));
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

	c1->Print("plots_pu/v21/v21_"+s_names[sig_num]+type+"_puWeight.png");
	



	return 0;
}

