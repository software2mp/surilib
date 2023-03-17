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

// Includes standard
#include <string>
#include <vector>

// Includes Suri
#include "LibraryItemEditorPart.h"
#include "SpatialReferenceEditionPart.h"
#include "UniversalGraphicalComponentPartInterface.h"
#include "UniversalGraphicalComponentPart.h"
#include "suri/LibraryManager.h"
#include "suri/SuriObject.h"
#include "suri/LibraryItem.h"
#include "suri/LibraryItemAttribute.h"
#include "GdalSrsItemOrigin.h"
#include "SpatialReference.h"
#include "suri/AuxiliaryFunctions.h"
#include "suri/LibraryClient.h"
#include "suri/LibraryManagerFactory.h"
#include "suri/AuxiliaryFunctions.h"
#include "TablePart.h"
#include "suri/Table.h"
#include "DefaultTable.h"
#include "TableTool.h"
#include "TablePartConfigurator.h"
#include "suri/Configuration.h"

// Includes Wx
#include "wx/choice.h"

// Defines
#define DATUM "Datum"
#define GEO_SR_ASOCIATED "Sistema geografico"
#define ID_SR_ELEMENT_ASOCIATED_LABEL "ID_SR_ELEMENT_ASOCIATED_LABEL"
#define AUTHORITY_CODE_TOKEN ":"
#define ID_AUTHORITY_NAME_TEXT "ID_AUTHORITY_NAME_TEXT"
#define ID_AUTHORITY_CODE_TEXT "ID_AUTHORITY_CODE_TEXT"
#define ID_DATUM_SELECT_PANEL "ID_DATUM_SELECT_PANEL"
#define ID_UNITS_PANEL "ID_UNITS_PANEL"
#define ID_SR_DATUM_TEXT "ID_SR_DATUM_TEXT"
#define ID_UNITS_CHOICE "ID_UNITS_CHOICE"
#define ID_TWIN1_ORIENTATION_CHOICE "ID_TWIN1_ORIENTATION_CHOICE"
#define ID_TWIN2_ORIENTATION_CHOICE "ID_TWIN2_ORIENTATION_CHOICE"
#define ID_TWIN1_NAME_TEXT "ID_TWIN1_NAME_TEXT"
#define ID_TWIN2_NAME_TEXT "ID_TWIN2_NAME_TEXT"
#define ID_USE_TWIN_AXES_CHECK "ID_USE_TWIN_AXES_CHECK"
#define ID_PARAMETERS_PANEL "ID_PARAMETERS_PANEL"
#define ID_PROJ_CHOICE "ID_PROJ_CHOICE"
#define UNIT_DEGREE_WKT "UNIT[\"Degree\",0.017453292519943295]"
#define UNIT_METRE_WKT "UNIT[\"metre\",1]"
#define UNIT_KILOMETRE_WKT "UNIT[\"kilometre\",0.001]"

namespace suri {

/** \cond MACRO_IMPLEMENT_EVENT_OBJECT */
START_IMPLEMENT_EVENT_OBJECT(ParametersTableEventHandler, SpatialReferenceEditionPart)
   IMPLEMENT_EVENT_CALLBACK(OnAddRowClick, OnAddRowClick(event), wxCommandEvent)
   IMPLEMENT_EVENT_CALLBACK(OnDelRowClick, OnDelRowClick(event), wxCommandEvent)
   IMPLEMENT_EVENT_CALLBACK(OnTwinAxisCheck, OnTwinAxisCheck(event), wxCommandEvent)
END_IMPLEMENT_EVENT_OBJECT
/** \endcond */


// forwards

/** Nombre del XRC que representa la ventana. */
const std::string SpatialReferenceEditionPart::SPATIAL_REFERENCE_EDITION_XRC_NAME =
      wxT("ID_REFERENCE_SYSTEM_EDITION_PANEL");
const std::string SpatialReferenceEditionPart::SPATIAL_REFERENCE_CGU_XRC_NAME  =
      wxT("ID_REFERENCE_SYSTEM_NAME_PANEL");

/** Ctor. Carga el XRC de la pantalla, e instancia los subpart */
SpatialReferenceEditionPart::SpatialReferenceEditionPart(
      const SuriObject::UuidType& LibraryId, LibraryManager* pManager,
      UniversalGraphicalComponentPartInterface::FeatureCombinationEnum Feature) :
      LibraryItemEditorPart(LibraryId, pManager, SPATIAL_REFERENCE_EDITION_XRC_NAME,
                            SPATIAL_REFERENCE_CGU_XRC_NAME, true, Feature),
                            pDatumSelector_(NULL), pManager_(pManager),
                            pTable_(NULL), pTablePart_(NULL),
      NEW_EVENT_OBJECT(ParametersTableEventHandler), unitIndexSelected_(-1),
      pItemCurrentSel_(NULL), pDatumLibrary_(NULL) {
}

/** Dtor */
SpatialReferenceEditionPart::~SpatialReferenceEditionPart() {
   DELETE_EVENT_OBJECT;
}

/** Indica si el Part tiene cambios para salvar. */
bool SpatialReferenceEditionPart::HasChanged() {
   bool modif = XRCCTRL(*(this->GetWidget()->GetWindow()),
         wxT(ID_AUTHORITY_NAME_TEXT), wxTextCtrl)->IsModified()
         || XRCCTRL(*(this->GetWidget()->GetWindow()),
               wxT(ID_AUTHORITY_CODE_TEXT), wxTextCtrl)->IsModified()
         || XRCCTRL(*(this->GetWidget()->GetWindow()),
               wxT(ID_TWIN1_NAME_TEXT), wxTextCtrl)->IsModified()
         || XRCCTRL(*(this->GetWidget()->GetWindow()),
               wxT(ID_TWIN2_NAME_TEXT), wxTextCtrl)->IsModified();
   int unit = XRCCTRL(*(this->GetWidget()->GetWindow()),
         wxT(ID_UNITS_CHOICE), wxChoice)->GetSelection();
   bool tablechanged = pTablePart_ ? pTablePart_->HasChanged() : false;
   return modif || (unit != unitIndexSelected_) || tablechanged || pCgu_->HasChanged();
}

/** Inicializa el part */
void SpatialReferenceEditionPart::SetInitialValues() {
}

/** Actualiza el estado de la parte */
void SpatialReferenceEditionPart::Update() {
}

/**  Metodo que obtiene a partir de los datos del editor, un vector con los atributos
 *   Este metodo es el que hace el comportamiento particular de cada editor a la hora
 *   de obtener la informacion que posee y transmitirla al sistema de bibliotecas.
 *   Es utilizado a modo de Template Method (en CommitChanges y en el notificador)
 *   @return vector con atributos
 *   @return NULL en caso de que la informnacion del part no sea valida
 *   **/
std::vector<LibraryItemAttribute*>* SpatialReferenceEditionPart::CreateAttributesFromFields() {
   std::vector<LibraryItemAttribute*>* pattrs = NULL;
   if (!resultWkt_.empty()) {
      LibraryItemAttribute* pattr = new LibraryItemAttribute(
            GdalSrsItemOrigin::WKT_ATTRIBUTE_NAME, resultWkt_);
      pattrs = new std::vector<LibraryItemAttribute*>;
      pattrs->push_back(pattr);
   }
   return pattrs;
}

/**
 *  Metodo auxiliar privado que configura el part en si para cada editor en particular.
 *  Este metodo es el que realmente tienen que implementar las clases hijas para
 * lograr la especializacion a la hora de iniciar el part (metodo que se llama
 * en CreateToolWindow, a modo de Template Method).
 **/
bool SpatialReferenceEditionPart::ConfigureFeatures() {
   wxChoice* pch1 = XRCCTRL(*(this->GetWidget()->GetWindow()),
               wxT(ID_TWIN1_ORIENTATION_CHOICE), wxChoice);
   wxChoice* pch2 = XRCCTRL(*(this->GetWidget()->GetWindow()),
               wxT(ID_TWIN2_ORIENTATION_CHOICE), wxChoice);
   pch1->Clear();
   pch2->Clear();
   std::string orientations[] = {"NORTH", "SOUTH", "EAST", "WEST", "UP", "DOWN"};
   for (int i = 0; i < 6 ; ++i) {
      pch1->Append(orientations[i].c_str());
      pch2->Append(orientations[i].c_str());
   }
   pch1->Select(0);
   pch2->Select(2);
   GET_CONTROL(*(this->GetWidget()->GetWindow()),
         "ID_ADD_ROW_BTN", wxBitmapButton)->Connect(
         wxEVT_COMMAND_BUTTON_CLICKED,
         wxCommandEventHandler(ParametersTableEventHandler::OnAddRowClick), NULL,
         pEventHandler_);
   GET_CONTROL(*(this->GetWidget()->GetWindow()),
         "ID_DEL_ROW_BTN", wxBitmapButton)->Connect(
         wxEVT_COMMAND_BUTTON_CLICKED,
         wxCommandEventHandler(ParametersTableEventHandler::OnDelRowClick), NULL,
         pEventHandler_);
   GET_CONTROL(*(this->GetWidget()->GetWindow()),
         wxT(ID_USE_TWIN_AXES_CHECK), wxCheckBox)->Connect(
         wxEVT_COMMAND_CHECKBOX_CLICKED,
         wxCommandEventHandler(ParametersTableEventHandler::OnTwinAxisCheck), NULL,
         pEventHandler_);
   return true;
}

/**
 * Metodo auxiliar privado que es necesario implementar en las clases hijas.
 * Este metodo es el encargado de actualizar realmente la informacion en el part
 * Es llamado en el metodo SetActiveItem a modo de Template Method
 **/
void SpatialReferenceEditionPart::UpdateFields(const LibraryItem* pItem,
                                               bool Modifiable) {
   if (pItem && pItemCurrentSel_ && pItem->GetId().compare(pItemCurrentSel_->GetId()) == 0)
        return;

   const LibraryItemAttribute* pattr =
         pItem ? pItem->GetAttribute(GdalSrsItemOrigin::WKT_ATTRIBUTE_NAME) : NULL;
   std::string srwkt = pattr ? pattr->GetValue() : "";
   std::vector < std::string > authoritycodesplit = tokenizer(
         SpatialReference::GetAuthorityId(srwkt), AUTHORITY_CODE_TOKEN);
   std::string authority;
   std::string code;
   if (authoritycodesplit.size() == 2) {
      authority = authoritycodesplit.at(0);
      code = authoritycodesplit.at(1);
   }

   std::string units;
   std::string axis1;
   std::string axis2;
   std::string datumorgeosr;

   std::string libraryid;
   const LibraryItemAttribute* pprojattr =
           pItem ? pItem->GetAttribute(GdalSrsItemOrigin::ProjectedAttr) : NULL;
   int projected = pprojattr ? StringToNumber<int>(pprojattr->GetValue()) : 0;
   std::string textlabel;
   if (SpatialReference::IsGeographicSpatialRef(srwkt)
         || (!SpatialReference::IsProjectedSpatialRef(srwkt) && projected != 1)) {
      datumorgeosr = SpatialReference::GetAuthorityId(
            SpatialReference::GetGeoCoordinateSystemDatum(srwkt));
      textlabel = _(DATUM);
      units = SpatialReference::GetGeoCoordinateSystemUnits(srwkt);
      if (!pDatumLibrary_) {
         LibraryClient client(pManager_, LibraryManager::READONLY);
         pDatumLibrary_ = client.GetLibraryByCode(LibraryManagerFactory::DatumLibraryCode);
      }
      libraryid = pDatumLibrary_->GetId();
      ConfigureTable(false);
   } else if (SpatialReference::IsProjectedSpatialRef(srwkt) || projected == 1) {
      datumorgeosr = SpatialReference::GetGeoCoordinateSystemAuthorityData(srwkt);
      libraryid = libraryId_;
      units = SpatialReference::GetProjCoordinateSystemUnits(srwkt);
      textlabel = _(GEO_SR_ASOCIATED);
      ConfigureTable(true, srwkt);
   }

   if (pDatumSelector_) {
      pDatumSelector_->SetLibrary(libraryid);
   } else {
      pDatumSelector_ = new UniversalGraphicalComponentPart(
            libraryid, pManager_, NULL,
            UniversalGraphicalComponentPartInterface::ExtendedRwWithFFAndNoLabel, true);
      pDatumSelector_->CreatePartTool(pToolWindow_);
      AddControl(pDatumSelector_->GetWidget(), wxT(ID_DATUM_SELECT_PANEL));
   }
   FillTextFields(authority, code, datumorgeosr, units, axis1, axis2, Modifiable);
   pItemCurrentSel_ = pItem;
}

/** Metodo auxiliar que se encarga de rellenar los campos del editor con
 *  la informacion pasada por parametro
 * @param Authority Autoridad del SR
 * @param Code codigo del SR
 * @param DatumOrGeoSR datum o sistema geografico asociado
 * @param units unidades del SR
 * @param Axis1 eje1
 * @param Axis2 eje2
 * @param Modifiable bool que indica si los componentes son editables o no
 */
void SpatialReferenceEditionPart::FillTextFields(const std::string& Authority,
                                                 const std::string& Code,
                                                 const std::string& DatumOrGeoSR,
                                                 const std::string& Units,
                                                 const std::string& Axis1,
                                                 const std::string& Axis2,
                                                 bool Modifiable) {
   GET_CONTROL(*(this->GetWidget()->GetWindow()),
            wxT(ID_AUTHORITY_NAME_TEXT), wxTextCtrl)->SetValue(Authority);
   GET_CONTROL(*(this->GetWidget()->GetWindow()),
            wxT(ID_AUTHORITY_NAME_TEXT), wxTextCtrl)->Enable(Modifiable);
   GET_CONTROL(*(this->GetWidget()->GetWindow()),
               wxT(ID_AUTHORITY_CODE_TEXT), wxTextCtrl)->SetValue(Code);
   GET_CONTROL(*(this->GetWidget()->GetWindow()),
               wxT(ID_AUTHORITY_CODE_TEXT), wxTextCtrl)->Enable(Modifiable);
   wxChoice* pchoice = XRCCTRL(*(this->GetWidget()->GetWindow()),
                  wxT(ID_UNITS_CHOICE), wxChoice);

   bool checkvalue = XRCCTRL(*(this->GetWidget()->GetWindow()),
               wxT(ID_USE_TWIN_AXES_CHECK), wxCheckBox)->GetValue();
   ConfigureTwinAxisControls(checkvalue && Modifiable);

   GET_CONTROL(*(this->GetWidget()->GetWindow()),
                  wxT(ID_USE_TWIN_AXES_CHECK), wxCheckBox)->Enable(Modifiable);

   GET_CONTROL(*(this->GetWidget()->GetWindow()),
               wxT(ID_PARAMETERS_PANEL), wxPanel)->Enable(Modifiable);
   pchoice->Clear();

   if (pDatumSelector_ && Modifiable)
      pDatumSelector_->GetWidget()->Enable();
   else if (pDatumSelector_ && !Modifiable)
      pDatumSelector_->GetWidget()->Disable();

   if (!Units.empty() && Units.compare(message_NO_DATA) != 0) {
      unitIndexSelected_ = pchoice->Append(Units.c_str());
   } else {
      std::string options[] = {"metre", "kilometre", "degree"};
      for (int i = 0; i < 3 ; ++i) {
         pchoice->Append(options[i].c_str());
      }
      unitIndexSelected_ = 0;
   }

   pchoice->SetSelection(unitIndexSelected_);
   pchoice->Enable(Modifiable);
   if (pDatumSelector_ && !DatumOrGeoSR.empty()) {
      pDatumSelector_->SelectItemByPrincipal(DatumOrGeoSR);
   }
}

/**
 * Metodo auxiliar privado que agrega comportamiento adicional al commit de la
 * clase base. Este comportamiento adicional es necesario cuando
 * el commit involucra objetos adicionales a los items de biblioteca
 * del editor (ej. reproyeccion, en donde se necesita actualizar los params
 * de reproyeccion ademas de leer el item de biblioteca)
 * @return true en caso de realizar correctamente el commit "interno"
 * @return false en caso de que ocurra algun error durante el commit
 */
bool SpatialReferenceEditionPart::OnCommitChanges() {
   GET_CONTROL(*(this->GetWidget()->GetWindow()),
            wxT(ID_AUTHORITY_NAME_TEXT), wxTextCtrl)->SetModified(false);
   GET_CONTROL(*(this->GetWidget()->GetWindow()),
               wxT(ID_AUTHORITY_CODE_TEXT), wxTextCtrl)->SetModified(false);
   GET_CONTROL(*(this->GetWidget()->GetWindow()),
            wxT(ID_TWIN1_NAME_TEXT), wxTextCtrl)->SetModified(false);
   GET_CONTROL(*(this->GetWidget()->GetWindow()),
            wxT(ID_TWIN2_NAME_TEXT), wxTextCtrl)->SetModified(false);
   const LibraryItem* psritem = pCgu_->GetSelectedItem();

   const LibraryItemAttribute* pprojattr =
         psritem ? psritem->GetAttribute(GdalSrsItemOrigin::ProjectedAttr) : NULL;
   int projected = pprojattr ? StringToNumber<int>(pprojattr->GetValue()) : 0;

   resultWkt_ = GetSpatialReferenceWkt(projected == 1);
   return !resultWkt_.empty();
}

/** Metodo auxiliar encargado de configurar la tabla **/
void SpatialReferenceEditionPart::ConfigureTable(bool Enable, const std::string& Wkt) {
   wxPanel* pcontainer = XRCCTRL(*pToolWindow_, wxT("ID_PROJ_DATA_CONTAINER"), wxPanel);
   if (pcontainer != NULL) {
      pcontainer->Show(Enable);
   }

   if (pTable_) {
      delete pTable_;
      pTable_ = NULL;
   }
   if (pTablePart_) {
      RemoveControl(pTablePart_);
      pTablePart_ = NULL;
   }
   if (Enable) {
      pTable_ = new suri::DefaultTable();
      pTable_->AppendColumn("Clave", Table::STRING);
      pTable_->AppendColumn("Valor", Table::STRING);
      std::vector<std::string> strings = tokenizer(Wkt, "PARAMETER[");
      if (strings.size() > 1) {
         std::vector<std::string>::iterator it = strings.begin();
         // skipeo el primero que contiene lo anterior a parameter
         ++it;
         int r = 0;
         for (; it != strings.end(); ++it) {
            std::vector<std::string> keyvaluestr = tokenizer(*it, ",");
            if (keyvaluestr.size() == 2) {
               std::string key = trim(keyvaluestr[0], "\"\"");
               std::string value = trim(keyvaluestr[1], "]");
               pTable_->AppendRow();
               pTable_->SetCellValue(0, r, key);
               pTable_->SetCellValue(1, r, value);
               ++r;
            }
         }
      }
      pTablePart_ = new TablePart(pTable_);
      pTablePart_->SetSelectionNotifier(NULL);
      AddControl(pTablePart_, wxT(ID_PARAMETERS_PANEL));
   }
}

/** Metodo auxiliar que se llama cuando se presiona el boton de agregar fila **/
void SpatialReferenceEditionPart::OnAddRowClick(const wxCommandEvent& Event) {
   if (pTable_)
      pTable_->AppendRow();
}

/** Metodo auxiliar que se llama cuando se presiona el boton de eliminar fila **/
void SpatialReferenceEditionPart::OnDelRowClick(const wxCommandEvent& Event) {
   if (pTablePart_ && pTable_) {
      wxArrayInt srows = pTablePart_->GetSelectedRows();
      for (unsigned int r = 0 ;r < srows.size(); ++r) {
         pTable_->DeleteRow(srows[r]);
      }
   }
}

 /** A traves de la definicion de los parametros arma un wkt con la
  *  proyeccion y los parametros. Resultado algo similar a:
  *  PROJECTION["Transverse_Mercator"],
  *  PARAMETER["latitude_of_origin",0],
  *  PARAMETER["central_meridian",-81],
  *  PARAMETER["scale_factor",0.9996],
  *  PARAMETER["false_easting",500000],
  *  PARAMETER["false_northing",0]
 **/
std::string SpatialReferenceEditionPart::GetProjectionParametersWkt() {
   wxChoice* pprojchoice = XRCCTRL(*(this->GetWidget()->GetWindow()),
            wxT(ID_PROJ_CHOICE), wxChoice);
   std::string transf = pprojchoice->GetStringSelection().c_str();
   // TODO(Gabriek - TCK #4062): Ver de cambiar por funciones de wkt
   std::string wkt = "PROJECTION[\"";
   wkt.append(transf);
   wkt.append("\"],");
   int rows = pTable_->GetRows();
   for (int r = 0; r < rows; ++r) {
      std::string key;
      pTable_->GetCellValue(0, r, key);
      std::string value;
      pTable_->GetCellValue(1, r, value);
      std::string paramwkt;
      GetParameterWkt(key, value, paramwkt);
      paramwkt.append(",");
      wkt.append(paramwkt);
   }
   /** elimina el ultimo caracter (la , final que esta demas) **/
   wkt.erase(wkt.size()-1,1);
   return wkt;
}

/** Obtiene el wkt asociado a los ejes gemelos definidos. El wkt resultado
 *  es la definicion de un par de ejes del estilo:
 *  AXIS["<name>", NORTH | SOUTH | EAST | WEST | UP | DOWN | OTHER],
 *  AXIS["<name>", NORTH | SOUTH | EAST | WEST | UP | DOWN | OTHER]
  **/
std::string SpatialReferenceEditionPart::GetTwinAxisWkt() {
   // TODO(Gabriek - TCK #4062): Ver de cambiar por funciones de wkt
   std::string axeswkt;
   wxTextCtrl* pt1name =
         XRCCTRL(*(this->GetWidget()->GetWindow()), wxT(ID_TWIN1_NAME_TEXT), wxTextCtrl);
   wxChoice* pt1orientation =
         XRCCTRL(*(this->GetWidget()->GetWindow()), wxT(ID_TWIN1_ORIENTATION_CHOICE),
               wxChoice);
   std::string t1name = pt1name->GetValue().c_str();
   std::string t1orientation = pt1orientation->GetStringSelection().c_str();
   GetAxisWkt(t1name, t1orientation, axeswkt);
   axeswkt.append(",");
   wxTextCtrl* pt2name =
         XRCCTRL(*(this->GetWidget()->GetWindow()), wxT(ID_TWIN2_NAME_TEXT), wxTextCtrl);
   wxChoice* pt2orientation =
         XRCCTRL(*(this->GetWidget()->GetWindow()), wxT(ID_TWIN2_ORIENTATION_CHOICE),
               wxChoice);

  std::string t2name = pt2name->GetValue().c_str();
  std::string t2orientation = pt2orientation->GetStringSelection().c_str();
  std::string axis2wkt;
  GetAxisWkt(t2name, t2orientation, axis2wkt);
  axeswkt.append(axis2wkt);
  return axeswkt;
}

/** Metodo que a partir del nombre y orientacion del eje genera el wkt asociado
 * @param[in] Name nombre del eje
 * @param[in] Orientation orientacion del eje
 * @param[out] wkt resultante de la generacion con formato:
 *  AXIS["<name>", NORTH | SOUTH | EAST | WEST | UP | DOWN | OTHER]
 **/
void SpatialReferenceEditionPart::GetAxisWkt(const std::string& Name, const std::string& Orientation,
                std::string& Wkt) {
   std::string axiswkt = "AXIS[\"" + Name + "\"," + Orientation + "]";
   Wkt = axiswkt;
}

/** Metodo que a partir de la clave y el valor del parametro obtiene el wkt
 *  correspondiente
 *  @param[in] Key clave(nombre) del parametro
 *  @param[in] Valor del parametro
 *  @param[out] Wkt wkt resultado: PARAMETER["<name>", <value>]
 */
void SpatialReferenceEditionPart::GetParameterWkt(const std::string& Key, const std::string& Value,
                     std::string& Wkt) {
   std::string wkt = "PARAMETER[\"" + Key + "\"," +  Value + "]";
   Wkt = wkt;
}
/** Metodo que a partir de la unidad del sistema devuelve el wkt asociado **/
std::string SpatialReferenceEditionPart::GetUnitWkt() {
   /** Opciones de unidades: metre|kilometre|degree **/
   std::string wkt;
   wxChoice* punitchoice =
         XRCCTRL(*(this->GetWidget()->GetWindow()), wxT(ID_UNITS_CHOICE),
               wxChoice);
   std::string unitselect = punitchoice->GetStringSelection().c_str();
   if (unitselect.compare("metre") == 0) {
      wkt = UNIT_METRE_WKT;
   } else if (unitselect.compare("kilometre") == 0) {
      wkt = UNIT_KILOMETRE_WKT;
   } else if (unitselect.compare("degree") == 0) {
      wkt = UNIT_DEGREE_WKT;
   }
   return wkt;
}

/** Metodo que en funcion de la autoridad y codigo definida genera el wkt correspondiente **/
std::string SpatialReferenceEditionPart::GetAuthorityWkt() {
   /** ej. AUTHORITY["EPSG",4326]] **/
   std::string aut = XRCCTRL(*(this->GetWidget()->GetWindow()),
         wxT(ID_AUTHORITY_NAME_TEXT), wxTextCtrl)->GetValue().c_str();
   std::string code = XRCCTRL(*(this->GetWidget()->GetWindow()),
         wxT(ID_AUTHORITY_CODE_TEXT), wxTextCtrl)->GetValue().c_str();
   return "AUTHORITY[\"" +  aut + "\"," + code + "]";
}

/** Genera el wkt correspondiente al sistema de referencia configurado en el editor */
std::string SpatialReferenceEditionPart::GetSpatialReferenceWkt(bool Projected) {
   std::string geoordatumwkt = GetAssociatedItemWkt(Projected);
   if (geoordatumwkt.empty())
      return "";
   bool checkvalue = XRCCTRL(*(this->GetWidget()->GetWindow()),
                  wxT(ID_USE_TWIN_AXES_CHECK), wxCheckBox)->GetValue();
   std::string axiswkt = checkvalue? GetTwinAxisWkt() : "";
   std::string unit = GetUnitWkt();
   std::string autstr = GetAuthorityWkt();
   std::string name = pCgu_->GetSelectedItem()->GetPrincipalAttribute()->GetValue();
   std::string wkt;
   if (Projected) {
      wkt = "PROJCS[";
   } else {
      wkt = "GEOGCS[";
   }
   wkt += "\"" + name + "\"" + ",";

   if (!geoordatumwkt.empty())
      wkt += geoordatumwkt + ",";

   wkt += autstr;

   if (!unit.empty())
      wkt +=  "," + unit;

   if (!axiswkt.empty())
      wkt += "," + axiswkt;

   if (Projected) {
      std::string projparamswkt = GetProjectionParametersWkt();
      wkt += "," + projparamswkt;
   }
   wkt += "]";
   return wkt;
}

/** Metodo auxiliar que se llama cuando se presiona el check de ejes gemelos **/
void SpatialReferenceEditionPart::OnTwinAxisCheck(const wxCommandEvent& Event) {
   bool checkvalue = XRCCTRL(*(this->GetWidget()->GetWindow()),
                  wxT(ID_USE_TWIN_AXES_CHECK), wxCheckBox)->GetValue();
   ConfigureTwinAxisControls(checkvalue);

}

/** Configura los controles graficos asociados a la configuracion de ejes
 *  gemelos en funcion de la activacion del Checkbox
 */
void SpatialReferenceEditionPart::ConfigureTwinAxisControls(bool Check) {
   GET_CONTROL(*(this->GetWidget()->GetWindow()),
         wxT(ID_TWIN1_NAME_TEXT), wxTextCtrl)->Enable(Check);
   GET_CONTROL(*(this->GetWidget()->GetWindow()),
         wxT(ID_TWIN2_NAME_TEXT), wxTextCtrl)->Enable(Check);
   GET_CONTROL(*(this->GetWidget()->GetWindow()),
         wxT(ID_TWIN1_ORIENTATION_CHOICE), wxChoice)->Enable(Check);
   GET_CONTROL(*(this->GetWidget()->GetWindow()),
         wxT(ID_TWIN2_ORIENTATION_CHOICE), wxChoice)->Enable(Check);
}

/** Obtiene el wkt del item asociado al sr que se esta configurando
 *  @return string con el wkt obtenido
 *  @return string vacio en caso de no poder obtener el wkt correcto
 */
std::string SpatialReferenceEditionPart::GetAssociatedItemWkt(bool Projected) {
   const LibraryItem* pitem = pDatumSelector_->GetSelectedItem();
   if (!pitem)
      return NULL;

   const LibraryItemAttribute* pasocattr = pitem->GetAttribute(
         GdalSrsItemOrigin::WKT_ATTRIBUTE_NAME);
   std::string geoordatumwkt = pasocattr ? pasocattr->GetValue() : "";
   if (!geoordatumwkt.empty() && SpatialReference::IsProjected(geoordatumwkt)
         && Projected) {
      switch (SHOWQUERY(GetWindow(),
                        _("El Sistema de referencia seleccionado es proyectado."
                  "Desea utilizar el geografico asociado?"),
            wxYES|wxNO|wxCANCEL|wxICON_QUESTION)) {
         case wxID_YES:
            geoordatumwkt = SpatialReference::GetProjGeoreferenceSpatialReference(
                  geoordatumwkt);
            break;
         case wxID_NO:
            geoordatumwkt = "";
            break;
         default:
            geoordatumwkt = "";
            break;
      }
   }
   return geoordatumwkt;
}
}  /** namespace suri */
