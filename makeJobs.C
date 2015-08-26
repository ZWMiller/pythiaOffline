
void makeJobs(Int_t mode = 1, Int_t pthatmin = 0, Int_t pthatmax = 100,Int_t maxRuns = 25)
{

  // For making jobs to submit online running
  char fName[100];
  char temp[200];
  
  if(mode == 1)
    sprintf(fName, "jobs/run_C%i_%i.job", pthatmin,pthatmax);
  if(mode == 2)
    sprintf(fName, "jobs/run_B%i_%i.job", pthatmin,pthatmax);
  std::ofstream outfile(fName,std::ofstream::out);
  outfile << "Universe        = vanilla\nNotification    = never\nRequirements    = (CPU_Type != \"crs\") && (CPU_Experiment == \"star\")\nRank            = CPU_Speed\nPriority        = +19\nGetEnv          = True\nInitialdir      = /star/u/zamiller/simu/NPETemplates\nInput           = /dev/null\nNotify_user     = zamiller@rcf.rhic.bnl.gov\nGetEnv          = True\n+Experiment     = \"star\"\n+Job_Type       = \"cas\"\n\n";
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
}
