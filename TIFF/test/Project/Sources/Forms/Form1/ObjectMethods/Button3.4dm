$pTiff:=OBJECT Get pointer:C1124(Object named:K67:5; "tiff")

$path:=Get 4D folder:C485(Current resources folder:K5:16)+"10-pages.tif"

READ PICTURE FILE:C678($path; $pTiff->)

OBJECT SET TITLE:C194(*; "pages"; "count pages: "+String:C10(TIFF Count pages($pTiff->)))

$pTiffs:=OBJECT Get pointer:C1124(Object named:K67:5; "tiffs")

ARRAY PICTURE:C279($pTiffs->; 0)

TIFF GET PAGES($pTiff->; $pTiffs->; -5; -1)