// -*- C++ -*-
//
// Package:    anatemplet/jwk_ana
// Class:      jwk_ana
//
/**\class Analyzer jwk_ana.cc EcalTiming/EcalTiming/plugins/jwk_ana.cc

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

//
// class declaration
//

// If the analyzer does not use TFileService, please remove
// the template argument to the base class so the class inherits
// from  edm::one::EDAnalyzer<>
// This will improve performance in multithreaded jobs.


//using reco::TrackCollection;

class jwk_ana : public edm::one::EDAnalyzer<edm::one::SharedResources>  {
   public:
      explicit jwk_ana(const edm::ParameterSet&);
      ~jwk_ana();

      static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);


   private:
      virtual void beginJob() override;
      virtual void analyze(const edm::Event&, const edm::EventSetup&) override;
      virtual void endJob() override;

	int run;
	int lumi;
	int event;
	int bx;


      // ----------member data ---------------------------

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
jwk_ana::jwk_ana(const edm::ParameterSet& iConfig)
{
   //now do what ever initialization is needed
}


jwk_ana::~jwk_ana()
{

   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)

}


//
// member functions
//

void
jwk_ana::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
   using namespace edm;

#ifdef THIS_IS_AN_EVENT_EXAMPLE
    Handle<TrackCollection> tracks;
    iEvent.getByToken(tracksToken_, tracks);
    for(TrackCollection::const_iterator itTrack = tracks->begin();
        itTrack != tracks->end();
        ++itTrack) {
      // do something with track parameters, e.g, plot the charge.
      // int charge = itTrack->charge();
    }
#endif

        run = iEvent.id().run();
        lumi = iEvent.luminosityBlock();
        event = iEvent.id().event();
        bx = iEvent.bunchCrossing();

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
jwk_ana::beginJob()
{

}

// ------------ method called once each job just after ending the event loop  ------------
void
jwk_ana::endJob()
{


}

// ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
void
jwk_ana::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
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
DEFINE_FWK_MODULE(jwk_ana);
