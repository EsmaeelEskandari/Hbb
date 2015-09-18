export MYBATCHDIR=/afs/cern.ch/work/n/nchernya/Hbb/skim_trees/
export WORKDIR=`pwd`
echo "Working on a machine:" `uname -a`
cd $WORKDIR

EOS=root://eoscms//eos/cms/store/group/phys_higgs/hbb/ntuples/V12/
TREE=tree_
ROOT=.root

output_dir=(QCD_HT100to200 QCD_HT200to300 QCD_HT300to500 QCD_HT500to700 QCD_HT700to1000 QCD_HT1000to1500 QCD_HT1500to2000 QCD_HT2000toInf VBFHToBB_M-125_13TeV_powheg VBFHToBB_M-130_13TeV_powheg VBFHToBB_M125_13TeV_amcatnlo)
max_trees_number=(885 263 196 172 140 154 38 27 20 9 28)
current_sample=0
while read sample
do
	cd $WORKDIR
#	mkdir ${output_dir[current_sample]}
	FILES=$EOS$sample$TREE
	current_tree=1
#	while [ $current_tree -le ${max_trees_number[current_sample]} ]
	while [ $current_tree -le 1 ]
	do
		f=$FILES$current_tree$ROOT
		echo $f
		bsub -q 8nh batch2.sh $f $current_tree $WORKDIR${output_dir[current_sample]}
 		current_tree=$(( $current_tree + 1 ))
	done
	current_sample=$((current_sample + 1))
	break
done < samples_paths.txt
