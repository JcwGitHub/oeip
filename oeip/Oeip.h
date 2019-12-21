#pragma once

#ifdef OEIP_EXPORT 
#define OEIPDLL_EXPORT __declspec(dllexport) 
#else
#define OEIPDLL_EXPORT __declspec(dllimport)
#endif

#include "OeipDefine.h"
#include <functional>

enum OeipLogLevel : int32_t
{
	OEIP_INFO,
	OEIP_WARN,
	OEIP_ERROR,
	OEIP_ALORT,
};

enum OeipVideoType : int32_t
{
	OEIP_VIDEO_OTHER,
	OEIP_VIDEO_NV12,//OEIP_YUVFMT_YUV420SP
	OEIP_VIDEO_YUY2,//OEIP_YUVFMT_YUY2I
	OEIP_VIDEO_YVYU,//OEIP_YUVFMT_YVYUI
	OEIP_VIDEO_UYVY,//OEIP_YUVFMT_UYVYI
	//设定自动编码成OEIP_VIDEO_YUY2
	OEIP_VIDEO_MJPG,
	OEIP_VIDEO_RGB24,
	OEIP_VIDEO_ARGB32,
	OEIP_VIDEO_RGBA32,
	OEIP_VIDEO_DEPTH,//U16
};

//Planar(YUV各自分开)Semi-Planar(Y单独分开,UV交并)Interleaved(YUV交并)
enum OeipYUVFMT : int32_t
{
	OEIP_YUVFMT_OTHER,
	OEIP_YUVFMT_YUV420SP,//Semi-Planar
	OEIP_YUVFMT_YUY2I,//Interleaved 设备
	OEIP_YUVFMT_YVYUI,//Interleaved
	OEIP_YUVFMT_UYVYI,//Interleaved
	OEIP_YUVFMT_YUY2P,//Planar 一般用于传输
	OEIP_YUVFMT_YUV420P,//Planar 一般用于传输
};

enum OeipLayerType : int32_t
{
	OEIP_NONE_LAYER,
	OEIP_INPUT_LAYER,
	OEIP_YUV2RGBA_LAYER,
	OEIP_MAPCHANNEL_LAYER,
	OEIP_RGBA2YUV_LAYER,
	OEIP_OUTPUT_LAYER,
	OEIP_MAX_LAYER,
};

#define AllLayerParamet void, InputParamet, YUV2RGBAParamet, MapChannelParamet, void, OutputParamet, void

enum OeipGpgpuType : int32_t
{
	OEIP_GPGPU_OTHER,
	OEIP_DX11,
	OEIP_CUDA,
	OEIP_Vulkun,
};

enum OeipAudioRecordType : int32_t
{
	OEIP_Mic = 1,
	OEIP_Loopback = 2,
	OEIP_Mic_Loopback = 3
};

enum OeipDeviceEventType :int32_t
{
	OEIP_Event_Other,
	OEIP_DeviceStop,
	//打开成功
	OEIP_DeviceOpen,
	//打开失败
	OEIP_DeviceNoOpen,
	//掉线
	OEIP_DeviceDropped,
};

enum VideoDeviceType : int32_t
{
	OEIP_VideoDevice_Other,
	OEIP_MF,
	OEIP_Decklink,
	OEIP_Realsense,
	OEIP_Virtual,
};

//C#/C++里bool都只有一字节，但是可能因为不同对齐方式导致差异，故与C#交互的结构bool全使用int32
//默认从CPU输入,如果要支持GPU输入,bGpu=true
struct InputParamet
{
	int32_t bCpu = true;
	int32_t bGpu = false;
};

struct OutputParamet
{
	int32_t bCpu = true;
	int32_t bGpu = true;
};

struct YUV2RGBAParamet
{
	OeipYUVFMT yuvType = OEIP_YUVFMT_YUV420SP;
};

//ARGB<->BGRA<->RGBA<->RRRR
struct MapChannelParamet
{
	uint32_t red = 0;
	uint32_t green = 1;
	uint32_t blue = 2;
	uint32_t alpha = 4;
};

struct ResizeParamet
{
	int32_t bLinear = true;
	int32_t width = 1920;
	int32_t height = 1080;
};

struct VideoFormat
{
	int32_t index = -1;
	int32_t width = 0;
	int32_t height = 0;
	OeipVideoType videoType = OEIP_VIDEO_OTHER;
	int32_t fps = 0;
};

struct Parametr
{
	long CurrentValue;
	long Min;
	long Max;
	long Step;
	long Default;
	long Flag;
};

struct CamParametrs
{
	Parametr Brightness;
	Parametr Contrast;
	Parametr Hue;
	Parametr Saturation;
	Parametr Sharpness;
	Parametr Gamma;
	Parametr ColorEnable;
	Parametr WhiteBalance;
	Parametr BacklightCompensation;
	Parametr Gain;

	Parametr Pan;
	Parametr Tilt;
	Parametr Roll;
	Parametr Zoom;
	Parametr Exposure;
	Parametr Iris;
	Parametr Focus;
};

struct OeipDeviceInfo
{
	int32_t id = -1;
	wchar_t deviceName[512];
	wchar_t deviceID[512];
};


struct OeipDateDesc
{
	int32_t elementSize;
	int32_t elementChannel;
};

//日志回调
typedef void(*logEventAction)(int32_t level, const char* message);
typedef std::function<void(int32_t, const char*)> logEventHandle;

//截取的声音回调
typedef void(*onAudioRecordAction)(void* data, int32_t dataLen, int32_t sampleRate, int32_t numChannels);
typedef std::function<void(void*, int32_t, int32_t, int32_t)> onAudioRecordHandle;

//设备事件，如中断等
typedef void(*onEventAction)(OeipDeviceEventType type, int32_t code);
typedef std::function<void(OeipDeviceEventType, int32_t)> onEventHandle;

//摄像机的数据处理回调，dataType指明data数据类型
typedef void(*onReviceAction)(uint8_t* data, int32_t width, int32_t height);
typedef std::function<void(uint8_t*, int32_t, int32_t)> onReviceHandle;

typedef void(*onProcessAction)(int32_t layerIndex, uint8_t* data, int32_t width, int32_t height, int32_t outputIndex);
typedef std::function<void(int32_t, uint8_t*, int32_t, int32_t, int32_t)> onProcessHandle;