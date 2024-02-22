#ifndef __SPLITTER_SINK_HPP__
#define __SPLITTER_SINK_HPP__

#include <QLoggingCategory>
#include <mediastreamer2/mscommon.h>
#include <mediastreamer2/msvideo.h>
#include <mediastreamer2/msfilter.h>

#include "SplitterSource.hpp"
#include "SplitterCamera.hpp"

class SplitterSink
{
private:
	static void _init(MSFilter *filter);
	static void _preprocess(MSFilter *filter);
	static void _process(MSFilter *filter);
	static void _postprocess(MSFilter *filter);
	static void _uninit(MSFilter *filter);

	static int _setFps(MSFilter *filter, void *arg);
	static int _getFps(MSFilter *filter, void *arg);
	static int _setVideoSize(MSFilter *filter, void *arg);
	static int _getVideoSize(MSFilter *filter, void *arg);
	static int _getPixFmt(MSFilter *filter, void *arg);

public:
	class State
	{
	public:
		MSFilter *splitterSource = nullptr;
		MSFilter *parent = nullptr;
	};

	static MSFilterMethod methods[];
	static MSFilterDesc description;
};

#endif
