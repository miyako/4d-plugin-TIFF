# 4d-plugin-multi-page-images
Basic tool for GIF, TIFF, PDF

### Platform

| carbon | cocoa | win32 | win64 |
|:------:|:-----:|:---------:|:---------:|
|<img src="https://cloud.githubusercontent.com/assets/1725068/22371562/1b091f0a-e4db-11e6-8458-8653954a7cce.png" width="24" height="24" />|<img src="https://cloud.githubusercontent.com/assets/1725068/22371562/1b091f0a-e4db-11e6-8458-8653954a7cce.png" width="24" height="24" />|<img src="https://cloud.githubusercontent.com/assets/1725068/22371562/1b091f0a-e4db-11e6-8458-8653954a7cce.png" width="24" height="24" />|<img src="https://cloud.githubusercontent.com/assets/1725068/22371562/1b091f0a-e4db-11e6-8458-8653954a7cce.png" width="24" height="24" />|

### Version

<img src="https://cloud.githubusercontent.com/assets/1725068/18940649/21945000-8645-11e6-86ed-4a0f800e5a73.png" width="32" height="32" /> <img src="https://cloud.githubusercontent.com/assets/1725068/18940648/2192ddba-8645-11e6-864d-6d5692d55717.png" width="32" height="32" />

[``libgd``](https://github.com/libgd/libgd) and [``libtiff``](http://www.simplesystems.org/libtiff/) for ``GIF`` and ``TIFF``. [``libgs``](https://ghostscript.com/) for ``PDF``.

* libraries

``LIBTIFF 4.0.8``
``IJG LIBJPEG 9b``
``LIBZ 1.2.11``
``LIBLZMA 5.0.3`` (mac only)

currently **beta**

only ``TIFF`` functions are implemented.

code based on [``tiffcp``](http://www.simplesystems.org/libtiff/man/tiffcp.1.html) and [``tiffsplit``](http://www.simplesystems.org/libtiff/man/tiffsplit.1.html).

---

## Syntax

```
count:=TIFF Count pages (image)
```

param|type|description
------------|------|----
image|PICTURE|can be composite image, but should contain ``TIFF``
count|LONGINT|number of images

```
TIFF GET PAGES (image;images{;start{;end}})
```

param|type|description
------------|------|----
image|PICTURE|can be composite image, but should contain ``TIFF``
images|ARRAY PICTURE|images
start|LONGINT|``1`` is first page. negative number means offset from last page
end|LONGINT|``0`` is last page. negative number means offset from last page

```
image:=TIFF Create from array (images)
```

param|type|description
------------|------|----
images|ARRAY PICTURE|can be composite image, but should contain ``TIFF``
image|PICTURE|merged ``TIFF``

## Examples

```
$path:=Get 4D folder(Current resources folder)+"10-pages.tif"

READ PICTURE FILE($path;$tiff)

ARRAY PICTURE($pages;0)

TIFF GET PAGES ($tiff;$pages)

APPEND TO ARRAY($pages;$tiff)//the 11th element has 10 pictures so the result contains 20 pages

$merged:=TIFF Create from array ($pages)

WRITE PICTURE FILE(System folder(Desktop)+"merged.tif";$merged)
```

<img width="540" alt="2017-11-09 11 38 45" src="https://user-images.githubusercontent.com/1725068/32588526-a23cf0b6-c552-11e7-8267-a503cd53217e.png">

