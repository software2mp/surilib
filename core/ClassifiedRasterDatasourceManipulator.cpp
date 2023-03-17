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
#include <sstream>

// Includes Suri
#include "ClassifiedRasterDatasourceManipulator.h"
#include "suri/Image.h"
#include "suri/Element.h"
#include "RasterRenderer.h"
#include "RasterDatasource.h"
#include "ClassInformation.h"
#include "suri/AuxiliaryFunctions.h"

// Includes Wx
#include "wx/tokenzr.h"

// Defines

// forwards

namespace suri {

/**
 * Valor para cantidad de clases invalida
 */
const int ClassifiedRasterDatasourceManipulator::InvalidCount = -1;

/**
 * Definiciones de nombres para la opcion SizeX
 */
const std::string ClassifiedRasterDatasourceManipulator::SizeXOptionName = "SIZEX";

/**
 * Definiciones de nombres para la opcion SizeY
 */
const std::string ClassifiedRasterDatasourceManipulator::SizeYOptionName = "SIZEY";

/**
 * Definiciones de nombres para la opcion Driver
 */
const std::string ClassifiedRasterDatasourceManipulator::DriverOptionName = "DRIVER";

/**
 * Constructor
 */
ClassifiedRasterDatasourceManipulator::ClassifiedRasterDatasourceManipulator() :
      validator_() {
}

/**
 * Destructor
 */
ClassifiedRasterDatasourceManipulator::~ClassifiedRasterDatasourceManipulator() {
}

/**
 * Crea los nodos de xml para que la fuente de datos se pueda clasificar
 * Ademas incluye una clase sin clase
 * @param[in] pDatasource fuente de datos sobre la cual crear los nodos xml correspondientes.
 */
void ClassifiedRasterDatasourceManipulator::CreateXmlNodes(DatasourceInterface* pDatasource) const {
   Element* pelement = pDatasource->GetElement();
   wxXmlNode* prootnode = pelement->GetNode(wxT(""));
   wxXmlNode* pclassificationode = new wxXmlNode(NULL, wxXML_ELEMENT_NODE,
                                                 wxT(CLASSIFICATION_CLASSES_NODE));
   pelement->AddNode(prootnode, pclassificationode, false);
   ClassInformation classinformation;
   CreateXmlClassNode(&classinformation, pDatasource);
}

/**
 * Crea una nueva fuente de datos que cumple con los requisitos de validacion
 * @param[in] FileName nombre del archivo para la creacion de la fuente de datos
 * @param[in] Option Opciones para la creacion de la fuente de datos
 * @return DatasourceInterface* referencia la fuente de datos creada
 * @return NULL en caso de no poder crear la fuente de datos
 */
DatasourceInterface* ClassifiedRasterDatasourceManipulator::Create(const std::string& FileName,
                                                                   const Option& Option) const {
   size_t sizex = StringToNumber<size_t>(Option.GetOption(SizeXOptionName));
   size_t sizey = StringToNumber<size_t>(Option.GetOption(SizeYOptionName));
   std::string driver = Option.GetOption(DriverOptionName);
   if (sizex <= 0 || sizey <= 0 || driver.empty())
      return NULL;
   // Se crea de tipo "uchar"
   // El tipo de dato de la creacion debe ser compatible con el tipo de dato
   // que se utiliza en el algoritmo de fusion. Si se cambia el tipo de dato
   // se debe revisar el utilizado por ClassFussionAlgorithm
   Image* pimage = Image::Open(RasterRenderer::GenerateImageId(FileName), Image::WriteOnly, driver,
                               1, sizex, sizey, DataInfo<unsigned char>::Name);
   Image::Close(pimage);
   DatasourceInterface* prasterdatasource = DatasourceInterface::Create("RasterDatasource",
                                                                        FileName);
   if (prasterdatasource != NULL) {
      AddNature(prasterdatasource);
   }
   return prasterdatasource;
}

/**
 * Efectua la logica de agregado para un Datasource validado
 * @param[in] pDatasource fuente de datos sobre la cual se agregara la naturaleza correspondiente
 */
bool ClassifiedRasterDatasourceManipulator::AddNature(DatasourceInterface* pDatasource) const {
   if (validator_.IsValid(pDatasource))
      return true;
   CreateXmlNodes(pDatasource);
   return true;
}

/**
 * Efectua la logica de limpieza para un Datasource validado
 * eliminando el nodo xml de clasificacion
 * @param[in] pDatasource fuente de datos.
 * @return true en caso de que se limpie correctamente, false en caso contrario
 */
bool ClassifiedRasterDatasourceManipulator::CleanNature(DatasourceInterface* pDatasource) const {
   if (!validator_.IsValid(pDatasource))
      return true;
   Element* pelement = pDatasource->GetElement();
   wxXmlNode* pnode = pelement->GetNode(wxT(CLASSIFICATION_CLASSES_NODE));
   pelement->RemoveNode(pnode->GetParent(), pnode);
   return true;
}

/** 
 * Efectua la logica de copia de un Datasource a otro 
 * 
 */
bool  ClassifiedRasterDatasourceManipulator::CopyNature(
                        DatasourceInterface* pSourceDatasource,
                        DatasourceInterface* pDestinationDatasource) const {
   ClassifiedRasterDatasourceValidator validator;

   if (!validator.IsValid(pSourceDatasource))
      return false;

   // En caso que el destino sea raster clasificado valido falla la copia.
   if (validator.IsValid(pDestinationDatasource))
      return false;

   if (!AddNature(pDestinationDatasource))
      return false;

   // Configuro EnclosureInformation de destino
   bool result = true;
   std::vector<ClassInformation> classes = GetClassInformationList(pSourceDatasource);
   std::vector<ClassInformation>::iterator it = classes.begin();
   for (; it != classes.end() && result; ++it)
      result = result && AddClassInformation(&(*it), pDestinationDatasource);

   return result;
}

/** Devuelve el nodo xml correspondiente a la clase que se indica por param
 * @param[in] pDatasource fuente de datos que posee la informacion de clases
 * @param[in] Index indice de la clase que se desea buscar dentro de la fuente de datos
 * @return wxXmlNode puntero al nodo de la clase en caso de encontrarla
 * @return NULL en caso de no encontrar el nodo XML correspondiente
 */
wxXmlNode* ClassifiedRasterDatasourceManipulator::GetClassInformationXmlNode(
      DatasourceInterface* pDatasource, ClassInformation::IndexType Index) const {
   wxXmlNode* pclassnode = NULL;
   Element* pelement = pDatasource->GetElement();
   wxXmlNode* pclassificationnode = pelement->GetNode(wxT(CLASSIFICATION_CLASSES_NODE));
   if (pclassificationnode != NULL) {
      bool classfound = false;
      wxXmlNode* pnode = pclassificationnode->GetChildren();
      while (pnode != NULL && !classfound) {
         if (pnode->GetName().compare(CLASSIFICATION_CLASS_NODE) == 0) {
            wxString indexvalue = pnode->GetPropVal(wxT(CLASSIFICATION_CLASS_PROPERTY), wxT(""));
            ClassInformation::IndexType classindex = wxAtoi(indexvalue);
            classfound = (classindex == Index);
            if (classfound)
               pclassnode = pnode;
         }
         if (!classfound)
            pnode = pnode->GetNext();
      }
   }
   return pclassnode;
}

/**
 * Agrega los nodos xml correspondientes a una clase para clasificacion
 * Precondicion: La fuente de datos tiene que ser clasificada
 * @param[in] pClassInformation clase que contiene la informacion a agregar a la fuente de datos
 * @param[out] pDatasource fuente de datos sobre la cual crear los nodos de la clase
 */
void ClassifiedRasterDatasourceManipulator::CreateXmlClassNode(
      ClassInformation* pClassInformation, DatasourceInterface* pDatasource) const {
   Element* pelement = pDatasource->GetElement();
   wxXmlNode* pnode = pelement->GetNode(wxT(CLASSIFICATION_CLASSES_NODE));
   wxXmlNode* pclassnode = new wxXmlNode(pnode, wxXML_ELEMENT_NODE, wxT(CLASSIFICATION_CLASS_NODE));
   wxString wxindex = wxString::Format(wxT("%i"), pClassInformation->GetIndex());
   pclassnode->AddProperty(wxT(CLASSIFICATION_CLASS_PROPERTY), wxindex);
   std::string cname = pClassInformation->GetName();
#ifdef __WINDOWS__
   cname = wxString(wxConvUTF8.cMB2WC(cname.c_str()),
                        wxCSConv(suri::XmlElement::xmlEncoding_.c_str())).c_str();
#endif

   pelement->AddNode(pclassnode, wxT(CLASSIFICATION_CLASS_NAME_NODE), cname);
   pelement->AddNode(pclassnode, wxT(CLASSIFICATION_CLASS_COLOR_NODE),
                     wxT(pClassInformation->GetColor().GetColorAsHexaString()));
   pelement->AddNode(pclassnode, wxT(CLASSIFICATION_CLASS_ENCLOSURE_NODE),
                     wxT(pClassInformation->GetDatasourceId()));
}

/**
 * Modifca los nodos xml correspondientes a una clase para clasificacion
 * Precondicion: La fuente de datos tiene que ser clasificada
 * @param[in] pClassInformation clase que contiene la informacion a agregar a la fuente de datos
 * @param[out] pDatasource fuente de datos sobre la cual crear los nodos de la clase
 * @return true en caso de poder modificar correctamente los datos de la clase
 * @return false en caso contrario
 */
bool ClassifiedRasterDatasourceManipulator::ModifyXmlClassNode(
      ClassInformation* pClassInformation, DatasourceInterface* pDatasource) const {
   bool replaced = false;
   wxXmlNode* pnode = GetClassInformationXmlNode(pDatasource, pClassInformation->GetIndex());
   if (pnode) {
      // elimino el nodo e inserto el nuevo
      Element* pelement = pDatasource->GetElement();
      pelement->RemoveNode(pnode->GetParent(), pnode);
      CreateXmlClassNode(pClassInformation, pDatasource);
   }

   return replaced;
}

/** Agrega una clase a la fuente de datos clasificada
 * @param[in] pClassInformation referencia a la clase a agregar
 * @param[out] pDatasource fuente de datos a la cual agregar
 * @param[in] Replace indica si se desea reemplazar la clase en que caso de que exista
 * @return true en caso de que se pueda agregar correctamente la clase a la fuente de datos
 * @return false en caso de que no se pueda agregar la clase a la fuente de datos
 */
bool ClassifiedRasterDatasourceManipulator::AddClassInformation(ClassInformation* pClassInformation,
                                                                DatasourceInterface* pDatasource,
                                                                bool Replace) const {
   bool added = true;
   if (validator_.IsValid(pDatasource)) {
      if (GetClassInformationXmlNode(pDatasource, pClassInformation->GetIndex()) && Replace)
         ModifyXmlClassNode(pClassInformation, pDatasource);
      else if (!GetClassInformationXmlNode(pDatasource, pClassInformation->GetIndex()))
         CreateXmlClassNode(pClassInformation, pDatasource);
      else
         added = false;
   }
   return added;
}

/** Devuelve un id para la creacion de un nuevo datasource
 * @param[in] pDatasource
 * @return int id disponible para la creacion de una clase. Se retorna: (idmax + 1)
 * @return -1 en caso de que no se puedan obtener los ids
 */
ClassInformation::IndexType ClassifiedRasterDatasourceManipulator::GetClassInformationNewId(
      DatasourceInterface* pDatasource) const {
   std::vector<ClassInformation> classes = GetClassInformationList(pDatasource);
   std::vector<ClassInformation>::const_iterator it = classes.begin();
   ClassInformation::IndexType maxid = -1;
   for (; it != classes.end(); ++it) {
      if ((*it).GetIndex() >= maxid)
         maxid = (*it).GetIndex();
   }
   return maxid + 1;
}

/**
 * Crea una clase a partir del nodo xml, si la clase fue encontrada, o crea una clase
 * "desconocida" a partir del indice.
 * @return ClassInformation* clase "desconocida" en caso de que no se encuentre
 * @return ClassInformation* clase correspondiente al nodo xml.
 */
ClassInformation ClassifiedRasterDatasourceManipulator::CreateClassInformationFromXml(
      wxXmlNode* pNode, ClassInformation::IndexType Index, bool ClassFound) const {
   std::string name;
   VectorStyle::Color color;
   SuriObject::UuidType id;
   if (ClassFound) {
      wxXmlNode* pclassesnodes = pNode->GetChildren();
      while (pclassesnodes != NULL) {
         if (pclassesnodes->GetName().CompareTo(wxT(CLASSIFICATION_CLASS_NAME_NODE)) == 0) {
            name = WS2S(pclassesnodes->GetNodeContent());
         } else if (pclassesnodes->GetName().CompareTo(wxT(CLASSIFICATION_CLASS_COLOR_NODE)) == 0) {
            std::string hexa = pclassesnodes->GetNodeContent().c_str();
            color.SetColorFromHexaString(hexa);
         } else if (pclassesnodes->GetName().CompareTo(wxT(CLASSIFICATION_CLASS_ENCLOSURE_NODE))
               == 0) {
            id = WS2S(pclassesnodes->GetNodeContent());
         }
         pclassesnodes = pclassesnodes->GetNext();
      }
   } else {
      std::stringstream stringindex;
      stringindex << ClassInformation::DefaultClassNotFoundName << Index;
      name = stringindex.str();
   }
   return ClassInformation(Index, name, color, id);
}

/** Obtiene una clase de la fuente de datos clasificada a partir de un indice
 * Esta clase no se hace cargo del ciclo de vida del ClassInformation devuelto.
 * Se debe liberar el recurso una vez termiada su utilizacion
 * @param[in] pDatasource fuente de datos sobre la cual obtener la clase
 * @param[in] Index indice de la clase
 * @return ClassInformation* referencia a la clase que corresponde al indice. En caso
 * de tener un indice que no se encuentra en la fuente de datos, se devolvera una classinformation
 * con un nombre "Clase Desconocida #indice"
 */
ClassInformation ClassifiedRasterDatasourceManipulator::GetClassInformation(
      DatasourceInterface* pDatasource, ClassInformation::IndexType Index) const {
   wxXmlNode* pnode = GetClassInformationXmlNode(pDatasource, Index);
   bool classfound = (pnode != NULL);
   ClassInformation classinformation = CreateClassInformationFromXml(pnode, Index, classfound);
   return classinformation;
}

/** Obtiene la cantidad de clases dentro de una fuente de datos clasificada
 * @param[in] pDatasource fuente de datos sobre la cual obtener la cantidad de clases
 * @return cantidad de clases que posee la fuente de datos
 * @return -1 en caso de que la fuente de datos no sea clasificada
 */
int ClassifiedRasterDatasourceManipulator::GetClassInformationCount(
      DatasourceInterface* pDatasource) const {
   Element* pelement = pDatasource->GetElement();
   wxXmlNode* pclasifnode = pelement->GetNode(wxT(CLASSIFICATION_CLASSES_NODE));
   if (pclasifnode == NULL)
      return ClassifiedRasterDatasourceManipulator::InvalidCount;
   int classcount = 0;
   wxXmlNode* pnode = pclasifnode->GetChildren();
   while (pnode != NULL) {
      if (pnode->GetName().compare(CLASSIFICATION_CLASS_NODE) == 0) {
         ++classcount;
      }
      pnode = pnode->GetNext();
   }
   return classcount;
}

/**
 * Devuelve una lista con las clases dentro de la fuente de datos
 * @param[in] pDatasource fuente de datos sobre la cual se obtendra la lista de clases
 * @return std::vector<ClassInformation> lista de class information del raster clasificado
 */
std::vector<ClassInformation> ClassifiedRasterDatasourceManipulator::GetClassInformationList(
      DatasourceInterface* pDatasource) const {
   std::vector<ClassInformation> classvector;
   Element* pelement = pDatasource->GetElement();
   wxXmlNode* pclasifnode = pelement->GetNode(wxT(CLASSIFICATION_CLASSES_NODE));
   if (pclasifnode) {
      wxXmlNode* pnode = pclasifnode->GetChildren();
      while (pnode != NULL) {
         if (pnode->GetName().compare(CLASSIFICATION_CLASS_NODE) == 0) {
            wxString indexvalue = pnode->GetPropVal(wxT(CLASSIFICATION_CLASS_PROPERTY), wxT(""));
            ClassInformation::IndexType classindex = wxAtoi(indexvalue);
            ClassInformation classinformation = CreateClassInformationFromXml(pnode, classindex);
            classvector.push_back(classinformation);
         }
         pnode = pnode->GetNext();
      }
   }
   return classvector;
}

} /** namespace suri */
