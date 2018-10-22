import os, re, sys, optparse, commands, shutil, imp
import math, ROOT

infolder = 'plusDiff_'

prehistfile =  '/afs/cern.ch/user/j/jaking/private/ecal/CMSSW_10_1_7/src/EcalTiming/EcalTiming/lxbatch/320688_Calib/output/ecalTiming.root'
posthistfile = '/afs/cern.ch/user/j/jaking/private/ecal/Jacks_Working/CMSSW_10_3_0_pre2/src/EcalTiming/EcalTiming/' + infolder + '320688/ecalTiming.root' 

histstring = infolder + '_Ratio'
inputfileList = ['BXTimeEEM', 'BXTimeEEP', 'BXTimeEB']
output_tfile = 'BXTime' + histstring + '.root'

def draw( inputfile ):


  histname = inputfile + histstring
  preTfile = ROOT.TFile(prehistfile,'READ')
  ROOT.SetOwnership(preTfile,True)
  postTfile = ROOT.TFile(posthistfile,'READ')
  ROOT.SetOwnership(postTfile,True)
  prehist    = preTfile.Get( inputfile )
  posthist    = postTfile.Get( inputfile )

  tfile = ROOT.TFile( output_tfile, "UPDATE")
  ROOT.SetOwnership(tfile,True)


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


for inputfile in inputfileList:

	draw( inputfile )
