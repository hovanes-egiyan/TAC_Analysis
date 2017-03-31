/*
 * JEventProcessor_TAC_Monitor.cc
 *
 *  Created on: Mar 30, 2017
 *      Author: hovanes
 */

#include <iostream>
#include <map>
#include <vector>
#include <sstream>

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

#include <JANA/JApplication.h>

#include <DAQ/Df250PulseData.h>
#include <DAQ/Df250WindowRawData.h>
#include <DAQ/DEPICSvalue.h>
#include <TAC/DTACDigiHit.h>
#include <TAC/DTACHit.h>

#include "JEventProcessor_TAC_Monitor.h"


using namespace jana;
using namespace std;


// Routine used to create our JEventProcessor
extern "C"{
    void InitPlugin(JApplication *app){
        InitJANAPlugin(app);
        app->AddProcessor(new JEventProcessor_TAC_Monitor());
    }
}

jerror_t JEventProcessor_TAC_Monitor::init(void) {
    TDirectory *mainDir = gDirectory;
    TDirectory *tacDir = gDirectory->mkdir("TAC");
    tacDir->cd();
	createHistograms();


	return NOERROR;

}

jerror_t JEventProcessor_TAC_Monitor::brun(jana::JEventLoop* eventLoop,
		int32_t runnumber) {
	return NOERROR;
}

jerror_t JEventProcessor_TAC_Monitor::evnt(jana::JEventLoop* eventLoop,
		uint64_t eventnumber) {
	return NOERROR;
}

jerror_t JEventProcessor_TAC_Monitor::erun(void) {
	return NOERROR;
}

jerror_t JEventProcessor_TAC_Monitor::fini(void) {
	return NOERROR;
}

void JEventProcessor_TAC_Monitor::createHistograms() {
	stringstream histName;
	stringstream histTitle;
	for ( unsigned trigBit = 0; trigBit < 16; trigBit++ ) {
	histName << "TACAmp" << trigBit ;
	histTitle << "TAC Signal Amplitude Trigger " << trigBit;

	TACAmp[k] = new TH1F( hnam, htit, 500, 0., 5000. );
	TACAmp[k]->GetXaxis()->SetTitle( "TAC Amplitude" );
	}
}
