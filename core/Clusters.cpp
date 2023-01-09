/** \file ClassContainer.cpp */
/*!
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

// Includes standard
#include <vector>

// Includes suri
#include "Clusters.h"
#include "suri/XmlFunctions.h"
#include "SRDHistogram.h"
#include "SRDStatistics.h"

namespace suri {

/** Ctor */
Clusters::Clusters() {
}
/** Dtor */
Clusters::~Clusters() {
}


/**
 * Persiste contenedor con clusters dentro de pParentNode. Si en el nodo ya
 * existen clases las pisa.
 * @param[in] pParentNode nodo donde se deben persistir los datos
 * @return bool que indica si tuvo exito.
 */
bool Clusters::WriteToXml(wxXmlNode* pParentNode) const {
   wxXmlNode* pcontainernode = GetNodeByName(wxT(CLUSTER_CONTAINER_NODE), pParentNode);
   if (pcontainernode) {
      while (pcontainernode->GetChildren())
         pcontainernode->RemoveChild(pcontainernode->GetChildren());
   } else {
      pcontainernode = new wxXmlNode(pParentNode, wxXML_ELEMENT_NODE,
                                                  wxT(CLUSTER_CONTAINER_NODE));
   }

   int clustercount = clusters_.size();
   for (int i = 0; i < clustercount; i++) {
      wxXmlNode *pclusternode = new wxXmlNode(pcontainernode, wxXML_ELEMENT_NODE,
                                                     wxT(CLUSTER_NODE));

      // Cargo nodo con id de clase
      wxString id = wxString::Format(wxT("%d"), clusters_[i].classId_);
      new wxXmlNode(new wxXmlNode(pclusternode, wxXML_ELEMENT_NODE,
              wxT(CLASSID_NODE)), wxXML_TEXT_NODE, wxEmptyString, id);

      // Creo nodo con estadisticas
//      wxXmlNode *pstatnode = CreateStatsNode(clusters_[i].statistics_);
      wxXmlNode *pstatnode = CreateStatsNode(clusters_[i].pStatistics_, NULL, Statistics::All);
      pclusternode->AddChild(pstatnode);
   }
   return true;
}

/**
 * Lee las clases desde pParentNode
 * @param[in] pParentNode nodo que contiene a nodo contenedor de clusters
 * @return bool que indica si tuvo exito
 */
bool Clusters::ReadFromXml(const wxXmlNode* &pParentNode) {
   bool classcontainernodefound = false;
   const wxXmlNode* pcontainernode = GetNodeByName(wxT(CLUSTER_CONTAINER_NODE), pParentNode);

   // Leo las clases en contendor
   const wxXmlNode* pchildnode = pcontainernode ? pcontainernode->GetChildren() : NULL;
   while (pchildnode) {
      if (pchildnode->GetName().IsSameAs(wxT(CLUSTER_NODE))) {
         ClusterData cluterdata;
         classcontainernodefound = true;
         // Cargo propiedades clase
         wxXmlNode *pclusternode = pchildnode->GetChildren();
         while (pclusternode) {
            // Obtiene el id de la clase
            if (pclusternode->GetName() == wxT(CLASSID_NODE))
               cluterdata.classId_ = StringToNumber<int>(
                     pclusternode->GetNodeContent().c_str());

            // Obtiene las estadisticas de la clase
            if (pclusternode->GetName() == wxT(STATISTICS_NODE)) {
               suri::raster::data::HistogramBase* phist = NULL;
               // GetStats(pclusternode, cluterdata.statistics_);
               GetStats(pclusternode, cluterdata.pStatistics_, phist);
            }

            pclusternode = pclusternode->GetNext();
         }
         // Agrego clase a lista de clases.
         clusters_.push_back(cluterdata);
      }
      pchildnode = pchildnode->GetNext();
   }

   return classcontainernodefound;
}

/**
 * Retorna un vector con los clusters en contenedor
 * @return vector con clusters
 */
std::vector<Clusters::ClusterData>& Clusters::GetClusterVector() {
   return clusters_;
}

/**
 * Agrega una clase a contenedor
 * @param[in] AssociatedClassId indica el valor de la clase asociada al pixel
 * en el raster de salida.
 * @param[in] Stats estadisticas del area de entrenamiento asociada al cluster
 * @return bool que indica si tuvo exito
 */
bool Clusters::AddCluster(int AssociatedClassId, const Statistics &Stats) {
   ClusterData clusterdata;
   clusterdata.classId_ = AssociatedClassId;
   clusterdata.statistics_ = Stats;
   clusters_.push_back(clusterdata);
   return true;
}

/**
 * Agrega una clase a contenedor
 * @param[in] AssociatedClassId indica el valor de la clase asociada al pixel
 * en el raster de salida.
 * @param[in] Stats estadisticas del area de entrenamiento asociada al cluster
 * @return bool que indica si tuvo exito
 */
bool Clusters::AddCluster(int AssociatedClassId, suri::raster::data::StatisticsBase* pStats) {
   if (!pStats)
      return false;
   ClusterData clusterdata;
   clusterdata.classId_ = AssociatedClassId;
   clusterdata.pStatistics_ = pStats;
   clusters_.push_back(clusterdata);
   return true;
}
/** Elimina todas las clases del contendor */
void Clusters::Clean() {
   clusters_.clear();
}

/**
 * verifica que las dimensiones de los parametros sean consistentes
 *  (misma cantidad de bandas)
 * @return True si las estadisticas de las clases tienen la misma dimension
 * @return True si la cantidad de clases es 0.
 * @return False alguna clase tiene estadisticas con dimension distinta o invalida.
 */
bool Clusters::Check() {
   bool consistentdimensions = true;
   std::vector<int>::iterator result;
   int dimension = -1;
   return !clusters_.empty();
   if (clusters_.size() > 0) {
      dimension = clusters_[0].statistics_.GetDimension(
            Statistics::Mean | Statistics::CovarianceMatrix);
   }
   consistentdimensions = (dimension != -1);
   // Se comenta codigo ya que genera problemas para la verificacion en algunas imagenes
#ifdef __ALL_CLUSTER_CHECK__
   for (size_t i = 1; i < clusters_.size() && consistentdimensions; i++)
      // verifico contra la dimension anterior
      consistentdimensions &= (dimension
                     == clusters_[i].statistics_.GetDimension(
                        Statistics::Mean | Statistics::CovarianceMatrix));
#endif
   // retorno el valor obtenido
   return consistentdimensions;
}

}  // namespace

