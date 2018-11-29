import os, re, sys, optparse, commands, shutil, imp
import math, ROOT

infolder = ''
prehistfile =  '/afs/cern.ch/user/j/jaking/private/ecal/CMSSW_10_1_7/src/EcalTiming/EcalTiming/lxbatch/320688_Calib/output/ecalTiming.root'
filedir = '/afs/cern.ch/work/j/jaking/private/ecal/CMSSW_10_3_0_pre2/src/EcalTiming/EcalTiming/'

bunchSlots = 3564
trainLength = 150
subtrainLength = 50
trainNumber = 25
nSigma = 2.0
maxRange = 10

inTreeFile = 'hist_LHCInfo_Event_Run320688.root'

def makeplot( inTreeFile ):

#  print( 'Creating Out File: '+ outfile)
#  tfile = ROOT.TFile( outfile, 'recreate')
#  ROOT.SetOwnership(tfile,True)

  print( 'Opening input file' )
  inTree = ROOT.TFile(inTreeFile,'read')
  ROOT.SetOwnership(inTree,True)

  ROOT.gStyle.SetOptStat(0)

  hist0 = inTree.Get('BXTime_rh_EB_0GeV_SubTrain' )
  hist0.SetLineColor(2)
  hist1 = inTree.Get('BXTime_rh_EB_1GeV_SubTrain' )
  hist1.SetLineColor(2)
  hist2 = inTree.Get('BXTime_rh_EB_2GeV_SubTrain' )
  hist2.SetLineColor(3)
  hist5 = inTree.Get('BXTime_rh_EB_5GeV_SubTrain' )
  hist5.SetLineColor(4)
  hist10 = inTree.Get('BXTime_rh_EB_10GeV_SubTrain' )
  hist10.SetLineColor(6)
  hist20 = inTree.Get('BXTime_rh_EB_20GeV_SubTrain' )
  hist20.SetLineColor(7)

#  hist1 = inTree.Get('BXTime_rh_EB_1GeV_BX' )
#  hist1.SetLineColor(2)
#  hist2 = inTree.Get('BXTime_rh_EB_2GeV_BX' )
#  hist2.SetLineColor(3)
#  hist5 = inTree.Get('BXTime_rh_EB_5GeV_BX' )
#  hist5.SetLineColor(4)
#  hist10 = inTree.Get('BXTime_rh_EB_10GeV_BX' )
#  hist10.SetLineColor(6)
#  hist20 = inTree.Get('BXTime_rh_EB_20GeV_BX' )
#  hist20.SetLineColor(7)

  canvas = ROOT.TCanvas('canvas', '', 500, 500)
  
 # hist0.Draw()
  hist1.Draw()
  hist2.Draw('same')
  hist5.Draw('same')
  hist10.Draw('same')
  hist20.Draw('same')

# Legend (description of what is in the plot):
  leg = ROOT.TLegend( 0.75, 0.70, 0.88, 0.85 )
  leg.SetFillColor(0)
  leg.SetLineColor(0)
  leg.AddEntry(hist1,'1 Gev', 'L')
  leg.AddEntry(hist2,'2 Gev', 'L')
  leg.AddEntry(hist5,'5 Gev', 'L')
  leg.AddEntry(hist10,'10 Gev', 'L')
  leg.AddEntry(hist20,'20 Gev', 'L')
  leg.Draw()


  canvas.SaveAs('stacked_gev_subtrain_plot.pdf')

################################################

makeplot( inTreeFile )


