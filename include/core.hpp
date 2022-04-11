#pragma once

#include <string>

const std::string prefix = "images_";

int setup_pipeline_and_run(const int num_frames, const int frame_rate,
                           const std::string &output_dir);
