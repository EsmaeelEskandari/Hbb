export MYBATCHDIR=/afs/cern.ch/work/n/nchernya/Hbb/skim_trees/
export WORKDIR=`pwd`
cd $WORKDIR

TREE=_skimmed_tree
max_samples_num=10 #number - 1
single=_single
ROOT=.root

output_dir=(/QCD_HT100to200 /QCD_HT200to300 /QCD_HT300to500 /QCD_HT500to700 /QCD_HT700to1000 /QCD_HT1000to1500 /QCD_HT1500to2000 /QCD_HT2000toInf /VBFHToBB_M-125_13TeV_powheg /VBFHToBB_M-130_13TeV_powheg /VBFHToBB_M125_13TeV_amcatnlo)
output_file=(QCD_HT100to200 QCD_HT200to300 QCD_HT300to500 QCD_HT500to700 QCD_HT700to1000 QCD_HT1000to1500 QCD_HT1500to2000 QCD_HT2000toInf VBFHToBB_M-125_13TeV_powheg VBFHToBB_M-130_13TeV_powheg VBFHToBB_M125_13TeV_amcatnlo)
current_sample=0
while [ $current_sample -le $max_samples_num ]
do
	cd $WORKDIR${output_dir[current_sample]}
	rm ${output_file[current_sample]}$TREE$ROOT 
	hadd ${output_file[current_sample]}$TREE$ROOT skimmed_tree_*.root
	current_sample=$((current_sample + 1))
done

current_sample=0
while [ $current_sample -le $max_samples_num ]
do
	cd $WORKDIR${output_dir[current_sample]}$single
	rm ${output_file[current_sample]}$TREE$single$ROOT 
	hadd ${output_file[current_sample]}$TREE$single$ROOT skimmed_tree_*.root
	current_sample=$((current_sample + 1))
done
