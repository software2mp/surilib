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

#ifndef MASKSELECTORPART_H_
#define MASKSELECTORPART_H_

// Includes standard
#include <set>

// Includes Suri
#include "suri/PartCollection.h"
#include "suri/UseFeatureSelection.h"
#include "suri/LayerList.h"
#include "suri/HtmlTreeWidget.h"
#include "suri/DataViewManager.h"
#include "suri/ProcessAtributeProvider.h"

// Includes Wx

// Includes App

// Defines

namespace suri {

// Forwards
class LayerTreeModel;
class LayerHtmlTreeNodeHandler;
DECLARE_EVENT_OBJECT(MaskPartEventHandler);

namespace ui {
   class HtmlListSelectionWidget;
}

/** Permite seleccionar la mascara que se va a aplicar sobre la entrada */
/**
 * Permite seleccionar un poligono y si se va a usar como mascara el interior
 * o el exterior de este.
 * Ademas, permite enmascarar los valores no validos de la imagen de
 * entrada.
 */
class MaskSelectionPart :public UseFeatureSelection, public Part, public ProcessAtributeProvider {
   /** Constructor copia **/
   MaskSelectionPart(const MaskSelectionPart&);
public:
   /** Constructor */
   MaskSelectionPart(DataViewManager* pDataViewManager, Element* pInputElement);
   /** Destructor */
   virtual ~MaskSelectionPart();
   /** Indica si el Part tiene cambios para salvar. */
   virtual bool HasChanged();
   /** Salva los cambios realizados en el Part. */
   virtual bool CommitChanges();
   /** Restaura los valores originales del Part. */
   virtual bool RollbackChanges();
   /** Inicializa el part */
   virtual void SetInitialValues();
   /** Devuelve si la parte tiene datos validos */
   virtual bool HasValidData();
   /** Actualiza el estado de la parte */
   virtual void Update();
   /** Implementacion del metodo de creacion de ventana de Widget */
   virtual bool CreateToolWindow();
   /** Retorna el icono de la herramienta */
   virtual void GetWindowIcon(wxBitmap& ToolBitmap, int X = 0, int Y = 0) const;
   /** Obtiene la lista de mascara resultante de la seleccion en el Part luego de
    *  guardar los cambios*/
   virtual LayerList* GetLastMaskList();



private:
   FRIEND_EVENT_OBJECT(MaskPartEventHandler);
   EVENT_OBJECT_PROPERTY(MaskPartEventHandler);
   /** Metodo que genera un mascara a partir de la fuente de datos que se pasa por parametro
    *  Precondicion: pDatasource tiene que ser no nulo**/
   VectorElement* GenerateMaskFromDatasource(DatasourceInterface* pDatasource);
   /** Metodo que se llama cuando se hace click sobre el radio para mascara interna*/
   void OnInternalRadioClick(wxCommandEvent& Event);
   /** Metodo que se llama cuando se hace click sobre el radio para mascara externa*/
   void OnExternalRadioClick(wxCommandEvent& Event);
   /** Metodo que se llama cuando se hace click sobre el check*/
   void OnChoiceEvent(wxCommandEvent& Event);
   /** Metodo auxiliar que configura los controles graficos del Part*/
   void ConfigureGui();
   /** Lista de elementos que se encuentra al momento de creacion del Part**/
   LayerList* pLayerList_;
   ui::HtmlListSelectionWidget* pTreeWidget_;
   /** Admistrador general de la aplicacion **/
   DataViewManager* pDataViewManager_;
   /** Elemento de entrada al proceso **/
   Element* pInputElement_;
   /** bool que indica si el estilo actual de la mascara es interna o no*/
   bool guiInternalMask_;
   /** bool que indica si el estilo guardado de la mascara es interno o no*/
   bool internalMask_;
   bool noDataValueDefined_;
   /** string que contiene el valor no valido definido **/
   std::string noDataValue_;
   /** Vector con los items checkeados sin cambios **/
   std::set<SuriObject::UuidType> checkedSet_;
   /** valor configurado en el part para enmascarasr **/
   int maskvalue_;
   VectorEditor* pMemoryVectorEditor_; /** Vector en memoria **/
};
}

#endif /* MASKSELECTORPART_H_ */
