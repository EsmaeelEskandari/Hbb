export WORKDIR=/afs/cern.ch/work/n/nchernya/tmva/TMVA-v4.2.0/test/qgl/
cd $WORKDIR

g++ make_QGLtree.C -g -o treeQGL  -lTMVA `root-config --cflags --glibs`

cp treeQGL /shome/nchernya/Hbb/skim_trees/v14/qgd/ 

max_samples_num=18
input_dir=(VBFHToBB_M-125_13TeV_powheg BTagCSV QCD_HT100to200 QCD_HT200to300 QCD_HT300to500 QCD_HT500to700 QCD_HT700to1000 QCD_HT1000to1500 QCD_HT1500to2000 QCD_HT2000toInf VBFHToBB_M-125_13TeV_powheg TTbar DYtoQQ ST_tW ttHtobb ttHtoNbb DYtoLL GF)
#path=/shome/nchernya/Hbb/skim_trees/v14/
#path2=root://t3se01.psi.ch//store/user/nchernya/
#path2=root://t3se01.psi.ch//store/user/nchernya/
#path2=root://t3se01.psi.ch:22125//pnfs/psi.ch/cms/trivcat//store/user/nchernya/
#path2=dcap://t3se01.psi.ch:22125//pnfs/psi.ch/cms/trivcat//store/user/nchernya/
path=dcap://t3se01.psi.ch:22125//pnfs/psi.ch/cms/trivcat///store/user/nchernya/Hbb/v14/skimmed/
QGD=qgd/qgd_
v14=_v14
v2=_2
ROOT=.root
double=_double
single=_single
slash=/


#current_sample=0
current_sample=5
while [ $current_sample -lt 6 ]  #$max_samples_num ]
do	
	qsub -q short.q batch2.sh $path${input_dir[ $current_sample ]}$v14$ROOT ${input_dir[ $current_sample ]} 0
#	qsub -q short.q batch2.sh $path${input_dir[ $current_sample ]}$v14$single$v2$ROOT ${input_dir[ $current_sample ]} 1
#	./treeQGL $path${input_dir[ $current_sample ]}$v14$slash${input_dir[ $current_sample ]}$v14$ROOT $path$QGD${input_dir[ $current_sample ]} 0
	#./treeQGL   $path${input_dir[ $current_sample ]}$v14$single$slash${input_dir[ $current_sample ]}$v14$single$v2$ROOT $path$QGD${input_dir[ $current_sample ]} 1
	current_sample=$(( $current_sample + 1 ))
done
