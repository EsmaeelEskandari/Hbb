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

