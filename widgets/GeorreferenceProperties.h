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

#ifndef GEORREFERENCEPROPERTIES_H_
#define GEORREFERENCEPROPERTIES_H_

#include "suri/Element.h"
#include "suri/PartCollection.h"
#include "suri/LibraryManager.h"
#include "suri/LibraryItemSelectionInterface.h"

#include "wx/wx.h"

namespace suri {

class DatasourceManagerInterface;
class DatasourceInterface;
class RasterElement;

DECLARE_EVENT_OBJECT(GeorreferencePropertiesEvent);

/**
 * Clase que carga en una ventana la informacion de georreferencia de un elemento,
 * solo es informativa y no permite modificar.
 * Los metodos heredados de part que sirven para aplicar cambios no tiene efecto.
 */
class GeorreferenceProperties : public Part, public LibraryItemSelectionInterface {
public:
   FRIEND_EVENT_OBJECT(GeorreferencePropertiesEvent);

   /** Ctor. */
   GeorreferenceProperties(Element* pElement, LibraryManager* pLibraryManager);

   /** Ctor. */
   GeorreferenceProperties(DatasourceManagerInterface* pDatasourceManager,
                           DatasourceInterface* pDatasource,
                           LibraryManager* pLibraryManager);

   /** Dtor. */
   virtual ~GeorreferenceProperties();

   /** Inicializa los controles */
   virtual void SetInitialValues();

   /** Devuelve el estado del Part. */
   virtual bool IsEnabled();

   /** Indica si el Part tiene cambios para salvar. */
   virtual bool HasChanged() { return srsChanged_ || modelChanged_; }

   /** Salva los cambios realizados en el Part. */
   virtual bool CommitChanges();

   /** Restaura los valores originales del Part. */
   virtual bool RollbackChanges();

   /** Actualiza el estado de la parte */
   virtual void Update() { }

   /** Implementacion del metodo de creacion de ventana de Widget */
   virtual bool CreateToolWindow();

   /** Retorna el icono de la herramienta */
   virtual void GetWindowIcon(wxBitmap& ToolBitmap, int X = 0, int Y = 0) const;

   /** Carga controles de Sistema Geografico */
   void LoadGeograficData();

   /** Carga controles de Sistema Proyectado */
   void LoadProjectionData();

   /** Carga el arbol de parametros */
   void LoadParametersTree();

   /**
    * Configura el editor con el item que se pasa por parametro. Se pasa por parametro
    * un boolean que indica si se trata de la primera seleccion (para que el part no piense
    * que posee cambios cuando en realidad es la seleccion inicial del CGU)
    */
   virtual void SetActiveItem(const LibraryItem* pItem);

   /**
    * Maneja aviso de que alguno de los textboxes del modelo
    * cambio.
    */
   void OnModelTextChangedHandler(wxCommandEvent &Event);

private:
   EVENT_OBJECT_PROPERTY(GeorreferencePropertiesEvent);

   Element* pElement_;
   DatasourceManagerInterface* pDatasourceManager_;
   DatasourceInterface* pDatasource_;
   LibraryManager* pLibraryManager_;
   bool srsChanged_;
   bool modelChanged_;
   std::string selectedSrs_;
   double selectedPixelX_;
   double selectedPixelY_;
   double selectedNorth_;
   double selectedModelP_;
   double selectedModelL_;
   double selectedModelX_;
   double selectedModelY_;
   bool hasSrs_;

   /**
    * Carga datos de modelo de imagen.
    */
   void LoadImageModel();

   /**
    * Configura el manejador para el evento de actualizacion
    * del textbox.
    */
   void SetupModelTextboxHandler(const std::string& Id);

   /**
    * Obtiene el valor del textbox.
    */
   std::string GetTextboxValue(const std::string& Id);

   /**
    * Inicializa valores internos.
    */
   void InitInternals();

   /** */
   void SaveSrsToElement(RasterElement* pRaster);

   /** */
   void SaveSrsToFile(RasterElement* pRaster);

   /** */
   void SaveModelToElement(RasterElement* pRaster);

   /** */
   void SaveModelToFile(RasterElement* pRaster);
};

}  // namespace suri

#endif  // GEORREFERENCEPROPERTIES_H_
