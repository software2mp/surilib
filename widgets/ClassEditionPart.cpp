/*! \file ClassEditionPart.cpp */
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
#include <vector>
#include <limits>

// Includes Suri
#include "resources.h"
#include "DefaultTable.h"
#include "ClassEditionPart.h"
#include "RasterDatasource.h"
#include "ClassifiedRasterDatasourceValidator.h"
#include "ClassifiedRasterDatasourceManipulator.h"
#include "suri/messages.h"
#include "suri/VectorStyle.h"

// Includes Wx

// Defines
#define ID_TABLE_CONTAINER "ID_TABLE_CONTAINER"

// forwards

namespace suri {

/**
 * Nombre de la primera columna
 */
const std::string ClassEditionPart::FirstColumn = "Nombre";

/**
 * Nombre de la segunda columna
 */
const std::string ClassEditionPart::SecondColumn = "Color";

/**
 * Constructor
 */
ClassEditionPart::ClassEditionPart(RasterDatasource *pRasterDatasource,
                                   LayerInterface* pLayer) :
      pRasterDatasource_(pRasterDatasource), pLayer_(pLayer), pVariablesTable_(NULL),
      pTablePart_(NULL) {
   windowTitle_ = caption_CLASS_PROPERTIES;
}

/**
 * Destructor
 */
ClassEditionPart::~ClassEditionPart() {
   delete pVariablesTable_;
}

/**
 * Retorna el icono de la herramienta
 */
void ClassEditionPart::GetWindowIcon(wxBitmap& ToolBitmap, int X, int Y) const {
   GET_BITMAP_RESOURCE(icon_CLASS_PROPERTIES, ToolBitmap);
}

/**
 * Indica si el Part tiene cambios para salvar
 */
bool ClassEditionPart::HasChanged() {
   return pTablePart_->HasChanged();
}

/**
 * Salva los cambios realizados en el Part
 */
bool ClassEditionPart::CommitChanges() {
   if (HasChanged()) {
      int rowcount = indexes_.size();
      // Genero tabla de colores con clases seleccionadas
      ColorTable colortable;
      colortable.Activate(true);
      /**
       * Se agrega como primer color de la tabla de colores el "valor no
       * valido de la imagen clasificada (el 0)
       */
      colortable.AddColor(ClassInformation::InvalidClassIndex, 1, 1, 1);

      for (int row = 0; row < rowcount; row++) {
         ClassifiedRasterDatasourceManipulator manipulator;
         std::string name;
         pVariablesTable_->GetCellValue(0, row, name);
         std::string color;
         pVariablesTable_->GetCellValue(1, row, color);
         ClassInformation::IndexType index = indexes_[row];
         ClassInformation classinfo = manipulator.GetClassInformation(
               pRasterDatasource_, index);
         if (name.compare(classinfo.GetName()) != 0)
            classinfo.SetName(name);
         if (color.compare(classinfo.GetColor().GetColorAsHexaString()) != 0)
            classinfo.SetColor(VectorStyle::Color(color));
         colortable.AddColor(classinfo.GetIndex(), classinfo.GetColor().red_,
                             classinfo.GetColor().green_, classinfo.GetColor().blue_);
         manipulator.AddClassInformation(&classinfo, pRasterDatasource_, true);
         if (row == rowcount - 1) {
            colortable.AddColor(std::numeric_limits<double>::max(),
                                classinfo.GetColor().red_, classinfo.GetColor().green_,
                                classinfo.GetColor().blue_);
         }
      }
      ConfigureDatasourceColorTable(colortable);
   }
   return true;
}

/**
 * Reemplaza los nodos de tabla de color de la capa activa y la fuente de datos
 * @param[in] ColorTable Tabla de colores configurada de acuerdo a la edicion de clases
 */
void ClassEditionPart::ConfigureDatasourceColorTable(const ColorTable& ColorTable) {
   // Genero nodo con datos parameters
   wxXmlNode* pctnode = NULL;
   if (!ColorTable::ToXml(ColorTable, pctnode)) {
      REPORT_DEBUG("Fallo al generar nodo de tabla de colores");
   }
   // TODO(Gabriel - TCK #863): De esta forma no se replica de forma inmediata en el part de xml
   // (ya que el xml que se muestra es el de la fuente de datos. Modifico la LUT del raster
   Element* pelementlayer = pLayer_->GetElement();
   if (!pelementlayer)
      return;
   wxXmlNode* prenderernodelayer = pelementlayer->GetNode(wxT(RENDERIZATION_NODE));
   wxString oldlutnodepath = wxString(wxT(RENDERIZATION_NODE)) + wxT("|")
         + wxString(wxT(LUT_NODE));
   wxXmlNode* poldlutnodelayer = pelementlayer->GetNode(oldlutnodepath.c_str());
   wxString oldctnodepath = wxString(wxT(RENDERIZATION_NODE)) + wxT("|")
         + wxString(wxT(COLOR_TABLE));
   wxXmlNode* pctoldnodelayer = pelementlayer->GetNode(oldctnodepath);
   if (pctoldnodelayer) {
      pelementlayer->RemoveNode(prenderernodelayer, pctoldnodelayer);
   }
   if (poldlutnodelayer) {
      prenderernodelayer->InsertChildAfter(pctnode, poldlutnodelayer);
      prenderernodelayer->RemoveChild(poldlutnodelayer);
   } else {
      pelementlayer->AddNode(prenderernodelayer, pctnode, true);
   }

   if (pRasterDatasource_) {
      Element* pelement = pRasterDatasource_->GetElement();
      pelement->AddNode(pelementlayer->GetNode(), new wxXmlNode(*prenderernodelayer));
      pelementlayer->AddNode(pelementlayer->GetNode(),
                             new wxXmlNode(*pelement->GetNode(CLASSIFICATION_NODE)));
      pelement->SetChanged();
      pelement->SendViewerUpdate();
   }
   pelementlayer->SetChanged();
   pelementlayer->SendViewerUpdate();
}
/**
 * Restaura los valores originales del Part
 */
bool ClassEditionPart::RollbackChanges() {
   SetInitialValues();
   return true;
}

/**
 * Crea la Tool Window
 */
bool ClassEditionPart::CreateToolWindow() {
   pToolWindow_ = wxXmlResource::Get()->LoadPanel(pParentWindow_,
                                                  wxT("ID_CLASS_MAPPING_PANEL"));
   pVariablesTable_ = new suri::DefaultTable();
   pVariablesTable_->AppendColumn(FirstColumn, Table::STRING);
   pVariablesTable_->AppendColumn(SecondColumn, Table::COLOR_RGBA_HEXA);

   pTablePart_ = new TablePart(pVariablesTable_);
   pTablePart_->SetSelectionNotifier(NULL);
   AddControl(pTablePart_, wxT(ID_TABLE_CONTAINER));
   SetInitialValues();
   return true;
}

/**
 * Inicializa los valores iniciales de la grilla del part
 */
void ClassEditionPart::SetInitialValues() {
   ClassifiedRasterDatasourceValidator validator;
   indexes_.clear();
   if (validator.IsValid(pRasterDatasource_)) {
      ClassifiedRasterDatasourceManipulator manipulator;
      std::vector<ClassInformation> clist = manipulator.GetClassInformationList(
            pRasterDatasource_);
      std::vector<ClassInformation>::iterator classit;
      bool append = pVariablesTable_->GetRows() == 0;
      int row;
      for (classit = clist.begin(), row = 0; classit != clist.end(); classit++, row++) {
         // Agrego una nueva fila
         if (append)
            pVariablesTable_->AppendRow();
         // Agrego los valores a la grilla
         std::string name = classit->GetName();
         std::string color = classit->GetColor().GetColorAsHexaString();
         pVariablesTable_->SetCellValue(0, row, name);
         pVariablesTable_->SetCellValue(1, row, color);
         // Asocio la fila con el indice
         ClassInformation::IndexType index = classit->GetIndex();
         indexes_.push_back(index);
      }
   }
}
/**
 * Actualiza el estado de la parte
 */
void ClassEditionPart::Update() {
   enabled_ = ClassifiedRasterDatasourceValidator().IsValid(pRasterDatasource_);
}

} /** namespace suri */
