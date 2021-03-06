#ifndef __ACDB_DATAINFO_H__
#define __ACDB_DATAINFO_H__
/*===========================================================================
    @file   acdb_datainfo.h

    The interface of the Acdb Parse project.

    This file will handle the parsing of an ACDB file. It will issue callbacks
    when encountering useful client information in the ACDB file.

                    Copyright (c) 2012-2013 Qualcomm Technologies, Inc.
                    All Rights Reserved.
                    Qualcomm Technologies Proprietary and Confidential.
========================================================================== */
/* $Header: //source/qcom/qct/multimedia2/Audio/audcal4/acdb_sw/main/latest/acdb/src/acdb_datainfo.h#2 $ */

/*===========================================================================
    EDIT HISTORY FOR MODULE

    This section contains comments describing changes made to the module.
    Notice that changes are listed in reverse chronological order. Please
    use ISO format for dates.

    when        who     what, where, why
    ----------  ---     -----------------------------------------------------
    2013-07-18  mh      Added new device property - AV Sync Delay
    2013-05-23  avi     Support Voice Volume boost feature (VocProcVolV2 tbl)

===========================================================================*/

/* ---------------------------------------------------------------------------
 * Include Files
 *--------------------------------------------------------------------------- */
#include "acdb_os_includes.h"

/* ---------------------------------------------------------------------------
 * Preprocessor Definitions and Constants
 *--------------------------------------------------------------------------- */
//Table properties info
#define AUDPROC_LUT_INDICES_COUNT 5
#define AUDPROC_CMD_INDICES_COUNT 3
#define AUDPROCTBL_INDICES_COUNT (AUDPROC_LUT_INDICES_COUNT-2)

#define AUDPROC_GAIN_DEP_LUT_INDICES_COUNT 5
#define AUDPROC_GAIN_DEP_CMD_INDICES_COUNT 3
#define AUDPROCTBL_GAIN_DEP_INDICES_COUNT (AUDPROC_GAIN_DEP_LUT_INDICES_COUNT-2)

#define AUDPROC_VOL_LUT_INDICES_COUNT 5
#define AUDPROC_VOL_CMD_INDICES_COUNT 3
#define AUDPROCTBL_VOL_INDICES_COUNT (AUDPROC_VOL_LUT_INDICES_COUNT-2)

#define AUDSTREAM_LUT_INDICES_COUNT 3
#define AUDSTREAM_CMD_INDICES_COUNT 1
#define AUDSTREAMTBL_INDICES_COUNT (AUDSTREAM_LUT_INDICES_COUNT-2)

#define VOCSTREAM_LUT_INDICES_COUNT 5
#define VOCSTREAM_CMD_INDICES_COUNT 0
#define VOCSTREAMTBL_INDICES_COUNT (VOCSTREAM_LUT_INDICES_COUNT-2)

#define VOCPROC_LUT_INDICES_COUNT 9
#define VOCPROC_CMD_INDICES_COUNT 4
#define VOCPROCTBL_INDICES_COUNT (VOCPROC_LUT_INDICES_COUNT-2)

#define VOCPROC_VOL_LUT_INDICES_COUNT 8
#define VOCPROC_VOL_CMD_INDICES_COUNT 2
#define VOCPROCTBL_VOL_INDICES_COUNT (VOCPROC_VOL_LUT_INDICES_COUNT-2)

#define ANC_LUT_INDICES_COUNT 3
#define ANC_CMD_INDICES_COUNT 2
#define ANCTBL_INDICES_COUNT (ANC_LUT_INDICES_COUNT-1)

#define ADIE_LUT_INDICES_COUNT 3
#define ADIE_CMD_INDICES_COUNT 2
#define ADIETBL_INDICES_COUNT (ADIE_LUT_INDICES_COUNT-1)

#define GLOBAL_LUT_INDICES_COUNT 3
#define GLOBAL_CMD_INDICES_COUNT 2
#define GLOBALTBL_INDICES_COUNT (GLOBAL_LUT_INDICES_COUNT-1)

#define AFE_LUT_INDICES_COUNT 4
#define AFE_CMD_INDICES_COUNT 2
#define AFETBL_INDICES_COUNT (AFE_LUT_INDICES_COUNT-2)

#define AFECMN_LUT_INDICES_COUNT 4
#define AFECMN_CMD_INDICES_COUNT 2
#define AFECMNTBL_INDICES_COUNT (AFECMN_LUT_INDICES_COUNT-2)

#define VOCPROCDEVCFG_LUT_INDICES_COUNT 4
#define VOCPROCDEVCFG_CMD_INDICES_COUNT 2
#define VOCPROCDEVCFGTBL_INDICES_COUNT (VOCPROCDEVCFG_LUT_INDICES_COUNT-2)

#define LSM_LUT_INDICES_COUNT 4
#define LSM_CMD_INDICES_COUNT 2
#define LSM_INDICES_COUNT (LSM_LUT_INDICES_COUNT-2)

#define CDC_FEATURES_DATA_LUT_INDICES_COUNT 3
#define CDC_FEATURES_DATA_CMD_INDICES_COUNT 2
#define CDC_FEATURES_DATA_INDICES_COUNT (CDC_FEATURES_DATA_LUT_INDICES_COUNT-1)

#define ADST_LUT_INDICES_COUNT 4
#define ADST_CMD_INDICES_COUNT 2
#define ADST_INDICES_COUNT (ADST_LUT_INDICES_COUNT-2)

#define AANC_CFG_LUT_INDICES_COUNT 3
#define AANC_CFG_CMD_INDICES_COUNT 1
#define AANC_CFG_TBL_INDICES_COUNT (AANC_CFG_LUT_INDICES_COUNT-2)

#define VOCPROC_VOL_V2_LUT_INDICES_COUNT 9
#define VOCPROC_VOL_V2_CMD_INDICES_COUNT 3
#define VOCPROCTBL_VOL_V2_INDICES_COUNT (VOCPROC_VOL_V2_LUT_INDICES_COUNT-2)

#define VOICE_VP3_LUT_INDICES_COUNT 4
#define VOICE_VP3_CMD_INDICES_COUNT 2
#define VOICE_VP3TBL_INDICES_COUNT (VOICE_VP3_LUT_INDICES_COUNT-2)

#define AUDREC_VP3_LUT_INDICES_COUNT 3
#define AUDREC_VP3_CMD_INDICES_COUNT 1
#define AUDREC_VP3TBL_INDICES_COUNT (AUDREC_VP3_LUT_INDICES_COUNT-2)

#define AUDREC_EC_VP3_LUT_INDICES_COUNT 4
#define AUDREC_EC_VP3_CMD_INDICES_COUNT 2
#define AUDREC_EC_VP3TBL_INDICES_COUNT (AUDREC_EC_VP3_LUT_INDICES_COUNT-2)

//Voc col info ids

#define ACDB_CAL_COLUMN_NETWORK					( 0x00011350 )
#define ACDB_CAL_NETWORK_ID_DUMMY_VALUE			( 0x0001135E )

#define ACDB_CAL_COLUMN_RX_SAMPLING_RATE		( 0x00011351 )
#define ACDB_CAL_RX_SAMPLING_ID_DUMMY_VALUE		( 0x00000000 )

#define ACDB_CAL_COLUMN_TX_SAMPLING_RATE		( 0x00011352 )
#define ACDB_CAL_TX_SAMPLING_ID_DUMMY_VALUE		( 0x00000000 )

#define ACDB_CAL_COLUMN_VOLUME_INDEX			( 0x00011358 )
#define ACDB_CAL_VOLUME_INDEX_ID_DUMMY_VALUE	( 0XFFFFFFFF )

#define ACDB_CAL_COLUMN_TYPE_32_BIT				( 0x0001135C )

/* ---------------------------------------------------------------------------
 * Type Declarations
 *--------------------------------------------------------------------------- */

// AV Acdb file Device dependent property PID's Info
static const uint32_t DEVPAIR = 0x000113A9;       //VocProc Device Pairs
static const uint32_t ANCDEVPAIR = 0x000113AC;       //ANC device pairs
static const uint32_t AUDPROC_CMN_TOPID = 0x000113AD;       //AudProc Common TopologyId list
static const uint32_t VOCPROC_CMN_TOPID = 0x000113AF;       //VocProc Common TopologyId list
static const uint32_t AFE_CMN_TOP_TABLE = 0x000113B0;       //Afe topology list
static const uint32_t RECORDED_DEVICEPAIR = 0x000113B2;    // Audio Rec EC Dev pairs
static const uint32_t APQ_MDM_COMP_DEVID = 0x000113B3;     // For APQ MDM dev pairs
static const uint32_t ISANC = 0x000113B4;
static const uint32_t ISTTY = 0x000113B5;
static const uint32_t DEVICE_CMN_INFO = 0x000113B6;
static const uint32_t DEVICE_SPEC_INFO = 0x000113B7;
static const uint32_t DEVICE_NAME = 0x000113B8;
static const uint32_t AV_SYNC_DELAY = 0x00012EED;  // AV Sync Delay

static const uint32_t IS_TX_DEV_FB_SPEAKER_PROT_ENABLED = 0x00012E0E;
static const uint32_t IS_RX_DEV_FB_SPEAKER_PROT_ENABLED = 0x00012E0F;
static const uint32_t FB_SPEAKER_PROT_RX_TX_DEV_PAIR = 0x00012E10;
static const uint32_t IS_TX_DEV_LISTEN_ENABLED = 0x00012E11;
static const uint32_t LSM_TOPID = 0x00012E12;       //LSM TopologyId

static const uint32_t IS_RX_DEV_ADAPTIVE_ANC_ENABLED = 0x00012E53;
static const uint32_t ADAPTIVE_ANCDEVPAIR = 0x00012E54;       //Adaptive ANC device pairs

// Global Acdb file property PID's Info
static const uint32_t AUD_VOL_STEPS = 0x000113AA;       //Audio vol steps
static const uint32_t VOC_VOL_STEPS = 0x000113AB;       //Voice vol steps
static const uint32_t AUDPROC_STRM_TOPID = 0x000113AE;       //AudProc Stream TopologyId list

static const uint32_t AUDIO_COPP_TOPO_INFO_ID = 0x00012E47;       //User added Audio COPP topology info
static const uint32_t AUDIO_POPP_TOPO_INFO_ID = 0x00012E48;       //User added Audio POPP topology info

static const uint32_t MID_PID_MAX_LEN = 0x00012EFE; // Mid Pid MaxLen info.

/* ---------------------------------------------------------------------------
* Class Definitions
*--------------------------------------------------------------------------- */

typedef struct  _AcdbDevPairInfo{
	uint32_t nRxDevId;
}AcdbDevPairInfo;

typedef struct _AcdbANCPairInfo{
	uint32_t nTxDevId;
}AcdbANCPairInfo;

typedef struct _AcdbAudprocTopInfo{
	uint32_t nAppId;
	uint32_t nTopoId;
}AcdbAudprocTopInfo;

typedef struct _AcdbVocprocTopInfo{
	uint32_t nTopoId;
}AcdbVocprocTopInfo;

typedef struct _AcdbAudStrmTopInfo{
	uint32_t nAppId;
	uint32_t nTopoId;
}AcdbAudStrmTopInfo;

typedef struct _AcdbCompRemoteDevIdInfo{
	uint32_t nDeviceId;
}AcdbCompRemoteDevIdInfo;

typedef struct _AcdbECRecDevPairInfo{
	uint32_t nRxDeviceId;
}AcdbECRecDevPairInfo;

typedef struct _AcdbTblInfo{
	uint32_t tblId;
}AcdbTblInfo;

#include "acdb_begin_pack.h"
 struct _ContentDefType{
	uint32_t nMid;
	uint32_t nPid;
}
#include "acdb_end_pack.h"
;

typedef struct _ContentDefType	ContentDefType;

typedef struct _ContentDefTblType{
	uint32_t nLen;
	ContentDefType *pCntDef;
}ContentDefTblType;

typedef struct _ContentDataOffsetsTblType{
	uint32_t nLen;
	uint32_t *pDataOffsets;
}ContentDataOffsetsTblType;

typedef struct _AcdbDataType{
	uint32_t nLen;
	uint8_t *pData;
}AcdbDataType;

//////
typedef struct _AudProcCmnDataLookupType {
   uint32_t nDeviceId;
   uint32_t nDeviceSampleRateId;
   uint32_t nApplicationType;
   uint32_t nCDEFTblOffset;
   uint32_t nCDOTTblOffset;
} AudProcCmnDataLookupType;

typedef struct _AudProcCmnDataLookupTblType {
   uint32_t nLen;
   AudProcCmnDataLookupType *pLut;
} AudProcCmnDataLookupTblType;

typedef struct _AudProcCmnCmdLookupType {
   uint32_t nDeviceId;
   uint32_t nDeviceSampleRateId;
   uint32_t nApplicationType;
} AudProcCmnCmdLookupType;

///////
typedef struct _AudProcGainDepDataLookupType {
   uint32_t nDeviceId;
   uint32_t nApplicationType;
   uint32_t nVolIdx;
   uint32_t nCDEFTblOffset;
   uint32_t nCDOTTblOffset;
} AudProcGainDepDataLookupType;

typedef struct _AudProcGainDepDataLookupTblType {
   uint32_t nLen;
   AudProcGainDepDataLookupType *pLut;
} AudProcGainDepDataLookupTblType;

typedef struct _AudProcGainDepCmdLookupType {
   uint32_t nDeviceId;
   uint32_t nApplicationType;
   uint32_t nVolIdx;
} AudProcGainDepCmdLookupType;

//////////////
typedef struct _AudStreamDataLookupType {
   uint32_t nApplicationType;
   uint32_t nCDEFTblOffset;
   uint32_t nCDOTTblOffset;
} AudStreamDataLookupType;

typedef struct _AudStreamDataLookupTblType {
   uint32_t nLen;
   AudStreamDataLookupType *pLut;
} AudStreamDataLookupTblType;

typedef struct _AudStreamCmdLookupType {
   uint32_t nApplicationType;
} AudStreamCmdLookupType;

typedef struct _VocProcDataLookupType {
   uint32_t nTxDevId;
   uint32_t nRxDevId;
   uint32_t nTxAfeSr;
   uint32_t nRxAfeSr;
   uint32_t nNetwork;
   uint32_t nTxVocSR;
   uint32_t nRxVocSR;
   uint32_t nCDEFTblOffset;
   uint32_t nCDOTTblOffset;
} VocProcDataLookupType;

typedef struct _VocProcDataLookupTblType {
   uint32_t nLen;
   VocProcDataLookupType *pLut;
} VocProcDataLookupTblType;

typedef struct _VocProcCmdLookupType {
   uint32_t nTxDevId;
   uint32_t nRxDevId;
   uint32_t nTxAfeSr;
   uint32_t nRxAfeSr;
} VocProcCmdLookupType;

typedef struct _VocProcVolDataLookupType {
   uint32_t nTxDevId;
   uint32_t nRxDevId;
   uint32_t nNetwork;
   uint32_t nTxVocSR;
   uint32_t nRxVocSR;
   uint32_t nVolStep;
   uint32_t nCDEFTblOffset;
   uint32_t nCDOTTblOffset;
} VocProcVolDataLookupType;

typedef struct _VocProcVolDataLookupTblType {
   uint32_t nLen;
   VocProcVolDataLookupType *pLut;
} VocProcVolDataLookupTblType;

typedef struct _VocProcVolV2DataLookupType {
   uint32_t nTxDevId;
   uint32_t nRxDevId;
   uint32_t nFeatureId;
   uint32_t nNetwork;
   uint32_t nTxVocSR;
   uint32_t nRxVocSR;
   uint32_t nVolStep;
   uint32_t nCDEFTblOffset;
   uint32_t nCDOTTblOffset;
} VocProcVolV2DataLookupType;

typedef struct _VocProcVolV2DataLookupTblType {
   uint32_t nLen;
   VocProcVolV2DataLookupType *pLut;
} VocProcVolV2DataLookupTblType;

typedef struct _VocProcVolCmdLookupType {
   uint32_t nTxDevId;
   uint32_t nRxDevId;
} VocProcVolCmdLookupType;

typedef struct _VocProcVolV2CmdLookupType {
   uint32_t nTxDevId;
   uint32_t nRxDevId;
   uint32_t nFeatureId;
} VocProcVolV2CmdLookupType;

typedef struct _VocStreamDataLookupType {
   uint32_t nNetwork;
   uint32_t nTxVocSR;
   uint32_t nRxVocSR;
   uint32_t nCDEFTblOffset;
   uint32_t nCDOTTblOffset;
} VocStreamDataLookupType;

typedef struct _VocStreamDataLookupTblType {
   uint32_t nLen;
   VocStreamDataLookupType *pLut;
} VocStreamDataLookupTblType;

typedef struct _ANCDataLookupType {
   uint32_t nDeviceId;
   uint32_t nPID;
   uint32_t nDataOffset;
} ANCDataLookupType;

typedef struct _ANCDataLookupTblType {
   uint32_t nLen;
   ANCDataLookupType *pLut;
} ANCDataLookupTblType;

typedef struct _ANCCmdLookupType {
   uint32_t nDeviceId;
   uint32_t nPID;
} ANCCmdLookupType;

////////
typedef struct _ADIEDataLookupType {
   uint32_t nDeviceId;
   uint32_t nPID;
   uint32_t nDataOffset;
} ADIEDataLookupType;

typedef struct _ADIEDataLookupTblType {
   uint32_t nLen;
   ADIEDataLookupType *pLut;
} ADIEDataLookupTblType;

typedef struct _ADIECmdLookupType {
   uint32_t nCodecId;
   uint32_t nPID;
} ADIECmdLookupType;

typedef struct _GlobalDataLookupType {
   uint32_t nMid;
   uint32_t nPid;
   uint32_t nDataOffset;
} GlobalDataLookupType;

typedef struct _GlobalDataLookupTblType {
   uint32_t nLen;
   GlobalDataLookupType *pLut;
} GlobalDataLookupTblType;

typedef struct _GlobalCmdLookupType {
   uint32_t nMid;
   uint32_t nPid;
} GlobalCmdLookupType;

//////

typedef struct _AfeDataLookupType {
   uint32_t nTxDevId;
   uint32_t nRxDevId;
   uint32_t nCDEFTblOffset;
   uint32_t nCDOTTblOffset;
} AfeDataLookupType;

typedef struct _AfeDataLookupTblType {
   uint32_t nLen;
   AfeDataLookupType *pLut;
} AfeDataLookupTblType;

typedef struct _AfeCmdLookupType {
   uint32_t nTxDevId;
   uint32_t nRxDevId;
} AfeCmdLookupType;
//////////

typedef struct _AfeCmnDataLookupType {
   uint32_t nDeviceId;
   uint32_t nDeviceSampleRateId;
   uint32_t nCDEFTblOffset;
   uint32_t nCDOTTblOffset;
} AfeCmnDataLookupType;

typedef struct _AfeCmnDataLookupTblType {
   uint32_t nLen;
   AfeCmnDataLookupType *pLut;
} AfeCmnDataLookupTblType;

typedef struct _AfeCmnCmdLookupType {
   uint32_t nDeviceId;
   uint32_t nDeviceSampleRateId;
} AfeCmnCmdLookupType;

///////////

typedef struct _VocprocDevCfgDataLookupType {
   uint32_t nTxDevId;
   uint32_t nRxDevId;
   uint32_t nCDEFTblOffset;
   uint32_t nCDOTTblOffset;
} VocprocDevCfgDataLookupType;

typedef struct _VocprocDevCfgDataLookupTblType {
   uint32_t nLen;
   VocprocDevCfgDataLookupType *pLut;
} VocprocDevCfgDataLookupTblType;

typedef struct _VocprocDevCfgCmdLookupType {
   uint32_t nTxDevId;
   uint32_t nRxDevId;
} VocprocDevCfgCmdLookupType;

//////

typedef struct _LSMTblLookupType {
   uint32_t nDevId;
   uint32_t nLSMAppTypeId;
   uint32_t nCDEFTblOffset;
   uint32_t nCDOTTblOffset;
} LSMTblLookupType;

typedef struct _LSMTblLookupTblType {
   uint32_t nLen;
   LSMTblLookupType *pLut;
} LSMTblLookupTblType;

typedef struct _LSMTblCmdLookupType {
   uint32_t nDevId;
   uint32_t nLSMAppTypeId;
} LSMTblCmdLookupType;

//////

typedef struct _CDCFeaturesDataLookupType {
   uint32_t nDeviceId;
   uint32_t nPID;
   uint32_t nDataOffset;
} CDCFeaturesDataLookupType;

typedef struct _CDCFeaturesDataLookupTblType {
   uint32_t nLen;
   CDCFeaturesDataLookupType *pLut;
} CDCFeaturesDataLookupTblType;

typedef struct _CDCFeaturesDataCmdLookupType {
   uint32_t nDeviceId;
   uint32_t nPID;
} CDCFeaturesDataCmdLookupType;

///////////

typedef struct _AdieSidetoneLookupType {
   uint32_t nTxDevId;
   uint32_t nRxDevId;
   uint32_t nCDEFTblOffset;
   uint32_t nCDOTTblOffset;
} AdieSidetoneLookupType;

typedef struct _AdieSidetoneLookupTblType {
   uint32_t nLen;
   AdieSidetoneLookupType *pLut;
} AdieSidetoneLookupTblType;

typedef struct _AdieSidetoneCmdLookupType {
   uint32_t nTxDevId;
   uint32_t nRxDevId;
} AdieSidetoneCmdLookupType;

////////////////////

typedef struct _AANCDataLookupType {
   uint32_t nDeviceId;
   uint32_t nCDEFTblOffset;
   uint32_t nCDOTTblOffset;
} AANCDataLookupType;

typedef struct _AANCDataLookupTblType {
   uint32_t nLen;
   AANCDataLookupType *pLut;
} AANCDataLookupTblType;

typedef struct _AANCCmdLookupType {
   uint32_t nDeviceId;
} AANCCmdLookupType;

typedef struct _VP3DevPairDataLookupType {
   uint32_t nTxDevId;
   uint32_t nRxDevId;
   uint32_t nCDEFTblOffset;
   uint32_t nCDOTTblOffset;
} VP3DevPairDataLookupType;

typedef struct _VP3DevPairDataLookupTblType {
   uint32_t nLen;
   VP3DevPairDataLookupType *pLut;
} VP3DevPairDataLookupTblType;

typedef struct _VP3DevDataLookupType {
   uint32_t nTxDevId;
   uint32_t nCDEFTblOffset;
   uint32_t nCDOTTblOffset;
} VP3DevDataLookupType;

typedef struct _VP3DevDataLookupTblType {
   uint32_t nLen;
   VP3DevDataLookupType *pLut;
} VP3DevDataLookupTblType;

typedef struct _VP3DevPairCmdLookupType {
   uint32_t nTxDevId;
   uint32_t nRxDevId;
} VP3DevPairCmdLookupType;

typedef struct _VP3DevCmdLookupType {
   uint32_t nTxDevId;
} VP3DevCmdLookupType;

////////////////////
typedef struct _DspCalHdrFormatType{
	uint32_t nModuleId;
	uint32_t nParamId;
	uint16_t nParamSize; //multiple of 4
	uint16_t nReserved; // Must be 0
}DspCalHdrFormatType;

typedef struct _VocProcCVDTblHdrFmtType{
	uint32_t nNetworkId;
	uint32_t nTxVocSr;
	uint32_t nRxVocSr;
	uint32_t nCalDataSize;
}VocProcCVDTblHdrFmtType;

typedef struct _VocProcVolCVDTblHdrFmtType{
	uint32_t nNetworkId;
	uint32_t nTxVocSr;
	uint32_t nRxVocSr;
	uint32_t nVolIdx;
	uint32_t nCalDataSize;
}VocProcVolCVDTblHdrFmtType;

////////////////////
typedef struct _DelayEntry {
   uint32_t nSampleRate;
   uint32_t nDelayInMicroSec;
}DelayEntry;

typedef struct _AVSyncDelay {
   uint32_t nNoOfEntries; // Number of entries
   DelayEntry *pInfo;     // Array of DelayEntry structure of size nNoOfEntries
}AVSyncDelay;

 struct _MaxLenDefType{
	uint32_t nMid;
	uint32_t nPid;
    uint32_t nMaxLen;
};

typedef struct _MaxLenDefType	MaxLenDefType;

typedef struct _MaxLenDefPrptyType{
	uint32_t nLen;
	MaxLenDefType *pCntDef;
}MaxLenDefPrptyType;

/* ---------------------------------------------------------------------------
 * Function Declarations and Documentation
 *--------------------------------------------------------------------------- */

#endif /* __ACDB_DATAINFO_H__ */
