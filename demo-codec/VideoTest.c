#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "vencoder.h"
#include <time.h>
#include "v4l2dev.h"

#define log printf
#define loge printf
#define logd printf
#define RUN_TEST printf("RUN_TEST __FILE__ %s __LINE__ %d \n",__FILE__,__LINE__);


//v4l2_read_frame(cam, frame_ptr, &frame_len);

int testmain()
{
	VencBaseConfig baseConfig;
	VencAllocateBufferParam bufferParam;
	VideoEncoder* pVideoEnc = NULL;
	VencInputBuffer inputBuffer;
	VencOutputBuffer outputBuffer;
	VencHeaderData sps_pps_data;
	VencH264Param h264Param;
	VencH264FixQP fixQP;
	EXIFInfo exifinfo;
	VencCyclicIntraRefresh sIntraRefresh;
	unsigned int src_width,src_height,dst_width,dst_height;

	VencROIConfig sRoiConfig[4];

	src_width = 1280;
	src_height = 720;
	dst_width = 1280;
	dst_height = 720;


	// roi
	sRoiConfig[0].bEnable = 1;
	sRoiConfig[0].index = 0;
	sRoiConfig[0].nQPoffset = 10;
	sRoiConfig[0].sRect.nLeft = 320;
	sRoiConfig[0].sRect.nTop = 180;
	sRoiConfig[0].sRect.nWidth = 320;
	sRoiConfig[0].sRect.nHeight = 180;


	sRoiConfig[1].bEnable = 1;
	sRoiConfig[1].index = 1;
	sRoiConfig[1].nQPoffset = 10;
	sRoiConfig[1].sRect.nLeft = 320;
	sRoiConfig[1].sRect.nTop = 180;
	sRoiConfig[1].sRect.nWidth = 320;
	sRoiConfig[1].sRect.nHeight = 180;


	sRoiConfig[2].bEnable = 1;
	sRoiConfig[2].index = 2;
	sRoiConfig[2].nQPoffset = 10;
	sRoiConfig[2].sRect.nLeft = 320;
	sRoiConfig[2].sRect.nTop = 180;
	sRoiConfig[2].sRect.nWidth = 320;
	sRoiConfig[2].sRect.nHeight = 180;


	sRoiConfig[3].bEnable = 1;
	sRoiConfig[3].index = 3;
	sRoiConfig[3].nQPoffset = 10;
	sRoiConfig[3].sRect.nLeft = 320;
	sRoiConfig[3].sRect.nTop = 180;
	sRoiConfig[3].sRect.nWidth = 320;
	sRoiConfig[3].sRect.nHeight = 180;


	//intraRefresh
	sIntraRefresh.bEnable = 1;
	sIntraRefresh.nBlockNumber = 10;

	//fix qp mode
	fixQP.bEnable = 1;
	fixQP.nIQp = 20;
	fixQP.nPQp = 30;
	
	exifinfo.ThumbWidth = 176;
	exifinfo.ThumbHeight = 144;

	//* h264 param
	h264Param.bEntropyCodingCABAC = 1;
	h264Param.nBitrate = 4*1024*1024; /* bps */
	h264Param.nFramerate = 30; /* fps */
	h264Param.nCodingMode = VENC_FRAME_CODING;
//	h264Param.nCodingMode = VENC_FIELD_CODING;
	
	h264Param.nMaxKeyInterval = 30;
	h264Param.sProfileLevel.nProfile = VENC_H264ProfileMain;
	h264Param.sProfileLevel.nLevel = VENC_H264Level31;
	h264Param.sQPRange.nMinqp = 10;
	h264Param.sQPRange.nMaxqp = 40;


	//int codecType = VENC_CODEC_H264;
	int codecType = VENC_CODEC_JPEG;
	int testNumber = 70;

	strcpy((char*)exifinfo.CameraMake,		"allwinner make test");
	strcpy((char*)exifinfo.CameraModel,		"allwinner model test");
	strcpy((char*)exifinfo.DateTime, 		"2014:02:21 10:54:05");
	strcpy((char*)exifinfo.gpsProcessingMethod,  "allwinner gps");

	exifinfo.Orientation = 0;
	
	exifinfo.ExposureTime.num = 2;
	exifinfo.ExposureTime.den = 1000;

	exifinfo.FNumber.num = 20;
	exifinfo.FNumber.den = 10;
	exifinfo.ISOSpeed = 50;


	exifinfo.ExposureBiasValue.num= -4;
	exifinfo.ExposureBiasValue.den= 1;

	exifinfo.MeteringMode = 1;
	exifinfo.FlashUsed = 0;

	exifinfo.FocalLength.num = 1400;
	exifinfo.FocalLength.den = 100;

	exifinfo.DigitalZoomRatio.num = 4;
	exifinfo.DigitalZoomRatio.den = 1;

	exifinfo.WhiteBalance = 1;
	exifinfo.ExposureMode = 1;

	exifinfo.enableGpsInfo = 1;

	exifinfo.gps_latitude = 23.2368;
	exifinfo.gps_longitude = 24.3244;
	exifinfo.gps_altitude = 1234.5;
	
	exifinfo.gps_timestamp = (long)time(NULL);

	FILE *in_file = NULL;
	FILE *out_file = NULL;


	v4l2dev_t camera = {
        -1,
	};


	v4l2_open(&camera, 0);
	v4l2_enum_fmt(&camera);
	v4l2_query_cap(&camera);
	v4l2_s_input(&camera, 0);
	v4l2_s_fmt(&camera, src_width, src_height, V4L2_PIX_FMT_NV12);
	v4l2_g_fmt(&camera);
	v4l2_reqbufs(&camera, 4);
	v4l2_stream(&camera, 1);




	if(codecType == VENC_CODEC_H264)
	{
		
		out_file = fopen("./test.h264", "wb");
		if(out_file == NULL)
		{
			loge("open out_file fail\n");
			return -1;
		}
	}
	else
	{
		
		out_file = fopen("./test.jpg", "wb");
		if(out_file == NULL)
		{
			loge("open out_file fail\n");
			return -1;
		}
	}

	memset(&baseConfig, 0 ,sizeof(VencBaseConfig));
	memset(&bufferParam, 0 ,sizeof(VencAllocateBufferParam));

	baseConfig.nInputWidth= src_width;
	baseConfig.nInputHeight = src_height;
	baseConfig.nStride = src_width;
	
	baseConfig.nDstWidth = dst_width;
	baseConfig.nDstHeight = dst_height;
	baseConfig.eInputFormat = VENC_PIXEL_YUV420SP;
	//baseConfig.eInputFormat = VENC_PIXEL_YVU420SP;

	bufferParam.nSizeY = baseConfig.nInputWidth*baseConfig.nInputHeight;
	bufferParam.nSizeC = baseConfig.nInputWidth*baseConfig.nInputHeight/2;
	bufferParam.nBufferNum = 4;
	
	RUN_TEST;

	//创建压缩引擎
	pVideoEnc = VideoEncCreate(codecType);

	RUN_TEST;

	if(codecType == VENC_CODEC_JPEG)
	{
		int JpegQuality = 1;
		//VENC_IndexParamJpegQuality
		VideoEncSetParameter(pVideoEnc, VENC_IndexParamJpegQuality, &JpegQuality);
	  	VideoEncSetParameter(pVideoEnc, VENC_IndexParamJpegExifInfo, &exifinfo);
		  RUN_TEST;
	}
	else if(codecType == VENC_CODEC_H264)
	{
		int value;

#if 0
		/* H264 param */
		VENC_IndexParamH264Param,						/**< reference type: VencH264Param */
		VENC_IndexParamH264SPSPPS,                    	/**< reference type: VencHeaderData (read only)*/
		VENC_IndexParamH264QPRange			= 0x100,	/**< reference type: VencQPRange */
		VENC_IndexParamH264ProfileLevel,              	/**< reference type: VencProfileLevel */
		VENC_IndexParamH264EntropyCodingCABAC,			/**< reference type: int(0:CAVLC 1:CABAC) */
		VENC_IndexParamH264CyclicIntraRefresh,			/**< reference type: VencCyclicIntraRefresh */
		VENC_IndexParamH264FixQP,						/**< reference type: VencH264FixQP */
#endif
		//设置压缩参数
		RUN_TEST;
		VideoEncSetParameter(pVideoEnc, VENC_IndexParamH264Param, &h264Param);

		value = 0;
		VideoEncSetParameter(pVideoEnc, VENC_IndexParamIfilter, &value);

		RUN_TEST;
		value = 0; //degree
		VideoEncSetParameter(pVideoEnc, VENC_IndexParamRotation, &value);

		VideoEncSetParameter(pVideoEnc, VENC_IndexParamH264FixQP, &fixQP);

		VideoEncSetParameter(pVideoEnc, VENC_IndexParamH264CyclicIntraRefresh, &sIntraRefresh);

		value = 720/4;
		VideoEncSetParameter(pVideoEnc, VENC_IndexParamSliceHeight, &value);

		//VideoEncSetParameter(pVideoEnc, VENC_IndexParamROIConfig, &sRoiConfig[0]);
		//VideoEncSetParameter(pVideoEnc, VENC_IndexParamROIConfig, &sRoiConfig[1]);
		//VideoEncSetParameter(pVideoEnc, VENC_IndexParamROIConfig, &sRoiConfig[2]);
		//VideoEncSetParameter(pVideoEnc, VENC_IndexParamROIConfig, &sRoiConfig[3]);
	}

	//初始化视频编码
	VideoEncInit(pVideoEnc, &baseConfig);

	
	if(codecType == VENC_CODEC_H264)
	{
		unsigned int head_num = 0;
		VideoEncGetParameter(pVideoEnc, VENC_IndexParamH264SPSPPS, &sps_pps_data);
		fwrite(sps_pps_data.pBuffer, 1, sps_pps_data.nLength, out_file);
		logd("sps_pps_data.nLength: %d", sps_pps_data.nLength);
		for(head_num=0; head_num<sps_pps_data.nLength; head_num++)
			logd("the sps_pps :%02x\n", *(sps_pps_data.pBuffer+head_num));
	}

	if(codecType == VENC_CODEC_JPEG)
	{
		testNumber = 1;
	}
	//申请输入缓冲区
	AllocInputBuffer(pVideoEnc, &bufferParam);

	unsigned char *frame_ptr = malloc(src_width*src_height*3);


	while(/*testNumber > 0*/1)
	{
		int frame_len;
		//usleep(30*1000);
		v4l2_read_frame(&camera, frame_ptr, &frame_len,&frame_ptr);
		if (frame_len <= 0)
			continue;

		GetOneAllocInputBuffer(pVideoEnc, &inputBuffer);
		memcpy(inputBuffer.pAddrVirY,frame_ptr,baseConfig.nInputWidth*baseConfig.nInputHeight);
		memcpy(inputBuffer.pAddrVirC,frame_ptr+baseConfig.nInputWidth*baseConfig.nInputHeight
		,baseConfig.nInputWidth*baseConfig.nInputHeight);

		#if 1
		inputBuffer.bEnableCorp = 0;
		inputBuffer.sCropInfo.nLeft =  240;
		inputBuffer.sCropInfo.nTop  =  240;
		inputBuffer.sCropInfo.nWidth  =  240;
		inputBuffer.sCropInfo.nHeight =  240;
		#endif

		FlushCacheAllocInputBuffer(pVideoEnc, &inputBuffer);

		AddOneInputBuffer(pVideoEnc, &inputBuffer);
		VideoEncodeOneFrame(pVideoEnc);

		AlreadyUsedInputBuffer(pVideoEnc,&inputBuffer);
		ReturnOneAllocInputBuffer(pVideoEnc, &inputBuffer);


		GetOneBitstreamFrame(pVideoEnc, &outputBuffer);
		printf("size: %d,%d", outputBuffer.nSize0,outputBuffer.nSize1);

		fwrite(outputBuffer.pData0, 1, outputBuffer.nSize0, out_file);

		if(outputBuffer.nSize1)
		{
			fwrite(outputBuffer.pData1, 1, outputBuffer.nSize1, out_file);
		}
	
		FreeOneBitStreamFrame(pVideoEnc, &outputBuffer);

		if(h264Param.nCodingMode==VENC_FIELD_CODING && codecType==VENC_CODEC_H264)
		{
			GetOneBitstreamFrame(pVideoEnc, &outputBuffer);
			printf("size: %d,%d", outputBuffer.nSize0,outputBuffer.nSize1);

			fwrite(outputBuffer.pData0, 1, outputBuffer.nSize0, out_file);

			if(outputBuffer.nSize1)
			{
				fwrite(outputBuffer.pData1, 1, outputBuffer.nSize1, out_file);
			}
				
			FreeOneBitStreamFrame(pVideoEnc, &outputBuffer);
		}

		testNumber--;
	}

out:	
	fclose(out_file);

	return 0;
}
