// Formatting library for C++ - color tests
//
// Copyright (c) 2012 - present, Victor Zverovich and {fmt} contributors
// All rights reserved.
//
// For the license information refer to format.h.

#include "fmt/color.h"

#include <iterator>  // std::back_inserter

#include "gtest-extra.h"  // EXPECT_WRITE, EXPECT_THROW_MSG

TEST(color_test, text_style) {
  EXPECT_FALSE(fmt::text_style().has_foreground());
  EXPECT_FALSE(fmt::text_style().has_background());
  EXPECT_FALSE(fmt::text_style().has_emphasis());

  EXPECT_TRUE(fg(fmt::rgb(0)).has_foreground());
  EXPECT_FALSE(fg(fmt::rgb(0)).has_background());
  EXPECT_FALSE(fg(fmt::rgb(0)).has_emphasis());
  EXPECT_TRUE(bg(fmt::rgb(0)).has_background());
  EXPECT_FALSE(bg(fmt::rgb(0)).has_foreground());
  EXPECT_FALSE(bg(fmt::rgb(0)).has_emphasis());

  EXPECT_TRUE(
      (fg(fmt::rgb(0xFFFFFF)) | bg(fmt::rgb(0xFFFFFF))).has_foreground());
  EXPECT_TRUE(
      (fg(fmt::rgb(0xFFFFFF)) | bg(fmt::rgb(0xFFFFFF))).has_background());
  EXPECT_FALSE(
      (fg(fmt::rgb(0xFFFFFF)) | bg(fmt::rgb(0xFFFFFF))).has_emphasis());

  EXPECT_EQ(fg(fmt::rgb(0x000000)) | fg(fmt::rgb(0x000000)),
            fg(fmt::rgb(0x000000)));
  EXPECT_EQ(fg(fmt::rgb(0x00000F)) | fg(fmt::rgb(0x00000F)),
            fg(fmt::rgb(0x00000F)));
  EXPECT_EQ(fg(fmt::rgb(0xC0F000)) | fg(fmt::rgb(0x000FEE)),
            fg(fmt::rgb(0xC0FFEE)));

  EXPECT_THROW_MSG(
      fg(fmt::terminal_color::black) | fg(fmt::terminal_color::black),
      fmt::format_error, "can't OR a terminal color");
  EXPECT_THROW_MSG(
      fg(fmt::terminal_color::black) | fg(fmt::terminal_color::white),
      fmt::format_error, "can't OR a terminal color");
  EXPECT_THROW_MSG(
      bg(fmt::terminal_color::black) | bg(fmt::terminal_color::black),
      fmt::format_error, "can't OR a terminal color");
  EXPECT_THROW_MSG(
      bg(fmt::terminal_color::black) | bg(fmt::terminal_color::white),
      fmt::format_error, "can't OR a terminal color");
  EXPECT_THROW_MSG(fg(fmt::terminal_color::black) | fg(fmt::color::black),
                   fmt::format_error, "can't OR a terminal color");
  EXPECT_THROW_MSG(bg(fmt::terminal_color::black) | bg(fmt::color::black),
                   fmt::format_error, "can't OR a terminal color");

  EXPECT_NO_THROW(fg(fmt::terminal_color::white) |
                  bg(fmt::terminal_color::white));
  EXPECT_NO_THROW(fg(fmt::terminal_color::white) | bg(fmt::rgb(0xFFFFFF)));
  EXPECT_NO_THROW(fg(fmt::terminal_color::white) | fmt::text_style());
  EXPECT_NO_THROW(bg(fmt::terminal_color::white) | fmt::text_style());
}