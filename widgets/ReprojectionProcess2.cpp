/** \file ReprojectionProcess2.cpp
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#include <memory>

#include "ReprojectionProcess2.h"
#include "suri/ParameterCollection.h"
#include "suri/TransformationFactory.h"
#include "suri/TransformationFactoryBuilder.h"

#include "wx/progdlg.h"

namespace suri {

const std::string ReprojectionProcess2::kProcessName = "ReprojectionProcess";
const std::string ReprojectionProcess2::kOutputElementType = "RasterElement";

/**
 * Ctor.
 */
ReprojectionProcess2::ReprojectionProcess2(Element* pInputElement,
                                           const Subset& ViewportSubset,
                                           Element*& pOutputElement,
                                           const SuriObject::UuidType& LibraryId,
                                           DataViewManager* pDataViewManager) :
      GeoreferenceProcess(pInputElement, ViewportSubset, pOutputElement, LibraryId,
                          pDataViewManager, false) {
   processName_ = kProcessName;
}

/**
 * Dtor
 */
ReprojectionProcess2::~ReprojectionProcess2() {

}

/**
 * Corre el proceso y genera la salida usando los metodos de configuracion.
 */
bool ReprojectionProcess2::RunProcess() {
   std::string srcfilepath = pInputElement_->GetUrl().c_str();
   std::string tempfilepath = wxFileName::CreateTempFileName(_("")).c_str();

   std::string destfilepath;
   GetProcessAdaptLayer()->GetAttribute<std::string>(
         ProcessAdaptLayer::OutputFileNameKeyAttr, destfilepath);

   std::string destfileformat;
   GetProcessAdaptLayer()->GetAttribute<std::string>(
         ProcessAdaptLayer::OutputFileFormatKeyAttr, destfileformat);

   std::string algorithm;
   GetProcessAdaptLayer()->GetAttribute<std::string>(
         ProcessAdaptLayer::AlgorithmKeyAttr, algorithm);

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

   std::string outputwkt;
      GetProcessAdaptLayer()->GetAttribute<std::string>(
            ProcessAdaptLayer::OutputSRSKeyAttr, outputwkt);

   // *** CALCULAR POLINOMICA *** //
   // *** PASAR A UN METODO *** //
   int Count = 0;
   GDAL_GCP* pGdalGcps = NULL;
   if (algorithm.compare(suri::TransformationFactory::kExact) != 0) {
      Subset insubset;
      pInputElement_->GetElementExtent(insubset);

      std::vector<Coordinates> coords = Subset::ExtractCoordinates(insubset, 300, 300);

      suri::ParameterCollection params;
      params.AddValue<std::string>(suri::TransformationFactory::kParamWktIn,
                                   pInputElement_->GetSpatialReference().c_str());
      params.AddValue<std::string>(suri::TransformationFactory::kParamWktOut,
                                   outputwkt);

      suri::TransformationFactory* pfactory =
            suri::TransformationFactoryBuilder::Build();

      suri::CoordinatesTransformation* pexact = pfactory->Create(
            suri::TransformationFactory::kExact, params);

      std::vector<GroundControlPoint> surigcps;

      RasterElement* pauxelem = dynamic_cast<RasterElement*>(pInputElement_);
      RasterSpatialModel* psourcerastermodel = RasterSpatialModel::Create(
            pauxelem->GetRasterModel());

      Count = coords.size();
      pGdalGcps = new GDAL_GCP[Count];
      for (int ix = 0; ix < Count; ++ix) {
         Coordinates src = coords[ix];
         Coordinates dest = coords[ix];

         if (psourcerastermodel) psourcerastermodel->InverseTransform(dest);

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

      delete pexact;
      pexact = NULL;
   }

   // *************************** //
   // No se encontro la magia negra que hace funcionar a la clase Progress asi
   // que se usa directamente el wxProgressDialog
   std::auto_ptr<wxProgressDialog> pconfprog(new wxProgressDialog("Configurando reproyeccion...", "", 10));
   pconfprog.get()->SetSize(wxSize(400, -1));
   pconfprog.get()->Centre(wxCENTRE_ON_SCREEN);

   bool transok = Translate(srcfilepath, tempfilepath, destfileformat, pGdalGcps, Count,
                            bands, pixels, lines, geobounds, pconfprog.get());

   wxString temp = tempfilepath;
   temp.Replace(".tmp", ".tif");
   wxRenameFile(tempfilepath, temp.c_str());

   std::auto_ptr<wxProgressDialog> reproprog(new wxProgressDialog("Reproyectando...", "", 10));
   reproprog.get()->SetSize(wxSize(400, -1));
   reproprog.get()->Centre(wxCENTRE_ON_SCREEN);
   bool warpok = Warp(temp.c_str(), destfilepath, destfileformat, outputwkt,
                      ((Count > 0) ? true : false), ((Count > 0) ? order : 0),
                      pixelwidth, pixelheight, reproprog.get());

   if (transok && warpok) ConfigureOutput();

   return transok && warpok;
}
}