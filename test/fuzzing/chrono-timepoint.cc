// Copyright (c) 2021, Paul Dreik
// For license information refer to format.h.
#include <fmt/chrono.h>

#include "fuzzer-common.h"

/*
 * a fuzzer for the chrono timepoints formatters
 * C is a clock (std::chrono::system_clock etc)
 */
template <typename C> void doit(const uint8_t* data, size_t size) {
  using Rep = typename C::time_point::rep;
  constexpr auto N = sizeof(Rep);
  if (size < N) return;

  const auto x = assign_from_buf<Rep>(data);
  typename C::duration dur{x};
  typename C::time_point timepoint{dur};
  data += N;
  size -= N;
  data_to_string format_str(data, size);

  std::string message = fmt::format(format_str.get(), timepoint);
}

extern "C" int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
  try {
    doit<std::chrono::system_clock>(data, size);
  } catch (...) {
  }
  return 0;
}



TEST(color_test, format) {
  EXPECT_EQ(fmt::format(fmt::text_style(), "no style"), "no style");
  EXPECT_EQ(fmt::format(fg(fmt::rgb(255, 20, 30)), "rgb(255,20,30)"),
            "\x1b[38;2;255;020;030mrgb(255,20,30)\x1b[0m");
  EXPECT_EQ(fmt::format(fg(fmt::rgb(255, 0, 0)) | fg(fmt::rgb(0, 20, 30)),
                        "rgb(255,20,30)"),
            "\x1b[38;2;255;020;030mrgb(255,20,30)\x1b[0m");
  EXPECT_EQ(
      fmt::format(fg(fmt::rgb(0, 0, 0)) | fg(fmt::rgb(0, 0, 0)), "rgb(0,0,0)"),
      "\x1b[38;2;000;000;000mrgb(0,0,0)\x1b[0m");
  EXPECT_EQ(fmt::format(fg(fmt::color::blue), "blue"),
            "\x1b[38;2;000;000;255mblue\x1b[0m");
  EXPECT_EQ(
      fmt::format(fg(fmt::color::blue) | bg(fmt::color::red), "two color"),
      "\x1b[38;2;000;000;255m\x1b[48;2;255;000;000mtwo color\x1b[0m");
  EXPECT_EQ(fmt::format(fmt::emphasis::bold, "bold"), "\x1b[1mbold\x1b[0m");
  EXPECT_EQ(fmt::format(fmt::emphasis::faint, "faint"), "\x1b[2mfaint\x1b[0m");
  EXPECT_EQ(fmt::format(fmt::emphasis::italic, "italic"),
            "\x1b[3mitalic\x1b[0m");
  EXPECT_EQ(fmt::format(fmt::emphasis::underline, "underline"),
            "\x1b[4munderline\x1b[0m");
  EXPECT_EQ(fmt::format(fmt::emphasis::blink, "blink"), "\x1b[5mblink\x1b[0m");
  EXPECT_EQ(fmt::format(fmt::emphasis::reverse, "reverse"),
            "\x1b[7mreverse\x1b[0m");
  EXPECT_EQ(fmt::format(fmt::emphasis::conceal, "conceal"),
            "\x1b[8mconceal\x1b[0m");
  EXPECT_EQ(fmt::format(fmt::emphasis::strikethrough, "strikethrough"),
            "\x1b[9mstrikethrough\x1b[0m");
  EXPECT_EQ(
      fmt::format(fg(fmt::color::blue) | fmt::emphasis::bold, "blue/bold"),
      "\x1b[1m\x1b[38;2;000;000;255mblue/bold\x1b[0m");
  EXPECT_EQ(fmt::format(fmt::emphasis::bold, "bold error"),
            "\x1b[1mbold error\x1b[0m");
  EXPECT_EQ(fmt::format(fg(fmt::color::blue), "blue log"),
            "\x1b[38;2;000;000;255mblue log\x1b[0m");
  EXPECT_EQ(fmt::format(fmt::text_style(), "hi"), "hi");
  EXPECT_EQ(fmt::format(fg(fmt::terminal_color::red), "tred"),
            "\x1b[31mtred\x1b[0m");
  EXPECT_EQ(fmt::format(bg(fmt::terminal_color::cyan), "tcyan"),
            "\x1b[46mtcyan\x1b[0m");
  EXPECT_EQ(fmt::format(fg(fmt::terminal_color::bright_green), "tbgreen"),
            "\x1b[92mtbgreen\x1b[0m");
  EXPECT_EQ(fmt::format(bg(fmt::terminal_color::bright_magenta), "tbmagenta"),
            "\x1b[105mtbmagenta\x1b[0m");
  EXPECT_EQ(fmt::format(fg(fmt::terminal_color::red), "{}", "foo"),
            "\x1b[31mfoo\x1b[0m");
  EXPECT_EQ(fmt::format("{}{}", fmt::styled("red", fg(fmt::color::red)),
                        fmt::styled("bold", fmt::emphasis::bold)),
            "\x1b[38;2;255;000;000mred\x1b[0m\x1b[1mbold\x1b[0m");
  EXPECT_EQ(fmt::format("{}", fmt::styled("bar", fg(fmt::color::blue) |
                                                     fmt::emphasis::underline)),
            "\x1b[4m\x1b[38;2;000;000;255mbar\x1b[0m");
  EXPECT_EQ(
      fmt::format(
          "{}", fmt::styled(
                    "all", fmt::emphasis::bold | fmt::emphasis::faint |
                               fmt::emphasis::italic |
                               fmt::emphasis::underline | fmt::emphasis::blink |
                               fmt::emphasis::reverse | fmt::emphasis::conceal |
                               fmt::emphasis::strikethrough)),
      "\x1b[1;2;3;4;5;7;8;9mall\x1b[0m");
}

TEST(color_test, format_to) {
  auto out = std::string();
  fmt::format_to(std::back_inserter(out), fg(fmt::rgb(255, 20, 30)),
                 "rgb(255,20,30){}{}{}", 1, 2, 3);
  EXPECT_EQ(fmt::to_string(out),
            "\x1b[38;2;255;020;030mrgb(255,20,30)123\x1b[0m");
}

TEST(color_test, print) {
  EXPECT_WRITE(stdout, fmt::print(fg(fmt::rgb(255, 20, 30)), "rgb(255,20,30)"),
               "\x1b[38;2;255;020;030mrgb(255,20,30)\x1b[0m");
}

TEST(color_test, println) {
  EXPECT_WRITE(stdout,
               fmt::println(fg(fmt::rgb(255, 20, 30)), "rgb(255,20,30)"),
               "\x1b[38;2;255;020;030mrgb(255,20,30)\x1b[0m\n");
  EXPECT_WRITE(stdout, fmt::println(fmt::emphasis::bold, "bold"),
               "\x1b[1mbold\x1b[0m\n");
  EXPECT_WRITE(
      stdout,
      fmt::println(fg(fmt::color::blue) | fmt::emphasis::bold, "blue/bold"),
      "\x1b[1m\x1b[38;2;000;000;255mblue/bold\x1b[0m\n");
}
