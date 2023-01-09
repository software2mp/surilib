/*! \file TablePart.cpp */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2012-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

// Includes Estandar
#include <sstream>
#include <vector>

// Includes Suri
#include "TablePart.h"
#include "wxGenericTableBase.h"
#include "suri/messages.h"
#include "suri/FeatureSelection.h"
#include "ModelOrderWidget.h"
#include "GeoreferenceRefSysPart.h"
#include "suri/LibraryManager.h"
#include "HotLinkCellTableHandler.h"
#include "ExtendedThresholdEditionCellHandler.h"
#include "SpatialReference.h"
#include "suri/ToolSupport.h"
#include "SearchWidget.h"
#include "QueryBuilderPart.h"
#include "EquationParser.h"
#include "suri/AuxiliaryFunctions.h"

// Includes Wx
#include "wx/grid.h"
#include "wx/regex.h"

#define SRS_LIBRARY_CODE "srs"

namespace suri {

/** \cond MACRO_IMPLEMENT_EVENT_OBJECT */
START_IMPLEMENT_EVENT_OBJECT(TablePartEventHandler, TablePart)
/** Evito el evento de destruccion */
IMPLEMENT_EVENT_CALLBACK(OnDestroy, OnDestroy(event) , wxWindowDestroyEvent)
/** Evento para el manejo de la seleccion*/
IMPLEMENT_EVENT_CALLBACK(OnGridSelect, OnGridSelect(event) , wxGridEvent)
IMPLEMENT_EVENT_CALLBACK(OnCellClick, OnCellClick(event) , wxGridEvent)
IMPLEMENT_EVENT_CALLBACK(OnCellDoubleClick, OnCellDoubleClick(event) , wxGridEvent)
IMPLEMENT_EVENT_CALLBACK(OnMultipleSelection, OnMultipleSelection(event), wxGridRangeSelectEvent)
END_IMPLEMENT_EVENT_OBJECT

const std::string Where = " WHERE ";
const std::string Or = " OR ";
const std::string Like = " LIKE ";
const std::string Percent = "%";
const std::string Quote = "'";

/**
 * Constructor
 */
TablePart::TablePart(Table* pTable, bool IncludeOrderSelection, bool IncludeRefSystem,
                     suri::DataViewManager* pDataViewManager, bool EnableSearch) :
      NEW_EVENT_OBJECT(TablePartEventHandler), pTableTool_(NULL), pTable_(pTable),
      pWxGenericTableBase_(NULL), pPanel_(NULL), pGrid_(NULL), pFeatureSelection_(NULL),
      includeOrderSelection_(IncludeOrderSelection), includeRefSystem_(IncludeRefSystem),
      pModelOrder_(NULL),  pRefSys_(NULL), pHandler_(new HotLinkCellTableHandler()),
      pDataViewManager_(pDataViewManager), enableSearch_(EnableSearch), pSearchWidget_(NULL) {
   pWxGenericTableBase_ = new wxGenericTableBase(pTable);
   if (includeOrderSelection_)
      pModelOrder_ = new ModelOrderWidget(this);

   if (includeRefSystem_ && pDataViewManager != NULL) {
      Library* plibrary = pDataViewManager->GetLibraryManager()->GetLibraryByCode(
            SRS_LIBRARY_CODE, LibraryManager::AccessKey(true));
      pRefSys_ = new GeoreferenceRefSysPart(plibrary->GetId(),
                                            pDataViewManager->GetLibraryManager(),
                                            this);
   }

   pHandler_->SetSucesor(new ExtendedThresholdEditionCellHandler);
   if (EnableSearch) pSearchWidget_ = new SearchWidget(this);

   windowTitle_ = _(caption_TABLE_PART);
   bitmapUrl_ = "";
}

/**
 * Destructor
 */
TablePart::~TablePart() {
   SetSelectionNotifier(NULL);
   if (pTableTool_) {
      pTableTool_->SetTablePart(NULL);
      // este delete debera ser agregado cuando se implemente la correcta delecion de los
      // button asociados a un command. Genera SIGSEGV en evento Button::OnUIUpdate()
      delete pTableTool_;
      pTableTool_ = NULL;
   }
   // No se utiliza porque genera SIGSEGV al salir de la aplicacion
#ifdef __UNUSED_CODE__
   pGrid_->Disconnect(wxEVT_DESTROY, wxWindowDestroyEventHandler(TablePartEventHandler::OnDestroy),
         NULL, pEventHandler_);
#endif
   DELETE_EVENT_OBJECT;

   if (pHandler_)
      delete pHandler_;
}

/**
 * Cambia el titulo de la ventana
 * @param Title
 */
void TablePart::ChangeWindowTitle(const std::string& Title) {
   windowTitle_ = Title;
}

/**
 * Setea el Table Tool
 * @param pTableTool
 */
void TablePart::SetTableTool(TableTool* pTableTool) {
   pTableTool_ = pTableTool;
}

/**
 * Getea el Table Tool
 * @return
 */
TableTool* TablePart::GetTableTool() {
   return pTableTool_;
}

/**
 * Agrega un comando
 * @param CommandName Nombre del Comando a ser agregado
 * @return Devuelve true si pudo agregar el comando satisfactoriamente, sino devuelve false
 */
bool TablePart::AddCommand(const std::string &CommandName) {
   // TODO(Sebastian - TCK #2218): Revisar
   commandList_.push_back(CommandName);
   return true;
}

/**
 * Agrega un comando
 * @param CommandName Nombre del Comando a ser quitado
 * @return Devuelve true si pudo quitar el comando satisfactoriamente, sino devuelve false
 */
bool TablePart::RemoveCommand(const std::string &CommandName) {
   // TODO(Sebastian - TCK #2218): Revisar
   commandList_.remove(CommandName);
   return true;
}

/**
 * Indica si el Part tiene cambios para salvar
 * @return
 */
bool TablePart::HasChanged() {
   // TODO(Sebastian - TCK #2218): Revisar
   return pWxGenericTableBase_->HasChanged();
}

/**
 * Salva los cambios realizados en el Part
 * @return
 */
bool TablePart::CommitChanges() {
   // TODO(Sebastian - TCK #2218): Revisar
   pWxGenericTableBase_->ResetChangesFlag();
   bool success = pTable_->Commit();
   return success;
}

/**
 * Restaura los valores originales del Part
 * @return
 */
bool TablePart::RollbackChanges() {
   return pTable_->Rollback();
}

/**
 * Crea el Tool Window
 * @return
 */
bool TablePart::CreateToolWindow() {
   pPanel_ = wxXmlResource::Get()->LoadPanel(pParentWindow_,
                                             wxT("ID_TABLE_EDITOR_PANEL"));
   pGrid_ = XRCCTRL(*pPanel_, wxT("ID_TABLE_EDITOR_GRID"), wxGrid);

   if (pGrid_ && pGrid_->SetTable(pWxGenericTableBase_)) {
      pToolWindow_ = pPanel_;

      pGrid_->Connect(wxEVT_DESTROY,
                      wxWindowDestroyEventHandler(TablePartEventHandler::OnDestroy),
                      NULL, pEventHandler_);

      pGrid_->Connect(wxEVT_GRID_LABEL_LEFT_CLICK,
                      wxGridEventHandler(TablePartEventHandler::OnGridSelect), NULL,
                      pEventHandler_);

      pGrid_->Connect(wxEVT_GRID_CELL_LEFT_CLICK,
                      wxGridEventHandler(TablePartEventHandler::OnCellClick), NULL,
                      pEventHandler_);

      pGrid_->Connect(wxEVT_GRID_CELL_LEFT_DCLICK,
                      wxGridEventHandler(TablePartEventHandler::OnCellDoubleClick), NULL,
                      pEventHandler_);

      pGrid_->Connect(wxEVT_GRID_RANGE_SELECT,
		               wxGridRangeSelectEventHandler(TablePartEventHandler::OnMultipleSelection),
		               NULL, pEventHandler_);
   }

   if (includeRefSystem_ && pRefSys_) {
      AddControl(pRefSys_, wxT("ID_MODEL_ORDER_PANEL"));
      if (!srs_.empty()) {
         pRefSys_->SetSrs(srs_);
      } else {
         RasterDatasource* prasterds = ToolSupport::GetRasterDatasource(
               pDataViewManager_->GetDatasourceManager(),
               pDataViewManager_->GetViewcontextManager());
         if (prasterds) {
            std::string sr = prasterds->GetSpatialReference();
            std::string name = SpatialReference::GetAuthorityId(sr);
            Library* plibrary =
                  pDataViewManager_->GetLibraryManager()->GetLibraryByCode(
                        SRS_LIBRARY_CODE, LibraryManager::AccessKey(true));
            const LibraryItem* pitem = plibrary->GetItemByPrincipal(name);
            if (pitem) pRefSys_->SetActiveItem(pitem, true);
         }
      }
   }

   if (pModelOrder_) {
      AddControl(pModelOrder_, wxT("ID_MODEL_ORDER_PANEL"));
   }

   if (pSearchWidget_)
      AddControl(pSearchWidget_, wxT("ID_SEARCH_PANEL"));
   return (pToolWindow_ != NULL);
}

/**
 * Inicializa el part
 */
void TablePart::SetInitialValues() {
}

/**
 * Actualiza el estado de la parte
 */
void TablePart::Update() {
}

/**
 * Getter de Table
 * @return
 */
Table* TablePart::GetTable() {
   return pTable_;
}

/**
 * Para no autodestruirse al eliminar un editor
 * @param Event evento de destruccion de la ventana
 */
void TablePart::OnDestroy(wxWindowDestroyEvent &Event) {
   Event.Skip(false);
}

/**
 * Cambia el icono de la ventana
 * @param ToolBitmap
 * @param X
 * @param Y
 */
void TablePart::SetWindowIcon(const std::string& BitmapUrl) {
   bitmapUrl_ = BitmapUrl;
}

/**
 * Obtiene el icono de la ventana
 * @param ToolBitmap
 * @param X
 * @param Y
 */
void TablePart::GetWindowIcon(wxBitmap& ToolBitmap, int X, int Y) const {
   if (bitmapUrl_.compare("") == 0) {
      Widget::GetWindowIcon(ToolBitmap, X, Y);
      return;
   }

   GET_BITMAP_RESOURCE(bitmapUrl_.c_str(), ToolBitmap);

   if (X > 0 && Y < 0) {
      Y = static_cast<int>(static_cast<double>(ToolBitmap.GetWidth()) * X
            / ToolBitmap.GetHeight());
   }
   if (X < 0 && Y > 0) {
      X = static_cast<int>(static_cast<double>(ToolBitmap.GetHeight()) * Y
            / ToolBitmap.GetWidth());
   }

   ESCALE_BITMAP(ToolBitmap, X, Y);
}

/** Obtiene las columnas seleccionadas. */
wxArrayInt TablePart::GetSelectedRows() {
   return pWxGenericTableBase_->GetView()->GetSelectedRows();
}

/** Obtiene las filas seleccionadas*/
wxArrayInt TablePart::GetSelectedColumns() {
   return pWxGenericTableBase_->GetView()->GetSelectedCols();
}

/** Metodo que es llamado cuando se actualiza la seleccion. */
void TablePart::SelectionUpdated() {
   if (!pFeatureSelection_)
      return;
   pGrid_->BeginBatch();
   selectedRowsIds_.clear();
   pGrid_->ClearSelection();
   std::vector<FeatureIdType> selectedfeatures =
         pFeatureSelection_->GetSelectedFeatures();
   std::vector<FeatureIdType>::const_iterator it = selectedfeatures.begin();
   for (; it != selectedfeatures.end(); ++it) {
      // si el feature seleccionado no estaba seleccionado anteriormente lo selecciono
      if (selectedRowsIds_.find(*it) == selectedRowsIds_.end()) {
         int row = pWxGenericTableBase_->GetTable()->GetRowById(*it);
         if (row >= 0) {
            pGrid_->SelectRow(row, true);
            selectedRowsIds_.insert(*it);
         }
      }
   }
   pGrid_->EndBatch();
}

/** Metodo auxiliar que se dispara cuando se selecciona una fila o una columna */
void TablePart::OnGridSelect(wxGridEvent &Event) {
   Table* ptable = pWxGenericTableBase_->GetTable();
   if (!ptable)
      return;
   if (Event.ControlDown()) {
      selectedRowsIds_.insert(ptable->GetRowId(Event.GetRow()));
   } else if (Event.ShiftDown()) {
      int rows = pWxGenericTableBase_->GetNumberRows();
      int i = (lastRowSelected_ + 1) % rows;
      for (; i <= Event.GetRow(); i = i + 1 % rows) {
         selectedRowsIds_.insert(ptable->GetRowId(i));
      }
   } else {
      selectedRowsIds_.clear();
      int row = Event.GetRow();
      /** En este caso no se estara seleccionando una
       * fila valida, por lo que no se incorporara
       */
      if (row < 0) {
         return;
      }
      selectedRowsIds_.insert(ptable->GetRowId(row));
   }
   lastRowSelected_ = Event.GetRow();
   if (pFeatureSelection_)
      pFeatureSelection_->UpdateSelection(selectedRowsIds_);
   Event.Skip();
}

/** Metodo que captura el evento de click de celda **/
void TablePart::OnCellClick(wxGridEvent &Event) {
   int row = Event.GetRow();
   int column = Event.GetCol();
   CellTableHandler::KeyDown keydown =
         Event.ControlDown() ? CellTableHandler::CTRL : CellTableHandler::NONE;
   CellTableHandler::MouseEvent mevent = CellTableHandler::LCLICK;
   pHandler_->Handle(row, column, pGrid_, pTable_, keydown, mevent);
   Event.Skip();
}

/** Metodo que captura el evento de doble click sobre una celda **/
void TablePart::OnCellDoubleClick(wxGridEvent &Event) {
	int row = Event.GetRow();
	int column = Event.GetCol();
	CellTableHandler::KeyDown keydown =
			Event.ControlDown() ?
					CellTableHandler::CTRL : CellTableHandler::NONE;
	CellTableHandler::MouseEvent mevent = CellTableHandler::LCLICK;
	pHandler_->Handle(row, column, pGrid_, pTable_, keydown, mevent);
	Event.Skip();
}

/** Configura la instancia encargada de notificar la seleccion**/
void TablePart::SetSelectionNotifier(FeatureSelection* pNotifier) {
   if (pFeatureSelection_)
      pFeatureSelection_->Detach(selectionObserverId_);
   pFeatureSelection_ = pNotifier;
   if (pFeatureSelection_)
      selectionObserverId_ = pFeatureSelection_->Attach(this);
}

/** Metodo que fuerza la actualizacion de la tabla **/
void TablePart::UpdateTable() {
   pTable_->UpdateGrid();
}

/** Metodo que devuelve el ModelOrderWidget,*/
ModelOrderWidget *TablePart::GetModelOrderWidget() {
   return pModelOrder_;
}

/** Se implementa el metodo para actualizar el estado del observer
 *  @param[in] Notificacion emitida por el cambio en el widget
 */

void TablePart::Update(NotificationInterface* pNotification) {
   pNotification->ApplyNotification(this);
}

/** Configura el titulo para el Part */
void TablePart::SetWindowTitle(const std::string& WindowTitle) {
   windowTitle_ = WindowTitle;
}

/** Limpia la seleccion actual*/
void TablePart::ClearSelection() {
   selectedRowsIds_.clear();
   pGrid_->ClearSelection();
   pFeatureSelection_->UpdateSelection(selectedRowsIds_);
}

/** Selecciona todos los registros de la tabla */
void TablePart::SelectAll() {
   std::vector<long> ids = pTable_->GetRowIds();
   std::vector<long>::iterator it = ids.begin();
   selectedRowsIds_.clear();
   for (; it != ids.end(); ++it) {
      selectedRowsIds_.insert(*it);
   }
   pGrid_->SelectAll();
   pFeatureSelection_->UpdateSelection(selectedRowsIds_);
}

/** Invierte la seleccion actual */
void TablePart::InvertSelection() {
   pGrid_->ClearSelection();
   std::set<long> invertselection;
   std::vector<long> ids = pTable_->GetRowIds();
   std::vector<long>::iterator it = ids.begin();
   for (; it != ids.end(); ++it) {
      if (selectedRowsIds_.find(*it) == selectedRowsIds_.end()) {
         invertselection.insert(*it);
         long row = pTable_->GetRowById(*it);
         if (row >= 0)
            pGrid_->SelectRow(row);
      }
   }
   selectedRowsIds_ = invertselection;
   pFeatureSelection_->UpdateSelection(selectedRowsIds_);
}
/** Oculta la columna correspondiente al indice que se pasa por parametro**/
bool TablePart::HiddeColumn(int ColumnIndex) {
   return pWxGenericTableBase_ ?
         pWxGenericTableBase_->HiddeColumn(ColumnIndex) : false;
}

/** Muestra la columna correspondiente al indice que se pasa por parametro**/
bool TablePart::ShowHiddenColumn(int ColumnIndex) {
   return pWxGenericTableBase_ ?
         pWxGenericTableBase_->ShowHiddenColumn(ColumnIndex) : false;
}

/** Oculta la columna correspondiente al indice que se pasa por parametro**/
bool TablePart::IsColumnHidden(int ColumnIndex) const {
   return
         pWxGenericTableBase_ ? pWxGenericTableBase_->IsColumnHidden(
                                      pTable_->GetColumnId(ColumnIndex)) :
                                false;
}

/** Configura una columna como readonly */
void TablePart::SetReadOnlyColumnView(const std::string& ColumnName, bool ReadOnly) {
   if (pWxGenericTableBase_)
      pWxGenericTableBase_->SetReadOnlyColumnView(ColumnName, ReadOnly);
}
/** Consulta si una columna es readonly**/
bool TablePart::IsReadOnlyColumn(const std::string& ColumnName) const {
   return pWxGenericTableBase_ ?
         pWxGenericTableBase_->IsReadOnlyColumn(ColumnName) : false;
}
/** Obtiene el indice correspondiente al modelo de la tabla **/
int TablePart::GetFixedColumnIndex(int ColumnIndex) const {
   return pWxGenericTableBase_ ?
         pWxGenericTableBase_->GetFixedColumnIndex(ColumnIndex) : ColumnIndex;
}

/** Establece el SRS del tablepart */
void TablePart::SetSrs(const std::string& Srs) {
   srs_ = Srs;
}

/** Arma la condicion con los parametros recibidos
 *  @param[in] Condition capturada por el SearchWidget
 */
void TablePart::SetFilter(const std::string &Condition) {
   bool rostate = true;
   if (!Condition.empty()) {
      wxRegEx Expression(("AND|OR|>|<|=|!="), wxRE_ICASE);
      if (Condition.compare(lastCondition_)==0)
         return;
      if (Expression.Matches(Condition))
         statement_ = Where + Condition;
      else {
         statement_ = Where;
         for (int ix = 0, lenix = pTable_->GetColumns(); ix < lenix; ++ix) {
            if (pTable_->GetColumnType(ix) == Table::STRING
                  && !pTable_->GetColumnName(ix).compare(VectorEditorDriver::GeometryColumnName)==0){
               statement_ = statement_ +
                     pTable_->GetColumnName(ix) + Like + Quote + Percent + Condition
                     + Percent + Quote + Or;
            }
         }
         // Borra el OR que sobra en la ultima condicion
         statement_ = statement_.substr(0, statement_.size() - 4);
      }
   } else {
      rostate = false;
      statement_ = Condition;
   }
   pTable_->SetReadOnlyState(rostate);

   lastCondition_ = Condition;

   if (!pTable_->ExecuteQuery(statement_.c_str())) {
      SHOW_ERROR(message_INVALID_CONDITION);
   }
}

/** Arma una consulta SQL con QueryBuilderPart */
void TablePart::QueryBuilder() {
	QueryBuilderPart* pquerybuilder = new QueryBuilderPart(this, pSearchWidget_,
			lastCondition_);
   pquerybuilder->CreateTool();
   pquerybuilder->ShowModal();
}

/** Maneja la selección múltiple de geometrias */
void TablePart::OnMultipleSelection(wxGridRangeSelectEvent &Event){
   if(!Event.Selecting())
      return;
   wxGridCellCoords topLeft = Event.GetTopLeftCoords();
   wxGridCellCoords bottomRight = Event.GetBottomRightCoords();
   if(!ValidateMultipleSelection(topLeft,bottomRight))
      return;
   Table* ptable = pWxGenericTableBase_->GetTable();
   if (!ptable)
      return;
   int row;
   /** Se agregan las columnas seleccionadas */
   for (row = topLeft.GetRow(); row <= bottomRight.GetRow(); row++)
      selectedRowsIds_.insert(ptable->GetRowId(row));
   lastRowSelected_ = row;
   if (pFeatureSelection_){
      /** Desconecto temporalmente el handler de este evento, porque el llamado a
       *  UpdateSelection() vuelve a dispararlo, lo cual resulta en recursión infinita
       */
      pGrid_->Disconnect(wxEVT_GRID_RANGE_SELECT,
                        wxGridRangeSelectEventHandler(TablePartEventHandler::OnMultipleSelection),
                        NULL, pEventHandler_);
      pFeatureSelection_->UpdateSelection(selectedRowsIds_);
      pGrid_->Connect(wxEVT_GRID_RANGE_SELECT,
                        wxGridRangeSelectEventHandler(TablePartEventHandler::OnMultipleSelection),
                        NULL, pEventHandler_);
   }
   Event.Skip();
}

/** Función auxiliar que valida si se debe actualizar la selección
 *  de las geometrías en el evento de selección múltiple sobre la tabla.
 */
bool TablePart::ValidateMultipleSelection(wxGridCellCoords topLeft,
                                          wxGridCellCoords bottomRight) const{
   int topLeftCol = topLeft.GetCol();
   int bottomRightCol = bottomRight.GetCol();
   int lastRow = pGrid_->GetNumberCols() - 1;
   if(topLeftCol != 0 || bottomRightCol != lastRow)
      return false;
   return true;
}

} /** namespace suri */
