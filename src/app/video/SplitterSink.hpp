#ifndef __SPLITTER_SINK_HPP__
#define __SPLITTER_SINK_HPP__

#include <QLoggingCategory>
#include <mediastreamer2/mscommon.h>
#include <mediastreamer2/msvideo.h>
#include <mediastreamer2/msfilter.h>

#include "SplitterSource.hpp"

class SplitterSink
{

public:
	class State
	{
	public:
		MSFilter *source = nullptr;
	};

	static MSFilterMethod methods[];
	static MSFilterDesc description;
};

#endif
