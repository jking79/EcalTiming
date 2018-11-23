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

#rootfile = 'dump_LHCInfo_Event_2018-11-14_19_35_00.root'
#outfile = 'hist_LHCInfo_Event_2018-11-14_19_35_00.root'
#ftitle = 'Run Low Event Test '
 
rootfile = 'dump_LHCInfo_Event_320688_BO7.root'
outfile = 'hist_LHCInfo_Event_320688_BO7.root'
ftitle = 'Run 320688-BO7 '
 
#rootfile = 'dump_LHCInfo_Event_324998_A48.root'
#outfile = 'hist_LHCInfo_Event_324998_A48.root'
#ftitle = 'Run 324998-A48 '


showsqrt = False

# set to -1 for do all
#maxEvent = -1
maxEvent = 1000000

inTreeFile = filedir + rootfile

def get_mean( ilist ):

	tsum = 0.0
	num = len(ilist)
	if not num : return 0.0
	for entry in ilist:
		tsum = tsum + entry[0]
	return tsum/num

def get_error( ilist, mean ):

	tsum = 0
	num = len(ilist)
	if not num : return 0.0
	for entry in ilist:
                tsum = tsum + ((entry[0]-mean)*(entry[0]-mean))
	if( tsum < 0 ):
		if showsqrt : print( 'get_error tsum = '+str(tsum) )
		return 0.0
	return math.sqrt(tsum)/num

def get_expanded_mean( ilist ):

        tsum = 0.0
	tnum = 0
        num = len(ilist)
        if not num : return 0.0
        for entry in ilist:
		tnum = tnum + entry[1]
                tsum = tsum + entry[0]*entry[1]
	if not tnum : return 0.0
        return tsum/tnum

def get_expanded_error( ilist, mean ):

        tsum = 0
	tnum = 0
	tsq = 0
        num = len(ilist)
        if not num : return 0.0
        for entry in ilist:
		tnum = tnum + entry[1]
                tsum = tsum + entry[0]*entry[1]
		tsq = tsq + entry[2]
	res = tsq - (2*mean*tsum) + (tnum*mean*mean)
        if( res < 0 ):
                if showsqrt : print( 'get_expanded_error res = '+str(res) )
                return 0.0
	if not tnum : return 0.0
        return math.sqrt(res)/tnum

def get_sample_mean( ilist ):

        tsum = 0.0
	count = 0
	tnum = 0
        num = len(ilist)
        if not num : return 0.0
        for entry in ilist:
		count += 1
		if( not count%3 ) :
			tnum = tnum + entry[1]
                	tsum = tsum + entry[0]*entry[1]
	if not tnum : return 0.0
        return tsum/tnum

def get_red_mean( ilist, smean ):

        tsum = 0.0
	count = 0
	tnum = 0
        num = len(ilist)
        if not num : return 0.0
        for entry in ilist:
		count += 1
		if( count%3 ) :
                	tnum = tnum + entry[1]
                	tsum = tsum + (entry[0] - smean)*entry[1]
	if not tnum : return 0.0
        return tsum/tnum

def get_red_error( ilist, mean, smean ):

        tsum = 0
        count = 0
        tnum = 0
        tsq = 0
        num = len(ilist)
        if not num : return 0.0
        for entry in ilist:
		count += 1
                if( count%3 ) :
                	tnum = tnum + entry[1]
                	tsum = tsum + entry[0]*entry[1]
                	tsq = tsq + entry[2]
        res = tsq - (2*mean*tsum) + (tnum*mean*mean)
        if( res < 0 ):
                if showsqrt : print( 'get_expanded_error res = '+str(res) )
                return 0.0
        if not tnum : return 0.0
        return math.sqrt(res)/tnum

def get_red_error_old( ilist, mean, smean ):

        tsum = 0
	count = 0
        tnum = 0
        tsq = 0
        num = len(ilist)
        if not num : return 0.0
        for entry in ilist:
		count += 1
                if( count%3 ) :
                	tnum = tnum + entry[1]
                	tsum = tsum + entry[0]*entry[1]
			term = entry[2] - (2*smean*entry[0]*entry[1]) + (smean*smean*entry[1])
			if term < 0 : term = 0
                	tsq = tsq + term
        res = tsq - (2*mean*tsum) + (tnum*mean*mean)
        if( res < 0 ):
                if not showsqrt : 
			if ( res + 0.00001 ) < 0 : 
				print( 'get_red_error res = '+str(res)+' tnum '+str(tnum)+' tsum '+str(tsum)+' tsq '+str(tsq)+' smean '+str(mean) )
                return 0.0
	if not tnum : return 0.0
        return math.sqrt(res)/tnum


#td - rh - ev - red


def parseTree( inTreeFile ):

# create out file
  print( 'Creating Out File: '+ outfile)
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
  sum2list = []
  BXTime_rh_hist = []
  BXTime_ev_hist = []
  BXTime_red_hist = []
  mtime_tlist = []
  MTimeLoc_hist = []

  print( 'Initializing input varibles' )
  for iloc in [0,1,2] :
	mtimelist.append([])
	sum2list.append([])
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
		sum2list[iloc].append([])
		BXTime_rh_hist[iloc].append([])
		BXTime_ev_hist[iloc].append([])
		BXTime_red_hist[iloc].append([])
		for itype, runlen in [ [0,bunchSlots],[1,trainLength],[2,subtrainLength] ]:  
			mtimelist[iloc][igev].append([])
			sum2list[iloc][igev].append([])
			BXTime_rh_hist[iloc][igev].append([])
			BXTime_ev_hist[iloc][igev].append([])
			BXTime_red_hist[iloc][igev].append([])
  			for ibx in xrange(runlen):
				mtimelist[iloc][igev][itype].append([])
				sum2list[iloc][igev][itype].append([])

  histname = "RF_Diffrence"
  histtitle = ";"+ftitle+"   RF Phase Diffrence (vc1-vc2) [ps];Count"
  RFDif_Dist_hist = ROOT.TH1F(histname,histtitle,200,-10.0,10.0)
  histname = "RF_Averge"
  histtitle = ";"+ftitle+"   RF Phase Average ([vc1+vc2]/2) [ps];Count"
  RFAve_Dist_hist = ROOT.TH1F(histname,histtitle,200,-10.0,10.0)
  histname = "RF_Sum"
  histtitle = ";"+ftitle+"   RF Phase Sum (vc1+vc2) [ps];Count"
  RFSum_Dist_hist = ROOT.TH1F(histname,histtitle,200,-10.0,10.0)
  histname = "RF_Phase_BX"
  histtitle = ";"+ftitle+"   RF Phase Average ([vc1+vc2]/2) [ps]; BX"
  RFSum_Dist_hist = ROOT.TH1F(histname,histtitle,200,-10.0,10.0)

  
  print( 'Reading input tree' )
  for iEntry in xrange( nEntry ):

	mtimemap = []
        sum2map = []
        countmap = []

  	for iloc in [0,1,2] :
		mtimemap.append([])
		sum2map.append([])
		countmap.append([])
		mtime_tlist.append([])
        	for igev in [0,1,2,3,4,5] :
			mtimemap[iloc].append([])
			sum2map[iloc].append([])
			countmap[iloc].append([])

	count += 1
	if( maxEvent > 0 and count > maxEvent ) : break
        if( not count%10000 ): print( 'Processing Entry '+str(count)+' Event '+str(event)+' Run '+str(run)+' LumiSection '+str(lumiSection)+' Fill '+str(fillNumber) )

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

#	if 'None' in phasetype:  corr = 0
#	elif 'Ave' in phasetype: corr = ave_phase
#	elif 'Diff' in phasetype:  corr = dif_phase
#	elif 'Sum' in phasetype:  corr = (2*vc2_phase) + dif_phase			
#	# 'None', 'Ave', 'Diff', 'Sum'
#
#	if 'Less' in phmultiplier : 
#		corr *= -1 	
#		corrstr = '( '+phmultiplier+' '+phasetype+' )'
#	else : corrstr = ''		
#	# 'Add', 'Less'
#	#print( 'Correction: '+str(corr))
	RFDif_Dist_hist.Fill( dif_phase )
	RFAve_Dist_hist.Fill( ave_phase )
	RFSum_Dist_hist.Fill( dif_phase + 2*vc2_phase )

	# EB
	mtime_tlist[0][train_number][train_position].append([tree.rh_EB_mtime,tree.rh_EB_count])
	#print( 'Mtime: '+str(tree.rh_EB_mtime)+' Corrected: '+str(tree.rh_EB_mtime))
	mtimemap[0][0].append(tree.rh_EB_mtime)
	sum2map[0][0].append(tree.rh_EB_s2time)
	countmap[0][0].append(tree.rh_EB_count)
        mtimemap[0][1].append(tree.rh1GeV_EB_mtime)
        sum2map[0][1].append(tree.rh1GeV_EB_s2time)
        countmap[0][1].append(tree.rh1GeV_EB_count)
        mtimemap[0][2].append(tree.rh2GeV_EB_mtime)
        sum2map[0][2].append(tree.rh2GeV_EB_s2time)
        countmap[0][2].append(tree.rh2GeV_EB_count)
        mtimemap[0][3].append(tree.rh5GeV_EB_mtime)
        sum2map[0][3].append(tree.rh5GeV_EB_s2time)
        countmap[0][3].append(tree.rh5GeV_EB_count)
        mtimemap[0][4].append(tree.rh10GeV_EB_mtime)
        sum2map[0][4].append(tree.rh10GeV_EB_s2time)
        countmap[0][4].append(tree.rh10GeV_EB_count)
        mtimemap[0][5].append(tree.rh20GeV_EB_mtime)
        sum2map[0][5].append(tree.rh20GeV_EB_s2time)
        countmap[0][5].append(tree.rh20GeV_EB_count)

        # EEP
	mtime_tlist[1][train_number][train_position].append([tree.rh_EEM_mtime,tree.rh_EEP_count])
        mtimemap[1][0].append(tree.rh_EEP_mtime)
        sum2map[1][0].append(tree.rh_EEP_s2time)
        countmap[1][0].append(tree.rh_EEP_count)
        mtimemap[1][1].append(tree.rh1GeV_EEP_mtime)
        sum2map[1][1].append(tree.rh1GeV_EEP_s2time)
        countmap[1][1].append(tree.rh1GeV_EEP_count)
        mtimemap[1][2].append(tree.rh2GeV_EEP_mtime)
        sum2map[1][2].append(tree.rh2GeV_EEP_s2time)
        countmap[1][2].append(tree.rh2GeV_EEP_count)
        mtimemap[1][3].append(tree.rh5GeV_EEP_mtime)
        sum2map[1][3].append(tree.rh5GeV_EEP_s2time)
        countmap[1][3].append(tree.rh5GeV_EEP_count)
        mtimemap[1][4].append(tree.rh10GeV_EEP_mtime)
        sum2map[1][4].append(tree.rh10GeV_EEP_s2time)
        countmap[1][4].append(tree.rh10GeV_EEP_count)
        mtimemap[1][5].append(tree.rh20GeV_EEP_mtime)
        sum2map[1][5].append(tree.rh20GeV_EEP_s2time)
        countmap[1][5].append(tree.rh20GeV_EEP_count)

        # EEM
	mtime_tlist[2][train_number][train_position].append([tree.rh_EEM_mtime,tree.rh_EEM_count])
        mtimemap[2][0].append(tree.rh_EEM_mtime)
        sum2map[2][0].append(tree.rh_EEM_s2time)
        countmap[2][0].append(tree.rh_EEM_count)
        mtimemap[2][1].append(tree.rh1GeV_EEM_mtime)
        sum2map[2][1].append(tree.rh1GeV_EEM_s2time)
        countmap[2][1].append(tree.rh1GeV_EEM_count)
        mtimemap[2][2].append(tree.rh2GeV_EEM_mtime)
        sum2map[2][2].append(tree.rh2GeV_EEM_s2time)
        countmap[2][2].append(tree.rh2GeV_EEM_count)
        mtimemap[2][3].append(tree.rh5GeV_EEM_mtime)
        sum2map[2][3].append(tree.rh5GeV_EEM_s2time)
        countmap[2][3].append(tree.rh5GeV_EEM_count)
        mtimemap[2][4].append(tree.rh10GeV_EEM_mtime)
        sum2map[2][4].append(tree.rh10GeV_EEM_s2time)
        countmap[2][4].append(tree.rh10GeV_EEM_count)
        mtimemap[2][5].append(tree.rh20GeV_EEM_mtime)
        sum2map[2][5].append(tree.rh20GeV_EEM_s2time)
        countmap[2][5].append(tree.rh20GeV_EEM_count)


	#print( 'Filling Lists')
  	for iloc, tiloc  in [ [0,'EB'],[1,'EEP'],[2,'EEM']] :
        	for igev, tigev in [[0,''],[1,'1GeV'],[2,'2GeV'],[3,'5GeV'],[4,'10GeV'],[5,'20GeV']] :
			#print( 'Filling '+tiloc+' for '+tigev )
                        mtimelist[iloc][igev][0][bx].append([mtimemap[iloc][igev][0],countmap[iloc][igev][0],sum2map[iloc][igev][0]])
                        mtimelist[iloc][igev][1][train_position].append([mtimemap[iloc][igev][0],countmap[iloc][igev][0],sum2map[iloc][igev][0]])
                        mtimelist[iloc][igev][2][subtrain_position].append([mtimemap[iloc][igev][0],countmap[iloc][igev][0],sum2map[iloc][igev][0]])

	#   end of loop
#  print( 'Closing Input File')
#  inTree.Close()

  print( "Initializing Histograms" )

  for iloc, tiloc  in [ [0,'EB'],[1,'EEP'],[2,'EEM']] :
	for ilen in xrange( trainLength ):
		histname = "MTimeLoc_"+tiloc+"_"+str(ilen)
		histtitle = "Mean BX Time[ns] for Train Position "+str(ilen)+" "+tiloc+";"+ftitle+"     Train Number; "+corrstr+" Time[ns]"
		MTimeLoc_hist[iloc][ilen].append(ROOT.TH1F(histname,histtitle,trainNumber,0,trainNumber))
        for igev, tigev in [[0,''],[1,'1GeV'],[2,'2GeV'],[3,'5GeV'],[4,'10GeV'],[5,'20GeV']] :
                for itype, runlen, titype in [ [0,bunchSlots,'BX'],[1,trainLength,'Train'],[2,subtrainLength,'SubTrain'] ]:  # 0 = bynchSlots, 1 = trainlength, 2 = subtrainlength
                        histname = "BXTime_rh_"+tiloc+"_"+tigev+"_"+titype
                        histtitle = "Mean BX Time[ns] by RecHit: "+tiloc+"  ( >"+tigev+");"+ftitle+"    "+titype+" Position; "+corrstr+" Time[ns]"
                        BXTime_rh_hist[iloc][igev][itype].append(ROOT.TH1F(histname,histtitle,runlen,0,runlen))
                        histname = "BXTime_ev_"+tiloc+"_"+tigev+"_"+titype
                        histtitle = "Mean BX Time[ns] by Event: "+tiloc+"  ( >"+tigev+");"+ftitle+"    "+titype+" Position; "+corrstr+" Time[ns]"
                        BXTime_ev_hist[iloc][igev][itype].append(ROOT.TH1F(histname,histtitle,runlen,0,runlen))
			histname = "BXTime_red_"+tiloc+"_"+tigev+"_"+titype
                        histtitle = "Mean BX Time[ns] by RecHit Conditioned: "+tiloc+"  ( >"+tigev+");"+ftitle+"    "+titype+" Position; "+corrstr+" Time[ns]"
                        BXTime_red_hist[iloc][igev][itype].append(ROOT.TH1F(histname,histtitle,runlen,0,runlen))

  print( "Filling Histograms" )

  for iloc, tiloc  in [ [0,'EB'],[1,'EEP'],[2,'EEM']] :
	print( 'Filling '+tiloc )
	for ilen in xrange( trainLength ):
		#print( 'Filling '+tiloc+' for Position '+str(ilen) )
		for inum in xrange( trainNumber ):
			mtime = get_mean( mtime_tlist[iloc][inum][ilen])
			MTimeLoc_hist[iloc][ilen][0].SetBinContent( inum, mtime )
			MTimeLoc_hist[iloc][ilen][0].SetBinError( inum, get_error( mtime_tlist[iloc][inum][ilen], mtime) )
        for igev, tigev in [[0,'All'],[1,'1GeV'],[2,'2GeV'],[3,'5GeV'],[4,'10GeV'],[5,'20GeV']] :
                #print( 'Filling '+tiloc+' for '+tigev )
                for itype, runlen, titype in [ [0,bunchSlots,'BX'],[1,trainLength,'Train'],[2,subtrainLength,'SubTrain'] ] :  # 0 = bynchSlots, 1 = trainlength, 2 = subtrainlength
			#print( 'Filling '+tiloc+' for '+tigev+' in '+titype)
			for ibx in xrange(runlen):
				mtime_rh = get_expanded_mean( mtimelist[iloc][igev][itype][ibx] )
                        	BXTime_rh_hist[iloc][igev][itype][0].SetBinContent( ibx, mtime_rh )
				BXTime_rh_hist[iloc][igev][itype][0].SetBinError( ibx, get_expanded_error( mtimelist[iloc][igev][itype][ibx], mtime_rh) )
				mtime_ev = get_mean( mtimelist[iloc][igev][itype][ibx] )
                                BXTime_ev_hist[iloc][igev][itype][0].SetBinContent( ibx, mtime_ev )
                                BXTime_ev_hist[iloc][igev][itype][0].SetBinError( ibx, get_error( mtimelist[iloc][igev][itype][ibx], mtime_ev) )
				mtime_sample = get_sample_mean( mtimelist[iloc][igev][itype][ibx] )
				mtime_red = get_red_mean( mtimelist[iloc][igev][itype][ibx], mtime_sample )
				BXTime_red_hist[iloc][igev][itype][0].SetBinContent( ibx, mtime_red )
                                BXTime_red_hist[iloc][igev][itype][0].SetBinError( ibx, get_red_error( mtimelist[iloc][igev][itype][ibx], mtime_red, mtime_sample) )


#.append(ROOT.TH1F('BXTime_'+tiloc+'_'+tigev+'_'+titype,"Mean BX Time[ns] "+tiloc+"; "+titype+" Position; Time[ns]",runlen,0,runlen))



  print( "Saving histograms" )
  tfile.cd()	


  RF_Dif_hist.Write()
  RF_Ave_hist.Write()
  RF_Sum_hist.Write()

  #EB_BXTime_Train_0GeV.Write()

  for iloc, tiloc  in [ [0,'EB'],[1,'EEP'],[2,'EEM']] :
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



