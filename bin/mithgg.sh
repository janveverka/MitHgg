if [ -z $CMSSW_BASE ]
then
  echo ""
  echo " Setting up MitHgg failed! (\$CMSSW_BASE = is empty)."
  echo ""
else
  export MIT_HGG_DIR="$CMSSW_BASE/src/MitHgg"
  export PATH="$MIT_HGG_DIR/bin:${PATH}"
  export PYTHONPATH="$MIT_HGG_DIR/python:${PYTHONPATH}"
fi
