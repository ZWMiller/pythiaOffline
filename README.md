# pythiaOffline
Root code to be used for plotting output of NPE-h ccbar/bbbar correlation studies in Pythia. The output deltaPhi templates are plotted after scaling by trigger number. Two options, batch mode hides all canvas creation and no output is visible other than created files. PDFmake makes a pdf of the interesting output canvases.

Usage:

root
.L offline.C
offline("file_name_no_extension","mode")

mode 0 = no specified reaction type, mode 1 = c/cbar reaction, mode 2 = b/bbar reaction
