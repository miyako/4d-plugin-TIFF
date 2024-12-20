![version](https://img.shields.io/badge/version-20%2B-E23089)
![platform](https://img.shields.io/static/v1?label=platform&message=mac-intel%20|%20mac-arm%20|%20win-64&color=blue)
[![license](https://img.shields.io/github/license/miyako/4d-plugin-TIFF)](LICENSE)
![downloads](https://img.shields.io/github/downloads/miyako/4d-plugin-TIFF/total)

# TIFF
Basic tool for TIFF


code based on [``tiffcp``](http://www.simplesystems.org/libtiff/man/tiffcp.1.html) and [``tiffsplit``](http://www.simplesystems.org/libtiff/man/tiffsplit.1.html).

---

## Syntax

```4d
count:=TIFF Count pages (image)
```

param|type|description
------------|------|----
image|PICTURE|can be composite image, but should contain ``TIFF``
count|LONGINT|number of images

```4d
TIFF GET PAGES (image;images{;start{;end}})
```

param|type|description
------------|------|----
image|PICTURE|can be composite image, but should contain ``TIFF``
images|ARRAY PICTURE|images
start|LONGINT|``1`` is first page. negative number means offset from last page
end|LONGINT|``0`` is last page. negative number means offset from last page

```4d
image:=TIFF Create from array (images)
```

param|type|description
------------|------|----
images|ARRAY PICTURE|can be composite image, but should contain ``TIFF``
image|PICTURE|merged ``TIFF``

## Examples

```4d
$path:=Get 4D folder(Current resources folder)+"10-pages.tif"

READ PICTURE FILE($path;$tiff)

ARRAY PICTURE($pages;0)

TIFF GET PAGES ($tiff;$pages)

APPEND TO ARRAY($pages;$tiff)//the 11th element has 10 pictures so the result contains 20 pages

$merged:=TIFF Create from array ($pages)

WRITE PICTURE FILE(System folder(Desktop)+"merged.tif";$merged)
```

<img width="540" alt="2017-11-09 11 38 45" src="https://user-images.githubusercontent.com/1725068/32588526-a23cf0b6-c552-11e7-8267-a503cd53217e.png">
