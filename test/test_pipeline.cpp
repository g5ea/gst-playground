#include <catch.hpp>

#include <gst/gst.h>

#include <data.hpp>

TEST_CASE("Pipeline has the correct number of frames set", "[Pipeline]") {
  gst_init(NULL, NULL);
  const int ref = 5;
  StreamData data(ref, 10, "./");

  gint num_frames = 0;
  REQUIRE(data);
  g_object_get(data.source, "num-buffers", &num_frames, NULL);

  REQUIRE(num_frames == ref);
}
