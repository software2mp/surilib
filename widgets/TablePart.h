/*! \file TablePart.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2012-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef TABLEPART_H_
#define TABLEPART_H_

// Includes Estandar
#include <list>
#include <string>
#include <set>

// Includes Suri
#include "TableTool.h"
#include "suri/Table.h"
#include "wxGenericTableBase.h"
#include "suri/Part.h"
#include "FeatureSelectionObserver.h"
#include "ModelOrderWidget.h"
#include "ModelOrderWidget.h"
#include "suri/Subject.h"
#include "CellTableHandler.h"
#include "suri/SearchFilterApplierInterface.h"
#include "suri/Option.h"

// Includes Wx

// Defines

// forwards
class wxGridEvent;
class wxGridRangeSelectEvent;

namespace suri {

DECLARE_EVENT_OBJECT(TablePartEventHandler);

// forwards
class FeatureSelection;
class ModelOrderWidget;
class GeoreferenceRefSysPart;
class SearchWidget;

/**
 * Part que administra un Table y una ToolBar de Command asociados
 */
class TablePart : public suri::Part,
                  public suri::FeatureSelectionObserver,
                  public Subject,
                  public SearchFilterApplierInterface {
   /** ctor copia **/
   TablePart(const TablePart&);

public:
   /** Constructor */
   explicit TablePart(Table* pTable, bool IncludeOrderSelection = false,
                      bool IncludeRefSystem = false, suri::DataViewManager* pDv = NULL,
                      bool EnableSearch = false);
   /** Destructor */
   virtual ~TablePart();
   /** Agrega un comando */
   bool AddCommand(const std::string &CommandName);
   /** Quita un comando */
   bool RemoveCommand(const std::string &CommandName);
   /** Indica si el Part tiene cambios para salvar */
   virtual bool HasChanged();
   /** Salva los cambios realizados en el Part */
   virtual bool CommitChanges();
   /** Restaura los valores originales del Part */
   virtual bool RollbackChanges();
   /** Crea el Tool Window */
   bool CreateToolWindow();
   /** Inicializa el part */
   virtual void SetInitialValues();
   /** Actualiza el estado de la parte */
   virtual void Update();
   /** Getter de Table */
   virtual Table* GetTable();
   /** Para no autodestruirse al eliminar un editor */
   void OnDestroy(wxWindowDestroyEvent &Event);
   /** Cambia el titulo de la ventana */
   void ChangeWindowTitle(const std::string& Title);
   /** Cambia el icono de la ventana */
   void SetWindowIcon(const std::string& BitmapUrl);
   /** Obtiene el icono de la ventana */
   virtual void GetWindowIcon(wxBitmap& ToolBitmap, int X, int Y) const;
   /** Setea el Table Tool */
   void SetTableTool(TableTool* pTableTool);
   /** Getea el Table Tool */
   TableTool* GetTableTool();
   /** Obtiene las columnas seleccionadas. */
   wxArrayInt GetSelectedRows();
   /** Obtiene las filas seleccionadas*/
   wxArrayInt GetSelectedColumns();
   /** Metodo que es llamado cuando se actualiza la seleccion. */
   virtual void SelectionUpdated();
   /** Configura la instancia encargada de notificar la seleccion**/
   void SetSelectionNotifier(FeatureSelection* pNotifier);
   /** Metodo que fuerza la actualizacion de la tabla **/
   void UpdateTable();
   /** Metodo que devuelve el ModelOrderWidget,*/
   ModelOrderWidget* GetModelOrderWidget();
   /** Metodo a ser redefinido para recibir Notificaciones de un Sujeto (Observable) */
   virtual void Update(NotificationInterface* pNotification);
   /** Configura el titulo para el Part */
   void SetWindowTitle(const std::string& WindowTitle);
   /** Limpia la seleccion actual*/
   void ClearSelection();
   /** Selecciona todos los registros de la tabla */
   void SelectAll();
   /** Invierte la seleccion actual */
   void InvertSelection();
   /** Armado de consultas */
   void QueryBuilder();
   /** Oculta la columna correspondiente al indice que se pasa por parametro**/
   bool HiddeColumn(int ColumnIndex);
   /** Muestra la columna correspondiente al indice que se pasa por parametro**/
   bool ShowHiddenColumn(int ColumnIndex);
   /** Oculta la columna correspondiente al indice que se pasa por parametro**/
   bool IsColumnHidden(int ColumnIndex) const;
   /** Configura una columna como readonly */
   void SetReadOnlyColumnView(const std::string& ColumnName, bool ReadOnly = true);
   /** Consulta si una columna es readonly**/
   bool IsReadOnlyColumn(const std::string& Name) const;
   /** Obtiene el indice correspondiente al modelo de la tabla **/
   int GetFixedColumnIndex(int ColumnIndex) const;
   /** Establece el SRS del tablepart */
   void SetSrs(const std::string& Srs);
   /** Aplica el filtro recibido por par�metro sobre la tabla */
   virtual void SetFilter(const std::string &Condition);
   void SetOption(const Option& Options) {
      options_.SetOption(Options.GetOption());
      if (pTable_)
         pTable_->SetOption(options_);
   }
   bool IsEditing() { return  pWxGenericTableBase_->IsEditing();}
   /** Este metodo es una herramienta para poder configurar eventos
    *  a la grilla desde afuera, es como una especie de visitor que
    *  deja la puerta abierta a customizaciones. Habria que buscar
    *
    */
   wxGrid* GetGridControl() {
      return pGrid_;
   }

private:
   /** Metodo auxiliar que se dispara cuando se selecciona una grila */
   void OnGridSelect(wxGridEvent &Event);
   /** Metodo que captura el evento de click de celda **/
   void OnCellClick(wxGridEvent &Event);
   /** Metodo que captura el evento de doble click sobre una celda **/
   void OnCellDoubleClick(wxGridEvent &Event);
   /** Metodo que maneja la selecci�n m�ltiple de filas*/
   void OnMultipleSelection(wxGridRangeSelectEvent &Event);
   /** Funci�n auxiliar para verificar si se debe actualizar la selecci�n de geometr�as*/
   bool ValidateMultipleSelection(wxGridCellCoords, wxGridCellCoords) const;
   /** Declara el objeto de eventos como clase friend */
   FRIEND_EVENT_OBJECT(TablePartEventHandler);
   EVENT_OBJECT_PROPERTY(TablePartEventHandler);         /*! Objeto para conectar */
   TableTool* pTableTool_;                               /*! Puntero a Table Tool*/
   Table* pTable_;                                       /*! Puntero a Table */
   std::list<std::string> commandList_;                  /*! Lista de Comandos */
   wxGenericTableBase* pWxGenericTableBase_;             /*! Puntero a wxGenericTableBase */
   wxPanel* pPanel_;                                     /*! Panel */
   wxGrid* pGrid_;                                       /*! Grid */
   std::string bitmapUrl_;                               /*! Url del icono de la ventana */
   std::set<long> selectedRowsIds_;                      /**! listado de filas seleccionadas */
   int lastRowSelected_;                                 /**! Ultima fila seleccionada */
   FeatureSelection *pFeatureSelection_;                 /** notificador de seleccion */
   int selectionObserverId_;                             /** Id de observer en pFeatureSelection_ */
   bool includeOrderSelection_; /*! boolean que decide si se agrega el widget de seleccion de orden**/
   bool includeRefSystem_;  // Incluye selector de sistema de referencia.
   ModelOrderWidget *pModelOrder_;
   GeoreferenceRefSysPart* pRefSys_;
   /** handler que maneja la accion sobre celdas con tipos de datos custom ej. hotlink**/
   CellTableHandler* pHandler_;
   /** administrador de infromacion de la aplicacion **/
   suri::DataViewManager* pDataViewManager_;
   std::string srs_;
   bool enableSearch_; // Incluye el SearchWidget
   SearchWidget* pSearchWidget_; /** Puntero a SearchWidget */
   std::string lastCondition_; /** Guarda la ultima condicion ejecutada */
   wxString statement_; /** Condicion de la consulta */
   Option options_;
};

} /** namespace suri */

#endif /* TABLEPART_H_ */