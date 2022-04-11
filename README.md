# save-frames

Example command line application to acquire images from video source (i.e. pylonsrc).

```
     _______      ________      _______      _______      ________
    |       |    |        |    |       |    |       |    |        |
    |  Src  | -> | Marker | -> |  Enc  | -> | Queue | -> |  Sink  |
    |_______|    |________|    |_______|    |_______|    |________|
                             ^                                |
                             |                                V
                       Buffer has been               Save buffer content
                        timestamped                    with timestamp
```

Marker is an identity element that adds custom metadata to each buffer. Custom metadata contains time information of when the buffer is handed off. Sink application retrieves custom metadata from the buffer and uses it to generate the timestamp.

It saves images in png format with timestamp:
```
images_YYYYMMDD_HHMMSS_ms.png
```

Files are located in:
```
<your_path>/gst-playground/src
```
and
```
<your_path>/gst-playground/include
```

Main loop and handlers are in:
```
<your_path>/gst-playground/src/core.cpp
```

Pipeline and stream data are defined in:
```
<your_path>/gst-playground/include/data.hpp
```

Command line parser is defined in:
```
<your_path>/gst-playground/include/parser.hpp
```

Custom metadata mark is in:
```
<your_path>/gst-playground/src/gstmark.c
<your_path>/gst-playground/include/gstmark.h
```

Tests are located in:
```
<your_path>/gst-playground/test/
```

## Dependencies

- GStreamer 1.16.x or newer
- gst-plugins-vision [GStreamer plugins for machine vision][1]
- pylonsrc: Video source for [Basler Pylon sources][2] (GigE Vision, USB3 Vision)
- Catch v2.13.8 [Catch2 Unit Tests][3]

## Installation

- Install GStreamer 1.16.x or newer
- Install specific frame grabber SDKs [Basler Pylon sources][3]
- Build project gst-plugins-vision [GStreamer plugins for machine vision][1]
- Create an environment variable `GST_PLUGIN_PATH` that points to where plugins are
- Create an environment variable `PYLON_CAMEMU` set to 1 (i.e. number of emulated camereras) if real hadware is not available

## Examples

Save 10 frames at 5 frames per second in the current directory:
>`./save-frames -n 10 -f 5 -o ./`

## Compiling

### Ubuntu

```
cd gst-playground
mkdir build
cd build
cmake ..
make
```

## Testing

In order to build unit tests it needs [Ctach2][3]. Git clone it in:
```
<your_path>/gst-playground/test/lib
```

And then:
```
cd gst-playground
mkdir build
cd build
cmake -DENABLE_TEST=ON ..
make
```

In oder to run unit tests:
```
cd gst-playground
cd  build
./test/test-save-frames
```

--------

[1]: https://github.com/joshdoe/gst-plugins-vision
[2]: https://www.baslerweb.com/
[3]: https://github.com/catchorg/Catch2
