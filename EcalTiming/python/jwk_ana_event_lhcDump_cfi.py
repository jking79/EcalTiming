#
# Jack W King III - KU
#
 
import FWCore.ParameterSet.Config as cms
 
my_ana = cms.EDAnalyzer("jwk_ana_event_lhcDump",
			pfTag = cms.InputTag('particleFlow'),
                    	recHitEBCollection = cms.InputTag("ecalRecHitEBSelector"),
                    	recHitEECollection = cms.InputTag("ecalRecHitEESelector"),
	
			)
