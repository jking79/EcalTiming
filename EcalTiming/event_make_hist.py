import os, re, sys, optparse, commands, shutil, imp
import math, ROOT

infolder = ''
#rootfile = 'dump_LHCInfo_Event_2018-11-08_04_15_55.root'
#outfile = 'hist_LHCInfo_Event_2018-11-08_04_15_55.root'
rootfile = 'dump_LHCInfo_Event_2018-11-09_19_50_20.root'
outfile = 'hist_LHCInfo_Event_2018-11-09_19_50_20.root'

prehistfile =  '/afs/cern.ch/user/j/jaking/private/ecal/CMSSW_10_1_7/src/EcalTiming/EcalTiming/lxbatch/320688_Calib/output/ecalTiming.root'
filedir = '/afs/cern.ch/work/j/jaking/private/ecal/CMSSW_10_3_0_pre2/src/EcalTiming/EcalTiming/'

inTreeFile = filedir + rootfile

bunchSlots = 3564
trainLength = 150
subtrainLength = 50
nSigma = 2.0
maxRange = 10

# set to -1 for do all
maxEvent = -1

def getsum( ilist ):
       
        tot = 0.0 
        for entry in ilist: tot = tot + entry
        return tot

def getsum_multi( ilist ):

        tot = 0.0
        for entry in ilist : 
#		print( entry )
		tot = tot + entry[0]*entry[1]
        return tot

def getsum_sec( ilist ):

        tot = 0.0
        for entry in ilist : 
#	        print( entry[0], entry[1] )
		tot = tot + entry[1]
        return tot

def getsum_first( ilist ):

        tot = 0.0
        for entry in ilist :
#               print( entry[0], entry[1] )
                tot = tot + entry[0]
        return tot

def getsum2( ilist ):
       
        tot2 = 0.0
        for entry in ilist: tot2 = tot2 + entry*entry
        return tot2

def getmean( ilist ):
       
        num = len(ilist)
        if not num :  return 0.0
        tot = getsum( ilist );
        return tot/num;

def getmean_wcount( ilist ):

	tnum = getsum_sec( ilist )
	if not tnum : return 0.0
	tot = getsum_multi( ilist )
        return tot/tnum;


def getstdev( ilist ):
       
        num = len(ilist);
        if not num : return 0.0; 
        mean = getmean( ilist );
        return sqrt( getsum2( ilist ) / num - mean*mean )


def getstdev2( ilist, mean ):
       
        num = len(ilist);
        if not num : return 0.0;
        return sqrt( getsum2( ilist ) / num - mean*mean )


def geterror( ilist ):
       
        num = len(ilist);
        if not num :  return 0.0;
        stdev = getstdev( ilist );
        return stdev/sqrt(num);

def geterror_wcount( ilist, mean ):

        tnum = getsum_sec(ilist)
        if not tnum :  return 0.0;
        stdev = sqrt( getsum_first( ilist ) / tnum - mean*mean )
        return stdev/sqrt(num);



def geterror2( num, stdev ):
       
        return stdev/sqrt(num);


def parseTree( inTreeFile ):

#Declaration of leaf types

  
  inTree = ROOT.TFile(inTreeFile,'READ')
  ROOT.SetOwnership(inTree,True)
  tree = inTree.Get('lhcInfoTree')

  count = 0

  mtimelist_bx = [ [[],[],[],[],[],[]], [[],[],[],[],[],[]], [[],[],[],[],[],[]] ] 
  mtimelist_train = [ [[],[],[],[],[],[]], [[],[],[],[],[],[]], [[],[],[],[],[],[]] ]
  mtimelist_subtrain = [ [[],[],[],[],[],[]], [[],[],[],[],[],[]], [[],[],[],[],[],[]] ]

  sum2list_bx = [ [[],[],[],[],[],[]], [[],[],[],[],[],[]], [[],[],[],[],[],[]] ]  
  sum2list_train = [ [[],[],[],[],[],[]], [[],[],[],[],[],[]], [[],[],[],[],[],[]] ]
  sum2list_subtrain = [ [[],[],[],[],[],[]], [[],[],[],[],[],[]], [[],[],[],[],[],[]] ]


  for iloc in [0,1,2] :
	for igev in [0,1,2,3,4,5] :
  		for ibx in xrange(bunchSlots):
			mtimelist_bx[iloc][igev].append([])
                        sum2list_bx[iloc][igev].append([])

  for iloc in [0,1,2] :
        for igev in [0,1,2,3,4,5]:
                for ibx in xrange(trainLength):
                        mtimelist_train[iloc][igev].append([])
                        sum2list_train[iloc][igev].append([])

  for iloc in [0,1,2]:
        for igev in [0,1,2,3,4,5]:
                for ibx in xrange(subtrainLength):
                        mtimelist_subtrain[iloc][igev].append([])
                        sum2list_subtrain[iloc][igev].append([])


  EB_BXTime_Train_0GeV = ROOT.TH1F("EB_BXTime_Train_0GeV",  "Mean BX Time[ns] EB; Train Position; Time[ns]",trainLength,0,trainLength)
  EB_BXTime_Train_1GeV = ROOT.TH1F("EB_BXTime_Train_1GeV",  "Mean BX Time[ns] EB; Train Position; Time[ns]",trainLength,0,trainLength)
  EB_BXTime_Train_2GeV = ROOT.TH1F("EB_BXTime_Train_2GeV",  "Mean BX Time[ns] EB; Train Position; Time[ns]",trainLength,0,trainLength)
  EB_BXTime_Train_5GeV = ROOT.TH1F("EB_BXTime_Train_5GeV",  "Mean BX Time[ns] EB; Train Position; Time[ns]",trainLength,0,trainLength)
  EB_BXTime_Train_10GeV = ROOT.TH1F("EB_BXTime_Train_10GeV",  "Mean BX Time[ns] EB; Train Position; Time[ns]",trainLength,0,trainLength)
  EB_BXTime_Train_20GeV = ROOT.TH1F("EB_BXTime_Train_20GeV",  "Mean BX Time[ns] EB; Train Position; Time[ns]",trainLength,0,trainLength)

  
  print( "Parsing Tree" )
  nEntry = tree.GetEntries()
  for iEntry in xrange( nEntry ):

  	mtimemap = [ [[],[],[],[],[],[]], [[],[],[],[],[],[]], [[],[],[],[],[],[]] ]
  	summap = [ [[],[],[],[],[],[]], [[],[],[],[],[],[]], [[],[],[],[],[],[]] ]
  	sum2map = [ [[],[],[],[],[],[]], [[],[],[],[],[],[]], [[],[],[],[],[],[]] ]
  	countmap = [ [[],[],[],[],[],[]], [[],[],[],[],[],[]], [[],[],[],[],[],[]] ]
	
	count += 1
	if( maxEvent > 0 and count > maxEvent ) : break

	tree.GetEntry(iEntry)
	
	run = tree.run
	lumi = tree.lumi
	event = tree.event
	bx = tree.bx

        lumiSection = tree.lumiSection
        fillNumber = tree.fillNumber
	
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


	# EB
	mtimemap[0][0].append(tree.rh_EB_mtime)
	sum2map[0][0].append(tree.rh_EB_s2time)
	summap[0][0].append(tree.rh_EB_stime)
	countmap[0][0].append(tree.rh_EB_count)
        mtimemap[0][1].append(tree.rh1GeV_EB_mtime)
        sum2map[0][1].append(tree.rh1GeV_EB_s2time)
        summap[0][1].append(tree.rh1GeV_EB_stime)
        countmap[0][1].append(tree.rh1GeV_EB_count)
        mtimemap[0][2].append(tree.rh2GeV_EB_mtime)
        sum2map[0][2].append(tree.rh2GeV_EB_s2time)
        summap[0][2].append(tree.rh2GeV_EB_stime)
        countmap[0][2].append(tree.rh2GeV_EB_count)
        mtimemap[0][3].append(tree.rh5GeV_EB_mtime)
        sum2map[0][3].append(tree.rh5GeV_EB_s2time)
        summap[0][3].append(tree.rh5GeV_EB_stime)
        countmap[0][3].append(tree.rh5GeV_EB_count)
        mtimemap[0][4].append(tree.rh10GeV_EB_mtime)
        sum2map[0][4].append(tree.rh10GeV_EB_s2time)
        summap[0][4].append(tree.rh10GeV_EB_stime)
        countmap[0][4].append(tree.rh10GeV_EB_count)
        mtimemap[0][5].append(tree.rh20GeV_EB_mtime)
        sum2map[0][5].append(tree.rh20GeV_EB_s2time)
        summap[0][5].append(tree.rh20GeV_EB_stime)
        countmap[0][5].append(tree.rh20GeV_EB_count)

        # EEP
        mtimemap[1][0].append(tree.rh_EEP_mtime)
        sum2map[1][0].append(tree.rh_EEP_s2time)
        summap[1][0].append(tree.rh_EEP_stime)
        countmap[1][0].append(tree.rh_EEP_count)
        mtimemap[1][1].append(tree.rh1GeV_EEP_mtime)
        sum2map[1][1].append(tree.rh1GeV_EEP_s2time)
        summap[1][1].append(tree.rh1GeV_EEP_stime)
        countmap[1][1].append(tree.rh1GeV_EEP_count)
        mtimemap[1][2].append(tree.rh2GeV_EEP_mtime)
        sum2map[1][2].append(tree.rh2GeV_EEP_s2time)
        summap[1][2].append(tree.rh2GeV_EEP_stime)
        countmap[1][2].append(tree.rh2GeV_EEP_count)
        mtimemap[1][3].append(tree.rh5GeV_EEP_mtime)
        sum2map[1][3].append(tree.rh5GeV_EEP_s2time)
        summap[1][3].append(tree.rh5GeV_EEP_stime)
        countmap[1][3].append(tree.rh5GeV_EEP_count)
        mtimemap[1][4].append(tree.rh10GeV_EEP_mtime)
        sum2map[1][4].append(tree.rh10GeV_EEP_s2time)
        summap[1][4].append(tree.rh10GeV_EEP_stime)
        countmap[1][4].append(tree.rh10GeV_EEP_count)
        mtimemap[1][5].append(tree.rh20GeV_EEP_mtime)
        sum2map[1][5].append(tree.rh20GeV_EEP_s2time)
        summap[1][5].append(tree.rh20GeV_EEP_stime)
        countmap[1][5].append(tree.rh20GeV_EEP_count)

        # EEM
        mtimemap[2][0].append(tree.rh_EEM_mtime)
        sum2map[2][0].append(tree.rh_EEM_s2time)
        summap[2][0].append(tree.rh_EEM_stime)
        countmap[2][0].append(tree.rh_EEM_count)
        mtimemap[2][1].append(tree.rh1GeV_EEM_mtime)
        sum2map[2][1].append(tree.rh1GeV_EEM_s2time)
        summap[2][1].append(tree.rh1GeV_EEM_stime)
        countmap[2][1].append(tree.rh1GeV_EEM_count)
        mtimemap[2][2].append(tree.rh2GeV_EEM_mtime)
        sum2map[2][2].append(tree.rh2GeV_EEM_s2time)
        summap[2][2].append(tree.rh2GeV_EEM_stime)
        countmap[2][2].append(tree.rh2GeV_EEM_count)
        mtimemap[2][3].append(tree.rh5GeV_EEM_mtime)
        sum2map[2][3].append(tree.rh5GeV_EEM_s2time)
        summap[2][3].append(tree.rh5GeV_EEM_stime)
        countmap[2][3].append(tree.rh5GeV_EEM_count)
        mtimemap[2][4].append(tree.rh10GeV_EEM_mtime)
        sum2map[2][4].append(tree.rh10GeV_EEM_s2time)
        summap[2][4].append(tree.rh10GeV_EEM_stime)
        countmap[2][4].append(tree.rh10GeV_EEM_count)
        mtimemap[2][5].append(tree.rh20GeV_EEM_mtime)
        sum2map[2][5].append(tree.rh20GeV_EEM_s2time)
        summap[2][5].append(tree.rh20GeV_EEM_stime)
        countmap[2][5].append(tree.rh20GeV_EEM_count)

	
  	for iloc in [0,1,2] :
        	for igev in [0,1,2,3,4,5] :
                        mtimelist_bx[iloc][igev][bx].append([mtimemap[iloc][igev][0],countmap[iloc][igev][0]])
			mtimelist_train[iloc][igev][train_position].append([mtimemap[iloc][igev][0],countmap[iloc][igev][0]])
                        mtimelist_subtrain[iloc][igev][subtrain_position].append([mtimemap[iloc][igev][0],countmap[iloc][igev][0]])
                        sum2list_bx[iloc][igev][bx].append([sum2map[iloc][igev][0],countmap[iloc][igev][0]])
                        sum2list_train[iloc][igev][train_position].append([sum2map[iloc][igev][0],countmap[iloc][igev][0]])
                        sum2list_subtrain[iloc][igev][subtrain_position].append([sum2map[iloc][igev][0],countmap[iloc][igev][0]])





	#   end of loop

  print( "Filling Histograms" )
  for ibx in xrange(trainLength):
#	print( mtimelist_train[0][0][ibx] )
	mtime = getmean_wcount(mtimelist_train[0][0][ibx])
	EB_BXTime_Train_0GeV.SetBinContent( ibx, mtime )
	EB_BXTime_Train_0GeV.SetBinError( ibx, geterror_wcount(sum2list_train[0][0][ibx], mtime) )





  tfile = ROOT.TFile( outfile, "RECREATE")
  ROOT.SetOwnership(tfile,True)

  EB_BXTime_Train_0GeV.Write()


  inTree.Close()
  tfile.Close()

# end of function


parseTree( inTreeFile )
#print( "syntax check good" )


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
#
#        if( instlumi > 12 ):
#        r = getMeanWithinNSigma(t_EBbxlist12[ibx])
#        t_BXTimeEB_12ifbs.SetBinContent( ibx, r[0] )
#        t_BXTimeEB_12ifbs.SetBinError( ibx, r[1] )
#  c1 = ROOT.TCanvas( 'BXTimeEB','',800,600)
#  c1.cd()
#  BXTimeEB.Draw('hist')
#  c1.Update()
#  c1.SaveAs( 'BXTimeEB.png')
#  c1.Close()
