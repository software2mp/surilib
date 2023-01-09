/*! \file ClassFussionParametersPart.cpp */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2012-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

// Includes Estandar
#include <string>
#include <map>
#include <vector>

// Includes Suri
#include "resources.h"
#include "DefaultTable.h"
#include "wxGenericTableBase.h"
#include "ClassificationRenderer.h"
#include "ClassFussionParametersPart.h"
#include "ClassifiedRasterDatasourceValidator.h"
#include "ClassifiedRasterDatasourceManipulator.h"
#include "suri/messages.h"
#include "suri/AuxiliaryFunctions.h"
#include "suri/ClassificationProcess.h"

// Includes Wx
#include "wx/clrpicker.h"

// Defines

// forwards
class wxCommandEvent;

namespace suri {

/** \cond MACRO_IMPLEMENT_EVENT_OBJECT */
START_IMPLEMENT_EVENT_OBJECT(ClassFusionEvent, ClassFussionParametersPart)
IMPLEMENT_EVENT_CALLBACK(
      OnAddClassButtonEventHandler, OnAddClassButtonEventHandler(event), wxCommandEvent)
END_IMPLEMENT_EVENT_OBJECT
/** \endcond */


/**
 * Constructor
 * @param[in] pElement elemento sobre el cual se muestra la informacion del part
 */
ClassFussionParametersPart::ClassFussionParametersPart(Element* pElement) :
      pElement_(pElement), NEW_EVENT_OBJECT(ClassFusionEvent) {
   windowTitle_ = _(caption_CLASS_FUSION);
}

/** dtor **/
ClassFussionParametersPart::~ClassFussionParametersPart() {
}

/** Retorna las propiedades de los pixels que no tienen clase */
bool ClassFussionParametersPart::GetNonClassifiedPixelInfo(
      ClassInformation &Properties) {
   return true;
}

/**
 * Metodo auxiliar para la configuracion del evento al boton de agregar clase
 */
void ClassFussionParametersPart::ConfigureButtonEvent() {
   GET_CONTROL(*(this->GetWidget()->GetWindow()),
         "ID_ADD_CLASS_BUTTON", wxBitmapButton)->Connect(
            wxEVT_COMMAND_BUTTON_CLICKED,
            wxCommandEventHandler(
                  ClassFusionEvent::OnAddClassButtonEventHandler),
            NULL, pEventHandler_);
}

/**
 * Crea la ventana de la parte
 * @return true si pudo crear la ventana
 */
bool ClassFussionParametersPart::CreateToolWindow() {
   pToolWindow_ = wxXmlResource::Get()->LoadPanel(pParentWindow_,
                                                  wxT("ID_CLASS_FUSION_PANEL"));
   ClassMappingPart::MappingTable map;
   std::vector< ClassMappingPart::ChoiceValues> options;
   bool insertedvalues = InsertClassesToTable(map, options);
   TableToClustersXmlNode(map);
   pClassMappingPart_ = new ClassMappingPart(pElement_,
                                          std::vector<ClassMappingPart::MappingTable>(1, map),
                                          options, ClassMappingPart::OriginField,
                                          std::vector<std::string>(1,
                                          ClassMappingPart::DestinationField));

   ClassifiedRasterDatasourceManipulator manipulator;
   DatasourceInterface* pdatasource = DatasourceInterface::Create("RasterDatasource",
                                                                  pElement_);
   // Agrego la clase a la lista de clases creadas
   lastindex_ = manipulator.GetClassInformationNewId(pdatasource);
   AddControl(pClassMappingPart_->GetWidget(), wxT("ID_CLASS_MAPPER_PANEL"));
   ConfigureButtonEvent();
   return insertedvalues;
}

/** Retorna el icono de la herramienta */
void ClassFussionParametersPart::GetWindowIcon(wxBitmap& ToolBitmap, int X, int Y) const {
   GET_BITMAP_RESOURCE(icon_CLASS_FUSSION, ToolBitmap);
}

/** Retorna el nombre del algoritmo que se usara para clasificar */
std::string ClassFussionParametersPart::GetAlgorithmName() {
   return "Fusion de Clases";
}

/** Retorna un puntero al algoritmo de clasificacion creado por part */
ClassificationAlgorithmInterface* ClassFussionParametersPart::GetClassificationManager() {
   return &algorithm_;
}

/** Indica si el Part tiene cambios para salvar. */
bool ClassFussionParametersPart::HasChanged() {
   return pClassMappingPart_->HasChanged();
}

/** Salva los cambios realizados en el Part. */
bool ClassFussionParametersPart::CommitChanges() {
   pClassMappingPart_->CommitChanges();
   std::vector<ClassMappingPart::MappingTable> map = pClassMappingPart_->GetResultMappingTable();
   if (map.empty())
      return false;
   TableToClustersXmlNode(map[0]);
   return true;
}

/** Restaura los valores originales del Part. */
bool ClassFussionParametersPart::RollbackChanges() {
   pClassMappingPart_->RollbackChanges();
   // por un bug existente
   SetInitialValues();
   return true;
}

/** Inicializa el part */
void ClassFussionParametersPart::SetInitialValues() {
   pClassMappingPart_->SetInitialValues();
}

/** Actualiza el estado de la parte */
void ClassFussionParametersPart::Update() {
}

/** Retorna la cantidad de clases */
int ClassFussionParametersPart::GetClassCount() {
   return 0;
}

/**
 * Devuelve las clases resultantes de la fusion de clases realizada.
 * @return true
 */
bool ClassFussionParametersPart::GetClassProperties(
      std::vector<ClassInformation> &Properties) {
   Properties = resultclasses_;

   /** Se agrega a SIN CLASE a las clases resultado.**/
   DatasourceInterface* pdatasource = DatasourceInterface::Create("RasterDatasource",
                                                                  pElement_);

   ClassifiedRasterDatasourceValidator validator;
   ClassifiedRasterDatasourceManipulator manipulator;
   ClassInformation noclassinformation = manipulator.GetClassInformation(
         pdatasource, ClassInformation::NoClassIndex);
   Properties.push_back(noclassinformation);

   // Clase valor no valido.
   ClassInformation ndvclassproperties(ClassInformation::NDVIndex,
                                       ClassInformation::NDVName);
   Properties.push_back(ndvclassproperties);

   return true;
}

/** Retorna el nodo que se debe agregar al elemento que se renderiza
 * @return wxXmlNode* referencia al nodo de clasificacion que corresponde al algoritmo
 * de fusion*/
wxXmlNode* ClassFussionParametersPart::GetClassificationRendererNode(
      RasterElement* pRasterElement) {
      // Genero nodo de algoritmo
   ClassificationRenderer::Parameters params;
   params.pAlgorithm_ = &algorithm_;
   return ClassificationRenderer::GetXmlNode(params);
}

/** Evento que se ejecuta al hacer click sobre boton de agregar clase
 * Agrega la clase creada al choice del mapping part
 * @param[in] Event evento de wx
 */
void ClassFussionParametersPart::OnAddClassButtonEventHandler(wxCommandEvent &Event) {
   if (pClassMappingPart_->IsEditing()) return;
   wxTextCtrl* ptextctrl = XRCCTRL(*(this->GetWidget()->GetWindow()),
                           wxT("ID_NEW_CLASS_NAME_TEXT"), wxTextCtrl);
   std::string value = ptextctrl->GetValue().c_str();
   if (!pClassMappingPart_->AddChoiceOption(value)) {
      SHOW_ERROR(message_ADD_FUSSION_CLASS_ERROR);
      return;
   }
   ptextctrl->Clear();
   wxColourPickerCtrl* pcolourpickerctrl = XRCCTRL(*(this->GetWidget()->GetWindow()),
                            wxT("ID_NEW_CLASS_COLOURCTRL"), wxColourPickerCtrl);
   unsigned char red = pcolourpickerctrl->GetColour().Red();
   unsigned char green = pcolourpickerctrl->GetColour().Green();
   unsigned char blue = pcolourpickerctrl->GetColour().Blue();
   unsigned char alpha = pcolourpickerctrl->GetColour().Alpha();
   VectorStyle::Color color = VectorStyle::Color(red, green, blue, alpha);
   ClassInformation classinformation(lastindex_, value, color);
   lastindex_++;
   ClassMappingPair classpair(value, classinformation);
   classmapping_.insert(classpair);
   pClassMappingPart_->AppendClass(value);
}

/** Convierte el nodo xml de clasificacion en un mapa con el nombre de la clase
 *  como clave y valor (se utiliza de esta forma para la carga inicial en el part
 *  de mapeo entre clases
 *  @return true si el contenido del elemento es valido como para generar el mapeo
 *  @return false en caso contrario
 */
bool ClassFussionParametersPart::InsertClassesToTable(
      ClassMappingPart::MappingTable &MappingTable,
      std::vector<ClassMappingPart::ChoiceValues> &ClassesMap) {
   ClassMappingPart::ChoiceValues ClassesNames;
   DatasourceInterface* pdatasource =
         DatasourceInterface::Create("RasterDatasource", pElement_);
   ClassifiedRasterDatasourceValidator validator;
   if (!pdatasource || !validator.IsValid(pdatasource))
      return false;
   ClassifiedRasterDatasourceManipulator manipulator;
   std::vector<ClassInformation> classes = manipulator.GetClassInformationList(
         pdatasource);
   std::vector<ClassInformation>::const_iterator it = classes.begin();
   for (; it != classes.end(); ++it) {
      ClassInformation classiformation = *it;
      long index = classiformation.GetIndex();
      std::string classname = classiformation.GetName();
      ClassesNames.push_back(classname);
      if (index != ClassInformation::NoClassIndex && index != ClassInformation::NDVIndex) {
         ClassMappingPart::MappingTablePair pair(classname, classname);
         MappingTable.insert(pair);
      }
      ClassMappingPair classpair(classname, classiformation);
      classmapping_.insert(classpair);
   }
   ClassesMap.push_back(ClassesNames);
   return true;
}

/** Convierte el contenido de la tabla en un nodo xml
 * para la configuracion del algoritmo*/
bool ClassFussionParametersPart::TableToClustersXmlNode(
      const ClassMappingPart::MappingTable& MappingTable) {
   resultclasses_.clear();
   ClassFussionAlgorithm::FussionClasesTables fussiontable;
   ClassMappingPart::MappingTable::const_iterator cit = MappingTable.begin();
   for ( ; cit != MappingTable.end(); ++cit) {
      ClassMapping::const_iterator indexit = classmapping_.find(cit->first);
      ClassMapping::const_iterator fussionit = classmapping_.find(cit->second);
      if (indexit != classmapping_.end() && fussionit != classmapping_.end()) {
         ClassFussionAlgorithm::IndexValue classindex  = indexit->second.GetIndex();
         ClassFussionAlgorithm::IndexValue fussionindex = fussionit->second.GetIndex();
         ClassFussionAlgorithm::FussionClasesTablesPair pair(classindex, fussionindex);
         fussiontable.insert(pair);
         resultclasses_.push_back(fussionit->second);
      }
   }
   algorithm_.SetFussionTable(fussiontable);
   return true;
}

} /** namespace suri */
