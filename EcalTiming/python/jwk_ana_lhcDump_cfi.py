#
# Jack W King III - KU
#
 
import FWCore.ParameterSet.Config as cms
 
my_ana = cms.EDAnalyzer("jwk_ana_lhcDump",
                    	recHitEECollection = cms.InputTag("ecalRecHitEBSelector"),
                    	recHitEBCollection = cms.InputTag("ecalRecHitEESelector"),
	
			)
