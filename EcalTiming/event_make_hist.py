import os, re, sys, optparse, commands, shutil, imp
import math, ROOT

infolder = ''
rootfile = 'dump_LHCInfo_Event_2018-11-14_19_35_00.root'
outfile = 'hist_LHCInfo_Event_2018-11-14_19_35_00.root'
#rootfile = 'dump_LHCInfo_Event_324998_A48.root'
#outfile = 'hist_LHCInfo_Event_324998_A48.root'


prehistfile =  '/afs/cern.ch/user/j/jaking/private/ecal/CMSSW_10_1_7/src/EcalTiming/EcalTiming/lxbatch/320688_Calib/output/ecalTiming.root'
filedir = '/afs/cern.ch/work/j/jaking/private/ecal/CMSSW_10_3_0_pre2/src/EcalTiming/EcalTiming/'

inTreeFile = filedir + rootfile

bunchSlots = 3564
trainLength = 150
subtrainLength = 50
trainNumber = 25
nSigma = 2.0
maxRange = 10

# set to -1 for do all
#maxEvent = -1
maxEvent = 500000

def _getsum( ilist ):
       
        tot = 0.0 
        for entry in ilist: tot = tot + entry
        return tot

def _getreducedsum( ilist, less ):

        tot = 0.0
        for entry in ilist: tot = tot + (entry - less)
        return tot

def _getsum_multi( ilist ):

        tot = 0.0
        for entry in ilist : 
		tot = tot + entry[0]*entry[1]
        return tot

def _getsum_sec( ilist ):

        tot = 0.0
        for entry in ilist : 
		tot = tot + entry[1]
        return tot

def _getsum_first( ilist ):

        tot = 0.0
        for entry in ilist :
                tot = tot + entry[0]
        return tot

def _getreducedsum_first( ilist, less ):

        tot = 0.0
	count = 0
        for entry in ilist :
		count = count + 1
                if( count%3 ) : tot = tot + (entry[0] - less)
        return tot

def _getsamplesum_first( ilist ):
	
	count = 0
	num = 0
	tot = 0.0
	for entry in ilist:
		count = count + 1
		if( not count%3 ):
			tot = tot + entry[0]
			num = num + 1
	return tot, num

def _getreducedsum2( ilist, less ):
       
        tot2 = 0.0
	count = 0
        for entry in ilist:
		count += count
		if( count%3 ): 
			tot2 = tot2 + ((entry[0] - less)*(entry[0] - less))
        return tot2

def _getsum2( ilist ):

        tot2 = 0.0
        for entry in ilist: tot2 = tot2 + (entry*entry)
        return tot2

def _getmean( ilist ):
       
        num = len(ilist)
        if not num :  return 0.0
        tot = getsum( ilist );
        return tot/num;

def _get_reduced_mean( ilist ):

        num = len(ilist)
        if not num :  return 0.0
	less, lnum = getsamplesum_first( ilist )
        tot = getreducedsum_first( ilist, less )
        return tot/(num-lnum)

def _getmean_nocount( ilist ):

        num = len(ilist)
        if not num :  return 0.0
        tot = getsum_first( ilist );
        return tot/num;

def _getmean_wcount( ilist ):

	tnum = getsum_sec( ilist )
	if not tnum : return 0.0
	tot = getsum_multi( ilist )
        return tot/tnum;


def _getstdev( ilist ):
       
        num = len(ilist);
        if not num : return 0.0; 
        mean = getmean( ilist );
        return math.sqrt( getsum2( ilist ) / num - mean*mean )


def _getstdev_wmean( ilist, mean, num ):
       
	res = getsum( ilist, 0 ) / num - mean*mean
	if res < 0 : return 0.0
        return math.sqrt( res  )

def _getstdev_w_nocount_mean( ilist, mean, num ):

        res = getsum_first( ilist ) / num - mean*mean
        if res < 0 : return 0.0
        return math.sqrt( res  )

def _getstdev_w_sum_mean( ilist, mean, num ):

        res = getsum2( ilist ) / num - mean*mean
        if res < 0 : return 0.0
        return math.sqrt( res  )

def _getstdev_w_reducedsum_mean( ilist, mean, num ):

	less, lnum = getsamplesum_first( ilist )
        res = (getreducedsum2( ilist, less ) / (num-lnum)) - mean*mean
        if res < 0 : return [0.0,lnum]
	ret = math.sqrt( res )
        return [ret, lnum]

def _geterror( ilist ):
       
        num = len(ilist);
        if not num :  return 0.0;
        stdev = getstdev( ilist );
        return stdev/math.sqrt(num);

def _geterror_wmean( ilist, mean ):

        num = len(ilist);
        if not num :  return 0.0;
        stdev = getstdev_wmean( ilist, mean, num );
        return stdev/math.sqrt(num);

def _geterror_w_nocount_mean( ilist, mean ):

        num = len(ilist);
        if not num :  return 0.0;
        stdev = getstdev_w_nocount_mean( ilist, mean, num );
        return stdev/math.sqrt(num);

def _geterror_w_sum_mean( ilist, mean ):

        num = len(ilist);
        if not num :  return 0.0;
        stdev = getstdev_w_sum_mean( ilist, mean, num );
        return stdev/math.sqrt(num);

def _geterror_w_sum_mean_reduced( ilist, mean ):

        num = len(ilist);
        if not num :  return 0.0;
        ret = getstdev_w_reducedsum_mean( ilist, mean, num );
        return ret[0]/math.sqrt(num-ret[1]);

def _geterror_wcount( ilist, mean ):

        tnum = getsum_sec(ilist)
        if not tnum :  return 0.0;
	res = getsum_first( ilist ) / tnum - mean*mean
	if( res < 0 ): return 0.0
        return math.sqrt(res/tnum)

def _geterror_wstdev( num, stdev ):
       
        return stdev/math.sqrt(num);

def get_flat_mean( ilist ):

	tot = 0.0
	num = len(ilist)
	if not num : return 0.0
	for entry in ilist:
		tot = tot + entry[0]
	return tot/num

def get_flat_error( ilist, mean ):

	tot = 0
	num = len(ilist)
	if not num : return 0.0
	for entry in ilist:
                tot = tot + ((entry[0]-mean)*(entry[0]-mean))
	return math.sqrt(tot)/num


def get_expanded_mean( ilist ):

        tot = 0.0
	tnum = 0	
        num = len(ilist)
        if not num : return 0.0
        for entry in ilist:
		tnum = tnum + entry[1]
                tot = tot + entry[0]*entry[1]
        return tot/tnum

def get_expanded_error( ilist, mean ):

        tot = 0
	tnum = 0
        num = len(ilist)
        if not num : return 0.0
        for entry in ilist:
		tnum = tnum + entry[1]
                tot = tot + (entry[0]*entry[0])
        return math.sqrt(tot-(tnum*mean*mean))/tnum

def get_red_mean( ilist, less ):

        tot = 0.0
        num = len(ilist)
        if not num : return 0.0
        for entry in ilist:
                tnum = tnum + entry[1]
                tot = tot + entry[0]*entry[1]
        return tot/tnum

#td - rh - ev - red


def parseTree( inTreeFile ):

# create out file
  print( 'Creating Out File' )
  tfile = ROOT.TFile( outfile, 'recreate')
  ROOT.SetOwnership(tfile,True)


#Declaration of leaf types

  print( 'Opening input file' )
  inTree = ROOT.TFile(inTreeFile,'read')
  ROOT.SetOwnership(inTree,True)
  tree = inTree.Get('lhcInfoTree')

  nEntry = tree.GetEntries()

  count = 0

  mtimelist = []
  BXTime_rh_hist = []
  BXTime_ev_hist = []
  BXTime_red_hist = []
  mtime_tlist = []
  MTimeLoc_hist = []

  print( 'Initializing input varibles' )
  for iloc in [0,1,2] :
	mtimelist.append([])
  	mtime_tlist.append([])
	BXTime_rh_hist.append([])
	BXTime_ev_hist.append([])
	BXTime_red_hist.append([])
	MTimeLoc_hist.append([])
	for ilen in xrange( trainLength ):
		MTimeLoc_hist[iloc].append([])
        for inum in xrange( trainNumber ):
        	mtime_tlist[iloc].append([])
                for ipos in xrange( trainLength ):
                	mtime_tlist[iloc][inum].append([])
	#		for entry in xrange( nEntry ):
				
	for igev in [0,1,2,3,4,5] :
		mtimelist[iloc].append([])
		BXTime_rh_hist[iloc].append([])
		BXTime_ev_hist[iloc].append([])
		BXTime_red_hist[iloc].append([])
		for itype, runlen in [ [0,bunchSlots],[1,trainLength],[2,subtrainLength] ]:  
			mtimelist[iloc][igev].append([])
			BXTime_rh_hist[iloc][igev].append([])
			BXTime_ev_hist[iloc][igev].append([])
			BXTime_red_hist[iloc][igev].append([])
  			for ibx in xrange(runlen):
				mtimelist[iloc][igev][itype].append([])

  
  print( 'Reading input tree' )
  for iEntry in xrange( nEntry ):

	mtimemap = []
        summap = []
        countmap = []

  	for iloc in [0,1,2] :
		mtimemap.append([])
		summap.append([])
		countmap.append([])
		mtime_tlist.append([])
        	for igev in [0,1,2,3,4,5] :
			mtimemap[iloc].append([])
			summap[iloc].append([])
			countmap[iloc].append([])

	count += 1
	if( maxEvent > 0 and count > maxEvent ) : break
        if( not count%100000 ): print( 'Processing Entry '+str(count)+' Event '+str(event)+' Run '+str(run)+' LumiSection '+str(lumiSection)+' Fill '+str(fillNumber) )

	tree.GetEntry(iEntry)
	
	run = tree.run
	lumi = tree.lumi
	event = tree.event
	bx = tree.bx

        lumiSection = tree.lumiSection
        fillNumber = tree.fillNumber
	

#	energy = tree.energy
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
	mtime_tlist[0][train_number][train_position].append([tree.rh_EB_mtime,tree.rh_EB_count])
	mtimemap[0][0].append(tree.rh_EB_mtime)
	summap[0][0].append(tree.rh_EB_stime)
	countmap[0][0].append(tree.rh_EB_count)
        mtimemap[0][1].append(tree.rh1GeV_EB_mtime)
        summap[0][1].append(tree.rh1GeV_EB_stime)
        countmap[0][1].append(tree.rh1GeV_EB_count)
        mtimemap[0][2].append(tree.rh2GeV_EB_mtime)
        summap[0][2].append(tree.rh2GeV_EB_stime)
        countmap[0][2].append(tree.rh2GeV_EB_count)
        mtimemap[0][3].append(tree.rh5GeV_EB_mtime)
        summap[0][3].append(tree.rh5GeV_EB_stime)
        countmap[0][3].append(tree.rh5GeV_EB_count)
        mtimemap[0][4].append(tree.rh10GeV_EB_mtime)
        summap[0][4].append(tree.rh10GeV_EB_stime)
        countmap[0][4].append(tree.rh10GeV_EB_count)
        mtimemap[0][5].append(tree.rh20GeV_EB_mtime)
        summap[0][5].append(tree.rh20GeV_EB_stime)
        countmap[0][5].append(tree.rh20GeV_EB_count)

        # EEP
	mtime_tlist[1][train_number][train_position].append([tree.rh_EEM_mtime,tree.rh_EEP_count])
        mtimemap[1][0].append(tree.rh_EEP_mtime)
        summap[1][0].append(tree.rh_EEP_stime)
        countmap[1][0].append(tree.rh_EEP_count)
        mtimemap[1][1].append(tree.rh1GeV_EEP_mtime)
        summap[1][1].append(tree.rh1GeV_EEP_stime)
        countmap[1][1].append(tree.rh1GeV_EEP_count)
        mtimemap[1][2].append(tree.rh2GeV_EEP_mtime)
        summap[1][2].append(tree.rh2GeV_EEP_stime)
        countmap[1][2].append(tree.rh2GeV_EEP_count)
        mtimemap[1][3].append(tree.rh5GeV_EEP_mtime)
        summap[1][3].append(tree.rh5GeV_EEP_stime)
        countmap[1][3].append(tree.rh5GeV_EEP_count)
        mtimemap[1][4].append(tree.rh10GeV_EEP_mtime)
        summap[1][4].append(tree.rh10GeV_EEP_stime)
        countmap[1][4].append(tree.rh10GeV_EEP_count)
        mtimemap[1][5].append(tree.rh20GeV_EEP_mtime)
        summap[1][5].append(tree.rh20GeV_EEP_stime)
        countmap[1][5].append(tree.rh20GeV_EEP_count)

        # EEM
	mtime_tlist[2][train_number][train_position].append([tree.rh_EEM_mtime,tree.rh_EEM_count])
        mtimemap[2][0].append(tree.rh_EEM_mtime)
        summap[2][0].append(tree.rh_EEM_stime)
        countmap[2][0].append(tree.rh_EEM_count)
        mtimemap[2][1].append(tree.rh1GeV_EEM_mtime)
        summap[2][1].append(tree.rh1GeV_EEM_stime)
        countmap[2][1].append(tree.rh1GeV_EEM_count)
        mtimemap[2][2].append(tree.rh2GeV_EEM_mtime)
        summap[2][2].append(tree.rh2GeV_EEM_stime)
        countmap[2][2].append(tree.rh2GeV_EEM_count)
        mtimemap[2][3].append(tree.rh5GeV_EEM_mtime)
        summap[2][3].append(tree.rh5GeV_EEM_stime)
        countmap[2][3].append(tree.rh5GeV_EEM_count)
        mtimemap[2][4].append(tree.rh10GeV_EEM_mtime)
        summap[2][4].append(tree.rh10GeV_EEM_stime)
        countmap[2][4].append(tree.rh10GeV_EEM_count)
        mtimemap[2][5].append(tree.rh20GeV_EEM_mtime)
        summap[2][5].append(tree.rh20GeV_EEM_stime)
        countmap[2][5].append(tree.rh20GeV_EEM_count)


	#print( 'Filling Lists')
  	for iloc, tiloc  in [ [0,'EB'],[1,'EEP'],[2,'EEM']] :
        	for igev, tigev in [[0,''],[1,'1GeV'],[2,'2GeV'],[3,'5GeV'],[4,'10GeV'],[5,'20GeV']] :
			#print( 'Filling '+tiloc+' for '+tigev )
                        mtimelist[iloc][igev][0][bx].append([mtimemap[iloc][igev][0],countmap[iloc][igev][0]])
                        mtimelist[iloc][igev][1][train_position].append([mtimemap[iloc][igev][0],countmap[iloc][igev][0]])
                        mtimelist[iloc][igev][2][subtrain_position].append([mtimemap[iloc][igev][0],countmap[iloc][igev][0]])

	#   end of loop
#  print( 'Closing Input File')
#  inTree.Close()

  print( "Initializing Histograms" )

  for iloc, tiloc  in [ [0,'EB'],[1,'EEP'],[2,'EEM']] :
	for ilen in xrange( trainLength ):
		histname = "MTimeLoc_"+tiloc+"_"+str(ilen)
		histtitle = "Mean BX Time[ns] for Train Position "+str(ilen)+" "+tiloc+"; Train Number; Time[ns]"
		MTimeLoc_hist[iloc][ilen].append(ROOT.TH1F(histname,histtitle,trainNumber,0,trainNumber))
        for igev, tigev in [[0,''],[1,'1GeV'],[2,'2GeV'],[3,'5GeV'],[4,'10GeV'],[5,'20GeV']] :
                for itype, runlen, titype in [ [0,bunchSlots,'BX'],[1,trainLength,'Train'],[2,subtrainLength,'SubTrain'] ]:  # 0 = bynchSlots, 1 = trainlength, 2 = subtrainlength
                        histname = "BXTime_rh_"+tiloc+"_"+tigev+"_"+titype
                        histtitle = "Mean BX Time[ns] by RecHit "+tiloc+" >"+tigev+"; "+titype+" Position; Time[ns]"
                        BXTime_rh_hist[iloc][igev][itype].append(ROOT.TH1F(histname,histtitle,runlen,0,runlen))
                        histname = "BXTime_ev_"+tiloc+"_"+tigev+"_"+titype
                        histtitle = "Mean BX Time[ns] by Event "+tiloc+" >"+tigev+"; "+titype+" Position; Time[ns]"
                        BXTime_ev_hist[iloc][igev][itype].append(ROOT.TH1F(histname,histtitle,runlen,0,runlen))
			histname = "BXTime_red_"+tiloc+"_"+tigev+"_"+titype
                        histtitle = "Mean BX Time[ns] by RecHit Reduced "+tiloc+" >"+tigev+"; "+titype+" Position; Time[ns]"
                        BXTime_red_hist[iloc][igev][itype].append(ROOT.TH1F(histname,histtitle,runlen,0,runlen))

  print( "Filling Histograms" )

  for iloc, tiloc  in [ [0,'EEB'],[1,'EEP'],[2,'EEM']] :
	for ilen in xrange( trainLength ):
		print( 'Filling '+tiloc+' for Position '+str(ilen) )
		for inum in xrange( trainNumber ):
			mtime = get_td_mean( mtime_tlist[iloc][inum][ilen])
			MTimeLoc_hist[iloc][ilen][0].SetBinContent( inum, mtime )
			MTimeLoc_hist[iloc][ilen][0].SetBinError( inum, get_td_error( mtime_tlist[iloc][inum][ilen], mtime) )
        for igev, tigev in [[0,'All'],[1,'1GeV'],[2,'2GeV'],[3,'5GeV'],[4,'10GeV'],[5,'20GeV']] :
                for itype, runlen, titype in [ [0,bunchSlots,'BX'],[1,trainLength,'Train'],[2,subtrainLength,'SubTrain'] ] :  # 0 = bynchSlots, 1 = trainlength, 2 = subtrainlength
			print( 'Filling '+tiloc+' for '+tigev+' in '+titype)
			for ibx in xrange(runlen):
				mtime_rh = get_rh_mean( mtimelist[iloc][igev][itype][ibx] )
                        	BXTime_rh_hist[iloc][igev][itype][0].SetBinContent( ibx, mtime_wc )
				BXTime_rh_hist[iloc][igev][itype][0].SetBinError( ibx, get_rh_error( sum2list[iloc][igev][itype][ibx], mtime_rh) )
				mtime_ev = get_ev_mean( mtimelist[iloc][igev][itype][ibx] )
                                BXTime_ev_hist[iloc][igev][itype][0].SetBinContent( ibx, mtime_nc )
                                BXTime_ev_hist[iloc][igev][itype][0].SetBinError( ibx, get_ev_error( sum2list[iloc][igev][itype][ibx], mtime_ev) )
				mtime_red = get_red_mean( mtimelist[iloc][igev][itype][ibx] )
				BXTime_red_hist[iloc][igev][itype][0].SetBinContent( ibx, mtime_red )
                                BXTime_red_hist[iloc][igev][itype][0].SetBinError( ibx, get_red_error( mtimelist[iloc][igev][itype][ibx], mtime_red) )


#.append(ROOT.TH1F('BXTime_'+tiloc+'_'+tigev+'_'+titype,"Mean BX Time[ns] "+tiloc+"; "+titype+" Position; Time[ns]",runlen,0,runlen))



  print( "Saving histograms" )
  tfile.cd()	

  #EB_BXTime_Train_0GeV.Write()
  for iloc, tiloc  in [ [0,'EEB'],[1,'EEP'],[2,'EEM']] :
	for ilen in xrange( trainLength ):
		MTimeLoc_hist[iloc][ilen][0].Write()
        for igev, tigev in [[0,''],[1,'1GeV'],[2,'2GeV'],[3,'5GeV'],[4,'10GeV'],[5,'20GeV']] :
                for itype, runlen, titype in [ [0,bunchSlots,'BX'],[1,trainLength,'Train'],[2,subtrainLength,'SubTrain'] ] :  # 0 = bynchSlots, 1 = trainlength, 2 = subtrainlength
                	BXTime_rh_hist[iloc][igev][itype][0].Write()
                        BXTime_ev_hist[iloc][igev][itype][0].Write()
			BXTime_red_hist[iloc][igev][itype][0].Write()

  print( 'Closing Input Tree' )
  inTree.Close()
  print( 'Closing Output File' )
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
#		return math.sqrt(_sum2 / _num - mean_ * mean_);
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
