import os, re, sys, optparse, commands, shutil, imp
import math, ROOT

infolder = ''
#rootfile = 'dump_LHCInfo_Event_2018-11-08_04_15_55.root'
#outfile = 'hist_LHCInfo_Event_2018-11-08_04_15_55.root'
rootfile = 'dump_LHCInfo_Event_2018-11-08_02_13_57.root'
outfile = 'hist_LHCInfo_Event_2018-11-08_02_13_57.root'

prehistfile =  '/afs/cern.ch/user/j/jaking/private/ecal/CMSSW_10_1_7/src/EcalTiming/EcalTiming/lxbatch/320688_Calib/output/ecalTiming.root'
filedir = '/afs/cern.ch/user/j/jaking/private/ecal/Jacks_Working/CMSSW_10_3_0_pre2/src/EcalTiming/EcalTiming/'

inTreeFile = filedir + rootfile

bunchSlots = 3564
trainLength = 150
subtrainLength = 50
nSigma = 2.0
maxRange = 10

# set to -1 for do all
maxEvent = -1

def getsum( list ):
       
        sum = 0.0 
        for entry in list: sum = sum + entry
        return sum


def getsum2( list ):
       
        sum2 = 0.0
        for entry in list: sum2 = sum2 + entry*entry
        return sum2


def getmean( list ):
       
        num = list.size()
        if not num :  return 0.0
        sum = getsum( list );
        return sum/num;


def getstdev( list ):
       
        num = list.size();
        if not num : return 0.0; 
        mean = getmean( list );
        return sqrt( getsum2( list ) / num - mean*mean )


def getstdev2( list, mean ):
       
        num = list.size();
        if not num : return 0.0;
        return sqrt( getsum2( list ) / num - mean*mean )


def geterror( list ):
       
        num = list.size();
        if not num :  return 0.0;
        stdev = getstdev( list );
        return stdev/sqrt(num);


def geterror2( num, stdev ):
       
        return stdev/sqrt(num);


def parseTree( inTreeFile ):

#Declaration of leaf types


  inTree = ROOT.TFile(inTreeFile,'READ')
  ROOT.SetOwnership(inTree,True)
  tree = inTree.Get('lhcInfoTree')

  count = 0

  lt_BXSpreadEB_list = []

  BXTimeEB = ROOT.TH1F("BXTimeEB",  "Mean BX Time[ns] EB; BX;Time[ns]",bunchSlots,0,bunchSlots);
  lt_BXTimeEB = ROOT.TH1F("lt_BXTimeEB",  "Mean Train_BX Time[ns] EB; Trian Position;Time[ns]",trainLength,0,trainLength);
  t_BXTimeEB = ROOT.TH1F("t_BXTimeEB",  "Mean Subtrain_BX Time[ns] EB; Subtrian Position;Time[ns]",subtrainLength,0,subtrainLength);  
  BXTimeEEM = ROOT.TH1F("BXTimeEEM", "Mean BX Time[ns] EE-;BX; Time[ns]",bunchSlots,0,bunchSlots);
  lt_BXTimeEEM = ROOT.TH1F("lt_BXTimeEEM",  "Mean Train_BX Time[ns] EE-; Trian Position;Time[ns]",trainLength,0,trainLength);
  t_BXTimeEEM = ROOT.TH1F("t_BXTimeEEM",  "Mean Subtrain_BX Time[ns] EE-; Subtrian Position;Time[ns]",subtrainLength,0,subtrainLength);
  BXTimeEEP = ROOT.TH1F("BXTimeEEP", "Mean BX Time[ns] EE+;BX; Time[ns]",bunchSlots,0,bunchSlots);
  lt_BXTimeEEP = ROOT.TH1F("lt_BXTimeEEP",  "Mean Train_BX Time[ns] EE+; Trian Position;Time[ns]",trainLength,0,trainLength);
  t_BXTimeEEP = ROOT.TH1F("t_BXTimeEEP",  "Mean Subtrain_BX Time[ns] EE+; Subtrian Position;Time[ns]",subtrainLength,0,subtrainLength);

  
  print( "Parsing Tree" )
  nEntry = tree.GetEntries()
  for iEntry in xrange( nEntry ):
	
	count += 1
	if( maxEvent > 0 and count > maxEvent ) : break

	tree.GetEntry(iEntry)
	
	run = tree.run
	lumi = tree.lumi
	event = tree.event
	bx = tree.bx
	
	time = tree.time
	energy = tree.energy
	instlumi = tree.instlumi

	subtrain_position = tree.subtrain_position
        train_position = tree.train_position
        subtrain_number = tree.subtrain_number
        train_number = tree.train_number



	fillNumber = tree.fillNumber
	lumiSection = tree.lumiSection

	ave_phase = tree.ave_phase
        dif_phase = tree.dif_phase
        vc2_phase = tree.vc2_phase


	#   end of loop

#  for ibx in xrange(bunchSlots):
	
  print( "Filling Histograms" )
  for ibx in xrange(bunchSlots):
	r = getMeanWithinNSigma(EBbxlist[ibx])
	BXTimeEB.SetBinContent( ibx, r[0] )
	BXTimeEB.SetBinError( ibx, r[1] )
        
        r = getMeanWithinNSigma(EEMbxlist[ibx])
        BXTimeEEM.SetBinContent( ibx, r[0] )
        BXTimeEEM.SetBinError( ibx, r[1] )
        
        r = getMeanWithinNSigma(EEPbxlist[ibx])
        BXTimeEEP.SetBinContent( ibx, r[0] )
        BXTimeEEP.SetBinError( ibx, r[1] )

#	if( instlumi > 4 ):
        r = getMeanWithinNSigma(EBbxlist4[ibx])
	BXTimeEB_4ifbs.SetBinContent( ibx, r[0] )
        BXTimeEB_4ifbs.SetBinError( ibx, r[1] )
#        if( instlumi > 8 ):
        r = getMeanWithinNSigma(EBbxlist8[ibx])
        BXTimeEB_8ifbs.SetBinContent( ibx, r[0] )
        BXTimeEB_8ifbs.SetBinError( ibx, r[1] )
#        if( instlumi > 12 ):
        r = getMeanWithinNSigma(EBbxlist12[ibx])
        BXTimeEB_12ifbs.SetBinContent( ibx, r[0] )
        BXTimeEB_12ifbs.SetBinError( ibx, r[1] )


  for ibx in xrange(trainLength):

	title = "BXTimeEB_lt" + str(ibx)
	lt_BXSpreadEB =  ROOT.TH1F(title,"BX Time Distro for trian position: "+str(ibx)+" EB;Time[ns];",1000,-5.0,5.0);
	for etime in lt_EBbxlist[ibx] :
		lt_BXSpreadEB.Fill( etime )
                lt_BXSpread2dEB.Fill( ibx, etime )
	lt_BXSpreadEB_list.append( lt_BXSpreadEB )
		
        r = getMeanWithinNSigma(lt_EBbxlist[ibx])
        lt_BXTimeEB.SetBinContent( ibx, r[0] )
        lt_BXTimeEB.SetBinError( ibx, r[1] )

        r = getMeanWithinNSigma(lt_EEMbxlist[ibx])
        lt_BXTimeEEM.SetBinContent( ibx, r[0] )
        lt_BXTimeEEM.SetBinError( ibx, r[1] )

        r = getMeanWithinNSigma(lt_EEPbxlist[ibx])
        lt_BXTimeEEP.SetBinContent( ibx, r[0] )
        lt_BXTimeEEP.SetBinError( ibx, r[1] )

#       if( instlumi > 4 ):
        r = getMeanWithinNSigma(lt_EBbxlist4[ibx])
        lt_BXTimeEB_4ifbs.SetBinContent( ibx, r[0] )
        lt_BXTimeEB_4ifbs.SetBinError( ibx, r[1] )
#        if( instlumi > 8 ):
        r = getMeanWithinNSigma(lt_EBbxlist8[ibx])
        lt_BXTimeEB_8ifbs.SetBinContent( ibx, r[0] )
        lt_BXTimeEB_8ifbs.SetBinError( ibx, r[1] )
#        if( instlumi > 12 ):
        r = getMeanWithinNSigma(lt_EBbxlist12[ibx])
        lt_BXTimeEB_12ifbs.SetBinContent( ibx, r[0] )
        lt_BXTimeEB_12ifbs.SetBinError( ibx, r[1] )

  for ibx in xrange(subtrainLength):
        r = getMeanWithinNSigma(t_EBbxlist[ibx])
        t_BXTimeEB.SetBinContent( ibx, r[0] )
        t_BXTimeEB.SetBinError( ibx, r[1] )

        r = getMeanWithinNSigma(t_EEMbxlist[ibx])
        t_BXTimeEEM.SetBinContent( ibx, r[0] )
        t_BXTimeEEM.SetBinError( ibx, r[1] )

        r = getMeanWithinNSigma(t_EEPbxlist[ibx])
        t_BXTimeEEP.SetBinContent( ibx, r[0] )
        t_BXTimeEEP.SetBinError( ibx, r[1] )

#       if( instlumi > 4 ):
        r = getMeanWithinNSigma(t_EBbxlist4[ibx])
        t_BXTimeEB_4ifbs.SetBinContent( ibx, r[0] )
        t_BXTimeEB_4ifbs.SetBinError( ibx, r[1] )
#        if( instlumi > 8 ):
        r = getMeanWithinNSigma(t_EBbxlist8[ibx])
        t_BXTimeEB_8ifbs.SetBinContent( ibx, r[0] )
        t_BXTimeEB_8ifbs.SetBinError( ibx, r[1] )
#        if( instlumi > 12 ):
        r = getMeanWithinNSigma(t_EBbxlist12[ibx])
        t_BXTimeEB_12ifbs.SetBinContent( ibx, r[0] )
        t_BXTimeEB_12ifbs.SetBinError( ibx, r[1] )



#  c1 = ROOT.TCanvas( 'BXTimeEB','',800,600)
#  c1.cd()
#  BXTimeEB.Draw('hist')
#  c1.Update()
#  c1.SaveAs( 'BXTimeEB.png')
#  c1.Close()

  tfile = ROOT.TFile( outfile, "RECREATE")
  ROOT.SetOwnership(tfile,True)

  for hist in lt_BXSpreadEB_list:
	hist.Write()

  lt_BXSpread2dEB.Write()

  BXTimeEB.Write()
  lt_BXTimeEB.Write()
  t_BXTimeEB.Write()
  BXTimeEB_4ifbs.Write()
  lt_BXTimeEB_4ifbs.Write()
  t_BXTimeEB_4ifbs.Write()
  BXTimeEB_8ifbs.Write()
  lt_BXTimeEB_8ifbs.Write()
  t_BXTimeEB_8ifbs.Write()
  BXTimeEB_12ifbs.Write()
  lt_BXTimeEB_12ifbs.Write()
  t_BXTimeEB_12ifbs.Write()
  BXTimeEEM.Write()
  lt_BXTimeEEM.Write()
  t_BXTimeEEM.Write()
  BXTimeEEP.Write()
  lt_BXTimeEEP.Write()
  t_BXTimeEEP.Write()


  inTree.Close()
  tfile.Close()

# end of function


#parseTree( inTreeFile )
print( "syntax check good" )


#          if(tz.first == 0) BXTimeEB_->SetBinContent(BXTimeEB_->FindBin(tx.first),untils->getMeanWithinNSigma(nSigma,maxRange));
#          else if(tz.first == 1) BXTimeEEM_->SetBinContent(BXTimeEEM_->FindBin(tx.first),untils->getMeanWithinNSigma(nSigma,maxRange));
#          else if(tz.first == 2) BXTimeEEP_->SetBinContent(BXTimeEEP_->FindBin(tx.first),untils->getMeanWithinNSigma(nSigma,maxRange));


#def scratch():
# Energy -> 1,2,5,10,20 GeV

#
#        if(iz == 0) ix += 85;
#        else if(iz < 0) iz = 1;
#        else if(iz > 0)iz = 2;
#
#	float range = std::min(maxRange, stdDev() * n_sigma);
#	for(auto te : _timingEvents) {
#		if(fabs(te - mean()) < range) {
#			_sumWithinNSigma[n_sigma] += te;
#			_sum2WithinNSigma[n_sigma] += te * te;
#			_sum3WithinNSigma[n_sigma] += te * te * te;
#			_numWithinNSigma[n_sigma]++;
#		}
#	}
#	getMeanWithinNSigma : return _sumWithinNSigma[n_sigma] / _numWithinNSigma[n_sigma];
#
#	inline float mean() const
#	{
#		if(not _num) return -999.f;
#		return _sum / _num;
#	}; 
#	inline float stdDev() const  
#	{
#		float mean_ = mean();
#		return sqrt(_sum2 / _num - mean_ * mean_);
#	};
#
#bool EcalTimeCalibrationUtils::add(std::vector<float>& timingEvents)
#{
#        _timingEvents = timingEvents;
#        _num = _timingEvents.size();
#        for(auto te : _timingEvents)
#            _sum += te;
#        for(auto te : _timingEvents)
#            _sum2 += te * te;
#
#        return true;
#}
#
#
##  prehist.SetLineColor(7)
##  posthist.SetLineColor(8)
#
#  ratiohist = prehist.Clone()
#
#  ratiohist.Add(posthist,-1)
#  ratiohist.Divide(posthist)
#
#  preTfile.Close()
#  postTfile.Close()
#
##  ratiohist.SetName( histname )
#  ratiohist.SetNameTitle( histname, histname ) 
#  ratiohist.Write()  
#
#  c = ROOT.TCanvas( histname,'',800,600)
#  c.cd()
#
#  ratiohist.Draw('hist')
##  posthist.Draw('same hist')
#
#  c.Update()
#
#  c.SaveAs( histname + '.png')
#  c.Close()
#  tfile.Close()


