/*
 * JEventProcessor_TAC_Monitor.h
 *
 *  Created on: Mar 30, 2017
 *      Author: hovanes
 */

#ifndef JEVENTPROCESSOR_TAC_MONITOR_H_
#define JEVENTPROCESSOR_TAC_MONITOR_H_

#include <iostream>
#include <map>
#include <vector>

#include <TH1.h>

#include <JANA/JEventProcessor.h>

class JEventProcessor_TAC_Monitor: public jana::JEventProcessor {
private:
	jerror_t init(void);          ///< Called once at program start.
	jerror_t brun(jana::JEventLoop *eventLoop, int32_t runnumber);          ///< Called everytime a new run number is detected.
	jerror_t evnt(jana::JEventLoop *eventLoop, uint64_t eventnumber);          ///< Called every event.
	jerror_t erun(void);          ///< Called everytime run number changes, provided brun has been called.
	jerror_t fini(void);          ///< Called after last event of last event source has been processed.


	// Map of all histograms for this monitoring plugin. the first index identifies the
	// name of the histogram, the second index (inner) identifies the trigger bit.
	static std::map<std::string, std::map<int,TH1*> > histoMap;

	// Method where the histograms are created
	virtual void createHistograms();
public:
	JEventProcessor_TAC_Monitor(){}
	virtual ~JEventProcessor_TAC_Monitor(){}

	const char* className(void) {
		return "JEventProcessor_TAC_Monitor";
	}

};

#endif /* JEVENTPROCESSOR_TAC_MONITOR_H_ */
