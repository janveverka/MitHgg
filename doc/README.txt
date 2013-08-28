== HOW TO INSTALL ==
Here are instructions on how to setup the MIT Hgg analysis 
on t3btch09[0-9].mit.edu using Bash.  Mingming put
them together for Jan on 20 Jun 2013.  Jan attemted
to clean them up a bit in the process of going through
them.

BAMBU_VERSION=029

###########
# 1. install CMSSW
###########
cp /home/mingyang/.bashrc ~/.bashrc
mkdir -p cms/MyProxy
cp /home/mingyang/cms/INIT ~/cms
source ~/.bashrc
eval cms${BAMBU_VERSION}

############
# 2. install Bamboo
############

cp /home/mingyang/cms/installBambu.sh ~/cms
cd ~/cms
chmod +x installBambu.sh
./installBambu.sh
cd ~/cms/cmssw/$BAMBU_VERSION/CMSSW_5_3_2_patch4/src
cvs co -d MitHgg   -r HEAD UserCode/MitHgg
cvs co -d MitPhysics   -r HEAD UserCode/MitPhysics
cvs co -d MitAna   -r HEAD UserCode/MitAna
cvs co -d MitCommon   -r HEAD UserCode/MitCommon
scram build -j4

cp    /home/mingyang/cms/example/runHgg2013Moriond.C  ~/cms/cmssw/029/CMSSW_5_3_2_patch4/src/MitHgg/macros
cp    /home/mingyang/cms/example/hgg.env              ~/cms/cmssw/029/CMSSW_5_3_2_patch4/src/MitHgg/config
cp    /home/mingyang/cms/example/hgg-2013Moriond.txt  ~/cms/cmssw/029/CMSSW_5_3_2_patch4/src/MitHgg/config
mkdir ~/cms/json
cp    /home/mingyang/cms/example/2013Moriond_190456-208686.json     ~/cms/json

# Comment: please replace "username" in hgg.env with your username

mkdir /scratch3/$(whoami)/condor

#############
# 3. configure root
#############

cp /home/paus/cms/root/.rootlogon.C ~/root/.rootlogon.C

######################
# 4. configuration and initialization
######################

source ~/.bashrc
kinit
cms029
mithgg
source  ~/cms/cmssw/029/CMSSW_5_3_2_patch4/src/MitHgg/config/hgg.env

######################
# 5. get the json
######################

mkdir ~/cms/json
cp    /home/mingyang/cms/example/2013Moriond_190456-208686.json     ~/cms/json

#######################
# 6. run the analysis
#You need to be on t3btch0{88,89,90}.mit.edu
#######################

cd ~/cms/cmssw/029/CMSSW_5_3_2_patch4/src/MitAna/bin
./process.sh

#######################
# 7. merge histograms
#######################

cd cms/cmssw/029/CMSSW_5_3_2_patch4/src/MitHgg/bin
./mergeHgg.sh

#######################
# 8. check the output histograms
#######################

cd /scratch3/username/hist/merged


#######################

