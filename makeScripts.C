/*
Makes the .cmnd file that is used to set the running environment and execute the pythia generation

Z. Miller - 8/26/15
 */


void makeScripts(Int_t mode = 1, Int_t pthatmin = 0, Int_t pthatmax = 100,Int_t maxRuns = 25)
{

  // For making scripts to use in running online
  char fName[100];
  char temp[200];
  for(Int_t ii=0;ii<maxRuns;ii++)
    {
      if(mode == 1)
	sprintf(fName, "script/run_C%i_%i_%i.csh", pthatmin,pthatmax,ii);
      if(mode == 2)
	sprintf(fName, "script/run_B%i_%i_%i.csh", pthatmin,pthatmax,ii);
      std::ofstream outFile(fName,std::ofstream::out);
      outFile << "#!/bin/csh" << endl << endl;
      outFile << "source /star/u/zamiller/.cshrc" << endl;
      outFile << "cd /star/u/zbtang/myTools/root/" << endl;
      outFile << "source bin/thisroot.csh" << endl;
      outFile << "cd /star/u/zamiller/simu/ptHatTemplates" << endl;
      sprintf(temp, "./NPEHDelPhiCorr cards/NpeC%i_%i_%i.cmnd output/NpeCHcorr%i_%i_%i.root C",pthatmin,pthatmax,ii,pthatmin,pthatmax,ii);
      outFile << temp << endl;
      outFile.close();
      }
}
