#include <iostream>
#include <TH1F.h>
#include <TStyle.h>
#include <TCanvas.h>
#include "TLorentzVector.h"
#include <iomanip>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "TFile.h"
#include "TROOT.h"

using namespace std;

int cat(int type_int){
	gROOT->ProcessLine(".x /afs/cern.ch/work/n/nchernya/setTDRStyle.C");
	double precision=0.05;
	double NCAT=5;
	double max1=0; double max2=0; double max3=0; double max4=0; double max5=0;
	double s1=0; double s2=0;double s3=0;  double s4=0; double s5=0;
	double b1=0; double b2=0;double b3=0;  double b4=0; double b5=0;
	double max = -10;
	double border1=0;
	double border2=0;
	double border3=0;
	double border4=0;
	double start1=-1+precision;
	double start2=-1+2*precision;
	double start3=-1+3*precision;
	double start4=-1+4*precision;

	double END = .9; //right end of BDT distibution
	
//	int type_int; //double  =0; single=1
	
	TString type;
	if (type_int==0) type = "_double";
	if (type_int==1) type = "_single";
	TString text_type;
	if (type_int==0) text_type = "Spring 15, DoubleBtag";
	if (type_int==1) text_type = "Spring 15, SingleBtag";
	
	

	TFile *file_s =  new TFile("BDT_hist_VBFHToBB_M-125_13TeV_powheg"+type+".root");
	TFile *file_b1 =  new TFile("BDT_hist_QCD_HT300to500"+type+".root");
	TFile *file_b2 =  new TFile("BDT_hist_QCD_HT500to700"+type+".root");
	TH1F *hist_S = (TH1F*)file_s->Get("BDT_output");
	TH1F *hist_B = (TH1F*)file_b1->Get("BDT_output");
	TH1F *hist_B2 = (TH1F*)file_b2->Get("BDT_output");
	hist_B->Add(hist_B2);

	TCanvas *c1 = new TCanvas();
	c1->SetBottomMargin(.12);
	c1->SetLogy();
	c1->cd();
	TH1F *frame2 = new TH1F("frame2","",400,-1.,1.);
	frame2->SetMinimum(1e-1);
   frame2->SetMaximum(1e7);
   frame2->GetYaxis()->SetTitleOffset(1.4);
   frame2->GetXaxis()->SetTitleOffset(1.);
   frame2->SetStats(0);
   frame2->SetTitleFont(42,"x");
	frame2->SetTitleFont(42,"y");
   frame2->SetTitleSize(0.05, "XYZ");
	frame2->SetYTitle("Events / 10 fb^{-1}");
	frame2->SetXTitle("BDT output");	
	frame2->GetXaxis()->SetLabelSize(0.05);
	frame2->Draw();
		TLatex* tex = new TLatex(0.95,0.95,"13 TeV, PU = 20, bx = 25 ns, 10 fb^{-1}");
      tex->SetNDC();
		tex->SetTextAlign(35);
      tex->SetTextFont(42);
      tex->SetTextSize(0.04);
      tex->SetLineWidth(2);
      TLatex *tex1 = new TLatex(0.2,0.83,"CMS");
      tex1->SetNDC();
      tex1->SetTextAlign(20);
      tex1->SetTextFont(61);
      tex1->SetTextSize(0.06);
      tex1->SetLineWidth(2);
      TLatex* tex2 = new TLatex(0.27,0.77,"Work in progress");
      tex2->SetNDC();
      tex2->SetTextAlign(20);
      tex2->SetTextFont(52);
      tex2->SetTextSize(0.04);
    	tex2->SetLineWidth(2);
		TLatex* tex_file = new TLatex(0.42,0.95,text_type);
      tex_file->SetNDC();
		tex_file->SetTextAlign(35);
      tex_file->SetTextFont(42);
      tex_file->SetTextSize(0.04);
      tex_file->SetLineWidth(2);	
		tex->Draw();
		tex1->Draw();
		tex2->Draw();
		tex_file->Draw();
	hist_S->SetLineColor(2);
	hist_S->SetLineWidth(2);
	hist_B->SetLineColor(4);
	hist_B->SetLineWidth(2);
	hist_S->Draw("same");
	hist_B->Draw("same");
	TLegend *leg = new TLegend(0.45,0.7,0.92,0.93);
	leg->SetBorderSize(0);
	leg->SetTextSize(0.04);
	leg->AddEntry(hist_S,"VBF powheg, m(H) = 125 GeV","L");
	leg->AddEntry(hist_B,"QCD, H_{T} = 300-700 GeV","L");
	leg->Draw("same");
	c1->Print("plots/BDT_output_signal_bg"+type+".png");
		


	int num_of_bins = hist_S->GetNbinsX();

	int i=0; int j=0; int k=0; int l=0; int m=0;
	double bin=0.;




	do {
		i=0;
		max1=0;
		s1=0;
		b1=0;
		do	{
			s1+=hist_S->GetBinContent(i+1);
			b1+=hist_B->GetBinContent(i+1);
			bin=(double) hist_S->GetBinCenter(i+1+1);
			i++;
			//cout<<" i   "<< i<<endl;
		} while (bin < start1);
		max1=pow(s1,2)/b1;
		start2=start1+precision;
		do {
			j=i;
			max2=0;
			s2=0;
			b2=0;
			do	{
				s2+=hist_S->GetBinContent(j+1);
				b2+=hist_B->GetBinContent(j+1);
				bin=(double) hist_S->GetBinCenter(j+1+1);
				j++;
		//		cout<<" j   "<<j<<endl;
			} while (bin < start2);
			max2=pow(s2,2)/b2;
			start3=start2+precision;
			do {
				m=j;
				max3=0;
				s3=0;
				b3=0;
				do	{
					s3+=hist_S->GetBinContent(m+1);
					b3+=hist_B->GetBinContent(m+1);
					//cout<<"max3 ="<<(max3)<<endl;
					bin=(double) hist_S->GetBinCenter(m+1+1);
					m++;
		//			cout<<" m   "<< m<<endl;
				} while (bin < start3);
				max3=pow(s3,2)/b3;
				start4=start3+precision;
				do {
					k=m;
					max4=0;
					s4=0;
					b4=0;
					do  {
						s4+=hist_S->GetBinContent(k+1);
						b4+=hist_B->GetBinContent(k+1);
						bin=(double) hist_S->GetBinCenter(k+1+1);
						k++;
						//cout<<" k   "<< k<<endl;
					} while (bin < start4);
					max4=pow(s4,2)/b4;
					l=k;
					max5=0;
					s5=0;
					b5=1.;
					do  {
						s5+=hist_S->GetBinContent(l+1);
						b5+=hist_B->GetBinContent(l+1);
						bin=(double) hist_S->GetBinCenter(l+1+1);
						l++;
				//	cout<<s5<< "    "<<b5<< "   "<<max5<<"  "<<l<<"  "<<k<<"   "<<bin <<endl;
					//	cout<<" l   "<< l<< "   "<< s5<<"  "<< b5<<endl;
					} while (bin < END);
					max5=pow(s5,2)/b5;
			//		cout<<max5<<"  "<<max<<"   "<< s5<< "   "<<b5  <<endl;
					if ((max1+max2+max3+max4+max5)>=max) {
						max = max1+max2+max3+max4+max5;
						border1=start1;
						border2=start2;
						border3=start3;
						border4=start4;
					}
				//	cout<<"after  "<<max5<<"  "<<max<<endl;
					start4+=precision;
				} while (start4<=(END-(NCAT-4)*precision));
				start3+=precision;
			} while (start3<=(END-(NCAT-3)*precision));
			start2+=precision;
		} while (start2<=(END-(NCAT-2)*precision));
		start1+=precision;
	} while (start1<=(END-(NCAT-1)*precision));

	ofstream out;
	out.open("output_txt/categories"+type+".txt");
	out<<"borders of categories : "<<border1<<"   "<<border2<<"   "<<border3<< "  "<<border4 <<endl;
	out<<"S**2/B in each category : "<<max1<<"   "<<max2<<"   " << max3<<"   "<<max4<<"   "<<max5<<"  , max = "<<max<<endl;
	out.close();
	cout<<border1<<"   "<<border2<<"   "<<border3<< "  "<<border4 <<endl;
	cout<<max1<<"   "<<max2<<"   " << max3<<"   "<<max4<<"   "<<max5<<"  , max = "<<max<<endl;

return 0;

}

