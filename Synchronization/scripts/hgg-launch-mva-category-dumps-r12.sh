#!/bin/bash
DATE=$(date +%b%d)
for DATASET in r12a r12b r12c r12d
do
    CONFIG_BASE=$CMSSW_BASE/src/MitHgg/Synchronization/python
    CONFIG=$CONFIG_BASE/dump_mva_categories_${DATASET}_cfg.py
    OUTPUT_BASE=$HOME/cms/hist/dumps
    OUTPUT=$OUTPUT_BASE/mit_mva_${DATASET}_${DATE}.txt
    COMMAND="hgg-dump-mva-categories $CONFIG > $OUTPUT &"
    echo $COMMAND
    eval $COMMAND
done
