export WORKDIR=`pwd`
cd $WORKDIR

g++ run_make_all.C -g -o run_all `root-config --cflags --glibs`  -lMLP -lXMLIO -lTMVA  

cp run_all /shome/nchernya/Hbb/tmva/main_mva/application/

max_samples_num=18
input_dir=(VBFHToBB_M-125_13TeV_powheg BTagCSV_golden QCD_HT100to200 QCD_HT200to300 QCD_HT300to500 QCD_HT500to700 QCD_HT700to1000 QCD_HT1000to1500 QCD_HT1500to2000 QCD_HT2000toInf VBFHToBB_M-125_13TeV_powheg TTbar DYtoQQ ST_tW ttHtobb ttHtoNbb DYtoLL GF)
path=dcap://t3se01.psi.ch:22125//pnfs/psi.ch/cms/trivcat///store/user/nchernya/Hbb/v14/qgd/
v14=_v14
v2=_2
ROOT=.root
double=_double
single=_single
slash=/
jets=_2jets
qgd=qgd_
mva=main_mva_v14_

current_sample=0
while [ $current_sample -lt $max_samples_num ]
#while [ $current_sample -lt 17 ]
do	
	qsub -q short.q batch2.sh $path${input_dir[ $current_sample ]}$double$ROOT $mva${input_dir[ $current_sample ]} 0
	qsub -q short.q batch2.sh $path$qgd${input_dir[ $current_sample ]}$single$jets$ROOT $mva${input_dir[ $current_sample ]} 1
	current_sample=$(( $current_sample + 1 ))
done
