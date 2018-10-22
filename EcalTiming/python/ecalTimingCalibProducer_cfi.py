import FWCore.ParameterSet.Config as cms

timing = cms.EDFilter("EcalTimingCalibProducer",
                    isSplash = cms.bool(False),
                    makeEventPlots = cms.bool(False),
                    timingCollection = cms.InputTag("EcalTimingEvents"),
                    recHitMinimumN = cms.uint32(2),
                    minRecHitEnergyStep = cms.double(0.5),
                    minRecHitEnergyNStep = cms.double(10),
                    energyThresholdOffsetEE = cms.double(0.0),
                    energyThresholdOffsetEB = cms.double(0.0),
                    minEntries = cms.uint32(1),
                    globalOffset = cms.double(0.),
                    storeEvents = cms.bool(False),
                    produceNewCalib = cms.bool(True),
                    outputDumpFile = cms.string('output.dat'),
                    maxSkewnessForDump = cms.double(2),
		    ph_sign_corr = cms.double(0.)
                    )
