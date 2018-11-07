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

//class jwk_ana_lhcDump : public edm::one::EDAnalyzer<edm::one::SharedResources>  {
class jwk_ana_lhcDump : public edm::one::EDAnalyzer<>  {
   public:
      explicit jwk_ana_lhcDump(const edm::ParameterSet&);
      ~jwk_ana_lhcDump();

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
                	void dbtoRoot( const LHCInfo& lhcInfo, edm::Handle<EcalRecHitCollection>& RecHitEBHandle, edm::Handle<EcalRecHitCollection>& RecHitEEHandle );
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
                        unsigned long eventCount;
                        unsigned int fillNumber;
                        float energy;
                        float instlumi;
                        float time;

			// EB
			float rh_EB_mtime;
		        float rh_EB_s2time;
			float rh_EB_stime;
			int rh_EB_count;
                        float rh1GeV_EB_mtime;
                        float rh1GeV__EB_s2time;
                        float rh1GeV__EB_stime;
                        int rh1GeV__EB_count;
                        float rh2GeV_EB_mtime;
                        float rh2GeV__EB_s2time;
                        float rh2GeV__EB_stime;
                        int rh2GeV__EB_count;
                        float rh5GeV_EB_mtime;
                        float rh5GeV__EB_s2time;
                        float rh5GeV__EB_stime;
                        int rh5GeV__EB_count;
                        float rh10GeV_EB_mtime;
                        float rh10GeV__EB_s2time;
                        float rh10GeV__EB_stime;
                        int rh10GeV__EB_count;
                        float rh20GeV_EB_mtime;
                        float rh20GeV__EB_s2time;
                        float rh20GeV__EB_stime;
                        int rh20GeV__EB_count;

                        // EEP
                        float rh_EEP_mtime;
                        float rh_EEP_s2time;
                        float rh_EEP_stime;
                        int rh_EEP_count;
                        float rh1GeV_EEP_mtime;
                        float rh1GeV__EEP_s2time;
                        float rh1GeV__EEP_stime;
                        int rh1GeV__EEP_count;
                        float rh2GeV_EEP_mtime;
                        float rh2GeV__EEP_s2time;
                        float rh2GeV__EEP_stime;
                        int rh2GeV__EEP_count;
                        float rh5GeV_EEP_mtime;
                        float rh5GeV__EEP_s2time;
                        float rh5GeV__EEP_stime;
                        int rh5GeV__EEP_count;
                        float rh10GeV_EEP_mtime;
                        float rh10GeV__EEP_s2time;
                        float rh10GeV__EEP_stime;
                        int rh10GeV__EEP_count;
                        float rh20GeV_EEP_mtime;
                        float rh20GeV__EEP_s2time;
                        float rh20GeV__EEP_stime;
                        int rh20GeV__EEP_count;

                        // EEM
                        float rh_EEM_mtime;
                        float rh_EEM_s2time;
                        float rh_EEM_stime;
                        int rh_EEM_count;
                        float rh1GeV_EEM_mtime;
                        float rh1GeV__EEM_s2time;
                        float rh1GeV__EEM_stime;
                        int rh1GeV__EEM_count;
                        float rh2GeV_EEM_mtime;
                        float rh2GeV__EEM_s2time;
                        float rh2GeV__EEM_stime;
                        int rh2GeV__EEM_count;
                        float rh5GeV_EEM_mtime;
                        float rh5GeV__EEM_s2time;
                        float rh5GeV__EEM_stime;
                        int rh5GeV__EEM_count;
                        float rh10GeV_EEM_mtime;
                        float rh10GeV__EEM_s2time;
                        float rh10GeV__EEM_stime;
                        int rh10GeV__EEM_count;
                        float rh20GeV_EEM_mtime;
                        float rh20GeV__EEM_s2time;
                        float rh20GeV__EEM_stime;
                        int rh20GeV__EEM_count;

			int subtrain_position;
                        int train_position;

                        std::vector<float>      offset_bx_phc;
		
			float			ave_phase;
			float			dif_phase;
			float			vc2_phase;

			TH1F *h23_bPhase;
                        TH1F *h24_dPhase;
			TH1F *h25_Phase;
			
			TH1I *h26_bxOcc;
			TH1F *h27_filledbx;

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
jwk_ana_lhcDump::jwk_ana_lhcDump(const edm::ParameterSet& iConfig):
//	_timingEvents(consumes<EcalTimingCollection>(iConfig.getParameter<edm::InputTag>("timingCollection")))
//	_barrelDigiCollection (iConfig.getParameter<edm::InputTag> ("EBdigiCollection")),
//	_endcapDigiCollection (iConfig.getParameter<edm::InputTag> ("EEdigiCollection")),
	_ecalRecHitsEBtoken(consumes<EBRecHitCollection>(iConfig.getParameter<edm::InputTag>("recHitEBCollection"))),
	_ecalRecHitsEEtoken(consumes<EERecHitCollection>(iConfig.getParameter<edm::InputTag>("recHitEECollection")))
{
   //now do what ever initialization is needed

}


jwk_ana_lhcDump::~jwk_ana_lhcDump()
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

float jwk_ana_lhcDump::getsum( std::vector<float> list )
{
	float sum(0.0);
        for( auto val : list ){ sum += val; }
	return sum;
}

float jwk_ana_lhcDump::getsum2( std::vector<float> list )
{
        float sum2(0.0);
        for( auto val : list ){ sum2 += val*val; }
        return sum2;
}

float jwk_ana_lhcDump::getmean( std::vector<float> list )
{
	int num = list.size();
	if (!num) return 0.0;
        float sum = getsum( list );
        return sum/num;
}

float jwk_ana_lhcDump::getstdev( std::vector<float> list )
{
        int num = list.size();
        if (!num) return 0.0;
	float mean = getmean( list );
	return std::sqrt( getsum2( list ) / num - mean*mean );;
}

float jwk_ana_lhcDump::getstdev( std::vector<float> list, float mean )
{
        int num = list.size();
        if (!num) return 0.0;
        return std::sqrt( getsum2( list ) / num - mean*mean );;
}

float jwk_ana_lhcDump::geterror( std::vector<float> list )
{
        int num = list.size();
        if (!num) return 0.0;
	float stdev = getstdev( list );	
	return stdev/std::sqrt(num);
}

float jwk_ana_lhcDump::geterror( int num, float stdev )
{
	return stdev/std::sqrt(num);
}

float jwk_ana_lhcDump::getmaxval( std::vector<float> list )
{
	float maxval(0.0);
	for( auto val : list ){ if( val > maxval ) maxval = val; }
	return maxval;
}

std::string jwk_ana_lhcDump::findAndReplaceAll(std::string data, std::string toSearch, std::string replaceStr)
{

	size_t pos = data.find(toSearch);
        while( pos != std::string::npos)
        {
                data.replace(pos, toSearch.size(), replaceStr);
                pos =data.find(toSearch, pos + toSearch.size());
        }
	return data;
}


std::string jwk_ana_lhcDump::timeToString(time_t t)
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

void jwk_ana_lhcDump::initRoot()
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
        tree->Branch("time",                    &time,                   "time/f");
        tree->Branch("energy",                  &energy,                 "energy/f");
        tree->Branch("instlumi",                &instlumi,               "instlumi/f");
        tree->Branch("lumiSection",             &lumiSection,            "lumiSection/i");
        tree->Branch("fillNumber",              &fillNumber,             "fillNumber/i");

//        tree->Branch("pre_zero_len",            &pre_zero_len,           "pre_zero_len/i");
        tree->Branch("subtrain_position",       &subtrain_position,      "subtrain_position/i");
        tree->Branch("train_position",          &train_position,         "train_position/i");
//        tree->Branch("train_number",            &train_number,           "train_number/i");
//        tree->Branch("long_train_number",       &long_train_number,      "long_train_number/i");

        tree->Branch("ave_phase",                    &ave_phase,                   "ave_phase/f");
        tree->Branch("dif_phase",                    &dif_phase,                   "dif_phase/f");
        tree->Branch("vc2_phase",                    &vc2_phase,                   "vc2_phase/f");

	tree->Branch("offset_bx_phc", &offset_bx_phc );

        tree->Branch("rh_EB_mtime",                    &rh_EB_mtime,                   "rh_EB_mtime/f");
        tree->Branch("rh_EB_count",                    &rh_EB_count,                   "rh_EB_count/f");

	tree->Branch("rh_EB_mtime",                    &rh_EB_mtime,                   "rh_EB_mtime/f");
	tree->Branch("rh_EB_s2time",                    &rh_EB_s2time,                   "rh_EB_s2time/f");
	tree->Branch("rh_EB_stime",                    &rh_EB_stime,                   "rh_EB_stime/f");
	tree->Branch("rh_EB_count",                    &rh_EB_count,                   "rh_EB_count/i");
	tree->Branch("rh1GeV_EB_mtime",                    &rh1GeV_EB_mtime,                   "rh1GeV_EB_mtime/f");
	tree->Branch("rh1GeV__EB_s2time",                    &rh1GeV__EB_s2time,                   "rh1GeV__EB_s2time/f");
	tree->Branch("rh1GeV__EB_stime",                    &rh1GeV__EB_stime,                   "rh1GeV__EB_stime/f");
	tree->Branch("rh1GeV__EB_count",                    &rh1GeV__EB_count,                   "rh1GeV__EB_count/i");
	tree->Branch("rh2GeV_EB_mtime",                    &rh2GeV_EB_mtime,                   "rh2GeV_EB_mtime/f");
	tree->Branch("rh2GeV__EB_s2time",                    &rh2GeV__EB_s2time,                   "rh2GeV__EB_s2time/f");
	tree->Branch("rh2GeV__EB_stime",                    &rh2GeV__EB_stime,                   "rh2GeV__EB_stime/f");
	tree->Branch("rh2GeV__EB_count",                    &rh2GeV__EB_count,                   "rh2GeV__EB_count/i");
	tree->Branch("rh5GeV_EB_mtime",                    &rh5GeV_EB_mtime,                   "rh5GeV_EB_mtime/f");
	tree->Branch("rh5GeV__EB_s2time",                    &rh5GeV__EB_s2time,                   "rh5GeV__EB_s2time/f");
	tree->Branch("rh5GeV__EB_stime",                    &rh5GeV__EB_stime,                   "rh5GeV__EB_stime/f");
	tree->Branch("rh5GeV__EB_count",                    &rh5GeV__EB_count,                   "rh5GeV__EB_count/i");
	tree->Branch("rh10GeV_EB_mtime",                    & rh10GeV_EB_mtime,                   "rh10GeV_EB_mtime/f");
	tree->Branch("rh10GeV__EB_s2time",                    &rh10GeV__EB_s2time,                   "rh10GeV__EB_s2time/f");
	tree->Branch("rh10GeV__EB_stime",                    &rh10GeV__EB_stime,                   "rh10GeV__EB_stime/f");
	tree->Branch("rh10GeV__EB_count",                    &rh10GeV__EB_count,                   "rh10GeV__EB_count/i");
	tree->Branch("rh20GeV_EB_mtime",                    &rh20GeV_EB_mtime,                   "rh20GeV_EB_mtime/f");
	tree->Branch("rh20GeV__EB_s2time",                    &rh20GeV__EB_s2time,                   "rh20GeV__EB_s2time/f");
	tree->Branch("rh20GeV__EB_stime",                    &rh20GeV__EB_stime,                   "rh20GeV__EB_stime/f");
	tree->Branch("rh20GeV__EB_count",                    &rh20GeV__EB_count,                   "rh20GeV__EB_count/i");

        tree->Branch("rh_EEP_mtime",                    &rh_EEP_mtime,                   "rh_EEP_mtime/f");
        tree->Branch("rh_EEP_count",                    &rh_EEP_count,                   "rh_EEP_count/f");

        tree->Branch("rh_EEM_mtime",                    &rh_EEM_mtime,                   "rh_EEM_mtime/f");
        tree->Branch("rh_EEM_count",                    &rh_EEM_count,                   "rh_EEM_count/f");

        h23_bPhase =            new TH1F("h23_bPhase","VC2_Phase Correction",1000,-50,50);
	h24_dPhase =		new TH1F("h24_dPhase","Diff_Phase Correction",1000,-50,50);
	h25_Phase =		new TH1F("h25_Phase","Phase Correction",1000,-50,50);

	h26_bxOcc = 		new TH1I("h26_bxOcc","BX Occupancy", 3564,0,3564 );
	h27_filledbx = 		new TH1F("h27_FilledBX","Filled BXs",13,-6.5,6.5);

        std::cout << "Tree created" << std::endl;
}

void jwk_ana_lhcDump::closeRoot()
{

	 tfile->cd();

         h23_bPhase->Write();
	 h24_dPhase->Write();        
	 h25_Phase->Write();
         h26_bxOcc->Write();
	 h27_filledbx->Write(); 
	 tree->Write("", TObject::kOverwrite);
	 
	 tfile->Write();
         tfile->Close();
}

void jwk_ana_lhcDump::dbtoRoot( const LHCInfo& lhcInfo, edm::Handle<EcalRecHitCollection>& RecHitEBHandle, edm::Handle<EcalRecHitCollection>& RecHitEEHandle )
{

        fillNumber = lhcInfo.fillNumber();
        instlumi = lhcInfo.instLumi();

	std::vector<float> rechit_EB_times;
        std::vector<float> rechit1_EB_times;
        std::vector<float> rechit2_EB_times;
        std::vector<float> rechit5_EB_times;
        std::vector<float> rechit10_EB_times;
        std::vector<float> rechit20_EB_times;

        std::vector<float> rechit_EEP_times;
        std::vector<float> rechit1_EEP_times;
        std::vector<float> rechit2_EEP_times;
        std::vector<float> rechit5_EEP_times;
        std::vector<float> rechit10_EEP_times;
        std::vector<float> rechit20_EEP_times;

        std::vector<float> rechit_EEM_times;
        std::vector<float> rechit1_EEM_times;
        std::vector<float> rechit2_EEM_times;
        std::vector<float> rechit5_EEM_times;
        std::vector<float> rechit10_EEM_times;
        std::vector<float> rechit20_EEM_times;

        for(auto  recHit : *RecHitEBHandle) {

	        rechit_EB_times.push_back(rechit.time());
	
	}
        
        for(auto  recHit : *RecHitEEHandle) {

	        EEDetId id(rechit.detid());
                iz = id.zside();			
		if( iz > 0 ){
                	rechit_EEP_times.push_back(rechit.time());
		}
		if( iz < 0 ){
                	rechit_EEM_times.push_back(rechit.time());
		}
        }

	//EB
        rh_EB_mtime = getmean( rechit_EB_times );
        rh_EB_count = rechit_EB_times.size();
        rh_EB_menergy = getmean( rechit_EB_energies );

        // EEP
        rh_EEP_mtime = getmean( rechit_EEP_times );
	rh_EEP_count = rechit_EEP_times.size();
        rh_EEP_menergy = getmean( rechit_EEP_energies );

        // EEM
        rh_EEM_mtime = getmean( rechit_EEM_times );
        rh_EEM_count = rechit_EEM_times.size();
        rh_EEM_menergy = getmean( rechit_EEM_energies );

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
        std::vector<unsigned int> train;
        std::vector<unsigned int> long_train;

        train_zero.clear();
        train_notzero.clear();
        long_train_notzero.clear();
        train.clear();
        long_train.clear();

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
                train.push_back(count);
                long_train.push_back(longcount);

//              std::cout << "BX: " << i << " Event: " << eventCount; 
//              std::cout << " Train #: " << count << " LongTrian #: " << longcount; 
//              std::cout << " Zero: " << zero << " !Zero: " << notzero << " Long!Zero: " << longnotzero << std::endl;
//      //      if(eventCount%10) h26_bxPhase->Fill(i,eventCount,phcor); 
        }

//        pre_zero_len = train_zero[bx];
        subtrain_position = train_notzero[bx];
        train_position = long_train_notzero[bx];
//        train_number = train[bx];
//        long_train_number = long_train[bx];

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

        tree->Fill();
}

/*
void jwk_ana_lhcDump::dbtoRoot(const LHCInfo & lhcInfo, const EcalRecHit& rechit )
{

	int _EcalBarrel(1);

        fillNumber = lhcInfo.fillNumber();
	instlumi = lhcInfo.instLumi();

	detid = rechit.detid().rawId();
	if( rechit.detid().subdetId() == _EcalBarrel ) {
        	EBDetId id(rechit.detid());
		ix = id.ieta();
		iy = id.iphi();
		iz = 0;
        //dumpTimingEventToTree(timingEventsTree, tEvent, id.rawId(), id.ieta(), id.iphi(), 0, elecID, iRing, run, lumi, event, bx);
        } else {
        	EEDetId id(rechit.detid());
                ix = id.ix();
                iy = id.iy();
                iz = id.zside();
        //dumpTimingEventToTree(timingEventsTree, tEvent, id.rawId(), id.ix(), id.iy(), id.zside(), elecID, iRing, run, lumi, event, bx);
        }

	time = rechit.time();
	energy = rechit.energy();

	bool first_zero( true );
	bool first_notzero( true );
	unsigned int zero(0);
        unsigned int notzero(0);
	unsigned int longnotzero(0);
        unsigned int count(0);
	unsigned int longcount(0);
//	float phcor(0.0);

        std::vector<float>  ave;
        std::vector<float>  dif;
        std::vector<float>  vc2;

	ave.clear();
	dif.clear();
        vc2.clear();

	std::vector<unsigned int> train_zero;
	std::vector<unsigned int> train_notzero;
        std::vector<unsigned int> long_train_notzero;
        std::vector<unsigned int> train;
        std::vector<unsigned int> long_train;

	train_zero.clear();
	train_notzero.clear();
	long_train_notzero.clear();
        train.clear();
        long_train.clear();

	for( unsigned int i  =  0; i < lhcInfo.beam1VC().size(); i++ ){
//        for( unsigned int i  = bx-4; i <= bx+5; i++ ){
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
		train.push_back(count);
		long_train.push_back(longcount);

//		std::cout << "BX: " << i << " Event: " << eventCount; 
//		std::cout << " Train #: " << count << " LongTrian #: " << longcount; 
//		std::cout << " Zero: " << zero << " !Zero: " << notzero << " Long!Zero: " << longnotzero << std::endl;
//	//	if(eventCount%10) h26_bxPhase->Fill(i,eventCount,phcor); 
	}

        pre_zero_len = train_zero[bx];
        subtrain_position = train_notzero[bx];
	train_position = long_train_notzero[bx];
	train_number = train[bx];
	long_train_number = long_train[bx];

	ave_phase = ave[bx];
        dif_phase = dif[bx];
        vc2_phase = vc2[bx];

	h23_bPhase->Fill(vc2_phase*1000);
	h24_dPhase->Fill(dif_phase*1000);
        h25_Phase->Fill(ave_phase*1000);
	h26_bxOcc->Fill(bx);	

	bxp5pc = ave[bx+5];
        bxp4pc = ave[bx+4];
        bxp3pc = ave[bx+3];
        bxp2pc = ave[bx+2];
        bxp1pc = ave[bx+1];
        bx0pc = ave[bx];
        bxm1pc = ave[bx-1];
        bxm2pc = ave[bx-2];
        bxm3pc = ave[bx-3];
        bxm4pc = ave[bx-4];

	if( bxp5pc != 0. ) h27_filledbx->Fill( 5 );
        if( bxp4pc != 0. ) h27_filledbx->Fill( 4 );
        if( bxp3pc != 0. ) h27_filledbx->Fill( 3 );
        if( bxp2pc != 0. ) h27_filledbx->Fill( 2 );
        if( bxp1pc != 0. ) h27_filledbx->Fill( 1 );
        if( bx0pc != 0. ) h27_filledbx->Fill( 0 );
        if( bxm1pc != 0. ) h27_filledbx->Fill( -1 );
        if( bxm2pc != 0. ) h27_filledbx->Fill( -2 );
        if( bxm3pc != 0. ) h27_filledbx->Fill( -3 );
        if( bxm4pc != 0. ) h27_filledbx->Fill( -4 );

        tree->Fill();
}
*/

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
jwk_ana_lhcDump::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
   using namespace edm;

        run = iEvent.id().run();
        lumi = iEvent.luminosityBlock();
        event = iEvent.id().event();
        bx = iEvent.bunchCrossing() - 1;

	eventCount++;	

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
/*
                std::cout << "LHCInfo Data: \n";
                std::cout << "Create Time: " << timeToString( (time_t)(lhcInfo->createTime()) ) << std::endl;
                std::cout << "beginTime : " << timeToString( lhcInfo->beginTime() )  << std::endl;
                std::cout << "fillNumber : " << lhcInfo->fillNumber() << std::endl;
                std::cout << "fillType : " << lhcInfo->fillType() << std::endl;
                std::cout << "lumiSection : " << lhcInfo->lumiSection() << std::endl;
*/

		dbtoRoot( *lhcInfo, *RecHitEBHandle, *RecHitEEHandle );
//        	for(auto  recHit_itr : *RecHitEBHandle) { dbtoRoot( *lhcInfo, recHit_itr );}
//        	for(auto  recHit_itr : *RecHitEEHandle) { dbtoRoot( *lhcInfo, recHit_itr );}
//		dump_txt( *pa, (time_t)i.since, (time_t)i.till, outfile );

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
jwk_ana_lhcDump::beginJob()
{
	eventCount = 0;
	initRoot();
}

// ------------ method called once each job just after ending the event loop  ------------
void
jwk_ana_lhcDump::endJob()
{




        closeRoot();
}

// ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
void
jwk_ana_lhcDump::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
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
DEFINE_FWK_MODULE(jwk_ana_lhcDump);
