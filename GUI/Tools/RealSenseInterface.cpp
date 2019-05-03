#include "RealSenseInterface.h"
#include <functional>
#include <string>
#include <cmath>

static void pt_loop(int n, RealSenseInterface* interfaceRS)
{
	while (true)
	{
		rs2::frameset frames = interfaceRS->pipe.wait_for_frames();

		rs2::frame depthFrame = frames.get_depth_frame();
		rs2::frame colorFrame = frames.get_color_frame();

		(*(interfaceRS->depthCallback))(depthFrame);
		(*(interfaceRS->rgbCallback))(colorFrame);
	}
}

void RealSenseInterface::Update()
{
	rs2::frameset frames = pipe.wait_for_frames();

	rs2::frame depthFrame = frames.get_depth_frame();
	rs2::frame colorFrame = frames.get_color_frame();

	(*(depthCallback))(depthFrame);
	(*(rgbCallback))(colorFrame);

}


#ifdef WITH_REALSENSE
RealSenseInterface::RealSenseInterface(int inWidth, int inHeight, int inFps)
	: width(inWidth),
	height(inHeight),
	fps(inFps),
	dev(nullptr),
	initSuccessful(true),
	pipe(),
	rgbCallback(nullptr),
	depthCallback(nullptr)
{
	std::cout << "RealSenseInterface: " << inWidth << ", " << inHeight << ", " << inFps << "\n";

	auto list = ctx.query_devices();
	if (list.size() == 0)
	{
		errorText = "No device connected.";
		initSuccessful = false;
		return;
	}

	dev = std::shared_ptr<rs2::device> (&list.front());
	
	rs2::config cfg;
	cfg.enable_stream(RS2_STREAM_DEPTH, width, height, RS2_FORMAT_Z16, fps);
	cfg.enable_stream(RS2_STREAM_COLOR, width, height, RS2_FORMAT_RGB8, fps);
	pipe.start(cfg);

	latestDepthIndex.assign(-1);
	latestRgbIndex.assign(-1);

	for (int i = 0; i < numBuffers; i++)
	{
		uint8_t * newImage = (uint8_t *)calloc(width * height * 3, sizeof(uint8_t));
		rgbBuffers[i] = std::pair<uint8_t *, int64_t>(newImage, 0);
	}

	for (int i = 0; i < numBuffers; i++)
	{
		uint8_t * newDepth = (uint8_t *)calloc(width * height * 2, sizeof(uint8_t));
		uint8_t * newImage = (uint8_t *)calloc(width * height * 3, sizeof(uint8_t));
		frameBuffers[i] = std::pair<std::pair<uint8_t *, uint8_t *>, int64_t>(std::pair<uint8_t *, uint8_t *>(newDepth, newImage), 0);
	}

	//setAutoExposure(true);
	//setAutoWhiteBalance(true);

	rgbCallback = std::shared_ptr<RGBCallback> ( new RGBCallback(lastRgbTime,
		latestRgbIndex,
		rgbBuffers) );

	depthCallback = std::shared_ptr<DepthCallback> ( new DepthCallback(lastDepthTime,
		latestDepthIndex,
		latestRgbIndex,
		rgbBuffers,
		frameBuffers) );
	//pt = std::thread (pt_loop, 0, this);
	//pt.join();
}

RealSenseInterface::~RealSenseInterface()
{
	if (initSuccessful)
	{
		pt.detach();

		for (int i = 0; i < numBuffers; i++)
		{
			free(rgbBuffers[i].first);
		}

		for (int i = 0; i < numBuffers; i++)
		{
			free(frameBuffers[i].first.first);
			free(frameBuffers[i].first.second);
		}
	}
}

void RealSenseInterface::setAutoExposure(bool value)
{	
	for (auto&& s : dev->query_sensors())
	{
		if (s.supports(RS2_CAMERA_INFO_NAME))
		{
			auto name = std::string ( s.get_info(RS2_CAMERA_INFO_NAME) );
			if (	( name.find ( "RGB" ) != std::string::npos ) && 
					s.supports(RS2_OPTION_ENABLE_AUTO_EXPOSURE))
			{
				s.set_option(RS2_OPTION_ENABLE_AUTO_EXPOSURE, value);
				return;
			}
		}
	}
}

void RealSenseInterface::setAutoWhiteBalance(bool value)
{
	for (auto&& s : dev->query_sensors())
	{
		if (s.supports(RS2_CAMERA_INFO_NAME))
		{
			auto name = std::string(s.get_info(RS2_CAMERA_INFO_NAME));
			if ((name.find("RGB") != std::string::npos) &&
				s.supports(RS2_OPTION_ENABLE_AUTO_WHITE_BALANCE))
			{
				s.set_option(RS2_OPTION_ENABLE_AUTO_WHITE_BALANCE, value);
				return;
			}
		}
	}
}

bool RealSenseInterface::getAutoExposure()
{
	for (auto&& s : dev->query_sensors())
	{
		if (s.supports(RS2_CAMERA_INFO_NAME))
		{
			auto name = std::string(s.get_info(RS2_CAMERA_INFO_NAME));
			if ((name.find("RGB") != std::string::npos) &&
				s.supports(RS2_OPTION_ENABLE_AUTO_EXPOSURE))
			{
				auto rf = s.get_option(RS2_OPTION_ENABLE_AUTO_EXPOSURE);
				return (!isnan(rf)) && (rf > std::numeric_limits<float>::epsilon()) ;
			}
		}
	}
	return false;
}

bool RealSenseInterface::getAutoWhiteBalance()
{
	for (auto&& s : dev->query_sensors())
	{
		if (s.supports(RS2_CAMERA_INFO_NAME))
		{
			auto name = std::string(s.get_info(RS2_CAMERA_INFO_NAME));
			if ((name.find("RGB") != std::string::npos) &&
				s.supports(RS2_OPTION_ENABLE_AUTO_WHITE_BALANCE))
			{
				auto rf = s.get_option(RS2_OPTION_ENABLE_AUTO_WHITE_BALANCE);
				return (!isnan(rf)) && (rf > std::numeric_limits<float>::epsilon());
			}
		}
	}
	return false;
}
#else

RealSenseInterface::RealSenseInterface(int inWidth, int inHeight, int inFps)
	: width(inWidth),
	height(inHeight),
	fps(inFps),
	initSuccessful(false)
{
	errorText = "Compiled without Intel RealSense library";
}

RealSenseInterface::~RealSenseInterface()
{
}

void RealSenseInterface::setAutoExposure(bool value)
{
}

void RealSenseInterface::setAutoWhiteBalance(bool value)
{
}

bool RealSenseInterface::getAutoExposure()
{
	return false;
}

bool RealSenseInterface::getAutoWhiteBalance()
{
	return false;
}
#endif