#ifndef __FILTER_HPP__
#define __FILTER_HPP__

#include <QLoggingCategory>
#include <mediastreamer2/mscommon.h>
#include <mediastreamer2/msvideo.h>
#include <mediastreamer2/msfilter.h>
#include <mediastreamer2/mswebcam.h>
#include <mediastreamer2/msticker.h>
#include <QSet>

class SplitterSource
{
public:
	class State
	{
	public:
		MSTicker *ticker = nullptr;
		MSFilter *source = nullptr;
		MSFilter *pixconv = nullptr;
		MSFilter *sizeconv = nullptr;
		QSet<MSFilter *> *sinks;
	};

	static MSFilterMethod methods[];
	static MSFilterDesc description;

	static void attach(MSFilter *splitterSource, MSFilter *splitterSink);
	static void detach(MSFilter *splitterSource, MSFilter *splitterSink);

	// static MSFilter *fromWebCam(MSWebCam *camera);
};

#endif // __FILTER_HPP__
