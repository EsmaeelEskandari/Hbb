export WORKDIR=`pwd`
cd $WORKDIR

g++ run_create_main_tmva_all.C -g -o run_all `root-config --cflags --glibs` 

max_samples_num=2  
path=dcap://t3se01.psi.ch:22125//pnfs/psi.ch/cms/trivcat///store/user/nchernya/Hbb/v14/qgd/
input_dir=( BTagCSV VBFHToBB_M-125_13TeV_powheg )
data=(1 0)
ROOT=.root
v14=_v14
single=_single
double=_double
qgd=qgd_
jets=_2jets


current_sample=0
while [ $current_sample -lt 1 ] #$max_samples_num ]
do	
	./run_all $path${input_dir[ $current_sample ]}$double$ROOT ${input_dir[ $current_sample ]} ${data[ $current_sample ]} 0
	./run_all $path$qgd${input_dir[ $current_sample ]}$single$jets$ROOT ${input_dir[ $current_sample ]} ${data[ $current_sample ]} 1

	current_sample=$(( $current_sample + 1 ))
done
