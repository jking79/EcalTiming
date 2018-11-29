#
# Jack W King III - KU
#

import FWCore.ParameterSet.Config as cms

my_filter = cms.EDFilter("jwk_filter",
                          fedRawDataCollectionTag = cms.InputTag("rawDataCollector"),
                          l1AcceptBunchCrossingCollectionTag = cms.InputTag("scalersRawToDigi","","DQM")
)

