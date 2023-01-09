/** \file GeoreferenceProcess.cpp
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

//Includes estandar
#include <string>
#include <sstream>
#include <cstdio>
// Includes suri
#include "GeoreferenceProcess.h"
#include "suri/TextFileGcpLoader.h"
#include "suri/GcpList.h"
#include "suri/RasterSpatialModel.h"
#include "suri/ProcessAdaptLayer.h"
#include "suri/Coordinates.h"
#include "ReprojectionProcess.h"
#include "suri/messages.h"
#include "suri/XmlFunctions.h"
// Includes GDAL
#include "gdal.h"
#include "gdalwarper.h"
#include "cpl_vsi.h"
#include "cpl_conv.h"
#include "cpl_string.h"
#include "gdal_priv.h"
#include "ogr_spatialref.h"
#include "vrtdataset.h"
// Includes wxWidgets
#include "wx/progdlg.h"
#include "wx/sstream.h"

namespace {

/**
 * Funcion callbaack para mostrar avance del proceso. Por el momento la funcion
 * hace nada. La idea seria que le pueda enviar mensajes al dialogo de progreso
 * utilizado en la aplicacion. Para eso probablemente haya que hacer algunos
 * cambios como por ejemplo lanzar el proceso en un thread.
 */
int CPL_DLL CPL_STDCALL DummyProgreeProc(double Complete, const char* pMessage,
                                         void* pProgressArg) {
   if (pProgressArg != NULL) {
      wxProgressDialog* pprogress = static_cast<wxProgressDialog*>(pProgressArg);
      pprogress->Update(static_cast<int>(Complete * 10));
   }
   return TRUE;
}

typedef struct {

    GDALTransformerInfo sTI;

    double   adfSrcGeoTransform[6];
    double   adfSrcInvGeoTransform[6];

    void     *pSrcGCPTransformArg;
    void     *pSrcRPCTransformArg;
    void     *pSrcTPSTransformArg;
    void     *pSrcGeoLocTransformArg;

    void     *pReprojectArg;

    double   adfDstGeoTransform[6];
    double   adfDstInvGeoTransform[6];

    void     *pDstGCPTransformArg;

} GDALGenImgProjTransformInfo_;

typedef struct {
    GDALTransformerInfo sTI;

    OGRCoordinateTransformation *poForwardTransform;
    OGRCoordinateTransformation *poReverseTransform;
} GDALReprojectionTransformInfo_;

CPLXMLNode *
GDALSerializeReprojectionTransformer_( void *pTransformArg )

{
    CPLXMLNode *psTree;
    GDALReprojectionTransformInfo_ *psInfo =
        (GDALReprojectionTransformInfo_ *) pTransformArg;

    psTree = CPLCreateXMLNode( NULL, CXT_Element, "ReprojectionTransformer" );

/* -------------------------------------------------------------------- */
/*      Handle SourceCS.                                                */
/* -------------------------------------------------------------------- */
    OGRSpatialReference *poSRS;
    char *pszWKT = NULL;

    poSRS = psInfo->poForwardTransform->GetSourceCS();
    poSRS->exportToWkt( &pszWKT );
    CPLCreateXMLElementAndValue( psTree, "SourceSRS", pszWKT );
    CPLFree( pszWKT );

/* -------------------------------------------------------------------- */
/*      Handle DestinationCS.                                           */
/* -------------------------------------------------------------------- */
    poSRS = psInfo->poForwardTransform->GetTargetCS();
    poSRS->exportToWkt( &pszWKT );
    CPLCreateXMLElementAndValue( psTree, "TargetSRS", pszWKT );
    CPLFree( pszWKT );

    return psTree;
}

void *
GDALDeserializeReprojectionTransformer_( CPLXMLNode *psTree )

{
    const char *pszSourceSRS = CPLGetXMLValue( psTree, "SourceSRS", NULL );
    const char *pszTargetSRS= CPLGetXMLValue( psTree, "TargetSRS", NULL );
    char *pszSourceWKT = NULL, *pszTargetWKT = NULL;
    void *pResult = NULL;

    if( pszSourceSRS != NULL )
    {
        OGRSpatialReference oSRS;

        if( oSRS.SetFromUserInput( pszSourceSRS ) == OGRERR_NONE )
            oSRS.exportToWkt( &pszSourceWKT );
    }

    if( pszTargetSRS != NULL )
    {
        OGRSpatialReference oSRS;

        if( oSRS.SetFromUserInput( pszTargetSRS ) == OGRERR_NONE )
            oSRS.exportToWkt( &pszTargetWKT );
    }

    if( pszSourceWKT != NULL && pszTargetWKT != NULL )
    {
        pResult = GDALCreateReprojectionTransformer( pszSourceWKT,
                                                     pszTargetWKT );
    }
    else
    {
        CPLError( CE_Failure, CPLE_AppDefined,
                  "ReprojectionTransformer definition missing either\n"
                  "SourceSRS or TargetSRS definition." );
    }

    CPLFree( pszSourceWKT );
    CPLFree( pszTargetWKT );

    return pResult;
}

void GDALRefreshGenImgProjTransformer_(void* hTransformArg)
{
    GDALGenImgProjTransformInfo_ *psInfo =
        static_cast<GDALGenImgProjTransformInfo_ *>( hTransformArg );

    if (psInfo->pReprojectArg)
    {
        CPLXMLNode* psXML = GDALSerializeReprojectionTransformer_(psInfo->pReprojectArg);
        GDALDestroyReprojectionTransformer(psInfo->pReprojectArg);
        psInfo->pReprojectArg = GDALDeserializeReprojectionTransformer_(psXML);
        CPLDestroyXMLNode(psXML);
    }
}


int GDALSuggestedWarpOutput2_MustAdjustForRightBorder_(
                     GDALTransformerFunc pfnTransformer, void *pTransformArg,
                     double* padfExtent, int nPixels, int nLines,
                     double dfPixelSizeX, double dfPixelSizeY)
 {
    int nSamplePoints;
    double dfRatio;
    int bErr;
    int nBadCount;
    int    abSuccess[21] = { 0 };
    double adfX[21] = { 0 };
    double adfY[21] = { 0 };
    double adfZ[21] = { 0 };

    //double dfMinXOut = padfExtent[0];
    //double dfMinYOut = padfExtent[1];
    double dfMaxXOut = padfExtent[2];
    double dfMaxYOut = padfExtent[3];

    // Take 20 steps
    nSamplePoints = 0;
    for( dfRatio = 0.0; dfRatio <= 1.01; dfRatio += 0.05 )
    {
        // Ensure we end exactly at the end.
        if( dfRatio > 0.99 )
            dfRatio = 1.0;

        // Along right
        adfX[nSamplePoints]   = dfMaxXOut;
        adfY[nSamplePoints]   = dfMaxYOut - dfPixelSizeY * dfRatio * nLines;
        adfZ[nSamplePoints++] = 0.0;
    }

    bErr = FALSE;
    if( !pfnTransformer( pTransformArg, TRUE, nSamplePoints,
                             adfX, adfY, adfZ, abSuccess ) )
    {
        bErr = TRUE;
    }

    if( !bErr && !pfnTransformer( pTransformArg, FALSE, nSamplePoints,
                             adfX, adfY, adfZ, abSuccess ) )
    {
        bErr = TRUE;
    }

    nSamplePoints = 0;
    nBadCount = 0;
    for( dfRatio = 0.0; !bErr && dfRatio <= 1.01; dfRatio += 0.05 )
    {
        double expected_x = dfMaxXOut;
        double expected_y = dfMaxYOut - dfPixelSizeY * dfRatio * nLines;
        if (fabs(adfX[nSamplePoints] -  expected_x) > dfPixelSizeX ||
            fabs(adfY[nSamplePoints] -  expected_y) > dfPixelSizeY)
            nBadCount ++;
        nSamplePoints ++;
    }

    return (nBadCount == nSamplePoints);
}

int GDALSuggestedWarpOutput2_MustAdjustForBottomBorder_(
                     GDALTransformerFunc pfnTransformer, void *pTransformArg,
                     double* padfExtent, int nPixels, int nLines,
                     double dfPixelSizeX, double dfPixelSizeY)
 {
    int nSamplePoints;
    double dfRatio;
    int bErr;
    int nBadCount;
    int    abSuccess[21] = { 0 };
    double adfX[21] = { 0 };
    double adfY[21] = { 0 };
    double adfZ[21] = { 0 };

    double dfMinXOut = padfExtent[0];
    double dfMinYOut = padfExtent[1];
    //double dfMaxXOut = padfExtent[2];
    //double dfMaxYOut = padfExtent[3];

    // Take 20 steps
    nSamplePoints = 0;
    for( dfRatio = 0.0; dfRatio <= 1.01; dfRatio += 0.05 )
    {
        // Ensure we end exactly at the end.
        if( dfRatio > 0.99 )
            dfRatio = 1.0;

        // Along right
        adfX[nSamplePoints]   = dfMinXOut + dfPixelSizeX * dfRatio * nPixels;
        adfY[nSamplePoints]   = dfMinYOut;
        adfZ[nSamplePoints++] = 0.0;
    }

    bErr = FALSE;
    if( !pfnTransformer( pTransformArg, TRUE, nSamplePoints,
                             adfX, adfY, adfZ, abSuccess ) )
    {
        bErr = TRUE;
    }

    if( !bErr && !pfnTransformer( pTransformArg, FALSE, nSamplePoints,
                             adfX, adfY, adfZ, abSuccess ) )
    {
        bErr = TRUE;
    }

    nSamplePoints = 0;
    nBadCount = 0;
    for( dfRatio = 0.0; !bErr && dfRatio <= 1.01; dfRatio += 0.05 )
    {
        double expected_x = dfMinXOut + dfPixelSizeX * dfRatio * nPixels;
        double expected_y = dfMinYOut;
        if (fabs(adfX[nSamplePoints] -  expected_x) > dfPixelSizeX ||
            fabs(adfY[nSamplePoints] -  expected_y) > dfPixelSizeY)
            nBadCount ++;
        nSamplePoints ++;
    }

    return (nBadCount == nSamplePoints);
}

CPLErr CPL_STDCALL
GDALSuggestedWarpOutput2_( GDALDatasetH hSrcDS,
                          GDALTransformerFunc pfnTransformer,
                          void *pTransformArg,
                          double *padfGeoTransformOut,
                          int *pnPixels, int *pnLines,
                          double *padfExtent, int nOptions )

{
    VALIDATE_POINTER1( hSrcDS, "GDALSuggestedWarpOutput2_", CE_Failure );

/* -------------------------------------------------------------------- */
/*      Setup sample points all around the edge of the input raster.    */
/* -------------------------------------------------------------------- */
    int    nSamplePoints = 0;
    int    nInXSize = GDALGetRasterXSize( hSrcDS );
    int    nInYSize = GDALGetRasterYSize( hSrcDS );

    if (pfnTransformer == GDALGenImgProjTransform)
    {
        /* In case CHECK_WITH_INVERT_PROJ has been modified */
        GDALRefreshGenImgProjTransformer_(pTransformArg);
    }

#define N_PIXELSTEP 50
    int nSteps = (int) (double(MIN(nInYSize, nInXSize)) / N_PIXELSTEP + .5);
    if (nSteps < 20)
        nSteps = 20;

retry:
    int nSampleMax = (nSteps + 1)*(nSteps + 1);
    int *pabSuccess = NULL;
    double *padfX, *padfY, *padfZ;
    double *padfXRevert, *padfYRevert, *padfZRevert;

    double dfRatio = 0.0;
    double dfStep = 1. / nSteps;

    pabSuccess = (int *) VSIMalloc3(sizeof(int), nSteps + 1, nSteps + 1);
    padfX = (double *) VSIMalloc3(sizeof(double) * 3, nSteps + 1, nSteps + 1);
    padfXRevert = (double *) VSIMalloc3(sizeof(double) * 3, nSteps + 1, nSteps + 1);
    if (pabSuccess == NULL || padfX == NULL || padfXRevert == NULL)
    {
        CPLFree( padfX );
        CPLFree( padfXRevert );
        CPLFree( pabSuccess );
        if (nSteps > 20)
        {
            nSteps = 20;
            goto retry;
        }
        return CE_Failure;
    }
    padfY = padfX + nSampleMax;
    padfZ = padfX + nSampleMax * 2;
    padfYRevert = padfXRevert + nSampleMax;
    padfZRevert = padfXRevert + nSampleMax * 2;


    // Take N_STEPS steps
    int iStep;
    for( iStep = 0; iStep <= nSteps; iStep ++ )
    {
        dfRatio = (iStep == nSteps) ? 1.0 : iStep * dfStep;

        // Along top
        padfX[nSamplePoints]   = dfRatio * nInXSize;
        padfY[nSamplePoints]   = 0.0;
        padfZ[nSamplePoints++] = 0.0;

        // Along bottom
        padfX[nSamplePoints]   = dfRatio * nInXSize;
        padfY[nSamplePoints]   = nInYSize;
        padfZ[nSamplePoints++] = 0.0;

        // Along left
        padfX[nSamplePoints]   = 0.0;
        padfY[nSamplePoints] = dfRatio * nInYSize;
        padfZ[nSamplePoints++] = 0.0;

        // Along right
        padfX[nSamplePoints]   = nInXSize;
        padfY[nSamplePoints] = dfRatio * nInYSize;
        padfZ[nSamplePoints++] = 0.0;
    }

    CPLAssert( nSamplePoints == 4 * (nSteps + 1) );

    memset( pabSuccess, 1, sizeof(int) * nSampleMax );

/* -------------------------------------------------------------------- */
/*      Transform them to the output coordinate system.                 */
/* -------------------------------------------------------------------- */
    int    nFailedCount = 0, i;

    if( !pfnTransformer( pTransformArg, FALSE, nSamplePoints,
                         padfX, padfY, padfZ, pabSuccess ) )
    {
        CPLError( CE_Failure, CPLE_AppDefined,
                  "GDALSuggestedWarpOutput() failed because the passed\n"
                  "transformer failed." );
        CPLFree( padfX );
        CPLFree( padfXRevert );
        CPLFree( pabSuccess );
        return CE_Failure;
    }

    for( i = 0; i < nSamplePoints; i++ )
    {
        if( !pabSuccess[i] )
            nFailedCount++;
    }

/* -------------------------------------------------------------------- */
/*      Check if the computed target coordinates are revertable.        */
/*      If not, try the detailed grid sampling.                         */
/* -------------------------------------------------------------------- */
    if (nFailedCount == 0 )
    {
        memcpy(padfXRevert, padfX, nSamplePoints * sizeof(double));
        memcpy(padfYRevert, padfY, nSamplePoints * sizeof(double));
        memcpy(padfZRevert, padfZ, nSamplePoints * sizeof(double));
        if( !pfnTransformer( pTransformArg, TRUE, nSamplePoints,
                             padfXRevert, padfYRevert, padfZRevert, pabSuccess ) )
        {
            nFailedCount = 1;
        }
        else
        {
            for( i = 0; nFailedCount == 0 && i < nSamplePoints; i++ )
            {
                if( !pabSuccess[i] )
                    nFailedCount++;

                dfRatio = 0.0 + (i/4) * dfStep;
                if (dfRatio>0.99)
                    dfRatio = 1.0;

                double dfExpectedX, dfExpectedY;
                if ((i % 4) == 0)
                {
                    dfExpectedX   = dfRatio * nInXSize;
                    dfExpectedY   = 0.0;
                }
                else if ((i % 4) == 1)
                {
                    dfExpectedX   = dfRatio * nInXSize;
                    dfExpectedY   = nInYSize;
                }
                else if ((i % 4) == 2)
                {
                    dfExpectedX   = 0.0;
                    dfExpectedY   = dfRatio * nInYSize;
                }
                else
                {
                    dfExpectedX   = nInXSize;
                    dfExpectedY   = dfRatio * nInYSize;
                }

                if (fabs(padfXRevert[i] - dfExpectedX) > nInXSize / nSteps ||
                    fabs(padfYRevert[i] - dfExpectedY) > nInYSize / nSteps)
                    nFailedCount ++;
            }
        }
    }

/* -------------------------------------------------------------------- */
/*      If any of the edge points failed to transform, we need to       */
/*      build a fairly detailed internal grid of points instead to      */
/*      help identify the area that is transformable.                   */
/* -------------------------------------------------------------------- */
    if( nFailedCount > 0 )
    {
        int iStep2;
        double dfRatio2;
        nSamplePoints = 0;

        // Take N_STEPS steps
        for( iStep = 0; iStep <= nSteps; iStep ++ )
        {
            dfRatio = (iStep == nSteps) ? 1.0 : iStep * dfStep;

            for( iStep2 = 0; iStep2 <= nSteps; iStep2 ++ )
            {
                dfRatio2 = (iStep2 == nSteps) ? 1.0 : iStep2 * dfStep;

                // Along top
                padfX[nSamplePoints]   = dfRatio2 * nInXSize;
                padfY[nSamplePoints]   = dfRatio * nInYSize;
                padfZ[nSamplePoints++] = 0.0;
            }
        }

        CPLAssert( nSamplePoints == nSampleMax );

        if( !pfnTransformer( pTransformArg, FALSE, nSamplePoints,
                             padfX, padfY, padfZ, pabSuccess ) )
        {
            CPLError( CE_Failure, CPLE_AppDefined,
                      "GDALSuggestedWarpOutput() failed because the passed\n"
                      "transformer failed." );

            CPLFree( padfX );
            CPLFree( padfXRevert );
            CPLFree( pabSuccess );

            return CE_Failure;
        }
    }

/* -------------------------------------------------------------------- */
/*      Collect the bounds, ignoring any failed points.                 */
/* -------------------------------------------------------------------- */
    double dfMinXOut=0, dfMinYOut=0, dfMaxXOut=0, dfMaxYOut=0;
    int    bGotInitialPoint = FALSE;

    nFailedCount = 0;
    for( i = 0; i < nSamplePoints; i++ )
    {

        int x_i = i % (nSteps + 1);
        int y_i = i / (nSteps + 1);

        if (x_i > 0 && (pabSuccess[i-1] || pabSuccess[i]))
        {
            double x_out_before = padfX[i-1];
            double x_out_after = padfX[i];
            int nIter = 0;
            double x_in_before = (x_i - 1) * nInXSize * 1.0 / nSteps;
            double x_in_after = x_i * nInXSize * 1.0 / nSteps;
            int valid_before = pabSuccess[i-1];
            int valid_after = pabSuccess[i];

            /* Detect discontinuity in target coordinates when the target x coordinates */
            /* change sign. This may be a false positive when the targe tx is around 0 */
            /* Dichotomic search to reduce the interval to near the discontinuity and */
            /* get a better out extent */
            while ( (!valid_before || !valid_after ||
                     x_out_before * x_out_after < 0) && nIter < 16 )
            {
                double x = (x_in_before + x_in_after) / 2;
                double y = y_i * nInYSize * 1.0 / nSteps;
                double z= 0;
                //fprintf(stderr, "[%d] (%f, %f) -> ", nIter, x, y);
                int bSuccess = TRUE;
                if( !pfnTransformer( pTransformArg, FALSE, 1,
                                     &x, &y, &z, &bSuccess ) || !bSuccess )
                {
                    //fprintf(stderr, "invalid\n");
                    if (!valid_before)
                    {
                        x_in_before = (x_in_before + x_in_after) / 2;
                    }
                    else if (!valid_after)
                    {
                        x_in_after = (x_in_before + x_in_after) / 2;
                    }
                    else
                        break;
                }
                else
                {
                    //fprintf(stderr, "(%f, %f)\n", x, y);

                    if( !bGotInitialPoint )
                    {
                        bGotInitialPoint = TRUE;
                        dfMinXOut = dfMaxXOut = x;
                        dfMinYOut = dfMaxYOut = y;
                    }
                    else
                    {
                        dfMinXOut = MIN(dfMinXOut,x);
                        dfMinYOut = MIN(dfMinYOut,y);
                        dfMaxXOut = MAX(dfMaxXOut,x);
                        dfMaxYOut = MAX(dfMaxYOut,y);
                    }

                    if (!valid_before || x_out_before * x < 0)
                    {
                        valid_after = TRUE;
                        x_in_after = (x_in_before + x_in_after) / 2;
                        x_out_after = x;
                    }
                    else
                    {
                        valid_before = TRUE;
                        x_out_before = x;
                        x_in_before = (x_in_before + x_in_after) / 2;
                    }
                }
                nIter ++;
            }
        }

        if( !pabSuccess[i] )
        {
            nFailedCount++;
            continue;
        }

        if( !bGotInitialPoint )
        {
            bGotInitialPoint = TRUE;
            dfMinXOut = dfMaxXOut = padfX[i];
            dfMinYOut = dfMaxYOut = padfY[i];
        }
        else
        {
            dfMinXOut = MIN(dfMinXOut, padfX[i]);
            dfMinYOut = MIN(dfMinYOut, padfY[i]);
            dfMaxXOut = MAX(dfMaxXOut, padfX[i]);
            dfMaxYOut = MAX(dfMaxYOut, padfY[i]);
        }
    }

    if( nFailedCount > nSamplePoints - 10 )
    {
        CPLError( CE_Failure, CPLE_AppDefined,
                  "Too many points (%d out of %d) failed to transform,\n"
                  "unable to compute output bounds.",
                  nFailedCount, nSamplePoints );

        CPLFree( padfX );
        CPLFree( padfXRevert );
        CPLFree( pabSuccess );

        return CE_Failure;
    }

    if( nFailedCount > 0 )
        CPLDebug( "GDAL",
                  "GDALSuggestedWarpOutput(): %d out of %d points failed to transform.",
                  nFailedCount, nSamplePoints );

/* -------------------------------------------------------------------- */
/*      Compute the distance in "georeferenced" units from the top      */
/*      corner of the transformed input image to the bottom left        */
/*      corner of the transformed input.  Use this distance to          */
/*      compute an approximate pixel size in the output                 */
/*      georeferenced coordinates.                                      */
/* -------------------------------------------------------------------- */
    double dfDiagonalDist, dfDeltaX, dfDeltaY;

    if( pabSuccess[0] && pabSuccess[nSamplePoints - 1] )
    {
        dfDeltaX = padfX[nSamplePoints-1] - padfX[0];
        dfDeltaY = padfY[nSamplePoints-1] - padfY[0];
    }
    else
    {
        dfDeltaX = dfMaxXOut - dfMinXOut;
        dfDeltaY = dfMaxYOut - dfMinYOut;
    }

    dfDiagonalDist = sqrt( dfDeltaX * dfDeltaX + dfDeltaY * dfDeltaY );

/* -------------------------------------------------------------------- */
/*      Compute a pixel size from this.                                 */
/* -------------------------------------------------------------------- */
    double dfPixelSize;

    dfPixelSize = dfDiagonalDist
        / sqrt(((double)nInXSize)*nInXSize + ((double)nInYSize)*nInYSize);

    *pnPixels = (int) ((dfMaxXOut - dfMinXOut) / dfPixelSize + 0.5);
    *pnLines = (int) ((dfMaxYOut - dfMinYOut) / dfPixelSize + 0.5);

    double dfPixelSizeX = dfPixelSize;
    double dfPixelSizeY = dfPixelSize;

    double adfExtent[4];
    const double adfRatioArray[] = { 0, 0.001, 0.01, 0.1, 1 };
    size_t nRetry;

#define N_ELEMENTS(x) (sizeof(x) / sizeof(x[0]))

/* -------------------------------------------------------------------- */
/*      Check that the right border is not completely out of source     */
/*      image. If so, adjust the x pixel size a bit in the hope it will */
/*      fit.                                                            */
/* -------------------------------------------------------------------- */
    for( nRetry = 0; nRetry < N_ELEMENTS(adfRatioArray); nRetry ++ )
    {
        double dfTryPixelSizeX =
            dfPixelSizeX - dfPixelSizeX * adfRatioArray[nRetry] / *pnPixels;
        adfExtent[0] = dfMinXOut;
        adfExtent[1] = dfMaxYOut - (*pnLines) * dfPixelSizeY;
        adfExtent[2] = dfMinXOut + (*pnPixels) * dfTryPixelSizeX;
        adfExtent[3] = dfMaxYOut;
        if (!GDALSuggestedWarpOutput2_MustAdjustForRightBorder_(
                                            pfnTransformer, pTransformArg,
                                            adfExtent, *pnPixels,  *pnLines,
                                            dfTryPixelSizeX, dfPixelSizeY))
        {
            dfPixelSizeX = dfTryPixelSizeX;
            break;
        }
    }

/* -------------------------------------------------------------------- */
/*      Check that the bottom border is not completely out of source    */
/*      image. If so, adjust the y pixel size a bit in the hope it will */
/*      fit.                                                            */
/* -------------------------------------------------------------------- */
    for( nRetry = 0; nRetry < N_ELEMENTS(adfRatioArray); nRetry ++ )
    {
        double dfTryPixelSizeY =
            dfPixelSizeY - dfPixelSizeY * adfRatioArray[nRetry] / *pnLines;
        adfExtent[0] = dfMinXOut;
        adfExtent[1] = dfMaxYOut - (*pnLines) * dfTryPixelSizeY;
        adfExtent[2] = dfMinXOut + (*pnPixels) * dfPixelSizeX;
        adfExtent[3] = dfMaxYOut;
        if (!GDALSuggestedWarpOutput2_MustAdjustForBottomBorder_(
                                            pfnTransformer, pTransformArg,
                                            adfExtent, *pnPixels,  *pnLines,
                                            dfPixelSizeX, dfTryPixelSizeY))
        {
            dfPixelSizeY = dfTryPixelSizeY;
            break;
        }
    }


/* -------------------------------------------------------------------- */
/*      Recompute some bounds so that all return values are consistant  */
/* -------------------------------------------------------------------- */
    dfMaxXOut = dfMinXOut + (*pnPixels) * dfPixelSizeX;
    dfMinYOut = dfMaxYOut - (*pnLines) * dfPixelSizeY;

    /* -------------------------------------------------------------------- */
    /*      Return raw extents.                                             */
    /* -------------------------------------------------------------------- */
    padfExtent[0] = dfMinXOut;
    padfExtent[1] = dfMinYOut;
    padfExtent[2] = dfMaxXOut;
    padfExtent[3] = dfMaxYOut;

    /* -------------------------------------------------------------------- */
    /*      Set the output geotransform.                                    */
    /* -------------------------------------------------------------------- */
    padfGeoTransformOut[0] = dfMinXOut;
    padfGeoTransformOut[1] = dfPixelSizeX;
    padfGeoTransformOut[2] = 0.0;
    padfGeoTransformOut[3] = dfMaxYOut;
    padfGeoTransformOut[4] = 0.0;
    padfGeoTransformOut[5] = - dfPixelSizeY;

    CPLFree( padfX );
    CPLFree( padfXRevert );
    CPLFree( pabSuccess );

    return CE_None;
}

}  // namespace anonimo


namespace suri {

/** Nombre del proceso **/
const std::string GeoreferenceProcess::kProcessName = "GeoreferenceProcess";
const std::string GeoreferenceProcess::kOutputElementType = "RasterElement";

/**
 * Ctor.
 */
GeoreferenceProcess::GeoreferenceProcess(Element* pInputElement,
                                         const Subset& ViewportSubset,
                                         Element*& pOutputElement,
                                         const SuriObject::UuidType& LibraryId,
                                         DataViewManager* pDataViewManager,
                                         bool AskGcpFile) :
   ReprojectionProcess(pInputElement, ViewportSubset, LibraryId,
                       pDataViewManager, AskGcpFile),
   pInputElement_(pInputElement), viewportSubset_(ViewportSubset),
   pOutputElement_(pOutputElement), libraryId_(LibraryId),
   pDataViewManager_(pDataViewManager) {
SetProcessName(kProcessName);
this->showResizePart_ = false;
}

/**
 * Dtor.
 */
GeoreferenceProcess::~GeoreferenceProcess() {
}

/**
 * Corre el proceso y genera la salida usando los metodos de configuracion.
 */
bool GeoreferenceProcess::RunProcess() {
   std::string srcfilepath = pInputElement_->GetUrl().c_str();
   std::string tempfilepath = wxFileName::CreateTempFileName(_("")).c_str();

   std::string destfilepath;
   GetProcessAdaptLayer()->GetAttribute<std::string>(
         ProcessAdaptLayer::OutputFileNameKeyAttr, destfilepath);

   std::string destfileformat;
   GetProcessAdaptLayer()->GetAttribute<std::string>(
         ProcessAdaptLayer::OutputFileFormatKeyAttr, destfileformat);

   int order = 0;
   GetProcessAdaptLayer()->GetAttribute<int>(ProcessAdaptLayer::AlgorithmOrderKeyAttr,
                                             order);

   std::vector<BandInfo> bands;
   GetProcessAdaptLayer()->GetAttribute<std::vector<BandInfo> >(
         ProcessAdaptLayer::BandInfoKeyAttr, bands);

   int pixels = 0, lines = 0;
   GetProcessAdaptLayer()->GetAttribute<int>(ProcessAdaptLayer::OutputSizePixelsKeyAttr,
                                             pixels);
   GetProcessAdaptLayer()->GetAttribute<int>(ProcessAdaptLayer::OutputSizeLinesKeyAttr,
                                             lines);

   double pixelwidth = 0.0, pixelheight = 0.0;
   GetProcessAdaptLayer()->GetAttribute<double>(
         ProcessAdaptLayer::OutputSizePixelWidthKeyAttr, pixelwidth);
   GetProcessAdaptLayer()->GetAttribute<double>(
         ProcessAdaptLayer::OutputSizePixelHeightKeyAttr, pixelheight);

   Subset geobounds;
   GetProcessAdaptLayer()->GetAttribute<Subset>(ProcessAdaptLayer::SpatialBoundsKeyAttr,
                                                geobounds);

   int count = 0;
   GDAL_GCP* pgdalgcps = NULL;
   std::string srcwkt;

   GetGdalGcps(pgdalgcps, count, srcwkt);

   if (count == 0)
      return false;

   // No se encontro la magia negra que hace funcionar a la clase Progress asi
   // que se usa directamente el wxProgressDialog
   wxProgressDialog* pprogress = new wxProgressDialog("Configurando georreferenciacion...", "", 10);
   pprogress->SetSize(wxSize(400, -1));
   pprogress->Centre(wxCENTRE_ON_SCREEN);

   bool transok = Translate(srcfilepath, tempfilepath, destfileformat, pgdalgcps, count,
                         bands, pixels, lines, geobounds, pprogress);

   delete pprogress;
   pprogress = NULL;

   pprogress = new wxProgressDialog("Georreferenciando...", "", 10);
   pprogress->SetSize(wxSize(400, -1));
   pprogress->Centre(wxCENTRE_ON_SCREEN);
   bool warpok = Warp(tempfilepath, destfilepath, destfileformat, srcwkt, true, order,
                      pixelwidth, pixelheight, pprogress);

   ReleaseGdalGcps(pgdalgcps, count);

   if (transok && warpok)
      ConfigureOutput();

   delete pprogress;
   pprogress = NULL;

   return transok && warpok;
}

/**
 * Permite configurar la salida del proceso antes de correrlo.
 */
bool GeoreferenceProcess::ConfigureProcess() {
   return true;
}

/**
 * Una vez ejecutado el proceso hace visible la salida.
 */
bool GeoreferenceProcess::ConfigureOutput() {
   std::string destfilepath;
      this->pAdaptLayer_->GetAttribute<std::string>(
            ProcessAdaptLayer::OutputFileNameKeyAttr, destfilepath);
   // Crea el elemento con el url del objeto salvado
   pOutputElement_ = Element::Create(kOutputElementType, destfilepath);
   wxFileName file(destfilepath);
   if (pOutputElement_) {
      pOutputElement_->SetName(file.GetName());
      std::string parameters = GetParametersAsXmlString();
      REPORT_DEBUG("Parametros del proceso %s", parameters.c_str());
      wxStringInputStream ss(parameters);
      wxXmlDocument doc(ss, suri::XmlElement::xmlEncoding_);
      if (pOutputElement_->GetNode(PROCESS_HISTORY_NODE) == NULL)
         pOutputElement_->AddNode(pOutputElement_->GetNode(), PROCESS_HISTORY_NODE);
      pOutputElement_->AddNode(pOutputElement_->GetNode(PROCESS_HISTORY_NODE), doc.DetachRoot());
   }
   return pOutputElement_ != NULL;
}

/**
 * Copia informacion de una banda.
 */
void GeoreferenceProcess::CopyBandInfo(GDALRasterBand* SrcBand, GDALRasterBand* DstBand,
                                       int CanCopyStatsMetadata, int CopyScale,
                                       int CopyNoData) {
   if (CanCopyStatsMetadata) {
      DstBand->SetMetadata(SrcBand->GetMetadata());
   } else {
      char** metadata = SrcBand->GetMetadata();
      char** metadatanew = NULL;
      for (int i = 0; metadata != NULL && metadata[i] != NULL; i++) {
         if (strncmp(metadata[i], "STATISTICS_", 11) != 0)
            metadatanew = CSLAddString(metadatanew, metadata[i]);
      }
      DstBand->SetMetadata(metadatanew);
      CSLDestroy(metadatanew);
   }

   DstBand->SetColorTable(SrcBand->GetColorTable());
   DstBand->SetColorInterpretation(SrcBand->GetColorInterpretation());
   if (strlen(SrcBand->GetDescription()) > 0)
      DstBand->SetDescription(SrcBand->GetDescription());

   if (CopyNoData) {
      int success = FALSE;
      double nodata = SrcBand->GetNoDataValue(&success);
      if (success) DstBand->SetNoDataValue(nodata);
   }

   if (CopyScale) {
      DstBand->SetOffset(SrcBand->GetOffset());
      DstBand->SetScale(SrcBand->GetScale());
   }

   DstBand->SetCategoryNames(SrcBand->GetCategoryNames());
   if (!EQUAL(SrcBand->GetUnitType(), "")) DstBand->SetUnitType(SrcBand->GetUnitType());
}

/**
 * Le agrega la informacion necesaria a la imagen resultante para luego poder
 * transformala y obtener una imagen georreferenciada.
 *
 * @param[in] SrcFile Archivo de origen que se va a usar para generar una imagen procesar.
 * @param[in] DestFile Archivo de destino al cual se le va a agregar la info necesaria
 * para una posterior transformacion.
 * @param[in] Format Driver (ej: GTiff)
 * @param[in] pGCPs Puntero a un vector con los puntos de control disponibles.
 * @param[in] GCPsCount Cantidad de puntos de control disponibles.
 * @param[in] Bands Bandas que se van a utilizar.
 * @param[in] Pixels Pixels de salida.
 * @param[in] Lines Lineas de salida.
 * @param[in] GeoBounds Limites georreferenciados de la imagen.
 *
 * \todo (danieltesta-#4136): verificar sub datasets.
 */
bool GeoreferenceProcess::Translate(const std::string& SrcFile,const std::string& DestFile,
                                    const std::string& Format, const GDAL_GCP* pGCPs,
                                    int GCPsCount, const std::vector<BandInfo>& Bands,
                                    int Pixels, int Lines, const Subset& GeoBounds,
                                    void* pProgressData) {
   GDALDriverH driver;
   GDALDatasetH srcds, destds;
   int rasterxsize = 0, rasterysize = 0;
   int srcwin[4] = { 0 };
   std::vector<int> bandVector;
   VRTDataset* pvirtds = NULL;

   int oxsize = 0, oysize = 0;
   double adfGeoTransform[6] = { 0.0 };

   // Inicializo Gdal con los drivers.
   GDALAllRegister();

   // Abro el origen.
   srcds = GDALOpen(SrcFile.c_str(), GA_ReadOnly);
   if (srcds == NULL) {
      SHOW_ERROR(message_CANNOT_OPEN_SOURCE_FILE_GDAL,
                 CPLGetLastErrorNo(), CPLGetLastErrorMsg());
      GDALDestroyDriverManager();
      return false;
   }

   // *** verificar sub datasets *** //
   // ****************************** //

   // Saco info del dataset.
   rasterxsize = GDALGetRasterXSize(srcds);
   rasterysize = GDALGetRasterYSize(srcds);



   // cargo las bandas.
   int bandcount = Bands.size();
   bandVector.resize(bandcount);
   for (int ix = 0; ix < bandcount; ++ix) {
      bandVector[ix] = Bands[ix].GetPosition() + 1;
   }

   // Cargo el driver para el formato de salida.
   driver = GDALGetDriverByName(const_cast<char*>(Format.c_str()));
   if (driver == NULL) {
      SHOW_ERROR(message_FORMAT_NOT_SUPPORTED, const_cast<char*>(Format.c_str()));
      GDALClose(srcds);
      GDALDestroyDriverManager();
      return false;
   }

   // ventana espacial
   if (GeoBounds.IsValid()) {
      RasterElement* praster = dynamic_cast<RasterElement*>(pInputElement_);
      if (praster != NULL) {
         RasterSpatialModel* prastermodel = RasterSpatialModel::Create(
               praster->GetRasterModel());
         Coordinates src(GeoBounds.ul_);
         Coordinates dest(GeoBounds.lr_);

         if (prastermodel) {
            prastermodel->InverseTransform(src);
            prastermodel->InverseTransform(dest);
            srcwin[0] = static_cast<int>(src.x_);
            srcwin[1] = static_cast<int>(src.y_);
            srcwin[2] = static_cast<int>(dest.x_ - src.x_);
            srcwin[3] = static_cast<int>(dest.y_ - src.y_);
            RasterSpatialModel::Destroy(prastermodel);
         }
      }
   } else {
      if (srcwin[2] == 0 && srcwin[3] == 0) {
         srcwin[2] = rasterxsize;
         srcwin[3] = rasterysize;
      }
   }

   // parametros
   if (Pixels == 0 && Lines == 0) {
      oxsize = srcwin[2];
      oysize = srcwin[3];
   } else {
      oxsize = Pixels;
      oysize = Lines;
   }

   // Dataset virtual para hacer los cambios y despues volcarlos a disco.
   pvirtds = (VRTDataset *) VRTCreate(oxsize, oysize);

   // Cargo los GCPs
   if (GCPsCount > 0) {
      const char *pgcpprojection = GDALGetGCPProjection(srcds);
      if (pgcpprojection == NULL)
         pgcpprojection = "";

      pvirtds->SetGCPs(GCPsCount, pGCPs, pgcpprojection);
   } else {
      const char* pszProjection = GDALGetProjectionRef(srcds);
      if (pszProjection != NULL && strlen(pszProjection) > 0)
         pvirtds->SetProjection(pszProjection);
   }

   if (GeoBounds.IsValid()) {
      adfGeoTransform[0] = GeoBounds.ul_.x_;
      adfGeoTransform[1] = (GeoBounds.lr_.x_ - GeoBounds.ul_.x_) / oxsize;
      adfGeoTransform[2] = 0.0;
      adfGeoTransform[3] = GeoBounds.ul_.y_;
      adfGeoTransform[4] = 0.0;
      adfGeoTransform[5] = (GeoBounds.lr_.y_ - GeoBounds.ul_.y_) / oysize;

      pvirtds->SetGeoTransform(adfGeoTransform);
   } else if (GDALGetGeoTransform(srcds, adfGeoTransform) == CE_None && GCPsCount == 0) {
      adfGeoTransform[0] += srcwin[0] * adfGeoTransform[1]
            + srcwin[1] * adfGeoTransform[2];
      adfGeoTransform[3] += srcwin[0] * adfGeoTransform[4]
            + srcwin[1] * adfGeoTransform[5];

      adfGeoTransform[1] *= srcwin[2] / (double) oxsize;
      adfGeoTransform[2] *= srcwin[3] / (double) oysize;
      adfGeoTransform[4] *= srcwin[2] / (double) oxsize;
      adfGeoTransform[5] *= srcwin[3] / (double) oysize;

      pvirtds->SetGeoTransform(adfGeoTransform);
   }

   char** ppmetadata = CSLDuplicate(((GDALDataset*)srcds)->GetMetadata());

   pvirtds->SetMetadata(ppmetadata);
   CSLDestroy(ppmetadata);

   const char* pinterleave = GDALGetMetadataItem(srcds, "INTERLEAVE",
                                                   "IMAGE_STRUCTURE");
   if (pinterleave)
      pvirtds->SetMetadataItem("INTERLEAVE", pinterleave, "IMAGE_STRUCTURE");

   // Mantengo datos espaciales.
   char** ppmd = ((GDALDataset*) srcds)->GetMetadata("RPC");
   if (ppmd != NULL) pvirtds->SetMetadata(ppmd, "RPC");

   ppmd = ((GDALDataset*) srcds)->GetMetadata("GEOLOCATION");
   if (ppmd != NULL) pvirtds->SetMetadata(ppmd, "GEOLOCATION");

   // Copio las bandas.
   for (int ix = 0; ix < bandcount; ++ix) {
      VRTSourcedRasterBand* pvirtband;
      GDALRasterBand* psrcband;

      int nSrcBand = bandVector[ix];

      psrcband = ((GDALDataset *) srcds)->GetRasterBand(ABS(nSrcBand));

      pvirtds->AddBand(psrcband->GetRasterDataType(), NULL);
      pvirtband = (VRTSourcedRasterBand *) pvirtds->GetRasterBand(ix + 1);
      if (nSrcBand < 0) {
         pvirtband->AddMaskBandSource(psrcband);
         continue;
      }

      pvirtband->AddSimpleSource(psrcband, srcwin[0], srcwin[1], srcwin[2],
                                 srcwin[3], 0, 0, oxsize, oysize);

      CopyBandInfo(psrcband, pvirtband, FALSE, TRUE, FALSE);
   }

   // Mando al disco.
   destds = GDALCreateCopy(driver, DestFile.c_str(),
                           (GDALDatasetH) pvirtds, FALSE, NULL,
                           DummyProgreeProc, pProgressData);

   if (destds != NULL) {
      int hasgoterr = FALSE;
      CPLErrorReset();
      GDALFlushCache(destds);
      if (CPLGetLastErrorType() != CE_None) hasgoterr = TRUE;
      GDALClose(destds);
      if (hasgoterr) destds = NULL;
   }

   GDALClose((GDALDatasetH) pvirtds);
   GDALClose(srcds);

   return true;
}

/**
 * Crea el archivo de salida.
 */
GDALDatasetH GeoreferenceProcess::GDALWarpCreateOutput(GDALDatasetH hSrcDS,
                                                       const char *pszFilename,
                                                       const char *pszFormat,
                                                       const char *pszSourceSRS,
                                                       const char *pszTargetSRS,
                                                       bool UseGCPs, int nOrder,
                                                       double PixelWidth,
                                                       double PixelHeight,
                                                       char **papszCreateOptions) {
   GDALDriverH driver;
   GDALDatasetH destds;
   void* transformarg;
   double adfdstgeotransform[6] = { 0.0 };
   double adfExtent[4] = { 0.0 };
   int pixels = 0, lines = 0;
   GDALColorTableH colortable;
   double dfWrkMinX = 0, dfWrkMaxX = 0, dfWrkMinY = 0, dfWrkMaxY = 0;
   double dfWrkResX = 0, dfWrkResY = 0;

   // Driver de salida.
   driver = GDALGetDriverByName(pszFormat);
   if (driver == NULL || GDALGetMetadataItem(driver, GDAL_DCAP_CREATE, NULL) == NULL) {
      SHOW_ERROR(message_FORMAT_NOT_SUPPORTED, pszFormat);
      return NULL;
   }

   // Transformacion de origen a destino.
   transformarg = GDALCreateGenImgProjTransformer(hSrcDS, pszSourceSRS, NULL,
                                                   pszTargetSRS, UseGCPs, 0.0, nOrder);

   if (transformarg == NULL) return NULL;

   GDALTransformerInfo* psInfo = (GDALTransformerInfo*)transformarg;
   if (GDALSuggestedWarpOutput2_(hSrcDS, psInfo->pfnTransform, transformarg,
                                adfdstgeotransform, &pixels, &lines, adfExtent,
                                0) != CE_None)
      return NULL;

   if (CPLGetConfigOption("CHECK_WITH_INVERT_PROJ", NULL) == NULL) {
      double MinX = adfExtent[0];
      double MaxX = adfExtent[2];
      double MaxY = adfExtent[3];
      double MinY = adfExtent[1];
      int bSuccess = TRUE;

      /* Check that the the edges of the target image are in the validity area */
      /* of the target projection */
#define N_STEPS 20
      int i, j;
      for (i = 0; i <= N_STEPS && bSuccess; i++) {
         for (j = 0; j <= N_STEPS && bSuccess; j++) {
            double dfRatioI = i * 1.0 / N_STEPS;
            double dfRatioJ = j * 1.0 / N_STEPS;
            double expected_x = (1 - dfRatioI) * MinX + dfRatioI * MaxX;
            double expected_y = (1 - dfRatioJ) * MinY + dfRatioJ * MaxY;
            double x = expected_x;
            double y = expected_y;
            double z = 0;
            /* Target SRS coordinates to source image pixel coordinates */
            if (!psInfo->pfnTransform(transformarg, TRUE, 1, &x, &y, &z, &bSuccess)
                  || !bSuccess) bSuccess = FALSE;
            /* Source image pixel coordinates to target SRS coordinates */
            if (!psInfo->pfnTransform(transformarg, FALSE, 1, &x, &y, &z, &bSuccess)
                  || !bSuccess) bSuccess = FALSE;
            if (fabs(x - expected_x) > (MaxX - MinX) / pixels
                  || fabs(y - expected_y) > (MaxY - MinY) / lines) bSuccess = FALSE;
         }
      }

      /* If not, retry with CHECK_WITH_INVERT_PROJ=TRUE that forces ogrct.cpp */
      /* to check the consistency of each requested projection result with the */
      /* invert projection */
      if (!bSuccess) {
         CPLSetConfigOption("CHECK_WITH_INVERT_PROJ", "TRUE");
         CPLDebug("WARP", "Recompute out extent with CHECK_WITH_INVERT_PROJ=TRUE");

         if (GDALSuggestedWarpOutput2_(hSrcDS, psInfo->pfnTransform, transformarg,
                                      adfdstgeotransform, &pixels, &lines,
                                      adfExtent, 0) != CE_None) {
            return NULL;
         }
      }
   }

   if( dfWrkMaxX == 0.0 && dfWrkMinX == 0.0 ) {
      dfWrkMinX = adfExtent[0];
      dfWrkMaxX = adfExtent[2];
      dfWrkMaxY = adfExtent[3];
      dfWrkMinY = adfExtent[1];
      dfWrkResX = adfdstgeotransform[1];
      dfWrkResY = ABS(adfdstgeotransform[5]);
   }

   GDALDestroyGenImgProjTransformer(transformarg);

   adfdstgeotransform[0] = dfWrkMinX;
   adfdstgeotransform[1] = dfWrkResX;
   adfdstgeotransform[2] = 0.0;
   adfdstgeotransform[3] = dfWrkMaxY;
   adfdstgeotransform[4] = 0.0;
   adfdstgeotransform[5] = -1 * dfWrkResY;

   pixels = static_cast<int>(((dfWrkMaxX - dfWrkMinX) / dfWrkResX + 0.5));
   lines = static_cast<int>(((dfWrkMaxY - dfWrkMinY) / dfWrkResY + 0.5));

   // Sobreescribo tamaño de pixel
   if (PixelWidth != 0 && PixelHeight != 0) {
      double dfMinX = adfdstgeotransform[0];
      double dfMaxX = adfdstgeotransform[0] + adfdstgeotransform[1] * pixels;
      double dfMaxY = adfdstgeotransform[3];
      double dfMinY = adfdstgeotransform[3] + adfdstgeotransform[5] * lines;

      pixels = static_cast<int>(((dfMaxX - dfMinX + (PixelWidth / 2.0)) / PixelWidth));
      lines = static_cast<int>(((dfMaxY - dfMinY + (PixelHeight / 2.0)) / PixelHeight));
      adfdstgeotransform[0] = dfMinX;
      adfdstgeotransform[3] = dfMaxY;
      adfdstgeotransform[1] = PixelWidth;
      adfdstgeotransform[5] = -PixelHeight;
   }

   // Archivo de salida.
   destds = GDALCreate(driver, pszFilename, pixels, lines, GDALGetRasterCount(hSrcDS),
                       GDALGetRasterDataType(GDALGetRasterBand(hSrcDS, 1)),
                       papszCreateOptions);

   if (destds == NULL) return NULL;

   // Definicion de la proyeccion.
   GDALSetProjection(destds, pszTargetSRS);
   GDALSetGeoTransform(destds, adfdstgeotransform);

   // Copio la tabla de colores.
   colortable = GDALGetRasterColorTable(GDALGetRasterBand(hSrcDS, 1));
   if (colortable != NULL) GDALSetRasterColorTable(GDALGetRasterBand(destds, 1), colortable);

   return destds;
}

/**
 * Aplica los GCPs guardados (mapeo de puntos y transformaciones) en la imagen
 * para generar la imagen final.
 *
 * @param[in] SrcFile Archivo de origen con los GCPs.
 * @param[in] DestFile Archivo de destino. Es la imagen que se quiere
 * como resultado final del proceso.
 * @param[in] Format Driver (ej: GTiff)
 * @param[in] InWkt SRS para la transformacion.
 * @param[in] Order Orden para el polinomio de transformacion.
 * @return true si el proceso finalizo con exito.
 */
bool GeoreferenceProcess::Warp(const std::string& SrcFile, const std::string& DestFile,
                               const std::string& Format, const std::string& InWkt,
                               bool UseGCPs, int Order, double PixelWidth,
                               double PixelHeight, void* pProgressData) {
   GDALDatasetH srcds = NULL, destds = NULL;
   char* sourcesrs = NULL;
   void* ptransformarg = NULL;
   void* pgenimgprojarg = NULL;
   void* papproxarg = NULL;
   double errorthreshold = 0.125;
   GDALTransformerFunc ptransformerproc = NULL;
   char** ppcreateoptions = NULL;

   GDALAllRegister();

   char* psrcfilename = const_cast<char*>(SrcFile.c_str());
   char* pdestfilename = const_cast<char*>(DestFile.c_str());

   srcds = GDALOpen(psrcfilename, GA_ReadOnly);

   if (srcds == NULL)
      return false;

   if (GDALGetRasterCount(srcds) == 0) {
      SHOW_ERROR(message_NO_RASTER_BANDS, psrcfilename);
      return false;
   }

   if (sourcesrs == NULL) {
      if (GDALGetProjectionRef(srcds) != NULL && strlen(GDALGetProjectionRef(srcds)) > 0)
         sourcesrs = CPLStrdup(GDALGetProjectionRef(srcds));

      else if (GDALGetGCPProjection(srcds) != NULL
            && strlen(GDALGetGCPProjection(srcds)) > 0 && GDALGetGCPCount(srcds) > 1)
         sourcesrs = CPLStrdup(GDALGetGCPProjection(srcds));
      else
         sourcesrs = CPLStrdup("");
   }

   char* ptargetsrs = const_cast<char*>(InWkt.c_str());
   if (InWkt.empty()) ptargetsrs = CPLStrdup(sourcesrs);

   CPLPushErrorHandler (CPLQuietErrorHandler);
   CPLPopErrorHandler();

   destds = GDALWarpCreateOutput(srcds, pdestfilename, Format.c_str(), sourcesrs,
                                 ptargetsrs, UseGCPs, Order, PixelWidth, PixelHeight,
                                 ppcreateoptions);
   CSLDestroy(ppcreateoptions);
   ppcreateoptions = NULL;

   if (destds == NULL)
      return false;

   ptransformarg = pgenimgprojarg =
         GDALCreateGenImgProjTransformer(srcds, sourcesrs, destds, ptargetsrs, UseGCPs, 0.0, Order);

   if (ptransformarg == NULL)
      return false;

   ptransformerproc = GDALGenImgProjTransform;

   // Verificar si esto es necesario o no ( ! )
   if (errorthreshold != 0.0) {
      ptransformarg = papproxarg = GDALCreateApproxTransformer(GDALGenImgProjTransform,
                                                               pgenimgprojarg,
                                                               errorthreshold);
      ptransformerproc = GDALApproxTransform;
   }

   // Configuro las opciones de transformacion
   GDALWarpOptions *psWarpOptions = GDALCreateWarpOptions();

   psWarpOptions->hSrcDS = srcds;
   psWarpOptions->hDstDS = destds;
   psWarpOptions->eResampleAlg = GRA_NearestNeighbour;

   psWarpOptions->nBandCount = GDALGetRasterCount(srcds);
   psWarpOptions->panSrcBands = (int*) CPLMalloc(sizeof(int) * psWarpOptions->nBandCount);
   psWarpOptions->panDstBands = (int *) CPLMalloc(sizeof(int) * psWarpOptions->nBandCount);

   for (int ix = 0; ix < psWarpOptions->nBandCount; ++ix) {
      psWarpOptions->panSrcBands[ix] = ix + 1;
      psWarpOptions->panDstBands[ix] = ix + 1;
   }

   psWarpOptions->pfnProgress = DummyProgreeProc;
   psWarpOptions->pProgressArg = pProgressData;

   // reproyeccion
   psWarpOptions->pTransformerArg = ptransformarg;
   psWarpOptions->pfnTransformer = ptransformerproc;

   GDALWarpOperation oOperation;

   oOperation.Initialize(psWarpOptions);
   oOperation.ChunkAndWarpImage(0, 0, GDALGetRasterXSize(destds), GDALGetRasterYSize(destds));

   GDALDestroyWarpOptions(psWarpOptions);

   if (papproxarg != NULL) GDALDestroyApproxTransformer(papproxarg);

   if (pgenimgprojarg != NULL) GDALDestroyGenImgProjTransformer(pgenimgprojarg);

   GDALFlushCache(destds);
   GDALClose(destds);

   GDALClose(srcds);

   GDALDumpOpenDatasets (stderr);

   return true;
}

/**
 * Obtiene los puntos de control del archivo especificado junto con la cantidad
 * y SRS.
 *
 * @param[out] pGdalGcps Puntero al vector de puntos de control.
 * @param[out] Count Cantidad de puntos cargados.
 * @param[out] SrcWkt SRS para los puntos de control.
 */
void GeoreferenceProcess::GetGdalGcps(GDAL_GCP*& pGdalGcps, int& Count, std::string& SrcWkt) {
   std::string gcpfilename;
   GetProcessAdaptLayer()->GetAttribute(ProcessAdaptLayer::GcpListFileNameKeyAttr,
                                        gcpfilename);

   GcpList gcplist;
   TextFileGcpLoader pgcpLoader;
   pgcpLoader.Load(gcpfilename, gcplist);

   SrcWkt = gcplist.GetSpatialReferenceSource();

   RasterSpatialModel* psourcerastermodel = RasterSpatialModel::Create(
         gcplist.GetRasterModelSource());

   std::vector<GroundControlPoint> surigcps = gcplist.CreateAdaptedGcps();

   Count = surigcps.size();
   pGdalGcps = new GDAL_GCP[Count];
   for (int ix = 0, lenix = surigcps.size(); ix < lenix; ++ix) {
      Coordinates src = surigcps[ix].GetSource();
      Coordinates dest = surigcps[ix].GetDestination();

      if (psourcerastermodel)
         psourcerastermodel->Transform(src);

      char* pszid = new char[11];
      sprintf(pszid, "%d", ix + 1);

      pGdalGcps[ix].pszId = pszid;
      pGdalGcps[ix].pszInfo = const_cast<char*>(std::string("").c_str());
      pGdalGcps[ix].dfGCPPixel = dest.x_;
      pGdalGcps[ix].dfGCPLine = dest.y_;
      pGdalGcps[ix].dfGCPX = src.x_;
      pGdalGcps[ix].dfGCPY = src.y_;
      pGdalGcps[ix].dfGCPZ = 0.0;
   }
}

/**
 * Libera la memoria reservada para los puntos de control.
 *
 * @param[in] pGdalGcps Puntero al vector de puntos de control a liberar.
 * @param[in] Count Cantidad de puntos de control.
 */
void GeoreferenceProcess::ReleaseGdalGcps(GDAL_GCP* pGdalGcps, int Count) {
   for (int ix = 0; ix < Count; ++ix) {
      delete[] pGdalGcps[ix].pszId;
   }
   delete[] pGdalGcps;
}

/** 
 * Metodo polimorfico que complementa a GetParametersAsXmlString para que las clases
 * agreguen sus atributos especificos
 * @return vector de wxXmlNode con los nodos xml de los parametros especificos del proceso     
 * @return vector vacio en caso de que no tenga parametros adicionales
 **/
std::vector<wxXmlNode*> GeoreferenceProcess::DoGetXmlParameters() const {
   std::vector<wxXmlNode*> params;
   params.push_back(CreateKeyValueNode("transformacion", 
                     pReprojectionParameters_->algorithm_));
   params.push_back(CreateKeyValueNode("orden", 
                           NumberToString<int>(pReprojectionParameters_->algorithmOrder_)));
   if (!pReprojectionParameters_->gcpListFileName_.empty())
      params.push_back(CreateKeyValueNode("gcps", pReprojectionParameters_->gcpListFileName_));   
   int pixels = 0, lines = 0;
   pAdaptLayer_->GetAttribute<int>(ProcessAdaptLayer::OutputSizePixelsKeyAttr,
                                             pixels);
   pAdaptLayer_->GetAttribute<int>(ProcessAdaptLayer::OutputSizeLinesKeyAttr,
                                             lines);
   params.push_back(CreateKeyValueNode("pixels", NumberToString<int>(pixels)));
   params.push_back(CreateKeyValueNode("lineas", NumberToString<int>(lines)));
   
   double pixelwidth = 0.0, pixelheight = 0.0;
   pAdaptLayer_->GetAttribute<double>(
         ProcessAdaptLayer::OutputSizePixelWidthKeyAttr, pixelwidth);
   pAdaptLayer_->GetAttribute<double>(
         ProcessAdaptLayer::OutputSizePixelHeightKeyAttr, pixelheight);

   params.push_back(CreateKeyValueNode("ancho-pixel", NumberToString<int>(pixelwidth)));
   params.push_back(CreateKeyValueNode("alto-pixel", NumberToString<double>(pixelheight)));

   Subset geobounds;
   pAdaptLayer_->GetAttribute<Subset>(ProcessAdaptLayer::SpatialBoundsKeyAttr,
                                                geobounds);
   std::stringstream ss;
   ss << geobounds;
   params.push_back(CreateKeyValueNode("subset", ss.str()));
   return params;
}


}
