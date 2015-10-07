export WORKDIR=`pwd`
cd $WORKDIR

g++ run_create_main_tmva_double.C -g -o run_double `root-config --cflags --glibs` 
g++ run_create_main_tmva_single.C -g -o run_single `root-config --cflags --glibs` 

max_samples_num=4  # -1
path=/afs/cern.ch/work/n/nchernya/Hbb/skim_trees/
input_dir=(VBFHToBB_M-125_13TeV_powheg QCD_HT300to500 QCD_HT500to700 skimmed_data VBFHToBB_M-125_13TeV_powheg )
TREE=_skimmed_tree
ROOT=.root
v13=_v13
single=_single
slash=/

###please before run on QCD 300 and 500 make sure that they are in correct proportion

current_sample=3
while [ $current_sample -le $max_samples_num ]
do	
	./run_double $path${input_dir[ $current_sample ]}$v13$slash${input_dir[ $current_sample ]}$v13$TREE$v13$ROOT ${input_dir[ $current_sample ]} $current_sample
	./run_single $path${input_dir[ $current_sample ]}$v13$single$slash${input_dir[ $current_sample ]}$TREE$v13$single$ROOT ${input_dir[ $current_sample ]} $current_sample
	current_sample=$(( $current_sample + 1 ))
done
