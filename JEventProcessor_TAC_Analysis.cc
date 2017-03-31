// $Id$
//
//    File: JEventProcessor_TAC_Analysis.cc
// Created: Fri Feb 26 13:00:07 EST 2016
// Creator: zihlmann (on Linux gluon47.jlab.org 2.6.32-358.23.2.el6.x86_64 x86_64)
//

#include "JEventProcessor_TAC_Analysis.h"
using namespace jana;

// Routine used to create our JEventProcessor
#include <JANA/JApplication.h>
#include <JANA/JFactory.h>
extern "C" {
	void InitPlugin( JApplication *app ) {
		InitJANAPlugin( app );
		app->AddProcessor( new JEventProcessor_TAC_Analysis() );
	}
} // "C"

//------------------
// JEventProcessor_TAC_Analysis (Constructor)
//------------------
JEventProcessor_TAC_Analysis::JEventProcessor_TAC_Analysis() {

}

//------------------
// ~JEventProcessor_TAC_Analysis (Destructor)
//------------------
JEventProcessor_TAC_Analysis::~JEventProcessor_TAC_Analysis() {

}

//------------------
// init
//------------------
jerror_t JEventProcessor_TAC_Analysis::init( void ) {
	// This is called once at program startup. If you are creating
	// and filling historgrams in this plugin, you should lock the
	// ROOT mutex like this:
	//
	// japp->RootWriteLock();
	//  ... fill historgrams or trees ...
	// japp->RootUnLock();
	//

	View = 1;
	gPARMS->SetDefaultParameter<string,int>( "TAC:VIEW_FADC_HISTO", View );
	gPARMS->GetParameter( "TAC:VIEW_FADC_HISTO" )->GetValue( View );

	cout << "Option for viewing is " << View << endl;

	if ( View ) {
		int argc = 0;
		char *argv[10];
		theApp = new TApplication( "App", &argc, argv );
		myc = new TCanvas( "myc", "Flash ADC 250 data", 200, 10, 1000, 700 );
		hTAC = new TH1F( "hTAC", "TAC Flash ADC data", 100, 0., 100. );
		hTAC->GetXaxis()->SetTitle( "FlashADC sample number [#]" );
	}

	SigAmp = 0;
	SigAmpOf = 0;

	return NOERROR;
}

//------------------
// brun
//------------------
jerror_t JEventProcessor_TAC_Analysis::brun( JEventLoop *eventLoop, int32_t runnumber ) {
	// This is called whenever the run number changes

	RunNumber = runnumber;

	char hnam[128];
	char htit[128];
	for ( int k = 0; k < 4; k++ ) {

		sprintf( hnam, "TACAmp%d", k );
		sprintf( htit, "TAC Signal Amplitude thresh200 RUN %d Trigger %d", RunNumber, k );
		TACAmp[k] = new TH1F( hnam, htit, 500, 0., 5000. );
		TACAmp[k]->GetXaxis()->SetTitle( "TAC Amplitude" );

		sprintf( hnam, "sigtime%d", k );
		sprintf( htit, "TAC SignalTime thresh200 RUN %d Trigger %d", RunNumber, k );
		sigtime[k] = new TH1F( hnam, htit, 100, 0., 100. );

		sprintf( hnam, "tagHid%d", k );
		sprintf( htit, "TAGH Hits Detector ID RUN %d Trigger %d", RunNumber, k );
		tagHid[k] = new TH1F( hnam, htit, 320, 0., 320. );
		tagHid[k]->GetXaxis()->SetTitle( "Tagger Hodoscope Det. Number [#]" );

		sprintf( hnam, "tagHidMatched%d", k );
		sprintf( htit, "Matched TAGH Hits Detector ID RUN %d Trigger %d", RunNumber, k );
		tagHid_matched[k] = new TH1F( hnam, htit, 320, 0., 320. );
		tagHid_matched[k]->GetXaxis()->SetTitle( "Tagger Hodoscope Det. Number [#]" );

		sprintf( hnam, "tagHt%d", k );
		sprintf( htit, "TAGH ADC time RUN %d Trigger %d", RunNumber, k );
		tagHt[k] = new TH1F( hnam, htit, 2000, 0., 400. );

		sprintf( hnam, "tagMid%d", k );
		sprintf( htit, "TAGM Detector ID RUN %d Trigger %d", RunNumber, k );
		tagMid[k] = new TH1F( hnam, htit, 110, 0., 110. );
		tagMid[k]->GetXaxis()->SetTitle( "Tagger Microscope Column Number [#]" );

		sprintf( hnam, "tagMt%d", k );
		sprintf( htit, "TAGM ADC Time RUN %d Trigger %d", RunNumber, k );
		tagMt[k] = new TH1F( hnam, htit, 2000, 0., 400. );

		sprintf( hnam, "TACvsH%d", k );
		sprintf( htit, "TAC Amplitude vs TAGH# RUN %d Trigger %d", RunNumber, k );
		TACvsH[k] = new TH2F( hnam, htit, 320, 0., 320., 1000, 10., 5000. );
		TACvsH[k]->GetXaxis()->SetTitle( "Tagger Hodoscope Det. Number [#]" );
		TACvsH[k]->GetYaxis()->SetTitle( "TAC Amplitude" );

		sprintf( hnam, "TAGHTvsH%d", k );
		sprintf( htit, "TAGH T vs TAGH# RUN %d Trigger %d", RunNumber, k );
		taghTimeID[k] = new TH2F( hnam, htit, 320, 0., 320., 2000, 0., 400. );
		taghTimeID[k]->GetXaxis()->SetTitle( "Tagger Hodoscope Det. Number [#]" );
		taghTimeID[k]->GetYaxis()->SetTitle( "TAGH Time" );

		sprintf( hnam, "TACvsM%d", k );
		sprintf( htit, "TAC Amplitude vs TAGM# RUN %d Trigger %d", RunNumber, k );
		TACvsM[k] = new TH2F( hnam, htit, 110, 0., 110., 1000, 10., 5000. );
		TACvsM[k]->GetXaxis()->SetTitle( "Tagger Microscope Column Number [#]" );
		TACvsM[k]->GetYaxis()->SetTitle( "TAC Amplitude" );
	}

	return NOERROR;
}

//------------------
// evnt
//------------------
jerror_t JEventProcessor_TAC_Analysis::evnt( JEventLoop *loop, uint64_t eventnumber ) {
	// This is called for every event. Use of common resources like writing
	// to a file or filling a histogram should be mutex protected. Using
	// loop->Get(...) to get reconstructed objects (and thereby activating the
	// reconstruction algorithm) should be done outside of any mutex lock
	// since multiple threads may call this method at the same time.
	// Here's an example:
	//
	// vector<const MyDataClass*> mydataclasses;
	// loop->Get(mydataclasses);
	//
	// japp->RootWriteLock();
	//  ... fill historgrams or trees ...
	// japp->RootUnLock();

	// Get First Trigger Type
	const DL1Trigger *trig_words = nullptr;
	uint32_t trig_mask = 0;
	try {
		loop->GetSingle( trig_words );
	} catch ( ... ) {
	};
	if ( trig_words ) {
		trig_mask = trig_words->trig_mask;
	} else {
		trig_mask = 0;
	}

	//cout<<"Trigger mask: "<<trig_mask<<endl;

	if ( trig_mask == 0 ) {
		return NOERROR;
	}
	int TRIGGER = trig_mask;

	if ( (TRIGGER > 3) || (TRIGGER < 0) ) {
		return NOERROR;
	}

	vector<const Df250WindowRawData*> RawData;
	loop->Get( RawData );

	const Df250WindowRawData* TACRaw = NULL;
	vector<const Df250WindowRawData*> TaggerHits;
	int cnt = 0;

	for ( unsigned int k = 0; k < RawData.size(); k++ ) {

		const Df250WindowRawData* hit = RawData[k];
		if ( hit->rocid == 14 ) { // This is FCAL4
			if ( (hit->slot == 20) && hit->channel == 0 ) { // This is TAC detector
				TACRaw = hit;
				cnt = 1;
			}
		} else if ( (hit->rocid == 71) || (hit->rocid == 73) ) { // This is TAGM or TAGH
			TaggerHits.push_back( hit );
		}

	}

	//cout << eventnumber << "      " << cnt << "     " << TaggerHits.size() <<endl;

	float TACMax = 0;
//	double fdcMinCut = 350;
//	double fdcMaxCut = 450;
	double fdcMinCut = 0;
	double fdcMaxCut = 45000;
	
	japp->RootWriteLock();

	if ( cnt ) {
		SigAmp++;
		if ( View ) {
			hTAC->Reset();
			hTAC->SetLineColor( 1 );
		}
		int overflow = 0;
		int max = 0;
		for ( unsigned int k = 0; k < TACRaw->samples.size(); k++ ) {
			if ( TACRaw->samples[k] > max ) {
				max = TACRaw->samples[k];
			}
//			if ( View ) {
//				hTAC->Fill( (float) k, TACRaw->samples[k] );
//			}
			if ( TACRaw->samples[k] > 4095 ) {
				overflow = 1;
			}
		}
		if( View && ( fdcMaxCut < max || max < fdcMinCut ) ) return NOERROR;
		if ( overflow ) {
			SigAmpOf++;
		}
		if ( View ) {

			for ( unsigned iSample = 0; iSample < TACRaw->samples.size(); iSample++ ) {
				if ( fdcMinCut < max && max < fdcMaxCut ) {
					hTAC->Fill( (float) iSample, TACRaw->samples[iSample] );
				}
			}
			myc->Clear();
			if ( overflow ) {
				hTAC->SetLineColor( 2 );
			}
			hTAC->GetYaxis()->SetRangeUser( 0., 1000. );
			hTAC->Draw( "L" );
			gPad->SetGrid();
			gPad->Update();
			if( fdcMinCut < max && max < fdcMaxCut )	sleep( 1 );
//			sleep( 1 );

		}

		if ( overflow ) {
			max = 4500;
		}
		TACAmp[TRIGGER]->Fill( (float) max );
		TACMax = max;
	}

	if ( !(eventnumber % 10000) ) {
		if ( SigAmp )
			cout << "Overflow = " << (float) SigAmpOf / (float) SigAmp * 100 << "%" << endl;
	}

	// find time of signal > 200
	if ( cnt ) {
		int t = 0;
		for ( unsigned int k = 0; k < TACRaw->samples.size(); k++ ) {
			if ( TACRaw->samples[k] > 200 ) {
				t = k;
				break;
			}
		}

		sigtime[TRIGGER]->Fill( (float) t );
	}

	// Tagger Hits
	if ( TaggerHits.size() > 0 ) {
		//cout<<TaggerHits.size()<<endl;
		vector<const DTAGHDigiHit*> TAGHHits;
		loop->Get( TAGHHits );
		for ( unsigned int k = 0; k < TAGHHits.size(); k++ ) {
			const DTAGHDigiHit* hit = TAGHHits[k];
			int ID = hit->counter_id;
			float time = (float) hit->pulse_time / 16.;

			taghTimeID[TRIGGER]->Fill( (float)ID, time );

			if( TMath::Abs( time - 105 ) < 20.0 ) {
				tagHid_matched[TRIGGER]->Fill( (float) ID );
			}
			tagHid[TRIGGER]->Fill( (float) ID );
			tagHt[TRIGGER]->Fill( time );

			if ( TMath::Abs( time - 105 ) < 20.0 ) {
				TACvsH[TRIGGER]->Fill( (float) ID, TACMax );
			}
		}

		vector<const DTAGMDigiHit*> TAGMHits;
		loop->Get( TAGMHits );
		for ( unsigned int k = 0; k < TAGMHits.size(); k++ ) {
			const DTAGMDigiHit* hit = TAGMHits[k];
			int ID = hit->column;
			float time = (float) hit->pulse_time / 16.;

			tagMid[TRIGGER]->Fill( (float) ID );
			tagMt[TRIGGER]->Fill( time );

			if ( TMath::Abs( time - 100 ) < 10. ) {
				TACvsM[TRIGGER]->Fill( (float) ID, TACMax );
			}
		}

	}

	int m = eventnumber % 200000;
	if ( m == 0 ) {
		WriteHistos();
	}
	
        japp->RootUnLock();


	return NOERROR;
}

//------------------
// erun
//------------------
jerror_t JEventProcessor_TAC_Analysis::erun( void ) {
	// This is called whenever the run number changes, before it is
	// changed to give you a chance to clean up before processing
	// events from the next run number.
	return NOERROR;
}

//------------------
// fini
//------------------
jerror_t JEventProcessor_TAC_Analysis::fini( void ) {
	// Called before program exit after event processing is finished.
	WriteHistos();
	return NOERROR;
}

jerror_t JEventProcessor_TAC_Analysis::WriteHistos( void ) {
	// Called before program exit after event processing is finished.
	char fnam[128];
	sprintf( fnam, "tac_run%d.root", RunNumber );
	TFile *OUTF = new TFile( fnam, "RECREATE" );
	OUTF->cd();
	for ( int k = 0; k < 4; k++ ) {
		TACAmp[k]->Write();
		sigtime[k]->Write();
		tagHid[k]->Write();
		tagHid_matched[k]->Write();
		tagHt[k]->Write();
		tagMid[k]->Write();
		tagMt[k]->Write();
		TACvsH[k]->Write();
		taghTimeID[k]->Write();
		TACvsM[k]->Write();

	}

	OUTF->Close();

	return NOERROR;
}

