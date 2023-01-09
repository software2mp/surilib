/** \file ExactTransformationFactory.cpp
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#include "suri/ExactTransformationFactory.h"
#include "suri/ExactCoordinatesTransformation.h"

namespace suri {

/**
 * Construye una nueva instancia especificando al sucesor.
 *
 * @param[in] pSuccessor Siguiente en la cadena.
 */
ExactTransformationFactory::ExactTransformationFactory(
      TransformationFactory* pSuccessor) :
      TransformationFactory(pSuccessor) {
}

/**
 * Destructor (elimina al sucesor)
 */
ExactTransformationFactory::~ExactTransformationFactory() {
   TransformationFactory* psuccessor = GetSuccessor();
   if (psuccessor != NULL) {
      delete psuccessor;
      psuccessor = NULL;
   }
}

/**
 * Implementa el metodo de la clase base para la creacion de una instancia de
 * ExactCoordinatesTransformation.
 *
 * @param[in] Type Tipo de transformacion que se desea crear.
 * @param[in] Params Parametros necesarios para la creacion de cada transformacion.
 * @return Devuelve una instancia a una transformacion de coordenadas.
 *
 */
CoordinatesTransformation* ExactTransformationFactory::Create(
      const std::string& Type, const suri::ParameterCollection& ParamsP) {

   if (Type.compare(suri::TransformationFactory::kExact) == 0) {
      suri::ParameterCollection& params = const_cast<suri::ParameterCollection&>(ParamsP);

      std::string srwktin;
      params.GetValue<std::string>(suri::TransformationFactory::kParamWktIn, srwktin);

      std::string srwktout;
      params.GetValue<std::string>(suri::TransformationFactory::kParamWktOut, srwktout);

      if (srwktout.empty()) {
         srwktout = GetWktOut(srwktin);
      }

      // TODO(Gabriel - TCK #2344): Sacar cuando SR Raster tenga un Wkt Valido
      if (SpatialReference::IsPixelLineSpatialRef(srwktin)
            && SpatialReference::IsPixelLineSpatialRef(srwktout)) {
         return new ExactCoordinatesTransformation(srwktin, srwktout);
      }

      // Verifica los WKTs de entrada
      if (!Wkt::IsValid(srwktin) || !Wkt::IsValid(srwktout)) {
         return NULL;
      }

      // Verifica si los WKTs son los mismos y en caso positivo devuelve la identidad
      if (srwktin.compare(srwktout) == 0) {
         return new ExactCoordinatesTransformation(srwktin, srwktout);
      }

      // Crea las referencias espaciales OGR
      OGRSpatialReference srin(srwktin.c_str());
      OGRSpatialReference srout(srwktout.c_str());
      if (srin.Validate() == OGRERR_CORRUPT_DATA
            || srout.Validate() == OGRERR_CORRUPT_DATA) {
         return NULL;
      }

      // Si OGR informa que los sistemas de referencia son iguales retorna
      // identidad.
      if (srin.IsSame(&srout)) {
         return new ExactCoordinatesTransformation(srwktin, srwktout);
      }

      // Crea la transformacion directa
      OGRCoordinateTransformation* ptransform =
            OGRCreateCoordinateTransformation(&srin, &srout);
      if (ptransform == NULL)
         return NULL;

      // Crea la transformacion inversa
      OGRCoordinateTransformation* pinversetransform =
            OGRCreateCoordinateTransformation(&srout, &srin);
      if (pinversetransform == NULL) {
         OCTDestroyCoordinateTransformation(ptransform);
         return NULL;
      }

      // Crea el objeto ExactCoordinatesTransformation con las correspondientes
      // transformaciones.
      ExactCoordinatesTransformation* pcoordtransform =
            new ExactCoordinatesTransformation(srwktin, srwktout, false, ptransform,
                                     pinversetransform);

      return pcoordtransform;
   } else {
      TransformationFactory* psuccessor = GetSuccessor();
      if (psuccessor != NULL)
         return psuccessor->Create(Type, ParamsP);
   }

   return NULL;
}

/**
 * Crea el WKT de salida.
 *
 * @param[in] WktIn WKT de entrada.
 * @return WKT de salida.
 */
std::string ExactTransformationFactory::GetWktOut(std::string& WktIn) {
   // Verifica el WKT de entrada
   if (!Wkt::IsValid(WktIn)) {
      return std::string("");
   }

   // Crea el WKT de salida (geografico)
   Wkt* psrinwkt = Wkt::Create(WktIn);
   std::string geogswkt;
   if (psrinwkt->GetRootValue() != "PROJCS") {
#ifdef __DONT_ALLOW_GEOCS_IDENTITY__
      Wkt::Destroy(srinwkt);
      return NULL;
#else
      geogswkt = psrinwkt->GetWktString("GEOGCS");
#endif
   } else {
      geogswkt = psrinwkt->GetWktString("PROJCS|GEOGCS");
   }
   Wkt::Destroy(psrinwkt);
   return geogswkt;
}

} // namespace suri
