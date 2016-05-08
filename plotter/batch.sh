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

export MYBATCHDIR=/mnt/t3nfs01/data01/shome/nchernya/Hbb/plotter/
cd $MYBATCHDIR

root=.root

./plot $1 $2 $3 $4 $5 $6 $7 $8 $9 $TMPDIR
echo $8
gfal-mkdir srm://t3se01.psi.ch/pnfs/psi.ch/cms/trivcat/store/user/nchernya/Hbb/plotterOutput/$8/

xrdfs t3se01.psi.ch rm /store/user/nchernya/Hbb/plotterOutput/$8/$2_$4_$3_trig$7_$8_$9.root
xrdfs t3se01.psi.ch rm /store/user/nchernya/Hbb/plotterOutput/$8/$2_$4_$3_trig$7_$8_$9.txt

xrdcp $TMPDIR/$2_$4_$3_trig$7_$8_$9.root root://t3se01.psi.ch//store/user/nchernya/Hbb/plotterOutput/$8/
xrdcp $TMPDIR/$2_$4_$3_trig$7_$8_$9.txt root://t3se01.psi.ch//store/user/nchernya/Hbb/plotterOutput/$8/

#$ -o /mnt/t3nfs01/data01/shome/nchernya/Hbb/skim_trees/batch_logs/
#$ -e /mnt/t3nfs01/data01/shome/nchernya/Hbb/skim_trees/batch_logs/
