/** \file LibraryManagerFactory.cpp */
/*!
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2013-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

// Includes standard
#include <vector>
#include <string>

// Includes suri
#include "suri/LibraryManagerFactory.h"
#include "suri/LibraryClient.h"
#include "suri/Configuration.h"
#include "GdalSrsItemOrigin.h"
#include "suri/TextFileLibraryItemOrigin.h"
#include "suri/DensitySliceLibraryOrigin.h"
#include "suri/DataTypes.h"
#include "LibraryConfig.h"
#include "LibraryConfigDefinition.h"
#include "LibraryItemOriginFactory.h"

// Includes wx
#include "wx/wx.h"

// Defines
#define SYSSOURCE_FILEFORMAT_s "%s_sys.lbr"
#define USERSOURCE_FILEFORMAT_s "%s_user.lbr"

namespace suri {

const char* librarynames[] = { "srs", "datum", "ellipsoid", "lut", "vectorstyles",
                                 "wxsservice", "algorithm", "densityslice", "wms",
                                 "spectralsign", "convolutionfilters", "equation", "index" };

/** Definicion de codigo de bibliotecas existentes **/
std::string LibraryManagerFactory::SpatialReferenceLibraryCode = "srs";
std::string LibraryManagerFactory::DatumLibraryCode = "datum";
std::string LibraryManagerFactory::EllipsoidLibraryCode = "ellipsoid";
std::string LibraryManagerFactory::DensitySliceCode = "densityslice";
std::string LibraryManagerFactory::WmsLibraryCode = "wms";
std::string LibraryManagerFactory::VectorstyleLibraryCode = "vectorstyles";
std::string LibraryManagerFactory::SpectralSignLibraryCode = "spectralsign";
std::string LibraryManagerFactory::ConvolutionFiltersCode = "convolutionfilters";
std::string LibraryManagerFactory::EquationLibraryCode = "equation";
std::string LibraryManagerFactory::IndexLibraryCode = "index";
/**
 * Inicializa LibraryManager
 * @return LibraryManager configurado
 */
LibraryManager* LibraryManagerFactory::CreateLibraryManager() {
   LibraryManager* pmanager = new LibraryManager();
   LibraryClient libraryclient(pmanager, LibraryManager::ALL);
   for (unsigned int i = 0; i < ARRAY_SIZE(librarynames) ; i++)
      libraryclient.AddLibrary(new Library(librarynames[i]));

   ResetLibraryManager(pmanager);
   return pmanager;
}

/**
 * Recarga todos los origenes en LibraryManager sin los cambios realizados
 * por el usuario
 * @param[in] pManager manager que quiero resetear.
 */
void LibraryManagerFactory::ResetLibraryManager(LibraryManager* pManager) {
   LibraryManager::AccessKey fullaccesskey(true, true, true);
   // Reseto las biblioteas
   // Agrego origenes de usuario y proyecto a todas las bibliotecas
#ifdef __LIB_FULL_LOAD__
   for (int i = 0; i < 7; i++) {
      std::string libname(librarynames[i]);
      Library* plibrary = pManager->GetLibraryByCode(libname, fullaccesskey);
      plibrary->Reset();
      std::string filename = wxString::Format(SYSSOURCE_FILEFORMAT_s, libname.c_str()).c_str();
      AddTextFileOriginToLibrary(plibrary, filename, LibraryItemOrigin::SYSTEM, libname);
      filename = wxString::Format(USERSOURCE_FILEFORMAT_s, libname.c_str()).c_str();
      AddTextFileOriginToLibrary(plibrary, filename, LibraryItemOrigin::USER, libname);
      AddMemoryOriginToLibrary(plibrary, LibraryItemOrigin::PROJECT, libname);
   }
#endif
   // Agrego origen con srs soportados por gdal
   Library* plibrary = pManager->GetLibraryByCode(SpatialReferenceLibraryCode, fullaccesskey);
   LoadOrigins(plibrary, SpatialReferenceLibraryCode);

   // Agrego origen con datums soportados por gdal
   plibrary = pManager->GetLibraryByCode(DatumLibraryCode, fullaccesskey);
   LoadOrigins(plibrary, DatumLibraryCode);

   // Agrego origen con ellipsoides soportados por gdal
   plibrary = pManager->GetLibraryByCode(EllipsoidLibraryCode, fullaccesskey);
   LoadOrigins(plibrary, EllipsoidLibraryCode);

   // Agrego origen con tablas de intervalos
   plibrary = pManager->GetLibraryByCode(DensitySliceCode, fullaccesskey);
   LoadOrigins(plibrary, DensitySliceCode);

   // Agrego origen con servidores wms
   plibrary = pManager->GetLibraryByCode(WmsLibraryCode, fullaccesskey);
   LoadOrigins(plibrary, WmsLibraryCode);

   // Agrego origen con servidores wms
   plibrary = pManager->GetLibraryByCode(VectorstyleLibraryCode, fullaccesskey);
   LoadOrigins(plibrary, VectorstyleLibraryCode);

   // Agrego origen con firmas espectrales
   plibrary = pManager->GetLibraryByCode(SpectralSignLibraryCode, fullaccesskey);
   LoadOrigins(plibrary, SpectralSignLibraryCode);

   // Agrego origen con filtros de convolución
   plibrary = pManager->GetLibraryByCode(ConvolutionFiltersCode, fullaccesskey);
   LoadOrigins(plibrary, ConvolutionFiltersCode);

   // Agrego origen con ecuaciones
   plibrary = pManager->GetLibraryByCode(EquationLibraryCode, fullaccesskey);
   LoadOrigins(plibrary, EquationLibraryCode);

   // Agrego origen con indices
   plibrary = pManager->GetLibraryByCode(IndexLibraryCode, fullaccesskey);
   LoadOrigins(plibrary, IndexLibraryCode);

}

/**
 * Agrega un origen de archivo a biblioteca indicada
 * @param[in] pLibrary Biblioteca donde quiero agregar el origen
 * @param[in] FileName path al archivo donde se persiste la biblioteca
 * @param[in] Category categoria del origen a agregar
 */
void LibraryManagerFactory::AddTextFileOriginToLibrary(Library* pLibrary,
                              const std::string &FileName,
                              LibraryItemOrigin::OriginCategoryType Category,
                              const std::string& OriginName) {
   std::vector<LibraryItemOrigin::CapabilityType> capabilites;
   capabilites.push_back(LibraryItemOrigin::READ);
   if (Category != LibraryItemOrigin::SYSTEM) {
      capabilites.push_back(LibraryItemOrigin::WRITE);
      capabilites.push_back(LibraryItemOrigin::APPEND);
   }

   std::string path = Configuration::GetParameter("app_library_dir", wxT("./"));
   path.append(FileName);

   LibraryItemOrigin* porigin = new TextFileLibraryItemOrigin(pLibrary->GetId(),
                                                              capabilites, path,
                                                              Category, OriginName);
   pLibrary->AddOrigin(porigin);
}

/**
 * Agrega un origen en memoria a biblioteca indicada
 * @param[in] pLibrary Biblioteca donde quiero agregar el origen
 * @param[in] Category categoria del origen a agregar
 * @param[in] OriginName nombre del origen
 */
void LibraryManagerFactory::AddMemoryOriginToLibrary(Library* pLibrary,
                                       LibraryItemOrigin::OriginCategoryType Category,
                                       const std::string& OriginName) {
   std::vector<LibraryItemOrigin::CapabilityType> capabilites;
   capabilites.push_back(LibraryItemOrigin::READ);
   if (Category != LibraryItemOrigin::SYSTEM) {
      capabilites.push_back(LibraryItemOrigin::WRITE);
      capabilites.push_back(LibraryItemOrigin::APPEND);
   }

   LibraryItemOrigin* porigin = new LibraryItemOrigin(pLibrary->GetId(), Category,
                                                      capabilites, OriginName);
   pLibrary->AddOrigin(porigin);
}

/**
 * Carga los distintos origenes disponibles a la libreria.
 */
void LibraryManagerFactory::LoadOrigins(Library* pLibrary, const std::string& LibCode) {
   LibraryConfigDefinition* pdef = LibraryConfig::GetInstance()->GetDefinitionByName(LibCode);
   if (pdef != NULL) {
      std::vector<LibraryConfigDefinitionOrigin*> origins = pdef->GetOrigins();
      for (int ix = 0, lenix = origins.size(); ix < lenix; ++ix) {
         LibraryItemOrigin* porigin = LibraryItemOriginFactory::Create(
               pLibrary, origins[ix], pdef->GetFavoritesIndexFileName());
         pLibrary->AddOrigin(porigin);
      }
   }
}

}  // namespace suri
