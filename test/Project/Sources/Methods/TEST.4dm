//%attributes = {}
$path:=Get 4D folder:C485(Current resources folder:K5:16)+"10-pages.tif"

READ PICTURE FILE:C678($path; $tiff)

ARRAY PICTURE:C279($pages; 0)

TIFF GET PAGES($tiff; $pages)

APPEND TO ARRAY:C911($pages; $tiff)  //the 11th element has 10 pictures so the result contains 20 pages

$merged:=TIFF Create from array($pages)

WRITE PICTURE FILE:C680(System folder:C487(Desktop:K41:16)+"merged.tif"; $merged)