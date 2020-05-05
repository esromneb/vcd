{
  "targets": [
    {
      "target_name": "vcd",
      "sources": [
        "vcd_parser.c",
        "vcd_spans.c",
        "vcd.cpp",
        "playback.cpp",
        "playback.hpp",
      ],

      'cflags': [ '-Wall', '-Wextra', '-Wno-unused-parameter' ],
      'cflags_cc': [ '-std=gnu++1z' ],
    }
  ]
}
