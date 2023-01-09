/*! \file suri.cpp */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#include "suri.h"

// Includes suri
#include "suri/Image.h"
#include "suri/Vector.h"
#include "suri/ProgressManager.h"

/** si no es DLL, necesito la llamada para que el linkeador agregue el codigo */
#ifndef __IS_DLL__

#  include "MaxLikelihoodAlgorithm.h"
#  include "KMeansAlgorithm.h"

#  include "LinearEnhancement.h"
#  include "Linear2PercentEnhancement.h"
#  include "GaussianEnhancement.h"
#  include "EqualizationEnhancement.h"
#  include "Linear255Enhancement.h"

#  include "MaxLikelihoodPart.h"

#  include "RasterRenderer.h"
#  include "WxsRenderer.h"
#  include "LutRenderer.h"
#  include "ConvolutionFilterRenderer.h"
#  include "BandMathRenderer.h"
#  include "FiltredVectorRenderer.h"
#  include "VectorRenderer.h"
#  include "ZoomRenderer.h"
#  include "BrightnessRenderer.h"
#  include "DataCastRenderer.h"
#  include "ColorTableRenderer.h"
#  include "ClassificationRenderer.h"
#  include "CacheRenderer.h"
#	include "suri/RasterMultiplexorRenderer.h"
#  include "suri/XmlUrlManager.h"

#endif   // __IS_DLL__

// Defines
/** version de la biblioteca */
#define SURI_LIB_BUILD __DATE__ " " __TIME__
/** Formatos minimos que se van a cargar, */
#define MINIMUM_SURI_SUPPORTED_FORMAT "GTiff"

/** variable para controlar las excepciones falsas de las macros THROW / CATCH */
bool fakeExceptionThrown = false;

/** Funcion de inicializacion de recursos XRC de wx que se usan desde la biblioteca */
#ifdef SURILIB_XRC_INITIALIZATION_FUNCTION
SURILIB_XRC_INITIALIZATION_FUNCTION_FORWARD;
#endif

/** namespace suri */
namespace suri {
/** Variables de configuracion de la biblioteca Suri */
ParametersType LibSuriConfig;

namespace {
class Dummy {
public:
   Dummy() {
      LibSuriConfig.variables_["GDAL_DATA"] = "";
      LibSuriConfig.variables_["GEOTIFF_CSV"] = "";
      LibSuriConfig.variables_["PROJ_LIB"] = "";
      LibSuriConfig.variables_["GDAL_SKIP"] = "";
      LibSuriConfig.variables_["SURI_GDAL_INCLUDE"] = MINIMUM_SURI_SUPPORTED_FORMAT;
   }
};
Dummy dummy;
}

/**  Funcion de inicializacion general para la biblioteca
 * @param[in] SuriDataDir directorio de datos de la app.
 */
void Initialize(const std::string &SuriDataDir) {
   LibSuriConfig.libBuild_ = SURI_LIB_BUILD;
   // hardcodeo directorios
   LibSuriConfig.dataDir_ = SuriDataDir;
   Image::Init();
   Vector::Init();
   SURILIB_XRC_INITIALIZATION_FUNCTION;
   ProgressManager::SetCreateFunction(suri::WxProgressManager);
}

/** si no es DLL, necesito la llamada para que el linkeador agregue el codigo */
#ifndef __IS_DLL__

/** si no es DLL, necesito la llamada para que el linkeador agregue el codigo */
void InitializeAllAlgorithms() {
   suri::MaxLikelihoodAlgorithm mxlikelihood;
   suri::KMeansAlgorithm kmeans;
}

/**
 * Metodo que registra las clases de realce
 * Esta funcion surge por debido a que
 * en el linkeo del .a se eliminan los renderizadores con registracion
 * automatica. En caso de tratarse de un .so/.dll este metodo es irrelevante.
 */
void InitializeAllEnhancements() {
   LinearEnhancement linenh;
   Linear255Enhancement lin255enh;
   Linear2PercentEnhancement lin2penh;
   GaussianEnhancement gausenh;
   EqualizationEnhancement eqenh;
}

/** Metodo para que el linkeador agregue el codigo */
void InitializeAllParts() {
   suri::MaxLikelihoodPart mxlikelihood;
}
/** todo: documentar */
void InitializeAllManagers() {
   XmlUrlManager xmlurl;
}

/** Funcion de registracion de Renderers. */
/**
 *  Esta funcion registra todos los Renderizadores conocidos.
 *  Es un poco desproposito porque se incluyen los .h de los renderizadores y
 * teoricamente esto no deberia hacerse. Esta funcion surge por debido a que
 * en el linkeo del .a se eliminan los renderizadores con registracion
 * automatica. En caso de tratarse de un .so/.dll este metodo es irrelevante.
 */
void InitializeAllRenderers() {
   suri::Renderer::Register(new suri::RasterRenderer);
   suri::Renderer::Register(new suri::WxsRenderer);
   suri::Renderer::Register(new suri::BrightnessRenderer);
   suri::Renderer::Register(new suri::LutRenderer);
   suri::Renderer::Register(new suri::ConvolutionFilterRenderer);
   suri::Renderer::Register(new suri::BandMathRenderer);
   suri::Renderer::Register(new suri::FiltredVectorRenderer);
   suri::Renderer::Register(new suri::VectorRenderer);
   suri::Renderer::Register(new suri::ZoomRenderer);
   suri::Renderer::Register(new suri::DataCastRenderer);
   suri::Renderer::Register(new suri::ColorTableRenderer);
   suri::Renderer::Register(new suri::ClassificationRenderer);
   suri::Renderer::Register(new suri::CacheRenderer);
}

#endif   // __IS_DLL__
}  // namespace suri
