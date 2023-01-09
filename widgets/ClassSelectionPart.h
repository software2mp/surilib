/*! \file ClassSelectionPart.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2014-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

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
