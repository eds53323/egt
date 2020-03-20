 @page media_page Media

This chapter discusses media formats supported by EGT.

@section media_intro Supported Media Formats

EGT has direct and indirect support for many standard image, video, and
audio formats through classes like egt::VideoWindow, egt::AudioPlayer,
egt::experimental::Sound, egt::Image, egt::SvgImage, and more.  In most cases,
the low level handling is implemented using third party libraries like libpng,
libjpeg, gstreamer, librsvg, and so on.

@section media_image Image Formats

The following image formats are directly supported by EGT:
- PNG
- JPEG
- BMP
- SVG

@section media_svg Working with SVG (Scalable Vector Graphics) Files

Scalable Vector Graphics (SVG) is an XML-based vector image format for
two-dimensional graphics with support for interactivity and animation. The SVG
specification is an open standard developed by the World Wide Web Consortium
(W3C) since 1999. SVG images and their behaviors are defined in XML text files.

EGT can load SVG files using the third party library
[librsvg](https://github.com/GNOME/librsvg).  librsvg is a library to render SVG
files using cairo.  On top of that, this library provides the ability to load
elements by id from SVG files.  What this means is a graphic designer can create
an SVG file and give each component of the UI a unique element ID.  Then, an EGT
programmer can individually load these components and assign them to widgets and
create logic around these components from a single SVG file.  Also, a graphic
designer can put elements in the SVG that are a hint where EGT should be used to
draw something, like text.

@note EGT does not support all SVG capabilities including things like
extensions, javascript, or animations.

@note While EGT supports loading SVG files and working with them dynamically, it
is also worth noting that there [are tools](https://github.com/akrinke/svg2cairo)
that can convert an SVG file into actual cairo code.  This may be useful for
several reasons, including reducing dependencies of EGT.

@note Example SVG files can be found
[on various websites](https://dev.w3.org/SVG/tools/svgweb/samples/svg-files/).

Working with an SVG file in EGT is mostly accomplished with the SvgImage class.
This class allows fine grained access into SVG specific properties,
while still allowing easy conversion to a normal raster Image instance.

egt::v1::experimental::Gauge, egt::v1::experimental::GaugeLayer, and
egt::v1::experimental::NeedleLayer are several classes useful in EGT for taking
advantage of SVG files directly by using complete SVG files or individual
objects in those SVG files to construct layered widgets.

@section media_video Video Formats

The following video stream formats are directly supported by EGT:

Format           | Processor Support
---------------- | -----------------
Uncompressed YUV | All
MPEG1            | All
MPEG2            | All
MPEG4            | All
H.264            | SAMA5D4
VP8              | SAMA5D4
VP9              | SAMA5D4


@section media_file_formats Metadata File Formats

EGT supports several different types of metadata file formats through
third-party libraries.

Format           | Support Library
---------------- | ---------------
XML              | RapidXML
JSON             | JsonCpp