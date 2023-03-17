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

// Includes standar
#include <vector>
#include <string>

// Includes Suri
#include "BandInfo.h"
#include "FileRenderization.h"
#include "SpatialReference.h"
#include "suri/AuxiliaryFunctions.h"
#include "suri/DataTypes.h"
#include "suri/DataViewManager.h"
#include "suri/Element.h"
#include "suri/FileExporterProcess.h"
#include "suri/FileManagementFunctions.h"
#include "suri/LayerList.h"
#include "suri/LibraryClient.h"
#include "suri/LibraryManagerFactory.h"
#include "suri/messages.h"
#include "suri/RasterElement.h"
#include "suri/Subset.h"
#include "suri/ProcessFactory.h"

// Includes wx
#include "wx/wx.h"
#include "wx/xrc/xmlres.h"

// Defines
/** Espacio extra para salvar aparte del tamanio estimado del raster */
#define EXTRA_SAVE_SPACE_REQUIRED 1000000

/** namespace suri */
namespace suri {

class DataViewManager;

REGISTER_PROCESS_CREATOR(FileExporterProcess, process::FileExporterProcessCreator)

/** Nombre del proceso **/
const std::string FileExporterProcess::kProcessName ="FileExporterProcess";

/** ctor a partir de la capa de adaptacion de procesos **/
FileExporterProcess::FileExporterProcess(DataViewManager* pDataViewManager) :
                                       RasterProcess(pDataViewManager){
   pRenderizationObject_ = new FileRenderization();
   SetProcessName(kProcessName);
}

/**
 * Constructor: agrega el titulo de la ventana
 * @param[in] pInputElement : puntero al elemento seleccionado de la lista
 * @param[in] ViewerSubset:
 * @param[out] pOutputElement: referencia al elemento creado por la herramienta
 */
FileExporterProcess::FileExporterProcess(Element *pInputElement,
                                         const Subset &ViewerSubset,
                                         DataViewManager* pDataViewManager) :
      RasterProcess(pInputElement, ViewerSubset, pDataViewManager){
   pRenderizationObject_ = new FileRenderization();
   SetProcessName(kProcessName);
}

/**
 * Ctor
 * @param[in] ElementsP Vector de elementos seleccionados de la lista
 * @param[in] ViewerSubset
 * @param[out] pOutputElement Referencia al elemento creado por la herramienta
 */
FileExporterProcess::FileExporterProcess(std::vector<Element*> InputElements,
                                         const Subset &ViewerSubset,
                                         DataViewManager* pDataViewManager) :
      RasterProcess(InputElements, ViewerSubset, pDataViewManager){
   pRenderizationObject_ = new FileRenderization();
   SetProcessName(kProcessName);
}

/**
 * Destructor
 */
FileExporterProcess::~FileExporterProcess() {
}

/**
 * Actualiza el estado del part con informacion de otros parts
 */
void FileExporterProcess::DataUpdate() {
   RasterProcess::DataUpdate();
#ifdef __UNUSED_CODE__
   if (!pFormatPart_) return;
   pFormatPart_->SetEnabled(HasValidSpectralData());
#endif
}

/**
 * Configura el nodo de seleccion de bandas en el raster
 * @param[in] pRasterElement puntero al elemento a configurar
 * @return true siempre
 */
bool FileExporterProcess::ConfigureRaster(RasterElement *pRasterElement) {
   // Tipo de dato para calcular el tamanio de salida de elemento
   dataType_ = pRasterElement->GetRasterProperties().DataType;
   return true;
}

/**
 * Obtiene el Filename y Opciones de la interfaz grafica, verifica su validez y
 * los setea a la propiedad interna de FileRenderization.
 * @return false filename invalido
 * @return false espacio en disco insuficiente.
 */
bool FileExporterProcess::ConfigureProcess() {
   // Configura el nombre y formato de archivo de salida
   std::string filename;
   Option option;
   GetSelectedOutput(filename, option);
   // Verifica que el url del archivo de salida no sea igual al de entrada
   if (!IsValidOutput(filename)) {
      message_.assign(message_INVALID_OUTPUT);
      return false;
   }

   FileRenderization *pfilerender = NULL;
   pfilerender = dynamic_cast<FileRenderization*>(pRenderizationObject_);
   pfilerender->SetFilename(filename);
   pfilerender->SetOption(option);

   // Verifica el espacio disponible
   wxFileName directory(filename);
   if (!CheckNeededDiskSpace(directory.GetPath(), GetNeededSpace())) {
      message_.assign(message_NOT_ENOUGHT_DISK_SPACE);
      return false;
   }
   return true;
}

/*!
 * Crea un elemento con el resultado de la renderizacion.
 * y devuelve true.
 * return bool que indica si pudo generar elemento
 */
bool FileExporterProcess::ConfigureOutput() {
   std::string filename;
   Option option;
   GetSelectedOutput(filename, option);

   // Crea el elemento con el url del objeto salvado
   Element* pelement = pInputElement_ ? pInputElement_ : inputElements_[0];
   Element* pNewElement;
   if (pelement) {
      Option opt = pelement->GetMetadataMap();
      pNewElement = Element::Create("RasterElement", filename, opt);
   } else {
      pNewElement = Element::Create("RasterElement", filename);
   }
   wxFileName file(filename);
   if (pNewElement) {
      pNewElement->SetName(file.GetName());
      std::string parameters = GetParametersAsXmlString();
      if (!parameters.empty()) {
#ifdef __WINDOWS__
      /**
       * Este fix se agrega para evitar que la creacion de wxXmlDocument == false
       * TODO (Federico Jurio - 09/04/2015): A futuro cuando el resto de los procesos sean
       * serializables analizar si Xml2String de XmlFunctions debe borrar el header
       */
      std::string header =
            "<?xml version=\"1.0\" encoding=\"" + suri::XmlElement::xmlEncoding_ + "\"?>\n";
      parameters = header +
            wxString(parameters.c_str(), wxCSConv(suri::XmlElement::xmlEncoding_.c_str()));
#endif
         REPORT_DEBUG("Parametros del proceso %s", parameters.c_str());
         wxStringInputStream ss(parameters);
         wxXmlDocument doc(ss, wxT(suri::XmlElement::xmlEncoding_));
         if (pNewElement->GetNode(PROCESS_HISTORY_NODE) == NULL)
            pNewElement->AddNode(pNewElement->GetNode(), PROCESS_HISTORY_NODE);
         pNewElement->AddNode(pNewElement->GetNode(PROCESS_HISTORY_NODE), doc.DetachRoot());
      }
   }
   this->pAdaptLayer_->AddAttribute<Element*>(
         ProcessAdaptLayer::OutputElementKeyAttr, pNewElement);
   return pNewElement != NULL;
}

/*!
 * Metodo que configura el nombre de archivo y formato del objeto para salvar
 * @param[out] Filename : contiene la ruta y nombre seleccionado para salvar.
 * @param[out] Options : contiene el formato de salida seleccionado.
 */
void FileExporterProcess::GetSelectedOutput(std::string &Filename, Option &Options) {
   std::string selectedformat;
   if (pAdaptLayer_
         && pAdaptLayer_->GetAttribute<std::string>(
               ProcessAdaptLayer::OutputFileFormatKeyAttr, selectedformat)) {
      Options.SetOption("Format", selectedformat);
   }
   std::string filename;
   if (pAdaptLayer_
         && pAdaptLayer_->GetAttribute<std::string>(
               ProcessAdaptLayer::OutputFileNameKeyAttr, filename)) {
      Filename = filename;
   }
}

/**
 * Obtiene el tamano en bytes del elemento a salvar. El tamanio es la cantidad
 * de bandas, por el tipo de dato, por el tamanio de la matriz, mas un espacio
 * por precaucion.
 * @return neededspace : espacio necesario en bytes.
 */
wxLongLong FileExporterProcess::GetNeededSpace() {
   wxLongLong neededspace = 0;

   // Obtengo las bandas de la herramienta de seleccion
   int bandscount = 0;
   if (HasValidSpectralData())
      bandscount = GetSelectedRasterBands().size();

   int hight = 0, width = 0;
   // TODO(Gabriel - TCK #2315): Las clases hijas no deberian usar el World
   // directamente tendrian que usar el mundo de salida?
//   pInputWorld_->GetViewport(hight, width);
   World* pworld = (pOutputWorld_)? pOutputWorld_ : pInputWorld_;
   pworld->GetViewport(hight, width);
   neededspace = bandscount * hight * width
         * SizeOf(dataType_) + EXTRA_SAVE_SPACE_REQUIRED;
   return neededspace;
}

/**
 * Verifica que exista espacio en el disco dado un tamanio.
 * @param[in] Path : directorio donde guardar
 * @param[in] NeededSpace: espacio necesario.
 * @return true : si espacio disponible > necesario
 * @return false : si espacio disponible < necesario
 */
bool FileExporterProcess::CheckNeededDiskSpace(const wxString &Path,
                                              wxLongLong NeededSpace) {
   wxLongLong total;
   wxLongLong free;
   GetDiskSpace(Path, &total, &free);

   if (NeededSpace > free) {
      return false;
   }
   return true;
}

/**
 * Recibe un url y lo compara con el url del elemento de entrada.
 * @param[in] Filename nombre del archivo a salvar.
 * @return true El url de entrada es distinto al de salida
 * @return false Los url son iguales.
 */
bool FileExporterProcess::IsValidOutput(const std::string &Filename) {
   wxFileName inputfile(GetInputElement()->GetUrl().c_str());
   wxFileName outputfile(Filename);

   return (inputfile != outputfile);
}
/** Metodo que especializa la configuracion del ProcessAdaptLayer para
 *  los procesos especificos
 */
void FileExporterProcess::DoOnProcessAdaptLayerSet() {
   ConfigureOutput();
}

}  /** namespace suri **/
