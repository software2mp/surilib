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

#include <vector>
#include <string>

#include "DefaultDatasourceList.h"
#include "FilteredDatasource.h"
#include "SpatialReference.h"
#include "SpatialSubsetSelectionPart.h"
#include "VectorFormatSelectionPart.h"
#include "suri/AuxiliaryFunctions.h"
#include "Filter.h"
#include "suri/FileVectorExporterProcess.h"
#include "suri/FileVectorRenderizationController.h"
#include "suri/WorldSerializer.h"
#include "suri/XmlFunctions.h"

#include "wx/sstream.h"

#define FEATURE_ID_FIELD_NAME "FID"

namespace suri {

const std::string FileVectorExporterProcess::EnableSelectionExportAttr =
      "ExportSelectionEnable";

/**
 * Constructor
 * @param pInputDatasource fuente de datos que entra al proceso
 * @param ViewerSubset subset
 * @param pDataViewManager administrador de fuente de datos
 */
FileVectorExporterProcess::FileVectorExporterProcess(
      std::vector<DatasourceInterface*>& InputDatasources,
      const Subset &ViewerSubset, Element* &pOutputElement,
      DataViewManager* pDataViewManager) :
      inputDatasources_(InputDatasources), viewerSubset_(ViewerSubset),
      pOutputElement_(pOutputElement),
      pDataViewManager_(pDataViewManager), pInputWorld_(new World) {
   this->pAdaptLayer_->SetProcessName("FileVectorExporterProcess");
   Initialize();
}

/** Destructor **/
FileVectorExporterProcess::~FileVectorExporterProcess() {
}

#ifdef __UNUSED_CODE__
/** Agrega las partes. Sacar este metodo cuando se desacoplen los procesos */
bool FileVectorExporterProcess::GetParts(std::vector<Part*> &ProcessParts) {
   pSubsetSeelctionPart_ = new SpatialSubsetSelectionPart(pDataViewManager_, pInputWorld_);
   ProcessParts.push_back(pSubsetSeelctionPart_);
   pFileSelectPart_ = new VectorFormatSelectionPart();
   ProcessParts.push_back(pFileSelectPart_);
   return true;
}
#endif

/** Permite configurar la salida del proceso antes de correrlo */
bool FileVectorExporterProcess::ConfigureProcess() {
   std::string filename;
   Option option;
   GetSelectedOutput(filename, option);
   pFileController_->SetFilename(filename);
   pFileController_->SetOption(option);
   return true;
}

/** Una vez ejecutado el proceso hace visible la salida */
bool FileVectorExporterProcess::ConfigureOutput() {
   std::string filename;
   Option option;
   GetSelectedOutput(filename, option);
   // Crea el elemento con el url del objeto salvado
   pOutputElement_ = Element::Create("VectorElement", filename);
   wxFileName file(filename);
   if (pOutputElement_)
{
      pOutputElement_->SetName(file.GetName());
      
      std::string parameters = GetParametersAsXmlString();
      REPORT_DEBUG("Parametros del proceso %s", parameters.c_str());
      wxStringInputStream ss(parameters);
      wxXmlDocument doc(ss, suri::XmlElement::xmlEncoding_);
      if (pOutputElement_->GetNode(PROCESS_HISTORY_NODE) == NULL)
         pOutputElement_->AddNode(pOutputElement_->GetNode(), PROCESS_HISTORY_NODE);
      pOutputElement_->AddNode(pOutputElement_->GetNode(PROCESS_HISTORY_NODE), doc.DetachRoot());
   }
   return pOutputElement_ != NULL;
}


/** Inicializa la instancia **/
void FileVectorExporterProcess::Initialize() {
   pFileController_ = new FileVectorRenderizationController();
   pController_= pFileController_;
   Element* pelement = this->inputDatasources_.at(0)->GetElement();
   /** Crear el mundo con las propiedades del elemento recibido */
   pInputWorld_->SetSpatialReference(pelement->GetSpatialReference().c_str());
   Subset elementsubset;
   pelement->GetElementExtent(elementsubset);
   pInputWorld_->SetWorld(elementsubset);
   pInputWorld_->SetWindow(elementsubset);
}

/**
 * Obtiene la lista de elementos a renderizar en el proceso.
 */
DatasourceContainerInterface* FileVectorExporterProcess::GetRenderizationList() {
   /** TODO: Si se checkeo la opcion de exportar seleccion se debe generar
    ** una fuente de datos que posea ese filtro.
    **/
   DatasourceContainerInterface* plist = new DefaultDatasourceList();
   std::vector<DatasourceInterface*>::iterator it = inputDatasources_.begin();
   for (; it != inputDatasources_.end(); ++it) {
      wxXmlNode* pselnode = (*it)->GetElement()->GetNode(
            VECTOR_SELECTION_NODE NODE_SEPARATION_TOKEN FID_SELECTION);
      DatasourceInterface* pdatasource = NULL;
      bool exportsel = false;
      pAdaptLayer_->GetAttribute<bool>(ProcessAdaptLayer::ExportSelectionKeyAttr,
                                       exportsel);
      if (!pselnode || !exportsel) {
         const char *pdatasourcetypes[] = { "FilteredDatasource", "VectorDatasource" };
         for (int i = 0; i < 2 && pdatasource == NULL; ++i) {
            pdatasource = DatasourceInterface::Create(pdatasourcetypes[i], *it);
         }
      } else {
         std::string fidselection = pselnode->GetNodeContent().c_str();
         std::vector<std::string> fids = tokenizer(fidselection, ",");
         FilteredDatasource*pfdatasource = new FilteredDatasource;
         pfdatasource->SetDatasource(*it);
         Filter* pfilter = new Filter;
         pfilter->SetFilteredName(
               dynamic_cast<VectorDatasource*>(*it)->GetActiveLayerName());
         pfilter->SetIdFieldName(FEATURE_ID_FIELD_NAME);
         std::vector<std::string>::iterator fid = fids.begin();
         for (; fid != fids.end(); ++fid)
            pfilter->AddEqualConstraint(StringToNumber<long>(*fid));
         pfdatasource->SetFilter(pfilter);
         pdatasource = pfdatasource;
      }

      // Arreglo para que utilice el renderizador correcto.
      SanitizeDatasource(pdatasource);

      if (pdatasource)
         plist->AddDatasource(pdatasource);
   }
   return plist;
}

/** Configura el subset espacial */
World* FileVectorExporterProcess::GetWorld() {
   World* poutworld = new World();
   Subset subset;
#ifdef __UNUSED_CODE
   if (!pSubsetSeelctionPart_->GetSubset(subset)) {
      delete poutworld;
      return NULL;
   }
#endif
   if (!pAdaptLayer_
         || !pAdaptLayer_->GetAttribute<Subset>(
               ProcessAdaptLayer::InputSubsetWindowKeyAttr,
                                                subset)) {
      return NULL;
   }

   std::string srout;
   pAdaptLayer_->GetAttribute<std::string>(ProcessAdaptLayer::OutputSRSKeyAttr, srout);
   poutworld->SetSpatialReference(srout);
   // pelement->GetElementExtent(subset);
   // Iguala extent del mundo al window
   poutworld->SetWorld(subset);
   // Lo asigna al window del mundo.
   poutworld->SetWindow(subset);
   return poutworld;
}

/** Obtiene el nombre y formato del archivo y lo asigna al objeto salvacion */
void FileVectorExporterProcess::GetSelectedOutput(std::string &FileName,
                                                  Option &Options) {
#ifdef __UNUSED_CODE__
   if (pFileSelectPart_) {
      FileName = pFileSelectPart_->GetFileName();
   }
#endif
   if (pAdaptLayer_) {
      pAdaptLayer_->GetAttribute<std::string>(ProcessAdaptLayer::OutputFileNameKeyAttr, FileName);
   }
}

/** Metodo que se llama cuando se configura el ProcessAdaptLayer del proceso**/
void FileVectorExporterProcess::OnProcessAdaptLayerSet() {
   if (pAdaptLayer_) {
      pAdaptLayer_->AddAttribute<Subset>(ProcessAdaptLayer::InputSubsetWindowKeyAttr,
                                         viewerSubset_);
      pAdaptLayer_->AddAttribute<DataViewManager*>(
            ProcessAdaptLayer::DataViewManagerKeyAttr, pDataViewManager_);
      pAdaptLayer_->AddAttribute<Element*>(ProcessAdaptLayer::OutputElementKeyAttr,
                                           pOutputElement_);
      pAdaptLayer_->AddAttribute<World*>(ProcessAdaptLayer::InputWorldKeyAttr,
                                         pInputWorld_);
      /*WorldSerializer* ws = new WorldSerializer();
      pAdaptLayer_->AddSerializer(ws);*/
      bool mask = false;
      pAdaptLayer_->AddAttribute<bool>(ProcessAdaptLayer::IncludeMaskKeyAttr, mask);
      pAdaptLayer_->AddAttribute<std::vector<DatasourceInterface*> >(
            ProcessAdaptLayer::InputDatasourcesKeyAttr, inputDatasources_);
      std::string srout = pInputWorld_->GetSpatialReference();
      pAdaptLayer_->AddAttribute<std::string>(ProcessAdaptLayer::OutputSRSKeyAttr, srout);
   }
}

/** 
    * Metodo que a partir de un xml que contiene los parametros del proceso configura el mismo.
    * @param[in] XmStr string en formato xml que contiene los parametros necesarios para la 
    * correcta ejecucion del proceso
    * @return true en caso de poder configurar correctamente el proceso en funcion de los parametros
    * @return false en caso contrario
   **/
bool FileVectorExporterProcess::ConfigureProcessFromXmlString(const std::string& XmlStr) {
   return false;
}


/** Metodo que obtiene los parametros asociados al proceso en formato XML. 
 * Esto permite que el codigo cliente pueda hacer eso del xml resultante 
 * (ej. para la exportacion del mismo)
 * @return string en formato xml con los parametros asociados al proceso
 * @return string vacio en caso contrario. 
**/
std::string FileVectorExporterProcess::GetParametersAsXmlString() const {
   // TODO(Gabriel - TCK #7139): Mejorar codigo y pasar constantes a xmlnames
   wxXmlNode* pparamsnode = new wxXmlNode(NULL, wxXML_ELEMENT_NODE, wxT(PROCESS_NODE));
   std::string processName = this->pAdaptLayer_->GetProcessName();
   pparamsnode->AddChild(CreateKeyValueNode(NAME_NODE, _(processName.c_str())));
 
   std::string url = inputDatasources_[0]->GetUrl().c_str();
   pparamsnode->AddChild(CreateKeyValueNode(URL_NODE, url));

   pparamsnode->AddChild(CreateKeyValueNode("fecha", wxNow().c_str()));;
   
   /** Configuracion de mundo de entrada **/
   World* pworld = NULL;
   if (pAdaptLayer_->GetAttribute<World*>(ProcessAdaptLayer::InputWorldKeyAttr, pworld)) {
      pparamsnode->AddChild(CreateKeyValueNode("sr-entrada", 
                           SpatialReference::GetAuthorityId(pworld->GetSpatialReference())));
      Subset sin;
      pworld->GetWindow(sin);
      std::stringstream sinstr;
      sinstr << sin;
      pparamsnode->AddChild(CreateKeyValueNode("extent-entrada", sinstr.str()));
   }
   /** configuracion de mundo de salida */
   Subset sout;
   if (pAdaptLayer_->GetAttribute<Subset>(ProcessAdaptLayer::OutputSubsetWindowKeyAttr,
                                               sout)) {
      std::stringstream soutstr;
      soutstr << sout;
      pparamsnode->AddChild(CreateKeyValueNode("extent-salida", soutstr.str()));
   }
   std::string srout;
   pAdaptLayer_->AddAttribute<std::string>(ProcessAdaptLayer::OutputSRSKeyAttr, srout);
   srout = srout.empty() ? pworld->GetSpatialReference() : srout;
   pparamsnode->AddChild(CreateKeyValueNode("sr-salida", SpatialReference::GetAuthorityId(srout)));
   
   std::string outfilename;
   pAdaptLayer_->GetAttribute<std::string>(ProcessAdaptLayer::OutputFileNameKeyAttr, outfilename);
   pparamsnode->AddChild(CreateKeyValueNode("url-salida", outfilename));   
   std::vector<wxXmlNode*> additionalparams = DoGetXmlParameters();
   for (size_t i = 0; i < additionalparams.size(); ++i)
      pparamsnode->AddChild(additionalparams[i]);
   wxXmlDocument newdoc;
   newdoc.SetRoot(pparamsnode);
   wxStringOutputStream stream;
   newdoc.Save(stream);

   return stream.GetString().c_str();
}

/**
 * Cambia el nodo de renderizacion para poder acceder a la exportacion.
 */
void FileVectorExporterProcess::SanitizeDatasource(DatasourceInterface* pDatasource) {
   if (pDatasource != NULL) {
      std::string searchnode = std::string(RENDERIZATION_NODE)
            + std::string(NODE_SEPARATION_TOKEN) + std::string(LAYERS_NODE);
      wxXmlNode* players = pDatasource->GetElement()->GetNode(searchnode.c_str());
      if (players != NULL) {
         players->SetName(LAYERS_EXPO_NODE);
      }
   }
}

}  // namespace suri
