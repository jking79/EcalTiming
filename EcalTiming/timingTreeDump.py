import os, re, sys, optparse, commands, shutil, imp
import math, ROOT

infolder = ''
rootfile = 'dump_LHCInfo_Event_2018-10-23_20_24_39.root'

prehistfile =  '/afs/cern.ch/user/j/jaking/private/ecal/CMSSW_10_1_7/src/EcalTiming/EcalTiming/lxbatch/320688_Calib/output/ecalTiming.root'
filedir = '/afs/cern.ch/user/j/jaking/private/ecal/Jacks_Working/CMSSW_10_3_0_pre2/src/EcalTiming/EcalTiming/' 

inTreeFile = filedir + rootfile


def pareTree( inTreeFile ):

#Declaration of leaf types


  inTree = ROOT.TFile(inTreeFile,'READ')
  ROOT.SetOwnership(inTree,True)
  tree = inTree.get('lhcInfoTree')

  nEvent = tree.GetEntries()
  for iEvent in xrange( nEvent ):

	run = tree.run(iEvent)
	lumi = tree.lumi(iEvent)
	event = tree.event(iEvent)
	bx = tree.bx(iEvent)
	detid = tree.detid(iEvent)
	ix = tree.ix(iEvent)
	iy = tree.iy(iEvent)
	iz = tree.iz(iEvent)
	time = tree.time(iEvent)
	energy = tree.energy(iEvent)
	pre_zero_len = tree.pre_zero_len(iEvent)
	train_count = tree.train_count(iEvent)
	long_train_count = tree.long_train_count(iEvent)
	train_number = tree.train_number(iEvent)


#  prehist.SetLineColor(7)
#  posthist.SetLineColor(8)

  ratiohist = prehist.Clone()

  ratiohist.Add(posthist,-1)
  ratiohist.Divide(posthist)

  preTfile.Close()
  postTfile.Close()

#  ratiohist.SetName( histname )
  ratiohist.SetNameTitle( histname, histname ) 
  ratiohist.Write()  

  c = ROOT.TCanvas( histname,'',800,600)
  c.cd()

  ratiohist.Draw('hist')
#  posthist.Draw('same hist')

  c.Update()

  c.SaveAs( histname + '.png')
  c.Close()
  tfile.Close()


