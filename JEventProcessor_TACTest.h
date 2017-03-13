// $Id$
//
//    File: JEventProcessor_TACTest.h
// Created: Fri Feb 26 13:00:07 EST 2016
// Creator: zihlmann (on Linux gluon47.jlab.org 2.6.32-358.23.2.el6.x86_64 x86_64)
//

#ifndef _JEventProcessor_TACTest_
#define _JEventProcessor_TACTest_

#include <JANA/JEventProcessor.h>
using namespace jana;
using namespace std;

#include <stdint.h>

#include "TRIGGER/DL1Trigger.h"
#include <DAQ/Df250PulseIntegral.h>
#include <DAQ/DDAQAddress.h>
#include <DAQ/Df250WindowRawData.h>
#include <TAGGER/DTAGHDigiHit.h>
#include <TAGGER/DTAGMDigiHit.h>


#include "TApplication.h"  // needed to display canvas
#include "TSystem.h"
#include "TFile.h"
#include "TH1.h"
#include "TH1F.h"
#include "TF1.h"
#include "TF2.h"
#include "TH2F.h"
#include "TMath.h"
#include "TCanvas.h"

class JEventProcessor_TACTest:public jana::JEventProcessor{
 public:
  JEventProcessor_TACTest();
  ~JEventProcessor_TACTest();
  const char* className(void){return "JEventProcessor_TACTest";}

  int RunNumber;
  int View;
  TApplication *theApp;
  TCanvas *myc;
  TH1F *hTAC;
  int SigAmp;
  int SigAmpOf;

  TH1F *TACAmp[4];
  TH1F *sigtime[4];
  TH1F *tagHid[4];
  TH1F *tagHid_matched[4];
  TH1F *tagHt[4] ;
  TH1F *tagMid[4];
  TH1F *tagMt[4] ;

  TH2F *TACvsH[4];

  TH2F *taghTimeID[4];

  TH2F *TACvsM[4];

 private:
  jerror_t init(void);
  jerror_t brun(jana::JEventLoop *eventLoop, int32_t runnumber);
  jerror_t evnt(jana::JEventLoop *eventLoop, uint64_t eventnumber);
  jerror_t erun(void);
  jerror_t fini(void);
  jerror_t WriteHistos(void);
};

#endif // _JEventProcessor_TACTest_

