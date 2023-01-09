/*! \file SingleRasterElementActivationLogic.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef SINGLERASTERELEMENTACTIVATIONLOGIC_H_
#define SINGLERASTERELEMENTACTIVATIONLOGIC_H_

// Includes standard

// Includes Suri
#include "suri/ActivationLogic.h"
#include "suri/ViewerWidget.h"

// Includes Wx

// Includes App

// Defines

/** Clase de seleccion de elementos del contexto. */
/**
 * Esta clase proporciona la logica para activar/desactivar elementos del
 * contexto Raster.
 * Verifica que no se activen vectores si no hay un raster activo y tambien
 * verifica que no se activen 2 imagenes al mismo tiempo.
 */
class SingleRasterElementActivationLogic : public suri::ActivationLogic {
public:
   /** Constructor */
   SingleRasterElementActivationLogic(suri::ViewerWidget *pViewer,
                                      const std::string &RequiredElement) :
         pViewer_(pViewer), requiredElement_(RequiredElement) {
   }
   /** Destructor */
   virtual ~SingleRasterElementActivationLogic() {
   }
   /** Activado/Desactiva elementos del contexto */
   virtual void SetActivation(suri::Element *pElement,
                              suri::LayerList::ElementListType &ElementList,
                              suri::LayerList::ElementSelectionType &ElementData, bool Status);

private:
   /** Posicion de los tipos de elemento en la lista */
   typedef enum {
      DefaultPosition = 0, AnotationPosition = 2, VectorPosition = 4, RasterPosition = 6
   } ListPositionType;
   suri::LayerList::ElementListType elementList_; /*! Lista elem activos al aplicar la logica */
   suri::ViewerWidget* pViewer_; /*! View asociado */
   std::string requiredElement_; /*! Texto para el mensaje de elemento requerido */
   /** Constructor de Copia */
   SingleRasterElementActivationLogic(
         const SingleRasterElementActivationLogic &SingleRasterElementActivationLogic);
};

#endif /*SINGLERASTERELEMENTACTIVATIONLOGIC_H_*/
