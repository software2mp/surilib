/*! \file CoordinatesNavigationInputInterface.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2013-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef COORDINATESNAVIGATIONINPUTINTERFACE_H_
#define COORDINATESNAVIGATIONINPUTINTERFACE_H_

#include "suri/Coordinates.h"
#include "LibraryItemEditorPart.h"

namespace suri {

class DataViewManager;

/**  Interface que representa el ingreso de coordenadas para navegacion.**/
class CoordinatesNavigationInputInterface {
   /** ctor copia **/
   CoordinatesNavigationInputInterface(const CoordinatesNavigationInputInterface&);

public:
   /** Ctor */
   CoordinatesNavigationInputInterface() : isProj_(false) { }

   /** Dtor */
   virtual ~CoordinatesNavigationInputInterface() { }

   /** retorna la coordenada que ingresaron, informa si es valida */
   virtual bool SetCoordinate(Coordinates Coordinate)=0;

   /** retorna la coordenada que ingresaron, informa si es valida */
   virtual bool GetCoordinate(Coordinates &Coordinate)=0;

   /** Obtiene el SRS */
   std::string GetSpatialReference() { return spatialReference_; }

   /** Establece el SRS */
   void SetSpatialReference(const std::string& SpatialReference) {
      spatialReference_ = SpatialReference;
      OnSpatialReferenceSet();
   }

   /** Habilita/deshabilita la navegacion pixel/linea **/
   virtual void EnablePixelLineNavigation(bool Enable = true) { }

   /** Obtiene el widget asociado */
   virtual Widget* GetInputWidget() { return NULL; }

   virtual void SetDataViewManager(DataViewManager* pDataViewManager) {
      pDataViewManager_ = pDataViewManager;
   }

   /**
    * Establece si las coordenadas son proyectadas.
    */
   void SetProj(bool Proj) { isProj_ = Proj; }

   /**
    * Indica si las coordenadas son proyectadas.
    */
   bool IsProj() { return isProj_; }

protected:
   friend class BasicLibraryItemSelector;

   /**
    * Metodo auxiliar que actualiza el panel de ingreso de coordenadas
    * en funcion del sistema de referencia.
    */
   virtual void UpdateCoordsInput() { }

   class BasicLibraryItemSelector : public LibraryItemEditorPart {
   public:
      /** ctor **/
      BasicLibraryItemSelector(
            CoordinatesNavigationInputInterface* pCoordsPart,
            const SuriObject::UuidType& LibraryId,
            LibraryManager* pManager,
            const std::string& WxWindowdId,
            const std::string& WxCguPanelId,
            bool FilterFavorites = false,
            const UniversalGraphicalComponentPartInterface::FeatureCombinationEnum& Feature =
                  UniversalGraphicalComponentPartInterface::EditionWithFavorites,
            LibraryManager::AccessType Access = LibraryManager::ALL) :
            LibraryItemEditorPart(LibraryId, pManager, WxWindowdId, WxCguPanelId,
                                  FilterFavorites, Feature, Access),
            pCoordsPart_(pCoordsPart) {
      }

   private:
      CoordinatesNavigationInputInterface* pCoordsPart_;
      virtual void UpdateFields(const LibraryItem* pItem, bool Modifiable = true) {
         if (pItem) {
            std::string spatial = pItem->GetAttribute("WKT")->GetValue();
            pCoordsPart_->spatialReference_ = spatial;
            pCoordsPart_->UpdateCoordsInput();
         }
      }
   };

   /** Metodo que se llama cuando se configura el sistema de referencia. Permite
    *  realizar alguna accion luego de configurar el sistema de referencia
    *  de trabajo
    */
   virtual void OnSpatialReferenceSet() { }

   std::string spatialReference_;
   DataViewManager* pDataViewManager_;
   bool isProj_;
};
}

#endif /* COORDINATESNAVIGATIONINPUTINTERFACE_H_ */
