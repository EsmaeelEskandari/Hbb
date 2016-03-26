int preselection_double(Int_t nJets, Float_t Jet_pt[300], Float_t Jet_eta[300], Float_t Jet_phi[300], Float_t Jet_mass[300], Float_t Jet_btagCSV[300], Int_t Jet_id[300],Int_t Jet_puId[300], Int_t& btag_max1_number, Int_t& btag_max2_number, Int_t& pt_max1_number, Int_t& pt_max2_number, Float_t trigger, TLorentzVector& Bjet1,TLorentzVector& Bjet2, TLorentzVector& Qjet1, TLorentzVector& Qjet2,TLorentzVector& qq,Int_t& cut_count){
	
	btag_max1_number = -1;
	btag_max2_number = -1;
	pt_max1_number = -1;
	pt_max2_number = -1;
	cut_count = 0;

	int not_pass=0;
	if (nJets<4) return -1;
	float Jet_pt_first[17];
	int tmp_count = 0;
	for (int i=0;i<nJets;i++){
		if (!((Jet_id[i]>2)&&(Jet_puId[i]>0))) continue;
		Jet_pt_first[tmp_count] = Jet_pt[i];
		tmp_count++; 
	}

	if (tmp_count<4) return -1;

	if (!(Jet_pt_first[0]>92.)) {not_pass= -2; cut_count++;}
	if (!(Jet_pt_first[1]>76.)) {not_pass= -3; cut_count++;}
	if (!(Jet_pt_first[2]>64.)) {not_pass= -4; cut_count++;}
	if (!(Jet_pt_first[3]>30.))  {not_pass= -5; cut_count++;}
	

		Float_t btag_max = -100;

		int loopJetmin = 7;
		if (nJets<7) loopJetmin=nJets;

		for (int i=0;i<loopJetmin;i++){
			if ((isnan(Jet_btagCSV[i])==1)||(isinf(Jet_btagCSV[i]==1)) ) Jet_btagCSV[i]=1.; 
			if (Jet_btagCSV[i]>1) Jet_btagCSV[i]=1.;
		}


		for (int i=0;i<loopJetmin;i++){
			if ((Jet_btagCSV[i]>btag_max)&&(Jet_id[i]>2)&&(Jet_puId[i]>0)){
				btag_max=Jet_btagCSV[i];
				btag_max1_number=i;
			}
		}
		btag_max = -100;
		for (int i=0;i<loopJetmin;i++){
			if ((Jet_btagCSV[i]>btag_max)&&(i!=btag_max1_number)&&(Jet_id[i]>2)&&(Jet_puId[i]>0)) {
				btag_max=Jet_btagCSV[i];
				btag_max2_number=i;
			} 
		}
		if (Jet_btagCSV[btag_max1_number]<0.5) {not_pass=-10; cut_count++;}
		if (Jet_btagCSV[btag_max2_number]<0.5) {not_pass=-11; cut_count++;}

		if (!((btag_max1_number>=0)&&(btag_max2_number>=0))) return -1;
		Bjet1.SetPtEtaPhiM(Jet_pt[btag_max1_number],Jet_eta[btag_max1_number],Jet_phi[btag_max1_number],Jet_mass[btag_max1_number]);
		
		Bjet2.SetPtEtaPhiM(Jet_pt[btag_max2_number],Jet_eta[btag_max2_number],Jet_phi[btag_max2_number],Jet_mass[btag_max2_number]);


		Float_t pt_max = 20.;
		for (int i=0;i<loopJetmin;i++){
			if ((Jet_pt[i]>pt_max)&&(i!=btag_max1_number)&&(i!=btag_max2_number)&&(Jet_id[i]>2)&&(Jet_puId[i]>0)) {
				pt_max=Jet_pt[i];
				pt_max1_number=i;	
			}
		}
		pt_max = 20.;
		for (int i=0;i<loopJetmin;i++){
			if ((Jet_pt[i]>pt_max)&&(i!=btag_max1_number)&&(i!=btag_max2_number)&&(i!=pt_max1_number)&&(Jet_id[i]>2)&&(Jet_puId[i]>0)) {
				pt_max=Jet_pt[i];
				pt_max2_number=i;	
			}
		}
		
		if (!((pt_max1_number>=0)&&(pt_max2_number>=0))) return -1;
			
		Qjet1.SetPtEtaPhiM(Jet_pt[pt_max1_number],Jet_eta[pt_max1_number],Jet_phi[pt_max1_number],Jet_mass[pt_max1_number]);
		
		Qjet2.SetPtEtaPhiM(Jet_pt[pt_max2_number],Jet_eta[pt_max2_number],Jet_phi[pt_max2_number],Jet_mass[pt_max2_number]);


		qq = Qjet1+Qjet2;
		Float_t Mqq = qq.M();
		Float_t bbDeltaPhi = TMath::Abs(Bjet1.DeltaPhi(Bjet2));
		Float_t qqDeltaEta = TMath::Abs(Qjet1.Eta()-Qjet2.Eta());
		if (!(Mqq>200)) {not_pass=-6; cut_count++;}
		if (!(qqDeltaEta>1.2)) {not_pass = -7; cut_count++;}
		if (!(bbDeltaPhi<2.4)) {not_pass = -8;	cut_count++;}
		
		if (trigger!=1) not_pass =  -9;
	return not_pass;
}
