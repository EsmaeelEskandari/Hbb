#include <stdio.h>
#include <iomanip>
#include <string.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include "TFile.h"
#include "TH1.h"
#include "TH1F.h"
#include "TH2.h"
#include "TTree.h"
#include <TROOT.h>
#include <TString.h>
#include <TStyle.h>
#include <TSystem.h>
#include <TCanvas.h>
#include <TMarker.h>
#include <THStack.h>
#include <TLegend.h>
#include <TLatex.h>
#include <TCut.h>
#include <TGraph.h>
#include <TGraphErrors.h>
#include <TLorentzVector.h>
#include <TMath.h>
#include <TF1.h>

#define SWAP(A, B) { Float_t t = A; A = B; B = t; }
#define SWAP2(A, B) { B.swap(A); }

void bubblesort(Float_t *a, string *str, int n)
{
  int i, j;
  for (i = n - 1; i >= 0; i--)
  {
    for (j = 0; j < i; j++)
    {
      if (a[j] < a[j + 1]){
        SWAP( a[j], a[j + 1] );
	//	cout<<str[j]<<"   "<<str[j+1]<<endl;
		SWAP2( str[j], str[j+1]);
	//	cout<<str[j]<<"   "<<str[j+1]<<endl;
		}
    }
  }
}


Float_t Discr(TH1F *h1, TH1F *h2){  
  Int_t h1s = h1->GetNbinsX();
  Int_t h2s = h2->GetNbinsX();
  if (h1s != h2s) {
  	printf("h1 size %d   h2 size %d \n",h1s,h2s);
  	return -1;
  }
  h1->Scale(1./h1->Integral());
  h2->Scale(1./h2->Integral());
  Float_t adiff = 0;
  for (Int_t i=0;i<h1s;i++){
	adiff +=  TMath::Abs(h1->GetBinContent(i) - h2->GetBinContent(i));
	//cout << "h1 bin " << i << " " << h1->GetBinContent(i) << endl;
	}
  return adiff/2;
}

using namespace std;

void analyzer_stackRatio(){
    
const int nfiles  = 10;
const int nhistos = 40;
TString leg_names[nfiles] = {"Data, 13 TeV, bx = 50 ns",/*"powheg, m(H) = 130 GeV","amc@NLO, m(H) = 125 GeV",*/"powheg, m(H) = 125 GeV","QCD, H_{T}=100-200 GeV","QCD, H_{T}=200-300 GeV","QCD, H_{T}=300-500 GeV","QCD, H_{T}=500-700 GeV","QCD, H_{T}=700-1000 GeV","QCD, H_{T}=1000-1500 GeV","QCD, H_{T}=1500-2000 GeV","QCD, H_{T}=2000-Inf GeV"};
//TString file_names[nfiles] = { "Data40pb_DoubleBtag",/*"Spring15_powheg_M130","Spring15_amcatnlo_M125",*/"Spring15_powheg_M125","Spring15_QCD_HT100to200","Spring15_QCD_HT200to300","Spring15_QCD_HT300to500","Spring15_QCD_HT500to700","Spring15_QCD_HT700to1000","Spring15_QCD_HT1000to1500","Spring15_QCD_HT1500to2000","Spring15_QCD_HT2000toInf"}; 		
TString file_names[nfiles] = { "Data40pb_DoubleBtag",/*"Spring15_powheg_M130","Spring15_amcatnlo_M125",*/"VBFHToBB_M-125_13TeV_powheg","QCD_HT100to200","QCD_HT200to300","QCD_HT300to500","QCD_HT500to700","QCD_HT700to1000","QCD_HT1000to1500","QCD_HT1500to2000","QCD_HT2000toInf"}; 		
for (int i=0;i<nfiles;i++){
//	if (i!=0)file_names[i].Prepend("SingleBtag_");
//	if (i==0) file_names[i] = "Data40pb_SingleBtag";
	if (i==0) file_names[i].Prepend("tree");
	if (i!=0) file_names[i].Prepend("skimmed_tree");
	file_names[i].Append(".root");
}
TString trigger = "RatioDoubleBtag_";
//TString trigger = "RatioSingleBtag_";
//TString dir_name= "plots_powheg_130/";
TString dir_name= "skimmed_plots_powheg_125/";
//TString dir_name = "plots_amc/";
Float_t lumi = 37.86;

	
Double_t xsec[nfiles] = { 1.,  /*1.96, 2.16,*/2.16, 2.75E07,  1.74E06,  3.67E05, 2.94E04, 6.52E03,1.064E03,   121.5,  2.54E01};


TLegend *leg = new TLegend(0.54,0.5,0.75,0.88);
leg->SetBorderSize(0);
leg->SetTextSize(0.04);

TString hist_names[nhistos]={"hJet1_pt","hJet2_pt","hJet3_pt","hJet4_pt","hJet1_eta","hJet2_eta","hJet3_eta","hJet4_eta","hJet1_phi","hJet2_phi","hJet3_phi","hJet4_phi","hMqq", "hEtaQQ", "hPhiBB","hMbb","hbtag","hbtag2","hcosOqqbb","hEtaQB1", "hEtaQB2", "hPhiQB1", "hPhiQB2","hx1","hx2","hVB1_mass","hVB2_mass","hEtot","hPxtot","hPytot","hPztot","hJet5_pt","hPtqqbb","hEtaqqbb","hPhiqqbb","hJet1_pt_bin","hJet2_pt_bin","hJet3_pt_bin","hJet4_pt_bin", "hMqq_bin" };
TString hist_names_sum[nhistos]={};
TString sum_histos_names[nhistos]={};
string hist_names_sort[nhistos];
for (int i=0;i<nhistos;i++){
	hist_names_sort[i] = hist_names[i];
	hist_names_sum[i] = hist_names[i];
	hist_names_sum[i].Append("_sum");
	sum_histos_names[i] = hist_names[i];
	sum_histos_names[i].Append("_sum0");
}
Float_t discriminators[nhistos];
TString stacks_names[nhistos];
for (int i=0;i<nhistos;i++){
	stacks_names[i] = hist_names[i];
	stacks_names[i].Prepend("s");
}
TString output_names[nhistos];
for (int i=0;i<nhistos;i++){
	output_names[i] = hist_names[i];
	output_names[i].Prepend(dir_name);
	output_names[i].Prepend(trigger);
	output_names[i].Append(".png");
}

TH1F *data_histos[nhistos];
TH1F *data_histos2[nhistos];
TH1F *signal_histos[nhistos];
TH1F *sum_histos[nhistos];
TH1F *histos_forsum[nhistos];
TH1F *discr_histos[nhistos];//Mqq,delta eta, delta phi, qgl, btag //12,13,14,21,22
int files=0; 
THStack *stacks[nhistos];
for (int i=0;i<nhistos;++i){
	stacks[i] = new THStack(stacks_names[i],"");
}
Double_t totalBG=0.;
ofstream out_efficiency;
ofstream out_discrimination;
out_efficiency.open(trigger+dir_name+"efficiency.txt"); 
do{
	
   TFile *file_initial = new TFile(file_names[files]);
//	file_initial->ls();   
	TH1F *histos[100];
	for (int hist=0;hist<nhistos;++hist){
		histos[hist] = (TH1F*)file_initial->Get(hist_names[hist]);
		if (files>1) {
			histos_forsum[hist] = (TH1F*)file_initial->Get(hist_names[hist])->Clone(hist_names_sum[hist]);
		}
		if (files==0) data_histos[hist] = (TH1F*)file_initial->Get(hist_names[hist]);
		if (files>0){
			histos[hist]->Sumw2(kFALSE);
			histos[hist]->Scale(lumi);
		}
		if (files==1) {
			sum_histos[hist] = (TH1F*)file_initial->Get(hist_names[hist])->Clone(sum_histos_names[hist]);
			signal_histos[hist] = (TH1F*)file_initial->Get(hist_names[hist]);
			signal_histos[hist]->Scale(lumi);
			signal_histos[hist]->Sumw2(kFALSE);
		}
	//	histos[hist]->SetLineColor(1+files);
	//	histos[hist]->SetFillColor(1+files);
		if (files==2) {histos[hist]->SetFillColor(kBlue-10); histos[hist]->SetLineColor(kBlue-10);}
		if (files==3) {histos[hist]->SetFillColor(kBlue); histos[hist]->SetLineColor(kBlue);}
		if (files==4) {histos[hist]->SetFillColor(kOrange); histos[hist]->SetLineColor(kOrange);}
		if (files==5) {histos[hist]->SetFillColor(kRed);histos[hist]->SetLineColor(kRed); }
		if (files==6) {histos[hist]->SetFillColor(kAzure+10);histos[hist]->SetLineColor(kAzure+10); }
		if (files==7) {histos[hist]->SetFillColor(kGreen+2);histos[hist]->SetLineColor(kGreen+2); }
		if (files==8) {histos[hist]->SetFillColor(kPink+9);histos[hist]->SetLineColor(kPink+9); }
		if (files==9) {histos[hist]->SetFillColor(kViolet+1);histos[hist]->SetLineColor(kViolet+1); }
		if (files==1) {
			histos[hist]->SetFillStyle(3324);
			histos[hist]->SetFillColor(1);
			histos[hist]->SetLineColor(1);
			signal_histos[hist]->SetFillStyle(3324);
			signal_histos[hist]->SetLineColor(1);
			signal_histos[hist]->SetFillColor(1);
		}
		if (files==0) {
			histos[hist]->SetLineColor(1);
			histos[hist]->SetMarkerStyle(20);
			data_histos[hist]->SetLineColor(1);
			data_histos[hist]->SetMarkerStyle(20);
		}
	 	if (files>=1)stacks[hist]->Add(histos[hist]);
		if ((hist==0)&&(files!=0)) leg->AddEntry(histos[hist],leg_names[files],"F");
		if ((hist==0)&&(files==0)) leg->AddEntry(histos[hist],leg_names[files],"P");
		if (files==2){
			discr_histos[hist] = (TH1F*)file_initial->Get(hist_names[hist]);
		}
		if ((files>2)){
			discr_histos[hist]->Add(histos[hist]); 
		}
		if (files>1){
			sum_histos[hist]->Add(histos_forsum[hist]); 
		}
   	}
	if (files>1) totalBG+=histos[4]->Integral();
	if (files==0) out_efficiency<<"Sample & \t\t\t yield(per $fb^{-1}$)& \t efficiency"<<endl;
	if (files==0) out_efficiency<<leg_names[files]<<"&\t \t \t"<<histos[5]->Integral()*1000. <<"&\t"<< histos[5]->Integral()/xsec[files] <<endl;
	else out_efficiency<<leg_names[files]<<"&\t  "<<histos[4]->Integral()*1000. <<"&\t"<< histos[4]->Integral()/xsec[files] <<endl;
	if (files==nfiles-1) out_efficiency<<"Total BG"<<"&\t \t \t  "<<totalBG*1000.<< endl;
files++;
}while (files<nfiles);
out_efficiency.close();




for (int d=0;d<nhistos;d++){
	discriminators[d] = Discr(discr_histos[d],signal_histos[d]);
}

bubblesort(discriminators, hist_names_sort,nhistos);

out_discrimination.open(trigger+dir_name+"discrimination.txt");
for (int d=0;d<nhistos;d++){
	if (d==0) out_discrimination<<"Variable &\t d"<<endl;
	out_discrimination<<"$"<<hist_names_sort[d]<<"$"<<" & \t "<< std::setprecision(2)<< discriminators[d]<<endl;
}
out_discrimination.close();
		TLatex* tex = new TLatex(0.90,0.92,"13 TeV, PU = 20, bx = 25 ns, 40 pb^{-1}");
      tex->SetNDC();
		tex->SetTextAlign(35);
      tex->SetTextFont(42);
      tex->SetTextSize(0.04);
      tex->SetLineWidth(2);
      TLatex *tex1 = new TLatex(0.13,0.83,"CMS");
      tex1->SetNDC();
      tex1->SetTextAlign(20);
      tex1->SetTextFont(61);
      tex1->SetTextSize(0.06);
      tex1->SetLineWidth(2);
      TLatex* tex2 = new TLatex(0.195,0.77,"Work in progress");
      tex2->SetNDC();
      tex2->SetTextAlign(20);
      tex2->SetTextFont(52);
      tex2->SetTextSize(0.04);
    	tex2->SetLineWidth(2);
		TLatex* tex_file = new TLatex(0.28,0.92,"Spring15, DoubleBtag");
//		TLatex* tex_file = new TLatex(0.28,0.92,"Spring15, SingleBtag");
      tex_file->SetNDC();
		tex_file->SetTextAlign(35);
      tex_file->SetTextFont(42);
      tex_file->SetTextSize(0.04);
      tex_file->SetLineWidth(2);	
	for (int i=0;i<nhistos;i++){
		TString temp_str;
		temp_str.Form("%2.2f",Discr(discr_histos[i],signal_histos[i]));
		TString disc_value = temp_str.Prepend(", d = ");
		TLatex *disc_value_text = new TLatex(0.86,0.81,disc_value);
      disc_value_text->SetNDC();
     	disc_value_text->SetTextAlign(35);
      disc_value_text->SetTextFont(42);
      disc_value_text->SetTextSize(0.04);
      disc_value_text->SetLineWidth(2);
		
		TCanvas *c1 = new TCanvas();
		c1->SetBottomMargin(.12);
		c1->cd();
		c1->SetLogy();
 		TPad *pad1 = new TPad("pad1", "pad1", 0, 0.3, 1, 1.0);
   	pad1->SetBottomMargin(0); // Upper and lower plot are joined
  		pad1->SetLogy();
    	pad1->SetBottomMargin(.015);
    	pad1->Draw();             // Draw the upper pad: pad1
   	pad1->cd();

		Double_t xmin = signal_histos[i]->GetBinCenter(0);
		Double_t xmax = signal_histos[i]->GetBinCenter(signal_histos[i]->GetNbinsX())*1.02;
		TH1F *frame = new TH1F("frame","",1,xmin,xmax);
		frame->SetMinimum(1e-4);
      frame->SetMaximum(1e13);
      frame->GetYaxis()->SetTitleOffset(0.9);
      frame->GetXaxis()->SetTitleOffset(0.91);
      frame->SetStats(0);
		frame->GetXaxis()->SetNdivisions(0);
      frame->SetTitleFont(42,"x");
		frame->SetTitleFont(42,"y");
      frame->SetTitleSize(0.05, "XYZ");
	//	frame->SetXTitle(signal_histos[i]->GetXaxis()->GetTitle());

      if ((i<4))frame->SetYTitle("Events  /  20 GeV");else
      if (i==37)frame->SetYTitle("Events  /  20 GeV");else
      if (i==41)frame->SetYTitle("Events  /  20 GeV");else
		if ((i>=4) && (i<8)) frame->SetYTitle("Events  /  0.5");else
		if (i==15) frame->SetYTitle("Events  /  0.5");else
		if ((i>=8) && (i<12)) frame->SetYTitle("Events  /  0.2");else
		if (i==40) frame->SetYTitle("Events / 100 GeV");else
		if ((i==10)||(i==12)||(i==16)||(i==17)||(i==18) || (i==20) || (i==25) || (i==26) || (i==27) || (i==35) || (i==36) ||(i==38) || (i==39)) frame->SetYTitle("Events / 10 GeV"); else
		if ((i==13)||(i==14)) frame->SetYTitle("Events / 0.1"); else
		if ((i==21)||(i==22)) frame->SetYTitle("Events / 0.05"); else
		if ((i==23)||(i==24)) frame->SetYTitle("Events / 0.01"); else
		if (i==28) frame->SetYTitle("Events / 0.02"); else
		if ((i==29) || (i==30) || (i==31) || (i==32) )frame->SetYTitle("Events / 0.1");else
		if ((i==33) || (i==33))frame->SetYTitle("Events / 0.01");
		else frame->SetYTitle("Events");
      frame->Draw();
		tex->Draw();
		tex1->Draw();
		tex2->Draw();
		tex_file->Draw();
    	stacks[i]->Draw("nostacksame");	
		leg->Draw("same");
		signal_histos[i]->Draw("same");
		data_histos[i]->Draw("Psame");
		disc_value_text->Draw();
	
		c1->cd();          // Go back to the main canvas before defining pad2
   	TPad *pad2 = new TPad("pad2", "pad2", 0, 0.0, 1, 0.3);
    	pad2->SetTopMargin(.1);
    	pad2->SetBottomMargin(.32);
    	pad2->Draw();
    	pad2->cd();

		TH1F *frame2 = new TH1F("frame2","",1,xmin,xmax);
		frame2->SetMinimum(0.);
      frame2->SetMaximum(2.);
      frame2->GetYaxis()->SetTitleOffset(0.4);
      frame2->GetXaxis()->SetTitleOffset(1.3);
      frame2->SetStats(0);
      frame2->SetTitleFont(42,"x");
		frame2->SetTitleFont(42,"y");
      frame2->SetTitleSize(0.11, "XYZ");
		frame2->GetYaxis()->SetNdivisions(505);
  		frame2->GetYaxis()->SetLabelSize(.09);
    	frame2->GetXaxis()->SetLabelSize(.12);	
		frame2->SetXTitle(signal_histos[i]->GetXaxis()->GetTitle());
		frame2->SetYTitle("Data / MC");
		frame2->Draw();	

 		Float_t aa[2] = {xmin,xmax};
   	Float_t bb[2] = {1,1};
   	TGraph *cons = new TGraph(2,aa,bb);
    	cons->SetLineStyle(2);
		cons->Draw("Lsame");

		data_histos2[i] = (TH1F*)data_histos[i]->Clone("new");
		data_histos2[i]->Scale(1./lumi);
		data_histos2[i]->Divide(sum_histos[i]);
		data_histos2[i]->Draw("PEsame");
	
		c1->Print(output_names[i]);
		c1->Delete();
	}
	
}
