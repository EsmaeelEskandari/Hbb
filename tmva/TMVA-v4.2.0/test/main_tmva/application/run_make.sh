export WORKDIR=`pwd`
cd $WORKDIR

#g++ run_make_double.C -g -o run_double `root-config --cflags --glibs`  -lMLP -lXMLIO -lTMVA  
#g++ run_make_single.C -g -o run_single `root-config --cflags --glibs`  -lMLP -lXMLIO -lTMVA  
g++ run_make_all.C -g -o run_all `root-config --cflags --glibs`  -lMLP -lXMLIO -lTMVA  
#############v12#########
#max_samples_num=10  # -1
#path=/afs/cern.ch/work/n/nchernya/Hbb/skim_trees/
#input_dir=(QCD_HT100to200 QCD_HT200to300 QCD_HT300to500 QCD_HT500to700 QCD_HT700to1000 QCD_HT1000to1500 QCD_HT1500to2000 QCD_HT2000toInf VBFHToBB_M-125_13TeV_powheg VBFHToBB_M-130_13TeV_powheg VBFHToBB_M125_13TeV_amcatnlo)


###########v13###########
max_samples_num=11  # -1
path=/afs/cern.ch/work/n/nchernya/Hbb/skim_trees/
input_dir=(QCD_HT100to200 QCD_HT200to300 QCD_HT300to500 QCD_HT500to700 QCD_HT700to1000 QCD_HT1000to1500 QCD_HT1500to2000 QCD_HT2000toInf VBFHToBB_M-125_13TeV_powheg VBFHToBB_M-130_13TeV_powheg VBFHToBB_M125_13TeV_amcatnlo skimmed_data)

TREE=_skimmed_tree
ROOT=.root
v13=_v13
double=_double
single=_single
slash=/
int_double=0
int_single=1

current_sample=10
while [ $current_sample -le $max_samples_num ]
do	
	#./run_double $path${input_dir[ $current_sample ]}$v13$slash${input_dir[ $current_sample ]}$TREE$v13$ROOT ${input_dir[ $current_sample ]}$v13$double $current_sample
	#./run_single $path${input_dir[ $current_sample ]}$v13$single$slash${input_dir[ $current_sample ]}$TREE$v13$single$ROOT ${input_dir[ $current_sample ]}$v13$single $current_sample
	./run_all $path${input_dir[ $current_sample ]}$v13$slash${input_dir[ $current_sample ]}$v13$TREE$v13$ROOT ${input_dir[ $current_sample ]}$v13$double $int_double
	./run_all $path${input_dir[ $current_sample ]}$v13$single$slash${input_dir[ $current_sample ]}$TREE$v13$single$ROOT ${input_dir[ $current_sample ]}$v13$single $int_single
	current_sample=$(( $current_sample + 1 ))
done
