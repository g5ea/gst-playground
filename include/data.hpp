#pragma once

#include <gst/gst.h>

struct StreamData {

  StreamData(const int num_frames, const int frame_rate,
             const std::string &dir) {
    valid = true;

    // clang-format off
    // Build up the pipeline:
    //
    //   _______      ________      _______      _______      ________
    //  |       |    |        |    |       |    |       |    |        |
    //  |  Src  | -> | Marker | -> |  Enc  | -> | Queue | -> |  Sink  |
    //  |_______|    |________|    |_______|    |_______|    |________|
    //                           ^                                |
    //                           |                                V
    //                     Buffer has been               Save buffer content
    //                      timestamped                    with timestamp
    //
    // clang-format on
    source = gst_element_factory_make("pylonsrc", "source");
    video_caps = gst_element_factory_make("capsfilter", "video_caps");
    marker = gst_element_factory_make("identity", "marker");
    encoder = gst_element_factory_make("pngenc", "encoder");
    queue = gst_element_factory_make("queue", "queue");
    sink = gst_element_factory_make("appsink", "sink");
    output_dir = dir;

    pipeline = gst_pipeline_new("main-pipeline");

    if (!pipeline || !source || !video_caps || !marker || !encoder || !queue ||
        !sink) {
      valid = false;
      GST_ERROR("Can't create pipeline element/s.\n");
      return;
    }

    // Pylon
    g_object_set(source, "num-buffers", num_frames, NULL);
    g_object_set(source, "testimage", 1, NULL);

    // video_caps settings
    std::string video_caps_info =
        "video/x-raw,framerate=" + std::to_string(frame_rate) + "/1";
    GstCaps *caps = gst_caps_from_string(video_caps_info.c_str());
    if (!caps) {
      GST_ERROR("Can't generate caps.\n");
      return;
    }

    g_object_set(G_OBJECT(video_caps), "caps", caps, NULL);
    gst_caps_unref(caps);
  }

  ~StreamData() {
    if (pipeline)
      gst_object_unref(pipeline);
  }

  operator bool() const { return valid; };

  GMainLoop *loop;
  GstElement *pipeline;
  GstElement *source;
  GstElement *video_caps;
  GstElement *marker;
  GstElement *queue;
  GstElement *encoder;
  GstElement *sink;
  std::string output_dir;
  bool valid;
};
