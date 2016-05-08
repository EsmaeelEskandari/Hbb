#include <iostream>
#include <fstream>
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

using namespace std;

int JECR_unc(int type_int, int type_jec){
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
	if (type_int==0) NCATS=3+1;
	else if (type_int==1) NCATS=4+1;
	if (type_int==0) {cats={-1.,0.12,0.44,0.57,0.6725};}
//		Float_t cats[10] = {-1.,0.12,0.44,0.57,0.6725};
	else if (type_int==1) {cats={-1.,-0.5,0.31,0.59,0.67,0.7325};}

	Float_t lumi = 2190.;

	const int num_ss = 2;	

//	TString s_names[num_ss] = {"VBFHToBB_M-125_13TeV_powheg","GF"};
	TString s_names[num_ss] = {"vbf","GF"};
//	TString s_names[num_ss] = {"vbf_76"};
	TString tex_s_names[num_ss] = {"VBF, m(H) = 125 GeV", "GF, m(H) = 125 GeV"};
		


	TCanvas *c0 = new TCanvas();
	TFile *file_nom =  new TFile("/shome/nchernya/Hbb/slim_trees/SysUnc/JECR/main_mva_v14_final_cmssw76_v14_"+s_names[0]+type+ToLower(jec)+"nom.root");
	TTree *tree_bdt_JEnom = (TTree*)file_nom->Get(ToLower(jec)+"nom");
	TH1F *hist_bdt_JEnom= new TH1F("hist_bdt_"+jec+"_nom","",NCATS,cats);
	hist_bdt_JEnom->GetXaxis()->SetTitle("BDT output");
	char cmd[50];
	sprintf(cmd,"\%s>>\%s",("BDT_VBF_"+jec+"_nom").Data(),("hist_bdt_"+jec+"_nom").Data());	
	tree_bdt_JEnom->Draw(cmd);

	TCanvas *c3 = new TCanvas();
	TFile *file_down =  new TFile("/shome/nchernya/Hbb/slim_trees/SysUnc/JECR/main_mva_v14_final_cmssw76_v14_"+s_names[0]+type+ToLower(jec)+"down.root");
	TTree *tree_bdt_JEdown = (TTree*)file_down->Get(ToLower(jec)+"down");
	TH1F *hist_bdt_JEdown= new TH1F("hist_bdt_"+jec+"_down","",NCATS,cats);
	hist_bdt_JEdown->GetXaxis()->SetTitle("BDT output"); 
	sprintf(cmd,"\%s>>\%s",("BDT_VBF_"+jec+"_down").Data(),("hist_bdt_"+jec+"_down").Data());	
	tree_bdt_JEdown->Draw(cmd);


	TCanvas *c2 = new TCanvas();
	TFile *file_up =  new TFile("/shome/nchernya/Hbb/slim_trees/SysUnc/JECR/main_mva_v14_final_cmssw76_v14_"+s_names[0]+type+ToLower(jec)+"up.root");
	TTree *tree_bdt_JEup= (TTree*)file_up->Get(ToLower(jec)+"up");
	TH1F * hist_bdt_JEup= new TH1F("hist_bdt_"+jec+"_up","",NCATS,cats);
	hist_bdt_JEup->GetXaxis()->SetTitle("BDT output");
	sprintf(cmd,"\%s>>\%s",("BDT_VBF_"+jec+"_up").Data(),("hist_bdt_"+jec+"_up").Data());	
	tree_bdt_JEup->Draw(cmd);



/*	const int numArray = 3;
	TH1F* histArray[numArray]={hist_bdt_JEnom, hist_bdt_JEup, hist_bdt_JEdown}; 
	for (int i=0;i<numArray;++i){
        	histArray[i]->SetLineWidth(2);
        	histArray[i]->Scale(lumi);
        	histArray[i]->Sumw2(kFALSE);
       	histArray[i]->GetYaxis()->SetTitle("N_{events}");
	}
*/




//TFile *file_nom =  new TFile("/shome/nchernya/Hbb/slim_trees/SysUnc/JECR/main_mva_v14_final_cmssw76_v14_vbf_singlejecnom.root");
//	TTree *tree_bdt_JEnom = (TTree*)file_nom->Get(ToLower(jec)+"nom");

/*
//TTree *tree_bdt_JEnom = (TTree*)file_nom->Get("jecnom");
//char cmd[50];
//TString jec="JEC";
sprintf(cmd,"\%s>>\%s",("BDT_VBF_"+jec+"_nom").Data(),("hist_bdt_"+jec+"_nom").Data());
tree_bdt_JEnom->Draw(cmd);
TH1F *hist_bdt_JEnom= new TH1F("hist_bdt_"+jec+"_nom","",NCATS,cats);
 sprintf(cmd,"\%s>>\%s",("BDT_VBF_"+jec+"_nom").Data(),("hist_bdt_"+jec+"_nom").Data());
cout<<cmd<<endl;
tree_bdt_JEnom->Draw(cmd);
hist_bdt_JEnom->Draw();

*/




	TCanvas *c1 = new TCanvas();
	c1->SetBottomMargin(.15);
	c1->SetRightMargin(.25);
	c1->cd();
	TH1F *frame2 = new TH1F("frame2","",50,-1.,1.);
	frame2->SetMinimum(0.);
   frame2->SetMaximum(hist_bdt_JEnom->GetMaximum()*1.6);
//	if (type_int==1)  frame2->SetMaximum(0.005);
   frame2->SetStats(0);
	frame2->SetYTitle("Events");
	frame2->SetXTitle("BDT output");	
	frame2->GetYaxis()->SetNdivisions(505);
	frame2->GetXaxis()->SetLabelSize(0.0);
  	frame2->GetXaxis()->SetTitleSize(0.05);
  	frame2->GetXaxis()->SetLabelSize(0.04);
	frame2->Draw();
	TLatex* tex = new TLatex(0.75,0.95,"13 TeV");
   tex->SetNDC();
	tex->SetTextAlign(35);
   tex->SetTextFont(42);
   tex->SetTextSize(0.035);
   tex->SetLineWidth(2);
   TLatex *tex1 = new TLatex(0.17,0.95,"CMS");
   tex1->SetNDC();
   tex1->SetTextAlign(20);
   tex1->SetTextFont(61);
   tex1->SetTextSize(0.04);
   tex1->SetLineWidth(2);
   TLatex* tex2 = new TLatex(0.25,0.89,"Work in progress");
   tex2->SetNDC();
   tex2->SetTextAlign(20);
   tex2->SetTextFont(52);
   tex2->SetTextSize(0.035);
  	tex2->SetLineWidth(2);	
	TLatex* tex_file = new TLatex(0.36,0.95,text_type);
   tex_file->SetNDC();
	tex_file->SetTextAlign(35);
   tex_file->SetTextFont(42);
   tex_file->SetTextSize(0.04);
   tex_file->SetLineWidth(2);	
	tex->Draw();
	tex1->Draw();
	tex2->Draw();
//	tex_file->Draw();
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

	TLegend *leg = new TLegend(0.77,0.6,0.92,0.9);
	leg->SetFillColor(0);
//	leg->SetBorderSize(0);
	leg->SetTextFont(42);
	leg->SetTextSize(0.025);
	leg->SetHeader(jec+" variation");
	leg->AddEntry(hist_bdt_JEnom,"central","L");
	leg->AddEntry(hist_bdt_JEdown,jec+" down","L");
	leg->AddEntry(hist_bdt_JEup,jec+" up","L");
	leg->Draw("same");
	c1->Print("plots/v14_vbf_"+type+"_"+jec+".png");
		


	return 0;
}

