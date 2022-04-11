#include <filesystem>
#include <iostream>
#include <sstream>

#include <core.hpp>
#include <parser.hpp>

static bool check_num_frames(const int n, const std::string &dir) {
  int found = 0;
  const std::filesystem::path path(dir);
  for (auto const &dir_entry : std::filesystem::directory_iterator{path}) {
    std::stringstream ss;
    ss << dir_entry;
    if (ss.str().find(prefix) != std::string::npos) {
      ++found;
    }
  }

  if (found < n)
    return false;

  return true;
}

int main(int arg, char *argv[]) {
  Parser parser(arg, argv);

  if (!parser) {
    std::cout << "Error parsing the command." << std::endl;
    std::cout << parser.get_usage() << std::endl;
    return -1;
  }

  // Blocking until main loop terminates
  if (setup_pipeline_and_run(parser.get_num_frames(), parser.get_frame_rate(),
                             parser.get_output_dir())) {
    std::cout << "Error running the pipeline." << std::endl;
    return -1;
  }

  // Weak self-check : report only if the number of files is less than number of
  // frames
  if (!check_num_frames(parser.get_num_frames(), parser.get_output_dir())) {
    std::cout << "Number of frames does not match." << std::endl;
    return -1;
  }

  return 0;
}
