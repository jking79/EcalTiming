// -*- C++ -*-
//
// Package:    anatemplet/jwk_ana_event_lhcDump
// Class:      jwk_ana_event_lhcDump
//
/**\class Analyzer jwk_ana_event_lhcDump.cc EcalTiming/EcalTiming/plugins/jwk_ana_event_lhcDump.cc

 Description: [one line class summary]

 Implementation:
     [Notes on implementation]
*/
//
// Original Author:  Jack King
//         Created:  Fri, 01 Nov 2018 19:30:49 GMT
//
//


// system include files
#include <memory>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/one/EDAnalyzer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/InputTag.h"

#include "CondFormats/DataRecord/interface/LHCInfoRcd.h"
#include "CondFormats/RunInfo/interface/LHCInfo.h"

#include "FWCore/Utilities/interface/StreamID.h"

#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/Framework/interface/EventSetup.h"

#include "DataFormats/EcalRecHit/interface/EcalRecHitCollections.h"
#include "EcalTiming/EcalTiming/interface/EcalTimingEvent.h"
#include "DataFormats/EcalDetId/interface/EcalDetIdCollections.h"

#include "DataFormats/EcalDigi/interface/EcalDigiCollections.h"

#include "DataFormats/EcalDetId/interface/EBDetId.h"
#include "DataFormats/EcalDetId/interface/EEDetId.h"

#include <iterator>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#include <TFile.h>
#include <TTree.h>

#include "TH1F.h"
#include "TH2F.h"

#include <stdio.h>
#include <time.h>

//
// class declaration
//

// If the analyzer does not use TFileService, please remove
// the template argument to the base class so the class inherits
// from  edm::one::EDAnalyzer<>
// This will improve performance in multithreaded jobs.


//using reco::TrackCollection;

//class jwk_ana_event_lhcDump : public edm::one::EDAnalyzer<edm::one::SharedResources>  {
class jwk_ana_event_lhcDump : public edm::one::EDAnalyzer<>  {
   public:
      explicit jwk_ana_event_lhcDump(const edm::ParameterSet&);
      ~jwk_ana_event_lhcDump();

      static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);

			static size_t const bunchSlots = 3564;
			typedef std::bitset<bunchSlots+1>  bunchConfigType;

   private:
      virtual void beginJob() override;
      virtual void analyze(const edm::Event&, const edm::EventSetup&) override;
      virtual void endJob() override;

//			edm::EDGetTokenT<EcalTimingCollection> _timingEvents; ///< input collection

			typedef int FillTypeId;
			typedef int ParticleTypeId;

                        void initRoot();
                	void dbtoRoot( const LHCInfo& lhcInfo, edm::Handle<EcalRecHitCollection> RecHitEBHandle, edm::Handle<EcalRecHitCollection> RecHitEEHandle );
                        void dbtoRoot( const LHCInfo& obja, const EcalRecHit& rechit );
                        void closeRoot();

                        static float getsum( std::vector<float> list );
			static float getsum2( std::vector<float> list );
			static float getmean( std::vector<float> list );
			static float getstdev( std::vector<float> list );
			static float getstdev( std::vector<float> list, float mean );
			static float geterror( std::vector<float> list );
                        static float geterror( int num, float stdev );
                        static float getmaxval( std::vector<float> list );

                        static std::string timeToString(time_t t);
        		static std::string findAndReplaceAll(std::string data, std::string toSearch, std::string replaceStr);

      // ----------member data ---------------------------

                        TTree *tree;
                        TFile *tfile;

                        unsigned int run;
                        unsigned int lumi;
                        unsigned long  event;
                        unsigned int bx;
                        unsigned int lumiSection;
                        unsigned int fillNumber;
                        float energy;
                        float instlumi;

			// EB
			float rh_EB_mtime;
		        float rh_EB_s2time;
			float rh_EB_stime;
			unsigned int rh_EB_count;
                        float rh1GeV_EB_mtime;
                        float rh1GeV_EB_s2time;
                        float rh1GeV_EB_stime;
                        unsigned int rh1GeV_EB_count;
                        float rh2GeV_EB_mtime;
                        float rh2GeV_EB_s2time;
                        float rh2GeV_EB_stime;
                        unsigned int rh2GeV_EB_count;
                        float rh5GeV_EB_mtime;
                        float rh5GeV_EB_s2time;
                        float rh5GeV_EB_stime;
                        unsigned int rh5GeV_EB_count;
                        float rh10GeV_EB_mtime;
                        float rh10GeV_EB_s2time;
                        float rh10GeV_EB_stime;
                        unsigned int rh10GeV_EB_count;
                        float rh20GeV_EB_mtime;
                        float rh20GeV_EB_s2time;
                        float rh20GeV_EB_stime;
                        unsigned int rh20GeV_EB_count;

                        // EEP
                        float rh_EEP_mtime;
                        float rh_EEP_s2time;
                        float rh_EEP_stime;
                        unsigned int rh_EEP_count;
                        float rh1GeV_EEP_mtime;
                        float rh1GeV_EEP_s2time;
                        float rh1GeV_EEP_stime;
                        unsigned int rh1GeV_EEP_count;
                        float rh2GeV_EEP_mtime;
                        float rh2GeV_EEP_s2time;
                        float rh2GeV_EEP_stime;
                        unsigned int rh2GeV_EEP_count;
                        float rh5GeV_EEP_mtime;
                        float rh5GeV_EEP_s2time;
                        float rh5GeV_EEP_stime;
                        unsigned int rh5GeV_EEP_count;
                        float rh10GeV_EEP_mtime;
                        float rh10GeV_EEP_s2time;
                        float rh10GeV_EEP_stime;
                        unsigned int rh10GeV_EEP_count;
                        float rh20GeV_EEP_mtime;
                        float rh20GeV_EEP_s2time;
                        float rh20GeV_EEP_stime;
                        unsigned int rh20GeV_EEP_count;

                        // EEM
                        float rh_EEM_mtime;
                        float rh_EEM_s2time;
                        float rh_EEM_stime;
                        unsigned int rh_EEM_count;
                        float rh1GeV_EEM_mtime;
                        float rh1GeV_EEM_s2time;
                        float rh1GeV_EEM_stime;
                        unsigned int rh1GeV_EEM_count;
                        float rh2GeV_EEM_mtime;
                        float rh2GeV_EEM_s2time;
                        float rh2GeV_EEM_stime;
                        unsigned int rh2GeV_EEM_count;
                        float rh5GeV_EEM_mtime;
                        float rh5GeV_EEM_s2time;
                        float rh5GeV_EEM_stime;
                        unsigned int rh5GeV_EEM_count;
                        float rh10GeV_EEM_mtime;
                        float rh10GeV_EEM_s2time;
                        float rh10GeV_EEM_stime;
                        unsigned int rh10GeV_EEM_count;
                        float rh20GeV_EEM_mtime;
                        float rh20GeV_EEM_s2time;
                        float rh20GeV_EEM_stime;
                        unsigned int rh20GeV_EEM_count;

			unsigned int subtrain_position;
                        unsigned int train_position;
                        unsigned int subtrain_number;
                        unsigned int train_number;

        //                std::vector<float>      offset_bx_phc;
		
			float			ave_phase;
			float			dif_phase;
			float			vc2_phase;

			TH1F *h23_bPhase;
                        TH1F *h24_dPhase;
			TH1F *h25_Phase;
			
			TH1I *h26_bxOcc;
			TH1F *h27_filledbx;

         		TH1F *h28_rh_EB_Energy;
         		TH1F *h29_rh_EEP_Energy;
         		TH1F *h30_rh_EEM_Energy;

			edm::EDGetTokenT<EBRecHitCollection> _ecalRecHitsEBtoken;
			edm::EDGetTokenT<EERecHitCollection> _ecalRecHitsEEtoken;
//     			edm::InputTag _barrelDigiCollection; //!< secondary name given to collection of digis	
//	   		edm::InputTag _endcapDigiCollection; //!< secondary name given to collection of digis


};

//
// constants, enums and typedefs
//

//
// static data member definitions
//

//
// constructors and destructor
//
jwk_ana_event_lhcDump::jwk_ana_event_lhcDump(const edm::ParameterSet& iConfig):
//	_timingEvents(consumes<EcalTimingCollection>(iConfig.getParameter<edm::InputTag>("timingCollection")))
//	_barrelDigiCollection (iConfig.getParameter<edm::InputTag> ("EBdigiCollection")),
//	_endcapDigiCollection (iConfig.getParameter<edm::InputTag> ("EEdigiCollection")),
	_ecalRecHitsEBtoken(consumes<EBRecHitCollection>(iConfig.getParameter<edm::InputTag>("recHitEBCollection"))),
	_ecalRecHitsEEtoken(consumes<EERecHitCollection>(iConfig.getParameter<edm::InputTag>("recHitEECollection")))
{
   //now do what ever initialization is needed

}


jwk_ana_event_lhcDump::~jwk_ana_event_lhcDump()
{

   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)

}


//
// member functions
//
/* ----  notes from python file -----
def maxval( timelist):

        maxtime = 0
        for time in timelist:
                if time > maxtime : time = maxtime
        return maxtime

def getMeanWithinNSigma( timelist ):

        numWsig = 0
        sumWsig = 0
        sum2Wsig = 0
        total = 0
        total2 = 0

        num = len(timelist)
        if( not num ): return [0.0,0.0]
        for time in timelist:
                total = total + time
                total2 = total2 + time*time

        mean = total/num
        stdev = math.sqrt(total2 / num - mean * mean )
        sRange = min( maxRange, stdev*nSigma )
        for time in timelist:
                if( abs(time - mean ) < sRange):
                        sumWsig = sumWsig + time
                        sum2Wsig = sum2Wsig + time*time
                        numWsig = numWsig + 1

        if( not numWsig ): return [0.0,0.0]
        meanWsig = sumWsig / numWsig
        stdevWsig = math.sqrt( sum2Wsig / numWsig - meanWsig * meanWsig )
        errorWsig = stdevWsig / math.sqrt( numWsig )
        return [ meanWsig, errorWsig ]
# end of function
*/

float jwk_ana_event_lhcDump::getsum( std::vector<float> list )
{
	float sum(0.0);
        for( auto val : list ){ sum += val; }
	return sum;
}

float jwk_ana_event_lhcDump::getsum2( std::vector<float> list )
{
        float sum2(0.0);
        for( auto val : list ){ sum2 += val*val; }
        return sum2;
}

float jwk_ana_event_lhcDump::getmean( std::vector<float> list )
{
	int num = list.size();
	if (!num) return 0.0;
        float sum = getsum( list );
        return sum/num;
}

float jwk_ana_event_lhcDump::getstdev( std::vector<float> list )
{
        int num = list.size();
        if (!num) return 0.0;
	float mean = getmean( list );
	return std::sqrt( getsum2( list ) / num - mean*mean );;
}

float jwk_ana_event_lhcDump::getstdev( std::vector<float> list, float mean )
{
        int num = list.size();
        if (!num) return 0.0;
        return std::sqrt( getsum2( list ) / num - mean*mean );;
}

float jwk_ana_event_lhcDump::geterror( std::vector<float> list )
{
        int num = list.size();
        if (!num) return 0.0;
	float stdev = getstdev( list );	
	return stdev/std::sqrt(num);
}

float jwk_ana_event_lhcDump::geterror( int num, float stdev )
{
	return stdev/std::sqrt(num);
}

float jwk_ana_event_lhcDump::getmaxval( std::vector<float> list )
{
	float maxval(0.0);
	for( auto val : list ){ if( val > maxval ) maxval = val; }
	return maxval;
}

std::string jwk_ana_event_lhcDump::findAndReplaceAll(std::string data, std::string toSearch, std::string replaceStr)
{

	size_t pos = data.find(toSearch);
        while( pos != std::string::npos)
        {
                data.replace(pos, toSearch.size(), replaceStr);
                pos =data.find(toSearch, pos + toSearch.size());
        }
	return data;
}


std::string jwk_ana_event_lhcDump::timeToString(time_t t)
{
        char buf[256];
        struct tm lt;
        std::string space(" ");
        std::string colon(":");
        std::string underline("_");
        localtime_r(&t, &lt);
        strftime(buf, sizeof(buf),"%F %R:%S", &lt);
        buf[sizeof(buf)-1] = 0;
        std::string ret(buf);
        ret = findAndReplaceAll( ret, " ", "_" );
        ret = findAndReplaceAll( ret, ":", "_" );
	return ret;
}

void jwk_ana_event_lhcDump::initRoot()
{
        std::stringstream title;
        std::stringstream fname;
	time_t timer;
	std::time(&timer);

        title << "Dump of LHCinfo & Event data";
        fname << "dump_LHCInfo_Event_";
	fname << timeToString(timer);
        fname << ".";
        fname << "r";
        fname << "o";
        fname << "o";
        fname << "t";
        std::cout << "Building tree " << title.str() << " on file " << fname.str() << std::endl;

        tfile = new TFile(fname.str().c_str(), "RECREATE", title.str().c_str());
        tree = new TTree("lhcInfoTree", title.str().c_str());

        tree->Branch("run",              	&run,            	 "run/i");
        tree->Branch("lumi",              	&lumi,            	 "lumi/i");
        tree->Branch("event",              	&event,            	 "event/l");
        tree->Branch("bx",              	&bx,            	 "bx/i");
        tree->Branch("instlumi",                &instlumi,               "instlumi/f");
        tree->Branch("lumiSection",             &lumiSection,            "lumiSection/i");
        tree->Branch("fillNumber",              &fillNumber,             "fillNumber/i");

//        tree->Branch("pre_zero_len",            &pre_zero_len,           "pre_zero_len/i");
        tree->Branch("subtrain_position",       &subtrain_position,      "subtrain_position/i");
        tree->Branch("train_position",          &train_position,         "train_position/i");
        tree->Branch("subtrain_number",         &subtrain_number,        "subtrain_number/i");
        tree->Branch("train_number",       	&train_number,      	 "train_number/i");

        tree->Branch("ave_phase",                    &ave_phase,                   "ave_phase/f");
        tree->Branch("dif_phase",                    &dif_phase,                   "dif_phase/f");
        tree->Branch("vc2_phase",                    &vc2_phase,                   "vc2_phase/f");

//	tree->Branch("offset_bx_phc", &offset_bx_phc );

	tree->Branch("rh_EB_mtime",                    &rh_EB_mtime,                   "rh_EB_mtime/f");
	tree->Branch("rh_EB_s2time",                    &rh_EB_s2time,                   "rh_EB_s2time/f");
	tree->Branch("rh_EB_stime",                    &rh_EB_stime,                   "rh_EB_stime/f");
	tree->Branch("rh_EB_count",                    &rh_EB_count,                   "rh_EB_count/i");
	tree->Branch("rh1GeV_EB_mtime",                    &rh1GeV_EB_mtime,                   "rh1GeV_EB_mtime/f");
	tree->Branch("rh1GeV_EB_s2time",                    &rh1GeV_EB_s2time,                   "rh1GeV_EB_s2time/f");
	tree->Branch("rh1GeV_EB_stime",                    &rh1GeV_EB_stime,                   "rh1GeV_EB_stime/f");
	tree->Branch("rh1GeV_EB_count",                    &rh1GeV_EB_count,                   "rh1GeV_EB_count/i");
	tree->Branch("rh2GeV_EB_mtime",                    &rh2GeV_EB_mtime,                   "rh2GeV_EB_mtime/f");
	tree->Branch("rh2GeV_EB_s2time",                    &rh2GeV_EB_s2time,                   "rh2GeV_EB_s2time/f");
	tree->Branch("rh2GeV_EB_stime",                    &rh2GeV_EB_stime,                   "rh2GeV_EB_stime/f");
	tree->Branch("rh2GeV_EB_count",                    &rh2GeV_EB_count,                   "rh2GeV_EB_count/i");
	tree->Branch("rh5GeV_EB_mtime",                    &rh5GeV_EB_mtime,                   "rh5GeV_EB_mtime/f");
	tree->Branch("rh5GeV_EB_s2time",                    &rh5GeV_EB_s2time,                   "rh5GeV_EB_s2time/f");
	tree->Branch("rh5GeV_EB_stime",                    &rh5GeV_EB_stime,                   "rh5GeV_EB_stime/f");
	tree->Branch("rh5GeV_EB_count",                    &rh5GeV_EB_count,                   "rh5GeV_EB_count/i");
	tree->Branch("rh10GeV_EB_mtime",                    & rh10GeV_EB_mtime,                   "rh10GeV_EB_mtime/f");
	tree->Branch("rh10GeV_EB_s2time",                    &rh10GeV_EB_s2time,                   "rh10GeV_EB_s2time/f");
	tree->Branch("rh10GeV_EB_stime",                    &rh10GeV_EB_stime,                   "rh10GeV_EB_stime/f");
	tree->Branch("rh10GeV_EB_count",                    &rh10GeV_EB_count,                   "rh10GeV_EB_count/i");
	tree->Branch("rh20GeV_EB_mtime",                    &rh20GeV_EB_mtime,                   "rh20GeV_EB_mtime/f");
	tree->Branch("rh20GeV_EB_s2time",                    &rh20GeV_EB_s2time,                   "rh20GeV_EB_s2time/f");
	tree->Branch("rh20GeV_EB_stime",                    &rh20GeV_EB_stime,                   "rh20GeV_EB_stime/f");
	tree->Branch("rh20GeV_EB_count",                    &rh20GeV_EB_count,                   "rh20GeV_EB_count/i");

        tree->Branch("rh_EEM_mtime",                    &rh_EEM_mtime,                   "rh_EEM_mtime/f");
        tree->Branch("rh_EEM_s2time",                    &rh_EEM_s2time,                   "rh_EEM_s2time/f");
        tree->Branch("rh_EEM_stime",                    &rh_EEM_stime,                   "rh_EEM_stime/f");
        tree->Branch("rh_EEM_count",                    &rh_EEM_count,                   "rh_EEM_count/i");
        tree->Branch("rh1GeV_EEM_mtime",                    &rh1GeV_EEM_mtime,                   "rh1GeV_EEM_mtime/f");
        tree->Branch("rh1GeV_EEM_s2time",                    &rh1GeV_EEM_s2time,                   "rh1GeV_EEM_s2time/f");
        tree->Branch("rh1GeV_EEM_stime",                    &rh1GeV_EEM_stime,                   "rh1GeV_EEM_stime/f");
        tree->Branch("rh1GeV_EEM_count",                    &rh1GeV_EEM_count,                   "rh1GeV_EEM_count/i");
        tree->Branch("rh2GeV_EEM_mtime",                    &rh2GeV_EEM_mtime,                   "rh2GeV_EEM_mtime/f");
        tree->Branch("rh2GeV_EEM_s2time",                    &rh2GeV_EEM_s2time,                   "rh2GeV_EEM_s2time/f");
        tree->Branch("rh2GeV_EEM_stime",                    &rh2GeV_EEM_stime,                   "rh2GeV_EEM_stime/f");
        tree->Branch("rh2GeV_EEM_count",                    &rh2GeV_EEM_count,                   "rh2GeV_EEM_count/i");
        tree->Branch("rh5GeV_EEM_mtime",                    &rh5GeV_EEM_mtime,                   "rh5GeV_EEM_mtime/f");
        tree->Branch("rh5GeV_EEM_s2time",                    &rh5GeV_EEM_s2time,                   "rh5GeV_EEM_s2time/f");
        tree->Branch("rh5GeV_EEM_stime",                    &rh5GeV_EEM_stime,                   "rh5GeV_EEM_stime/f");
        tree->Branch("rh5GeV_EEM_count",                    &rh5GeV_EEM_count,                   "rh5GeV_EEM_count/i");
        tree->Branch("rh10GeV_EEM_mtime",                    & rh10GeV_EEM_mtime,                   "rh10GeV_EEM_mtime/f");
        tree->Branch("rh10GeV_EEM_s2time",                    &rh10GeV_EEM_s2time,                   "rh10GeV_EEM_s2time/f");
        tree->Branch("rh10GeV_EEM_stime",                    &rh10GeV_EEM_stime,                   "rh10GeV_EEM_stime/f");
        tree->Branch("rh10GeV_EEM_count",                    &rh10GeV_EEM_count,                   "rh10GeV_EEM_count/i");
        tree->Branch("rh20GeV_EEM_mtime",                    &rh20GeV_EEM_mtime,                   "rh20GeV_EEM_mtime/f");
        tree->Branch("rh20GeV_EEM_s2time",                    &rh20GeV_EEM_s2time,                   "rh20GeV_EEM_s2time/f");
        tree->Branch("rh20GeV_EEM_stime",                    &rh20GeV_EEM_stime,                   "rh20GeV_EEM_stime/f");
        tree->Branch("rh20GeV_EEM_count",                    &rh20GeV_EEM_count,                   "rh20GeV_EEM_count/i");

        tree->Branch("rh_EEP_mtime",                    &rh_EEP_mtime,                   "rh_EEP_mtime/f");
        tree->Branch("rh_EEP_s2time",                    &rh_EEP_s2time,                   "rh_EEP_s2time/f");
        tree->Branch("rh_EEP_stime",                    &rh_EEP_stime,                   "rh_EEP_stime/f");
        tree->Branch("rh_EEP_count",                    &rh_EEP_count,                   "rh_EEP_count/i");
        tree->Branch("rh1GeV_EEP_mtime",                    &rh1GeV_EEP_mtime,                   "rh1GeV_EEP_mtime/f");
        tree->Branch("rh1GeV_EEP_s2time",                    &rh1GeV_EEP_s2time,                   "rh1GeV_EEP_s2time/f");
        tree->Branch("rh1GeV_EEP_stime",                    &rh1GeV_EEP_stime,                   "rh1GeV_EEP_stime/f");
        tree->Branch("rh1GeV_EEP_count",                    &rh1GeV_EEP_count,                   "rh1GeV_EEP_count/i");
        tree->Branch("rh2GeV_EEP_mtime",                    &rh2GeV_EEP_mtime,                   "rh2GeV_EEP_mtime/f");
        tree->Branch("rh2GeV_EEP_s2time",                    &rh2GeV_EEP_s2time,                   "rh2GeV_EEP_s2time/f");
        tree->Branch("rh2GeV_EEP_stime",                    &rh2GeV_EEP_stime,                   "rh2GeV_EEP_stime/f");
        tree->Branch("rh2GeV_EEP_count",                    &rh2GeV_EEP_count,                   "rh2GeV_EEP_count/i");
        tree->Branch("rh5GeV_EEP_mtime",                    &rh5GeV_EEP_mtime,                   "rh5GeV_EEP_mtime/f");
        tree->Branch("rh5GeV_EEP_s2time",                    &rh5GeV_EEP_s2time,                   "rh5GeV_EEP_s2time/f");
        tree->Branch("rh5GeV_EEP_stime",                    &rh5GeV_EEP_stime,                   "rh5GeV_EEP_stime/f");
        tree->Branch("rh5GeV_EEP_count",                    &rh5GeV_EEP_count,                   "rh5GeV_EEP_count/i");
        tree->Branch("rh10GeV_EEP_mtime",                    & rh10GeV_EEP_mtime,                   "rh10GeV_EEP_mtime/f");
        tree->Branch("rh10GeV_EEP_s2time",                    &rh10GeV_EEP_s2time,                   "rh10GeV_EEP_s2time/f");
        tree->Branch("rh10GeV_EEP_stime",                    &rh10GeV_EEP_stime,                   "rh10GeV_EEP_stime/f");
        tree->Branch("rh10GeV_EEP_count",                    &rh10GeV_EEP_count,                   "rh10GeV_EEP_count/i");
        tree->Branch("rh20GeV_EEP_mtime",                    &rh20GeV_EEP_mtime,                   "rh20GeV_EEP_mtime/f");
        tree->Branch("rh20GeV_EEP_s2time",                    &rh20GeV_EEP_s2time,                   "rh20GeV_EEP_s2time/f");
        tree->Branch("rh20GeV_EEP_stime",                    &rh20GeV_EEP_stime,                   "rh20GeV_EEP_stime/f");
        tree->Branch("rh20GeV_EEP_count",                    &rh20GeV_EEP_count,                   "rh20GeV_EEP_count/i");

        h23_bPhase =            new TH1F("h23_bPhase","VC2_Phase Correction",1000,-50,50);
	h24_dPhase =		new TH1F("h24_dPhase","Diff_Phase Correction",1000,-50,50);
	h25_Phase =		new TH1F("h25_Phase","Phase Correction",1000,-50,50);
	h26_bxOcc = 		new TH1I("h26_bxOcc","BX Occupancy", 3564,0,3564 );
	h27_filledbx = 		new TH1F("h27_FilledBX","Filled BXs",13,-6.5,6.5);
        h28_rh_EB_Energy =      new TH1F("h28_rh_EB_Energy","RecHit Energy Distribution EB",50,0,50);
        h29_rh_EEP_Energy =      new TH1F("h29_rh_EEP_Energy","RecHit Energy Distribution EEP",50,0,50);
        h30_rh_EEM_Energy =      new TH1F("h30_rh_EEM_Energy","RecHit Energy Distribution EEM",50,0,50);

        std::cout << "Tree created" << std::endl;
}

void jwk_ana_event_lhcDump::closeRoot()
{

	 tfile->cd();

         h23_bPhase->Write();
	 h24_dPhase->Write();        
	 h25_Phase->Write();
         h26_bxOcc->Write();
	 h27_filledbx->Write(); 
         h28_rh_EB_Energy->Write();
         h29_rh_EEP_Energy->Write();
         h30_rh_EEM_Energy->Write();
	 tree->Write("", TObject::kOverwrite);
	 
//	 tfile->Write();
         tfile->Close();
}

void jwk_ana_event_lhcDump::dbtoRoot( const LHCInfo& lhcInfo, edm::Handle<EcalRecHitCollection> RecHitEBHandle, edm::Handle<EcalRecHitCollection> RecHitEEHandle )
{

        fillNumber = lhcInfo.fillNumber();
        instlumi = lhcInfo.instLumi();
	lumiSection = lhcInfo.lumiSection();

	std::vector<float> fill_e;
	std::vector<std::vector<float>> fill_be;
	std::vector<std::vector<std::vector<float>>> timemap;
	for( int be = 0; be < 3; be++ ){ timemap.push_back(fill_be);} // 0=EB, 1=EEP, 2=EEM
	for( int be = 0; be < 3; be++ ){ for( int e = 0; e < 6; e++ ){ timemap[be].push_back(fill_e); }} // 0 = all, 1 = 1GeV, 2 = 2GeV, 3 = 5GeV, 4 = 10Gev, 5 = 20GeV

        for(auto  recHit : *RecHitEBHandle) {

		timemap[0][0].push_back(recHit.time());
		if( recHit.energy() > 1 ){ timemap[0][1].push_back(recHit.time());}
                if( recHit.energy() > 2 ){ timemap[0][2].push_back(recHit.time());}
                if( recHit.energy() > 5 ){ timemap[0][3].push_back(recHit.time());}
                if( recHit.energy() > 10 ){ timemap[0][4].push_back(recHit.time());}
                if( recHit.energy() > 20 ){ timemap[0][5].push_back(recHit.time());}
		h28_rh_EB_Energy->Fill(recHit.energy());
//	        recHit_EB_times.push_back(rechit.time());
	
	}
        
        for(auto  recHit : *RecHitEEHandle) {

	        EEDetId id(recHit.detid());
                float iz = id.zside();			
		if( iz > 0 ){
                	timemap[1][0].push_back(recHit.time());
                	if( recHit.energy() > 1 ){ timemap[1][1].push_back(recHit.time());}
                	if( recHit.energy() > 2 ){ timemap[1][2].push_back(recHit.time());}
                	if( recHit.energy() > 5 ){ timemap[1][3].push_back(recHit.time());}
                	if( recHit.energy() > 10 ){ timemap[1][4].push_back(recHit.time());}
                	if( recHit.energy() > 20 ){ timemap[1][5].push_back(recHit.time());}
                	h29_rh_EEP_Energy->Fill(recHit.energy());
// 		       	recHit_EEP_times.push_back(rechit.time());
		}
		if( iz < 0 ){
                	timemap[2][0].push_back(recHit.time());
                        if( recHit.energy() > 1 ){ timemap[2][1].push_back(recHit.time());}
                        if( recHit.energy() > 2 ){ timemap[2][2].push_back(recHit.time());}
                        if( recHit.energy() > 5 ){ timemap[2][3].push_back(recHit.time());}
                        if( recHit.energy() > 10 ){ timemap[2][4].push_back(recHit.time());}
                        if( recHit.energy() > 20 ){ timemap[2][5].push_back(recHit.time());}
                	h30_rh_EEM_Energy->Fill(recHit.energy());
//                	recHit_EEM_times.push_back(rechit.time());
		}
        }


        // EB
        rh_EB_mtime = getmean( timemap[0][0] );
        rh_EB_s2time = getsum2( timemap[0][0] );
        rh_EB_stime = getsum( timemap[0][0] );
        rh_EB_count = timemap[0][0].size();
	//std::cout << "EB RH Count: " << rh_EB_count << std::endl;
        rh1GeV_EB_mtime = getmean( timemap[0][1] );
        rh1GeV_EB_s2time = getsum2( timemap[0][1] );
        rh1GeV_EB_stime = getsum( timemap[0][1] );
        rh1GeV_EB_count = timemap[0][1].size();
        rh2GeV_EB_mtime = getmean( timemap[0][2] );
        rh2GeV_EB_s2time = getsum2( timemap[0][2] );
        rh2GeV_EB_stime = getsum( timemap[0][2] );
        rh2GeV_EB_count = timemap[0][2].size();
        rh5GeV_EB_mtime = getmean( timemap[0][3] );
        rh5GeV_EB_s2time = getsum2( timemap[0][3] );
        rh5GeV_EB_stime = getsum( timemap[0][3] );
        rh5GeV_EB_count = timemap[0][3].size();
        rh10GeV_EB_mtime = getmean( timemap[0][4] );
        rh10GeV_EB_s2time = getsum2( timemap[0][4] );
        rh10GeV_EB_stime = getsum( timemap[0][4] );
        rh10GeV_EB_count = timemap[0][4].size();
        rh20GeV_EB_mtime = getmean( timemap[0][5] );
        rh20GeV_EB_s2time = getsum2( timemap[0][5] );
        rh20GeV_EB_stime = getsum( timemap[0][5] );
        rh20GeV_EB_count = timemap[0][5].size();

        // EEP
        rh_EEP_mtime = getmean( timemap[1][0] );
        rh_EEP_s2time = getsum2( timemap[1][0] );
        rh_EEP_stime = getsum( timemap[1][0] );
        rh_EEP_count = timemap[1][0].size();
        rh1GeV_EEP_mtime = getmean( timemap[1][1] );
        rh1GeV_EEP_s2time = getsum2( timemap[1][1] );
        rh1GeV_EEP_stime = getsum( timemap[1][1] );
        rh1GeV_EEP_count = timemap[1][1].size();
        rh2GeV_EEP_mtime = getmean( timemap[1][2] );
        rh2GeV_EEP_s2time = getsum2( timemap[1][2] );
        rh2GeV_EEP_stime = getsum( timemap[1][2] );
        rh2GeV_EEP_count = timemap[1][2].size();
        rh5GeV_EEP_mtime = getmean( timemap[1][3] );
        rh5GeV_EEP_s2time = getsum2( timemap[1][3] );
        rh5GeV_EEP_stime = getsum( timemap[1][3] );
        rh5GeV_EEP_count = timemap[1][3].size();
        rh10GeV_EEP_mtime = getmean( timemap[1][4] );
        rh10GeV_EEP_s2time = getsum2( timemap[1][4] );
        rh10GeV_EEP_stime = getsum( timemap[1][4] );
        rh10GeV_EEP_count = timemap[1][4].size();
        rh20GeV_EEP_mtime = getmean( timemap[1][5] );
        rh20GeV_EEP_s2time = getsum2( timemap[1][5] );
        rh20GeV_EEP_stime = getsum( timemap[1][5] );
        rh20GeV_EEP_count = timemap[1][5].size();

        // EEM
        rh_EEM_mtime = getmean( timemap[2][0] );
        rh_EEM_s2time = getsum2( timemap[2][0] );
        rh_EEM_stime = getsum( timemap[2][0] );
        rh_EEM_count = timemap[2][0].size();
        rh1GeV_EEM_mtime = getmean( timemap[2][1] );
        rh1GeV_EEM_s2time = getsum2( timemap[2][1] );
        rh1GeV_EEM_stime = getsum( timemap[2][1] );
        rh1GeV_EEM_count = timemap[2][1].size();
        rh2GeV_EEM_mtime = getmean( timemap[2][2] );
        rh2GeV_EEM_s2time = getsum2( timemap[2][2] );
        rh2GeV_EEM_stime = getsum( timemap[2][2] );
        rh2GeV_EEM_count = timemap[2][2].size();
        rh5GeV_EEM_mtime = getmean( timemap[2][3] );
        rh5GeV_EEM_s2time = getsum2( timemap[2][3] );
        rh5GeV_EEM_stime = getsum( timemap[2][3] );
        rh5GeV_EEM_count = timemap[2][3].size();
        rh10GeV_EEM_mtime = getmean( timemap[2][4] );
        rh10GeV_EEM_s2time = getsum2( timemap[2][4] );
        rh10GeV_EEM_stime = getsum( timemap[2][4] );
        rh10GeV_EEM_count = timemap[2][4].size();
        rh20GeV_EEM_mtime = getmean( timemap[2][5] );
        rh20GeV_EEM_s2time = getsum2( timemap[2][5] );
        rh20GeV_EEM_stime = getsum( timemap[2][5] );
        rh20GeV_EEM_count = timemap[2][5].size();

        bool first_zero( true );
        bool first_notzero( true );
        unsigned int zero(0);
        unsigned int notzero(0);
        unsigned int longnotzero(0);
        unsigned int count(0);
        unsigned int longcount(0);
//      float phcor(0.0);
        
        std::vector<float>  ave;
        std::vector<float>  dif;
        std::vector<float>  vc2;

        std::vector<unsigned int> train_zero;
        std::vector<unsigned int> train_notzero;
        std::vector<unsigned int> long_train_notzero;
        std::vector<unsigned int> subtrain_num;
        std::vector<unsigned int> train_num;

        train_zero.clear();
        train_notzero.clear();
        long_train_notzero.clear();
        subtrain_num.clear();
        train_num.clear();

        for( unsigned int i  =  0; i < lhcInfo.beam1VC().size(); i++ ){

                ave.push_back(((lhcInfo.beam1VC()[i]+lhcInfo.beam2VC()[i])/2.0)*(2.5/360.0));
                dif.push_back((lhcInfo.beam1VC()[i]-lhcInfo.beam2VC()[i])*(2.5/360.0));
                vc2.push_back((lhcInfo.beam2VC()[i])*(2.5/360.0));

                if( lhcInfo.beam1VC()[i] == 0.0 ) {
                        if( first_zero == true ) {
                                first_zero = false;
                                first_notzero = true;
                                zero = 0;
                        }
                        zero++;
                }
                else {
                        if( first_notzero == true ) {
                                first_notzero = false;
                                first_zero = true;
                                notzero = 0;
                                if( zero > 10 ){
                                        longnotzero = 0;
                                        longcount++;
                                }
                                count++;
                        }
                        notzero++;
                        longnotzero++;
                }
                train_zero.push_back(zero);
                train_notzero.push_back(notzero);
                long_train_notzero.push_back(longnotzero);
                subtrain_num.push_back(count);
                train_num.push_back(longcount);

//              std::cout << "BX: " << i << " Event: " << eventCount; 
//              std::cout << " Train #: " << count << " LongTrian #: " << longcount; 
//              std::cout << " Zero: " << zero << " !Zero: " << notzero << " Long!Zero: " << longnotzero << std::endl;
//      //      if(eventCount%10) h26_bxPhase->Fill(i,eventCount,phcor); 
        }

//        pre_zero_len = train_zero[bx];
        subtrain_position = train_notzero[bx];
        train_position = long_train_notzero[bx];
        subtrain_number = subtrain_num[bx];
        train_number = train_num[bx];

        ave_phase = ave[bx];
        dif_phase = dif[bx];
        vc2_phase = vc2[bx];

        h23_bPhase->Fill(vc2_phase*1000);
        h24_dPhase->Fill(dif_phase*1000);
        h25_Phase->Fill(ave_phase*1000);
        h26_bxOcc->Fill(bx);

	for( int i = 5; i > -5; i-- ){ 
//		offset_bx_phc.push_back( ave[bx+i] ); 
		if( ave[bx+i] != 0. ) h27_filledbx->Fill( i );
	}
 //       std::cout << "EB RH Count prefill: " << rh_EB_count << std::endl;
        tree->Fill();
 //       std::cout << "EB RH Count postfill: " << rh_EB_count << std::endl;
}

//EcalTimingEvents= cms.EDProducer("EcalTimingEventProducer",
//                    recHitEECollection = cms.InputTag("ecalRecHitEBSelector"),
//                    recHitEBCollection = cms.InputTag("ecalRecHitEESelector"),
//                    )

//	EcalTimingEvent(const EcalRecHit& rec) {
//           detid_ = rec.detid();
//           setTime(rec.time());
//           setEnergy(rec.energy());
//        }

	/// Time is stored in a int16_t in ps. time() returns a float in ns
//        float time() const{ return float(time_)/1000.0f; }
        /// Energy is stored in a uint16_t in 10's of MeV. energy() returns a float in GeV
//        float energy() const { return float(energy_)/100.0f; }


void
jwk_ana_event_lhcDump::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
   using namespace edm;

        run = iEvent.id().run();
        lumi = iEvent.luminosityBlock();
        event = iEvent.id().event();
        bx = iEvent.bunchCrossing() - 1;

        edm::ESHandle<LHCInfo> lhcInfoHnd;
        iSetup.get<LHCInfoRcd>().get(lhcInfoHnd);

	edm::Handle<EcalRecHitCollection> RecHitEBHandle;
	iEvent.getByToken(_ecalRecHitsEBtoken, RecHitEBHandle);
	edm::Handle<EcalRecHitCollection> RecHitEEHandle;
	iEvent.getByToken(_ecalRecHitsEEtoken, RecHitEEHandle);

//	Handle<EEDigiCollection> endcapDigis;
//	iEvent.getByLabel(_endcapDigiCollection, endcapDigis);
//	Handle<EBDigiCollection> barrelDigis;
//	iEvent.getByLabel(_barrelDigiCollection, barrelDigis);

//	edm::Handle<EcalTimingCollection> timingCollection;
//	iEvent.getByToken(_timingEvents, timingCollection);

//	for(auto  timeEvent : *timingCollection) {}
//	for(auto  recHit_itr : *RecHitEBHandle) {
//		timing_out->push_back(EcalTimingEvent(recHit_itr));
//	}
//	for(auto  recHit_itr : *RecHitEEHandle) {
//		timing_out->push_back(EcalTimingEvent(recHit_itr));
//	}

        if(!lhcInfoHnd.isValid()) {
                std::cout << "LHCInfo not found?\n";
        }else{
                const LHCInfo* lhcInfo = lhcInfoHnd.product();

		dbtoRoot( *lhcInfo, RecHitEBHandle, RecHitEEHandle );

        }//lhc


#ifdef THIS_IS_AN_EVENT_EXAMPLE
   Handle<ExampleData> pIn;
   iEvent.getByLabel("example",pIn);
#endif

#ifdef THIS_IS_AN_EVENTSETUP_EXAMPLE
   ESHandle<SetupData> pSetup;
   iSetup.get<SetupRecord>().get(pSetup);
#endif
}


// ------------ method called once each job just before starting event loop  ------------
void
jwk_ana_event_lhcDump::beginJob()
{
	initRoot();
}

// ------------ method called once each job just after ending the event loop  ------------
void
jwk_ana_event_lhcDump::endJob()
{




        closeRoot();
}

// ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
void
jwk_ana_event_lhcDump::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  //The following says we do not know what parameters are allowed so do no validation
  // Please change this to state exactly what you do use, even if it is no parameters
  edm::ParameterSetDescription desc;
  desc.setUnknown();
  descriptions.addDefault(desc);

  //Specify that only 'tracks' is allowed
  //To use, remove the default given above and uncomment below
  //ParameterSetDescription desc;
  //desc.addUntracked<edm::InputTag>("tracks","ctfWithMaterialTracks");
  //descriptions.addDefault(desc);
}

//define this as a plug-in
DEFINE_FWK_MODULE(jwk_ana_event_lhcDump);
