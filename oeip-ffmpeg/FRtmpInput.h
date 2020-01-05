#pragma once

#include "Oeipffmpeg.h"
#include "OeipFree.h"
#include <mutex>

//拉流
class OEIPFMDLL_EXPORT FRtmpInput
{
public:
	FRtmpInput();
	~FRtmpInput();
public:
	bool bTempOpen = false;
private:
	OAVFormatContext fmtCtx = nullptr;
	OAVCodecContext audioCtx = nullptr;
	OAVCodecContext videoCtx = nullptr;
	OeipVideoEncoder videoEncoder = {};
	OeipAudioEncoder audioEncoder = {};

	int32_t videoIndex = -1;
	int32_t audioIndex = -1;
	uint64_t audioTimestamp = 0;
	uint64_t videoTimestamp = 0;
	std::string url = "";
	bool bVideo = true;
	bool bAudio = true;
	bool bRtmp = true;
	bool bOpenPull = false;

	OAVFrame frame = nullptr;
	std::vector<uint8_t> videoData;

	std::mutex mtx;
	//信号量.
	std::condition_variable signal;

	onVideoDataHandle onVideoDataEvent;
private:
	void readPack();
public:
	int32_t openURL(const char* url, bool bVideo, bool bAudio);
	void close();

	void setVideoDataEvent(onVideoDataHandle onHandle);
};

