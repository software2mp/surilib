/*! \file CategorizedVectorStylePart.cpp */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2013-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

// Includes Estandar
#include <map>
#include <utility>
#include <set>
#include <string>
// Includes Suri
#include "CategorizedVectorStylePart.h"
#include "TablePartConfigurator.h"
#include "DefaultTable.h"
#include "TablePart.h"
#include "suri/MemoryDriver.h"
#include "suri/Option.h"
#include "VectorLayer.h"
#include "CategorizedVectorRenderer.h"
#include "suri/xmlnames.h"
#include "suri/VectorElement.h"
#include "VectorStyleClassGenerationWidget.h"
#include "suri/AuxiliaryFunctions.h"

// Includes Wx
// Defines
#define ID_CLASS_TABLE_PANEL "ID_CLASS_TABLE_PANEL"
#define STYLE_COL_NUMBER 2
#define LABEL_COL_NUMBER 0
#define CLASS_COL_NUMBER 1
#define MAX_PROP "max"
#define MIN_PROP "min"
#define CLASS_GEN_METHOD_PROP "class_method"
#define FIELD_PROP "field"

// forwards

namespace suri {
namespace ui {

/** \cond MACRO_IMPLEMENT_EVENT_OBJECT */
START_IMPLEMENT_EVENT_OBJECT(CategorizedVectorStylePartHandler, CategorizedVectorStylePart)
      IMPLEMENT_EVENT_CALLBACK(OnAddClassClick, OnAddClassClick(event), wxCommandEvent)
      IMPLEMENT_EVENT_CALLBACK(OnRemoveClassClick, OnRemoveClassClick(event), wxCommandEvent)
      IMPLEMENT_EVENT_CALLBACK(OnClassGenerationClick,
                               OnClassGenerationClick(event), wxCommandEvent)
END_IMPLEMENT_EVENT_OBJECT
/** \endcond */

CategorizedVectorStylePart::CategorizedVectorStylePart(
      DataViewManager* pDataViewManager, VectorLayer *pVectorLayer) :
      pTablePart_(NULL), pTable_(NULL),
      pDataViewManager_(pDataViewManager), pVectorLayer_(pVectorLayer),
      pEventHandler_(new CategorizedVectorStylePartHandler(this)),
      pClassGenerationWidget_(NULL) {
}

CategorizedVectorStylePart::~CategorizedVectorStylePart() {
   delete pEventHandler_;
   delete pTable_;
}

/** Indica si el Part tiene cambios para salvar. */
bool CategorizedVectorStylePart::HasChanged() {
   return pTablePart_->HasChanged();
}
/** Salva los cambios realizados en el Part. */
bool CategorizedVectorStylePart::CommitChanges() {
   pTablePart_->CommitChanges();
   wxXmlNode* pnode = GetCategorizedRenderNodeFromTable();
   if (!pnode) {
      SHOW_ERROR(_(message_VECTOR_CATEGORIZED_STYLE_INVALID));
      return false;
   }
   pVectorLayer_->GetElement()->AddNode(pVectorLayer_->GetElement()->GetNode(""), pnode,
                                        true);
   pVectorLayer_->GetElement()->SetChanged();
   pVectorLayer_->GetElement()->SendViewerUpdate();
  
   return true;
}

/** Metodo auxiliar que genera el nodo xml para la renderizacion vectorial
 *  clasificada a partir de la configuracion realizada por el usuario
 */
wxXmlNode* CategorizedVectorStylePart::GetCategorizedRenderNodeFromTable() {
   if (!pTable_)
      return NULL;
   vector::render::CategorizedVectorRenderer::CategorizedParameters params;
   VectorElement* pelement = dynamic_cast<VectorElement*>(pVectorLayer_->GetElement());
   params.vectorUrl_ = pelement->GetUrl().c_str();
   size_t activelayer = pelement->GetActiveLayer();
   params.activelayer_ = activelayer;
   params.layersSR_.insert(
         std::make_pair(activelayer, pelement->GetSpatialReference().c_str()));
   /** set para almacenar las condiciones y ver que no se repita ninguna **/
   // TODO: Agregar en el nodo XML la configuracion del estilo clasificado 
   // 1. campo, min, max, tipo de gen (si fue rango o valores unicos)
   std::set<std::string> conditions;
   bool validconditions = true;
   for (int r = 0; validconditions && r < pTable_->GetRows(); ++r) {
      std::string condition;
      pTable_->GetCellValue(CLASS_COL_NUMBER, r, condition);
      std::string style;
      pTable_->GetCellValue(STYLE_COL_NUMBER, r, style);
      std::string label;
      pTable_->GetCellValue(LABEL_COL_NUMBER, r, label);
      validconditions = !condition.empty() && !style.empty() && !label.empty();
      params.categorizedStyleNames_.push_back(label);
      if (activelayer >= params.categorizedlayerstyle_.size()) {
         std::vector< std::pair <std::string, std::string> > styles;
         styles.push_back(std::make_pair(condition, style));
         params.categorizedlayerstyle_.push_back(styles);
      } else {
         params.categorizedlayerstyle_[activelayer].push_back(std::make_pair(condition, style));
      }
      std::set<std::string>::iterator fit = conditions.find(condition);
      if (fit == conditions.end()) {
         conditions.insert(condition);
      } else {
         validconditions = false;
      }
   }
   if (!validconditions)
      return NULL;
   std::string field = pClassGenerationWidget_->GetField();
   double min =  pClassGenerationWidget_->GetMinValue();
   double max =  pClassGenerationWidget_->GetMaxValue();
   int classgentype = pClassGenerationWidget_->GetClassGenerationType();
   wxXmlNode* prendernode = vector::render::CategorizedVectorRenderer::GetXmlNode(params);
   prendernode->GetChildren()->AddProperty(FIELD_PROP, field);
   prendernode->GetChildren()->AddProperty(MIN_PROP, DoubleToString(min));
   prendernode->GetChildren()->AddProperty(MAX_PROP, DoubleToString(max));
   prendernode->GetChildren()->AddProperty(CLASS_GEN_METHOD_PROP, NumberToString<int>(classgentype));
   return prendernode;
}
/** Restaura los valores originales del Part. */
bool CategorizedVectorStylePart::RollbackChanges() {
   return false;
}
/** Inicializa el part */
void CategorizedVectorStylePart::SetInitialValues() {
}

/** Actualiza el estado de la parte */
void CategorizedVectorStylePart::Update() {

}
/** Implementacion del metodo de creacion de ventana de Widget */
bool CategorizedVectorStylePart::CreateToolWindow() {
   pToolWindow_ = wxXmlResource::Get()->LoadPanel(pParentWindow_,
         wxT("ID_CATEGORIZED_STYLE_PANEL"));
   if (!pToolWindow_)
      return false;

   // configuracion de evento para los botones
   GET_CONTROL(*(this->GetWidget()->GetWindow()),
         "ID_REMOVE_CLASS_BTN", wxBitmapButton)->Connect(wxEVT_COMMAND_BUTTON_CLICKED,
   wxCommandEventHandler(CategorizedVectorStylePartHandler::OnRemoveClassClick),
                                                         NULL, pEventHandler_);
   GET_CONTROL(*(this->GetWidget()->GetWindow()),
         "ID_ADD_CLASS_BTN", wxBitmapButton)->Connect(wxEVT_COMMAND_BUTTON_CLICKED,
   wxCommandEventHandler(CategorizedVectorStylePartHandler::OnAddClassClick),
                                                      NULL, pEventHandler_);
   GET_CONTROL(*(this->GetWidget()->GetWindow()),
            "ID_GEN_CLASS_BTN", wxButton)->Connect(wxEVT_COMMAND_BUTTON_CLICKED,
      wxCommandEventHandler(CategorizedVectorStylePartHandler::OnClassGenerationClick),
                                                         NULL, pEventHandler_);

   pClassGenerationWidget_ = new VectorStyleClassGenerationWidget(pVectorLayer_);
   AddControl(pClassGenerationWidget_, "ID_CLASS_GEN_PANEL");

   ConfigureClassTable();
   AddControl(pTablePart_->GetWidget(), "ID_CLASS_TABLE_PANEL");
   pToolWindow_->SetAutoLayout(true);
   return true;
}

/* Metodo auxiliar para la configuracion de la tabla **/
void CategorizedVectorStylePart::ConfigureClassTable() {
   pTable_ = new DefaultTable();
   MemoryDriver* pdriver = new MemoryDriver();
   pTable_->SetDriver(pdriver);
   pTable_->AppendColumn("Etiqueta", Table::STRING);
   std::string classfield = "Clase";
   pTable_->AppendColumn(classfield.c_str(), Table::QBUILDER);
   pTable_->AppendColumn("Estilo", Table::VSTYLE);

   TablePartConfigurator config;
   Option options;
   options.SetOption(TablePartConfigurator::RoColumnsKeyOption, classfield);
   options.SetOption(TablePartConfigurator::TableEditorKeyOption,
                     TablePartConfigurator::VectorTableEditorOption);
   options.SetOption(TablePartConfigurator::VectorDatasourceUrl,
                     pVectorLayer_->GetElement()->GetUrl().c_str());
   pTable_->SetOption(options);
   pTablePart_ = config.Initialize(pDataViewManager_, pTable_, options);
   pTablePart_->SetOption(options);

   wxXmlNode* pcatnode = pVectorLayer_->GetElement()->GetNode(
         RENDERIZATION_NODE NODE_SEPARATION_TOKEN CATEGORIZED_LAYER_NODE);
   if (pcatnode) {
      vector::render::CategorizedVectorRenderer::CategorizedParameters params =
            vector::render::CategorizedVectorRenderer::GetParameters(
                  pVectorLayer_->GetElement()->GetNode(""));
      VectorElement* pvec = dynamic_cast<VectorElement*>(pVectorLayer_->GetElement());
      if (params.categorizedlayerstyle_.empty() || !pvec)
         return;
      std::vector<std::string>::iterator cnamesit = params.categorizedStyleNames_.begin();
      size_t activelayer = pvec->GetActiveLayer(); 
      if (activelayer < params.categorizedlayerstyle_.size()) {
         std::vector< std::pair<std::string, std::string> >::iterator stylesit = 
                                    params.categorizedlayerstyle_[activelayer].begin();
         
         std::string initialstyle = stylesit->second;
         std::string finalstyle = params.categorizedlayerstyle_[activelayer].rbegin()->second;
         for (int r = 0; stylesit != params.categorizedlayerstyle_[activelayer].end(); 
                                          ++stylesit, ++cnamesit, ++r) {
            pTable_->AppendRow();
            std::string condition = stylesit->first;
            pTable_->SetCellValue(CLASS_COL_NUMBER, r, condition);
            pTable_->SetCellValue(STYLE_COL_NUMBER, r, stylesit->second);
            std::string label = *cnamesit;
            pTable_->SetCellValue(LABEL_COL_NUMBER, r, label);
         }
         // configuracion de estilo base, color inicial y color final
         pClassGenerationWidget_->SetBaseStyle(initialstyle);  
         VectorStyle* pinitvstyle = VectorStyle::Create(initialstyle); 
         VectorStyle* pfinishvstyle = VectorStyle::Create(finalstyle); 
         if (pinitvstyle->GetBrush()) {
            pClassGenerationWidget_->SetInitialColor(pinitvstyle->GetBrush()->color_);
            pClassGenerationWidget_->SetFinalColor(pfinishvstyle->GetBrush()->color_);
         } else if (pinitvstyle->GetPen()) {
            pClassGenerationWidget_->SetInitialColor(pinitvstyle->GetPen()->color_);
            pClassGenerationWidget_->SetFinalColor(pfinishvstyle->GetPen()->color_);
         } else {
            pClassGenerationWidget_->SetInitialColor(pinitvstyle->GetSymbol()->color_);
            pClassGenerationWidget_->SetFinalColor(pfinishvstyle->GetSymbol()->color_);
         }
         VectorStyle::Destroy(pinitvstyle);
         VectorStyle::Destroy(pfinishvstyle);
         wxString minprop, maxprop, fieldprop, typeprop;
         // si tiene la propiedad de campo deberian estar configuradas las demas 
         if (pcatnode->GetPropVal(FIELD_PROP, &fieldprop)) {
            pcatnode->GetPropVal(MIN_PROP, &minprop);
            pcatnode->GetPropVal(MAX_PROP, &maxprop);
            pcatnode->GetPropVal(CLASS_GEN_METHOD_PROP, &typeprop);
            pClassGenerationWidget_->SetClassGenerationType(StringToNumber<int>(typeprop.c_str()));
            pClassGenerationWidget_->SetField(fieldprop.c_str());
            pClassGenerationWidget_->SetMaxValue(StringToNumber<double>(maxprop.c_str()));
            pClassGenerationWidget_->SetMinValue(StringToNumber<double>(minprop.c_str()));
         }
      }
   }
}
/**
 *  Metodo que se llama cuando se genera el evento de click sobre el boton
 *  de remover clase
 */
void CategorizedVectorStylePart::OnRemoveClassClick(wxCommandEvent& Event) {
   wxArrayInt rows = pTablePart_->GetSelectedRows();
   for (size_t r = 0 ; r < rows.size() ; ++r) {
      pTable_->DeleteRow(rows[r]);
      modified_ = true;
   }
}

/**
 *  Metodo que se llama cuando se genera el evento de click sobre el boton
 *  de agregar clase
 */
void CategorizedVectorStylePart::OnAddClassClick(wxCommandEvent& Event) {
   if (pTable_)
      pTable_->AppendRow();   
   pToolWindow_->GetParent()->GetParent()->Fit();
   pToolWindow_->Fit();
   pTablePart_->GetWidget()->GetWindow()->Layout();
   pTablePart_->GetGridControl()->ForceRefresh();   
   pToolWindow_->GetParent()->GetParent()->GetParent()->Refresh();
   pToolWindow_->GetParent()->GetParent()->GetParent()->Update();   
   pToolWindow_->GetParent()->GetParent()->Refresh();
   pToolWindow_->GetParent()->GetParent()->Update();   
   pToolWindow_->GetParent()->GetParent()->GetParent()->Layout();
   pToolWindow_->GetParent()->GetParent()->Layout();
   pToolWindow_->GetParent()->Layout();

   pToolWindow_->GetParent()->Refresh();
   pToolWindow_->GetParent()->Update();
   pToolWindow_->Refresh();
   pToolWindow_->Update();
}


/**
 * Metodo que se llama cuando se genera el evento de click sobre el boton
 * de generar clases
 */
void CategorizedVectorStylePart::OnClassGenerationClick(wxCommandEvent& Event) {
   std::vector<VectorStyleClassGenerationWidget::ClassDefinition> classes =
         pClassGenerationWidget_->GetClasses();
   pTable_->Clear();
   for (size_t c = 0; c < classes.size(); ++c) {
      pTable_->AppendRow();
      pTable_->SetCellValue(CLASS_COL_NUMBER, c, classes[c].condition_);
      pTable_->SetCellValue(STYLE_COL_NUMBER, c, classes[c].wktStyle_);
      pTable_->SetCellValue(LABEL_COL_NUMBER, c, classes[c].name_);
   }
   pToolWindow_->GetParent()->GetParent()->Fit();
   pToolWindow_->Fit();
   pTablePart_->GetWidget()->GetWindow()->Layout();
   pTablePart_->GetGridControl()->ForceRefresh();   
   pToolWindow_->GetParent()->GetParent()->GetParent()->Refresh();
   pToolWindow_->GetParent()->GetParent()->GetParent()->Update();   
   pToolWindow_->GetParent()->GetParent()->Refresh();
   pToolWindow_->GetParent()->GetParent()->Update();   
   pToolWindow_->GetParent()->GetParent()->GetParent()->Layout();
   pToolWindow_->GetParent()->GetParent()->Layout();
   pToolWindow_->GetParent()->Layout();

   pToolWindow_->GetParent()->Refresh();
   pToolWindow_->GetParent()->Update();
   pToolWindow_->Refresh();
   pToolWindow_->Update();

}
} /** namespace ui */
} /** namespace suri */
