/*! \file ThresholdClassSelectionPart.cpp */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2013-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

// Includes Estandar
#include <vector>
#include <string>

// Includes Suri
#include "resources.h"
#include "ThresholdClassSelectionPart.h"
#include "TablePart.h"
#include "suri/MemoryDriver.h"
#include "DefaultTable.h"
#include "TablePartConfigurator.h"
#include "ThresholdClassificationAlgorithm.h"
#include "ClassificationRenderer.h"

// Includes Wx
// Defines
// forwards

namespace suri {
/** \cond MACRO_IMPLEMENT_EVENT_OBJECT */
START_IMPLEMENT_EVENT_OBJECT(ThresholdClassEventHandler, ThresholdClassSelectionPart)
      IMPLEMENT_EVENT_CALLBACK(OnAddClassClick, OnAddClassClick(event), wxCommandEvent)
      IMPLEMENT_EVENT_CALLBACK(OnRemoveClassClick, OnRemoveClassClick(event), wxCommandEvent)
      IMPLEMENT_EVENT_CALLBACK(OnEditClassClick, OnEditClassClick(event), wxCommandEvent)
END_IMPLEMENT_EVENT_OBJECT
/** \endcond */

/** ctor **/
ThresholdClassSelectionPart::ThresholdClassSelectionPart(DataViewManager* pDataView,
                                                         Element* pInputElement) :
      pEventHandler_(new ThresholdClassEventHandler(this)),
                     pInputElement_(pInputElement), pTable_(NULL), pTablePart_(NULL),
      pDataView_(pDataView), palgorithm_(new ThresholdClassificationAlgorithm) {
   windowTitle_ = _(caption_CLASSIFICATION_OPTIONS);
}

/** dtor **/
ThresholdClassSelectionPart::~ThresholdClassSelectionPart() {
   delete pEventHandler_;
   delete pTable_;
}

/** Indica si el Part tiene cambios para salvar. */
bool ThresholdClassSelectionPart::HasChanged() {
   return modified_ || pTablePart_->HasChanged();
}

/** Salva los cambios realizados en el Part. */
bool ThresholdClassSelectionPart::CommitChanges() {
   // TODO(Gabriel - TCK #5057): Unificar informacion de structs
   if (pTablePart_->HasChanged()) {
      pTablePart_->CommitChanges();
      LoadClassesFromTable();
   }
   std::vector<ThresholdClassificationAlgorithm::ClassThresholdInfo> classes;
   CreateClassificationClasses(classes);
   palgorithm_->SetClasses(classes);
   modified_ = false;
   return true;
}

/** Restaura los valores originales del Part. */
bool ThresholdClassSelectionPart::RollbackChanges() {
   if (pTablePart_->HasChanged()) {
      pTablePart_->RollbackChanges();
      LoadClassesFromTable();
      modified_ = true;
   }
   return true;
}

/** Inicializa el part */
void ThresholdClassSelectionPart::SetInitialValues() {
}

/** Actualiza el estado de la parte */
void ThresholdClassSelectionPart::Update() {
}

/** Retorna el nombre del algoritmo que se usara para clasificar */
std::string ThresholdClassSelectionPart::GetAlgorithmName() {
   return _(caption_THRESHOLD_CLASS);
}

/** Retorna un puntero al algoritmo de clasificacion creado por part */
ClassificationAlgorithmInterface* ThresholdClassSelectionPart::
                                                GetClassificationManager() {
   return palgorithm_;
}

/** Implementacion del metodo de creacion de ventana de Widget */
bool ThresholdClassSelectionPart::CreateToolWindow() {
   pToolWindow_ = wxXmlResource::Get()->LoadPanel(pParentWindow_,
                                                  wxT("ID_THRESHOLD_CLASS_SELECT_PANEL"));
   if (!pToolWindow_)
      return false;
   // configuracion de evento para los botones
   GET_CONTROL(*(this->GetWidget()->GetWindow()),
         "ID_REMOVE_CLASS_BTN", wxBitmapButton)->Connect(wxEVT_COMMAND_BUTTON_CLICKED,
   wxCommandEventHandler(ThresholdClassEventHandler::OnRemoveClassClick),
                                                         NULL, pEventHandler_);
   GET_CONTROL(*(this->GetWidget()->GetWindow()),
         "ID_ADD_CLASS_BTN", wxBitmapButton)->Connect(wxEVT_COMMAND_BUTTON_CLICKED,
   wxCommandEventHandler(ThresholdClassEventHandler::OnAddClassClick),
                                                      NULL, pEventHandler_);
   GET_CONTROL(*(this->GetWidget()->GetWindow()),
         "ID_EDIT_CLASS_BTN", wxBitmapButton)->Connect(wxEVT_COMMAND_BUTTON_CLICKED,
   wxCommandEventHandler(ThresholdClassEventHandler::OnEditClassClick),
                                                      NULL, pEventHandler_);

   ConfigureClassTable();

   AddControl(pTablePart_->GetWidget(), "ID_CLASS_TABLE_PANEL");
   return true;
}

/** Metodo auxiliar que configura la tabla para la generacion de clases por umbrales **/
void ThresholdClassSelectionPart::ConfigureClassTable() {
   pTable_ = new DefaultTable();
   MemoryDriver* pdriver = new MemoryDriver();
   pTable_->SetDriver(pdriver);
   std::string classfield = "Clase";
   pTable_->AppendColumn(classfield.c_str(), Table::STRING);
   pTable_->AppendColumn("Color", Table::COLOR_RGBA_HEXA);
   TablePartConfigurator config;
   Option options;
   options.SetOption(TablePartConfigurator::RoColumnsKeyOption, classfield);
   options.SetOption(TablePartConfigurator::TableEditorKeyOption,
                     TablePartConfigurator::VectorTableEditorOption);
   pTablePart_ = config.Initialize(pDataView_, pTable_, options);

}
/** Retorna la cantidad de clases */
int ThresholdClassSelectionPart::GetClassCount() {
   return classes_.size();
}
/** Retorna las propiedades de la clase en la posicion indicada. Las Posiciones
 * van de 0 a GetClassCount()-1.**/
bool ThresholdClassSelectionPart::GetClassProperties(
      std::vector<ClassInformation> &Properties) {
   //
   Properties.push_back(ClassInformation(ClassInformation::NoClassIndex));

   //
   ClassInformation ndvclassproperties(ClassInformation::NDVIndex,
                                       ClassInformation::NDVName);
   Properties.push_back(ndvclassproperties);

   //
   ClassVector::iterator it =    classes_.begin();
   for (; it != classes_.end(); ++it) {
      VectorStyle::Color color;
      color.SetColorFromHexaString(it->classhexacolor_);
      ClassInformation cinfo(it->classindex_, it->classname_, color);
      Properties.push_back(cinfo);
   }
   return true;
}
/** Retorna el nodo que se debe agregar al elemento que se renderiza */
wxXmlNode* ThresholdClassSelectionPart::GetClassificationRendererNode(
      RasterElement* pRasterElement) {
   ClassificationRenderer::Parameters params;
   params.pAlgorithm_ = palgorithm_;
   return ClassificationRenderer::GetXmlNode(params);
}
/**
 * Devuelve el icono de la parte
 * @param[out] ToolBitmap icono de la ventana
 * @param[in] X ancho del icono
 * @param[in] Y alto del icono
 */
void ThresholdClassSelectionPart::GetWindowIcon(wxBitmap& ToolBitmap, int X,
                                                 int Y) const {
   GET_BITMAP_RESOURCE(tool_THRESHOLD_CLASSIFICATION_32, ToolBitmap);
}
/**
 *  Metodo que se llama cuando se genera el evento de click sobre el boton
 *  de remover clase
 */
void ThresholdClassSelectionPart::OnRemoveClassClick(wxCommandEvent& Event) {
   wxArrayInt rows = pTablePart_->GetSelectedRows();
   for (size_t r = 0 ; r < rows.size() ; ++r) {
      ClassVector::iterator it = classes_.begin();
      it = it + r;
      it = classes_.erase(it);
      pTable_->DeleteRow(rows[r]);
      modified_ = true;
   }
}
/**
 *  Metodo que se llama cuando se genera el evento de click sobre el boton
 *  de agregar clase
 */
void ThresholdClassSelectionPart::OnAddClassClick(wxCommandEvent& Event) {
   ThresholdClassDefinitionWidget::ThresholdClassInformation classinfo;
   ThresholdClassDefinitionWidget* pclasswidget = new ThresholdClassDefinitionWidget(
         pDataView_, pInputElement_, classinfo);
   pclasswidget->SetProcessLayer(pPal_);
   // TODO(Gabriel - #5057): No funciona correctamente el modal. Es posible
   // cerrar la ventana de proceso y no cerrar la de umbrales. Esto hace que
   // quede la ventana vacia abierta y no sea posible cerrarla.
   // Workaround: habilitar boton para cerrar la ventana
   if (pclasswidget->CreateTool() && pclasswidget->GetFrame()->EnableCloseButton()
         && pclasswidget->ShowModal() == wxID_OK) {
      // para que empiece a contar en 1 y no en 0, el 0 esta reservado para noclase
      classinfo.classindex_ = classes_.size() + 1;
      classes_.push_back(classinfo);
      pTable_->AppendRow();
      pTable_->SetCellValue(0, pTable_->GetRows() - 1, classinfo.classname_);
      modified_ = true;
   }
}
/**
 * Metodo que se llama cuando se genera el evento de click sobre el boton
 * de editar clase
 */
void ThresholdClassSelectionPart::OnEditClassClick(wxCommandEvent& Event) {
   wxArrayInt rows = pTablePart_->GetSelectedRows();
   if (rows.size() > 0) {
      ThresholdClassDefinitionWidget::
      ThresholdClassInformation classinfo = classes_.at(rows[0]);
      ThresholdClassDefinitionWidget* pclasswidget =
            new ThresholdClassDefinitionWidget(pDataView_, pInputElement_, classinfo);
        pclasswidget->SetProcessLayer(pPal_);
        // TODO(Gabriel - #5057): No funciona correctamente el modal. Es posible
        // cerrar la ventana de proceso y no cerrar la de umbrales. Esto hace que
      // quede la ventana vacia abierta y no sea posible cerrarla.
      // Workaround: habilitar boton para cerrar la ventana
      if (pclasswidget->CreateTool() && pclasswidget->GetFrame()->EnableCloseButton()
            && pclasswidget->ShowModal() == wxID_OK) {
         ClassVector::iterator it =    classes_.begin() + rows[0];
         if (it != classes_.end()) {
            it = classes_.erase(it);
         }
         // reemplazo la informacion de la clase
         classes_.insert(it, classinfo);
         modified_ = true;
      }
   }
}

/**
 * Metodo que indica la seleccion de bandas aciva para la clasificacion.
 *  Al recibirse una nueva seleccion de bandas es necesario actualizar las bandas
 *  que participan en la clasificacion por umbrales para que esto sea compatible
 *  con las bandas que llegan al renderizador en el momento de clasificar
 **/
void ThresholdClassSelectionPart::SetSelectedBands(
      const std::vector<int>& SelectedBands) {
   selectedBands_ = SelectedBands;
   std::vector<ThresholdClassificationAlgorithm::ClassThresholdInfo> classes;
   CreateClassificationClasses(classes);
   // se actualizan las clases en funcion de la seleccion de bandas realizadas
   palgorithm_->SetClasses(classes);
}

/**
 *  Metodo que a partir de las clases definidas en la tabla genera las
 *  clases en una estructura compatible con el algoritmo de clasificacion
 */
void ThresholdClassSelectionPart::CreateClassificationClasses(
      std::vector<ThresholdClassificationAlgorithm::ClassThresholdInfo>& Classes) {
   // En caso de que se haya configurado  la seleccion de bandas
   // crea las clases ordenando por los indices de la seleccion
   ClassVector::iterator it = classes_.begin();
   for (; it != classes_.end(); ++it) {
      ThresholdClassificationAlgorithm::ClassThresholdInfo aclass;
      LoadClassificationBandInfo(*it, aclass);
      Classes.push_back(aclass);

   }
}

/**
 * Metodo auxiliar que a partir de la definicion de umbrales de una clase configura
 * los umbrales en la estructura de clase del algoritmo a utilizar.
 * @param[in] ClassIn definicion de clase de la que se extrae la informacion de umbrales
 * @param[out] Class clase en la que se guarda la configuracion de umbrales
 */
void ThresholdClassSelectionPart::LoadClassificationBandInfo(
      ThresholdClassDefinitionWidget::ThresholdClassInformation& ClassIn,
      ThresholdClassificationAlgorithm::ClassThresholdInfo& Class) {
   if (selectedBands_.empty()) {
      std::vector<ThresholdClassDefinitionWidget::ThresholdBandInformation>::iterator bit =
            ClassIn.bandsThreshold_.begin();
      Class.classIndex_ = ClassIn.classindex_;
      for (; bit != ClassIn.bandsThreshold_.end(); ++bit) {
         ThresholdClassificationAlgorithm::BandThresholdInfo aband;
         aband.band_ = bit->bandindex_;
         aband.min_ = bit->min_;
         aband.max_ = bit->max_;
         Class.bands_.push_back(aband);
      }
   } else {
      Class.classIndex_ = ClassIn.classindex_;
      std::vector<int>::iterator it = selectedBands_.begin();
      for (; it != selectedBands_.end(); ++it) {
         bool bandfound = false;
         std::vector<ThresholdClassDefinitionWidget::ThresholdBandInformation>::iterator bit =
               ClassIn.bandsThreshold_.begin();
         for (; !bandfound && bit != ClassIn.bandsThreshold_.end(); ++bit) {
            bandfound = (bit->bandindex_ == *it);
            if (bandfound)
               break;
         }
         if (bandfound) {
            ThresholdClassificationAlgorithm::BandThresholdInfo aband;
            aband.band_ = bit->bandindex_;
            aband.min_ = bit->min_;
            aband.max_ = bit->max_;
            Class.bands_.push_back(aband);
         }
      }
   }
}

/**
 *  Metodo que actualiza la definicion de las clases en funcion de lo que se
 *  encuentra configurado en la tabla
 */
void ThresholdClassSelectionPart::LoadClassesFromTable() {
   // Actualiza los nombres de las clases y los colores de las mismas
   for (int r = 0; r < pTable_->GetRows(); ++r) {
      std::string classname;
      pTable_->GetCellValue(0, r, classname);
      std::string hexacolor;
      pTable_->GetCellValue(1, r, hexacolor);
      if ( static_cast<int>(classes_.size()) > r) {
         classes_.at(r).classname_ = classname;
         classes_.at(r).classhexacolor_ = hexacolor;
      }
   }
}
} /** namespace suri */
