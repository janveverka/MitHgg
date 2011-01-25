#!/bin/bash
#===================================================================================================
# Script to merge all historgram files from a complete analysis task such that each sample with
# a well defined cross section is in one 'merged' file.
#
#                                                                             Ch.Paus (Aug 15, 2010)
#===================================================================================================

#---------------------------------------------------------------------------------------------------
# Basic definitions
#---------------------------------------------------------------------------------------------------
MACROS=/home/$USER/cms/root
CATALOG=/home/cmsprod/catalog
BOOK=t2mit/filefi
HIST=/home/$USER/cms/hist

NAME=hgg-v0

./mergeHist.py \
     --InputPath=$HIST/$NAME/$BOOK \
    --OutputPath=$HIST/$NAME/$BOOK/merged \
    --FilenameHeader=$NAME \
    --DatasetListFile=/home/$USER/cms/root/${NAME}.txt
