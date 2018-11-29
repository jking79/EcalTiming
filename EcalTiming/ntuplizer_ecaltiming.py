import FWCore.ParameterSet.Config as cms

process = cms.Process("Demo")

process.load("FWCore.MessageService.MessageLogger_cfi")
process.MessageLogger.cerr.FwkReport.reportEvery = cms.untracked.int32(10000)

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )

process.source = cms.Source("PoolSource",
	secondaryFileNames = cms.untracked.vstring(),
	# fileNames = cms.untracked.vstring(options.files),
	fileNames = cms.untracked.vstring(
    *(
'/store/data/Run2018D/EGamma/AOD/PromptReco-v2/000/320/688/00000/FEE64C09-FC97-E811-804D-FA163E1CED1A.root',
    )),
	skipEvents = cms.untracked.uint32(0)
)


process.LHCInfoReader = cms.ESSource("PoolDBESSource",
                DBParameters = cms.PSet(
                        messageLevel = cms.untracked.int32(0),
                        authenticationPath = cms.untracked.string('')
                ),
                toGet = cms.VPSet(
                        cms.PSet(
                                record = cms.string("LHCInfoRcd"),
                                tag = cms.string("LHCInfoStartFillTest_v2")
                        )
                ),
                connect = cms.string('frontier://FrontierPrep/CMS_CONDITIONS')
)

process.lhcinfo_prefer = cms.ESPrefer("PoolDBESSource","LHCInfoReader")

process.my_ana = cms.EDAnalyzer("jwk_ana_event_lhcDump",
                        pfTag = cms.InputTag('particleFlow'),
                        recHitEBCollection = cms.InputTag("ecalRecHitEBSelector"),
                        recHitEECollection = cms.InputTag("ecalRecHitEESelector"),
        
                        )

#process.demo = cms.EDAnalyzer('VertexDistr',
#pfTag = cms.InputTag('particleFlow'),
#)

#process.TFileService = cms.Service("TFileService",
#     fileName = cms.string('output_long.root')
#)

process.p = cms.Path(process.my_ana)
