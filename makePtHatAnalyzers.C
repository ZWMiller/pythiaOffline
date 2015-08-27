
Int_t makeScripts(Int_t,Int_t,Int_t,Int_t,const char*);
Int_t makeCards(Int_t,Float_t,Float_t,Int_t,const char*);
Int_t makeJobs(Int_t,Int_t,Int_t,Int_t,const char*);

void makePtHatAnalyzers(Int_t mode = 1, Int_t pthatmin = 0, Int_t pthatmax = 100,Int_t maxRuns = 50,const char* folder="recent")
{
  if(mode < 1 || mode > 2)
    {
      cout << "Not a valid mode. Choose 1=ccbar, or 2==bbar\n";
      exit(1);
    }
  /// Make the directory system
  gSystem->mkdir(folder);
  char temp[100];
  sprintf(temp,"%s/script",folder);
  gSystem->mkdir(temp);
  sprintf(temp,"%s/cards",folder);
  gSystem->mkdir(temp);
  sprintf(temp,"%s/jobs",folder);
  gSystem->mkdir(temp);

  // Create the files (cards, job list, execution scripts)
  Int_t ms=0,mc=0,mj=0;
  ms = makeScripts(mode,pthatmin,pthatmax,maxRuns,folder);
  mc = makeCards(mode,(Float_t)pthatmin,(Float_t)pthatmax,maxRuns,folder);
  mj = makeJobs(mode,pthatmin,pthatmax,maxRuns,folder);

  if(ms && mc && mj)
    cout << "\nAnalyzer succesfully created\n"
	 << "move to RCF with: scp filename zamiller@rftpexp.rhic.bnl.gov:/star/u/zamiller/directory\n" << endl;
  else
    cout << "\nFailed, Debug Codes: \n"
	 << "ms: " << ms << " mc: " << mc << " mj: " << mj << endl << endl;

}

Int_t makeScripts(Int_t mode, Int_t pthatmin, Int_t pthatmax, Int_t maxRuns, const char* folder)
{

  // For making scripts to use in running online
  char fName[100];
  char temp[200];
  for(Int_t ii=0;ii<maxRuns;ii++)
    {
      if(mode == 1)
	sprintf(fName, "%s/script/run_C%i_%i_%i.csh",folder, pthatmin,pthatmax,ii);
      if(mode == 2)
	sprintf(fName, "%s/script/run_B%i_%i_%i.csh",folder, pthatmin,pthatmax,ii);
      std::ofstream outFile(fName,std::ofstream::out);
      outFile << "#!/bin/csh" << endl << endl;
      outFile << "source /star/u/zamiller/.cshrc" << endl;
      outFile << "cd /star/u/zbtang/myTools/root/" << endl;
      outFile << "source bin/thisroot.csh" << endl;
      outFile << "cd /star/u/zamiller/simu/ptHatTemplates" << endl;
      if(mode==1)
	sprintf(temp, "./NPEHDelPhiCorr cards/NpeC%i_%i_%i.cmnd output/npeC%i_%i/NpeCHcorr%i_%i_%i.root C",pthatmin,pthatmax,ii,pthatmin,pthatmax,pthatmin,pthatmax,ii);
      if(mode==2)
	sprintf(temp, "./NPEHDelPhiCorr cards/NpeB%i_%i_%i.cmnd output/npeB%i_%i/NpeBHcorr%i_%i_%i.root B",pthatmin,pthatmax,ii,pthatmin,pthatmax,pthatmin,pthatmax,ii);
      outFile << temp << endl;
      outFile.close();
      }
  return 1;
}

Int_t makeJobs(Int_t mode, Int_t pthatmin, Int_t pthatmax, Int_t maxRuns, const char* folder)
{

  // For making jobs to submit online running
  char fName[100];
  char temp[200];
  
  if(mode == 1)
    sprintf(fName, "%s/jobs/run_C%i_%i.job", folder,pthatmin,pthatmax);
  if(mode == 2)
    sprintf(fName, "%s/jobs/run_B%i_%i.job", folder,pthatmin,pthatmax);
  std::ofstream outfile(fName,std::ofstream::out);
  outfile << "Universe        = vanilla\nNotification    = never\nRequirements    = (CPU_Type != \"crs\") && (CPU_Experiment == \"star\")\nRank            = CPU_Speed\nPriority        = +19\nGetEnv          = True\nInitialdir      = /star/u/zamiller/simu/ptHatTemplates\nInput           = /dev/null\nNotify_user     = zamiller@rcf.rhic.bnl.gov\nGetEnv          = True\n+Experiment     = \"star\"\n+Job_Type       = \"cas\"\n\n";
  if(mode == 1)
    for(Int_t run=0; run < maxRuns; run++)
      {
	sprintf(temp,"Executable       = script/run_C%i_%i_%i.csh\n",pthatmin,pthatmax,run);
	outfile << temp;
	sprintf(temp,"Output           = log/C%i_%i_%i.out\n",pthatmin,pthatmax,run);
	outfile << temp;
	sprintf(temp,"Error            = log/C%i_%i_%i.err\n",pthatmin,pthatmax,run);
	outfile << temp;
	sprintf(temp,"Log              = log/C%i_%i_%i.olog\n",pthatmin,pthatmax,run);
	outfile << temp;
	outfile << "Queue\n\n";
	
      }
  if(mode == 2)
    for(Int_t run=0; run < maxRuns; run++)
      {
	sprintf(temp,"Executable       = script/run_B%i_%i_%i.csh\n",pthatmin,pthatmax,run);
	outfile << temp;
	sprintf(temp,"Output           = log/B%i_%i_%i.out\n",pthatmin,pthatmax,run);
	outfile << temp;
	sprintf(temp,"Error            = log/B%i_%i_%i.err\n",pthatmin,pthatmax,run);
	outfile << temp;
	sprintf(temp,"Log              = log/B%i_%i_%i.olog\n",pthatmin,pthatmax,run);
	outfile << temp;
	outfile << "Queue\n\n";
	
      }
  outfile.close();
  return 1;
}


Int_t makeCards(Int_t mode, Float_t pthatmin, Float_t pthatmax, Int_t numRuns, const char* folder)
{
  
  for(Int_t run=0; run < numRuns; run++){
    // open an output file
    char ch[1000];
    if(mode == 1)
      sprintf(ch,"%s/cards/NpeC%.0f_%.0f_%i.cmnd",folder,pthatmin,pthatmax,run);
    if(mode == 2)
      sprintf(ch,"%s/cards/NpeB%.0f_%.0f_%i.cmnd",folder,pthatmin,pthatmax,run);
    
    std::ofstream outfile (ch);
    
    
    outfile << "#==============================================================================\n# STAR Heavy Flavor Tune 1.0\n#\n# PYTHIA Version 8.1.08\n# Date: September 9, 2008\n# Last updated by: Thomas Ullrich\n#\n# This file contains commands to be read in for a Pythia8 run. \n# Lines not beginning with a letter or digit are comments.\n# Names are case-insensitive  -  but spellings-sensitive!\n#==============================================================================\n\n#------------------------------------------------------------------------------\n# Parameters that need to be set by whoever runs it.\n# Note that they have no meaning unless restored and used\n# in the user provided code (main program).\n# This is not part of the star_hf tune (just convenient)\n# Documentation: <pyhiadir>/htmldoc/MainProgramSettings.html\n#------------------------------------------------------------------------------\nMain:numberOfEvents = 500000      ! number of events to generate\nMain:numberToList = 10              ! number of events to print\nMain:timesToShow = 1000            ! show how far along run is this many times\nMain:timesAllowErrors = 50000     ! abort run after this many flawed events\nMain:showChangedSettings = on      ! print changed flags/modes/parameters\nMain:showAllSettings = on          ! print all flags/modes/parameters\nMain:showAllStatistics = on        ! print statistics at the end\n\n#------------------------------------------------------------------------------\n# Colliding beams and center-of-mass energy\n# Documentation: <pyhiadir>/htmldoc/BeamParameters.html\n#------------------------------------------------------------------------------\nBeams:idA = 2212                  ! proton\nBeams:idB = 2212                  ! proton\nBeams:eCM = 200.                  ! RHIC nominal (GeV)\n\n#------------------------------------------------------------------------------\n# Process Selection\n# Make you selection by uncommenting the referring switches\n# \n# Warning: the b and c producing processes do not catch all possible \n# production modes. You would need to use HardQCD:all or even SoftQCD:minBias\n# for that. But the hard ones are the dominating ones and they are in.\n# Note that for pt -> 0 things might go very wrong. A lower pTHat cut avoids\n# this especially for charm and bottom production.\n# Documentation: <pyhiadir>/htmldoc/QCDProcesses.html\n# Documentation: <pyhiadir>/htmldoc/OniaProcesses.html\n#------------------------------------------------------------------------------\n# Uncomment for charmonium\n#Charmonium:all = on   ! charmonium production\n\n# Uncomment for charmonium singlet only\n# Charmonium:gg2QQbar[3S1(1)]g = on\n# Charmonium:gg2QQbar[3P0(1)]g = on\n# Charmonium:gg2QQbar[3P1(1)]g = on\n# Charmonium:gg2QQbar[3P2(1)]g = on\n# Charmonium:qg2QQbar[3P0(1)]q = on\n# Charmonium:qg2QQbar[3P1(1)]q = on\n# Charmonium:qg2QQbar[3P2(1)]q = on\n# Charmonium:qqbar2QQbar[3P0(1)]g = on\n# Charmonium:qqbar2QQbar[3P1(1)]g = on\n# Charmonium:qqbar2QQbar[3P2(1)]g = on\n\n# Uncomment for charmonium octett only\n# Charmonium:gg2QQbar[3S1(8)]g = on\n# Charmonium:gg2QQbar[1S0(8)]g = on\n# Charmonium:gg2QQbar[3PJ(8)]g = on\n# Charmonium:qg2QQbar[3S1(8)]q = on\n# Charmonium:qg2QQbar[1S0(8)]q = on\n# Charmonium:qg2QQbar[3PJ(8)]q = on\n# Charmonium:qqbar2QQbar[3S1(8)]g = on\n# Charmonium:qqbar2QQbar[1S0(8)]g = on\n# Charmonium:qqbar2QQbar[3PJ(8)]g = on\n\n# Uncomment for bottomonium\n# Bottomonium:all = on  ! bottomonium production\n\n# Uncomment for Drell-Yan\n# WeakSingleBoson:ffbar2gmZ = on\n\n# Hard processes main switch \n# HardQCD:all = on\n\n# Minimum bias \n# SoftQCD:minBias = on\n#------------------------------------------------------------------------------\n# K factor\n# Multiply almost all cross sections by this common fix factor.\n# This is usually no very useful. The data can be shifted up and down\n# later anyhow as we please. \n# Documentation: <pyhiadir>/htmldoc/CouplingsAndScales.html\n#------------------------------------------------------------------------------\n# SigmaProcess:Kfactor = 3\n\n#------------------------------------------------------------------------------\n# Scales (Ramona's suggestions)\n# This sets the scale to settings typically for hard probes:\n# mu_F = mu_R = 2*mT\n# Documentation: <pyhiadir>/htmldoc/CouplingsAndScales.html\n#------------------------------------------------------------------------------\nSigmaProcess:renormScale2 = 3\nSigmaProcess:factorScale2 = 3\nSigmaProcess:renormMultFac = 2   ! 2mT\nSigmaProcess:factorMultFac = 2   ! 2mT\n\n#------------------------------------------------------------------------------\n# PDF Selection:\n# Note: you need LHAPDF to be installed. Pythia 8 only provides a \n# minimal set to get started.\n# The choice of PDF here is greatly motivated by:\n# A.~Sherstnev and R.~S.~Thorne, arXiv:0807.2132 and arXiv:0711.2473v3\n# and W. Vogelsang (private communication)\n# These are PDFs especially made for LO Monte-Carlo generators such\n# as PYTHIA.\n# The state-of-the-art NLO PDF is cteq66.LHgrid which can be used\n# as an alternative (also from LHAPDF) but with the known problems\n# that arise when using a NLO PDF in an LO simulator.\n# Documentation: <pyhiadir>/htmldoc/PDFSelection.html\n#------------------------------------------------------------------------------\nPDF:useLHAPDF = on\nPDF:LHAPDFset = MRSTMCal.LHgrid\nPDF:extrapolateLHAPDF = on\n\n#------------------------------------------------------------------------------\n# Settings for the event generation process in the Pythia8 library\n# Effect/Relevance of MI, ISR and FSR need to be checked. For sure\n# the use more CPU and hence less events/s.\n# If HadronLevel:Hadronize = off we end up with the pure c, b spectra\n# (which might be useful at times)\n# Documentation: <pyhiadir>/htmldoc/MasterSwitches.html\n# Documentation: <pyhiadir>/htmldoc/MultipleInteractions.html\n#------------------------------------------------------------------------------\nPartonLevel:MI = on              ! multiple interactions\nPartonLevel:ISR = on             ! initial-state radiation \nBeamRemnants:primordialKT = on    ! primordial kt\nPartonLevel:FSR = on             ! final-state radiation\n#HadronLevel:Hadronize = off     ! no hadronization use\n\n#------------------------------------------------------------------------------\n# Relative production ratio vector/pseudoscalar for charm and bottom mesons\n# This was originally PARJ(13) where PARJ(13) = V/(PS+V) that is the \n# vector meson  fraction of primary charm+bottom mesons. \n# Andre David (CERN\nA60) made an exhaustive study and found that the\n# world data supports 0.6 while PYTHIA default was PARJ(13) = 3/4 = 0.75\n# from simple spin counting.\n# In PYTHIA8 we now use V/PS not V/(PS+V)\n# Documentation: <pyhiadir>/htmldoc/FlavourSelection.html\n#------------------------------------------------------------------------------\nStringFlav:mesonCvector = 1.5    ! same as PARJ(13)=0.6   -> 1.5\nStringFlav:mesonBvector = 3      ! leave at PARJ(13)=0.75 -> 3\n\n#------------------------------------------------------------------------------\n# Heavy quark masses.\n# Note that this should match with the ones used in the PDF.\n# The masses are listed in the header of the refering PDF file.\n# Documentation: <pyhiadir>/htmldoc/ParticleDataScheme.html\n# Documentation: <pyhiadir>/htmldoc/ParticleData.html\n#------------------------------------------------------------------------------\n4:m0 = 1.43\n5:m0 = 4.30\n\n#------------------------------------------------------------------------------# Particle Decay limits\n# When on, only particles with a decay within a volume limited by \n# rho = sqrt(x^2 + y^2) < xyMax and |z| < zMax are decayed. \n# The above xyMax, expressed in mm/c.\n#------------------------------------------------------------------------------\nParticleDecays:limitCylinder = on\nParticleDecays:xyMax = 600\nParticleDecays:zMax = 1000\n\nOther options below set in each card\n\n#------------------------------------------------------------------------------\n# To limit particle production to a certain pthat range uncomment\n# these lines. Use only when you 100% know what you are doing.\n# It is extremely useful to split runs up in ptHat bins to generate\n# statistics evenly in pt. Book keeping is important then (cross-sections,\n# number of events) to compile the final complete spectra.\n# Documentation: <pyhiadir>/htmldoc/PhaseSpaceCuts.html\n#------------------------------------------------------------------------------\n\n#------------------------------------------------------------------------------\n# Random Number\n# Initialize random generator according to time. Otherwise multiple jobs\n# will produce the same sequence (unless you pass a different seed every\n# time which is not practical).\n# Documentation: <pythiadir>/htmldoc/RandomNumberSeed.html\n#------------------------------------------------------------------------------\n"; // End put of standard options

    if(mode == 1)
      {
	outfile << "\n# Uncomment next 2 lines for charm\n"
		<< "HardQCD:gg2ccbar = on    ! g g -> c cbar\n"
		<< "HardQCD:qqbar2ccbar = on ! q qbar -> c cbar\n\n"
		<< "# Uncomment next 2 lines for bottom\n"
		<< "# HardQCD:gg2bbbar = on    ! g g -> b bbar\n"
		<< "# HardQCD:qqbar2bbbar = on ! q qbar -> b bbar\n\n";
      }
    if(mode == 2)
      {
	outfile << "\n# Uncomment next 2 lines for charm\n"
		<< "# HardQCD:gg2ccbar = on    ! g g -> c cbar\n"
		<< "# HardQCD:qqbar2ccbar = on ! q qbar -> c cbar\n\n"
		<< "# Uncomment next 2 lines for bottom\n"
		<< "HardQCD:gg2bbbar = on    ! g g -> b bbar\n"
		<< "HardQCD:qqbar2bbbar = on ! q qbar -> b bbar\n\n";
      }
    
    outfile << "PhaseSpace:pTHatMin = " << pthatmin << "\n"
	    << "PhaseSpace:pTHatMax = " << pthatmax << "\n";
    
    outfile << "Random:setSeed = on\n"
	    << "Random:seed = " << 1000+run << "\n"
	    << "#EOF";
    
    outfile.close();
  }
  return 1;
}
