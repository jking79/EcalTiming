import os, re, sys, optparse, commands, shutil, imp
import math, ROOT

infolder = ''
prehistfile =  '/afs/cern.ch/user/j/jaking/private/ecal/CMSSW_10_1_7/src/EcalTiming/EcalTiming/lxbatch/320688_Calib/output/ecalTiming.root'
filedir = '/afs/cern.ch/work/j/jaking/private/ecal/CMSSW_10_3_0_pre2/src/EcalTiming/EcalTiming/'

bunchSlots = 3564
bigBXSlots = 356
trainLength = 150
subtrainLength = 50
trainNumber = 25
nSigma = 2.0
maxRange = 10

ftitle = 'Run 320688'
rootfile = 'lxbatch/320688/dump2_LHCInfo_320668_D.root'
outfile = 'lxbatch/320688/hist_LHCInfo_3gev_Run320688_D.root'
#rootfile = 'lxbatch/320688/JOB_1/dump_LHCInfo_Event_2018-11-21_21_35_39.root'
#outfile = 'lxbatch/320688/JOB_1/hist_LHCInfo_2SigmaEvent_Run320688_J1.root'
#rootfile = 'lxbatch/320688/JOB_2/dump_LHCInfo_Event_2018-11-21_21_33_30.root'
#outfile = 'lxbatch/320688/JOB_2/hist_LHCInfo_2SigmaEvent_Run320688_J2.root'
#rootfile = 'lxbatch/320688/JOB_3/dump_LHCInfo_Event_2018-11-21_21_36_35.root'
#outfile = 'lxbatch/320688/JOB_3/hist_LHCInfo_2SigmaEvent_Run320688_J3.root'
#rootfile = 'lxbatch/320688/JOB_4/dump_LHCInfo_Event_2018-11-21_21_34_26.root'
#outfile = 'lxbatch/320688/JOB_4/hist_LHCInfo_2SigmaEvent_Run320688_J4.root'
#rootfile = 'lxbatch/320688/JOB_5/dump_LHCInfo_Event_2018-11-21_21_33_52.root'
#outfile = 'lxbatch/320688/JOB_5/hist_LHCInfo_2SigmaEvent_Run320688_J5.root'
#rootfile = 'lxbatch/320688/JOB_6/dump_LHCInfo_Event_2018-11-21_21_35_44.root'
#outfile = 'lxbatch/320688/JOB_6/hist_LHCInfo_2SigmaEvent_Run320688_J6.root'
#rootfile = 'lxbatch/320688/JOB_7/dump_LHCInfo_Event_2018-11-21_21_36_14.root'
#outfile = 'lxbatch/320688/JOB_7/hist_LHCInfo_2SigmaEvent_Run320688_J7.root'
#rootfile = 'lxbatch/320688/JOB_8/dump_LHCInfo_Event_2018-11-21_21_33_12.root'
#outfile = 'lxbatch/320688/JOB_8/hist_LHCInfo_2SigmaEvent_Run320688_J8.root'
#rootfile = 'lxbatch/320688/JOB_9/dump_LHCInfo_Event_2018-11-21_21_36_24.root'
#outfile = 'lxbatch/320688/JOB_9/hist_LHCInfo_2SigmaEvent_Run320688_J9.root'
#rootfile = 'lxbatch/320688/JOB_10/dump_LHCInfo_Event_2018-11-21_21_36_32.root'
#outfile = 'lxbatch/320688/JOB_10/hist_LHCInfo_2SigmaEvent_Run320688_J10.root'
#rootfile = 'lxbatch/320688/JOB_11/dump_LHCInfo_Event_2018-11-21_21_33_43.root'
#outfile = 'lxbatch/320688/JOB_11/hist_LHCInfo_2SigmaEvent_Run320688_J11.root'
#rootfile = 'lxbatch/320688/JOB_12/dump_LHCInfo_Event_2018-11-21_21_52_42.root'
#outfile = 'lxbatch/320688/JOB_12/hist_LHCInfo_2SigmaEvent_Run320688_J12.root'

#rootfile = 'rechit_ntuples/dump_LHCInfo_Event_320688_BO7.root'
#outfile = 'hist_LHCInfo_Event_320688_BO7.root'
#ftitle = 'Run 320688-BO7 '
 
#rootfile = 'dump_LHCInfo_Event_324998_A48.root'
#outfile = 'hist_LHCInfo_Event_324998_A48.root'
#ftitle = 'Run 324998-A48 '

corrstr = ''

showsqrt = False

# set to -1 for do all
maxEvent = -1
#maxEvent = 1000

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
#		if showsqrt : print( 'get_error tsum = '+str(tsum) )
		return 0.0
	return math.sqrt(tsum)/num

def get_stdev( ilist, mean ):
 
        tsum = 0
        num = len(ilist)
        if not num : return 0.0
        for entry in ilist:
                tsum = tsum + ((entry[0]-mean)*(entry[0]-mean))
        if( tsum < 0 ):
#               if showsqrt : print( 'get_error tsum = '+str(tsum) )
                return 0.0
        return math.sqrt(tsum/num)

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
 #               if showsqrt : print( 'get_expanded_error res = '+str(res) )
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

def get_sample_error( ilist, mean ):

        tsum = 0
        tnum = 0
        tsq = 0
	count = 0
        num = len(ilist)
        if not num : return 0.0
        for entry in ilist:
		count += 1
                if( not count%3 ) :
                	tnum = tnum + entry[1]
                	tsum = tsum + entry[0]*entry[1]
                	tsq = tsq + entry[2]
        res = tsq - (2*mean*tsum) + (tnum*mean*mean)
        if( res < 0 ):
 #               if showsqrt : print( 'get_expanded_error res = '+str(res) )
                return 0.0
        if not tnum : return 0.0
        return math.sqrt(res)/tnum

def get_body_mean( ilist ):

        tsum = 0.0
        count = 0
        tnum = 0
        num = len(ilist)
        if not num : return 0.0
        for entry in ilist:
                count += 1
                if( count%3 ) :
                        tnum = tnum + entry[1]
                        tsum = tsum + entry[0]*entry[1]
        if not tnum : return 0.0
        return tsum/tnum

def get_body_error( ilist, mean ):

        tsum = 0
        tnum = 0
        tsq = 0
        count = 0
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
 #               if showsqrt : print( 'get_expanded_error res = '+str(res) )
                return 0.0
        if not tnum : return 0.0
        return math.sqrt(res)/tnum

def get_cor_mean( ilist, clist ):

        tsum = 0.0
        count = 0
        tnum = 0
        num = len(ilist)
        if not num : return 0.0
        for entry, corr in zip( ilist, clist) :
                count += 1
                if( count%3 ) :
                        tnum = tnum + entry[1]
                        tsum = tsum + (entry[0] + corr[0])*entry[1]
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

def get_red_error( ilist, mean ):

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
#                if showsqrt : print( 'get_expanded_error res = '+str(res) )
                return 0.0
        if not tnum : return 0.0
        return math.sqrt(res)/tnum

def get_2sig_list( ilist ):

	retlist = []

	mean = get_mean( ilist )
	stdev = get_stdev( ilist, mean )
	
	for entry in ilist:
		if( entry[0] > (mean - 2*stdev) ):
			if( entry[0] < (mean + 2*stdev) ):
				retlist.append( entry )

	return retlist

def get_2sig_plist( ilist , plist):

        retlist = []

        mean = get_mean( ilist )
        stdev = get_stdev( ilist, mean )

        for entry, phase in zip( ilist, plist ):
                if( entry[0] > (mean - 2*stdev) ):
                        if( entry[0] < (mean + 2*stdev) ):
                                retlist.append( phase )

        return retlist

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
  phaselist = []
  sum2list = []

  BXTime_pc_hist = []
  BXTime_rh_hist = []
  BXTime_cor_hist = []
  BXTime_red_hist = []
  BXTime_sam_hist = []
  BXTime_bod_hist = []

  mtime_tlist = []
  MTimeLoc_hist = []

  print( 'Initializing input varibles' )
  for itype, runlen in [ [0,bunchSlots],[1,trainLength],[2,subtrainLength],[3,bigBXSlots] ]:
        phaselist.append([])
        for ibx in xrange(runlen):
                phaselist[itype].append([])

  for iloc in [0] : #[0,1,2] :
	mtimelist.append([])
 	phaselist.append([])
	sum2list.append([])
  	mtime_tlist.append([])
        BXTime_pc_hist.append([])
	BXTime_rh_hist.append([])
	BXTime_cor_hist.append([])
	BXTime_red_hist.append([])
        BXTime_sam_hist.append([])
        BXTime_bod_hist.append([])
	MTimeLoc_hist.append([])
	for ilen in xrange( trainLength ):
		MTimeLoc_hist[iloc].append([])
        for inum in xrange( trainNumber ):
        	mtime_tlist[iloc].append([])
                for ipos in xrange( trainLength ):
                	mtime_tlist[iloc][inum].append([])
	for igev in [0]: #[0,1,2,3,4,5] :
		mtimelist[iloc].append([])
		sum2list[iloc].append([])
                BXTime_pc_hist[iloc].append([])
		BXTime_rh_hist[iloc].append([])
		BXTime_cor_hist[iloc].append([])
		BXTime_red_hist[iloc].append([])
                BXTime_sam_hist[iloc].append([])
                BXTime_bod_hist[iloc].append([])
		for itype, runlen in [ [0,bunchSlots],[1,trainLength],[2,subtrainLength],[3,bigBXSlots] ]:  
			mtimelist[iloc][igev].append([])
			sum2list[iloc][igev].append([])
                        BXTime_pc_hist[iloc][igev].append([])
			BXTime_rh_hist[iloc][igev].append([])
			BXTime_cor_hist[iloc][igev].append([])
			BXTime_red_hist[iloc][igev].append([])
                        BXTime_sam_hist[iloc][igev].append([])
                        BXTime_bod_hist[iloc][igev].append([])
  			for ibx in xrange(runlen):
				mtimelist[iloc][igev][itype].append([])
				sum2list[iloc][igev][itype].append([])

  histname = "RF_Diffrence"
  histtitle = ";"+ftitle+"   RF Phase Diffrence (vc1-vc2) [ps];Events / 0.1 ps"
  RFDif_Dist_hist = ROOT.TH1F(histname,histtitle,200,-10.0,10.0)
  histname = "RF_Averge"
  histtitle = ";"+ftitle+"   RF Phase Average ([vc1+vc2]/2) [ps];Events / 0.1 ps"
  RFAve_Dist_hist = ROOT.TH1F(histname,histtitle,200,-10.0,10.0)
  histname = "RF_Sum"
  histtitle = ";"+ftitle+"   RF Phase Sum (vc1+vc2) [ps];Events / 0.1 ps"
  RFSum_Dist_hist = ROOT.TH1F(histname,histtitle,200,-10.0,10.0)
#  histname = "RF_Phase_BX"
#  histtitle = ";"+ftitle+"   RF Phase Average ([vc1+vc2]/2) [ps];Events / 0.1 ps"
#  RFSum_Dist_hist = ROOT.TH1F(histname,histtitle,200,-10.0,10.0)

  
  print( 'Reading input tree' )
  for iEntry in xrange( nEntry ):

	mtimemap = []
        sum2map = []
        countmap = []

  	for iloc in [0]: #[0,1,2] :
		mtimemap.append([])
		sum2map.append([])
		countmap.append([])
		mtime_tlist.append([])
        	for igev in [0]: #[0,1,2,3,4,5] :
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
	
	big_bx = int(bx/10)

	fillNumber = tree.fillNumber
	lumiSection = tree.lumiSection

	ave_phase = tree.ave_phase
        dif_phase = tree.dif_phase
        vc2_phase = tree.vc2_phase
	sum_phase = tree.ave_phase*2   #dif_phase + 2*vc2_phase

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
	RFDif_Dist_hist.Fill( (dif_phase*1000) )
	RFAve_Dist_hist.Fill( (ave_phase*1000) )
	RFSum_Dist_hist.Fill( (sum_phase*1000) )

	# EB
	mtime_tlist[0][train_number][train_position].append([tree.rh_EB_mtime,tree.rh_EB_count])
	#print( 'Mtime: '+str(tree.rh_EB_mtime)+' Corrected: '+str(tree.rh_EB_mtime))
	mtimemap[0][0].append(tree.rh3GeV_EB_mtime)
	sum2map[0][0].append(tree.rh3GeV_EB_s2time)
	countmap[0][0].append(tree.rh3GeV_EB_count)


	#print( 'Filling Lists')
        phaselist[0][bx].append([ave_phase])
        phaselist[1][train_position].append([ave_phase])
        phaselist[2][subtrain_position].append([ave_phase])
	phaselist[3][big_bx].append([sum_phase])
  	for iloc, tiloc  in [ [0,'EB'] ]: #,[1,'EEP'],[2,'EEM']] :
        	for igev, tigev in [[0,'3GeV']]: #[[0,''],[1,'1GeV'],[2,'2GeV'],[3,'5GeV'],[4,'10GeV'],[5,'20GeV']] :
			#print( 'Filling '+tiloc+' for '+tigev )
                        mtimelist[iloc][igev][0][bx].append([mtimemap[iloc][igev][0],countmap[iloc][igev][0],sum2map[iloc][igev][0]])
                        mtimelist[iloc][igev][1][train_position].append([mtimemap[iloc][igev][0],countmap[iloc][igev][0],sum2map[iloc][igev][0]])
                        mtimelist[iloc][igev][2][subtrain_position].append([mtimemap[iloc][igev][0],countmap[iloc][igev][0],sum2map[iloc][igev][0]])
                        mtimelist[iloc][igev][3][big_bx].append([mtimemap[iloc][igev][0],countmap[iloc][igev][0],sum2map[iloc][igev][0]])

	#   end of loop
#  print( 'Closing Input File')
#  inTree.Close()

  print( "Initializing Histograms" )

  for iloc, tiloc  in [ [0,'EB'] ]: #,[1,'EEP'],[2,'EEM']] :
	for ilen in xrange( trainLength ):
		histname = "MTimeLoc_"+tiloc+"_"+str(ilen)
		histtitle = "Train Position: "+str(ilen)+"   "+tiloc+";"+ftitle+"     Train Number; "+corrstr+" Mean BX Time[ns]"
		MTimeLoc_hist[iloc][ilen].append(ROOT.TH1F(histname,histtitle,trainNumber,0,trainNumber))
        for igev, tigev in [[0,'3GeV']]: #[[0,'0gev'],[1,'1gev'],[2,'2gev'],[3,'5gev'],[4,'10gev'],[5,'20gev']] :
                for itype, runlen, titype in [ [0,bunchSlots,'BX'],[1,trainLength,'Train'],[2,subtrainLength,'Subtrain'],[3,bigBXSlots,'BX/10'] ]: 
                        histname = "BXTime_pc_"+tiloc+"_"+tigev+"_"+titype
                        histtitle = "rf phase correction[ns] "+tiloc+"  ( >"+tigev+");"+ftitle+"    "+titype+" position; RF Phase Correction ([vc1+vc2]*[2.5/360]) [ns]"
                        BXTime_pc_hist[iloc][igev][itype].append(ROOT.TH1F(histname,histtitle,runlen,0,runlen))
                        histname = "BXTime_rh_"+tiloc+"_"+tigev+"_"+titype
                        histtitle = "mean bx time[ns] "+tiloc+"  ( >"+tigev+");"+ftitle+"    "+titype+" position; Mean BX Time[ns]"
                        BXTime_rh_hist[iloc][igev][itype].append(ROOT.TH1F(histname,histtitle,runlen,0,runlen))
                        histname = "BXTime_cor_"+tiloc+"_"+tigev+"_"+titype
                        histtitle = "mean bx time[ns] "+tiloc+"  ( >"+tigev+");"+ftitle+"    "+titype+" position; Mean BX Time + RF Phase Correction ([vc1+vc2]*[2.5/360]) [ns]"
                        BXTime_cor_hist[iloc][igev][itype].append(ROOT.TH1F(histname,histtitle,runlen,0,runlen))
			histname = "BXTime_red_"+tiloc+"_"+tigev+"_"+titype
                        histtitle = "mean conditioned bx time[ns] "+tiloc+"  ( >"+tigev+");"+ftitle+"    "+titype+" position; Corrected Mean BX Time[ns]"
                        BXTime_red_hist[iloc][igev][itype].append(ROOT.TH1F(histname,histtitle,runlen,0,runlen))
                        histname = "BXTime_sam_"+tiloc+"_"+tigev+"_"+titype
                        histtitle = "sample mean bx time[ns] "+tiloc+"  ( >"+tigev+");"+ftitle+"    "+titype+" position; Sample Mean BX Time[ns]"
                        BXTime_sam_hist[iloc][igev][itype].append(ROOT.TH1F(histname,histtitle,runlen,0,runlen))
                        histname = "BXTime_bod_"+tiloc+"_"+tigev+"_"+titype
                        histtitle = "data mean bx time[ns] "+tiloc+"  ( >"+tigev+");"+ftitle+"    "+titype+" position; Data Mean BX Time[ns]"
                        BXTime_bod_hist[iloc][igev][itype].append(ROOT.TH1F(histname,histtitle,runlen,0,runlen))

  print( "filling histograms" )

  for iloc, tiloc  in [ [0,'EB'] ]: #,[1,'eep'],[2,'eem']] :
	print( 'filling '+tiloc )
	for ilen in xrange( trainLength ):
		#print( 'filling '+tiloc+' for position '+str(ilen) )
		for inum in xrange( trainNumber ):
			input_list = get_2sig_list(mtime_tlist[iloc][inum][ilen])
			mtime = get_mean( input_list )
			MTimeLoc_hist[iloc][ilen][0].SetBinContent( inum, mtime )
			MTimeLoc_hist[iloc][ilen][0].SetBinError( inum, get_error( input_list, mtime) )
        for igev, tigev in [[0,'3GeV']]: #[[0,'all'],[1,'1gev'],[2,'2gev'],[3,'5gev'],[4,'10gev'],[5,'20gev']] :
                #print( 'filling '+tiloc+' for '+tigev )
                for itype, runlen, titype in [ [0,bunchSlots,'bx'],[1,trainLength,'train'],[2,subtrainLength,'subtrain'],[3,bigBXSlots,'BX/10'] ]: 
			#print( 'filling '+tiloc+' for '+tigev+' in '+titype)
			for ibx in xrange(runlen):
                        	input_list = get_2sig_list(mtimelist[iloc][igev][itype][ibx])
				phase_list = get_2sig_plist(mtimelist[iloc][igev][itype][ibx], phaselist[itype][ibx])
				#print( 'phase: '+str(phaselist[itype]))
				mtime_ph = get_mean(phaselist[itype][ibx])
				error_ph = get_error( phaselist[itype][ibx], mtime_ph )
                                BXTime_pc_hist[iloc][igev][itype][0].SetBinContent( ibx, mtime_ph )
				BXTime_pc_hist[iloc][igev][itype][0].SetBinError( ibx, error_ph )
				mtime_rh = get_expanded_mean( input_list )
				error_rh = get_expanded_error( input_list, mtime_rh )
                        	BXTime_rh_hist[iloc][igev][itype][0].SetBinContent( ibx, mtime_rh )
				BXTime_rh_hist[iloc][igev][itype][0].SetBinError( ibx, error_rh )
                                mtime_cor = get_cor_mean( input_list, phase_list )
                                BXTime_cor_hist[iloc][igev][itype][0].SetBinContent( ibx, mtime_cor )
                                BXTime_cor_hist[iloc][igev][itype][0].SetBinError( ibx, error_rh )
				mtime_sample = get_sample_mean( input_list )
				error_sample = get_sample_error( input_list, mtime_sample )
                                mtime_red = get_red_mean( input_list, mtime_sample )
                                error_red = get_red_error( input_list, mtime_red )
				BXTime_red_hist[iloc][igev][itype][0].SetBinContent( ibx, mtime_red )
                                BXTime_red_hist[iloc][igev][itype][0].SetBinError( ibx, error_red )
                                BXTime_sam_hist[iloc][igev][itype][0].SetBinContent( ibx, mtime_sample )
                                BXTime_sam_hist[iloc][igev][itype][0].SetBinError( ibx, error_sample )
                                mtime_bod = get_body_mean( input_list )
                                error_bod = get_body_error( input_list, mtime_bod )
                                BXTime_bod_hist[iloc][igev][itype][0].SetBinContent( ibx, mtime_bod )
                                BXTime_bod_hist[iloc][igev][itype][0].SetBinError( ibx, error_bod )



  print( "saving histograms" )
  tfile.cd()	

  RFDif_Dist_hist.Write()
  RFAve_Dist_hist.Write()
  RFSum_Dist_hist.Write()

  #eb_BXTime_train_0gev.Write()

  for iloc, tiloc  in [ [0,'eb'] ]: #,[1,'eep'],[2,'eem']] :
	for ilen in xrange( trainLength ):
		MTimeLoc_hist[iloc][ilen][0].Write()
        for igev, tigev in [[0,'3GeV']]: #[[0,''],[1,'1gev'],[2,'2gev'],[3,'5gev'],[4,'10gev'],[5,'20gev']] :
                for itype, runlen, titype in [ [0,bunchSlots,'bx'],[1,trainLength,'train'],[2,subtrainLength,'subtrain'],[3,bigBXSlots,'BX/10'] ] : 
                	BXTime_rh_hist[iloc][igev][itype][0].Write()
                        BXTime_cor_hist[iloc][igev][itype][0].Write()
			BXTime_red_hist[iloc][igev][itype][0].Write()
                        BXTime_sam_hist[iloc][igev][itype][0].Write()
                        BXTime_pc_hist[iloc][igev][itype][0].Write()
                        BXTime_bod_hist[iloc][igev][itype][0].Write()

  print( 'closing input tree' )
  inTree.Close()
  print( 'closing output file' )
  tfile.Close()

# end of function

parseTree( inTreeFile )



