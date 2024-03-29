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

#ifndef PROCESSADAPTLAYER_H_
#define PROCESSADAPTLAYER_H_

// Includes Estandar
#include <string>
#include "suri/SerializableCollection.h"

// Includes Suri
// Includes Wx
// Defines
// forwards

namespace suri {

/** Clase encargada de brindar atributos necesarios
 * para la configuracion del proceso.*/
class ProcessAdaptLayer : public Serializable {
public:
   /** Constructor **/
   ProcessAdaptLayer();
   ProcessAdaptLayer(SerializableCollection* Params);
   /** Destructor **/
   virtual ~ProcessAdaptLayer();
   /** Obtiene el atributo que corresponde a la clave pasado por parametro
    *  y al tipo que corresponde al que se indica en el template
    *  @return true en caso de poder obtener el valor
    *  @return false en caso contrario
    */
   template<class V>
   bool GetAttribute(const std::string& Key, V& Value) const{
      return params_->GetValue<V>(Key, Value);
   }
   /** Agregar un atributo con la clave y valro pasado por parametro
    *  @return true en caso de poder agregarlo correctamente
    *  @return false en caso contrario */
   template<class V>
   void AddAttribute(const std::string& Key, V& Value){
      params_->AddValue<V>(Key, &Value);
   }
   /** Agregar un atributo con la clave y valor, que luego se serializará
    *  @return true en caso de poder agregarlo correctamente
    *  @return false en caso contrario */
   template<class V>
   void AddSerializableAttribute(const std::string& Key, V& Value) {
      params_->AddSerializableValue<V>(Key, &Value);
   }
   template<class T>
   void AddSerializableAttribute(const std::string& KeyP, const std::vector<T>& vect) {
      params_->AddSerializableValue<T>(KeyP, vect);
   }

   /** Obtiene los atributos en un string con formato xml**/
   std::string GetAttributesAsXmlString() const;

   /**
    * Agrega un serializador custom a la colección.
    * El mismo es eliminado en el destructor de la clase.
    */
   void AddSerializer(const std::string& KeyP, Serializer* Serializer);

   wxXmlNode* SerializeXml() const;
   static ProcessAdaptLayer* DeserializeXml(wxXmlNode* Xml);

   std::string GetProcessName() const;
   void SetProcessName(const std::string& str);
   /** Nombres de claves de atributos **/
   static const std::string ProcessKeyAttr;
   static const std::string DataViewManagerKeyAttr;
   static const std::string InputWorldKeyAttr;
   static const std::string OutputWorldKeyAttr;
   static const std::string InputElementsKeyAttr;
   static const std::string InputDatasourcesKeyAttr;
   static const std::string OutputElementKeyAttr;
   static const std::string IncludeMaskKeyAttr;
   static const std::string LayerListKeyAttr;
   static const std::string LibraryIdKeyAttr;
   static const std::string SpectralSelectionKeyAttr;
   static const std::string MaskListKeyAttr;

   static const std::string CombinationInfoKeyAttr;
   static const std::string BandInfoKeyAttr;
   static const std::string InputSubsetWindowKeyAttr;
   static const std::string OutputSubsetWindowKeyAttr;
   static const std::string OutputFileFormatKeyAttr;
   static const std::string InputFileNameKeyAttr;
   static const std::string OutputFileNameKeyAttr;
   static const std::string IncludeSpatialSelKeyAttr;
   static const std::string IncludeTransformationParametersKeyAttr;
   /** Clave para el atributo de seleccion de bandas **/
   static const std::string SelectedBandsKey;
   /** Flag para indicar si se muestra la seleccion espectral en el proceso
    *  de clasificacion */
   static const std::string SpectralPartKeyAttr;

   static const std::string GcpListFileNameKeyAttr;
   static const std::string AlgorithmKeyAttr;
   static const std::string AlgorithmOrderKeyAttr;

   static const std::string OutputSizePixelsKeyAttr;
   static const std::string OutputSizeLinesKeyAttr;

   static const std::string OutputSizePixelWidthKeyAttr;
   static const std::string OutputSizePixelHeightKeyAttr;

   static const std::string SpatialBoundsKeyAttr;

   static const std::string OutputSRSKeyAttr;
   static const std::string IncludeBufferKeyAttr;
   static const std::string BufferDistanceKeyAttr;
   static const std::string BufferQuantityKeyAttr;
   static const std::string BufferIsFromFieldKeyAttr;
   static const std::string BufferFieldKeyAttr;
   static const std::string BufferConversionFactorKeyAttr;

   static const std::string IncludeVectorOperationKeyAttr;
   static const std::string VectorOperationTypeKeyAttr;
   static const std::string VectorOperationDatasourcesKeyAttr;

   static const std::string OutputRGBBands;
   static const std::string OutputHighResBand;

   static const std::string OutputRGBNVDAvailable;
   static const std::string OutputRGBNVD;

   static const std::string InputRGBElements;
   static const std::string InputPanElements;
   static const std::string CsvFormatPartKeyAttr;
   static const std::string IncludeMergeLayersKeyAttr;
   static const std::string MergeFieldsMapKey;
   static const std::string TasseledCapInputKeyAttr;
   static const std::string TasseledCapSensorKeyAttr;
   static const std::string TasseledCapOutputBandsKeyAttr;

   static const std::string PCAnalysisInputKeyAttr;
   static const std::string ExportSelectionKeyAttr;

   static const std::string MatrixTypeKeyAttr;

   static const std::string ComponentsCountKeyAttr;

   static const std::string ExportMetadataKeyAttr;

   static const std::string XMarginSizeKeyAttr;
   static const std::string YMarginSizeKeyAttr;
   static const std::string InputRasterElementsAttr;

   static const std::string MosaicPixelDistanceAttr;
   static const std::string MosaicNoDataColourAttr;
   static const std::string MosaicRasterVectorAttr;

   static const std::string HistogramMatchingInputAttr;
   static const std::string HistogramMatchingOutputBandsAttr;

   static const std::string RasterizationVectorDatasourceAttr;
   static const std::string RasterizationAttrSelectionAttr;
   static const std::string RasterizationTypeSelectionAttr;
private:
   SerializableCollection* params_;
   std::string processName_;
};

} /** namespace suri */

#endif /* PROCESSADAPTLAYER_H_ */
