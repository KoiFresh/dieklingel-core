# Video - SplitterCamera driver

The classes SplitterCamera, SplitterSink and SplitterSource are implementing a custom video driver for [mediastreamer2](https://gitlab.linphone.org/BC/public/mediastreamer2). The driver is called `SplitterCamera`. The purpose of the driver is to use a single real camera and act like a `tee`filter, but with a custom ticker. A usual video graph could look like this:

```txt
V4L2 -> SizeConv -> Video Display
```

or with a Tee element:

```txt
V4L2 -> SizeConv -> Tee -> some video sink
                                |
                                + -> some other video sink
```

In both of the above examples, there is a single ticker for the whole graph. It is not possible for the creator of `some video sink` and `some other video sink` to use the samer `source` without knowing of each other, while both are thinking they are the only user of the `source`. Here comes the `SplitterCamera` driver. A `SplitterCamera` graph will look like the following:

```txt
V4L2 -> SizeConv -> SplitterSource
                               SplitterSink -> PixConv -> SizeConv -> some video sink
                               SplitterSink -> PixConv -> some other video sink
                               SplitterSink -> ... -> some other video sink
```

Its not a single graph with a single ticker, now there are 4 graphs, each with its own ticker. As soon as one of the `SplitterSink` graphs gets a ticker attached, the `SplitterSource` will also get a ticker attached and magically filling all connected `SplitterSink`s with an attached ticker. The SplitterSource could be registered for a singel real camera and will then show up as new camera device. Applications like linphone should then use the `SplitterCamera: V4L2: /dev/video0` device instead of the `V4L2: /dev/video0` device, which leads to no graph is blocking the real video device for other graphs, but will share/splitt the video device with other graphs, using the same `SplitterCamera`. You should note that Settings will be passed from the `SplitterSink` to the `SplitterSource` and then either delegated to the real camera or the `SizeConv` filter. So changed are made on one `SplitterSink` graph, alos have an impact on the other `SplitterSink` graphs. The reason we created this camera driver, is that the only option in liblinphone to select a camera is the camera id. And then during running media streams, the only option to interact withe the stream is `takeVideoSnapshot` into a file.
