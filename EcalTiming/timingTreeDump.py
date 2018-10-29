import os, re, sys, optparse, commands, shutil, imp
import math, ROOT

infolder = ''
rootfile = 'dump_LHCInfo_Event_2018-10-23_20_24_39.root'
outfile = 'hist_LHCInfo_Event_2018-10-23_20_24_39.root'

prehistfile =  '/afs/cern.ch/user/j/jaking/private/ecal/CMSSW_10_1_7/src/EcalTiming/EcalTiming/lxbatch/320688_Calib/output/ecalTiming.root'
filedir = '/afs/cern.ch/user/j/jaking/private/ecal/Jacks_Working/CMSSW_10_3_0_pre2/src/EcalTiming/EcalTiming/' 

inTreeFile = filedir + rootfile

bunchSlots = 3564
trainLength = 150
subtrainLength = 50
nSigma = 2.0
maxRange = 10

def getMeanWithinNSigma( timelist ):

        numWsig = 0
        sumWsig = 0
	sum2Wsig = 0
	total = 0
	total2 = 0

	num = len(timelist)
        if( not num ): return [0.0,0.0]
	for time in timelist:
		total = total + time
                total2 = total2 + time*time

        mean = total/num
        stdev = math.sqrt(total2 / num - mean * mean )
        sRange = min( maxRange, stdev*nSigma )
        for time in timelist:
                if( abs(time - mean ) < sRange):
                        sumWsig = sumWsig + time
			sum2Wsig = sum2Wsig + time*time
                        numWsig = numWsig + 1
	
	meanWsig = sumWsig / numWsig
	stdevWsig = math.sqrt( sum2Wsig / numWsig - meanWsig * meanWsig )
	errorWsig = stdevWsig / math.sqrt( numWsig )
        return [ meanWsig, errorWsig ]
# end of function

def parseTree( inTreeFile ):

#Declaration of leaf types


  inTree = ROOT.TFile(inTreeFile,'READ')
  ROOT.SetOwnership(inTree,True)
  tree = inTree.Get('lhcInfoTree')

  EBbxlist = []
  lt_EBbxlist = []
  t_EBbxlist = []
  EEMbxlist = []
  EEPbxlist = []
  for ibx in xrange(bunchSlots):
	EBbxlist.append([])
        EEMbxlist.append([])
        EEPbxlist.append([])
  for ibx in xrange(trainLength):
        lt_EBbxlist.append([])
  for ibx in xrange(subtrainLength):
        t_EBbxlist.append([])

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
  nEvent = tree.GetEntries()
  for iEvent in xrange( nEvent ):
	
	tree.GetEntry(iEvent)
	
	run = tree.run
	lumi = tree.lumi
	event = tree.event
	bx = tree.bx

	detid = tree.detid
	ix = tree.ix
	iy = tree.iy
	iz = tree.iz

	time = tree.time
	energy = tree.energy

	pre_zero_len = tree.pre_zero_len
	train_count = tree.train_count
	long_train_count = tree.long_train_count
	train_number = tree.train_number
        long_train_number = tree.long_train_number

	fillNumber = tree.fillNumber

	bx0pc = tree.bx0pc
        bxm4pc = tree.bxm4pc
        bxm3pc = tree.bxm3pc
        bxm2pc = tree.bxm2pc
        bxm1pc = tree.bxm1pc
        bxp1pc = tree.bxp1pc
        bxp2pc = tree.bxp2pc
        bxp3pc = tree.bxp3pc
        bxp4pc = tree.bxp4pc
        bxp5pc = tree.bxp5pc

	lumiSection = tree.lumiSection

	ave_phase = tree.ave_phase
        dif_phase = tree.dif_phase
        vc2_phase = tree.vc2_phase

	if(iz == 0): 
		EBbxlist[bx].append(time)
		lt_EBbxlist[long_train_count].append(time)	
                t_EBbxlist[train_count].append(time)
        if(iz < 0): 
		EEMbxlist[bx].append(time)
        if(iz > 0): 
		EEPbxlist[bx].append(time)


	#   end of loop

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

  for ibx in xrange(trainLength):
        r = getMeanWithinNSigma(lt_EBbxlist[ibx])
        lt_BXTimeEB.SetBinContent( ibx, r[0] )
        lt_BXTimeEB.SetBinError( ibx, r[1] )
        r = getMeanWithinNSigma(EEMbxlist[ibx])
        lt_BXTimeEEM.SetBinContent( ibx, r[0] )
        lt_BXTimeEEM.SetBinError( ibx, r[1] )
        r = getMeanWithinNSigma(EEPbxlist[ibx])
        lt_BXTimeEEP.SetBinContent( ibx, r[0] )
        lt_BXTimeEEP.SetBinError( ibx, r[1] )

  for ibx in xrange(subtrainLength):
        r = getMeanWithinNSigma(t_EBbxlist[ibx])
        t_BXTimeEB.SetBinContent( ibx, r[0] )
        t_BXTimeEB.SetBinError( ibx, r[1] )
        r = getMeanWithinNSigma(EEMbxlist[ibx])
        t_BXTimeEEM.SetBinContent( ibx, r[0] )
        t_BXTimeEEM.SetBinError( ibx, r[1] )
        r = getMeanWithinNSigma(EEPbxlist[ibx])
        t_BXTimeEEP.SetBinContent( ibx, r[0] )
        t_BXTimeEEP.SetBinError( ibx, r[1] )

#  c1 = ROOT.TCanvas( 'BXTimeEB','',800,600)
#  c1.cd()
#  BXTimeEB.Draw('hist')
#  c1.Update()
#  c1.SaveAs( 'BXTimeEB.png')
#  c1.Close()

  tfile = ROOT.TFile( outfile, "UPDATE")
  ROOT.SetOwnership(tfile,True)

  BXTimeEB.Write()
  lt_BXTimeEB.Write()
  t_BXTimeEB.Write()
  BXTimeEEM.Write()
  lt_BXTimeEEM.Write()
  t_BXTimeEEM.Write()
  BXTimeEEP.Write()
  lt_BXTimeEEP.Write()
  t_BXTimeEEP.Write()


  inTree.Close()
  tfile.Close()

# end of function


parseTree( inTreeFile )



#          if(tz.first == 0) BXTimeEB_->SetBinContent(BXTimeEB_->FindBin(tx.first),untils->getMeanWithinNSigma(nSigma,maxRange));
#          else if(tz.first == 1) BXTimeEEM_->SetBinContent(BXTimeEEM_->FindBin(tx.first),untils->getMeanWithinNSigma(nSigma,maxRange));
#          else if(tz.first == 2) BXTimeEEP_->SetBinContent(BXTimeEEP_->FindBin(tx.first),untils->getMeanWithinNSigma(nSigma,maxRange));


#def scratch():
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
#		if(!_num) return -999.f;
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


