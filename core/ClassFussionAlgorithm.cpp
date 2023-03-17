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
#include "ClassFussionAlgorithm.h"
#include "suri/DataTypes.h"
#include "suri/xmlnames.h"
#include "logmacros.h"
#include "suri/AuxiliaryFunctions.h"

// Includes Wx
#include "wx/xml/xml.h"
// Defines

// forwards

namespace suri {

const int ClassFussionAlgorithm::NoClusterIndex = 0;

/** Registra la clase entre los tipos de algoritmos de clasificacion */
AUTO_REGISTER_CLASS(ClassificationAlgorithmInterface, ClassFussionAlgorithm, 0)


/** Funcion clasificacion */
typedef bool (*ClassFussionFunctionType)(
      int*, std::vector<void*>&, size_t, int, double, bool,
      const ClassFussionAlgorithm::FussionClasesTables& FussionTable);

/** Template ClassFussion */
/**
 * Realiza la fusion de las clases sobre la imagen
 * utilizando el mapeo que se pasa por parametro
 * @param[out] pDest imagen con ints donde se guardan los ids
 * @param[in] pSource imagen que se quiere clasificar
 * @param[in] Size cantidad de pixeles en la imagen a clasificar
 * @param[in] FussionTable mapa de asignacion para la fusion de clases
 */
template<typename T>
bool ClassFussion(int* pDest, std::vector<void*> &pSource, size_t Size,
                  int NDVPixelValue, double Ndv, bool NdvAvailable,
                  const ClassFussionAlgorithm::FussionClasesTables& FussionTable) {
   if (pSource.size() != 1)
      return false;
   std::vector<T*> psrc;
   psrc.push_back(static_cast<T*>(pSource[0]));
   // Para cada pixel en la imagen
   for (size_t pixelposition = 0; pixelposition < Size; ++pixelposition) {
      bool invaliddn = true;
      if (static_cast<double>(psrc[0][pixelposition]) != CLASSIFICATION_NDV)
         invaliddn = false;

      if (invaliddn == false) {
         ClassFussionAlgorithm::IndexValue index = psrc[0][pixelposition];
         ClassFussionAlgorithm::FussionClasesTables::const_iterator it =
               FussionTable.lower_bound(index);
         if (index != ClassFussionAlgorithm::NoClusterIndex
               && it != FussionTable.end()) {
            pDest[pixelposition] = it->second;
         } else {
            pDest[pixelposition] = ClassFussionAlgorithm::NoClusterIndex;
         }
      } else {
         pDest[pixelposition] = NDVPixelValue;
      }
   }
   return true;
}

/** Inicializa mapa de tipos de datos. */
INITIALIZE_DATATYPE_MAP(ClassFussionFunctionType, ClassFussion);

/** ctor */
ClassFussionAlgorithm::ClassFussionAlgorithm() {
}

/** dtor */
ClassFussionAlgorithm::~ClassFussionAlgorithm() {
}

/**
 *  Configura el mapeo entre clase origen (a fusionar) y clase destino
 *  @param[in] FussionTable map que tiene la informacion de mapeo entre clases
 */
void ClassFussionAlgorithm::SetFussionTable(const FussionClasesTables& FussionTable) {
   fussionTable_ = FussionTable;
}

/**
 * Inicializa la instancia a partir de un string
 * @param[in] InitializeString
 * @return ClassFussionAlgortihm*
 */
ClassFussionAlgorithm* ClassFussionAlgorithm::Initialize(const std::string& InitializeString) {
   return this;
}

/**
 * Verifica que el nodo cluster/nocluster tengan la estructura correcta
 * @param[in] pNode nodo xml cluster/nocluster
 * @param[in] IsNoCluster indica si el nodo corresponde a un cluster o nocluster
 * @return true en caso de que sea valida
 * @return false en caso contrario
 */
bool ClassFussionAlgorithm::IsClusterNodeValid(const wxXmlNode* pNode, bool IsNoCluster) {
   bool hasindex = false;
   bool hasfussionindex = false;
   wxXmlNode* pchild = pNode->GetChildren();
   while (pchild != NULL) {
      if (pchild->GetName().CompareTo(CLUSTER_INDEX_NODE) == 0) {
         hasindex = true;
      } else if (pchild->GetName().CompareTo(CLUSTER_FUSSION_INDEX) == 0) {
         hasfussionindex = true;
      }
      pchild = pchild->GetNext();
   }
   bool valid =
         IsNoCluster ? (hasindex && !hasfussionindex) : (hasindex && hasfussionindex);
   return valid;
}

/** Metodo auxiliar para la validacion del xml
 * Se considera un xml valido con una estructura como la siguiente:
 * \verbatim
 * <classificacion>
 *        <algoritmo>
 *          <tipo>ClassFussionAlgorithm</tipo>
 *          <umbral></umbral>
 *          <nocluster>
 *                 <indice>0</indice> !-- nocluster no puede tener indice anterior.
 *          </nocluster>
 *          <cluster>
 *               <indice>1</indice> <!-- Indice que posee el cluster -->
 *                <!--Indice del cluster sobre el cual se fusionar -->
 *                <indice_fusion>0</indice_anterior>
 *          </cluster>
 *        </algoritmo>
 * </classificacion>
 * \endverbatim
 * @param[in] pNode nodo del xml que se quiere validar.
 * @return true en caso de que la estructura del xml sea valida
 * @return false en caso de que el xml sea invalido*/
bool ClassFussionAlgorithm::IsXmlValid(const wxXmlNode* pNode) {
   bool isvalid = true;
   while (pNode != NULL && isvalid) {
      if (pNode->GetName().CompareTo(wxT(ALGORITHM_NAME_NODE)) == 0) {
         isvalid = (pNode->GetNodeContent().CompareTo(wxT(ALGORITHM_VALUE_CLASSFUSION))
               == 0);
      } else if (pNode->GetName().CompareTo(wxT(NOCLUSTER_NODE)) == 0) {
         isvalid = IsClusterNodeValid(pNode, true);

      } else if (pNode->GetName().CompareTo(wxT(CLUSTER_NODE)) == 0) {
         isvalid = IsClusterNodeValid(pNode);
      }
      pNode = pNode->GetNext();
   }
   return isvalid;
}

/** Actualiza los datos internos del algoritmo con los del nodo.
 * @param[in] pAlgorithmNode nodo con la definicion de los parametros del algoritmo
 * @return true en caso de que configuren los parametros correctamente
 * @return false en caso de que el nodo tenga estructura invalida
 */
bool ClassFussionAlgorithm::Update(const wxXmlNode* pAlgorithmNode) {
   fussionTable_.clear();
   return XmlToMapFussion(pAlgorithmNode);
}

/** Genera nodo con los datos internos del algoritmo.
 * No es responsabilidad de esta clase la destruccion del nodo pedido.
 * @return wxXmlNode* nodo con los datos del algoritmo
 */
wxXmlNode* ClassFussionAlgorithm::GetNode() {
   wxXmlNode *palgorithmnode = new wxXmlNode(NULL, wxXML_ELEMENT_NODE,
                                             CLASSIFICATION_ALGORITHM_NODE);
   // Creo nodo con tipo de algoritmo
   // No se pierde la referencia ya que se pasa palgorithmnode como parent, del new interior y
   // este objeto como parent del new exterior, por lo tanto palgorithmnode da acceso a ambos.
   new wxXmlNode(new wxXmlNode(palgorithmnode, wxXML_ELEMENT_NODE,
           ALGORITHM_NAME_NODE), wxXML_TEXT_NODE, wxEmptyString,
           ALGORITHM_VALUE_CLASSFUSION);
   if (FussionMapToXml(palgorithmnode)) {
      return palgorithmnode;
   } else {
      delete palgorithmnode;
      return NULL;
   }
}

/**
 * Clasifica los datos de entrada(pSource) y guarda en pDest un entero con
 * el id de la clase ganadora.
 * Precondicion: La imagen a procesar es una imagen raster clasificada
 * Esto quiere decir que pose solo una banda, de tipo de dato entero.
 * Precondicion: Se configuro el mapeo de fusion de clases mediante el metodo
 * SetFussionTable.
 * Si no se realizo la configuracion del mapeo por defecto se realiza una fusion
 * de todas las clases a la no clase.
 * @param[out] pDest destino de la clasificacion
 * @param[in] pSource fuente de datos para la clasificacion
 * @param[in] Size cantidad de datos
 * @param[in] DataType tipo de dato de la imagen.
 * @return true en caso de clasificar correctamente la imagen
 * @return false en caso de poder clasificar la imagen
 */
bool ClassFussionAlgorithm::Classify(int* pDest, std::vector<void*> pSource,
                                     size_t Size, const std::string &DataType) {
   ClassFussionFunctionType function = ClassFussionTypeMap[DataType];
   return function(pDest, pSource, Size, GetNDVPixelValue(), GetNoDataValue(),
                   IsNoDataValueAvailable(), fussionTable_);
}

/** Verifica que el algoritmo este bien configurado
 *  @return true en caso de que el algoritmo este correctamente configurado
 *  @return false en caso contrario (no existe ese caso todavia).
 */
bool ClassFussionAlgorithm::Check() const {
   return true;
}

/**
 * Retorna la cantidad de cluster origen que posee la asociacion
 * @return int cantidad de cluster origen
 * @return 0 en caso de no tener configurada la asociacion
 */
int ClassFussionAlgorithm::GetClassCount() const {
   return fussionTable_.size();
}

/** Convierte el map seteado al xml
 *  Se devuelve un xml valido para la definicion del algoritmo de fusion.
 *  No es responsabilidad de esta clase la vida del nodo devuelto
 *  @param[out] pParentNode referencia al nodo padre que contendra la asosiacion
 *  @return true si se puede crear la configuracion en xml de la asociacion
 *  @return false en caso contrario
 */
bool ClassFussionAlgorithm::FussionMapToXml(wxXmlNode* pParentNode) {
   FussionClasesTables::iterator it = fussionTable_.begin();
   // Creo el nodo nocluster primero
   wxXmlNode* pnode = new wxXmlNode(pParentNode, wxXML_ELEMENT_NODE, NOCLUSTER_NODE);
   new wxXmlNode(pnode, wxXML_ELEMENT_NODE, CLUSTER_INDEX_NODE,
                 NumberToString<int>(NoClusterIndex));
   for (; it != fussionTable_.end(); ++it) {
      wxXmlNode* pnode = new wxXmlNode(pParentNode, wxXML_ELEMENT_NODE, CLUSTER_NODE);
      new wxXmlNode(pnode, wxXML_ELEMENT_NODE, CLUSTER_INDEX_NODE,
                    NumberToString<IndexValue>(it->first));
      new wxXmlNode(pnode, wxXML_ELEMENT_NODE, CLUSTER_FUSSION_INDEX,
                    NumberToString<IndexValue>(it->second));
   }
   return true;
}

/**
 * A partir del nodo cluster crea la informacion para insertar al mapa de fusion
 * @param[in] pNode nodo cluster que se quiere crear la entrada en el mapa
 * @return true en caso de que el nodo cluster tenga la informacion necesaria para
 * insertar en el mapa
 * @return false en caso contrario
 */
bool ClassFussionAlgorithm::InsertFussionMapFromXml(const wxXmlNode* pNode) {
   wxXmlNode* pchildren = pNode->GetChildren();
   IndexValue index = -1;
   IndexValue fussionindex = -1;
   while (pchildren != NULL) {
      if (pchildren->GetName().CompareTo(CLUSTER_INDEX_NODE) == 0) {
         index = StringToNumber<IndexValue>(pchildren->GetContent().c_str());
      } else if (pchildren->GetName().CompareTo(CLUSTER_FUSSION_INDEX) == 0) {
         fussionindex = StringToNumber<IndexValue>(pchildren->GetContent().c_str());
      }
      pchildren = pchildren->GetNext();
   }
   // Se valida que ademas no este la clase "noclase" como origen
   if (index <= 0 || fussionindex == -1)
      return false;
   FussionClasesTablesPair pair(index, fussionindex);
   fussionTable_.insert(pair);
   return true;
}

/** Convierte el xml a el mapea de asignacion
 * @param[in] pNode referencia a nodo xml que contiene la configuracion del algoritmo
 * @return true en aso de poder configurar el algoritmo de forma correcta
 * @return false en caso contrario
 */
bool ClassFussionAlgorithm::XmlToMapFussion(const wxXmlNode* pNode) {
   wxXmlNode* pchildren = pNode->GetChildren();
   while (pchildren != NULL) {
      if (pchildren->GetName().CompareTo(CLUSTER_NODE) == 0) {
         InsertFussionMapFromXml(pchildren);
      }
      pchildren = pchildren->GetNext();
   }

   return true;
}
} /** namespace suri */
