/*! \file GdalRasterizer.cpp */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2015-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */


// Includes Estandar
// Includes Suri
#include "VectorDatasource.h"
#include "suri/Vector.h"
#include "suri/Dimension.h"
#include "suri/DataTypes.h"
// Includes Wx
// Includes Gdal
#include "cpl_conv.h"
#include "cpl_multiproc.h"
#include "cpl_string.h"
#include "cpl_vsi.h"
#include "gdal_priv.h"
#include "gdal.h"
#include "gdalwarper.h"
#include "ogr_spatialref.h"
#include "vrtdataset.h"
#include "ogrsf_frmts.h"

// Defines
// forwards






#include "GdalRasterizer.h"

namespace suri{
const std::string GdalRasterizer::kDefaultDriver("GTiff");

int CPL_DLL CPL_STDCALL ProgressProc2(double Complete, const char* pMessage, void* pProgressArg) {
   if (pProgressArg != NULL) {
      wxProgressDialog* pprogress = static_cast<wxProgressDialog*>(pProgressArg);
      pprogress->Update(static_cast<int>(Complete * 10));
   }
   return TRUE;
}

GdalRasterizer::GdalRasterizer(VectorDatasource* pVectorDatasource,
                               const std::vector<int>& SelectedBands,
                               const std::string& DestUrl, const std::string& DataType):
                                     destUrl_(DestUrl),
                                     selectedBands_(SelectedBands), dataType_(DataType){
   pVectorDatasource_ = pVectorDatasource;
   CreateTypeMap();
}

bool GdalRasterizer::Rasterize(const std::string& BurnAttr, int Width, int Height) {
   if (!pVectorDatasource_) return false;
   GDALDataset* srcds;
   GDALAllRegister();
   srcds = CreateGdalRasterDataset(Width, Height);
   if (!srcds) {
       return false;
   }
   Vector* pVector = pVectorDatasource_->GetDatasourceAsVector();
   if (!pVector || pVector->GetLayerCount() == 0) return false;
   OGRLayer* layers[pVector->GetLayerCount()];
   for (int x = 0; x < pVector->GetLayerCount(); x++) {
      layers[x] = pVector->GetLayer(x);
   }
   char** options = NULL;
   options = CSLSetNameValue(options, "ALL_TOUCHED", "TRUE");
   options = CSLSetNameValue(options, "ATTRIBUTE", BurnAttr.c_str());
   int band_list[selectedBands_.size()];
   std::vector<int>::const_iterator it = selectedBands_.begin();
   /* Paso de formato de nro de bandas de sopi a nro de bandas de gdal arranca en 1 */
   int y = 0;
   for (; it != selectedBands_.end(); ++it,y++) {
      band_list[y] = ((*it) + 1);
   }
   wxProgressDialog pprogress(_(caption_OPERATION_IN_PROGRESS), "", 10);
   pprogress.SetSize(wxSize(400, -1));
   pprogress.Centre(wxCENTRE_ON_SCREEN);
   CPLErr err = GDALRasterizeLayers(srcds, selectedBands_.size(), band_list,
                       pVector->GetLayerCount(), (OGRLayerH*)layers, NULL, NULL, NULL,
                       options, ProgressProc2, &pprogress);
   CSLDestroy(options);
   GDALClose(srcds);
   if (err == CE_None)
        return true;
   return false;
}

GDALDataset * GdalRasterizer::CreateGdalRasterDataset(int Width, int Height) {
   GDALDataset *pDataset;
   GDALDriver* pDriver = GetGDALDriverManager()->GetDriverByName(kDefaultDriver.c_str());
   VectorElement* pVectorElement = dynamic_cast<VectorElement*>(pVectorDatasource_->GetElement());
   Subset sub;
   pVectorElement->GetElementExtent(sub);
   Dimension dim(sub);
   double adfGeoTransform[6] = { sub.ul_.x_, dim.GetWidth() / Width, 0,
                                 sub.ul_.y_, 0, -dim.GetHeight() / Height};

   pDataset = pDriver->Create(destUrl_.c_str(), Width, Height, 1, sopiToGdalTypeMap_[dataType_],
                              NULL);
   pDataset->SetProjection(pVectorDatasource_->GetSpatialReference().c_str());
   pDataset->SetGeoTransform(adfGeoTransform);
   return pDataset;
}


void GdalRasterizer::CreateTypeMap() {
   sopiToGdalTypeMap_[DataInfo<unsigned char>::Name] = GDT_Byte;
   sopiToGdalTypeMap_[DataInfo<unsigned short>::Name] = GDT_UInt16;
   sopiToGdalTypeMap_[DataInfo<short>::Name] = GDT_Int16;
   sopiToGdalTypeMap_[DataInfo<unsigned int>::Name] =   GDT_UInt32;
   sopiToGdalTypeMap_[DataInfo<int>::Name] =   GDT_Int32;
   sopiToGdalTypeMap_[DataInfo<float>::Name] =   GDT_Float32;
   sopiToGdalTypeMap_[DataInfo<double>::Name] =   GDT_Float64;
}


}
