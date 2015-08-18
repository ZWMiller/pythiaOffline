#!/bin/bash
if [ "$#" -eq 3 ]; then
scp zamiller@rftpexp.rhic.bnl.gov:/star/u/zamiller/simu/NPETemplates/output/pythia_tree_$1_$2_$3.root outputs/.

elif [ "$#" -eq 2 ]; then
scp zamiller@rftpexp.rhic.bnl.gov:/star/u/zamiller/simu/NPETemplates/output/pythia_tree_$1_$2.root outputs/.
fi

if [ $? -eq 0 ]; then
    echo "pythia_tree_$1_$2.root moved to local directory."
else
    echo "---SCP FAILED---"
fi


