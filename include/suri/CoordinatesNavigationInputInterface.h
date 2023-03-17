/* Copyright (c) 2006-2023 SpaceSUR and CONAE

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU Affero General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU Affero General Public License for more details.

You should have received a copy of the GNU Affero General Public License
along with this program.  If not, see <https://www.gnu.org/licenses/>.

This file is part of the SuriLib project and its derived programs. You must retain
this copyright notice and license text in all copies or substantial
portions of the software.

If you develop a program based on this software, you must provide a visible
notice to its users that it contains code from the SuriLib project and provide
a copy of this license. The notice should be displayed in a way that is easily
accessible to users, such as in the program's "About" box or documentation.

For more information about SpaceSUR, visit <https://www.spacesur.com>.
For more information about CONAE, visit <http://www.conae.gov.ar/>. */

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
