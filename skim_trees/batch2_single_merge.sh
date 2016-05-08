#source $VO_CMS_SW_DIR/cmsset_default.sh
# shopt -s expand_aliases is needed if you want to use the alias 'cmsenv' created by $VO_CMS_SW_DIR/cmsset_default.sh instead of the less mnemonic eval `scramv1 runtime -sh`

source $VO_CMS_SW_DIR/cmsset_default.sh
source /swshare/psit3/etc/profile.d/cms_ui_env.sh  # for bash

export MYCMSENVDIR=/mnt/t3nfs01/data01/shome/nchernya/CMSSW_7_4_10/src/
cd $MYCMSENVDIR
eval `scramv1 runtime -sh`
shopt -s expand_aliases 
cmsenv
#export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/usr/lib64/dcap 

export MYBATCHDIR=/mnt/t3nfs01/data01/shome/nchernya/Hbb/skim_trees/
cd $MYBATCHDIR

export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/usr/lib64/dcap

path=root://t3dcachedb.psi.ch:1094///pnfs/psi.ch/cms/trivcat/
path2=/store/user/nchernya/Hbb/v20/skimmed/$1/

xrdfs t3se01.psi.ch rm $path2$1_v20.root

xrdfs t3se01.psi.ch ls $path2 > $TMPDIR/list.txt
awk -v path="$path" '{print path $0}' $TMPDIR/list.txt > $TMPDIR/list2.txt

hadd  -f0 -k  -O $TMPDIR/$1_v20.root  `cat $TMPDIR/list2.txt | xargs` 
 

xrdcp $TMPDIR/$1_v20.root root://t3se01.psi.ch/$path2
 
#$ -o /mnt/t3nfs01/data01/shome/nchernya/Hbb/skim_trees/batch_logs/
#$ -e /mnt/t3nfs01/data01/shome/nchernya/Hbb/skim_trees/batch_logs/
