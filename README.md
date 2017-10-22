top2bottom is a command-line tool that converts top-posted emails to
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

# Running

You can run top2bottom with:

`$ /path/to/top2bottom < top-posted-file [> bottom-posted-file]`

# Supported top-posting flavor

top2bottom supports a particular flavor of top-posted emails which follow the
format:

    aaa

    B wrote:
    > bbb
    >
    > C wrote:
    >>
    >> ccc

top2bottom also supports conversion of emails which were originally
bottom-posted but at some point were changed to top-posted:

    aaa

    B wrote:
    > C wrote:
    >>
    >> ccc
    >
    > bbb

If top2bottom cannot convert the email, then it returns the original message in
stdout, and a warning with information about which line caused the
conversion to fail in stderr.
