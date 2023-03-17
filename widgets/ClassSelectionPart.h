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

#ifndef CLASSSELECTIONPART_H_
#define CLASSSELECTIONPART_H_

// Includes standard
#include <set>

// Includes Suri
#include "suri/Part.h"
#include "suri/Element.h"
#include "suri/SuriObject.h"
#include "suri/VectorElement.h"
#include "suri/ClassStatisticsFileParser.h"

// Includes Wx
#include "wx/wx.h"
// Includes App
#include "suri/ProcessAtributeProvider.h"

// Defines

/** namespace suri */
namespace suri {
class LayerHtmlTreeNodeHandler;
class HtmlTreeWidget;
class LayerTreeModel;
class DataViewManager;

namespace ui {
class HtmlListSelectionWidget;
class HtmlListItemInterface;
}

class ClassificationClass;
class ClassSelectionPartEventHandler;

/** Muestra poligonos en layerlist y permite editarlos */
/**
 * Muestra los elementos de tipo poligono en la LayerList y al hacer commit
 * guarda en lista interna los elementos activos.
 */
class ClassSelectionPart : public Part, public ProcessAtributeProvider {
public:
   static const int SelectionButtonFlag = 0x01;
   static const int ClearSelectionButtunFlag = 0x02;
   static const int ImportClassSelectionButtonFlag = 0x04;
   static const int TrainClassFilter = 0x08;
   static const int TestClassFilter = 0x10;
   /** Ctor */
   explicit ClassSelectionPart(DataViewManager* pDataViewManager,
               int LayoutFlags = SelectionButtonFlag | ClearSelectionButtunFlag | TrainClassFilter);
   /** Dtor */
   virtual ~ClassSelectionPart();

   /** Crea la ventana de la parte */
   virtual bool CreateToolWindow();
   /** Devuelve el estado del Part. */
   virtual bool IsEnabled();
   /** Indica si el Part tiene cambios para salvar. */
   virtual bool HasChanged();
   /** Salva los cambios realizados en el Part. */
   virtual bool CommitChanges();
   /** Restaura los valores originales del Part. */
   virtual bool RollbackChanges();
   /** Inicializa los datos mostrados */
   virtual void SetInitialValues();
   /** Devuelve el icono de la parte */
   virtual void GetWindowIcon(wxBitmap& ToolBitmap, int X, int Y) const;
   /** Informa si el subset seleccionado es valido */
   virtual bool HasValidData();
   /** Obtiene las clases seleccionadas **/
   std::list<ClassificationClass*> GetSelectedClasses();

   /** Actualiza el estado de la parte */
   virtual void Update() {
   }
   /** Agrega un item al final de la lista */
   void AppendItem(ui::HtmlListItemInterface* pitem);

protected:
   void OnSelectAllClick(wxCommandEvent& Event);
   void OnClearSelectionClick(wxCommandEvent& Event); 
   /** Captura el evento de click sobre el boton importar clase */
   void OnImportButton(wxCommandEvent &Event);
   /** Carga los elementos en el control */
   virtual std::list<Element*> GetActiveElements();
   /** Metodo que genera un mascara a partir de la fuente de datos que se pasa por parametro
    *  Precondicion: pDatasource tiene que ser no nulo**/
   VectorElement* GenerateMaskFromDatasource(DatasourceInterface* pDatasource);
   /** Metodo que inicializa la lista html que contiene las clases **/
   void InitializeTreeWidget();
   ui::HtmlListSelectionWidget* pTreeWidget_;
   /** Vector con enclosures que se usan para clasificar */
   std::set<SuriObject::UuidType> activeLeafs_;
   DataViewManager* pDataViewManager_; /*! Datos del proyecto */
   friend class ClassSelectionPartEventHandler;
   ClassSelectionPartEventHandler* pEventHandler_;
   int layoutflags_;
};

/**
 * Clase abstracta que representa una clase a utilizar en la clasificacion.
 * Esta clase podra ser interpretada como una capa geometria (que viene dada
 * por la clase Element) o por un conjunto de estadisticas
 *
 */
class ClassificationClass {
public:
   virtual ~ClassificationClass() {}
   virtual Element* GetClassAsElement()=0;
   virtual ClassStatisticsFileParser::ClassStatisticsInfo* GetClass()=0;
};

class ClassificationElementClass : public ClassificationClass {
public:
   ClassificationElementClass(Element* pElement) :
         pElement_(pElement) {
   }
   virtual ~ClassificationElementClass() {
   }
   ClassStatisticsFileParser::ClassStatisticsInfo* GetClass() {
      return NULL;
   }
   virtual Element* GetClassAsElement() {
      return pElement_;
   }

private:
   Element* pElement_;
};

class ClassificationStatisticsClass : public ClassificationClass {
public:
   ClassificationStatisticsClass(ClassStatisticsFileParser::ClassStatisticsInfo* pClass) :
         pClass_(pClass) {
   }
   virtual ~ClassificationStatisticsClass() {
   }
   ClassStatisticsFileParser::ClassStatisticsInfo* GetClass() {
      return pClass_;
   }
   virtual Element* GetClassAsElement() {
      return NULL;
   }

private:
   ClassStatisticsFileParser::ClassStatisticsInfo* pClass_;

};
}  // namespace suri

#endif /* CLASSSELECTIONPART_H_ */
