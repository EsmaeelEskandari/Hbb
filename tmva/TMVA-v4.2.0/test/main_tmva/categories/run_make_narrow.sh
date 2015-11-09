export WORKDIR=`pwd`
cd $WORKDIR

#g++ make_hist_narrow_double.C -g -o hist_double `root-config --cflags --glibs`  -lMLP -lXMLIO 
#g++ make_hist_narrow_single.C -g -o hist_single `root-config --cflags --glibs`  -lMLP -lXMLIO 
g++ make_hist_narrow_all.C -g -o hist_all `root-config --cflags --glibs`  -lMLP -lXMLIO 

max_samples_num=11   # -1
input_dir=(QCD_HT100to200 QCD_HT200to300 QCD_HT300to500 QCD_HT500to700 QCD_HT700to1000 QCD_HT1000to1500 QCD_HT1500to2000 QCD_HT2000toInf VBFHToBB_M-125_13TeV_powheg VBFHToBB_M-130_13TeV_powheg VBFHToBB_M125_13TeV_amcatnlo skimmed_data)
path=/afs/cern.ch/work/n/nchernya/tmva/TMVA-v4.2.0/test/main_tmva/application/v13/
mva=main_tmva_4_
ROOT=.root
double=_double
single=_single
v13=_v13

current_sample=0
while [ $current_sample -le $max_samples_num ]
do	
#	./hist_double $double ${input_dir[ $current_sample ]} $current_sample
#	./hist_single $single ${input_dir[ $current_sample ]} $current_sample
	./hist_all $double ${input_dir[ $current_sample ]}$v13 $current_sample 0
	./hist_all $single ${input_dir[ $current_sample ]}$v13 $current_sample 1
	current_sample=$(( $current_sample + 1 ))
done

