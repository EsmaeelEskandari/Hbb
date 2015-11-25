#include <stdio.h>
#include <iomanip>
#include <string>
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
#include "TROOT.h"
#include "TPad.h"
#include <boost/format.hpp>

#define SWAP(A, B) { Float_t t = A; A = B; B = t; }
#define SWAP2(A, B) { B.swap(A); }

void bubblesort(Float_t *a, std::string *str, int n)
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
  if (h1->Integral()!=0 ) h1->Scale(1./h1->Integral());
  if (h2->Integral()!=0 ) h2->Scale(1./h2->Integral());
  Float_t adiff = 0;
  for (Int_t i=0;i<h1s;i++){
	adiff +=  TMath::Abs(h1->GetBinContent(i) - h2->GetBinContent(i));
	//cout << "h1 bin " << i << " " << h1->GetBinContent(i) << endl;
	}
  return adiff/2;
}

using namespace std;

int main(int argc, char* argv[]){
//int analyzer_stackRatio(){
gROOT->ProcessLine(".x /afs/cern.ch/work/n/nchernya/setTDRStyle.C");
 
const int nfiles  = 16;
TString leg_names[nfiles] = {"Data, 13 TeV, bx = 25 ns",/*"powheg, m(H) = 130 GeV","amc@NLO, m(H) = 125 GeV",*/"VBF, m(H) = 125 GeV","t#bar{t}H, H#rightarrow b#bar{b}","t#bar{t}H, non b#bar{b}","QCD","QCD, H_{T}=200-300 GeV","QCD, H_{T}=300-500 GeV","QCD, H_{T}=500-700 GeV","QCD, H_{T}=700-1000 GeV","QCD, H_{T}=1000-1500 GeV","QCD, H_{T}=1500-2000 GeV","QCD, H_{T}=2000-Inf GeV","t#bar{t}","single top (tW)","Z(qq) + jets","Z(ll) + jets"};
//TString file_names[nfiles] = { "data_BTagCSV",/*"Spring15_powheg_M130","Spring15_amcatnlo_M125",*/"VBFHToBB_M-125_13TeV_powheg","QCD_HT100to200","QCD_HT200to300","QCD_HT300to500","QCD_HT500to700","QCD_HT700to1000","QCD_HT1000to1500","QCD_HT1500to2000","QCD_HT2000toInf"}; 		
TString file_names[nfiles] = { "BTagCSV",/*"VBFHToBB_M-130_13TeV_powheg",*/"VBFHToBB_M-125_13TeV_powheg","ttHtobb","ttHtoNbb","DYtoQQ","DYtoLL","ST_tW","TTbar","QCD_HT2000toInf","QCD_HT1500to2000","QCD_HT1000to1500","QCD_HT100to200","QCD_HT700to1000","QCD_HT200to300","QCD_HT500to700","QCD_HT300to500"}; 

//int FILLCOLOR[nfiles+1] = {1,1,kBlue+0,kCyan+2,kGreen+1,kSpring+8,kSpring+5,kOrange-2,kRed+2,kRed+1,kRed+0, kRed-7,kRed-9,kRed-6,kRed-8,kRed-10};
//int LINECOLOR[nfiles+1] = {1,1,kBlue+0,kCyan+2,kGreen+1,kSpring+8,kSpring+5,kOrange-2,kRed+2,kRed+1,kRed+0, kRed-7,kRed-9,kRed-6,kRed-8,kRed-10};
int FILLCOLOR[nfiles+1] = {1,1,kBlue+0,kCyan+2,kGreen+1,kSpring+8,kSpring+5,kOrange-2,kRed+2,kRed+2,kRed+2,kRed+2,kRed+2,kRed+2,kRed+2,kRed+2};
int LINECOLOR[nfiles+1] = {1,1,kBlue+0,kCyan+2,kGreen+1,kSpring+8,kSpring+5,kOrange-2,kRed+2,kRed+2,kRed+2,kRed+2,kRed+2,kRed+2,kRed+2,kRed+2};
int LINESTYLE[nfiles+1] = {1,1,2,8,1,1,1,1,1,1,1,1,1,1,1,1};
int LINEWIDTH[nfiles+1] = {1,2,2,2,1,1,1,1,1,1,1,1,1,1,1,1};
//int FILLSTYLE[nfiles] = {1001,3324,3001,3144,1001,1001,1001,1001,1001,1001,1001,1001,1001,1001,1001};
int FILLSTYLE[nfiles+1] = {1001,1001,1001,1001,1001,1001,1001,1001,1001,1001,1001,1001,1001,1001,1001,1001};
	
//int order_legend[nfiles] = {0,1,6,8,10,9,7,5,4,3,2};
//int order[nfiles] = {0,1,2,3,14,13,12,11,10,9,4,8,5,7,6};
//int order_legend[nfiles] = {0,1,2,3,10,12,14,13,11,9,8,7,6,5,4};
int order[nfiles+1] = {0,1,2,3,14,15,13,12,11,10,9,4,8,5,7,6};
int order_legend[nfiles+1] = {0,1,2,3,11,13,15,14,12,10,9,8,7,6,4,5};
	

int set_type = atoi(argv[1]); //0 - double, 1 - single
TString btag[2] = {"v14, DoubleBtag","v14, SingleBtagBlike"}; 
int region_type=atoi(argv[2]); // 0 - analysis, 1 - control region , top
TString region[3]={"_analysis","_controlTop","_controlDY"}; // 0 - analysis, 1 - control region , top; 2 - control region DY

for (int i=0;i<nfiles;i++){
	if (set_type==1) file_names[i].Prepend("_single");
	file_names[i].Prepend(region[region_type]);
	file_names[i].Prepend("skimmed_tree");
	file_names[i].Prepend("../../output_hist/v14/");
//	file_names[i].Append("_v14_exclusive_blike_pu");
	file_names[i].Append("_v14_exclusive_pu_triggerWeight_single_vtype23");
	file_names[i].Append(".root");
}
TString trigger[2] = {"RatioDoubleBtag_", "trigWeightRatioSingleBtag_blike_"};
//TString dir_name= "plots_powheg_130/";
TString dir_name= "plots_powheg_125_exclusive";
dir_name.Append(region[region_type]+"/");
//TString dir_name = "plots_amc/";
Float_t lumi = 1280.23;

	


TLegend *leg = new TLegend(0.68,0.6,0.85,0.9);
//TLegend *leg = new TLegend(0.4,0.6,0.62,0.9);
leg->SetFillColor(0);
leg->SetBorderSize(0);
leg->SetTextSize(0.04);
TLegend *leg2 = new TLegend(0.68,0.6,0.85,0.9);
leg2->SetFillColor(0);
leg2->SetBorderSize(0);
leg2->SetTextSize(0.04);

const int nhistos = 71; //40//52
TString hist_names[nhistos]={"hJet1_pt","hJet2_pt","hJet3_pt","hJet4_pt","hJet1_eta","hJet2_eta","hJet3_eta","hJet4_eta","hJet1_phi","hJet2_phi","hJet3_phi","hJet4_phi","hMqq", "hEtaQQ", "hPhiBB","hMbb","hbtag","hbtag2","hcosOqqbb","hEtaQB1", "hEtaQB2", "hPhiQB1", "hPhiQB2","hx1","hx2","hVB1_mass","hVB2_mass","hEtot","hPxtot","hPytot","hPztot","hJet5_pt","hPtqqbb","hEtaqqbb","hPhiqqbb","hJet1_pt_bin","hJet2_pt_bin","hJet3_pt_bin","hJet4_pt_bin", "hMqq_bin","hEtaSoftJets", "hPtSoftJets","hMassSoftJets","hHTsoft","hSoft_n2","hSoft_n5","hSoft_n10","hqgl","hqgl2", "hPtSoftJets2","hPtSoftJets3","hPVs", "hJet1q_pt", "hJet1q_eta", "hJet1q_ptd", "hJet1q_axis2", "hJet1q_mult", "hJet2q_pt", "hJet2q_eta", "hJet2q_ptd", "hJet2q_axis2", "hJet2q_mult","hMbb_regVBF","hMbb_regVBF_fsr","hblike1","hblike2", "hmet",  "hselLeptons_tightId" , "hselLeptons_relIso03" , "hselLeptons_chargedHadRelIso03", "hselLeptons_pfRelIso03"};
TString hist_names_sum[nhistos]={};
TString sum_histos_names[nhistos]={};
std::string hist_names_sort[nhistos];
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
	output_names[i].Prepend(trigger[set_type]);
	output_names[i].Append(region[region_type]);
	output_names[i].Append(".png");
}

TH1F *data_histos[nhistos];
TH1F *data_histos2[nhistos];
TH1F *signal_histos[nhistos];
TH1F *signal_histos2[nhistos];
TH1F *tthbb_histos[nhistos];
TH1F *tthnbb_histos[nhistos];
TH1F *sum_histos[nhistos];
TH1F *histos_forUnc[nhistos];
TH1F *histos_for_legened[nhistos];
TH1F *discr_histos[nhistos];//Mqq,delta eta, delta phi, qgl, btag //12,13,14,21,22
TH1F *hBkgVis[nhistos];
TH1F *hBkgUncUp[nhistos];
TH1F *hBkgUncLo[nhistos];


int files=0; 
THStack *stacks[nhistos];
for (int i=0;i<nhistos;++i){
	stacks[i] = new THStack(stacks_names[i],"");
}
Double_t totalBG=0.;
Double_t totalQCD=0.;
Double_t totalMC=0.;
Double_t totalData=0.;
Double_t totalDataQCD=0.;
ofstream out_efficiency;
ofstream out_discrimination;
out_efficiency.open(trigger[set_type]+dir_name+"efficiency.txt"); 


Float_t MC_data[2] = {1.,1.};////////////for efficiency 
//Float_t MC_data[2] = {1./0.708,1./.9};////////////TO BE CHANGED
//Float_t MC_data[2] = {1./0.704,1./.899};////////////for trigger logic
//Float_t MC_data[2] = {1./0.703,1./.964};////////////for blike single btag logic
Float_t lumi_qcd=lumi/MC_data[set_type];
do{
	
   TFile *file_initial = new TFile(file_names[files]);
//	file_initial->ls();   
	TH1F *histos[100];
	for (int hist=0;hist<nhistos;++hist){
		if (region_type==2) ((TH1F*)file_initial->Get(hist_names[hist]))->Rebin(2);
		histos[hist] = (TH1F*)file_initial->Get(hist_names[hist])->Clone("h");
		if (files==0) data_histos[hist] = (TH1F*)file_initial->Get(hist_names[hist])->Clone("data");
		if ((files>0)&&(files<7))histos[hist]->Scale(lumi);
		if (files>=7)histos[hist]->Scale(lumi_qcd);
		
		if (files==1) 	sum_histos[hist] = (TH1F*)histos[hist]->Clone(sum_histos_names[hist]);
		if (files>1)	sum_histos[hist]->Add(histos[hist]); 

		if (files>0) histos[hist]->Sumw2(kFALSE);
		if (hist==4) cout<<files<<"   "<<histos[4]->Integral() <<endl;

		if (files==1) {
			signal_histos[hist] = (TH1F*)file_initial->Get(hist_names[hist])->Clone(hist_names_sum[hist]+"newhist");
			signal_histos[hist]->Scale(lumi);
			signal_histos[hist]->Sumw2(kFALSE);
			signal_histos[hist]->SetLineColor(LINECOLOR[files]);
			signal_histos[hist]->SetLineStyle(LINESTYLE[files]);
			signal_histos2[hist]=(TH1F*)signal_histos[hist]->Clone("signalHist2");
		}
		if (files==2) {
			tthbb_histos[hist] = (TH1F*)file_initial->Get(hist_names[hist])->Clone(hist_names_sum[hist]+"tthb");
			tthbb_histos[hist]->Scale(lumi);
			tthbb_histos[hist]->Sumw2(kFALSE);
			tthbb_histos[hist]->SetLineColor(LINECOLOR[files]);
			tthbb_histos[hist]->SetLineStyle(LINESTYLE[files]);
		}
		if (files==3) {
			tthnbb_histos[hist] = (TH1F*)file_initial->Get(hist_names[hist])->Clone(hist_names_sum[hist]+"tthnb");
			tthnbb_histos[hist]->Scale(lumi);
			tthnbb_histos[hist]->Sumw2(kFALSE);
			tthnbb_histos[hist]->SetLineColor(LINECOLOR[files]);
			tthnbb_histos[hist]->SetLineStyle(LINESTYLE[files]);
		}
		histos[hist]->SetLineColor(LINECOLOR[files]);
		histos[hist]->SetLineStyle(LINESTYLE[files]);
		histos[hist]->SetLineWidth(LINEWIDTH[files]);
		histos[hist]->SetFillStyle(FILLSTYLE[files]);
		if ((files!=0)) histos[hist]->SetFillColor(FILLCOLOR[files]);
		
		if (files==0) {
			histos[hist]->SetMarkerStyle(20);
			data_histos[hist]->SetLineColor(1);
			data_histos[hist]->SetMarkerStyle(20);
			data_histos[hist]->SetMarkerSize(.8);
		}
	 	if (files>=4) stacks[hist]->Add(histos[hist]);
		if (hist==0) histos_for_legened[files] = (TH1F*)histos[0]->Clone("newd");
		if (files==4)	discr_histos[hist] = (TH1F*)file_initial->Get(hist_names[hist])->Clone("discr");
		if (files>4)	discr_histos[hist]->Add(histos[hist]); 
   	}
	if (files>3) totalBG+=histos[4]->Integral();
	if (files>6) totalQCD+=histos[4]->Integral();
	if (files>0) totalMC+=histos[4]->Integral();
	if (files==0) {totalData+=histos[4]->Integral(); totalDataQCD+=histos[4]->Integral();}
	if ((files>0)&&(files<7)) {totalDataQCD-=histos[4]->Integral();}
	if (files==0) out_efficiency<<"Sample & \t\t\t yield(per "<< lumi<<" $pb^{-1}$)"<<endl;
	if (files==0) out_efficiency<<leg_names[order[files]]<<"&\t \t \t"<< std::setprecision(5)<<histos[4]->Integral() <<endl;
	else out_efficiency<<leg_names[order[files]]<<"&\t\t\t  "<<std::setprecision(5)<<histos[4]->Integral()<<endl;
	if (files==nfiles-1) out_efficiency<<"Total BG"<<"&\t \t \t  "<<std::setprecision(5)<<totalBG<<endl;
	if (files==nfiles-1) out_efficiency<<"Total MC"<<"&\t \t \t  "<<std::setprecision(5)<<totalMC<<endl;
	if (files==nfiles-1) out_efficiency<<"Data/MC"<<"&\t \t \t  "<<std::setprecision(3)<<totalData/totalMC<<endl;
	if (files==nfiles-1) out_efficiency<<"DataQCD/QCD"<<"&\t \t \t  "<<std::setprecision(3)<<totalDataQCD/totalQCD<<endl;
files++;
}while (files<nfiles);
out_efficiency.close();

for (int hist=0;hist<nhistos;hist++){
	hBkgUncUp[hist] = (TH1F*)sum_histos[hist]->Clone("hBkgUncUp");
	hBkgUncLo[hist] = (TH1F*)sum_histos[hist]->Clone("hBkgUncLo");
  	hBkgVis[hist]   = (TH1F*)sum_histos[hist]->Clone("hbkgVis");
  	for(int i=0;i<hBkgUncUp[hist]->GetNbinsX();i++) {
  	 float e = 0.0;
    if (sum_histos[hist]->GetBinContent(i+1) != 0) {
      e = sum_histos[hist]->GetBinError(i+1)/sum_histos[hist]->GetBinContent(i+1);
    }
    hBkgUncUp[hist]->SetBinContent(i+1,e);
    hBkgUncLo[hist]->SetBinContent(i+1,-e);
	}
  	 hBkgVis[hist]->SetMarkerSize(0);
 	 hBkgVis[hist]->SetFillColor(kBlack);
 	 hBkgVis[hist]->SetFillStyle(3004);
 	 hBkgUncUp[hist]->SetLineColor(kBlack);
 	 hBkgUncUp[hist]->SetLineWidth(1);
 	 hBkgUncUp[hist]->SetFillColor(kBlack);
 	 hBkgUncUp[hist]->SetFillStyle(3004);
	 hBkgUncLo[hist]->SetLineColor(kBlack);
 	 hBkgUncLo[hist]->SetLineWidth(1);
  	 hBkgUncLo[hist]->SetFillColor(kBlack);
	 hBkgUncLo[hist]->SetFillStyle(3004);
}




//Float_t TSF[2] = {0.452433,0.525697};
Float_t TSF[2] = {1.,1.};
Float_t kfactors[2] = {0.,0.};
kfactors[set_type] = 1./(MC_data[set_type] * TSF[set_type]);
cout<<"kfactor = "<<kfactors[set_type]<<endl;
cout<<"Data/MC = "<< MC_data[set_type]<<endl;
//lumi/=kfactors[set_type];



for (int i=0;i<nfiles;i++){
	if (i==0) leg->AddEntry(histos_for_legened[order_legend[i]],leg_names[i],"P");
	if (i>11) {
//	else if ((i==1)||(i==2)||(i==3))  leg->AddEntry(histos_for_legened[order_legend[i]],leg_names[i],"L");
		leg->AddEntry(histos_for_legened[order_legend[i]],leg_names[i],"F");
	}
}
	leg->AddEntry(histos_for_legened[order_legend[4]],leg_names[4],"F");
	leg->AddEntry(hBkgUncUp[0],"MC stat. unc.","F");
for (int i=0;i<nfiles;i++){
	if ((i>=4) && (i<=11)) {
	leg2->AddEntry(histos_for_legened[order_legend[i]],leg_names[i],"F");
	}
}



for (int d=0;d<nhistos;d++){
	discriminators[d] = Discr(discr_histos[d],signal_histos2[d]);
}

bubblesort(discriminators, hist_names_sort,nhistos);

out_discrimination.open(trigger[set_type]+dir_name+"discrimination.txt");
for (int d=0;d<nhistos;d++){
	if (d==0) out_discrimination<<"Variable &\t d"<<endl;
	out_discrimination<<"$"<<hist_names_sort[d]<<"$"<<" & \t "<< std::setprecision(2)<< discriminators[d]<<endl;
}
out_discrimination.close();
		TLatex* tex = new TLatex(0.95,0.95,"13 TeV, bx = 25 ns, 1280.23 pb^{-1}");
      tex->SetNDC();
		tex->SetTextAlign(35);
      tex->SetTextFont(42);
      tex->SetTextSize(0.05);
      tex->SetLineWidth(2);
      TLatex *tex1 = new TLatex(0.2,0.84,"CMS");
      tex1->SetNDC();
      tex1->SetTextAlign(20);
      tex1->SetTextFont(61);
      tex1->SetTextSize(0.07);
      tex1->SetLineWidth(2);
      TLatex* tex2 = new TLatex(0.27,0.76,"Work in progress");
      tex2->SetNDC();
      tex2->SetTextAlign(20);
      tex2->SetTextFont(52);
      tex2->SetTextSize(0.045);
    	tex2->SetLineWidth(2);
//		TLatex* tex_file = new TLatex(0.3,0.95,"v13, DoubleBtag");
		TLatex* tex_file;
		if (set_type==0) tex_file = new TLatex(0.32,0.95, btag[set_type]);
		if (set_type==1) tex_file = new TLatex(0.38,0.95, btag[set_type]);
      tex_file->SetNDC();
		tex_file->SetTextAlign(35);
      tex_file->SetTextFont(42);
      tex_file->SetTextSize(0.05);
      tex_file->SetLineWidth(2);	
		TString temp_str;
		temp_str.Form("%2.2f",kfactors[set_type]);
//		temp_str.Form("%2.2f",(1./MC_data[set_type]));
		TString k_factor_str = temp_str.Prepend("QCD MC #times  ");
		TLatex* tex_k = new TLatex(0.35,0.68,k_factor_str);
      tex_k->SetNDC();
		tex_k->SetTextAlign(35);
      tex_k->SetTextFont(42);
      tex_k->SetTextSize(0.05);
      tex_k->SetLineWidth(2);
	for (int i=0;i<nhistos;i++){
		temp_str.Form("%2.2f",Discr(discr_histos[i],signal_histos2[i]));
		TString disc_value = temp_str.Prepend(" d = ");
		TLatex *disc_value_text = new TLatex(0.92,0.903,disc_value);
      disc_value_text->SetNDC();
     	disc_value_text->SetTextAlign(35);
      disc_value_text->SetTextFont(42);
      disc_value_text->SetTextSize(0.04);
      disc_value_text->SetLineWidth(2);
		
		TCanvas *c1 = new TCanvas();
		c1->SetBottomMargin(.1);
		c1->cd();
	//	if ((i!=51)&&(i<=65)/*||(i!=15)||(i!=63)||(i!=62)*/) c1->SetLogy();
 		TPad *pad1 = new TPad("pad1","pad1", 0, 0.3, 1, 1.0);
   	pad1->SetBottomMargin(0); // Upper and lower plot are joined
  	//	if ((i!=51)&&(i<=65)/*&&(i!=15)&&(i!=63)&&(i!=62)*/) pad1->SetLogy();
    	pad1->SetBottomMargin(.03);
		pad1->SetTopMargin(0.06);
		pad1->SetTicks(1,1);
    	pad1->Draw();             // Draw the upper pad: pad1
   	pad1->cd();

		Double_t xmin = signal_histos[i]->GetBinCenter(0);
		Double_t xmax = signal_histos[i]->GetBinCenter(signal_histos[i]->GetNbinsX())*1.02;
//		if (i==70) {xmin=-2.;xmax=0.5;}
//		if (i==68) {xmin=0.;xmax=0.6;}
		if (i==66) {xmin=0.;xmax=400;}
		TH1F *frame = new TH1F("frame","",1,xmin,xmax);
		frame->SetMinimum(1e-4);
      frame->SetMaximum(1e18);
	//	if ((i==51)||(i>65)/*||(i==15)||(i==63)||(i==62)*/) {
			frame->SetMinimum(0.);
      	frame->SetMaximum((hBkgVis[i]->GetMaximum())*1.7);
		//	if (set_type==1)  frame->SetMaximum((hBkgVis[i]->GetMaximum())*1.7);
	//	}
		if ((i>67)/*||(i==15)||(i==63)||(i==62)*/) {
      	frame->SetMaximum(data_histos[i]->GetBinContent(9)*5);
			if (set_type==1)  frame->SetMaximum(hBkgVis[i]->GetBinContent(9)*5);
		}
      frame->GetYaxis()->SetTitleOffset(0.9);
      frame->GetXaxis()->SetTitleOffset(0.91);
      frame->SetStats(0);
		frame->GetXaxis()->SetNdivisions(0);
      frame->SetTitleFont(42,"x");
		frame->SetTitleFont(42,"y");
      frame->SetTitleSize(0.06, "XYZ");
	//	frame->SetXTitle(signal_histos[i]->GetXaxis()->GetTitle());

      if ((i<4))frame->SetYTitle("Events  /  20 GeV");else
      if (i==37)frame->SetYTitle("Events  /  20 GeV");else
      if (i==41)frame->SetYTitle("Events  /  20 GeV");else
		if ((i>=4) && (i<8)) frame->SetYTitle("Events  /  0.5");else
		if (i==15) frame->SetYTitle("Events  /  5 GeV");else
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
		//tex_k->Draw();
	//	stacks[i]->Scale(TSF[set_type]);
	//	stacks[i]->Scale(kfactors[set_type]);	
	//	leg2->Draw("same");
    	stacks[i]->Draw("same");	
		tex->Draw();
		tex1->Draw();
		tex2->Draw();
		tex_file->Draw();
		leg->Draw("same");
//		signal_histos[i]->Draw("same");
//		tthbb_histos[i]->Draw("same");
	//	tthnbb_histos[i]->Draw("same");
/////////////////cross check of stupid THStack//////
//
//		sum_histos[i]->SetLineColor(kCyan);
//		sum_histos[i]->Scale(lumi);
//		sum_histos[i]->Draw("Lsame");
//
///////////////////////////////////////////////////
	//	data_histos[i]->Scale(1./TSF[set_type]);
	//	data_histos[i]->Scale(1./kfactors[set_type]);	
		data_histos[i]->Draw("Psame");
		hBkgVis[i]->Draw("same E2");
		//disc_value_text->Draw();
	
		c1->cd();          // Go back to the main canvas before defining pad2
   	TPad *pad2 = new TPad("pad2", "pad2", 0, 0.0, 1, 0.3);
    	pad2->SetTopMargin(.13);
    	pad2->SetBottomMargin(.4);
    	pad2->Draw();
    	pad2->cd();

		TH1F *frame2 = new TH1F("frame2","",1,xmin,xmax);
		frame2->SetMinimum(-.5);
      frame2->SetMaximum(.5);
      frame2->GetYaxis()->SetTitleOffset(0.4);
      frame2->GetXaxis()->SetTitleOffset(1.3);
      frame2->SetStats(0);
      frame2->SetTitleFont(42,"x");
		frame2->SetTitleFont(42,"y");
      frame2->SetTitleSize(0.13, "XYZ");
		frame2->GetYaxis()->SetNdivisions(505);
  		frame2->GetYaxis()->SetLabelSize(.09);
    	frame2->GetXaxis()->SetLabelSize(.12);	
		frame2->SetXTitle(signal_histos[i]->GetXaxis()->GetTitle());
		frame2->SetYTitle("Data / MC - 1");
		frame2->Draw();	

 		Float_t aa[2] = {xmin,xmax};
   	Float_t bb[2] = {0,0};
   	TGraph *cons = new TGraph(2,aa,bb);
    	cons->SetLineStyle(2);
		cons->Draw("Lsame");

		data_histos2[i] = (TH1F*)data_histos[i]->Clone("new");
	//	sum_histos[i]->Scale(TSF[set_type]);
	//	sum_histos[i]->Scale(kfactors[set_type]);	
		data_histos2[i]->Add(sum_histos[i],-1);
		data_histos2[i]->Divide(sum_histos[i]);
		data_histos2[i]->Draw("PEsame");
		hBkgUncUp[i]->Draw("HIST same");
		hBkgUncLo[i]->Draw("HIST same");
		pad1->RedrawAxis();
		pad2->RedrawAxis();
		c1->Print(output_names[i]);
		c1->Delete();
	}



return 0;
}
