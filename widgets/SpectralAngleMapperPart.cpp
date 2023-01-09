/*! \file SpectralAngleMapperPart.cpp */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2014-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

// Includes Estandar
// Includes Suri
#include "SpectralAngleMapperPart.h"
#include "suri/messages.h"
#include "ClassificationRenderer.h"
#include "suri/BasicLibraryItemSelector.h"
#include "suri/LibraryClient.h"
#include "suri/LibraryManagerFactory.h"
#include "suri/Table.h"
#include "TablePart.h"
#include "DefaultTable.h"
#include "suri/MemoryDriver.h"
#include "suri/SpectralSignItemAttribute.h"
#include "suri/LibraryItemAttributeFactory.h"
#include "resources.h"

// Includes Wx
#include "wx/regex.h"
// Defines
// forwards

namespace suri {

const std::string SpectralAngleMapperPart::Clase = "Clase";
const std::string SpectralAngleMapperPart::Color = "Color";

/** \cond MACRO_IMPLEMENT_EVENT_OBJECT */
START_IMPLEMENT_EVENT_OBJECT(SpectralAngleMapperPartEvent, SpectralAngleMapperPart)
   IMPLEMENT_EVENT_CALLBACK(OnAddClassClick, OnAddClassClick(event), wxCommandEvent)
   IMPLEMENT_EVENT_CALLBACK(OnDeleteClassClick, OnDeleteClassClick(event), wxCommandEvent)
   IMPLEMENT_EVENT_CALLBACK(OnSamTexCtrlChanged, OnSamTexCtrlChanged(event), wxCommandEvent)
END_IMPLEMENT_EVENT_OBJECT
/** \endcond */

/** Constructor */
SpectralAngleMapperPart::SpectralAngleMapperPart(LibraryManager* pManager) :
      NEW_EVENT_OBJECT(SpectralAngleMapperPartEvent), pTable_(NULL),
      palgorithm_(new SpectralAngleMapperAlgorithm) {
   windowTitle_ = _(caption_CLASSIFICATION_OPTIONS);
   pClient_ = pManager ? new LibraryClient(pManager, LibraryManager::READONLY) : NULL;
   const Library* plib = pClient_->GetLibraryByCode(
         LibraryManagerFactory::SpectralSignLibraryCode);
   pItemSelector_ = new BasicLibraryItemSelector(
         this, plib->GetId(), pManager, "ID_LIBRARY_SELECTION_PANEL",
         "ID_LIBRARY_SELECTION_PANEL", true,
         UniversalGraphicalComponentPartInterface::ExtendedROWithFF);
}

/** Destructor */
SpectralAngleMapperPart::~SpectralAngleMapperPart() {
   DELETE_EVENT_OBJECT;
}

/** Implementacion del metodo de creacion de ventana de Widget */
bool SpectralAngleMapperPart::CreateToolWindow() {
   pToolWindow_ = wxXmlResource::Get()->LoadPanel(pParentWindow_, "ID_ANGLE_MAPPER_PARAMETERS");

   if (!pTable_)
      ConfigureFeatures();

   // Conecto los controles con sus eventos.
   GET_CONTROL(*(this->GetWidget()->GetWindow()),
         "ID_BITMAPBUTTON_ADD_COL", wxBitmapButton)->Connect(
         wxEVT_COMMAND_BUTTON_CLICKED,
         wxCommandEventHandler(SpectralAngleMapperPartEvent::OnAddClassClick), NULL,
         pEventHandler_);

   GET_CONTROL(*(this->GetWidget()->GetWindow()),
         "ID_BITMAPBUTTON_DELETE_COL", wxBitmapButton)->Connect(
         wxEVT_COMMAND_BUTTON_CLICKED,
         wxCommandEventHandler(SpectralAngleMapperPartEvent::OnDeleteClassClick), NULL,
         pEventHandler_);

   GET_CONTROL(*pToolWindow_,
         "ID_TEXTCTRL_SAM", wxTextCtrl)->Connect(
         wxEVT_COMMAND_TEXT_UPDATED,
         wxCommandEventHandler(SpectralAngleMapperPartEvent::OnSamTexCtrlChanged), NULL,
         pEventHandler_);

   if (pItemSelector_)
      AddControl(pItemSelector_->GetWidget(), "ID_SS_SELECTOR_PANEL");

   return true;
}

/** Configura la tabla */
void SpectralAngleMapperPart::ConfigureFeatures() {
   pTable_ = new DefaultTable();
   pTable_->SetDriver(new MemoryDriver);
   AppendColumns();
   pTablePart_ = new TablePart(pTable_, false, false, NULL, false);
   AddControl(pTablePart_->GetWidget(), "ID_CLASSES_TABLE_PANEL");
}

/** Agrega las columnas de la tabla */
void SpectralAngleMapperPart::AppendColumns() {
   pTable_->AppendColumn(Clase, Table::STRING, Table::VirtualFlag);
   pTable_->AppendColumn(Color, Table::COLOR_RGBA_HEXA,Table::VirtualFlag);
}

/** Agrega una clase a la tabla */
void SpectralAngleMapperPart::OnAddClassClick(wxCommandEvent &Event) {
   const LibraryItem* pItem = pItemSelector_->GetActiveItem();
   std::string itemvalue;
   if (pItem)
      itemvalue = pItem->GetAttribute(LibraryItemAttribute::PrincipalAttrName)->GetValue();
   if (!CanAddClass(itemvalue)) {
      SHOW_ERROR(message_FIELD_NAME_DUPLICATED);
      return;
   }
   wxArrayInt rows = pTablePart_->GetSelectedRows();
   size_t count = rows.GetCount();
   int maxrow = -1;
   for (size_t i = 0; i < count; i++) {
      if (rows.Item(i) > maxrow)
         maxrow = rows.Item(i);
   }
   int rowpos = -1;
   if (maxrow > 0) {
      rowpos = pTable_->GetRows() > 1 ? maxrow + 1 : -1;
      pTable_->InsertRow(maxrow + 1);
   } else {
      pTable_->AppendRow();
      rowpos = pTable_->GetRows() >= 1 ? pTable_->GetRows() - 1 : -1;
   }
   if (rowpos >= 0) {
      pTable_->SetCellValue(CLASS_COL, rowpos, itemvalue);
      std::string hexacolor = "FFFFFF";
      pTable_->SetCellValue(HEXCOLOR_COL, rowpos, hexacolor);
   }
   pTablePart_->GetWidget()->GetWindow()->Layout();
   pTablePart_->GetGridControl()->ForceRefresh();
   pToolWindow_->Fit();
}

/**
 * Verifica que la clase que se quiere agregar no este duplicada
 * @param[in] string con nombre del item principal
 * @return true si la clase no existe en la tabla, caso contrario false
 */
bool SpectralAngleMapperPart::CanAddClass(std::string ItemValue) {
   bool success = true;
   int count = pTable_->GetRows();
   for (int i = 0; i < count; ++i) {
      std::string colvalue;
      pTable_->GetCellValue(CLASS_COL, i, colvalue);
      if (colvalue.compare(ItemValue) == 0)
         success = false;
   }
   return success;
}

/** Elimina una clase de la tabla */
void SpectralAngleMapperPart::OnDeleteClassClick(wxCommandEvent &Event) {
   if (pTablePart_ && pTable_) {
      wxArrayInt srows = pTablePart_->GetSelectedRows();
      for (unsigned int r = 0; r < srows.size(); ++r) {
         pTable_->DeleteRow(srows[r]);
      }
   }
}

/**
 * Indica si el Part tiene cambios para salvar.
 * @return bool que indica si tuvo exito
 */
bool SpectralAngleMapperPart::HasChanged() {
   return modified_;
}

/** Salva los cambios realizados en el Part. */
bool SpectralAngleMapperPart::CommitChanges() {
   if (!pTable_->GetRows() > 0) {
      SHOW_ERROR(message_MISSING_CLASSES_ERROR);
      return false;
   }
   if (modified_ && pPal_) {
      modified_ = false;
      hasvaliddata_ = true;
   }
   return true;
}

/**
 * Restaura los valores originales del Part.
 * @return bool que indica si tuvo exito
 */
bool SpectralAngleMapperPart::RollbackChanges() {
   return true;
}

/** Inicializa el part */
void SpectralAngleMapperPart::SetInitialValues() {
   modified_ = true;
}

/**
 * Devuelve si la parte contiene datos validos.
 * Devuelve true : siempre.
 */
bool SpectralAngleMapperPart::HasValidData() {
   return hasvaliddata_;
}

/** Retorna la cantidad de clases */
int SpectralAngleMapperPart::GetClassCount() {
   return pTable_->GetRows();
}

/** Retorna las propiedades de la clase en la posicion indicada. Las Posiciones
 * van de 0 a GetClassCount()-1.**/
bool SpectralAngleMapperPart::GetClassProperties(
      std::vector<ClassInformation> &Properties) {
   // Sin clase.
   Properties.push_back(ClassInformation(ClassInformation::NoClassIndex));

   // Clase valor no valido.
   ClassInformation ndvclassproperties(ClassInformation::NDVIndex,
                                       ClassInformation::NDVName);
   Properties.push_back(ndvclassproperties);

   int rows = pTable_->GetRows();
   for (int i = 0; i < rows; ++i) {
      std::string value;
      VectorStyle::Color color;
      pTable_->GetCellValue(HEXCOLOR_COL, i, value);
      color.SetColorFromHexaString(value);
      pTable_->GetCellValue(CLASS_COL, i, value);
      // TODO(Gabriel #6482): Se utiliza i+1 ya que el 0 se considera no valido
      ClassInformation cinfo(i+1, value, color);
      Properties.push_back(cinfo);
   }
   return true;
}

/** Retorna el nodo que se debe agregar al elemento que se renderiza */
wxXmlNode* SpectralAngleMapperPart::GetClassificationRendererNode(
      RasterElement* pRasterElement) {
   ClassificationRenderer::Parameters params;
   params.pAlgorithm_ = palgorithm_;
   return ClassificationRenderer::GetXmlNode(params);
}

/** Retorna el nombre del algoritmo que se usara para clasificar */
std::string SpectralAngleMapperPart::GetAlgorithmName() {
   return _(caption_SPECTRAL_ANGLE_MAPPER);
}

/** Retorna un puntero al algoritmo de clasificacion creado por part */
ClassificationAlgorithmInterface* SpectralAngleMapperPart::GetClassificationManager() {
   return palgorithm_;
}

/**
 *  Metodo que indica la seleccion de bandas aciva para la clasificacion.
 **/
void SpectralAngleMapperPart::SetSelectedBands(
      const std::vector<int>& SelectedBands) {
   selectedBands_ = SelectedBands;
   std::vector<SpectralAngleMapperAlgorithm::ClassSpectralInfo> classes;
   CreateClassificationClasses(classes);
   // se actualizan las clases en funcion de la seleccion de bandas realizadas
   palgorithm_->SetClasses(classes);
}

/**
 *  Metodo que a partir de las clases definidas en la tabla genera las
 *  clases en una estructura compatible con el algoritmo de clasificacion
 */
void SpectralAngleMapperPart::CreateClassificationClasses(
      std::vector<SpectralAngleMapperAlgorithm::ClassSpectralInfo>& Classes) {
   wxString strvalue = USE_CONTROL(*pToolWindow_, wxT("ID_TEXTCTRL_SAM"), wxTextCtrl,
                                 GetValue(), wxT(message_NOT_APPLICABLE));
   double value;
   strvalue.ToDouble(&value);
   for (int r = 0; r < pTable_->GetRows(); ++r) {
      SpectralAngleMapperAlgorithm::ClassSpectralInfo aclass;
      // TODO(Gabriel #6482): Se utiliza i+1 ya que el 0 se considera no valido
      aclass.classIndex_ = r+1;
      aclass.maxangle_ = value;
      std::string value;
      pTable_->GetCellValue(CLASS_COL, r, value);
      LoadClassificationBandInfo(aclass, value);
      Classes.push_back(aclass);
   }
}

/**
 * Metodo auxiliar que a partir de la definicion de reflectancias de una firma espectral configura
 * la reflectancia correspondiente a cada banda
 * @param[out] Class clase en la que se guarda la configuracion de
 * firmas espectrales para cada banda
 */
void SpectralAngleMapperPart::LoadClassificationBandInfo(
      SpectralAngleMapperAlgorithm::ClassSpectralInfo& Class, std::string Principal) {
   const Library* plib = pClient_->GetLibraryByCode(
         LibraryManagerFactory::SpectralSignLibraryCode);
   int band = 0;
   SpectralAngleMapperAlgorithm::BandSpectralInfo aband;
   if (!plib)
      return;
   const LibraryItem* pitem = plib->GetItemByPrincipal(Principal);
   if (!pitem)
      return;
   LibraryItem::MapAttribute attributes = pitem->GetAttributes();
   LibraryItem::MapAttribute::iterator it = attributes.begin();
   for (; it != attributes.end(); ++it) {
      LibraryItemAttribute* pattr = it->second;
      if (pattr->GetName().find(LibraryItemAttributeFactory::SpectralSignString)
            != std::string::npos) {
         const SpectralSignItemAttribute* pcomplex =
               LibraryItemAttributeFactory::CreateSpectralSignItemAttribute(pattr);
         aband.band_ = band;
         band++;
         aband.reflectance_ = pcomplex->GetReflectanceValue();
         Class.bands_.push_back(aband);
      }
   }
}

/**
 * Valida que la cantidad de registros en la firma se la misma
 * que bandas en el raster seleccionado.
 */
bool SpectralAngleMapperPart::HasValidBandsAndClasses() const {
   bool retval = false;

   for (int row = 0, lenrow = pTable_->GetRows(); row < lenrow; ++row) {
      std::string value;
      pTable_->GetCellValue(CLASS_COL, row, value);

      const Library* plib = pClient_->GetLibraryByCode(
            LibraryManagerFactory::SpectralSignLibraryCode);

      if (plib != NULL) {
         const LibraryItem* pitem = plib->GetItemByPrincipal(value);
         if (pitem != NULL) {
            unsigned int attrcount = 0;
            LibraryItem::MapAttribute attributes = pitem->GetAttributes();
            LibraryItem::MapAttribute::iterator itattr = attributes.begin();
            for ( ; itattr != attributes.end(); ++itattr) {
               if (itattr->second->GetName().find(
                     LibraryItemAttributeFactory::SpectralSignString)
                     != std::string::npos) {
                  ++attrcount;
               }
            }
            retval = (selectedBands_.size() == attrcount);
         }
      }
   }

   return retval;
}

/** Accion de ingresar un valor en el TextCtrl de distancia */
void SpectralAngleMapperPart::OnSamTexCtrlChanged(wxCommandEvent &Event) {
   wxRegEx Expression(("^[0-9|.]*$"), wxRE_ICASE);
   if (!Expression.Matches(Event.GetString())) {
      wxTextCtrl* ptext = XRCCTRL(*pToolWindow_, wxT("ID_TEXTCTRL_SAM"), wxTextCtrl);
      wxString aux = Event.GetString();
      ptext->SetValue(aux.RemoveLast());
      SHOW_ERROR(message_FLOAT_INPUT_ERROR);
   }
}
/** Retorna el icono de la herramienta */
void SpectralAngleMapperPart::GetWindowIcon(wxBitmap& ToolBitmap, int X, int Y) const {
   GET_BITMAP_RESOURCE(tool_SAM_32, ToolBitmap);
}

/**
 * Metodo que sirve para verificar si la clasificacion es valida.
 */
bool SpectralAngleMapperPart::IsValid() {
   if (!HasValidBandsAndClasses()) {
      SHOW_ERROR(message_WRONG_BAND_COUNT);
      return false;
   }
   return true;
}

} /** namespace suri */
