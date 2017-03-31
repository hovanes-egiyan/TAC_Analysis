// $Id$
//
//    File: JEventProcessor_TAC_Analysis.h
// Created: Fri Feb 26 13:00:07 EST 2016
// Creator: zihlmann (on Linux gluon47.jlab.org 2.6.32-358.23.2.el6.x86_64 x86_64)
//

#ifndef _JEventProcessor_TAC_Analysis_
#define _JEventProcessor_TAC_Analysis_

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

class JEventProcessor_TAC_Analysis:public jana::JEventProcessor{
 public:
  JEventProcessor_TAC_Analysis();
  ~JEventProcessor_TAC_Analysis();
  const char* className(void){return "JEventProcessor_TAC_Analysis";}

  int RunNumber;
  int View = 0;
  TApplication *theApp = nullptr;
  TCanvas *myc = nullptr;
  TH1F *hTAC = nullptr;
  int SigAmp = 0;
  int SigAmpOf = 0;

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

 protected:
  virtual jerror_t init(void);
  virtual jerror_t brun(jana::JEventLoop *eventLoop, int32_t runnumber);
  virtual jerror_t evnt(jana::JEventLoop *eventLoop, uint64_t eventnumber);
  virtual jerror_t erun(void);
  virtual jerror_t fini(void);
  virtual jerror_t WriteHistos(void);
};

#endif // _JEventProcessor_TAC_Analysis_

