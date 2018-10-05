// -*- C++ -*-
//
// Package:    templates/jwk_filter
// Class:      jwk_filter
// 
/**\class jwk_filter jwk_filter.cc templates/jwk_filter/plugins/jwk_filter.cc

 Description: [one line class summary]

 Implementation:
     [Notes on implementation]
*/
//
// Original Author:  Jack King
//         Created:  Fri, 14 Sep 2018 19:50:57 GMT
//
//


// system include files
#include <memory>
#include <iostream>
#include <time.h>
#include <fstream>
#include <string>
#include <vector>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/stream/EDFilter.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/StreamID.h"

#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/Framework/interface/EventSetup.h"

#include "CondFormats/DataRecord/interface/LHCInfoRcd.h"
#include "CondFormats/RunInfo/interface/LHCInfo.h"

//#include "DataFormats/FEDRawData/interface/FEDRawData.h"
//#include "DataFormats/FEDRawData/interface/FEDNumbering.h"
//#include "DataFormats/FEDRawData/interface/FEDRawDataCollection.h"
//#include "DataFormats/Scalers/interface/L1AcceptBunchCrossing.h"
//#include "DataFormats/Scalers/interface/ScalersRaw.h"

#include "FWCore/Utilities/interface/InputTag.h"

//
// class declaration
//

class jwk_filter : public edm::stream::EDFilter<> {
   public:
      explicit jwk_filter(const edm::ParameterSet&);
      ~jwk_filter();

      static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);

   private:
      virtual void beginStream(edm::StreamID) override;
      virtual bool filter(edm::Event&, const edm::EventSetup&) override;
      virtual void endStream() override;

      //virtual void beginRun(edm::Run const&, edm::EventSetup const&) override;
      //virtual void endRun(edm::Run const&, edm::EventSetup const&) override;
      //virtual void beginLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&) override;
      //virtual void endLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&) override;
      static std::string timeToString(time_t t);

      // ----------member data --------------------------- 
      
      LHCInfo* lhcinfo;

      //tcc sequence number of currenlty parsed tower block of one DCC
      int iTcc_;
//      edm::InputTag fedRawDataCollectionTag_;
//      edm::InputTag l1AcceptBunchCrossingCollectionTag_;
//      edm::EDGetTokenT<FEDRawDataCollection> fedRawDataCollectionToken_;
//      edm::EDGetTokenT<L1AcceptBunchCrossingCollection> l1AcceptBunchCrossingCollectionToken_;


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
jwk_filter::jwk_filter(const edm::ParameterSet& iConfig)//:
//   fedRawDataCollectionTag_(iConfig.getParameter<edm::InputTag>("fedRawDataCollectionTag")),
//   l1AcceptBunchCrossingCollectionTag_(iConfig.getParameter<edm::InputTag>("l1AcceptBunchCrossingCollectionTag"))

{
   //now do what ever initialization is needed
//   fedRawDataCollectionToken_ = consumes<FEDRawDataCollection>(fedRawDataCollectionTag_);
//   l1AcceptBunchCrossingCollectionToken_ = consumes<L1AcceptBunchCrossingCollection>(l1AcceptBunchCrossingCollectionTag_);

}


jwk_filter::~jwk_filter()
{
 
   // do anything here that needs to be done at destruction time
   // (e.g. close files, deallocate resources etc.)

}


//
// member functions
//

std::string jwk_filter::timeToString(time_t in)
{
        char buf[256];
	struct tm lt;
	time_t t = ( in >> 32 );
        localtime_r(&t, &lt);
        strftime(buf, sizeof(buf),"%F %R:%S", &lt);
        buf[sizeof(buf)-1] = 0;
        return std::string(buf);
}


// ------------ method called on each new Event  ------------
bool
jwk_filter::filter(edm::Event& iEvent, const edm::EventSetup& iSetup)
{
   using namespace edm;
#ifdef THIS_IS_AN_EVENT_EXAMPLE
   Handle<ExampleData> pIn;
   iEvent.getByLabel("example",pIn);
#endif

//   edm::Handle<FEDRawDataCollection> rawdata;
//   iEvent.getByToken(fedRawDataCollectionToken_, rawdata);
//   const FEDRawData & fedData = rawdata->FEDData(ScalersRaw::SCALERS_FED_ID);
//   unsigned short int length =  fedData.size();
//   std::cout << "fedData Size : " << length << std::endl; 

   std::cout << ">>>>>>>>>>>>>>>>>>>>> It Worked <<<<<<<<<<<<<<<<<<<<<<" << std::endl;

        int run = iEvent.id().run();
        int lumi = iEvent.luminosityBlock();
        int event = iEvent.id().event();
        int bx = iEvent.bunchCrossing();  

	std::cout << "Run: " << run << " Lumi: " << lumi << " Event: " << event << " BX: " << bx << std::endl;	

	edm::ESHandle<LHCInfo> lhcInfoHnd;
	iSetup.get<LHCInfoRcd>().get(lhcInfoHnd);


     	if(!lhcInfoHnd.isValid()) {
     		std::cout << "LHCInfo not found?\n";	
     	}else{
		const LHCInfo* lhcInfo = lhcInfoHnd.product();
	       	std::cout << "LHCInfo Data: \n";
		std::cout << "Create Time: " << timeToString( (time_t)(lhcInfo->createTime()) ) << std::endl;
        	std::cout << "beginTime : " << timeToString( lhcInfo->beginTime() )  << std::endl;
		std::cout << "fillNumber : " << lhcInfo->fillNumber() << std::endl;
		std::cout << "fillType : " << lhcInfo->fillType() << std::endl;
		std::cout << "lumiSection : " << lhcInfo->lumiSection() << std::endl;

     	}

//   std::cout << "----------------------------------------------------------------------\n";

//   if(1){
//     edm::Handle<L1AcceptBunchCrossingCollection> l1aHist;
//     iEvent.getByToken(l1AcceptBunchCrossingCollectionToken_, l1aHist);
//     if(!l1aHist.isValid()) {
//       std::cout << "L1A history not found.\n";
//     } else if (l1aHist->empty()) {
//       std::cout << "L1A history is empty.\n";
//     } else{
//       std::cout << "L1A history: \n";
//       for(L1AcceptBunchCrossingCollection::const_iterator it = l1aHist->begin(); it != l1aHist->end(); ++it){
//         	std::cout << "L1A offset: " <<  it->l1AcceptOffset() << "\t"
//              		<< "BX: " <<  it->bunchCrossing() << "\t"
//              		<< "Orbit ID: " << it->orbitNumber() << "\t"
//              		<< "Trigger type: " << it->eventType() << "\n";
//              		<< " ("
//			        << trigNames[it->eventType()&0xF] << ")\n";
//         }
//     }
//     std::cout << "----------------------------------------------------------------------\n";
//  }



#ifdef THIS_IS_AN_EVENTSETUP_EXAMPLE
   ESHandle<SetupData> pSetup;
   iSetup.get<SetupRecord>().get(pSetup);
#endif
   return true;
}

// ------------ method called once each stream before processing any runs, lumis or events  ------------
void
jwk_filter::beginStream(edm::StreamID)
{
}

// ------------ method called once each stream after processing all runs, lumis and events  ------------
void
jwk_filter::endStream() {
}

// ------------ method called when starting to processes a run  ------------
/*
void
jwk_filter::beginRun(edm::Run const&, edm::EventSetup const&)
{ 
}
*/
 
// ------------ method called when ending the processing of a run  ------------
/*
void
jwk_filter::endRun(edm::Run const&, edm::EventSetup const&)
{
}
*/
 
// ------------ method called when starting to processes a luminosity block  ------------
/*
void
jwk_filter::beginLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&)
{
}
*/
 
// ------------ method called when ending the processing of a luminosity block  ------------
/*
void
jwk_filter::endLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&)
{
}
*/
 
// ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
void
jwk_filter::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  //The following says we do not know what parameters are allowed so do no validation
  // Please change this to state exactly what you do use, even if it is no parameters
  edm::ParameterSetDescription desc;
  desc.setUnknown();
  descriptions.addDefault(desc);
}
//define this as a plug-in
DEFINE_FWK_MODULE(jwk_filter);
