/* --------------------------------------------------------------------------------
 #
 #	4DPlugin.h
 #	source generated by 4D Plugin Wizard
 #	Project : multi-page-images
 #	author : miyako
 #	2017/11/08
 #
 # --------------------------------------------------------------------------------*/

#include "tiffio.h"
#include "tiffio.hxx"

// --- TIFF
void TIFF_Count_pages(sLONG_PTR *pResult, PackagePtr pParams);
void TIFF_GET_PAGES(sLONG_PTR *pResult, PackagePtr pParams);
void TIFF_Create_from_array(sLONG_PTR *pResult, PackagePtr pParams);

// --- GIF
void GIF_Count_pages(sLONG_PTR *pResult, PackagePtr pParams);
void GIF_GET_PAGES(sLONG_PTR *pResult, PackagePtr pParams);
void GIF_Create_from_array(sLONG_PTR *pResult, PackagePtr pParams);

// --- PDF
void PDF_Count_pages(sLONG_PTR *pResult, PackagePtr pParams);
void PDF_GET_PAGES(sLONG_PTR *pResult, PackagePtr pParams);
void PDF_Create_from_array(sLONG_PTR *pResult, PackagePtr pParams);

typedef struct
{
	uint8_t *ptr;
	PA_long32 len;
	PA_long32 pos;
} tiff_src;

typedef struct
{
	std::vector<uint8_t> *buf;
	PA_long32 pos;
} tiff_dst;
