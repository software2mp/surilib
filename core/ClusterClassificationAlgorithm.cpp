/** \file ClusterClassificationAlgorithm.cpp */
/*!
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

// Includes standard
#include <string>

// Includes suri
#include "ClusterClassificationAlgorithm.h"
#include "suri/XmlFunctions.h"
#include "suri/xmlnames.h"

/** namespace suri */
namespace suri {

/** Ctor */
ClusterClassificationAlgorithm::ClusterClassificationAlgorithm(
                                    const std::string &AlgorithmName) {
   algorithmName_ = AlgorithmName;
   pClusters_ = new Clusters();
}

/** Dtor */
ClusterClassificationAlgorithm::~ClusterClassificationAlgorithm() {
   delete pClusters_;
}

/**
 * Actualiza los datos internos del algoritmo con los del nodo.
 * @param[in] pAlgorithmNode nodo xml con el algoritmo
 * @return true si pudo actualizar los datos
 */
bool ClusterClassificationAlgorithm::Update(const wxXmlNode* pAlgorithmNode) {
   if (!pAlgorithmNode->GetName().IsSameAs(wxT(CLASSIFICATION_ALGORITHM_NODE)))
      return false;

   if (!pClusters_->ReadFromXml(pAlgorithmNode))
      return false;

   algorithmName_ = "";
   const wxXmlNode *pdatanode = GetNodeByName(wxT(ALGORITHM_NAME_NODE), pAlgorithmNode);
   if (pdatanode)
      algorithmName_ = pdatanode->GetNodeContent().c_str();

   noClassPixelValue_ = threshold_ = 0;
   pdatanode = GetNodeByName(wxT(TRHEASHOLD_NODE), pAlgorithmNode);
   if (pdatanode)
      threshold_ = StringToNumber<double>(pdatanode->GetNodeContent().c_str());

   pdatanode = GetNodeByName(wxT(NDV_PIXEL_VALUE_NODE), pAlgorithmNode);
   if (pdatanode)
      SetNDVPixelValue(StringToNumber<int>(pdatanode->GetNodeContent().c_str()));

   pdatanode = GetNodeByName(wxT(NO_CLASS_PIXEL_VALUE_NODE), pAlgorithmNode);
   if (pdatanode)
      noClassPixelValue_ = StringToNumber<int>(pdatanode->GetNodeContent().c_str());

   return true;
}

/**
 * Genera nodo con los datos itnernos del algoritmo.
 * @return Nodo configurado con datos del algoritmo
 * \attention el nodo generado es resposabilidad del metodo que invoca GetNode
 */
wxXmlNode* ClusterClassificationAlgorithm::GetNode() {
   wxXmlNode *palgorithmnode = new wxXmlNode(NULL, wxXML_ELEMENT_NODE,
                                           wxT(CLASSIFICATION_ALGORITHM_NODE));

   // Creo nodo con tipo de algoritmo
   new wxXmlNode(new wxXmlNode(palgorithmnode, wxXML_ELEMENT_NODE,
           wxT(ALGORITHM_NAME_NODE)), wxXML_TEXT_NODE, wxEmptyString,
           algorithmName_.c_str());

   // Creo nodo con threshold
   wxString threshold = wxString::Format(wxT("%f"), threshold_);
   new wxXmlNode(new wxXmlNode(palgorithmnode, wxXML_ELEMENT_NODE,
           wxT(TRHEASHOLD_NODE)), wxXML_TEXT_NODE, wxEmptyString, threshold);

   // Creo nodo con valor de pixel sin clase
   wxString ndvvalue = wxString::Format(wxT("%d"), GetNDVPixelValue());
   new wxXmlNode(
         new wxXmlNode(palgorithmnode, wxXML_ELEMENT_NODE, wxT(NDV_PIXEL_VALUE_NODE)),
         wxXML_TEXT_NODE, wxEmptyString, ndvvalue);

   // Creo nodo con valor de pixel sin clase
   wxString noclassvalue = wxString::Format(wxT("%d"), noClassPixelValue_);
   new wxXmlNode(new wxXmlNode(palgorithmnode, wxXML_ELEMENT_NODE,
           wxT(NO_CLASS_PIXEL_VALUE_NODE)), wxXML_TEXT_NODE, wxEmptyString, noclassvalue);

   // Agrega los nodos de las clusters
   pClusters_->WriteToXml(palgorithmnode);

   return palgorithmnode;
}

/**
 * Retorna el threshold configurado
 * @return threshold
 */
double ClusterClassificationAlgorithm::GetThreshold() const {
   return threshold_;
}

/**
 * Configura el threshold del algoritmo
 * @param[in] Threshold umbral del algoritmo
 */
void ClusterClassificationAlgorithm::SetThreshold(double Threshold) {
   threshold_ = Threshold;
}

/**
 * Retorna el valor que el algoritmo asigna a los pixels no clasificador
 * @return valor del pixel sin clase
 */
int ClusterClassificationAlgorithm::GetNoClassPixelValue() const {
   return noClassPixelValue_;
}

/**
 * Configura el valor que el algoritmo asigna a los pixels no clasificados
 * @param[in] Valor de pixels sin clase
 */
void ClusterClassificationAlgorithm::SetNoClassPixelValue(int NoClassPixelValue) {
   noClassPixelValue_ = NoClassPixelValue;
}
/**
 * Configura los clusters que se usan para clasificar
 * @param[in] pClusters clusters que usa el algoritmo para clasificar.
 * \attention pClusters pasa a ser responsabilidad de esta clase, no eliminar
 * fuera de la misma.
 */
void ClusterClassificationAlgorithm::SetClusters(Clusters* pClusters) {
   if (!pClusters)
      return;
   delete pClusters_;
   pClusters_ = pClusters;
}

/**
 * \deprecated
 * Retorna la clase que administra los clusters que se usan para clasificar
 * @return puntero a Clusters
 * \attention La clase retornada es responsabilidad de esta clase no eliminar.
 */
Clusters* ClusterClassificationAlgorithm::GetClusters() const {
   return pClusters_;
}


}  // namespace

