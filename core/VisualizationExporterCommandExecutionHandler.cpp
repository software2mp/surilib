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

// Includes Estandar
#include <string>
#include <vector>

// Includes Suri
#include "suri/VisualizationExporterCommandExecutionHandler.h"
#include "suri/GenericTool.h"
#include "suri/RasterElement.h"
#include "suri/ToolSupport.h"
#include "suri/ProcessInterface.h"
#include "suri/ProcessAdaptLayer.h"
#include "suri/VisualizationExporterProcess.h"
#include "suri/ProcessNotebookWidget.h"
#include "suri/DynamicViewerWidget.h"
#include "suri/AuxiliaryFunctions.h"
#include "BandInfo.h"
#include "Margin.h"
#include "suri/Viewer2D.h"

// Defines
#define SRS_LIBRARY_CODE "srs"

namespace suri {

namespace core {

/** Constructor **/
VisualizationExporterCommandExecutionHandler::VisualizationExporterCommandExecutionHandler(
      DataViewManager* pDataView) : pDataView_(pDataView) {
}

/** Destructor **/
VisualizationExporterCommandExecutionHandler::~VisualizationExporterCommandExecutionHandler() {
}

/**
 * Metodo que en funcion del Command que se pasa por parametro se encarga de administrar
 * la ejecucion del mismo (en caso de que pueda manejar el tipo de comando).
 * @return true en caso de poder manejar la ejecucion del comando
 * @return false en caso contrario
 */
bool VisualizationExporterCommandExecutionHandler::Execute(
      const Command* pCommand, GenericTool* pTool) {
   if (pCommand->GetId() != pTool->GetId("VisualizationExporterProcess"))
      return false;
   RasterElement* praster = ToolSupport::GetRaster(pDataView_->GetViewcontextManager());
   ProcessAdaptLayer* pPal=NULL;
   if (praster) {
      Subset elementsubset;
      World* pworld = ToolSupport::GetWorld(pDataView_->GetViewportManager());
      if(!pworld){
         SHOW_WARNING(message_PROCESS_START_ERROR);
         return false;
      }
      pworld->GetWindow(elementsubset);
      // Configuro manualmente el subset ya que la herramienta
      // no posee el part de seleccion espacial
      std::vector<Element*> inputelements = GetInputElements();
      VisualizationExporterProcess process(praster,
            inputelements, elementsubset, pDataView_);
      pPal = process.GetProcessAdaptLayer();
      SetAttributes(praster, *pPal);
      pPal->AddAttribute<Subset>(ProcessAdaptLayer::InputSubsetWindowKeyAttr, elementsubset);
      pPal->AddAttribute<Subset>(ProcessAdaptLayer::OutputSubsetWindowKeyAttr, elementsubset);
      Viewer2D *pviewer = dynamic_cast<Viewer2D*>(
            pDataView_->GetViewportManager()->GetSelectedViewport());
      if (!pviewer){
         DynamicViewerWidget* pview = NULL;
         pview = dynamic_cast<DynamicViewerWidget*>(
                           pDataView_->GetViewportManager()->GetSelectedViewport());
         pviewer = dynamic_cast<Viewer2D*>(pview->GetViewer());
      }
      if (pviewer->GetMode() != Viewer2D::Gis) {
         process.OnProcessAdaptLayerSet();
         suri::ProcessNotebookWidget* pprocesswidget = new suri::ProcessNotebookWidget(
               &process, caption_VISUALIZATION_EXPORT, pDataView_->GetLibraryManager());
         if (pprocesswidget->CreateTool() && pprocesswidget->ShowModal() == wxID_OK) {
            Element* pNewElement = NULL;
            pPal->GetAttribute<Element*>(
                        ProcessAdaptLayer::OutputElementKeyAttr, pNewElement);
            DatasourceInterface* pdatasource = DatasourceInterface::
                  Create("RasterDatasource", pNewElement);
            if (pdatasource)
               pDataView_->GetDatasourceManager()->AddDatasource(pdatasource);
         }
      } else { //TODO: Detallar error
         SHOW_WARNING(message_PROCESS_START_ERROR);
      }
   } else {
      SHOW_WARNING(message_PROCESS_START_ERROR);
   }
   return true;
}

/** Configura atributos del proceso a traves del pal */
void VisualizationExporterCommandExecutionHandler::SetAttributes(
      RasterElement* pRaster, ProcessAdaptLayer &Pal) {
   bool notneededparts = false;
   Pal.AddAttribute<bool>(ProcessAdaptLayer::SpectralPartKeyAttr, notneededparts);
   Pal.AddAttribute<bool>(ProcessAdaptLayer::IncludeMaskKeyAttr, notneededparts);
   Pal.AddAttribute<bool>(ProcessAdaptLayer::IncludeSpatialSelKeyAttr, notneededparts);

   wxXmlNode* pnode = pRaster->GetNode(RENDERIZATION_NODE);
   wxXmlNode* pchild = pnode->GetChildren();
   while (pchild) {
      if (pchild->GetName().compare(BAND_COMBINATION_NODE) == 0) {
         std::vector < std::string > bands = tokenizer(pchild->GetNodeContent().c_str(), " ");
         std::vector<BandInfo> bandsinfo;
         std::vector<int> selectedbands;
         BandInfo pb;
         for (size_t b = 0; b < bands.size(); ++b) {
            std::string bname;
            pRaster->GetBandName(bname, StringToNumber<int>(bands[b]));
            pb = BandInfo(pRaster, StringToNumber<int>(bands[b]), bname, 0, 0, "");
            bandsinfo.push_back(pb);
            selectedbands.push_back(b);
         }
         Pal.AddAttribute<std::vector<int> >(ProcessAdaptLayer::SelectedBandsKey, selectedbands);
         /**
          * Si el elemento de entrada posee una banda simulo una combinacion para formar una
          * imagen de tres bandas para poder conservar el color de los vectores (de lo contrario
          * se renderizarian en escala de grises)
          */
         for (int b = 0; b < 2 && bands.size() < 3; ++b)
            bandsinfo.push_back(pb);
         //Agrego el vector serializable
         Pal.AddSerializableAttribute<BandInfo>(ProcessAdaptLayer::BandInfoKeyAttr, bandsinfo);
         break;
      }
      pchild = pchild->GetNext();
   }
}

/**
 * Recorre los elementos y agrega a un vector los que se encuentran activos
 * @return vector con los elementos activos
 */
std::vector<Element*> VisualizationExporterCommandExecutionHandler::GetInputElements() {
   LayerList* playerlist =
         pDataView_->GetViewcontextManager()->GetSelectedViewcontext()->GetLayerList();
   int elementcount = playerlist->GetElementCount(false);
   std::vector<Element*> inputelements;
   /**
    * Para agregar titulo y subtitulo a la exportacion de la visualizacion se utiliza una capa de
    * tipo punto a la cual se le configura un estilo vectorial que no muestra el punto pero si
    * muestra la expresion de un campo en la capa que corresponde al texto ingresado por el usuario
    * en el part. Estas capas se agregan al vector en esta instancia ya que en otras instancias del
    * proceso no se renderizan en la capa de salida.
    */
   Margin margin(NULL);
   margin.RemoveTmpFiles();
   std::string Ext = ".shp";
   World* pworld = ToolSupport::GetWorld(pDataView_->GetViewportManager());
   std::string tmpfile[] = {Margin::TitleLayerName, Margin::SubtitleLayerName,
                            Margin::TopMarginLayerName, Margin::BottomMarginLayerName,
                            Margin::LeftMarginLayerName, Margin::RightMarginLayerName};

   for (int i = 0; i  < 6; ++i) {
      Vector* pvec = Vector::Open(margin.GetTmpFilename(tmpfile[i], Ext), Vector::ReadWrite);
      if (pvec) {
         pvec->CreateLayer(margin.GetTmpFilename(tmpfile[i], Ext), pworld->GetSpatialReference(),
                           Vector::Polygon);
         VectorElement* pvectorelement =
               VectorElement::Create(margin.GetTmpFilename(tmpfile[i], Ext));
         if (tmpfile[i].compare(Margin::TitleLayerName) == 0)
            pvectorelement->SetStyle(Margin::TitleFontStyle);
         else if (tmpfile[i].compare(Margin::SubtitleLayerName) == 0)
            pvectorelement->SetStyle(Margin::SubtitleFontStyle);
         else
            pvectorelement->SetStyle(Margin::MarginStyle);
         inputelements.push_back(pvectorelement);
         delete pvec;
      }
   }

   for (int i = 0; i < elementcount; i++) {
      Element* pelement = playerlist->GetElement(i, false);
      if (pelement && pelement->IsActive() && (pelement->GetClassId() != "RasterElement"))
         inputelements.push_back(pelement);
   }

   if (inputelements.size() == 0) {
      RasterElement* praster = ToolSupport::GetRaster(pDataView_->GetViewcontextManager());
      inputelements.push_back(praster);
   }

   return inputelements;
}

}  /** namespace core **/
} /** namespace suri */
