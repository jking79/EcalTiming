// -*- C++ -*-
//
// Package:    anatemplet/jwk_ana_lhcDump
// Class:      jwk_ana_lhcDump
//
/**\class Analyzer jwk_ana_lhcDump.cc EcalTiming/EcalTiming/plugins/jwk_ana_lhcDump.cc

 Description: [one line class summary]

 Implementation:
     [Notes on implementation]
*/
//
// Original Author:  Jack King
//         Created:  Fri, 14 Sep 2018 19:30:49 GMT
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

class jwk_ana_lhcDump : public edm::one::EDAnalyzer<edm::one::SharedResources>  {
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

	edm::EDGetTokenT<EcalTimingCollection> _timingEvents; ///< input collection

			typedef int FillTypeId;
			typedef int ParticleTypeId;

			unsigned int run;
			unsigned int lumi;
			unsigned long  event;
			unsigned int bx;

                        void initRoot();
                        void dbToRoot( const LHCInfo & obja );
                        void closeRoot();
        //                void dump_txt( const L & obja, time_t tb, time_t tei, std::ostream& file_out );
        //                void dump_phase( const L & obja, time_t tb, time_t tei, std::ostream& file_out );
                        static std::string timeToString(time_t t);
        		void findAndReplaceAll(std::string & data, std::string toSearch, std::string replaceStr);

      // ----------member data ---------------------------

                        TTree *tree;
                        TFile *tfile;


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

//        		unsigned short  	bunchesInBeam1;
//        		unsigned short  	bunchesInBeam2;
//        		unsigned short  	collidingBunches;
//        		unsigned short  	targetBunches;
	
//        		FillTypeId      	fillType;
//        		ParticleTypeId  	particleTypeForBeam1;
//        		ParticleTypeId  	particleTypeForBeam2;

//        		float           	crossingAngle;
//        		float           	betaStar;
//        		float        		intensityForBeam1;
//        		float       		intensityForBeam2;

//        		float           	energy;
//        		float           	delivLumi;
//        		float           	recLumi;
//			float			instLumi;
//			float			instLumiError;

//        		cond::Time_t    	createTime;
//        		cond::Time_t    	beginTime;
//        		cond::Time_t    	endTime;
//        		cond::Time_t    	deltaTime;
//			cond::Time_t            binTime;
//        		std::string     	injectionScheme;
//        		std::vector<float>      lumiPerBX;
//			unsigned int		Size_lumiPerBX;
//        		std::string     	lhcComment;
//        		std::string     	lhcState;
//        		std::string     	ctppsStatus;
        		unsigned int    	lumiSection;

        		std::vector<float>      beam1VC;
                        unsigned int            Size_beam1VC;
        		std::vector<float>      beam2VC;
                        unsigned int            Size_beam2VC;
//        		std::vector<float>      beam1RF;
//                      unsigned int            Size_beam1RF;
//        		std::vector<float>      beam2RF;
//                      unsigned int            Size_beam2RF;

//			bool			is25nsBunchSpacing;

//        		std::vector<int>         bunchConfigurationForBeam1;
//                       unsigned int            Size_bunchConfigurationForBeam1;
//        		std::vector<int>         bunchConfigurationForBeam2;
//                        unsigned int            Size_bunchConfigurationForBeam2;

			std::vector<float>	phase;

//			std::vector<std::string> errorList;
//			  TH1F *h1_binWidth;
//                        TH1F *h2_binTime;
//                        TH1F *h3_fillNumber;
//                       TH1F *h4_numBunchesB1;
//                        TH1F *h5_numBunchesB2;
//                        TH1F *h6_collidingBunches;
//                        TH1F *h7_targetBunches;
//                        TH1F *h8_fillType;
//                        TH1F *h9_particleTypeB1;
//                        TH1F *h10_particleTypeB2;
//                        TH1F *h11_crossingAngle;
//                        TH1F *h12_betaStar;
//                        TH1F *h13_intensityB1;
//                        TH1F *h14_intensityB2;
//                        TH1F *h15_energy;
//                        TH1F *h16_delivLumi;
//                        TH1F *h17_recLumi;
//                        TH1F *h18_SizeLumiPerBX;
//	           	  TH2F *h18b_SizeVsLumiPerBX;
//                        TH1F *h19_SizeRF1;
//                        TH1F *h20_SizeRF2;
//                        TH1F *h21_SizeVC1;
//                        TH1F *h22_SizeVC2;
//                        TH1F *h23_SizeBC1;
//                        TH1F *h24_SizeBC2;
			TH1F *h25_Phase;
			TH1I *h26_bxOcc;
			TH1F *h27_filledbx;

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
jwk_ana_lhcDump::jwk_ana_lhcDump(const edm::ParameterSet& iConfig) :
	_timingEvents(consumes<EcalTimingCollection>(iConfig.getParameter<edm::InputTag>("timingCollection")))
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

void jwk_ana_lhcDump::findAndReplaceAll(std::string & data, std::string toSearch, std::string replaceStr)
{

        size_t pos = data.find(toSearch);
        while( pos != std::string::npos)
        {
                data.replace(pos, toSearch.size(), replaceStr);
                pos =data.find(toSearch, pos + toSearch.size());
        }
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
//        findAndReplaceAll( ret, " ", "_" );
        size_t pos = ret.find(space);
        while( pos != std::string::npos)
        {
                ret.replace(pos, space.size(), underline);
                pos = ret.find(space, pos + space.size());
        }
//        findAndReplaceAll( ret, ":", "_" );
        pos = ret.find(colon);
        while( pos != std::string::npos)
        {
                ret.replace(pos, colon.size(), underline);
                pos = ret.find(colon, pos + colon.size());
        }
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
        std::cout << "Building tree " << title.str() << " on file " << fname.str()
                << std::endl;

        tfile = new TFile(fname.str().c_str(), "RECREATE", title.str().c_str());
        tree = new TTree("lhcInfoTree", title.str().c_str());

        tree->Branch("run",              	&run,            	"run/i");
        tree->Branch("lumi",              	&lumi,            	"lumi/i");
        tree->Branch("event",              	&event,            	"event/l");
        tree->Branch("bx",              	&bx,            	"bx/i");

        tree->Branch("fillNumber",          	&fillNumber,          	"fillNumber/i");


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



//        tree->Branch("bunchesInBeam1",          &bunchesInBeam1,        "bunchesInBeam1/s");
//        tree->Branch("bunchesInBeam2",   	&bunchesInBeam2,        "bunchesInBeam2/s");
//        tree->Branch("collidingBunches",        &collidingBunches,      "collidingBunches/s");
//        tree->Branch("targetBunches",          	&targetBunches,         "targetBunches/s");

//        tree->Branch("fillType",         	&fillType,         	"fillType/s");
//        tree->Branch("particleTypeForBeam1",    &particleTypeForBeam1,  "particleTypeForBeam1/s");
//        tree->Branch("particleTypeForBeam2",    &particleTypeForBeam2,  "particleTypeForBeam2/s");
//        tree->Branch("crossingAngle",           &crossingAngle,         "crossingAngle/F");
//        tree->Branch("betaStar",          	&betaStar,          	"betaStar/F");
//        tree->Branch("intensityForBeam1",       &intensityForBeam1,     "intensityForBeam1/F");
//        tree->Branch("intensityForBeam2",       &intensityForBeam2,     "intensityForBeam2/F");
//
//        tree->Branch("energy",            	&energy,            	"energy/F");
//        tree->Branch("delivLumi",          	&delivLumi,          	"delivLumi/F");
//        tree->Branch("recLumi",            	&recLumi,            	"recLumi/F");
//        tree->Branch("instLumi",		&instLumi,		"instLumi/F");
//        tree->Branch("instLumiError",		&instLumiError,		"instLumiError/F");
//
//
//        tree->Branch("createTime",            	&createTime,           	"createTime/i");
//        tree->Branch("beginTime",            	&beginTime,            	"beginTime/i");
//        tree->Branch("endTime",          	&endTime,          	"endTime/i");
//        tree->Branch("deltaTime",         	&deltaTime,         	"deltaTime/i");
//
//        tree->Branch("injectionScheme",         &injectionScheme,       "injectionScheme/C");
//        tree->Branch("lumiPerBX",            	"std::vector<float>",   &lumiPerBX);
//        tree->Branch("lhcComment",            	&lhcComment,            "lhcComment/C");
//        tree->Branch("lhcState",            	&lhcState,            	"lhcState/C");
//        tree->Branch("ctppsStatus",            	&ctppsStatus,           "ctppsStatus/C");
          tree->Branch("lumiSection",            	&lumiSection,           "lumiSection/i");
//
//        tree->Branch("beam1VC",          	"std::vector<float>",	&beam1VC);
//        tree->Branch("beam2VC",         	"std::vector<float>",   &beam2VC);
//
//        tree->Branch("is25nsBunchSpacing",      &is25nsBunchSpacing,    "is25nsBunchSpacing/O");
//
//        tree->Branch("bunchConfigurationForBeam1",	"std::vector<int>",   &bunchConfigurationForBeam1);
//        tree->Branch("bunchConfigurationForBeam2",	"std::vector<int>",   &bunchConfigurationForBeam2);
//
//        tree->Branch("phase", "std::vector<float>",	&phase);

//        h4_numBunchesB1 = 	new TH1F("h4_numBunchesB1","Number of Bunches for Beam 1",bunchSlots,0,bunchSlots);
//        h5_numBunchesB2 = 	new TH1F("h5_numBunchesB2","Number of Bunches for Beam 2",bunchSlots,0,bunchSlots);
//        h6_collidingBunches = 	new TH1F("h6_collidingBunches","Number of Coliding Bunches",bunchSlots,0,bunchSlots);
//        h7_targetBunches = 	new TH1F("h7_targetBunches","Number of Target Bunches",bunchSlots,0,bunchSlots);
//        h8_fillType = 		new TH1F("h8_fillType","Fill Type",bins,since,till);
//        h9_particleTypeB1 = 	new TH1F("h9_particleTypeB1","Particle Type for Beam 1",bins,since,till);
//        h10_particleTypeB2 = 	new TH1F("h10_particleTypeB2","Particle Type for Beam 1",bins,since,till);
//        h11_crossingAngle = 	new TH1F("h11_crossingAngle","Crossing Angle",360,0,360);
//        h12_betaStar = 		new TH1F("h12_betaStar","Beta Star",100,0,360);
//        h13_intensityB1 = 	new TH1F("h13_intensityB1","Intensity of Beam 1",1000,0,1000);
//        h14_intensityB2 = 	new TH1F("h14_intensityB2","Intensity of Beam 2",1000,0,1000);
//        h15_energy = 		new TH1F("h15_energy","Energy",bins,since,till);
//        h16_delivLumi = 	new TH1F("h16_delivLumi","Delivered Luminosity",bins,since,till);
//        h17_recLumi = 		new TH1F("h17_recLumi","Recorded Luminosity",bins,since,till);
//        h18_SizeLumiPerBX =     new TH1F("h18_SizeLumiPerBX","Filled BX",bins,since,till);
//	  h18b_SizeVsLumiPerBX =  new TH2F("h18b_SizeVsLumiPerBX","Num Vs Lumi per BX",bins,since,till,bunchSlots,0,bunchSlots);	
//        h19_SizeRF1 =           new TH1F("h19_SizeRF1","Beam1RF BX",bins,since,till);
//        h20_SizeRF2 =           new TH1F("h20_SizeRF2","Beam2RF BX",bins,since,till);
//        h21_SizeVC1 =           new TH1F("h21_SizeVC1","Beam1VC BX",bins,since,till);
//        h22_SizeVC2 =           new TH1F("h22_SizeVC2","Beam2VC BX",bins,since,till);
//        h23_SizeBC1 =           new TH1F("h23_SizeBC1","BunchConfig BX",bins,since,till);
//        h24_SizeBC2 =           new TH1F("h24_SizeBC2","BunchConfig BX",bins,since,till);
	h25_Phase = 		new TH1F("h25_Phase","Phase Correction",1000,-50,50);
	h26_bxOcc = 		new TH1I("h26_bxOcc","BX Occupancy", 3564,0,3564 );
	h27_filledbx = 		new TH1F("h27_FilledBX","Filled BXs",12,-5,6);

//      lumiPerBX.clear();
	beam1VC.clear();
	beam2VC.clear();
//	beam1RF.clear();
//	beam1RF.clear();
	phase.clear();

//	bunchConfigurationForBeam1.clear();
//	bunchConfigurationForBeam2.clear();

        std::cout << "Tree created" << std::endl;
}

void jwk_ana_lhcDump::closeRoot()
{
//        tree->Write();

//         h4_numBunchesB1->Write();
//         h5_numBunchesB2->Write();
//         h6_collidingBunches->Write();
//         h7_targetBunches->Write();
//         h11_crossingAngle->Write();
//         h12_betaStar->Write();
//         h13_intensityB1->Write();
//         h14_intensityB2->Write();
         h25_Phase->Write();
         h26_bxOcc->Write();
	 h27_filledbx->Write(); 

	tfile->Write();
        tfile->Close();
}

void jwk_ana_lhcDump::dbToRoot(const LHCInfo & obja )
{
//        createTime = obja.createTime();
//        beginTime = obja.beginTime();
//        endTime = obja.endTime();

        fillNumber = obja.fillNumber();

//        bunchesInBeam1 = obja.bunchesInBeam1();
//        h4_numBunchesB1->Fill(bunchesInBeam1);
//        bunchesInBeam2 = obja.bunchesInBeam2();
//        h5_numBunchesB2->Fill(bunchesInBeam2);
//        collidingBunches = obja.collidingBunches();
//        h6_collidingBunches->Fill(collidingBunches);
//        targetBunches = obja.targetBunches();
//        h7_targetBunches->Fill(targetBunches);
//
//        fillType = obja.fillType();
//        particleTypeForBeam1 = obja.particleTypeForBeam1();
//        particleTypeForBeam2 = obja.particleTypeForBeam2();
//        crossingAngle = obja.crossingAngle();
//        h11_crossingAngle->Fill(crossingAngle);
//        betaStar = obja.betaStar();
//        h12_betaStar->Fill(betaStar);
//
//        intensityForBeam1 = obja.intensityForBeam1();
//        h13_intensityB1->Fill(intensityForBeam1);
//        intensityForBeam2 = obja.intensityForBeam2();
//        h14_intensityB2->Fill(intensityForBeam2);
//        energy = obja.energy();
//        delivLumi = obja.delivLumi();
//        recLumi = obja.recLumi();
//        instLumi = obja.instLumi();
//        instLumiError = obja.instLumiError();
//
//        injectionScheme = obja.injectionScheme();
//
//        Size_lumiPerBX = obja.lumiPerBX().size();
//        for( unsigned int i  =  0; i < Size_lumiPerBX; i++ ){ 
//		lumiPerBX.push_back( obja.lumiPerBX()[i] ); 
//	}
//
//        lhcState = obja.lhcState();
//        lhcComment = obja.lhcComment();
//        ctppsStatus = obja.ctppsStatus();
//        lumiSection = obja.lumiSection();

        Size_beam1VC = obja.beam1VC().size();
        for( unsigned int i  =  0; i < Size_beam1VC; i++ ){ beam1VC.push_back( obja.beam1VC()[i] ); }
        Size_beam2VC = obja.beam2VC().size();
        for( unsigned int i  =  0; i < Size_beam2VC; i++ ){ beam2VC.push_back( obja.beam2VC()[i] ); }       
//        Size_beam1RF = obja.beam1RF().size();
//        for( unsigned int i  =  0; i < Size_beam1RF; i++ ){ beam1RF.push_back( obja.beam1RF()[i] ); }
//        Size_beam2RF = obja.beam2RF().size();
//        for( unsigned int i  =  0; i < Size_beam2RF; i++ ){ beam2RF.push_back( obja.beam2RF()[i] ); }

//        is25nsBunchSpacing = obja.is25nsBunchSpacing();
//        Size_bunchConfigurationForBeam1 = obja.bunchConfigurationForBeam1().size();
//        for( unsigned int i  =  0; i < Size_bunchConfigurationForBeam1; i++ ){ bunchConfigurationForBeam1.push_back( obja.bunchConfigurationForBeam1()[i] ); }
//        Size_bunchConfigurationForBeam2 = obja.bunchConfigurationForBeam2().size();
//        for( unsigned int i  =  0; i < Size_bunchConfigurationForBeam2; i++ ){ bunchConfigurationForBeam2.push_back( obja.bunchConfigurationForBeam2()[i] ); }

	for( unsigned int i  =  0; i < Size_beam1VC; i++ ){
		float phcor = ((obja.beam1VC()[i]+obja.beam2VC()[i])/2.0)*(2.5/360.0)*1000.0; 
		phase.push_back(phcor);
	//	std::cout << i << " " << eventCount << " " << phcor << std::endl;
	//	if(eventCount%10) h26_bxPhase->Fill(i,eventCount,phcor); 
	}
        h25_Phase->Fill(phase[bx]);
	h26_bxOcc->Fill(bx);	

	bxp5pc = phase[bx+5];
        bxp4pc = phase[bx+4];
        bxp3pc = phase[bx+3];
        bxp2pc = phase[bx+2];
        bxp1pc = phase[bx+1];
        bx0pc = phase[bx];
        bxm1pc = phase[bx-1];
        bxm2pc = phase[bx-2];
        bxm3pc = phase[bx-3];
        bxm4pc = phase[bx-4];

	if( bxp5pc != 0. ) h27_filledbx->Fill( 5.);
        if( bxp4pc != 0. ) h27_filledbx->Fill( 4. );
        if( bxp3pc != 0. ) h27_filledbx->Fill( 3. );
        if( bxp2pc != 0. ) h27_filledbx->Fill( 2. );
        if( bxp1pc != 0. ) h27_filledbx->Fill( 1. );
        if( bx0pc != 0. ) h27_filledbx->Fill( 0. );
        if( bxm1pc != 0. ) h27_filledbx->Fill( -1. );
        if( bxm2pc != 0. ) h27_filledbx->Fill( -2. );
        if( bxm3pc != 0. ) h27_filledbx->Fill( -3. );
        if( bxm4pc != 0. ) h27_filledbx->Fill( -4. );

        tree->Fill();
}

void
jwk_ana_lhcDump::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
   using namespace edm;

        run = iEvent.id().run();
        lumi = iEvent.luminosityBlock();
        event = iEvent.id().event();
        bx = iEvent.bunchCrossing() - 1;

	eventCount++;	

//	std::cout << "Event count : " << eventCount << std::endl;

        edm::ESHandle<LHCInfo> lhcInfoHnd;
        iSetup.get<LHCInfoRcd>().get(lhcInfoHnd);

	edm::Handle<EcalTimingCollection> timingCollection;
	iEvent.getByToken(_timingEvents, timingCollection);

	for(auto  timeEvent : *timingCollection) {}

        if(!lhcInfoHnd.isValid()) {
                std::cout << "LHCInfo not found?\n";
        }else{
                const LHCInfo* lhcInfo = lhcInfoHnd.product();
/*                std::cout << "LHCInfo Data: \n";
                std::cout << "Create Time: " << timeToString( (time_t)(lhcInfo->createTime()) ) << std::endl;
                std::cout << "beginTime : " << timeToString( lhcInfo->beginTime() )  << std::endl;
                std::cout << "fillNumber : " << lhcInfo->fillNumber() << std::endl;
                std::cout << "fillType : " << lhcInfo->fillType() << std::endl;
                std::cout << "lumiSection : " << lhcInfo->lumiSection() << std::endl;
*/
		dbToRoot( *lhcInfo );
//		dump_txt( *pa, (time_t)i.since, (time_t)i.till, outfile );

        }




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
