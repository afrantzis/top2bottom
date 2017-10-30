top2bottom is a command-line tool that converts top-posted email messages to
bottom-posted.

# Building requirements

To build top2bottom you need:

 * the meson build system
 * a C++14 compiler

# Building and installing

top2bottom uses the meson build system.

To configure top2bottom use:

`$ meson build`

This will create a top2bottom build configuration in the **build** directory.

To see/set the full list of available options use:

`$ mesonconf [-Dopt=val] build`

To build use:

`$ ninja -C build`

To install use:

`$ [DESTDIR=...] ninja -C build install`

Run the tests with:

`$ ninja -C build test`

# Running

You can run top2bottom with:

`$ /path/to/top2bottom < top-posted-file [> bottom-posted-file]`

# Supported top-posting flavor

top2bottom supports a particular flavor of top-posted messages that looks like:

    aaa

    B wrote:
    > bbb
    >
    > C wrote:
    >>
    >> ccc

top2bottom also supports conversion of messages which were originally
bottom-posted or interleaved-posted but at some point continued as
top-posted:

    aaa

    B wrote:
    > C wrote:
    >>
    >> ccc
    > bbb
    >> ccc
    >
    > bbb

Messages that are already properly bottom-posted or interleaved-posted are
returned unchanged.

If top2bottom cannot convert a message, then it returns the message
unchanged in stdout, and a warning with information about which line caused the
conversion to fail in stderr.
