#!/bin/bash
#===================================================================================================
# Script to process our complete analysis task.
#
#                                                                             Ch.Paus (Aug 15, 2010)
#===================================================================================================

#---------------------------------------------------------------------------------------------------
# Basic definitions
#---------------------------------------------------------------------------------------------------
MACROS=/home/$USER/cms/root
CATALOG=~cmsprod/catalog
BOOK=t2mit/filefi
HIST=/home/$USER/cms/hist

#---------------------------------------------------------------------------------------------------
# Specific analysis macro
#---------------------------------------------------------------------------------------------------
NAME=hgg-v0
ANA=runHgg.C

#---------------------------------------------------------------------------------------------------
# Submitting the complete task
#---------------------------------------------------------------------------------------------------
echo " Config: ${MIT_HGG_DIR}/config/${NAME}.txt"
for dataset in `cat ${MIT_HGG_DIR}/config/${NAME}.txt | grep -v ^# | tr -s ' ' | cut -d' ' -f 2`
do
  # find the line to this dataset and do further analysis
  line=`grep $dataset ${MIT_HGG_DIR}/config/${NAME}.txt`
  # determine the MIT version of this sample
  export VERSION=`echo $line | tr -s ' ' | cut -d ' ' -f 1`
  # find potential JSON file
  export MIT_PROD_JSON=`echo $line | tr -s ' ' | cut -d ' ' -f 7`
  # find potential cut to remove overlap
  export MIT_PROD_OVERLAP=`echo $line | tr -s ' ' | cut -d ' ' -f 6`
  echo " JSON: $MIT_PROD_JSON  Overlap: $MIT_PROD_OVERLAP"
  # now submit the sucker
  submit.sh $ANA $CATALOG $BOOK/$VERSION $dataset noskim $NAME $HIST 1
done

exit 0
