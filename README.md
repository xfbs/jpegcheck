# JPEG check

If you want to use a boot splash image (see [this wiki](https://www.coreboot.org/SeaBIOS) for more information about that), you may notice that SeaBIOS is very particular about what JPEG images it accepts. This little tool will run an arbitrary image through the SeaBIOS JPEG parser and tell you if it throws any errors while decoding and yields some information about the JPEG file.

## Usage

    $ git clone https://github.com/xfbs/jpegcheck
    $ git submodule init
    $ make
    $ ./jpegcheck <imagefile>

## License

MIT License.
