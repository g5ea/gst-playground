#pragma once

#include <cstring>
#include <sstream>
#include <vector>

#include <getopt.h>

static struct option longopts[] = {
    {"number-of-frames", required_argument, NULL, 'n'},
    {"frames-per-second", required_argument, NULL, 'f'},
    {"output-directory", required_argument, NULL, 'o'},
    {"help", no_argument, NULL, 'h'},
    {0}};

class Parser {
public:
  Parser(int arg, char *argv[]) {

    valid_ = true;
    // App name
    if (argv && argv[0]) {
      name_.assign(argv[0], strlen(argv[0] + 1));
    }
    // Reset getopt
    optind = 1;

    while (true) {

      const int opt = getopt_long(arg, argv, "n:f:o:", longopts, 0);

      if (opt == -1) {
        break;
      }

      switch (opt) {
      case 'n': {
        std::stringstream ss(optarg);
        if (!(ss >> num_frames_))
          valid_ = false;
        if (num_frames_ <= 0)
          valid_ = false;
        break;
      }
      case 'f': {
        std::stringstream ss(optarg);
        if (!(ss >> frame_rate_))
          valid_ = false;
        if (frame_rate_ <= 0)
          valid_ = false;
        break;
      }
      case 'o': {
        std::stringstream ss(optarg);
        if (!(ss >> output_dir_))
          valid_ = false;
        break;
      }
      case 'h': {
        valid_ = false;
        break;
      }
      default:
        // Ignore anything else
        break;
      };
    }
  }

  operator bool() const { return valid_; }

  int get_frame_rate() const { return frame_rate_; }

  int get_num_frames() const { return num_frames_; }

  std::string get_output_dir() const { return output_dir_; }

  std::string get_usage() const {
    std::vector<std::string> text = {"Number of frames to collect (default: 1)",
                                     "Frames per second (default: 1)",
                                     "Output directory (default: ./)",
                                     "This help"};
    std::stringstream ss;
    ss << "Usage:" << std::endl;
    ss << name_ << " [OPTIONS]" << std::endl;
    ss << std::endl;
    for (int i = 0; i < text.size(); ++i)
      ss << "--" << longopts[i].name << " or -"
         << std::string(1, longopts[i].val) << ", " << text.at(i) << std::endl;

    return ss.str();
  }

private:
  int frame_rate_ = 1;
  int num_frames_ = 1;
  std::string output_dir_ = "./";
  std::string name_;
  bool valid_;
};
