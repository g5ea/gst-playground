#include <chrono>
#include <fstream>
#include <iomanip>

#include <gst/gst.h>

#include <core.hpp>
#include <data.hpp>
#include <gstmark.h>

static std::time_t
get_time_t(const std::chrono::system_clock::time_point &now) {
  return std::chrono::system_clock::to_time_t(now);
}

static int get_milli(const std::chrono::system_clock::time_point &now) {
  auto seconds = std::chrono::time_point_cast<std::chrono::seconds>(now);
  auto fraction = now - seconds;
  auto milliseconds =
      std::chrono::duration_cast<std::chrono::milliseconds>(fraction);
  return static_cast<int>(milliseconds.count());
}

static std::string get_timestamp(const std::time_t &t, const int milli) {
  std::stringstream ss;
  ss << std::put_time(std::localtime(&t), "%Y%m%d_%H%M%S_") << milli;

  return ss.str();
}

static GstFlowReturn new_sample_handler(GstElement *sink, gpointer data) {
  GstSample *sample;
  StreamData *stream_data = reinterpret_cast<StreamData *>(data);

  /* Retrieve the buffer, there should be one */
  g_signal_emit_by_name(sink, "pull-sample", &sample);
  if (sample) {
    GstMapInfo map;
    GstBuffer *buffer = gst_sample_get_buffer(sample);
    if (buffer && gst_buffer_map(buffer, &map, GST_MAP_READ)) {
      // Get timestamp from the buffer
      GstMetaMarking *meta = GST_META_MARKING_GET(buffer);
      std::stringstream filename;
      // Generate filename and save it
      filename << stream_data->output_dir << "/";
      filename << prefix;
      filename << get_timestamp(meta->in_timestamp.t, meta->in_timestamp.milli);
      filename << ".png";
      GST_INFO("pull-sample filename: %s\n", filename.str().c_str());
      std::ofstream file(filename.str(), std::ios::binary);
      file.write(reinterpret_cast<const char *>(map.data), map.size);
      gst_buffer_unmap(buffer, &map);
    }
    gst_sample_unref(sample);
    return GST_FLOW_OK;
  }
  return GST_FLOW_ERROR;
}

static void handoff_handler(GstElement *identity, GstBuffer *buffer, void *) {
  // Record current time
  const std::chrono::system_clock::time_point now =
      std::chrono::system_clock::now();
  // Add timestamp into the buffer
  GstMetaMarking *meta = GST_META_MARKING_ADD(buffer);
  meta->in_timestamp.t = get_time_t(now);
  meta->in_timestamp.milli = get_milli(now);
  GST_INFO(
      "handoff timestamp: %s\n",
      get_timestamp(meta->in_timestamp.t, meta->in_timestamp.milli).c_str());
}

static gboolean bus_msg_handler(GstBus *bus, GstMessage *msg, gpointer data) {
  GMainLoop *loop = reinterpret_cast<GMainLoop *>(data);

  const GstStructure *s;
  // Handle only basic messages
  switch (GST_MESSAGE_TYPE(msg)) {
  case GST_MESSAGE_ERROR:
    GError *err;
    gchar *debug_info;
    gst_message_parse_error(msg, &err, &debug_info);
    g_printerr("Error received from element %s: %s\n",
               GST_OBJECT_NAME(msg->src), err->message);
    g_printerr("Debugging information: %s\n", debug_info ? debug_info : "none");
    g_clear_error(&err);
    g_free(debug_info);
    g_main_loop_quit(loop);
    break;
  case GST_MESSAGE_EOS:
    GST_INFO("Eos received from element: %s\n", GST_OBJECT_NAME(msg->src));
    g_main_loop_quit(loop);
    break;
  default:
    break;
  };
  return TRUE;
}

int setup_pipeline_and_run(const int num_frames, const int frame_rate,
                           const std::string &dir) {
  GstBus *bus = nullptr;

  // Gstreamer initialization
  gst_init(NULL, NULL);

  // Stream data initizialitation
  StreamData data(num_frames, frame_rate, dir);

  if (!data) {
    g_printerr("Invalid stream initialization.\n");
    return -1;
  }

  data.loop = g_main_loop_new(NULL, FALSE);
  if (!data.loop) {
    g_printerr("Unable to create main loop.\n");
    return -1;
  }

  // Bus message handler
  bus = gst_pipeline_get_bus(GST_PIPELINE(data.pipeline));
  guint bus_watch_id = gst_bus_add_watch(bus, bus_msg_handler, data.loop);
  gst_object_unref(bus);

  // Add all to the bin and link the elements together
  gst_bin_add_many(GST_BIN(data.pipeline), data.source, data.video_caps,
                   data.marker, data.encoder, data.queue, data.sink, NULL);
  if (!gst_element_link_many(data.source, data.video_caps, data.marker,
                             data.encoder, data.queue, data.sink, NULL)) {
    g_printerr("Elements could not be linked");
    return -1;
  }

  // Marker callback to timestamp buffer coming from the src pad
  g_object_set(G_OBJECT(data.marker), "signal-handoffs", TRUE, NULL);
  g_signal_connect(data.marker, "handoff", G_CALLBACK(handoff_handler), NULL);

  // App message to deal with buffers, there is a queue in front of it to
  // allievate timing constraints
  g_object_set(data.sink, "emit-signals", TRUE, NULL);
  g_signal_connect(data.sink, "new-sample", G_CALLBACK(new_sample_handler),
                   &data);

  // Start playing
  gst_element_set_state(data.pipeline, GST_STATE_PLAYING);

  // Main loop can start too
  g_main_loop_run(data.loop);

  gst_object_unref(bus);
  gst_element_set_state(data.pipeline, GST_STATE_NULL);

  return 0;
}
