export WORKDIR=`pwd`
cd $WORKDIR

g++ run_create_main_tmva_double.C -g -o run_double `root-config --cflags --glibs` 
g++ run_create_main_tmva_single.C -g -o run_single `root-config --cflags --glibs` 

max_samples_num=2  # -1
path=/afs/cern.ch/work/n/nchernya/Hbb/skim_trees/
input_dir=(VBFHToBB_M-125_13TeV_powheg QCD_HT300to500 QCD_HT500to700)
TREE=_skimmed_tree
ROOT=.root
single=_single
slash=/

current_sample=0
while [ $current_sample -le $max_samples_num ]
do	
	./run_double $path${input_dir[ $current_sample ]}$slash${input_dir[ $current_sample ]}$TREE$ROOT ${input_dir[ $current_sample ]} $current_sample
	./run_single $path${input_dir[ $current_sample ]}$single$slash${input_dir[ $current_sample ]}$TREE$single$ROOT ${input_dir[ $current_sample ]} $current_sample
	current_sample=$(( $current_sample + 1 ))
done
