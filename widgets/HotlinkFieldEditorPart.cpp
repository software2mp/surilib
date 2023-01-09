/*! \file HotlinkFieldEditorPart.cpp */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

// Includes standard
#include <string>
#include <vector>
#include <set>

// Includes Suri
#include "HotlinkFieldEditorPart.h"
#include "suri/Configuration.h"
#include "suri/messages.h"
#include "suri/VectorEditor.h"
#include "suri/VectorElement.h"
#include "VectorDatasource.h"
#include "LayerTablePart.h"
#include "logmacros.h"

// Includes Wx
#include "wx/wx.h"
#include "wx/xrc/xmlres.h"
#include "wx/choice.h"
#include "wx/grid.h"
#include "wx/file.h"

// Includes App

// Defines
/** Indice para la primer columna */
#define FIRST_COLUMN 0
/** Indice que indica sin hotlink */
#define NO_HOTLINK_POSITION 0
/** Indice para el nuevo hotlink */
#define NEW_HOTLINK_POSITION 1
/** Posicion del primer campo */
#define FIRST_FIELD_POSITION 2

/**
 * No se usa vector en memoria porque para remplazar el que esta en disco no debe
 * estar abierto. El renderizador siempre tiene un Vector abierto.
 */
#undef __USE__MEMORY__VECTOR__

// fordward
/** inicializa los recursos de la ventana */
void InitHotlinkEditorXmlResource();

/** namespace suri */
namespace suri {

/** \cond MACRO_IMPLEMENT_EVENT_OBJECT */
START_IMPLEMENT_EVENT_OBJECT(HotlinkFieldEditorPartEvent, HotlinkFieldEditorPart)
   IMPLEMENT_EVENT_CALLBACK(OnChoiceChanged, OnChoiceChanged(event), wxCommandEvent)
#ifdef __MARK__INVALID__HOTLINKS__
   IMPLEMENT_EVENT_CALLBACK(OnCellChange, OnCellChange(event), wxGridEvent)
#endif
   IMPLEMENT_EVENT_CALLBACK(OnCellDoubleClick, OnCellDoubleClick(event), wxGridEvent)
   IMPLEMENT_EVENT_CALLBACK(OnKeyDown, OnKeyDown(event), wxKeyEvent)
END_IMPLEMENT_EVENT_OBJECT
/** \endcond */

/**
 * Constructor
 * @param[in] Layer numero de capa
 * @param[in] pVectorDatasource vector editado
 */
HotlinkFieldEditorPart::HotlinkFieldEditorPart(int Layer,
                                               VectorDatasource* pVectorDatasource) :
      layer_(Layer), pVectorEditor_(NULL), pVectorDatasource_(pVectorDatasource),
      pVectorElement_(NULL), pLayerPart_(NULL), selectedItem_(NO_HOTLINK_POSITION),
      newColumnName_(""), readOnly_(false), NEW_EVENT_OBJECT(HotlinkFieldEditorPartEvent) {
   pVectorElement_ = dynamic_cast<VectorElement*>(pVectorDatasource_->GetElement());
}

/** Ctor */
HotlinkFieldEditorPart::HotlinkFieldEditorPart(int Layer, VectorElement* pVectorElement) :
      layer_(Layer), pVectorEditor_(NULL), pVectorDatasource_(NULL),
      pVectorElement_(pVectorElement), pLayerPart_(NULL),
      selectedItem_(NO_HOTLINK_POSITION), newColumnName_(""), readOnly_(false),
      NEW_EVENT_OBJECT(HotlinkFieldEditorPartEvent) {
}

/**
 * Dtor
 */
HotlinkFieldEditorPart::~HotlinkFieldEditorPart() {
   delete pVectorEditor_;
}

/**
 * Crea la ventana de la parte
 * @return true siempre
 */
bool HotlinkFieldEditorPart::CreateToolWindow() {
   pToolWindow_ = wxXmlResource::Get()->LoadPanel(pParentWindow_,
                                                  wxT("ID_HOTLINK_TABLE_PART"));
   wxPanel *ptablepanel = XRCCTRL(*pToolWindow_, wxT("ID_TABLE_PANEL"), wxPanel);

   GET_CONTROL(*pToolWindow_, wxT("ID_FIELD_CHOICE"), wxChoice)
      ->Connect(wxEVT_COMMAND_CHOICE_SELECTED,
                wxCommandEventHandler(HotlinkFieldEditorPartEvent::OnChoiceChanged),
                NULL, pEventHandler_);

#ifdef __USE__MEMORY__VECTOR__
   std::string vectormemoryurl = std::string(wxT("shpmemory:")) +
         pVectorDatasource_->GetUrl().c_str();
   pLayerPart_ = new LayerTablePart(layer_, vectormemoryurl, EDIT_NOTHING);
#else
   pLayerPart_ = new LayerTablePart(layer_, pVectorDatasource_->GetUrl().c_str(), EDIT_NOTHING);
#endif
   pLayerPart_->CreatePartTool(ptablepanel);
#ifdef __UNUSED_CODE__
   ptablepanel->GetSizer()->Add(pLayerPart_->GetWindow(), 1, wxEXPAND, 0);
#endif

   ptablepanel->Fit();

   wxGrid *playerdata = XRCCTRL(*(pLayerPart_->GetWindow()), wxT("ID_LAYER_GRID"), wxGrid);

#ifdef __MARK__INVALID__HOTLINKS__
   playerdata->Connect(wxEVT_GRID_CELL_CHANGE,
         wxGridEventHandler(HotlinkFieldEditorPartEvent::OnCellChange),
         NULL, pEventHandler_);
#endif

   playerdata->Connect(
         wxEVT_GRID_CELL_LEFT_DCLICK,
         wxGridEventHandler(HotlinkFieldEditorPartEvent::OnCellDoubleClick), NULL,
         pEventHandler_);

   // Conecto evento KeyDown
   playerdata->Connect(wxEVT_KEY_DOWN,
                       wxKeyEventHandler(HotlinkFieldEditorPartEvent::OnKeyDown), NULL,
                       pEventHandler_);

   return true;
}

/**
 * Devuelve el estado del Part.
 * @return enabled_
 */
bool HotlinkFieldEditorPart::IsEnabled() {
   return enabled_;
}

/**
 * Indica si el Part tiene cambios para salvar.
 * @return true si hay cambios, false en otro caso
 */
bool HotlinkFieldEditorPart::HasChanged() {
   bool returnvalue = false;
   // Verifico si el campo es diferente al que indica el nodo
   wxString fieldname = pVectorElement_->GetHotLinkFieldName(layer_);
   if (!fieldname.IsSameAs(GetSelectedHotlink().c_str())) {
      returnvalue = true;
   }
   // Si el campo es el mismo, veo si hay cambios en la tabla
   if (!returnvalue) {
      returnvalue = pLayerPart_->HasChanged();
   }
   return returnvalue;
}

/**
* Salva los cambios realizados en el Part. Si el campo es nuevo, actualiza
 * el contenido del wxChoice
 * @return true si pudo aplicar los cambios, false en otro caso
 */
bool HotlinkFieldEditorPart::CommitChanges() {
   bool returnvalue = false;

   // Si es necesario modifico el nodo del hotlink en el elemento
   wxString fieldname = pVectorElement_->GetHotLinkFieldName(layer_);
   if (!fieldname.IsSameAs(GetSelectedHotlink().c_str())) {
      pVectorElement_->SetHotLinkFieldName(layer_, GetSelectedHotlink().c_str());
   }

   returnvalue = pLayerPart_->CommitChanges();

   // Recalculo la lista (si agrego una columna)
   LoadFieldList();
   LoadHotlinkFromXml();
   pLayerPart_->RollbackChanges();

   return returnvalue;
}

/**
 * Restaura los valores originales del Part.
 * @return true si pudo restaurar los valores, false en otro caso
 */
bool HotlinkFieldEditorPart::RollbackChanges() {
   // Elimino los cambios antes de llamar a LoadFieldList
   GetVectorEditor()->OpenVector(pVectorDatasource_->GetUrl().c_str());
   GetVectorEditor()->OpenLayer(layer_);
   readOnly_ = !GetVectorEditor()->CanWrite();
   GetVectorEditor()->CloseVector();
   LoadFieldList();
   LoadHotlinkFromXml();
   bool returnvalue = pLayerPart_->RollbackChanges();
#ifdef __MARK__INVALID__HOTLINKS__
   ValidateTable();
#endif
   return returnvalue;
}

/**
 * Inicializa los datos mostrados
 */
void HotlinkFieldEditorPart::SetInitialValues() {
   GetVectorEditor()->OpenVector(pVectorDatasource_->GetUrl().c_str());
   GetVectorEditor()->OpenLayer(layer_);
   readOnly_ = !GetVectorEditor()->CanWrite();
   GetVectorEditor()->CloseVector();
   LoadFieldList();
   LoadHotlinkFromXml();
   pLayerPart_->SetInitialValues();
#ifdef __MARK__INVALID__HOTLINKS__
   ValidateTable();
#endif
}

/**
 * Informa si el choice o las tablas tienen datos correctos
 * @return true si el control tiene datos validos, false en otro caso
 */
bool HotlinkFieldEditorPart::HasValidData() {
   bool returnvalue;

   wxChoice* pchoice = XRCCTRL(*pToolWindow_, wxT("ID_FIELD_CHOICE"), wxChoice);
   if (!pchoice) {
      REPORT_DEBUG("D:No existe control para seleccionar campos");
      return false;
   }
   size_t valueselected = pchoice->GetCurrentSelection();
   if (selectedItem_ != NEW_HOTLINK_POSITION && selectedItem_ != valueselected) {
      REPORT_DEBUG("D:Inconsistencia entre variable interna y control");
      return false;
   }
   if (selectedItem_ == NEW_HOTLINK_POSITION
         && valueselected != (pchoice->GetCount() - 1)) {
      REPORT_DEBUG("D:El campo seleccionado deberia ser el ultimo");
      return false;
   }

   returnvalue = pLayerPart_->HasValidData();
   return returnvalue;
}

/**
 * Retorna el nombre de la capa
 * @return string con el nombre de la capa
 */
std::string HotlinkFieldEditorPart::GetName() {
   if (pLayerPart_) {
      return pLayerPart_->GetName();
   } else {
      return "";
   }
}

/**
 * Carga un filtro en la tabla que solo muestra el campo de nombre indicado
 * @param[in] FieldName nombre del campo que se quiere mostrar. Si el nombre
 * del campo esta vacio o no corresponde a un campo de la tabla no
 * muestra ningun campo.
 * @param[in] Reload determina si hay que refrescar la tabla luego de
 * cambiar los datos.
 * \pre los registros en LayerTablePart no deben tener cambios
 * \post si encuentra el nombre del campo en la lista actualiza selectedItem_
 */
bool HotlinkFieldEditorPart::SetLayerTablePartFilter(const std::string &FieldName, bool Reload) {
   wxChoice* pchoice = XRCCTRL(*pToolWindow_, wxT("ID_FIELD_CHOICE"), wxChoice);
   if (!pchoice) {
      REPORT_DEBUG("D:No se pudo cargar el control choice");
      return false;
   }

   std::set<std::string> filter;

   // Cargo registros que no son texto
#ifdef __USE__MEMORY__VECTOR__
   std::string vectormemoryurl = std::string(wxT("shpmemory:")) +
         pVectorDatasource_->GetUrl().c_str();
   GetVectorEditor()->OpenVector(vectormemoryurl);
#else
   GetVectorEditor()->OpenVector(pVectorDatasource_->GetUrl().c_str());
#endif

   GetVectorEditor()->OpenLayer(layer_);
   int fieldcount = GetVectorEditor()->GetFieldCount();
   for (int j = 0; j < fieldcount; j++)
      if (GetVectorEditor()->GetFieldType(j) != DataInfo<unsigned char>::Name) {
         filter.insert(GetVectorEditor()->GetFieldName(j));
      }
   GetVectorEditor()->CloseVector();

   // Cargo registros de tipo texto que estan en choice
   std::vector<std::string>::iterator it = posibleHotlinkFields_.begin();
   it++;  // Ignoro string 'no hay campo seleccionado'
   it++;  // Ignoro string 'crear campo nuevo'
   for (int i = 0; it != posibleHotlinkFields_.end(); i++, it++) {
      if ((*it).compare(FieldName) != 0) {
         filter.insert(*it);
      } else {
         selectedItem_ = i + FIRST_FIELD_POSITION;
      }
   }
   pchoice->SetSelection(selectedItem_);
   if (selectedItem_ < FIRST_FIELD_POSITION && FieldName != "") {
      REPORT_DEBUG("D:El nombre no existe en la tabla");
   }

   if ((pLayerPart_->SetFilter(filter) || pLayerPart_->HasChanged()) && Reload) {
      pLayerPart_->RollbackChanges();
   }

   return true;
}

/**
 * Carga el nombre de los campos (de tipo string) en wxChoice. Ademas
 * agrega las opciones de 'Sin Hotlink' y 'Nuevo hotlink'
 * @return true si puede cargar el nombre de los campos, false en otro caso
 */
bool HotlinkFieldEditorPart::LoadFieldList() {
   wxChoice* pchoice = XRCCTRL(*pToolWindow_, wxT("ID_FIELD_CHOICE"), wxChoice);
   if (!pchoice) {
      REPORT_DEBUG("D:No se pudo cargar el control choice");
      return false;
   }

   posibleHotlinkFields_.clear();
   pchoice->Clear();
   // Cargo los strings sin hotlink y nuevo campo
   posibleHotlinkFields_.push_back(wxT(label_NO_HOTLINK));
   posibleHotlinkFields_.push_back(wxT(label_NEW_FIELD));
   pchoice->Append(wxT(label_NO_HOTLINK));
   if (!readOnly_) {
      pchoice->Append(wxT(label_NEW_FIELD));
   } else {
      pchoice->Append(wxT("-"));
   }

#ifdef __USE__MEMORY__VECTOR__
   std::string vectormemoryurl = std::string(wxT("shpmemory:")) +
         pVectorDatasource_->GetUrl().c_str();
   GetVectorEditor()->OpenVector(vectormemoryurl);
#else
   GetVectorEditor()->OpenVector(pVectorDatasource_->GetUrl().c_str());
#endif
   GetVectorEditor()->OpenLayer(layer_);

   // Cargo los valores en el choice
   int fieldcount = GetVectorEditor()->GetFieldCount();
   for (int j = 0; j < fieldcount; j++) {
      if (GetVectorEditor()->GetFieldType(j) == DataInfo<unsigned char>::Name) {
         wxString fieldname(GetVectorEditor()->GetFieldName(j));
         posibleHotlinkFields_.push_back(fieldname.c_str());
         pchoice->Append(fieldname);
      }
   }

   GetVectorEditor()->CloseVector();

   return true;
}

/**
 * Llama a SetLayerTablePartFilter con nombre de obtenido del nodo del elemento
 * \pre ejecutar LoadFieldList
 * \pre los registros en LayerTablePart no deben tener cambios
 * @return informa si SetLayerTablePartFilter tubo exito
 */
bool HotlinkFieldEditorPart::LoadHotlinkFromXml() {
   // Actualizo el filtro de la tabla+
   wxString fieldname = pVectorElement_->GetHotLinkFieldName(layer_);
   selectedItem_ = NO_HOTLINK_POSITION;

   // Busco si el nombre esta en la base de datos
   std::vector<std::string>::iterator it = posibleHotlinkFields_.begin();
   it++;
   it++;
   bool found = false;
   for (; it != posibleHotlinkFields_.end() && !found; it++)
      if ((*it).compare(fieldname) == 0) {
         found = true;
      }

   if (!found) {
      pVectorElement_->SetHotLinkFieldName(layer_, "");
      fieldname = "";
   }

   return SetLayerTablePartFilter(fieldname.c_str());
}

/**
 * Retorna el nombre del campo seleccionado en el choice. Si es un campo
 * nuevo retorna el nombre ingresado por el usuario.
 * @return nombre de campo asignado a hotlinks
 */
std::string HotlinkFieldEditorPart::GetSelectedHotlink() {
   std::string selectedfieldname = "";
   if (selectedItem_ < 0 || selectedItem_ > posibleHotlinkFields_.size()) {
      REPORT_DEBUG("D:Error. Indice fuera de rango");
   } else {
      if (selectedItem_ == NEW_HOTLINK_POSITION) {
         selectedfieldname = newColumnName_;
      }
      if (selectedItem_ >= FIRST_FIELD_POSITION) {
         selectedfieldname = posibleHotlinkFields_[selectedItem_];
      }
   }
   return selectedfieldname;
}

/**
 * Si la tabla actual sufrio cambios pregunta si quiere continuar.
 * Si se quiere campo nuevo se llama a LayerTalbePart::AddColumn y se guarda
 * el nombre del campo para usarlo en commit.
 * Si se selecciono un campo se muestra una columna con su contenido
 * @param[in] Event wxGridEvent
 */
void HotlinkFieldEditorPart::OnChoiceChanged(wxCommandEvent &Event) {
   wxChoice* pchoice = XRCCTRL(*pToolWindow_, wxT("ID_FIELD_CHOICE"), wxChoice);

   // En caso de no encontrar el choice
   if (!pchoice) {
      REPORT_DEBUG("D:Error. No se pudo encontrar el choice");
      return;
   }

   // Segun el dato seleccionado realizo una tarea distinta
   int newselecteditem = pchoice->GetCurrentSelection();

   // Si no se modifico el campo no hago nada
   if (selectedItem_ != NEW_HOTLINK_POSITION
         && static_cast<int>(selectedItem_) == newselecteditem) {
      return;
   }
   // Si el campo es nuevo pero no se modifico el seleccionado no hago nada
   if (selectedItem_ == NEW_HOTLINK_POSITION
         && newselecteditem == static_cast<int>(pchoice->GetCount() - 1)) {
      return;
   }

   // Si estoy en solo escritura, nuevo vector no hace nada
   if (newselecteditem == NEW_HOTLINK_POSITION && readOnly_) {
      pchoice->SetSelection(selectedItem_);
      return;
   }

   // Si el campo es diferente y hay cambios en la tabla pregunto si esta seguro
   if (pLayerPart_->HasChanged() || selectedItem_ == NEW_HOTLINK_POSITION) {
      // Informo que se van a perder los datos. Y pregunto si quiere continuar
      int optionselected = SHOWQUERY(pToolWindow_, _(question_APPLY_PART_CHANGES),
            wxYES_NO|wxCANCEL|wxYES_DEFAULT|wxICON_QUESTION);

      if (optionselected == wxID_YES) {
         CommitChanges();
      } else {
         if (optionselected == wxID_NO) {
            pLayerPart_->RollbackChanges();
         } else {
            if (selectedItem_ == NEW_HOTLINK_POSITION) {
               pchoice->SetSelection(pchoice->GetCount() - 1);
            } else {
               pchoice->SetSelection(selectedItem_);
            }
            return;
         }
      }
   }

   // Si la columna era nueva la elimino
   if (newColumnName_ != "") {
      LoadFieldList();
      newColumnName_ = "";
   }

   if (newselecteditem == NO_HOTLINK_POSITION) {
      // No muestra ningun campo
      selectedItem_ = newselecteditem;
      SetLayerTablePartFilter("", true);
      pLayerPart_->RollbackChanges();
   }

   if (newselecteditem == NEW_HOTLINK_POSITION) {
      SetLayerTablePartFilter("", true);

      newColumnName_ = pLayerPart_->AddColumn(DataInfo<unsigned char>::Name);

      if (newColumnName_ != "") {
         pchoice->Append(newColumnName_);
         selectedItem_ = newselecteditem;
         pchoice->SetSelection(pchoice->GetCount() - 1);
      } else if (LoadHotlinkFromXml()) {
         pLayerPart_->RollbackChanges();
      }
   }

   if (newselecteditem >= FIRST_FIELD_POSITION) {
      SetLayerTablePartFilter(posibleHotlinkFields_[newselecteditem], true);
   }

   pLayerPart_->RollbackChanges();
}

/**
 * Activa la ventana para seleccionar archivo en disco.
 * @param[in] Event wxGridEvent
 */
void HotlinkFieldEditorPart::OnCellDoubleClick(wxGridEvent &Event) {
   if (readOnly_) {
      return;
   }

   wxGrid *playerdata = XRCCTRL(*(pLayerPart_->GetWindow() ), wxT("ID_LAYER_GRID"), wxGrid);
   if (!playerdata) {
      REPORT_DEBUG("D:Error. No se pudo encontrar la grilla");
      return;
   }

   // Obtengo el archivo
   wxFileDialog filedlg(pToolWindow_, _(caption_SELECT_HOTLINK), wxT(""), wxT(""),
                        filter_ALL_FILES);
   if (filedlg.ShowModal() == wxID_CANCEL) {
      return;
   }
   wxFileName fnpreview(filedlg.GetPath().c_str());
   wxString pathpreview = fnpreview.GetVolume().c_str()
         + (fnpreview.HasVolume() ? fnpreview.GetVolumeSeparator() : wxT(""))
         + fnpreview.GetFullPath(wxPATH_UNIX);

   // Actualizo el control
   playerdata->SetCellValue(Event.GetRow(), Event.GetCol(), pathpreview);
}

#ifdef __MARK__INVALID__HOTLINKS__
/** Controla si hay diferencias con la base de datos */
void HotlinkFieldEditorPart::OnCellChange(wxGridEvent &Event) {
   ValidateTable();
}

/** Valida si un URL es correcto o no */
bool HotlinkFieldEditorPart::IsUrlValid(const std::string &Url) {
   return wxFile::Exists(Url.c_str() );
}

/** Valida si los URLs son correcto o no */
void HotlinkFieldEditorPart::ValidateTable() {
   wxGrid *playerdata = XRCCTRL(*(pLayerPart_->GetWindow()), wxT("ID_LAYER_GRID"), wxGrid);

   // Si no tiene campo hotlink retorno
   if (playerdata->GetNumberCols() == 0) {
      return;
   }

   for (int i = 0; i < playerdata->GetNumberRows(); i++) {
      wxString cellcontent = playerdata->GetCellValue(i, FIRST_COLUMN);
      if (IsUrlValid(cellcontent.c_str())) {
         playerdata->SetCellTextColour(i, FIRST_COLUMN, *wxBLACK);
      } else {
         playerdata->SetCellTextColour(i, FIRST_COLUMN, *wxRED);
      }
   }
}
#endif

/**
 * Retorna el VectorEditor que se usa para acceder al vector.
 * Originalemente LayerTablePart obtenia el VectorEditor en el constructor. Y
 * VectorTablesPart era el encargado de crearlo. El problema surge porque
 * se ejecuta el destructor de VectorTablesPart durante LoadLayer->RemoveColumns
 * al cancelar los cambios. Luego al usar el VectorEditor se genera un segmentation
 * foult. Para solucionarlo ahora es responsabilidad de esta
 * clase generar el VectorEditor. Ver TCK #611
 * @return VectorEditor que se usa para acceder al vector
 */
VectorEditor* HotlinkFieldEditorPart::GetVectorEditor() {
   if (!pVectorEditor_) {
      pVectorEditor_ = new VectorEditor();
   }
   return pVectorEditor_;
}

/**
 *  Controla si se presiono delete y limpia los campos/filas seleccionados
 *  Ver TCK 612
 *  @param[in] Event evento de la tecla presionada
 */
void HotlinkFieldEditorPart::OnKeyDown(wxKeyEvent &Event) {
   if (Event.GetKeyCode() == WXK_DELETE) {
      wxGrid *playerdata = XRCCTRL(*(pLayerPart_->GetWindow()), wxT("ID_LAYER_GRID"), wxGrid);
      // Limpia los campos
      for (int i = 0; i < playerdata->GetNumberRows(); i++) {
         if (playerdata->IsInSelection(i, FIRST_COLUMN)) {
            playerdata->SetCellValue(i, FIRST_COLUMN, "");
         }
      }
      // Limpial las filas
      wxArrayInt selectedrows = playerdata->GetSelectedRows();
      for (size_t i = 0; i < selectedrows.size(); i++) {
         playerdata->SetCellValue(selectedrows[i], FIRST_COLUMN, "");
      }
   }
}
}
