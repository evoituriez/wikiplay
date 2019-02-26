Description
-----------
This module makes it so that the Image module can accept SVG files when uploaded
through an Image field. It also makes it so that image styles will not attempt
to process SVGs. Instead the original SVG image will always be shown, even for
images that run through theme_image_style(). Despite using the original image,
the image dimensions for width and height should be set correctly when using
image styles that scale or crop an image.

Requirements
------------
Drupal 7.x

Installation
------------
1. Copy the entire svg_image directory the Drupal sites/all/modules directory.

2. Login as an administrator. Enable the module in the "Administer" -> "Modules"

3. Edit any existing image field on the site and add "svg" to the list of
   allowed file extensions.

4. Create content as you would normally, using the same image fields as before.
   SVG images should now be allowed to upload.

Limitations
-----------

This module has a few limitations that may result in unexpected behavior at
times.

- SVG images that do not specify width and height will not display properly.
  These properties are optional for SVG images (since they can display at any
  size). Such images will not be given HTML width and height attributes, which
  may cause them to display very large.
- Upload validators for minimum and maximum image dimensions are not respected
  for SVG images.

Credits
-------

Written by Nate Lampton (quicksketch)
