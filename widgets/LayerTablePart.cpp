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

#include "LayerTablePart.h"

// Includes Suri
#include "suri/messages.h"
#include "resources.h"
#include "logmacros.h"
#include "suri/VectorEditor.h"
#include "suri/DataTypes.h"
#include "suri/AuxiliaryFunctions.h"
#include "suri/Vector.h"
#include "MemoryVector.h"

// Includes Wx
#include "wx/wx.h"
#include "wx/xrc/xmlres.h"
#include "wx/grid.h"
// Includes standar
#include <sstream>
#include <ctime>

// defines
/** Valor por defecto para los campos numericos */
#define DEFAULT_NUMERIC_VALUE "0"
/** Tamanio minimo de las columnas */
#define MIN_COLUMN_SIZE 100
/** Margen derecho de la grilla */
#define GRID_RIGHT_MARGIN 25
/** Tamanio para la fila con el nombre de los campos */
#define ROW_LABEL_SIZE 40
/** Maxima cantidad de caracteres permitida para el nombre del campo en ESRI Shapefile */
#define FIELD_NAME_SIZE 10
/**
 * Si esta definido, los nombres de los campos son case insensitive y no pueden
 * estar repetidos.
 * \note ESRI Shapefile no permite nombres repetidos y versiones anteriores a
 *       GDAL 1.7 fallan al verificar los nombres.
 */
#define __FIELD_NAME_CASE_INSENSITIVE__

// forward
// forward
/** inicializa los recursos de la ventana */
void InitLayerTableXmlResource();
/** inicializa los recursos de la ventana */
void InitContextMenuXmlResource();

/** namespace suri */
namespace suri {
/** \cond MACRO_IMPLEMENT_EVENT_OBJECT */
START_IMPLEMENT_EVENT_OBJECT(LayerTablePartEvent, LayerTablePart)IMPLEMENT_EVENT_CALLBACK(OnLabelRightDown, OnLabelRightDown(event), wxGridEvent)
   IMPLEMENT_EVENT_CALLBACK(OnResize, OnResize(event), wxGridEvent)
   IMPLEMENT_EVENT_CALLBACK(OnContextMenu, OnContextMenu(event), wxMenuEvent)
END_IMPLEMENT_EVENT_OBJECT
/** \endcond */
/**
 * Constructor
 * @param[in] Layer numero de capa del vector que se esta editando
 * @param[in] VectorUrl url para el vector editado
 * @param[in] EditFlags operaciones se pueden realizar sobre la tabla
 */
LayerTablePart::LayerTablePart(int Layer, std::string VectorUrl, long EditFlags) :
      pWxLayerGridTable_(NULL), layer_(Layer), pMemoryVectorEditor_(NULL),
      fileName_(VectorUrl), editFlags_(EditFlags), selectedRow_(-1), readOnly_(false),
      NEW_EVENT_OBJECT(LayerTablePartEvent)
{
   windowTitle_ = _(caption_VECTOR_TABLE_EDITION);
}

/**
 *  Dtor
 */
LayerTablePart::~LayerTablePart() {
   if (pWxLayerGridTable_->GetView()) {
      pWxLayerGridTable_->GetView()->SetTable(NULL);
   }

   delete pWxLayerGridTable_;
   pWxLayerGridTable_ = NULL;

   // Cierra y elmina pMemoryVectorEditor_
   pMemoryVectorEditor_->CloseVector();
   delete pMemoryVectorEditor_;
}

/**
 * Crea la ventana de la parte
 * @return true si puede crear la ventana
 * @return false si no puede obtener el XRC de la ventana
 */
bool LayerTablePart::CreateToolWindow() {
   pToolWindow_ = wxXmlResource::Get()->LoadPanel(pParentWindow_,
                                                  wxT("ID_LAYER_TABLE"));

   // Obtiene recurso wxGrid con XRC y setea la tabla a NULL.
   wxGrid *pgrid = XRCCTRL(*pToolWindow_, wxT("ID_LAYER_GRID"), wxGrid);
   if (!pgrid) {
      return false;
   }
   pgrid->SetTable(NULL);
   pgrid->SetExtraStyle(wxWS_EX_BLOCK_EVENTS);

   // Crea wxLayerGridTable con el VectorEditor y la guarda en pWxLayerGridTable_
   pMemoryVectorEditor_ = new VectorEditor();
   pWxLayerGridTable_ = new wxVectorLayerGridTable(pMemoryVectorEditor_);
   pgrid->SetTable(pWxLayerGridTable_);

   // Conecta evento de click sobre boton derecho en wxGrid.
   if ((editFlags_ & (ADD_COLUMNS | REMOVE_ROWS)) != 0) {
      pgrid->Connect(wxEVT_GRID_LABEL_RIGHT_CLICK,
                     wxGridEventHandler(LayerTablePartEvent::OnLabelRightDown), NULL,
                     pEventHandler_);
   }

   pgrid->Connect(wxEVT_SIZE, wxGridEventHandler(LayerTablePartEvent::OnResize), NULL,
                  pEventHandler_);

   if ((editFlags_ & EDIT_CELLS) == 0) {
      pgrid->DisableCellEditControl();
   }

   return true;
}

/**
 * Devuelve el estado del Part.
 * @return true si el part esta habilitado, false en otro caso.
 */
bool LayerTablePart::IsEnabled() {
   return enabled_;
}

/**
 * Informa si se realizaron cambios sobre tabla
 * @return indica si se modifico la tabla
 */
bool LayerTablePart::HasChanged() {
   wxGrid *pgrid = XRCCTRL(*pToolWindow_, wxT("ID_LAYER_GRID"), wxGrid);
   return (pgrid != NULL && pgrid->IsCellEditControlEnabled())
         || pWxLayerGridTable_->IsModified();
}

/**
 * Guarda los cambios del vector en memoria en disco.
 * Si no pudo guardar muestra error por pantalla
 * @return bool que indica si pudo actualizar vector.
 */
bool LayerTablePart::CommitChanges() {
   wxGrid *pgrid = XRCCTRL(*pToolWindow_, wxT("ID_LAYER_GRID"), wxGrid);
   if (!pgrid) {
      REPORT_DEBUG("D:Error. No encontro wxGrid");
      return false;
   }
   // Si se estaba editando una celda se guardan los cambios antes de hacer comit
   if (pgrid->IsCellEditControlEnabled()) {
      pgrid->SaveEditControlValue();
   }

   if (!HasValidData()) {
      return false;
   }

   if (!HasChanged()) {
      return true;
   }

   // Cierra VectorEditor
   pMemoryVectorEditor_->CloseFeature();

   // Copia Vector en memoria a disco
   // Si el vector en disco no se puede modificar informa error y retorna.
   Vector* peditedvector = Vector::Open(GetMemoryUrl());
   if (!peditedvector || !peditedvector->Save(fileName_, "ESRI Shapefile")) {
      SHOW_ERROR(message_VECTOR_SAVE_TO_DISK_ERROR);
      Vector::Close(peditedvector);
      return false;
   }
   Vector::Close(peditedvector);
   return ResetGridTable();
}

/**
 * Pisa vector en memoria con una copia del vector en disco llamando ResetGridTable
 * @return bool, informa si pudo deshacer los cambios
 */
bool LayerTablePart::RollbackChanges() {
   return ResetGridTable();
}

/**
 * Pisa vector en memoria con una copia del vector en disco llamando ResetGridTable
 * \todo: Debe funcionar como el resto de los Part, debe cargar los datos.
 * @return bool, informa si pudo deshacer los cambios
 */
void LayerTablePart::SetInitialValues() {
   ResetGridTable();
}

/**
 * Devuelve el icono de la parte
 * @param[out] ToolBitmap icono de la ventana
 * @param[in] X ancho del icono de la ventana
 * @param[in] Y alto del icono de la ventana
 */
void LayerTablePart::GetWindowIcon(wxBitmap& ToolBitmap, int X, int Y) const {
   GET_BITMAP_RESOURCE(icon_VECTOR_TABLE_EDITOR, ToolBitmap);
}

/**
 * Informa si los datos en la tabla son de tipo correcto. Revisa que todos los valores
 * en la tabla se puedan convertir a datos validos en el vector.
 * @return informa si tuvo exito con todos los campos de la tabla.
 */
bool LayerTablePart::HasValidData() {
   wxGrid *pgrid = XRCCTRL(*pToolWindow_, wxT("ID_LAYER_GRID"), wxGrid);
   return pgrid && !pgrid->IsCellEditControlEnabled();
}

/**
 * Determina que campos de la tabla del vector no se deben mostrar.
 * @param[in] FiltredFieldNames Conjunto con
 * los nombre de los campos que se deben ignorar.
 */
bool LayerTablePart::SetFilter(const std::set<std::string> &FiltredFieldNames) {
   // Le pasa el filtro a pwxLayerGridTable_
   return pWxLayerGridTable_->SetFilter(FiltredFieldNames);
}
/**
 * metodo OnResize
 * @param[in] Event Evento de grilla
 *
 */
void LayerTablePart::OnResize(wxGridEvent &Event) {
   FitColumnsToWindow();
}

/**
 * Este metodo se ejecuta cuando la lista recibe el evento de click der.(sin
 * soltar el boton).
 * Carga menu que corresponda dependiendo de la posicion sobre la que se hizo
 * click
 * @param[in]	Event: evento.
 */
void LayerTablePart::OnLabelRightDown(wxGridEvent &Event) {
   wxMenu *pmenu = NULL;
   wxGrid *playerdata = XRCCTRL(*pToolWindow_, wxT("ID_LAYER_GRID"), wxGrid);
   if (!playerdata) {
      return;
   }

   if (Event.GetRow() < 0 && (editFlags_ & ADD_COLUMNS) && !readOnly_
         && pMemoryVectorEditor_->CanCreateField()) {
      pmenu = wxXmlResource::Get()->LoadMenu(wxT("ID_EDIT_COLUMS_MENU"));
   }

   if (Event.GetRow() >= 0 && (editFlags_ & REMOVE_ROWS) && !readOnly_
         && pMemoryVectorEditor_->CanDeleteFeature()) {
      pmenu = wxXmlResource::Get()->LoadMenu(wxT("ID_EDIT_ROWS_MENU"));
   }

   if (pmenu) {
      pmenu->Connect(wxEVT_COMMAND_MENU_SELECTED,
                     wxMenuEventHandler(LayerTablePartEvent::OnContextMenu), NULL,
                     pEventHandler_);
      selectedRow_ = Event.GetRow();
      playerdata->PopupMenu(pmenu, Event.GetPosition().x, Event.GetPosition().y);

      delete pmenu;
   }
}

/** Retorna el nombre de la capa */
std::string LayerTablePart::GetName() {
   std::stringstream namestream;
   namestream << label_LAYER_TABLE;
   namestream << layer_;
   if (readOnly_) {
      namestream << " (" << label_READ_ONLY << ")";
   }
   return namestream.str();
}

/**
 * Modifica el ancho de las columnas en la tabla para que ocupen el ancho completo
 * de la ventana. Si hay muchas columnas todas tendran ancho
 * MIN_COLUMN_SIZE. Se deje un margen a la derecha para que
 * no aparezca la barra de desplazamiento horizontal
 */
void LayerTablePart::FitColumnsToWindow() {
   wxGrid *playerdata = XRCCTRL(*pToolWindow_, wxT("ID_LAYER_GRID"), wxGrid);

   playerdata->SetDefaultColSize(MIN_COLUMN_SIZE, true);
   playerdata->SetRowLabelSize(ROW_LABEL_SIZE);

   // Calculo el ancho de la ventana
   int gridcolumncount = playerdata->GetNumberCols();
   int windowwidth, windowheight;

   playerdata->GetClientSize(&windowwidth, &windowheight);
   windowwidth -= playerdata->GetRowLabelSize();

   // Se deje un margen a la derecha para que
   // no aparezca la barra de desplazamiento horizontal
   windowwidth -= GRID_RIGHT_MARGIN;

   // Modifico el ancho de las columnas
   int columnwidth = windowwidth;

   if (gridcolumncount > 0) {
      columnwidth = std::max(windowwidth / gridcolumncount, MIN_COLUMN_SIZE);
   }

   for (int i = 0; i < gridcolumncount; i++)
      playerdata->SetColSize(i, columnwidth);

   // Hago un refresh de la tabla
   playerdata->ForceRefresh();
}

/**
 * Este metodo se ejecuta cuando el menu contextual, creado al hacer click der.
 * sobre la lista, recibe un evento de click sobre alguno de sus items.
 * \post Se ejecuta el metodo asociado a cada item del menu.
 * @param[in] Event evento generado al hacer click derecho sobre un elemento de
 *             la lista
 */
void LayerTablePart::OnContextMenu(wxMenuEvent &Event) {
   // Menu de propiedades
   if (Event.GetId() == XRCID("ID_ADD_COLUM") && (ADD_COLUMNS & editFlags_) != 0) {
      AddColumn();
   }
   if (Event.GetId() == XRCID("ID_REMOVE_FEATURE") && (REMOVE_ROWS & editFlags_) != 0) {
      RemoveRow();
   }
   Event.Skip();
}

/**
 * Pregunta al usuario el nombre de la columna, el tipo de dato
 * (usa FieldType si dif de "") y la agrega a la tabla wx.
 * @param[in] FieldType Determina el tipo de dato que tendra la columna,
 * si no se pasa se puede elegir cualquiera.
 * @return std::string, retorna el nombre de la columna que se creo.
 * "" si falla o el usuario cancela la operacion
 */
std::string LayerTablePart::AddColumn(std::string FieldType) {
   wxString columnname;
   std::string returnvalue = "";
   if (FieldType != DataInfo<int>::Name && FieldType != DataInfo<double>::Name
         && FieldType != DataInfo<unsigned char>::Name && FieldType != "") {
      REPORT_DEBUG("D:Error. No pudo agregar columna. Parametro incorrecto");
      return returnvalue;
   }

   wxGrid *playerdata = XRCCTRL(*pToolWindow_, wxT("ID_LAYER_GRID"), wxGrid);
   if (!playerdata) {
      REPORT_DEBUG("D:Error. No se encontro la grilla");
      return returnvalue;
   }

   // creo un dialogo para ingresar nombre y tipo de dato
   wxDialog dialog(NULL, wxID_ANY, _(caption_COLUMN_NAME_INPUT), wxDefaultPosition,
                   wxDefaultSize, wxDEFAULT_DIALOG_STYLE);
   dialog.SetSizer(new wxBoxSizer(wxVERTICAL));
   wxTextCtrl *ptxtctrl = new wxTextCtrl(&dialog, wxID_ANY,
                                         _(content_DEFAULT_COLUMN_NAME),
                                         wxDefaultPosition, wxSize(50, -1));
   ptxtctrl->SetMaxLength(FIELD_NAME_SIZE);
   wxArrayString datatypes;
   if (FieldType == "") {
      datatypes.Add(wxT(label_INT_DATA_TYPE));
      datatypes.Add(wxT(label_DOUBLE_DATA_TYPE));
      datatypes.Add(wxT(label_UCHAR_DATA_TYPE));
   } else {
      if (FieldType.compare(DataInfo<int>::Name) == 0) {
         datatypes.Add(wxT(label_INT_DATA_TYPE));
      }
      if (FieldType.compare(DataInfo<double>::Name) == 0) {
         datatypes.Add(wxT(label_DOUBLE_DATA_TYPE));
      }
      if (FieldType.compare(DataInfo<unsigned char>::Name) == 0) {
         datatypes.Add(wxT(label_UCHAR_DATA_TYPE));
      }
   }

   wxChoice *ptypechoice = new wxChoice(&dialog, wxID_ANY, wxDefaultPosition,
                                        wxSize(-1, -1), datatypes);
   ptypechoice->Select(0);

   dialog.GetSizer()->Add(ptxtctrl, 0, wxEXPAND | wxALL, 5);
   dialog.GetSizer()->Add(ptypechoice, 0, wxEXPAND | wxALL, 5);
   dialog.GetSizer()->Add(dialog.CreateButtonSizer(wxOK | wxCANCEL), 0,
                          wxEXPAND | wxALIGN_BOTTOM);
   dialog.Fit();
   dialog.Update();
   // Valor por default
   columnname = _(content_DEFAULT_COLUMN_NAME);
   ptxtctrl->SetValidator(wxTextValidator(wxFILTER_NONE, &columnname));
   ptxtctrl->SelectAll();
   ptxtctrl->SetFocusFromKbd();

   // muestro el dialogo
   if (dialog.ShowModal() == wxID_OK) {
      dialog.TransferDataFromWindow();

      // Agrego columna
      std::string datatype;
      wxString controltype = ptypechoice->GetStringSelection();
      if (controltype.IsSameAs(wxT(label_INT_DATA_TYPE))) {
         datatype = DataInfo<int>::Name;
      }
      if (controltype.IsSameAs(wxT(label_DOUBLE_DATA_TYPE))) {
         datatype = DataInfo<double>::Name;
      }
      if (controltype.IsSameAs(wxT(label_UCHAR_DATA_TYPE))) {
         datatype = DataInfo<unsigned char>::Name;
      }

      bool validname = true;
      if (!columnname.IsAscii()) {
         SHOW_ERROR(message_NON_ASCII_FIELD_NAME_ERROR);
         validname = false;
      }

      // Verfico que el nombre no este repetido en la base de datos
      int fieldcount = pMemoryVectorEditor_->GetFieldCount();
      if (fieldcount < 0) {
         return "";
      }

      // Recorro el resto de las columnas para buscar
      // el nombre de la columna
      for (int j = 0; j < fieldcount && validname; j++) {
         wxString fieldname(pMemoryVectorEditor_->GetFieldName(j));
#ifdef __FIELD_NAME_CASE_INSENSITIVE__
         fieldname.MakeLower();
         columnname.MakeLower();
#endif
         // Si encuentra otra columna del mismo nombre
         if (fieldname == columnname) {
            validname = false;
            SHOW_ERROR(wxT(message_FIELD_NAME_DUPLICATED));
         }
      }

      // Agrego campo al vector en memoria
      pMemoryVectorEditor_->CloseFeature();
      if (validname
            && pMemoryVectorEditor_->CreateField(columnname.c_str(), datatype)) {
         returnvalue = columnname.c_str();
         pWxLayerGridTable_->SetModified(true);
         pWxLayerGridTable_->LoadDisplayedFields();
         ConfigureGrid();
      }

      playerdata->Update();
   }

   // Reajusto el ancho de las columnas despues de modif la tabla
   FitColumnsToWindow();

   return returnvalue;
}

/**
 * Elimina fila seleccionada de la tabla. Si la fila seleccionada es
 * invalida no hace nada.
 */
void LayerTablePart::RemoveRow() {
   // Elimina la fila del vector en memoria
   long featureid = pMemoryVectorEditor_->GetFeatureId(selectedRow_);
   if (featureid < 0) {
      SHOW_ERROR("No pudo eliminar la columna");
      return;
   }
   pMemoryVectorEditor_->DeleteFeature(featureid);
   pWxLayerGridTable_->SetModified(true);

   // Resetea wxgrid
   ConfigureGrid();

   // Reajusto el ancho de las columnas despues de modif la tabla
   FitColumnsToWindow();
}

/**
 * Configura los datos en el Grid para que sean identicos a los del archivo
 * en disco.
 * @return bool que indica si pudo configurar el wxGrid correctamente.
 */
bool LayerTablePart::ResetGridTable() {
   // Cierra VectorEditor en memoria
   pMemoryVectorEditor_->CloseVector();

   // Copia Vector en disco a memoria
   Vector* pmemoryvector = Vector::Open(GetMemoryUrl(), Vector::ReadWrite);
   Vector* pdiskvector = Vector::Open(fileName_);
   if (pmemoryvector == NULL || pdiskvector == NULL) {
      REPORT_DEBUG("DEBUG: Fallo al crear vector en disco y/o memoria");
      return false;
   }
   pdiskvector->Copy(pmemoryvector);
   Vector::Close(pdiskvector);

   // Actualiza VectorEditor
   pMemoryVectorEditor_->OpenVector(GetMemoryUrl());
   Vector::Close(pmemoryvector);
   pMemoryVectorEditor_->OpenLayer(layer_);

   // Actualiza pWxLayerGridTable_
   pWxLayerGridTable_->SetModified(false);
   pWxLayerGridTable_->LoadDisplayedFields();

   return ConfigureGrid();
}

/**
 * Retorna string con nombre del vector en memoria que se usa para guardar
 * cambios echos por el usuario.
 * @return string con nombre del vector en memoria
 */
std::string LayerTablePart::GetMemoryUrl() {
   return MEMORY + "layertablepart_" + GET_NAME(fileName_);
}

/**
 * Llama a wxGrid::SetTable(pwxLayerGridTable_) para actualizar pantalla
 * Analiza si el vector en disco se puede modificar(actualiza readOnly_).
 * Actualiza el atributo de los campos para que solo se puedan ingresar datos
 * del tipo correcto y si readOnly_=false no se puedan editar.
 * @return bool que indica si pudo configurar el grid.
 */
bool LayerTablePart::ConfigureGrid() {
   // Analiza si el vector en disco se puede modificar
   // (trata de abrirlo sin readonly) y actualiza readOnly_.
   Vector* pdiskvector = Vector::Open(fileName_, Vector::ReadWrite);
   if (!pdiskvector) {
      readOnly_ = true;
   }
   Vector::Close(pdiskvector);

   pWxLayerGridTable_->SetReadOnly(readOnly_);
   pWxLayerGridTable_->RefreshGrid();

   FitColumnsToWindow();

   return true;
}
}

