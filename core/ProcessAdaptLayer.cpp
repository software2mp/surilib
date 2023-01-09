/*! \file ProcessAdaptLayer.cpp */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2013-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

// Includes Estandar
#include <string>

// Includes Suri
#include "suri/ProcessAdaptLayer.h"

// Includes Wx
#include "wx/xml/xml.h"

// Defines
// forwards

namespace suri {

/** Nombres de claves de atributos **/
const std::string ProcessAdaptLayer::ProcessKeyAttr = "Process";
const std::string ProcessAdaptLayer::DataViewManagerKeyAttr = "DataViewManager";
const std::string ProcessAdaptLayer::InputWorldKeyAttr = "InputWorld";
const std::string ProcessAdaptLayer::OutputWorldKeyAttr = "OutputWorld";
const std::string ProcessAdaptLayer::InputElementsKeyAttr = "InputElements";
const std::string ProcessAdaptLayer::InputDatasourcesKeyAttr = "InputDSs";
const std::string ProcessAdaptLayer::OutputElementKeyAttr = "OutputElem";
const std::string ProcessAdaptLayer::IncludeMaskKeyAttr = "IncludeMask";
const std::string ProcessAdaptLayer::IncludeSpatialSelKeyAttr = "IncludeSpatial";
const std::string ProcessAdaptLayer::LayerListKeyAttr = "LayerList";
const std::string ProcessAdaptLayer::LibraryIdKeyAttr = "LibraryId";
const std::string ProcessAdaptLayer::SpectralSelectionKeyAttr = "IncludeSpectralPart";
const std::string ProcessAdaptLayer::MaskListKeyAttr = "MaskList";
const std::string ProcessAdaptLayer::CombinationInfoKeyAttr = "CombinationInfo";
const std::string ProcessAdaptLayer::BandInfoKeyAttr = "BandInfo";
const std::string ProcessAdaptLayer::InputSubsetWindowKeyAttr = "SubsetInWindow";
const std::string ProcessAdaptLayer::OutputSubsetWindowKeyAttr = "SubsetOutWindow";
const std::string ProcessAdaptLayer::OutputFileFormatKeyAttr = "FileFormat";
const std::string ProcessAdaptLayer::InputFileNameKeyAttr = "InputFilename";
const std::string ProcessAdaptLayer::OutputFileNameKeyAttr = "OutputFilename";
const std::string ProcessAdaptLayer::IncludeTransformationParametersKeyAttr =
                                                                            "IncludeTransformation";
/** Clave para el atributo de seleccion de bandas **/
const std::string ProcessAdaptLayer::SelectedBandsKey = "SelectedBands";
/** Flag para indicar si se muestra la seleccion espectral en el proceso
 *  de clasificacion */
const std::string ProcessAdaptLayer::SpectralPartKeyAttr = "SpectralOn";

const std::string ProcessAdaptLayer::GcpListFileNameKeyAttr = "GcpListFileName";
const std::string ProcessAdaptLayer::AlgorithmKeyAttr = "Algorithm";
const std::string ProcessAdaptLayer::AlgorithmOrderKeyAttr = "AlgorithmOrder";

const std::string ProcessAdaptLayer::OutputSizePixelsKeyAttr = "OutputSizePixels";
const std::string ProcessAdaptLayer::OutputSizeLinesKeyAttr = "OutputSizeLines";

const std::string ProcessAdaptLayer::OutputSizePixelWidthKeyAttr = "OutputSizePixelWidth";
const std::string ProcessAdaptLayer::OutputSizePixelHeightKeyAttr = "OutputSizePixelHeight";

const std::string ProcessAdaptLayer::SpatialBoundsKeyAttr = "SpatialBounds";

const std::string ProcessAdaptLayer::OutputSRSKeyAttr = "OutputSRS";

const std::string ProcessAdaptLayer::IncludeBufferKeyAttr = "IncludeBuffer";
const std::string ProcessAdaptLayer::BufferDistanceKeyAttr = "Distance";
const std::string ProcessAdaptLayer::BufferQuantityKeyAttr = "Quantity";
const std::string ProcessAdaptLayer::BufferFieldKeyAttr = "Field";
const std::string ProcessAdaptLayer::BufferIsFromFieldKeyAttr = "IsFromField";
const std::string ProcessAdaptLayer::BufferConversionFactorKeyAttr = "ConversionFactor";

const std::string ProcessAdaptLayer::IncludeVectorOperationKeyAttr = "IncludeVectorOperation";
const std::string ProcessAdaptLayer::VectorOperationTypeKeyAttr = "VectorOperationType";
const std::string ProcessAdaptLayer::VectorOperationDatasourcesKeyAttr =
      "VectorOperationDatasources";

const std::string ProcessAdaptLayer::IncludeMergeLayersKeyAttr = "IncludeMergeLayers";

const std::string ProcessAdaptLayer::OutputRGBBands = "OutputRGBBands";
const std::string ProcessAdaptLayer::OutputHighResBand = "OutputHighResBand";

const std::string ProcessAdaptLayer::OutputRGBNVDAvailable = "OutputRGBNVDAvailable";
const std::string ProcessAdaptLayer::OutputRGBNVD = "OutputRGBNVD";

const std::string ProcessAdaptLayer::InputRGBElements = "InputRGBElements";
const std::string ProcessAdaptLayer::InputPanElements = "InputPanElements";

const std::string ProcessAdaptLayer::CsvFormatPartKeyAttr = "CsvFormatOn";

const std::string ProcessAdaptLayer::MergeFieldsMapKey = "MergeFieldsMap";

const std::string ProcessAdaptLayer::TasseledCapInputKeyAttr = "TasseledCapInput";
const std::string ProcessAdaptLayer::TasseledCapSensorKeyAttr = "TasseledCapSensor";
const std::string ProcessAdaptLayer::TasseledCapOutputBandsKeyAttr = "TasseledCapOutputBands";

const std::string ProcessAdaptLayer::PCAnalysisInputKeyAttr = "PCAnalysisInputKey";
const std::string ProcessAdaptLayer::ExportSelectionKeyAttr = "ExportVectorSelection";
const std::string ProcessAdaptLayer::MatrixTypeKeyAttr = "MatrixTypeKey";
const std::string ProcessAdaptLayer::ComponentsCountKeyAttr = "ComponentsCountKey";
const std::string ProcessAdaptLayer::ExportMetadataKeyAttr = "ExportMetadataKey";

const std::string ProcessAdaptLayer::XMarginSizeKeyAttr = "XMarginSizeKey";
const std::string ProcessAdaptLayer::YMarginSizeKeyAttr = "YMarginSizeKey";

const std::string ProcessAdaptLayer::InputRasterElementsAttr = "InputRasterElements";

const std::string ProcessAdaptLayer::MosaicPixelDistanceAttr = "MosaicPixelDistance";
const std::string ProcessAdaptLayer::MosaicNoDataColourAttr = "MosaicNoDataColour";
const std::string ProcessAdaptLayer::MosaicRasterVectorAttr = "MosaicRasterVector";

const std::string ProcessAdaptLayer::HistogramMatchingInputAttr = "HistogramMatchingInput";
const std::string ProcessAdaptLayer::HistogramMatchingOutputBandsAttr = "HistogramOutputBands";

const std::string ProcessAdaptLayer::RasterizationVectorDatasourceAttr = "RasterizationVectorDatasource";
const std::string ProcessAdaptLayer::RasterizationAttrSelectionAttr = "RasterizationAttr";
const std::string ProcessAdaptLayer::RasterizationTypeSelectionAttr = "RasterizationTypeSelection";
/** Constructor **/
ProcessAdaptLayer::ProcessAdaptLayer() {
   this->params_ = new SerializableCollection();
}

ProcessAdaptLayer::ProcessAdaptLayer(SerializableCollection* Params) {
   this->params_ = Params;
}

/** Destructor **/
ProcessAdaptLayer::~ProcessAdaptLayer() {
   delete this->params_;
}

wxXmlNode* ProcessAdaptLayer::SerializeXml() const{
   wxXmlNode* xmlRoot = new wxXmlNode(0, wxXML_ELEMENT_NODE,
                                      PROCESS_ADAPT_LAYER_NODE);
   CreateKeyValueNode(NAME_NODE, this->processName_, xmlRoot);
   wxXmlNode* xmlParams = this->params_->SerializeXml();
   xmlRoot->AddChild(xmlParams);
   return xmlRoot;
}

ProcessAdaptLayer* ProcessAdaptLayer::DeserializeXml(wxXmlNode* pRoot){
   wxXmlNode* pChild = pRoot->GetChildren();
   std::string processName = Serializable::DeserializeString(pChild, NAME_NODE);
   pChild = pChild->GetNext();
   SerializableCollection* auxCol = SerializableCollection::DeserializeXml(pChild);
   ProcessAdaptLayer* pPal = new ProcessAdaptLayer(auxCol);
   pPal->processName_ = processName;
   return pPal;
}

/** Obtiene los atributos en un string con formato xml**/
std::string ProcessAdaptLayer::GetAttributesAsXmlString() const{
   wxXmlNode* xmlNode = SerializeXml();
   return Xml2String(xmlNode);
}

/**
 * Agrega un serializador custom a la colección.
 * El mismo es eliminado en el destructor de la clase.
 */
void ProcessAdaptLayer::AddSerializer(const std::string& KeyP,
                                      Serializer* Serializer){
   this->params_->AddSerializer(KeyP, Serializer);
}

std::string ProcessAdaptLayer::GetProcessName() const{
   return this->processName_;
}

void ProcessAdaptLayer::SetProcessName(const std::string& processName){
   this->processName_ = processName;
}

} /** namespace suri */
