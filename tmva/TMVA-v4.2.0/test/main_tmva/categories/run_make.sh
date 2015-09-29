export WORKDIR=`pwd`
cd $WORKDIR

g++ run_make_hist.C -g -o hist `root-config --cflags --glibs`  -lMLP -lXMLIO 

max_samples_num=2  # -1
path=/afs/cern.ch/work/n/nchernya/tmva/TMVA-v4.2.0/test/main_tmva/application/
input_dir=(VBFHToBB_M-125_13TeV_powheg QCD_HT300to500 QCD_HT500to700)
mva=main_tmva_4_
ROOT=.root
double=_double
single=_single

current_sample=0
while [ $current_sample -le $max_samples_num ]
do	
	./hist $double ${input_dir[ $current_sample ]} $current_sample
	./hist $single ${input_dir[ $current_sample ]} $current_sample
	current_sample=$(( $current_sample + 1 ))
done

