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

// Includes Suri
#include "EnclosureManipulator.h"
#include "wxmacros.h"
#include "EnclosureValidator.h"
#include "VectorDatasource.h"
#include "suri/VectorEditor.h"
#include "suri/Vector.h"
#include "suri/xmlnames.h"

// Includes Wx
#include "wx/filename.h"

// Defines

// forwards
class OGRLayer;

namespace suri {


// Definiciones para nombres de optiones
const std::string EnclosureManipulator::SpatialReferenceOptionName = "SPATIALREFERENCE";
const std::string EnclosureManipulator::DriverOptionName = "DRIVER";

/**
 * Constructor
 */
EnclosureManipulator::EnclosureManipulator(bool TrainArea) : trainArea_(TrainArea) {
}

/**
 * Destructor
 */
EnclosureManipulator::~EnclosureManipulator() {
}

/**
 * Metodo auxiliar para crear los campos para un recito
 * @param[in] EnclosureInformation informacion del recinto
 * @param[out] pDatasource fuente de datos donde se deben crear los campos
 * @return true en caso de poder crear los campos, false en caso contrario
 */
bool EnclosureManipulator::CreateEnclosureFields(
                        EnclosureInformation &EnclosureInformation,
                        DatasourceInterface* pDatasource) const {
   return CreateEnclosureFields(EnclosureInformation, pDatasource->GetElement());
}

/** Metodo auxiliar para crear los campos para un recito */
bool EnclosureManipulator::CreateEnclosureFields(EnclosureInformation &EnclosureInformation,
                           Element* pElement) const {
   std::string fieldname = pElement->GetUrl().c_str();
   VectorElement* pvectordatasource = dynamic_cast<VectorElement*>(pElement);
   int layerindex = pvectordatasource->GetActiveLayer();

   VectorEditor vectoreditor;
   if (!vectoreditor.OpenVector(fieldname) || !vectoreditor.OpenLayer(layerindex))
      return false;
   vectoreditor.CreateField(EnclosureInformation.GetTypeAreaName(),
                            EnclosureValidator::TypeRecord::Name,
                            EnclosureValidator::TypeRecord::Size);
   vectoreditor.CreateField(EnclosureInformation.GetDescAreaName(),
                            EnclosureValidator::DescRecord::Name,
                            EnclosureValidator::DescRecord::Size);
   for (int i = 0; i < vectoreditor.GetFeatureCount(); ++i) {
      if (vectoreditor.OpenFeature(vectoreditor.GetFeatureId(i)) >= 0) {
         vectoreditor.SetField(EnclosureInformation.GetTypeAreaName(), trainArea_ ? 0 : 1);
      }
   }
   
   return true;
}

/** Metodo auxiliar para la creacion del conteneido del nodo consulta del xml del recinto
 * @param[in] LayerName nombre de la capa sobre la cual realizar la consulta
 * @param[in] FieldName nombre del campo
 * @param[in] Condition condicion sobre la cual evaluar al campo de la consulta
 * @return string con el query correspondiente a los parametros dados **/
std::string EnclosureManipulator::CreateQuery(const std::string& LayerName,
                                              const std::string& FieldName,
                                              const std::string& Condition) const {
   std::string query = "SELECT * FROM ";
   query += LayerName;
   query += " WHERE ";
   query += FieldName;
   query += Condition;
   return query;
}

/**
 * Metodo auxiliar para crear los nodos xml para un recito
 * \deprecated
 * @param[in] EnclosureInformation informacion del recinto
 * @param[out] pElement elemento sobre el cual se crean los nodos xml
 * @return true en caso de poder crear correctamente los nodos xml
 */
bool EnclosureManipulator::CreateEnclosureXmlNodes(
                           EnclosureInformation &EnclosureInformation,
                           Element* pElement) const {
   wxXmlNode* prootnode = pElement->GetNode(wxT(""));
   wxXmlNode* penclosurenode = new wxXmlNode(NULL, wxXML_ELEMENT_NODE,
                                             wxT(ENCLOSURE_NODE));
   if (prootnode == NULL || penclosurenode == NULL)
      return false;
   wxXmlNode* poldenclosurenode = pElement->GetNode(wxT(ENCLOSURE_NODE));
   /** si existia el nodo lo elimino para actualizarlo **/
   if (poldenclosurenode)
      pElement->RemoveNode(pElement->GetNode(), poldenclosurenode);
   pElement->AddNode(prootnode, penclosurenode, true);
   pElement->AddNode(penclosurenode, wxT(ENCLOSURE_TYPE_AREA_NODE),
          EnclosureInformation.GetTypeAreaName(), wxT(ENCLOSURE_TYPE_PROPERTY),
          EnclosureInformation.GetTypeAreaFieldType(), false);
   pElement->AddNode(penclosurenode, wxT(ENCLOSURE_DESC_NODE),
          EnclosureInformation.GetDescAreaName(), wxT(ENCLOSURE_TYPE_PROPERTY),
          EnclosureInformation.GetDescFieldType(), false);
   pElement->AddNode(penclosurenode, wxT(ENCLOSURE_QUERY_NODE),
                              wxT(EnclosureInformation.GetQuery()));   
   pElement->AddNode(penclosurenode, wxT(ENCLOSURE_TYPE_NODE),
                              wxT(trainArea_ ? TRAIN_ENCLOSURE_TYPE : TEST_ENCLOSURE_TYPE));

   return true;
}

/**
 * Crea una nueva fuente de datos que cumple con los requisitos de validacion de Enclosure
 * @param[in] FileName nombre del archivo
 * @param[in] Option Opciones para la creacion de la fuente de datos
 * @return DatasourceInterface* Nueva fuente de datos valida como Enclosure
 */
DatasourceInterface* EnclosureManipulator::Create(const std::string& FileName,
                                                  const Option& Option) const {
   DatasourceInterface* pdatasource = NULL;
   wxFileName file(FileName);
   Vector* pvector = Vector::Open(FileName, Vector::WriteOnly, 1, Vector::Polygon);
   if (pvector) {
      std::string spatialreference = Option.GetOption(SpatialReferenceOptionName);
      OGRLayer* player = pvector->CreateLayer(file.GetName().c_str(), spatialreference,
                                              Vector::Polygon);
      pvector->Close(pvector);
      if (player)
         pdatasource = DatasourceInterface::Create("VectorDatasource", FileName);
      if (pdatasource != NULL)
         AddNature(pdatasource);
   }
   return pdatasource;
}

/**
 * Obtiene el nombre de la capa activa
 * @param[in] DatasourceUrl url de la fuente de datos
 * @param[in] LayerIndex indice de la capa activa
 * @return string con el nombre de la capa.
 * En caso de no poder obtener el nombre, devuelve un string vacia
 */
std::string EnclosureManipulator::GetActiveLayerName(const std::string DatasourceUrl,
                                                     int LayerIndex) const {
   /**
    *  se utilizan variables estaticas para cachear los valores y evitar
    *  que se intente abrir el vector en cada llamada
    */
   static int layerindex = 0;
   static std::string layername;
   static std::string dsurl;
   if (layerindex == LayerIndex && dsurl.compare(DatasourceUrl) == 0
         && !layername.empty())
      return layername;
   dsurl = DatasourceUrl;
   VectorEditor vectoreditor;
   if (vectoreditor.OpenVector(DatasourceUrl) && vectoreditor.OpenLayer(LayerIndex))
      layername = vectoreditor.GetOpenLayerName();
   layerindex = LayerIndex;
   return layername;
}

/**
 * Efectua la logica de agregado para un Datasource validado
 * @param[out] pDatasource
 * @return true en caso de poder agregar la naturaleza de recinto
 * @return false en caso de que no se pueda abrir el archivo del datasource
 */
bool EnclosureManipulator::AddNature(DatasourceInterface* pDatasource) const {
   EnclosureValidator validator;
   if (!validator.IsValidDatasourceType(pDatasource))
      return false;
   if (validator.IsValid(pDatasource)) {
      EnclosureInformation einfo = GetEnclosureInformation(pDatasource);
      EnclosureInformation::EnclosureType desiretype = trainArea_ ?
                                 EnclosureInformation::Train : EnclosureInformation::Test;
      if (einfo.GetEnclosureType() == desiretype)
         return true;
   }
   VectorDatasource* pvectordatasource = dynamic_cast<VectorDatasource*>(pDatasource);
   bool arefieldscreated = false;
   bool isxmlcreated = false;
   if (pvectordatasource) {
      int layerindex = pvectordatasource->GetActiveLayer();
      std::string layername = GetActiveLayerName(pDatasource->GetUrl(), layerindex);
      std::string condition = trainArea_ ? " IN(0)": " IN(1)";
      std::string querycontent =
                  CreateQuery(layername, EnclosureInformation::TypeNameDefault, condition);
      EnclosureInformation enclosureinfo(layername, querycontent,
                  trainArea_ ? EnclosureInformation::Train : EnclosureInformation::Test);
      arefieldscreated = CreateEnclosureFields(enclosureinfo, pDatasource);
      isxmlcreated = CreateEnclosureXmlNodes(enclosureinfo, pDatasource->GetElement());
   }
   return (arefieldscreated && isxmlcreated);
}

bool EnclosureManipulator::AddNature(Element* pElement) const {
   VectorElement* pvectorelement = dynamic_cast<VectorElement*>(pElement);
   if (!pvectorelement)
      return false;
   std::string layername = pvectorelement->GetActiveLayerName();
   VectorEditor vectoreditor;
   std::string querycontent =
            CreateQuery(layername, EnclosureInformation::TypeNameDefault);
   EnclosureInformation enclosureinfo(layername, querycontent,
               trainArea_ ? EnclosureInformation::Train : EnclosureInformation::Test);
   return CreateEnclosureXmlNodes(enclosureinfo, pElement);
}

/**
 * Efectua la logica de limpieza para un Datasource validado
 * @param[out] pDatasource
 * @return true en caso de borrar el comportamiento de recinto
 * @return false en caso contrario
 */
bool EnclosureManipulator::CleanNature(DatasourceInterface* pDatasource) const {
   EnclosureValidator validator;
   if (!validator.IsValid(pDatasource))
      return true;
   Element* pelement = pDatasource->GetElement();
   wxXmlNode* pnode = pelement->GetNode(wxT(ENCLOSURE_NODE));
   pelement->RemoveNode(NULL, pnode);
   return true;
}

/**
 * Efectua la logica de copia de los datos del enclosure de un datasource
 * a otro
 * @param[in] pSourceDatasource recinto que quiero copiar
 * @param[in] pDestinationDatasource datasource donde hay que copiar
 * los datos del recinto
 * @return bool que indica si tuvo exito
 */
bool EnclosureManipulator::CopyNature(DatasourceInterface* pSourceDatasource,
                           DatasourceInterface* pDestinationDatasource) const {
   EnclosureValidator validator;

   if (!validator.IsValidDatasourceType(pSourceDatasource)
                  || !validator.IsValidDatasourceType(pDestinationDatasource)
                  || !validator.IsValid(pSourceDatasource))
      return false;

   // En caso que el destino sea un enclosure valido falla la copia.
   if (validator.IsValid(pDestinationDatasource))
      return false;

   // Configuro EnclosureInformation de destino
   EnclosureInformation sourceinfo = GetEnclosureInformation(pSourceDatasource);
   VectorDatasource* pvectordatasource = dynamic_cast<VectorDatasource*>(
                                                      pDestinationDatasource);
   std::string layername = GetActiveLayerName(pvectordatasource->GetUrl(),
                                           pvectordatasource->GetActiveLayer());
   std::string querycontent = CreateQuery(layername, sourceinfo.GetTypeAreaName());
   EnclosureInformation destinationinfo(layername, querycontent,
            trainArea_ ? EnclosureInformation::Train : EnclosureInformation::Test,
            sourceinfo.GetTypeAreaName(), sourceinfo.GetDescAreaName());

   // Configuro destino con el enclosure
   return CreateEnclosureFields(destinationinfo, pDestinationDatasource)
           && CreateEnclosureXmlNodes(destinationinfo, pDestinationDatasource->GetElement());
}

/** 
 * Devuelve la informacion de recinto de la fuente de datos
 * No es resonsabilidad de la clase la eliminacion de la referencia del enclosure.
 * @param[in] pDatasource recinto que quiero analizar
 * @return EnclosureInformation clase con la informacion que posee la fuente de datos
 * @return EnclosureInformation clase con informacion vacia en caso de no poder
 * obtener el recinto solicitado
 */
EnclosureInformation EnclosureManipulator::GetEnclosureInformation(
                                   DatasourceInterface* pDatasource) const {
   return GetEnclosureInformation(pDatasource->GetElement());
}
/** 
 * Devuelve la informacion de recinto de la fuente de datos
 * No es resonsabilidad de la clase la eliminacion de la referencia del enclosure.
 * @param[in] pElement recinto que quiero analizar
 * @return EnclosureInformation clase con la informacion que posee la fuente de datos
 * @return EnclosureInformation clase con informacion vacia en caso de no poder
 * obtener el recinto solicitado
 */
EnclosureInformation EnclosureManipulator::GetEnclosureInformation(Element* pElement) const {
   VectorElement* pvelem = dynamic_cast<VectorElement*>(pElement);
   if (!pvelem)
      return EnclosureInformation("", "", EnclosureInformation::Train);
   std::string layername = pvelem->GetActiveLayerName();

   wxXmlNode* pnode = pElement->GetNode(wxT(ENCLOSURE_NODE));
   std::string typearea, descarea, query, area;
   if (pnode) {
      wxXmlNode* pchild = pnode->GetChildren();
      // valor por defecto que devuelve el propval si no existe la propiedad;
      while (pchild != NULL) {
         if (pchild->GetName().CompareTo(ENCLOSURE_TYPE_AREA_NODE) == 0) {
            wxString propval = pchild->GetPropVal(wxT(ENCLOSURE_TYPE_PROPERTY),
                                                  wxT(" "));
            if (propval.CompareTo(wxT(EnclosureInformation::TypeRecord::Name)) == 0) {
               typearea = pchild->GetNodeContent();
            }
         } else if (pchild->GetName().CompareTo(ENCLOSURE_DESC_NODE) == 0) {
            wxString propval = pchild->GetPropVal(wxT(ENCLOSURE_TYPE_PROPERTY),
                                                  wxT(" "));
            if (propval.CompareTo(wxT(EnclosureInformation::DescRecord::Name)) == 0) {
               descarea = pchild->GetNodeContent();
            }
         } else if (pchild->GetName().CompareTo(ENCLOSURE_QUERY_NODE) == 0) {
            query = pchild->GetNodeContent();
         } else if (pchild->GetName().CompareTo(ENCLOSURE_TYPE_NODE) == 0) {
            area = pchild->GetNodeContent();
         }
         pchild = pchild->GetNext();
      }
   }
   bool trainarea = area.compare(TRAIN_ENCLOSURE_TYPE) == 0;
   EnclosureInformation enclosureinformation(layername, query,
      trainarea ? EnclosureInformation::Train : EnclosureInformation::Test, typearea, descarea);
   return enclosureinformation;
}
} /** namespace suri */
