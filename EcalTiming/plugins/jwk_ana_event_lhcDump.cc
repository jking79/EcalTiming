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

                        static float getsum( vector<float> list );
			static float getsum2( vector<float> list );
			static float getmean( vector<float> list );
			static float getstdev( vector<float> list );
			static float getstdev( vector<float> list, float mean );
			static float geterror( vector<float> list );
                        static float geterror( int num, float stdev );
                        static float getmaxval( vector<float> list );

                        static std::string timeToString(time_t t);
        		static std::string findAndReplaceAll(std::string data, std::string toSearch, std::string replaceStr);

      // ----------member data ---------------------------

                        TTree *tree;
                        TFile *tfile;

                        unsigned int run;
                        unsigned int lumi;
                        unsigned long  event;
                        unsigned int bx;

                        unsigned int detid;
			unsigned int ix;
			unsigned int iy;
                        unsigned int iz;

                        float time;



                        float energy;
                        float instlumi;

			int pre_zero_len;
			int train_count;
                        int long_train_count;
			int train_number;
                        int long_train_number;

			unsigned long		eventCount;
        		unsigned int		fillNumber;

                        float                   bx0pc;
                        float                   bxm4pc;
                        float                   bxm3pc;
                        float                   bxm2pc;
                        float                   bxm1pc;
                        float                   bxp1pc;
                        float                   bxp2pc;
                        float                   bxp3pc;
                        float                   bxp4pc;
                        float                   bxp5pc;

        		unsigned int    	lumiSection;

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

float jwk_ana_lhcDump::getsum( vector<float> list )
{
	float sum(0.0);
        for( auto val : list ){ sum += val; }
	return sum;
}

float jwk_ana_lhcDump::getsum2( vector<float> list )
{
        float sum2(0.0);
        for( auto val : list ){ sum += val*val; }
        return sum2;
}

float jwk_ana_lhcDump::getmean( vector<float> list )
{
	int num = list.size();
	if (!num) return 0.0;
        float sum = getsum( list );
        return sum/num;
}

float jwk_ana_lhcDump::getstdev( vector<float> list )
{
        int num = list.size();
        if (!num) return 0.0;
	float mean = getmean( list );
	return std::sqrt( getsum2( list ) / num - mean*mean );;
}

float jwk_ana_lhcDump::getstdev( vector<float> list, float mean )
{
        int num = list.size();
        if (!num) return 0.0;
        return std::sqrt( getsum2( list ) / num - mean*mean );;
}

float jwk_ana_lhcDump::geterror( vector<float> list )
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

float jwk_ana_lhcDump::getmaxval( vector<float> list )
{
	float maxval(0.0);
	for( auto val : list ){ if( val > maxval ) maxval = val; }
	return maxval;
}

std::string jwk_ana_lhcDump::findAndReplaceAll(std::string data, std::string toSearch, std::string replaceStr)
{

	size_t pos = data.find(toSearch);
        while( pos != std::string::np s)
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

        tree->Branch("run",              	&run,            	"run/i");
        tree->Branch("lumi",              	&lumi,            	"lumi/i");
        tree->Branch("event",              	&event,            	"event/l");
        tree->Branch("bx",              	&bx,            	"bx/i");

        tree->Branch("detid",                   &detid,                  "detid/i");
	tree->Branch("ix", 			&ix, 			 "ix/I");
	tree->Branch("iy", 			&iy,			 "iy/I");
	tree->Branch("iz", 			&iz,			 "iz/I");

        tree->Branch("time",                    &time,                   "time/f");
        tree->Branch("energy",                  &energy,                 "energy/f");
        tree->Branch("instlumi",                &instlumi,               "instlumi/f");

        tree->Branch("pre_zero_len",            &pre_zero_len,           "pre_zero_len/i");
        tree->Branch("train_count",             &train_count,            "train_count/i");
        tree->Branch("long_train_count",        &long_train_count,       "long_train_count/i");
        tree->Branch("train_number",            &train_number,           "train_number/i");
        tree->Branch("long_train_number",       &long_train_number,      "long_train_number/i");

        tree->Branch("fillNumber",          	&fillNumber,          	"fillNumber/i");

        tree->Branch("ave_phase",                    &ave_phase,                   "ave_phase/f");
        tree->Branch("dif_phase",                    &dif_phase,                   "dif_phase/f");
        tree->Branch("vc2_phase",                    &vc2_phase,                   "vc2_phase/f");

        tree->Branch("bx0pc",               &bx0pc,              "bx0pc/F");
        tree->Branch("bxm4pc",              &bxm4pc,              "bxm4pc/F");
        tree->Branch("bxm3pc",              &bxm3pc,              "bxm3pc/F");
        tree->Branch("bxm4pc",              &bxm2pc,              "bxm2pc/F");
        tree->Branch("bxm1pc",              &bxm1pc,              "bxm1pc/F");
        tree->Branch("bxp1pc",              &bxp1pc,              "bxp5pc/F");
        tree->Branch("bxp2pc",              &bxp2pc,              "bxp4pc/F");
        tree->Branch("bxp3pc",              &bxp3pc,              "bxp3pc/F");
        tree->Branch("bxp4pc",              &bxp4pc,              "bxp2pc/F");
        tree->Branch("bxp5pc",              &bxp5pc,              "bxp1pc/F");

        tree->Branch("lumiSection",            	&lumiSection,           "lumiSection/i");

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

        int _EcalBarrel(1);

        fillNumber = obja.fillNumber();
        instlumi = obja.instLumi();

	for( ){
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

	}        

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

        for( unsigned int i  =  0; i < obja.beam1VC().size(); i++ ){
//        for( unsigned int i  = bx-4; i <= bx+5; i++ ){
                ave.push_back(((obja.beam1VC()[i]+obja.beam2VC()[i])/2.0)*(2.5/360.0));
                dif.push_back((obja.beam1VC()[i]-obja.beam2VC()[i])*(2.5/360.0));
                vc2.push_back((obja.beam2VC()[i])*(2.5/360.0));

                if( obja.beam1VC()[i] == 0.0 ) {
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

        pre_zero_len = train_zero[bx];
        train_count = train_notzero[bx];
        long_train_count = long_train_notzero[bx];
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


void jwk_ana_lhcDump::dbtoRoot(const LHCInfo & obja, const EcalRecHit& rechit )
{

	int _EcalBarrel(1);

        fillNumber = obja.fillNumber();
	instlumi = obja.instLumi();

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

	for( unsigned int i  =  0; i < obja.beam1VC().size(); i++ ){
//        for( unsigned int i  = bx-4; i <= bx+5; i++ ){
		ave.push_back(((obja.beam1VC()[i]+obja.beam2VC()[i])/2.0)*(2.5/360.0));
		dif.push_back((obja.beam1VC()[i]-obja.beam2VC()[i])*(2.5/360.0));
		vc2.push_back((obja.beam2VC()[i])*(2.5/360.0));	
	
		if( obja.beam1VC()[i] == 0.0 ) {
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
        train_count = train_notzero[bx];
	long_train_count = long_train_notzero[bx];
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
