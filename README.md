![version](https://img.shields.io/badge/version-20%2B-E23089)
![platform](https://img.shields.io/static/v1?label=platform&message=mac-intel%20|%20mac-arm%20|%20win-64&color=blue)
[![license](https://img.shields.io/github/license/miyako/4d-plugin-TIFF)](LICENSE)
![downloads](https://img.shields.io/github/downloads/miyako/4d-plugin-TIFF/total)

# 4d-plugin-TIFF

A basic multi-page TIFF toolkit for 4D, built on `libtiff` (the same code paths as its `tiffcp`/`tiffsplit` command-line tools). It works on 4D `Picture` variables: you can count how many pages a TIFF-bearing picture has, split it into an array of one-page pictures, or merge an array of pictures back into a single multi-page TIFF. Any picture parameter can be a composite (multi-format) picture — the plugin looks for the picture's own `image/tiff` representation internally and ignores any other formats stored alongside it.

| Command | Returns | Purpose |
|---|---|---|
| [`TIFF Count pages`](#tiff-count-pages) | Longint | Number of pages (directories) in a TIFF picture |
| [`TIFF GET PAGES`](#tiff-get-pages) | — | Split a TIFF picture into an array of single-page pictures |
| [`TIFF Create from array`](#tiff-create-from-array) | Picture | Merge an array of pictures into one multi-page TIFF |

**Platforms:** macOS (Intel & Apple Silicon), Windows 64-bit.

---

## Requirements & platform notes

- No installation requirements beyond the plugin itself — there's no OS permission, minimum-OS-version, or external-service dependency for any command; everything runs against `libtiff` in-process.
- All three commands take their picture/array parameters **by reference**. For `TIFF GET PAGES` and `TIFF Create from array`, the array parameter doesn't need to be declared with `ARRAY PICTURE` beforehand — if it's undefined, the plugin creates it as an `ARRAY PICTURE` for you. If it's already an `ARRAY PICTURE`, its **existing contents are cleared** before being repopulated (see [`TIFF GET PAGES`](#tiff-get-pages) below) — the plugin replaces, it doesn't append.
- A picture that has no `image/tiff` representation (not a TIFF, or a composite picture without a TIFF variant) is never treated as an error by any of these three commands — it's simply treated as "0 pages" / "nothing to copy." Check the count or array size afterward if you need to detect that case.
- Behavior described below reflects the source as reviewed and fixed in this session (see the accompanying code review): with the fixes applied, a page that fails to copy is skipped rather than silently included as corrupted data, and malformed/corrupt TIFF input no longer risks a crash. This is true of the fixed source — confirm your installed plugin build actually includes these fixes before relying on that behavior in production.

---

## TIFF Count pages

### Syntax

```4d
count:=TIFF Count pages(image)
```

| Parameter | Type | Description |
|---|---|---|
| `image` | Picture | Can be a composite picture, but must contain a TIFF representation |
| `Result` | Longint | Number of pages in the TIFF |

### Description

Returns the number of TIFF directories (pages) in `image`. If `image` has no `image/tiff` representation, or the TIFF data can't be opened (empty/corrupt), the command returns `0` rather than raising a 4D error — there's no way to distinguish "0-page TIFF" from "not a TIFF" from the return value alone.

### Example

From the plugin's own test method (`TEST.4dm`), as part of a larger sequence:

```4d
$path:=Get 4D folder:C485(Current resources folder:K5:16)+"10-pages.tif"
READ PICTURE FILE:C678($path; $tiff)
```

Counting pages on that same picture:

```4d
$count:=TIFF Count pages($tiff)
ALERT:C41("This TIFF has "+String:C10($count)+" page(s).")
```

Using it to guard a loop over pages before calling [`TIFF GET PAGES`](#tiff-get-pages):

```4d
$count:=TIFF Count pages($tiff)
If ($count>0)
	ARRAY PICTURE:C279($pages; 0)
	TIFF GET PAGES($tiff; $pages)
End if
```

---

## TIFF GET PAGES

### Syntax

```4d
TIFF GET PAGES(image; images{; start{; end}})
```

| Parameter | Type | Description |
|---|---|---|
| `image` | Picture | Can be a composite picture, but must contain a TIFF representation |
| `images` | Array Picture | Filled with one picture per extracted page. Existing contents are cleared first; the array is created automatically if not yet declared |
| `start` | Longint | Optional, defaults to the first page. `1` is the first page; a negative number is an offset from the last page |
| `end` | Longint | Optional, defaults to the last page. `0` (or omitted) means the last page; a negative number is an offset from the last page |

This command has no function result — it modifies `images` in place.

### Description

Extracts a page range from `image` into `images`, one page per array element, each as its own single-page TIFF picture.

- `start`/`end` follow 4D's usual optional-trailing-parameter convention: omit `end` to go to the last page, omit both to get every page.
- Negative values count backward from the last page (e.g. `-1` refers near the end of the TIFF); the plugin clamps the resolved position to at least page `1` if the offset would otherwise go before the start of the document.
- `end` is capped at the actual page count — asking for more pages than exist just gets you every page up to the end, not an error.
- If, after resolving `start` and `end`, the effective start position isn't before the effective end position, `images` simply ends up empty (cleared, nothing appended) — no error is raised.
- Each extracted page is written out as a standalone single-page TIFF (endianness matches the source file), independent of the other pages — you can use each array element on its own (write it to a file, pass it to another command) without needing the rest of the array.

### Example

From the plugin's own test method (`TEST.4dm`):

```4d
$path:=Get 4D folder:C485(Current resources folder:K5:16)+"10-pages.tif"

READ PICTURE FILE:C678($path; $tiff)

ARRAY PICTURE:C279($pages; 0)

TIFF GET PAGES($tiff; $pages)

APPEND TO ARRAY:C911($pages; $tiff)  //the 11th element has 10 pictures so the result contains 20 pages
```

Extracting only the first 3 pages:

```4d
ARRAY PICTURE:C279($firstThree; 0)
TIFF GET PAGES($tiff; $firstThree; 1; 3)
```

Extracting only the last 2 pages using negative offsets:

```4d
ARRAY PICTURE:C279($lastTwo; 0)
TIFF GET PAGES($tiff; $lastTwo; -2; 0)
```

---

## TIFF Create from array

### Syntax

```4d
image:=TIFF Create from array(images)
```

| Parameter | Type | Description |
|---|---|---|
| `images` | Array Picture | Pictures to merge. Each can be a composite picture, but must contain a TIFF representation to be included |
| `Result` | Picture | The merged multi-page TIFF |

### Description

Builds one multi-page TIFF from every picture in `images`, in array order. Each source picture can itself already be a multi-page TIFF — all of its pages are copied into the merged result in sequence, so merging ten single-page pictures and merging one ten-page picture both land in the same output shape.

Any array element with no `image/tiff` representation is skipped — it contributes no pages to the result, and no error is raised for it. If `images` isn't actually an `Array Picture` (wrong parameter type) or the merge can't be started at all, the command returns an empty picture rather than a populated TIFF.

### Example

From the plugin's own test method (`TEST.4dm`), continuing directly from the [`TIFF GET PAGES`](#tiff-get-pages) example above:

```4d
$merged:=TIFF Create from array($pages)

WRITE PICTURE FILE:C680(System folder:C487(Desktop:K41:16)+"merged.tif"; $merged)
```

Merging two independently-loaded TIFFs into one file:

```4d
ARRAY PICTURE:C279($toMerge; 2)
READ PICTURE FILE:C678($path1; $toMerge{1})
READ PICTURE FILE:C678($path2; $toMerge{2})
$merged:=TIFF Create from array($toMerge)
```

---

## Error handling & troubleshooting

- **A "0 pages" or empty result usually means "no TIFF representation found," not a failure.** All three commands locate a picture's TIFF data by its internal `image/tiff` representation; a picture with no such representation is treated the same as an empty TIFF, not as an error condition you can catch.
- **`TIFF GET PAGES` replaces the array, it doesn't add to it.** If you need to accumulate pages from more than one source picture, do it in 4D code after the call (as the plugin's own test method does with `APPEND TO ARRAY`), not by calling the command twice on the same array.
- **An out-of-range or inverted page range silently yields an empty array**, not an error — check `Size of array` on `images` afterward if you need to confirm pages were actually extracted.
- **A page that fails to copy is skipped, not corrupted into the output.** With the fixes applied during this review, if an individual page can't be copied (e.g. an unsupported encoding), it's left out of the result rather than appearing as truncated/garbage image data — confirm this behavior against your actual installed build if you're not certain it includes the fix.
- **`TIFF Create from array` always returns a picture, even on bad input**, so a wrong parameter type or an unusable array won't hang your 4D method — you'll get back an empty picture instead, which you can check for before using it.

---

## Quick reference

```4d
// Count pages
$count:=TIFF Count pages($tiff)

// Split into single-page pictures
ARRAY PICTURE:C279($pages; 0)
TIFF GET PAGES($tiff; $pages)

// Split a specific range (1-based, negative = from the end)
ARRAY PICTURE:C279($range; 0)
TIFF GET PAGES($tiff; $range; 2; -1)

// Merge back into one multi-page TIFF
$merged:=TIFF Create from array($pages)
WRITE PICTURE FILE:C680($path; $merged)
```
