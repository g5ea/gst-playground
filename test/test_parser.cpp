#include <catch.hpp>

#include <parser.hpp>

std::vector<char *> get_argv(const std::vector<std::string> &args) {
  std::vector<char *> argv;
  for (const auto &arg : args)
    argv.push_back((char *)arg.data());
  argv.push_back(nullptr);

  return argv;
}

TEST_CASE("Incorrect number of frames: negative", "[Parser]") {
  std::vector<std::string> args = {"exe", "-n", "-1", "-f", "5", "-o", "./"};

  auto argv = get_argv(args);
  Parser parser(argv.size() - 1, argv.data());

  REQUIRE(!parser);
}

TEST_CASE("Incorrect number of frames: string", "[Parser]") {
  std::vector<std::string> args = {"exe", "-n", "two", "-f", "5", "-o", "./"};

  auto argv = get_argv(args);
  Parser parser(argv.size() - 1, argv.data());

  REQUIRE(!parser);
}

TEST_CASE("Correct number of frames: long option", "[Parser]") {
  std::vector<std::string> args = {
      "exe", "--number-of-frames", "1", "-f", "5", "-o", "./"};

  auto argv = get_argv(args);
  Parser parser(argv.size() - 1, argv.data());

  REQUIRE(parser);
}

TEST_CASE("Correct number of frames: short option", "[Parser]") {
  std::vector<std::string> args = {"exe", "-n", "1", "-f", "5", "-o", "./"};

  auto argv = get_argv(args);
  Parser parser(argv.size() - 1, argv.data());

  REQUIRE(parser);
}

TEST_CASE("Incorrect frame rate: negative", "[Parser]") {
  std::vector<std::string> args = {"exe", "-n", "1", "-f", "-5", "-o", "./"};

  auto argv = get_argv(args);
  Parser parser(argv.size() - 1, argv.data());

  REQUIRE(!parser);
}

TEST_CASE("Incorrect frame rate: string", "[Parser]") {
  std::vector<std::string> args = {"exe", "-n", "1", "-f", "ten", "-o", "./"};

  auto argv = get_argv(args);
  Parser parser(argv.size() - 1, argv.data());

  REQUIRE(!parser);
}

TEST_CASE("Correct frame rate: long option", "[Parser]") {
  std::vector<std::string> args = {
      "exe", "--number-of-frames", "1", "--frames-per-second", "5", "-o", "./"};

  auto argv = get_argv(args);
  Parser parser(argv.size() - 1, argv.data());

  REQUIRE(parser);
}

TEST_CASE("Fewer input parameter", "[Parser]") {
  std::vector<std::string> args = {"exe", "-n", "1", "-f", "10"};

  auto argv = get_argv(args);
  Parser parser(argv.size() - 1, argv.data());

  REQUIRE(parser);
}

TEST_CASE("Swizzle input parameter", "[Parser]") {
  std::vector<std::string> args = {
      "exe", "--output-directory", "./", "-f", "1", "-n", "10"};

  auto argv = get_argv(args);
  Parser parser(argv.size() - 1, argv.data());

  REQUIRE(parser);
}
