/*****************************************************************************
* Copyright (c) 2013 Qualcomm Technologies, Inc.  All Rights Reserved.       *
* Qualcomm Technologies Proprietary and Confidential.                        *
*****************************************************************************/
#include "QMobicatComposer.h"

extern "C" {
#include "mct_stream.h"
#include "eztune_vfe_diagnostics.h"
#include "eztune_diagnostics.h"
#include <stdlib.h>
}

/* -----------------------------------------------------------------------
** Type Declarations
** ----------------------------------------------------------------------- */
/* -----------------------------------------------------------------------
** Global Object Definitions
** ----------------------------------------------------------------------- */
/* -----------------------------------------------------------------------
** Local Object Definitions
** ----------------------------------------------------------------------- */

static const char mobicat_magic_string[] = "Qualcomm Camera Attributes v2";

/* -----------------------------------------------------------------------
** Forward Declarations
** ----------------------------------------------------------------------- */
/* =======================================================================
**                          Macro Definitions
** ======================================================================= */

#define MAX_MOBICAT_LENGTH 60000
/**
 * PARSE_F - parse a float value
 * @T - mobicat tag
 * @V - value to parse
 */
#define PARSE_F(T,V)    (parseVal<float>("%f", T, V))
/**
 * PARSE_U - parse an unsigned value
 * @T - mobicat tag
 * @V - value to parse
 */
#define PARSE_U(T,V)    (parseVal<uint32_t>("%lu", T, V))

/**
 * PARSE_U16 - parse an uint16_t value
 * @T - mobicat tag
 * @V - value to parse
 */
#define PARSE_U16(T,V)    (parseVal<uint16_t>("%hu", T, V))

/**
 * PARSE_I16 - parse an uint16_t value
 * @T - mobicat tag
 * @V - value to parse
 */
#define PARSE_I16(T,V)    (parseVal<uint16_t>("%hd", T, V))
/**
 * PARSE_C - parse a char value
 * @T - mobicat tag
 * @V - value to parse
 */
#define PARSE_C(T,V)    (parseVal<char>("%hhd", T, V))
/**
 * PARSE_C - parse a char value
 * @T - mobicat tag
 * @V - value to parse
 */
#define PARSE_UC(T,V)    (parseVal<unsigned char>("%hhu", T, V))
/**
 * PARSE_I - parse an int value
 * @T - mobicat tag
 * @V - value to parse
 */
#define PARSE_I(T,V)    (parseVal<int>("%d", T, V))
/**
 * PARSE_SI - parse a short int value
 * @T - mobicat tag
 * @V - value to parse
 */
#define PARSE_SI(T,V)   (parseVal<short>("%hd", T, V))
/**
 * PARSE_F_A1 - parse a 1D float array
 * @T - mobicat tag
 * @V - array to parse
 */
#define PARSE_F_A1(T,V) (parseValArr<float>("%f", T, &(V[0]), \
  sizeof(V)/sizeof(V[0])))

/**
 * PARSE_I_A1 - parse a 1D int array
 * @T - mobicat tag
 * @V - array to parse
 */
#define PARSE_I_A1(T,V) (parseValArr<int>("%d", T, &(V[0]), \
  sizeof(V)/sizeof(V[0])))

/**
 * PARSE_SI_A1 - parse a 1D int array
 * @T - mobicat tag
 * @V - array to parse
 */
#define PARSE_SI_A1(T,V) (parseValArr<short>("%hd", T, &(V[0]), \
  sizeof(V)/sizeof(V[0])))

/**
 * PARSE_U_A1 - parse a 1D unsigned array
 * @T - mobicat tag
 * @V - array to parse
 */
#define PARSE_U_A1(T,V) (parseValArr<uint32_t>("%lu", T, &(V[0]), \
  sizeof(V)/sizeof(V[0])))

/**
 * PARSE_U16_A1 - parse an uint16_t value
 * @T - mobicat tag
 * @V - value to parse
 */
#define PARSE_U16_A1(T,V) (parseValArr<uint16_t>("%hu", T, &(V[0]), \
  sizeof(V)/sizeof(V[0])))

/**
 * PARSE_F_A2 - parse a 2D float array
 * @T - mobicat tag
 * @V - array to parse
 */
#define PARSE_F_A2(T,V) (parseValArr<float>("%f", T, &(V[0][0]), \
  sizeof(V)/sizeof(V[0][0])))

#define ARR_SZ(a) (sizeof(a)/sizeof(a[0]))

/* =======================================================================
**                          Function Definitions
** ======================================================================= */

/*===========================================================================
 * Function: QMobicatComposer
 *
 * Description: Constuctor
 *
 * Input parameters: none
 *
 * Return values: none
 *
 * Notes: none
 *==========================================================================*/
QMobicatComposer::QMobicatComposer()
{
  mScratchBuf = 0;
  mMobicatStr = 0;
}

/*===========================================================================
 * Function: ~QMobicatComposer
 *
 * Description: destructor
 *
 * Input parameters:
 *   none
 *
 * Return values:
 *   none
 *
 * Notes: none
 *==========================================================================*/
QMobicatComposer::~QMobicatComposer()
{
  if (mScratchBuf) {
     free(mScratchBuf);
  }
  if (mMobicatStr) {
     free(mMobicatStr);
  }
}
/*===========================================================================
 * Function: ParseMobicatData
 *
 * Description: Parse metadata into mobicat tags and compose string
 *
 * Input parameters:
 *   metadata - metadata
 *   metadata_size - size of metadata
 *
 * Return values:
 *   char* - composed mobicat string
 *
 * Notes: none
 *==========================================================================*/
char* QMobicatComposer::ParseMobicatData(uint8_t *metadata)
{
  int i = 0;
  cam_metadata_info_t *lMeta;
  mct_stream_session_metadata_info *lSessionMeta;
  awb_update_t lAwbData;
  stats_get_data_t lAecData;
  vfe_diagnostics_t lvfeDiag;
  ez_pp_params_t lEzPpParams;

  /* typecast private meta data */
  lMeta = (cam_metadata_info_t *) metadata;
  lSessionMeta = (mct_stream_session_metadata_info *) lMeta->private_metadata;
  memcpy(&lAwbData, &lSessionMeta->isp_stats_awb_data.private_data,
    sizeof(lAwbData));
  memcpy(&lAecData, &lSessionMeta->stats_aec_data.private_data,
    sizeof(lAecData));
  memcpy(&lvfeDiag, &lMeta->chromatix_lite_isp_data.private_isp_data,
    sizeof(lvfeDiag));
  memcpy(&lEzPpParams, &lMeta->chromatix_lite_pp_data.private_pp_data,
    sizeof(lEzPpParams));

  mMobicatStr = (char *)malloc(MAX_MOBICAT_LENGTH * sizeof(char));
  if (mMobicatStr == NULL) {
      QIDBG_ERROR("%s:%d] Error no memory", __func__, __LINE__);
      return 0;
  }
  memset(mMobicatStr, 0x0, MAX_MOBICAT_LENGTH * sizeof(char));

  mScratchBuf = (char *)malloc(MAX_MOBICAT_LENGTH * sizeof(char));
  if (mScratchBuf == NULL) {
      QIDBG_ERROR("%s:%d] Error no memory", __func__, __LINE__);
      free(mMobicatStr);
      return 0;
  }
  memset(mScratchBuf, 0x0, MAX_MOBICAT_LENGTH * sizeof(char));

  /* Append magic string to beginning */
  strcat(mMobicatStr, mobicat_magic_string);

  /* Parse AWB */
  PARSE_F("awb-rGain=", lAwbData.gain.r_gain);
  PARSE_F("awb-gGain=", lAwbData.gain.g_gain);
  PARSE_F("awb-bGain=", lAwbData.gain.b_gain);
  PARSE_U("awb-colorTemp=", lAwbData.color_temp);
  PARSE_C("awb-boundingBoxLumaYMin=", lAwbData.bounding_box.y_min);
  PARSE_C("awb-boundingBoxLumaYMax=", lAwbData.bounding_box.y_max);
  PARSE_C("awb-slopeRegion1=", lAwbData.bounding_box.m1);
  PARSE_C("awb-slopeRegion1=", lAwbData.bounding_box.m2);
  PARSE_C("awb-slopeRegion1=", lAwbData.bounding_box.m3);
  PARSE_C("awb-slopeRegion1=", lAwbData.bounding_box.m4);
  PARSE_SI("awb-cbOffset1=", lAwbData.bounding_box.c1);
  PARSE_SI("awb-cbOffset1=", lAwbData.bounding_box.c2);
  PARSE_SI("awb-cbOffset1=", lAwbData.bounding_box.c3);
  PARSE_SI("awb-cbOffset1=", lAwbData.bounding_box.c4);
  PARSE_U("awb-extremeColor1=", lAwbData.exterme_color_param.t1);
  PARSE_U("awb-extremeColor2=", lAwbData.exterme_color_param.t2);
  PARSE_U("awb-extremeColor3=", lAwbData.exterme_color_param.t3);
  PARSE_U("awb-extremeColor4=", lAwbData.exterme_color_param.t4);
  PARSE_U("awb-extremeColor5=", lAwbData.exterme_color_param.t5);
  PARSE_U("awb-extremeColorMG=", lAwbData.exterme_color_param.mg);
  PARSE_I("awb-wbMode=", lAwbData.wb_mode);
  PARSE_I("awb-bestMode=", lAwbData.best_mode);
  PARSE_I_A1("awb-sampleDecision=", lAwbData.sample_decision);
  PARSE_I("awb-greyWorldStats=", lAwbData.grey_world_stats);
  PARSE_I("awb-ccmFlag=", lAwbData.ccm_flag);
  PARSE_F_A2("awb-curCcm=", lAwbData.cur_ccm);

  /* Parse AEC */
  PARSE_F_A1("aec-realGain=", lAecData.aec_get.real_gain);
  PARSE_U_A1("aec-lineCount=", lAecData.aec_get.linecount);
  PARSE_F("aec-ledOffGain=", lAecData.aec_get.led_off_gain);
  PARSE_U("aec-ledOffLinecount=", lAecData.aec_get.led_off_linecount);
  PARSE_I("aec-validEntries=", lAecData.aec_get.valid_entries);
  PARSE_I("aec-triggerLed=", lAecData.aec_get.trigger_led);
  PARSE_F("aec-expTime=", lAecData.aec_get.exp_time);
  PARSE_F("aec-luxIdx=", lAecData.aec_get.lux_idx);
  PARSE_I("aec-flag=", lAecData.flag);


  colorcorrection_t *lColCorr = &lvfeDiag.prev_colorcorr;
  rolloff_t *lRolloff = &lvfeDiag.prev_rolloff;
  demuxchannelgain_t *lDemuxChanGain = &lvfeDiag.prev_demuxchannelgain;
  asfsharpness5x5_t *lAsfSharp =  &lvfeDiag.prev_asf5x5;
  lumaadaptation_t *lLumaAdapt = &lvfeDiag.prev_lumaadaptation;
  chromasuppression_t *lChromaSupp = &lvfeDiag.prev_chromasupp;
  memorycolorenhancement_t *lMemColorEnhan = &lvfeDiag.prev_memcolorenhan;
  badcorrection_t *lBpc = &lvfeDiag.prev_bpc;
  badcorrection_t *lBcc = &lvfeDiag.prev_bcc;
  abffilterdata_t *lAbFilter = &lvfeDiag.prev_abfilter;
  demosaic3_t *lDemosaic = &lvfeDiag.prev_demosaic;
  skincolorenhancement_t *lSkinColEnhan = &lvfeDiag.prev_skincolorenhan;
  linearization_t *lLinear = &lvfeDiag.prev_linear;
  chromalumafiltercoeff_t *lChromaLuma = &lvfeDiag.prev_chromalumafilter;

  asfsharpness7x7_t *lAsfSharp7x7 = &lEzPpParams.prev_asf7x7;
  wavelet_t *lWnr = &lEzPpParams.prev_wnr;

  if (lMeta->is_chromatix_lite_isp_valid) {
    /* Parse VFE diagnostics data */
    PARSE_I("vfe-ColorCorr-coefRtoR=", lColCorr->coef_rtor);
    PARSE_I("vfe-ColorCorr-coefGtoR=", lColCorr->coef_gtor);
    PARSE_I("vfe-ColorCorr-coefBtoR=", lColCorr->coef_btor);
    PARSE_I("vfe-ColorCorr-coefRtoG=", lColCorr->coef_rtog);
    PARSE_I("vfe-ColorCorr-coefGtoG=", lColCorr->coef_gtog);
    PARSE_I("vfe-ColorCorr-coefBtoG=", lColCorr->coef_btog);
    PARSE_I("vfe-ColorCorr-coefRtoB=", lColCorr->coef_rtob);
    PARSE_I("vfe-ColorCorr-coefGtoB=", lColCorr->coef_gtob);
    PARSE_I("vfe-ColorCorr-coefBtoB=", lColCorr->coef_btob);
    PARSE_I("vfe-ColorCorr-coefRtoR=", lColCorr->coef_rtor);


    PARSE_I("vfe-colorConv-paramAp=", lvfeDiag.colorconv.param_ap);
    PARSE_I("vfe-colorConv-paramAm=", lvfeDiag.colorconv.param_am);
    PARSE_I("vfe-colorConv-paramBp=", lvfeDiag.colorconv.param_bp);
    PARSE_I("vfe-colorConv-paramBm=", lvfeDiag.colorconv.param_bm);
    PARSE_I("vfe-colorConv-paramCp=", lvfeDiag.colorconv.param_cp);
    PARSE_I("vfe-colorConv-paramCm=", lvfeDiag.colorconv.param_cm);
    PARSE_I("vfe-colorConv-paramDp=", lvfeDiag.colorconv.param_dp);
    PARSE_I("vfe-colorConv-paramDm=", lvfeDiag.colorconv.param_dm);
    PARSE_I("vfe-colorConv-paramKcb=", lvfeDiag.colorconv.param_kcb);
    PARSE_I("vfe-colorConv-paramKcr=", lvfeDiag.colorconv.param_kcr);
    PARSE_I("vfe-colorConv-paramRtoY=", lvfeDiag.colorconv.param_rtoy);
    PARSE_I("vfe-colorConv-paramGtoY=", lvfeDiag.colorconv.param_gtoy);
    PARSE_I("vfe-colorConv-paramBtoY=", lvfeDiag.colorconv.param_btoy);
    PARSE_I("vfe-colorConv-paramYOffset=", lvfeDiag.colorconv.param_yoffset);


    PARSE_F_A1("vfe-Rolloff-coefftableR=", lRolloff->coefftable_R);
    PARSE_F_A1("vfe-Rolloff-coefftableGr=", lRolloff->coefftable_Gr);
    PARSE_F_A1("vfe-Rolloff-coefftableGb=", lRolloff->coefftable_Gb);
    PARSE_F_A1("vfe-Rolloff-coefftableB=", lRolloff->coefftable_B);

    PARSE_U("vfe-DemuxChanGain-greenEvenRow=",
        lDemuxChanGain->greenEvenRow);
    PARSE_U("vfe-DemuxChanGain-oddEvenRow=",
        lDemuxChanGain->greenOddRow);
    PARSE_U("vfe-DemuxChanGain-Blue=",
        lDemuxChanGain->blue);
    PARSE_U("vfe-DemuxChanGain-Red=",
        lDemuxChanGain->red);


    PARSE_U("vfe-Asf5x5-smoothfilterEnabled=", lAsfSharp->smoothfilterEnabled);
    PARSE_U("vfe-Asf5x5-sharpMode=", lAsfSharp->sharpMode);
    PARSE_U("vfe-Asf5x5-lpfMode=", lAsfSharp->lpfMode);
    PARSE_U("vfe-Asf5x5-smoothcoefCenter=", lAsfSharp->smoothcoefCenter);
    PARSE_U("vfe-Asf5x5-smoothcoefSurr=", lAsfSharp->smoothcoefSurr);
    PARSE_U("vfe-Asf5x5-pipeflushCount=", lAsfSharp->pipeflushCount);
    PARSE_U("vfe-Asf5x5-pipeflushOvd=", lAsfSharp->sharpMode);
    PARSE_U("vfe-Asf5x5-flushhaltOvd=", lAsfSharp->flushhaltOvd);
    PARSE_U("vfe-Asf5x5-cropEnable=", lAsfSharp->cropEnable);
    PARSE_U("vfe-Asf5x5-normalizeFactor=", lAsfSharp->normalizeFactor);
    PARSE_U("vfe-Asf5x5-sharpthreshE1=", lAsfSharp->sharpthreshE1);
    PARSE_U("vfe-Asf5x5-sharpthreshE2=", lAsfSharp->sharpthreshE2);
    PARSE_U("vfe-Asf5x5-sharpthreshE3=", lAsfSharp->sharpthreshE3);
    PARSE_U("vfe-Asf5x5-sharpthreshE4=", lAsfSharp->sharpthreshE4);
    PARSE_U("vfe-Asf5x5-sharpthreshE5=", lAsfSharp->sharpthreshE5);
    PARSE_U("vfe-Asf5x5-sharpK1=", lAsfSharp->sharpK1);
    PARSE_U("vfe-Asf5x5-sharpK1=", lAsfSharp->sharpK1);


    PARSE_I("vfe-Asf5x5-F1Coef0=", lAsfSharp->f1coef0);
    PARSE_I("vfe-Asf5x5-F1Coef1=", lAsfSharp->f1coef1);
    PARSE_I("vfe-Asf5x5-F1Coef2=", lAsfSharp->f1coef2);
    PARSE_I("vfe-Asf5x5-F1Coef3=", lAsfSharp->f1coef3);
    PARSE_I("vfe-Asf5x5-F1Coef4=", lAsfSharp->f1coef4);
    PARSE_I("vfe-Asf5x5-F1Coef5=", lAsfSharp->f1coef5);
    PARSE_I("vfe-Asf5x5-F1Coef6=", lAsfSharp->f1coef6);
    PARSE_I("vfe-Asf5x5-F1Coef7=", lAsfSharp->f1coef7);
    PARSE_I("vfe-Asf5x5-F2Coef0=", lAsfSharp->f2coef0);
    PARSE_I("vfe-Asf5x5-F2Coef1=", lAsfSharp->f2coef1);
    PARSE_I("vfe-Asf5x5-F2Coef2=", lAsfSharp->f2coef2);
    PARSE_I("vfe-Asf5x5-F2Coef3=", lAsfSharp->f2coef3);
    PARSE_I("vfe-Asf5x5-F2Coef4=", lAsfSharp->f2coef4);
    PARSE_I("vfe-Asf5x5-F2Coef5=", lAsfSharp->f2coef5);
    PARSE_I("vfe-Asf5x5-F2Coef6=", lAsfSharp->f2coef6);
    PARSE_I("vfe-Asf5x5-F2Coef7=", lAsfSharp->f2coef7);
    PARSE_I("vfe-Asf5x5-F3Coef0=", lAsfSharp->f3coef0);
    PARSE_I("vfe-Asf5x5-F3Coef1=", lAsfSharp->f3coef1);
    PARSE_I("vfe-Asf5x5-F3Coef2=", lAsfSharp->f3coef2);
    PARSE_I("vfe-Asf5x5-F3Coef3=", lAsfSharp->f3coef3);
    PARSE_I("vfe-Asf5x5-F3Coef4=", lAsfSharp->f3coef4);
    PARSE_I("vfe-Asf5x5-F3Coef5=", lAsfSharp->f3coef5);
    PARSE_I("vfe-Asf5x5-F3Coef6=", lAsfSharp->f3coef6);
    PARSE_I("vfe-Asf5x5-F3Coef7=", lAsfSharp->f3coef7);

    PARSE_I_A1("vfe-LumaAdaptation-lutYratio=", lLumaAdapt->lut_yratio);


    PARSE_U("vfe-ChromaSupp-ysup1=", lChromaSupp->ysup1);
    PARSE_U("vfe-ChromaSupp-ysup2=", lChromaSupp->ysup2);
    PARSE_U("vfe-ChromaSupp-ysup3=", lChromaSupp->ysup3);
    PARSE_U("vfe-ChromaSupp-ysupM1=", lChromaSupp->ysupM1);
    PARSE_U("vfe-ChromaSupp-ysupM3=", lChromaSupp->ysupM3);
    PARSE_U("vfe-ChromaSupp-ysupS1=", lChromaSupp->ysupS1);
    PARSE_U("vfe-ChromaSupp-ysupS3=", lChromaSupp->ysupS3);
    PARSE_U("vfe-ChromaSupp-csup1=", lChromaSupp->csup1);
    PARSE_U("vfe-ChromaSupp-csup2=", lChromaSupp->csup2);
    PARSE_U("vfe-ChromaSupp-csupM1=", lChromaSupp->csupM1);
    PARSE_U("vfe-ChromaSupp-csupS1=", lChromaSupp->csupS1);


    PARSE_U("vfe-MemColorEnhan-qk=", lMemColorEnhan->qk);
    PARSE_U("vfe-MemColorEnhan-red-y1=", lMemColorEnhan->red.y1);
    PARSE_U("vfe-MemColorEnhan-red-y2=", lMemColorEnhan->red.y2);
    PARSE_U("vfe-MemColorEnhan-red-y3=", lMemColorEnhan->red.y3);
    PARSE_U("vfe-MemColorEnhan-red-y4=", lMemColorEnhan->red.y4);
    PARSE_U("vfe-MemColorEnhan-red-yM1=", lMemColorEnhan->red.yM1);
    PARSE_U("vfe-MemColorEnhan-red-yM3=", lMemColorEnhan->red.yM3);
    PARSE_U("vfe-MemColorEnhan-red-yS1=", lMemColorEnhan->red.yS1);
    PARSE_U("vfe-MemColorEnhan-red-yS3=", lMemColorEnhan->red.yS3);
    PARSE_U("vfe-MemColorEnhan-red-transWidth=", lMemColorEnhan->red.transWidth);
    PARSE_U("vfe-MemColorEnhan-red-transTrunc=", lMemColorEnhan->red.transTrunc);
    PARSE_I("vfe-MemColorEnhan-red-crZone=", lMemColorEnhan->red.crZone);
    PARSE_I("vfe-MemColorEnhan-red-cbZone=", lMemColorEnhan->red.cbZone);
    PARSE_I("vfe-MemColorEnhan-red-translope=", lMemColorEnhan->red.translope);
    PARSE_I("vfe-MemColorEnhan-red-k=", lMemColorEnhan->red.k);

    PARSE_U("vfe-Bpc-fminThreshold=", lBpc->fminThreshold);
    PARSE_U("vfe-Bpc-fmaxThreshold=", lBpc->fmaxThreshold);
    PARSE_U("vfe-Bpc-rOffsetLo=", lBpc->rOffsetLo);
    PARSE_U("vfe-Bpc-rOffsetHi=", lBpc->rOffsetHi);
    PARSE_U("vfe-Bpc-grOffsetLo=", lBpc->grOffsetLo);
    PARSE_U("vfe-Bpc-gbOffsetLo=", lBpc->gbOffsetLo);
    PARSE_U("vfe-Bpc-gbOffsetHi=", lBpc->gbOffsetHi);
    PARSE_U("vfe-Bpc-grOffsetHi=", lBpc->grOffsetHi);
    PARSE_U("vfe-Bpc-bOffsetLo=", lBpc->bOffsetLo);
    PARSE_U("vfe-Bpc-bOffsetHi=", lBpc->bOffsetHi);

    PARSE_U("vfe-Bcc-fminThreshold=", lBcc->fminThreshold);
    PARSE_U("vfe-Bcc-fmaxThreshold=", lBcc->fmaxThreshold);
    PARSE_U("vfe-Bcc-rOffsetLo=", lBcc->rOffsetLo);
    PARSE_U("vfe-Bcc-rOffsetHi=", lBcc->rOffsetHi);
    PARSE_U("vfe-Bcc-grOffsetLo=", lBcc->grOffsetLo);
    PARSE_U("vfe-Bcc-gbOffsetLo=", lBcc->gbOffsetLo);
    PARSE_U("vfe-Bcc-gbOffsetHi=", lBcc->gbOffsetHi);
    PARSE_U("vfe-Bcc-grOffsetHi=", lBcc->grOffsetHi);
    PARSE_U("vfe-Bcc-bOffsetLo=", lBcc->bOffsetLo);
    PARSE_U("vfe-Bcc-bOffsetHi=", lBcc->bOffsetHi);


    PARSE_U16_A1("vfe-AbFilter-red-Threshold=", lAbFilter->red.threshold);
    PARSE_U16_A1("vfe-AbFilter-red-Pos=", lAbFilter->red.pos);
    PARSE_SI_A1("vfe-AbFilter-red-Neg=", lAbFilter->red.neg);
    PARSE_U16_A1("vfe-AbFilter-green-Threshold=", lAbFilter->green.threshold);
    PARSE_U16_A1("vfe-AbFilter-green-Pos=", lAbFilter->green.pos);
    PARSE_SI_A1("vfe-AbFilter-green-Neg=", lAbFilter->green.neg);
    PARSE_U16_A1("vfe-AbFilter-blue-Threshold=", lAbFilter->blue.threshold);
    PARSE_U16_A1("vfe-AbFilter-blue-Pos=", lAbFilter->blue.pos);
    PARSE_SI_A1("vfe-AbFilter-blue-Neg=", lAbFilter->blue.neg);


    for (uint32_t i = 0; i < ARR_SZ(lDemosaic->lut); i++) {
      char tag_str[50];
      sprintf(&tag_str[0], "vfe-Demosaic-lut[%d]-wk=", i);
      PARSE_U(tag_str, lDemosaic->lut[i].wk);
      sprintf(&tag_str[0], "vfe-Demosaic-lut[%d]-bk=", i);
      PARSE_U(tag_str, lDemosaic->lut[i].bk);
      sprintf(&tag_str[0], "vfe-Demosaic-lut[%d]-lk=", i);
      PARSE_U(tag_str, lDemosaic->lut[i].lk);
      sprintf(&tag_str[0], "vfe-Demosaic-lut[%d]-tk=", i);
      PARSE_U(tag_str, lDemosaic->lut[i].tk);
    }

    PARSE_U("vfe-Demosaic-aG=", lDemosaic->aG);
    PARSE_U("vfe-Demosaic-bL=", lDemosaic->bL);


    PARSE_I("vfe-SkinColorEnhan-CrCoord-vertex00=",  lSkinColEnhan->crcoord.vertex00);
    PARSE_I("vfe-SkinColorEnhan-CrCoord-vertex01=",  lSkinColEnhan->crcoord.vertex01);
    PARSE_I("vfe-SkinColorEnhan-CrCoord-vertex02=",  lSkinColEnhan->crcoord.vertex02);
    PARSE_I("vfe-SkinColorEnhan-CrCoord-vertex10=",  lSkinColEnhan->crcoord.vertex10);
    PARSE_I("vfe-SkinColorEnhan-CrCoord-vertex11=",  lSkinColEnhan->crcoord.vertex11);
    PARSE_I("vfe-SkinColorEnhan-CrCoord-vertex12=",  lSkinColEnhan->crcoord.vertex12);
    PARSE_I("vfe-SkinColorEnhan-CrCoord-vertex20=",  lSkinColEnhan->crcoord.vertex20);
    PARSE_I("vfe-SkinColorEnhan-CrCoord-vertex21=",  lSkinColEnhan->crcoord.vertex21);
    PARSE_I("vfe-SkinColorEnhan-CrCoord-vertex22=",  lSkinColEnhan->crcoord.vertex22);
    PARSE_I("vfe-SkinColorEnhan-CrCoord-vertex30=",  lSkinColEnhan->crcoord.vertex30);
    PARSE_I("vfe-SkinColorEnhan-CrCoord-vertex31=",  lSkinColEnhan->crcoord.vertex31);
    PARSE_I("vfe-SkinColorEnhan-CrCoord-vertex32=",  lSkinColEnhan->crcoord.vertex32);
    PARSE_I("vfe-SkinColorEnhan-CrCoord-vertex40=",  lSkinColEnhan->crcoord.vertex40);
    PARSE_I("vfe-SkinColorEnhan-CrCoord-vertex41=",  lSkinColEnhan->crcoord.vertex41);
    PARSE_I("vfe-SkinColorEnhan-CrCoord-vertex42=",  lSkinColEnhan->crcoord.vertex42);

    PARSE_I("vfe-SkinColorEnhan-CbCoord-vertex00=",  lSkinColEnhan->cbcoord.vertex00);
    PARSE_I("vfe-SkinColorEnhan-CbCoord-vertex01=",  lSkinColEnhan->cbcoord.vertex01);
    PARSE_I("vfe-SkinColorEnhan-CbCoord-vertex02=",  lSkinColEnhan->cbcoord.vertex02);
    PARSE_I("vfe-SkinColorEnhan-CbCoord-vertex10=",  lSkinColEnhan->cbcoord.vertex10);
    PARSE_I("vfe-SkinColorEnhan-CbCoord-vertex11=",  lSkinColEnhan->cbcoord.vertex11);
    PARSE_I("vfe-SkinColorEnhan-CbCoord-vertex12=",  lSkinColEnhan->cbcoord.vertex12);
    PARSE_I("vfe-SkinColorEnhan-CbCoord-vertex20=",  lSkinColEnhan->cbcoord.vertex20);
    PARSE_I("vfe-SkinColorEnhan-CbCoord-vertex21=",  lSkinColEnhan->cbcoord.vertex21);
    PARSE_I("vfe-SkinColorEnhan-CbCoord-vertex22=",  lSkinColEnhan->cbcoord.vertex22);
    PARSE_I("vfe-SkinColorEnhan-CbCoord-vertex30=",  lSkinColEnhan->cbcoord.vertex30);
    PARSE_I("vfe-SkinColorEnhan-CbCoord-vertex31=",  lSkinColEnhan->cbcoord.vertex31);
    PARSE_I("vfe-SkinColorEnhan-CbCoord-vertex32=",  lSkinColEnhan->cbcoord.vertex32);
    PARSE_I("vfe-SkinColorEnhan-CbCoord-vertex40=",  lSkinColEnhan->cbcoord.vertex40);
    PARSE_I("vfe-SkinColorEnhan-CbCoord-vertex41=",  lSkinColEnhan->cbcoord.vertex41);
    PARSE_I("vfe-SkinColorEnhan-CbCoord-vertex42=",  lSkinColEnhan->cbcoord.vertex42);


    PARSE_I("vfe-SkinColorEnhan-CrCoeff-coef00=", lSkinColEnhan->crcoeff.coef00);
    PARSE_I("vfe-SkinColorEnhan-CrCoeff-coef01=", lSkinColEnhan->crcoeff.coef01);
    PARSE_I("vfe-SkinColorEnhan-CrCoeff-coef10=", lSkinColEnhan->crcoeff.coef10);
    PARSE_I("vfe-SkinColorEnhan-CrCoeff-coef11=", lSkinColEnhan->crcoeff.coef11);
    PARSE_I("vfe-SkinColorEnhan-CrCoeff-coef20=", lSkinColEnhan->crcoeff.coef20);
    PARSE_I("vfe-SkinColorEnhan-CrCoeff-coef21=", lSkinColEnhan->crcoeff.coef21);
    PARSE_I("vfe-SkinColorEnhan-CrCoeff-coef30=", lSkinColEnhan->crcoeff.coef30);
    PARSE_I("vfe-SkinColorEnhan-CrCoeff-coef31=", lSkinColEnhan->crcoeff.coef31);
    PARSE_I("vfe-SkinColorEnhan-CrCoeff-coef40=", lSkinColEnhan->crcoeff.coef40);
    PARSE_I("vfe-SkinColorEnhan-CrCoeff-coef41=", lSkinColEnhan->crcoeff.coef41);
    PARSE_I("vfe-SkinColorEnhan-CrCoeff-coef50=", lSkinColEnhan->crcoeff.coef50);
    PARSE_I("vfe-SkinColorEnhan-CrCoeff-coef51=", lSkinColEnhan->crcoeff.coef51);

    PARSE_I("vfe-SkinColorEnhan-CbCoeff-coef00=", lSkinColEnhan->cbcoeff.coef00);
    PARSE_I("vfe-SkinColorEnhan-CbCoeff-coef01=", lSkinColEnhan->cbcoeff.coef01);
    PARSE_I("vfe-SkinColorEnhan-CbCoeff-coef10=", lSkinColEnhan->cbcoeff.coef10);
    PARSE_I("vfe-SkinColorEnhan-CbCoeff-coef11=", lSkinColEnhan->cbcoeff.coef11);
    PARSE_I("vfe-SkinColorEnhan-CbCoeff-coef20=", lSkinColEnhan->cbcoeff.coef20);
    PARSE_I("vfe-SkinColorEnhan-CbCoeff-coef21=", lSkinColEnhan->cbcoeff.coef21);
    PARSE_I("vfe-SkinColorEnhan-CbCoeff-coef30=", lSkinColEnhan->cbcoeff.coef30);
    PARSE_I("vfe-SkinColorEnhan-CbCoeff-coef31=", lSkinColEnhan->cbcoeff.coef31);
    PARSE_I("vfe-SkinColorEnhan-CbCoeff-coef40=", lSkinColEnhan->cbcoeff.coef40);
    PARSE_I("vfe-SkinColorEnhan-CbCoeff-coef41=", lSkinColEnhan->cbcoeff.coef41);
    PARSE_I("vfe-SkinColorEnhan-CbCoeff-coef50=", lSkinColEnhan->cbcoeff.coef50);
    PARSE_I("vfe-SkinColorEnhan-CbCoeff-coef51=", lSkinColEnhan->cbcoeff.coef51);

    PARSE_I("vfe-SkinColorEnhan-CrOffset-offset0=", lSkinColEnhan->croffset.offset0);
    PARSE_I("vfe-SkinColorEnhan-CrOffset-offset1=", lSkinColEnhan->croffset.offset1);
    PARSE_I("vfe-SkinColorEnhan-CrOffset-offset2=", lSkinColEnhan->croffset.offset2);
    PARSE_I("vfe-SkinColorEnhan-CrOffset-offset3=", lSkinColEnhan->croffset.offset3);
    PARSE_I("vfe-SkinColorEnhan-CrOffset-offset4=", lSkinColEnhan->croffset.offset4);
    PARSE_I("vfe-SkinColorEnhan-CrOffset-offset5=", lSkinColEnhan->croffset.offset5);
    PARSE_U("vfe-SkinColorEnhan-CrOffset-shift0=", lSkinColEnhan->croffset.shift0);
    PARSE_U("vfe-SkinColorEnhan-CrOffset-shift1=", lSkinColEnhan->croffset.shift1);
    PARSE_U("vfe-SkinColorEnhan-CrOffset-shift2=", lSkinColEnhan->croffset.shift2);
    PARSE_U("vfe-SkinColorEnhan-CrOffset-shift3=", lSkinColEnhan->croffset.shift3);
    PARSE_U("vfe-SkinColorEnhan-CrOffset-shift4=", lSkinColEnhan->croffset.shift4);
    PARSE_U("vfe-SkinColorEnhan-CrOffset-shift5=", lSkinColEnhan->croffset.shift5);

    PARSE_I("vfe-SkinColorEnhan-CrOffset-offset0=", lSkinColEnhan->croffset.offset0);
    PARSE_I("vfe-SkinColorEnhan-CrOffset-offset1=", lSkinColEnhan->croffset.offset1);
    PARSE_I("vfe-SkinColorEnhan-CrOffset-offset2=", lSkinColEnhan->croffset.offset2);
    PARSE_I("vfe-SkinColorEnhan-CrOffset-offset3=", lSkinColEnhan->croffset.offset3);
    PARSE_I("vfe-SkinColorEnhan-CrOffset-offset4=", lSkinColEnhan->croffset.offset4);
    PARSE_I("vfe-SkinColorEnhan-CrOffset-offset5=", lSkinColEnhan->croffset.offset5);
    PARSE_U("vfe-SkinColorEnhan-CbOffset-shift0=", lSkinColEnhan->cboffset.shift0);
    PARSE_U("vfe-SkinColorEnhan-CbOffset-shift1=", lSkinColEnhan->cboffset.shift1);
    PARSE_U("vfe-SkinColorEnhan-CbOffset-shift2=", lSkinColEnhan->cboffset.shift2);
    PARSE_U("vfe-SkinColorEnhan-CbOffset-shift3=", lSkinColEnhan->cboffset.shift3);
    PARSE_U("vfe-SkinColorEnhan-CbOffset-shift4=", lSkinColEnhan->cboffset.shift4);
    PARSE_U("vfe-SkinColorEnhan-CbOffset-shift5=", lSkinColEnhan->cboffset.shift5);


    PARSE_U16_A1("vfe-Linear-rlut_pl=", lLinear->rlut_pl);
    PARSE_U16_A1("vfe-Linear-grlut_pl=", lLinear->grlut_pl);
    PARSE_U16_A1("vfe-Linear-gblut_pl=", lLinear->gblut_pl);
    PARSE_U16_A1("vfe-Linear-blut_pl=",  lLinear->blut_pl);
    PARSE_U16_A1("vfe-Linear-rlut_base=", lLinear->rlut_base);
    PARSE_U16_A1("vfe-Linear-grlut_base=", lLinear->grlut_base);
    PARSE_U16_A1("vfe-Linear-gblut_base=", lLinear->gblut_base);
    PARSE_U16_A1("vfe-Linear-blut_base=",  lLinear->blut_base);
    PARSE_U_A1("vfe-Linear-rlut_delta=", lLinear->rlut_delta);
    PARSE_U_A1("vfe-Linear-grlut_delta=", lLinear->grlut_delta);
    PARSE_U_A1("vfe-Linear-gblut_delta=", lLinear->gblut_delta);
    PARSE_U_A1("vfe-Linear-blut_delta=",  lLinear->blut_delta);

    PARSE_F_A1("vfe-ChromaLumaFilter-Chroma-hcoeff=", lChromaLuma->chromafilter.hcoeff);
    PARSE_F_A1("vfe-ChromaLumaFilter-Chroma-vcoeff=", lChromaLuma->chromafilter.vcoeff);

    PARSE_U16_A1("vfe-ChromaLumaFilter-Luma-thresholdRed=", lChromaLuma->lumafilter.threshold_red);
    PARSE_F_A1("vfe-ChromaLumaFilter-Luma-scalefactorRed=", lChromaLuma->lumafilter.scalefactor_red);
    PARSE_U16_A1("vfe-ChromaLumaFilter-Luma-thresholdGreen=", lChromaLuma->lumafilter.threshold_green);
    PARSE_F_A1("vfe-ChromaLumaFilter-Luma-scalefactorGreenr=", lChromaLuma->lumafilter.scalefactor_green);
    PARSE_U16_A1("vfe-ChromaLumaFilter-Luma-thresholdBlue=", lChromaLuma->lumafilter.threshold_blue);
    PARSE_F_A1("vfe-ChromaLumaFilter-Luma-scalefactorBlue=", lChromaLuma->lumafilter.scalefactor_blue);
    PARSE_F_A1("vfe-ChromaLumaFilter-Luma-tablepos=", lChromaLuma->lumafilter.tablepos);
    PARSE_F_A1("vfe-ChromaLumaFilter-Luma-tableneg=", lChromaLuma->lumafilter.tableneg);
  } else {
    QIDBG_ERROR("%s:%d] Vfe data not valid!", __func__, __LINE__);
  }
  /* pp params */

  if (lMeta->is_chromatix_lite_pp_valid) {
    PARSE_F("pp-Asf7x7-smoothPercent=", lAsfSharp7x7->smoothpercent);
    PARSE_UC("pp-Asf7x7-negAbsY1=", lAsfSharp7x7->neg_abs_y1);
    PARSE_UC("pp-Asf7x7-dynaClampEn=", lAsfSharp7x7->dyna_clamp_en);
    PARSE_UC("pp-Asf7x7-spEffEn=", lAsfSharp7x7->sp_eff_en);
    PARSE_I16("pp-Asf7x7-clampHh=", lAsfSharp7x7->clamp_hh);
    PARSE_I16("pp-Asf7x7-clampHl=", lAsfSharp7x7->clamp_hl);
    PARSE_I16("pp-Asf7x7-clampVh=", lAsfSharp7x7->clamp_vh);
    PARSE_I16("pp-Asf7x7-clampVl=", lAsfSharp7x7->clamp_vl);
    PARSE_F("pp-Asf7x7-clampScaleMax=", lAsfSharp7x7->clamp_scale_max);
    PARSE_F("pp-Asf7x7-clampScaleMin=", lAsfSharp7x7->clamp_scale_min);
    PARSE_U16("pp-Asf7x7-clampOffsetMax=", lAsfSharp7x7->clamp_offset_max);
    PARSE_U16("pp-Asf7x7-clampOffsetMin=", lAsfSharp7x7->clamp_offset_min);
    PARSE_U("pp-Asf7x7-NzFlag=", lAsfSharp7x7->nz_flag);
    PARSE_F_A1("pp-Asf7x7-SobelHCoeff=", lAsfSharp7x7->sobel_h_coeff);
    PARSE_F_A1("pp-Asf7x7-SobelVCoeff=", lAsfSharp7x7->sobel_v_coeff);
    PARSE_F_A1("pp-Asf7x7-HpfHCoeff=", lAsfSharp7x7->hpf_h_coeff);
    PARSE_F_A1("pp-Asf7x7-HpfVCoeff=", lAsfSharp7x7->hpf_v_coeff);
    PARSE_F_A1("pp-Asf7x7-LpfVCoeff=", lAsfSharp7x7->lpf_coeff);
    PARSE_F_A1("pp-Asf7x7-lut1=", lAsfSharp7x7->lut1);
    PARSE_F_A1("pp-Asf7x7-lut2=", lAsfSharp7x7->lut2);
    PARSE_F_A1("pp-Asf7x7-lut3=", lAsfSharp7x7->lut3);

    PARSE_F_A1("pp-Wnr-BilateralScalecore0=", lWnr->bilateral_scalecore0);
    PARSE_F_A1("pp-Wnr-BilateralScalecore1=", lWnr->bilateral_scalecore1);
    PARSE_F_A1("pp-Wnr-BilateralScalecore2=", lWnr->bilateral_scalecore2);
    PARSE_F_A1("pp-Wnr-NoiseThresholdCore0=", lWnr->noise_thresholdcore0);
    PARSE_F_A1("pp-Wnr-NoiseThresholdCore1=", lWnr->noise_thresholdcore1);
    PARSE_F_A1("pp-Wnr-NoiseThresholdCore2=", lWnr->noise_thresholdcore2);
    PARSE_F_A1("pp-Wnr-WeightCore0=", lWnr->weightcore0);
    PARSE_F_A1("pp-Wnr-WeightCore1=", lWnr->weightcore1);
    PARSE_F_A1("pp-Wnr-WeightCore2=", lWnr->weightcore2);
  } else {
    QIDBG_ERROR("%s:%d] PP data not valid!", __func__, __LINE__);
  }

  return mMobicatStr;
}

/*===========================================================================
 * Function: parseVal
 *
 * Description: Parses a value of type T into a string, using format string fmt
 *
 * Input parameters:
 *   fmt, aTag, aVal
 *
 * Return values:
 *   void
 *
 * Notes: none
 *==========================================================================*/
template <typename T> void QMobicatComposer::parseVal(const char *fmt,
  const char *aTag, T aVal)
{
  snprintf(mScratchBuf, MAX_MOBICAT_LENGTH, "%s", aTag);
  strcat(mMobicatStr, mScratchBuf);

  snprintf(mScratchBuf, MAX_MOBICAT_LENGTH, fmt, aVal);
  strcat(mMobicatStr, mScratchBuf);

  strcat(mMobicatStr, "\n");
}

/*===========================================================================
 * Function: parseVal
 *
 * Description: Parses an array of type T into a string, using format string fmt
 *
 * Input parameters:
 *   fmt, aTag, aValPtr, aLen
 *
 * Return values:
 *   void
 *
 * Notes: none
 *==========================================================================*/
template <typename T> void QMobicatComposer::parseValArr(const char *fmt,
  const char *aTag, T *aValPtr, int aLen)
{
  snprintf(mScratchBuf, MAX_MOBICAT_LENGTH, "%s", aTag);
  strcat(mMobicatStr, mScratchBuf);

  while (aLen > 0) {
    snprintf(mScratchBuf, MAX_MOBICAT_LENGTH, fmt, *aValPtr);
    strcat(mMobicatStr, mScratchBuf);
    if (aLen != 1) {
      strcat(mMobicatStr, ",");
    }
    aValPtr++;
    aLen--;
  }
  strcat(mMobicatStr, "\n");
}
