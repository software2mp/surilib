/*! \file AnotationElement.cpp
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

// Includes standard
#include <string>
#include <vector>

// Includes Suri
#include "AnotationElement.h"
#include "suri/xmlnames.h"
#include "suri/Vector.h"
#include "suri/VectorStyle.h"
#include "suri/messages.h"
#include "AnotationPropertiesPart.h"
#include "VectorTablesPart.h"
#include "resources.h"
#include "wxmacros.h"

// Includes Wx
#include "wx/xrc/xmlres.h"
#include "wx/valgen.h"
#include "wx/clrpicker.h"
#include "wx/spinctrl.h"

// Includes App

// Includes OGR
#include "ogrsf_frmts.h"

// Defines
/**
 *  Se usa en seymour porque las anotaciones tienen un unico feature y nombre=anotacion.
 */
#define __USE_ELEMENT_NAME_AS_ANOTATION_VALUE__

// namespace suri
namespace suri {
// Macro para registrar el tipo de elemento
AUTO_REGISTER_CLASS(Element, AnotationElement, 0)

/**
 * Ctor. default.
 */
AnotationElement::AnotationElement() {
}

/**
 * Dtor.
 */
AnotationElement::~AnotationElement() {
}

AnotationElement::AnotationElement(const AnotationElement &Anotation) {
}

/**
 * Metodo estatico para crear un elemento Anotacion(genera el xml del elemento)
 * a partir de una serie de datos.
 * Verifica que:
 *    - se pueda crear el vector en memoria.
 *    - el vector creado tenga al menos 1 capa.
 *    - se pueda crear la anotacion en el punto pasado por parametro.
 * @param[in] FileName : nombre para el vector en memoria
 * @param[in]	Anotation: cadena utilizada para crear vector en memoria(nombre
 * del vector), para establecer el nombre del elemento y el contenido en la base
 * de datos del vector.
 * @param[in]	SpatialReference: Referencia Espacial del vector
 * @param[in]	Position: Posicion de la anotacion.
 * @return puntero a un elemento Anotacion.
 * @return NULL en caso de no poder crear la anotacion.
 * \attention quien invoque a este metodo es responsable del puntero devuelto,
 * por lo tanto debe hacerse cargo de su eliminacion, La eliminacion
 * se debe realizar mediante el metodo Destroy().
 * \post Este metodo crea el vector (shp) asociado a la anotacion en memoria.
 * \attention Se debe utilizar el metodo Save para guardarlo en un archivo.
 */
AnotationElement* AnotationElement::Create(const std::string &FileName,
                                           const wxString &Anotation,
                                           const std::string &SpatialReference,
                                           const Coordinates &Position) {
   // Creo el element mediante la factory, para que lo registre
   // la destruccion se debe hacer mediante Destroy()
   AnotationElement *pelement = dynamic_cast<AnotationElement*>(Element::Create(
         "AnotationElement"));

   if (!pelement) {
      REPORT_AND_FAIL_VALUE("D:Error al crear el element", NULL);
   }

   // crea un vector en memoria
   Vector *pvector = Vector::Open(FileName.c_str());
   // Fallo la creacion del vector
   if (!pvector) {
      delete pelement;
      REPORT_AND_FAIL_VALUE("D:Error al crear el vector", NULL);
   }
   // Creo el campo de la anotacion
   OGRFieldDefn field(SUR_ANOTATION_FIELD, OFTString);

   // Obtengo capa
   OGRLayer *player = pvector->GetLayer(pelement->GetActiveLayer());
   if (!player) {
      Vector::Close(pvector);
      delete pelement;
      REPORT_AND_FAIL_VALUE("D:Error al obtener capa. No se pudo crear anotacion", NULL);
   }

   // \todo debo usar puntero y no eliminar?
   player->CreateField(&field, TRUE);
   pelement->InitVector(pvector, pelement->GetUrl().c_str());
   pelement->SetName(Anotation);
   Vector::Close(pvector);

   /* \todo (12/08/2009 - Alejandro): si se quiere crear una Anotacion sin
    * utilizar la herramienta de dibujo para Anotaciones, hay que implementar el
    * uso del VectorStyleManager en los metodos AnotationElement::Create para
    * setear el estilo deseado a la Anotacion que se esta creando, ya que en el
    * metodo VectorElement::Initialize se le setea un estilo de la categoria
    * "Default" la cual no posee estilo para Anotacion.
    */

   OGRFeature *panotationfeature = OGRFeature::CreateFeature(player->GetLayerDefn());
   OGRPoint *ppoint = dynamic_cast<OGRPoint*>(OGRGeometryFactory::createGeometry(
         wkbPoint));
   if (ppoint) {
      ppoint->setX(Position.x_);
      ppoint->setY(Position.y_);
      ppoint->setZ(Position.z_);
      panotationfeature->SetGeometry(ppoint);
   } else {
      delete pelement;
      REPORT_AND_FAIL_VALUE("D:No se pudo crear el punto de anotacion", NULL);
   }
   if (player->SetFeature(panotationfeature) != OGRERR_NONE) {
      delete pelement;
      REPORT_AND_FAIL_VALUE("D:No se pudo escribir la anotacion", NULL);
   }
#ifdef __UNUSED_CODE__
   /* Se reemplazan por GetAnotationFieldValue y SetAnotationFieldValue.
    * Verificar si su funcionamiento es el mismo.*/

   pelement->SetAnotation(Anotation);
#endif
   pelement->SetAnotationFieldValue(Anotation);

   // si tiene campo SURAnotation, inserto el nodo "etiqueta" en el xml.
   if (pelement->HasAnotationField()) {
      pelement->AddLabelNode(SUR_ANOTATION_FIELD);
   }

   return pelement;
}

/**
 * Metodo estatico para crear un elemento Anotacion(genera el xml del elemento),
 * a partir de un nombre de archivo.
 * \pre vector cuyo Url se pasa por parametro debe existir.
 * \pre vector creado(la anotacion) tenga el campo SURLabel,
 * \post setea el nombre del elemento con el contenido del campo SURLabel y
 *       setea el estilo del elemento (WKT del estilo del vector).
 * @param[in]	FileName: url del vector.
 * @return puntero a un elemento Anotacion.
 * @return NULL en caso no poder crear el elemento
 * @return NULL en caso que el vector no tenga el campo SURLabel
 * \attention quien invoque a este metodo es responsable del puntero devuelto,
 *            por lo tanto debe hacerse cargo de su eliminacion, La eliminacion
 *            se debe realizar mediante Destroy().
 */
AnotationElement* AnotationElement::Create(const wxString &FileName,
                                           const Option& Metadata) {
   // Creo el elemento con la factory, asi lo registra.
   // El elemento creado debe ser eliminado mediante Destroy()
   AnotationElement *pelement = dynamic_cast<AnotationElement *>(Element::Create(
         "AnotationElement"));

   // Verifico el casteo
   if (pelement) {
      if (pelement->Initialize(FileName.c_str()) != pelement) {
         delete pelement;
         pelement = NULL;
      }
   }

   return pelement;
}

/**
 * @return wxString con el string del icono del elemento.
 */
wxString AnotationElement::DoGetIcon() const {
   return icon_ANOTATION_ELEMENT;
}

/**
 * @return wxString vacio
 */
wxString AnotationElement::GetDetails() const {
   return wxT("");
}

/** Retorna el tipo de elemento */
wxString AnotationElement::GetTypeAsString() {
   return _(label_ANOTATION);
}

/**
 * Devuelve el nombre del campo, en la base de datos de la anotacion, utilizado
 * para guardar el contenido de la anotacion.
 * @param[in]	Layer: capa del vector.
 * @param[out]	FieldName: nombre del campo en la base de datos de vector.
 * @return nombre del campo
 * @return cadena vacia si no encuentra el nodo capa
 * @return cadena vacia si no encuentra el nodo etiqueta
 */
wxString AnotationElement::GetAnotationFieldName(const int Layer, wxString &FieldName) {
   FieldName = wxT("");
   // obtengo el nodo capas
   wxXmlNode *pnode = GetNode(wxT(LAYERS_NODE));
   // obtengo el nodo de la capa buscada
   pnode = GetLayerNode(Layer, pnode);
   if (!pnode) {
      REPORT_AND_FAIL_VALUE("D:No se encuentra la capa: %d", wxT(""), Layer);
   }
   // busco nodo etiqueta
   pnode = GetNode(wxT(LABEL_NODE), pnode);
   if (!pnode) {
      REPORT_AND_FAIL_VALUE("D:No se encuentra el nodo etiqueta", wxT(""));
   }
   return FieldName = pnode->GetNodeContent().c_str();
}

/**
 * Obtiene el contenido del campo SURAnotation de la base de datos del vector.
 * @return contenido del campo SURAnotation
 * @return cadena vacia en caso de error.
 */
wxString AnotationElement::GetAnotationFieldValue() {
   Vector *pvector = Vector::Open(GetUrl().c_str());
   if (!pvector || !pvector->GetLayer(GetActiveLayer())
         || pvector->GetLayer(GetActiveLayer())->GetFeatureCount() < 1) {
      Vector::Close(pvector);
      return wxT("");
   }

   // inicializo la busqueda
   pvector->GetLayer(GetActiveLayer())->ResetReading();
   // obtengo el ultimo feature
   OGRFeature *pnextfeat = pvector->GetLayer(GetActiveLayer())->GetNextFeature();
   OGRFeature *pfeat = NULL;
   while (pnextfeat != NULL) {
      OGRFeature::DestroyFeature(pfeat);
      pfeat = pnextfeat;
      pnextfeat = pvector->GetLayer(GetActiveLayer())->GetNextFeature();
   }

   if (!pfeat) {
      Vector::Close(pvector);
      return wxT("");
   }

   wxString value = pfeat->GetFieldAsString(SUR_ANOTATION_FIELD);
   OGRFeature::DestroyFeature(pfeat);

   Vector::Close(pvector);

   return value;
}

/**
 * Modifica el xml del elemento, agregando el nodo Etiqueta como hijo del nodo
 * "capas|capa". Tambien setea el contenido del nodo Etiqueta.
 * @param[in]	Layer: numero de capa del vector. Utilizada para identificar el nodo.
 * @param[in]	FieldName: nombre del campo. Contenido del nodo Etiqueta.
 */
void AnotationElement::AddLabelNode(const int Layer, const wxString &FieldName) {
   // obtengo el nodo capas
   wxXmlNode *playersnode = GetNode(wxT(LAYERS_NODE));
   // obtengo el nodo de la capa buscada
   wxXmlNode *playernode = GetLayerNode(Layer, playersnode);
   // Agrego el nodo Etiqueta.
   AddNode(playernode, wxT(LABEL_NODE), FieldName);  // dejar con Replace=True
}

/**
 * \overload
 * Modifica el Nombre del campo para la capa activa.
 * @param[in] FieldName nombre del campo
 */
void AnotationElement::AddLabelNode(const wxString &FieldName) {
   AddLabelNode(GetActiveLayer(), FieldName);
}

/**
 * Devuelve las paginas especificos de la Anotacion que se van a mostrar en las
 * Propiedades del elemento.
 * @return vector de Part con los Part de las Propiedades especificas de la
 * Anotacion.
 */
std::vector<Part *> AnotationElement::DoGetParts() {
   std::vector<Part *> vectorparts;
   vectorparts.push_back(new AnotationPropertiesPart((VectorElement*) this));
#ifdef __USE_ELEMENT_NAME_AS_ANOTATION_VALUE__
   Vector *pvector = Vector::Open(GetUrl().c_str());
   bool includetablepart = pvector && pvector->GetLayer(GetActiveLayer())
         && pvector->GetLayer(GetActiveLayer())->GetFeatureCount() > 1;
   Vector::Close(pvector);
   if (includetablepart)
#endif
      vectorparts.push_back(new VectorTablesPart(this, false));
   return vectorparts;
}

/**
 * Verifica si la base de datos del vector posee el campo SURAnotation.
 * @return true en caso de que el vector posea el campo.
 * @return false si no se pudo verificar el nombre del campo.
 * \todo no deberia recorrer los campos, hay un metodo para obtener el indice
 *       dado un nombre o un codigo de error.
 */
bool AnotationElement::HasAnotationField() {
   bool ret = false;

   // no existe el vector o tiene 0 capas
   Vector *pvector = Vector::Open(GetUrl().c_str());
   if (!pvector || pvector->GetLayerCount() == 0) {
      Vector::Close(pvector);
      return false;
   }

   OGRFeatureDefn *pfeatdef = pvector->GetLayer(GetActiveLayer())->GetLayerDefn();
   if (!pfeatdef) {
      Vector::Close(pvector);
      return false;
   }

   for (int i = 0; i < pfeatdef->GetFieldCount(); i++)
      if (pfeatdef->GetFieldDefn(i) &&
      std::string(pfeatdef->GetFieldDefn(i)->GetNameRef() ) ==
      SUR_ANOTATION_FIELD) {ret=true;
   }

   Vector::Close(pvector);
   return ret;
}

/**
 * Invoca al metodo de la clase base para asigar el nombre al elemento, e
 * invoca al metodo que asigna el texto a la anotacion con el nombre del
 * elemento. Cuando se modifica el nombre, tambien se modifica la anotacion.
 */
void AnotationElement::SetName(const wxString& ElementName) {
   Element::SetName(ElementName);
#ifdef __USE_ELEMENT_NAME_AS_ANOTATION_VALUE__
   Vector *pvector = Vector::Open(GetUrl().c_str());
   if (pvector && pvector->GetLayer(GetActiveLayer())
         && pvector->GetLayer(GetActiveLayer())->GetFeatureCount() == 1) {
      SetAnotationFieldValue(ElementName);
   }
   Vector::Close(pvector);
#endif
}

/**
 *
 */
AnotationElement* AnotationElement::Initialize(const std::string &FileName,
                                               const Option& Metadata) {
   // uso un elemento vector para que me filtre el URL
   VectorElement *ptempelem = VectorElement::Create(FileName.c_str());
   wxString filename = FileName;

   if (!ptempelem) {
      return NULL;
   }

   filename = ptempelem->GetUrl();
   delete ptempelem;

   Vector *pvector = Vector::Open(filename.c_str(), Vector::ReadWrite);

   if (!pvector) {
      return NULL;
   }

   SetUrl(filename);
   InitVector(pvector, GetUrl().c_str());
   Vector::Close(pvector);
   if (HasAnotationField()) {
      /* \todo (12/08/2009 - Alejandro): si se quiere crear una Anotacion sin
       * utilizar la herramienta de dibujo para Anotaciones, hay que implementar el
       * uso del VectorStyleManager en los metodos AnotationElement::Create para
       * setear el estilo deseado a la Anotacion que se esta creando, ya que en el
       * metodo VectorElement::Initialize se le setea un estilo de la categoria
       * "Default" la cual no posee estilo para Anotacion.
       */
      SetName(GetAnotationFieldValue());
      return this;
   }
   // si no tiene el campo retorna nulo
   return NULL;
}

/**
 * Establece el contenido del campo SURAnotation de la base de datos del vector.
 * @param[in]	Value: contenido a escribir en el campo SURAnotation del vetor.
 */
void AnotationElement::SetAnotationFieldValue(const wxString &Value) {
   Vector *pvector = Vector::Open(GetUrl().c_str(), Vector::ReadWrite, 0, Vector::Undefined);
   if (!pvector || !pvector->GetLayer(GetActiveLayer())
         || pvector->GetLayer(GetActiveLayer())->GetFeatureCount() < 1) {
      Vector::Close(pvector);
      REPORT_AND_FAIL("D:Error al intentar acceder a la capa vectorial");
   }

   // inicializo la busqueda
   pvector->GetLayer(GetActiveLayer())->ResetReading();
   // obtengo el ultimo feature
   OGRFeature *pnextfeat = pvector->GetLayer(GetActiveLayer())->GetNextFeature();
   OGRFeature *pfeat = NULL;
   while (pnextfeat != NULL) {
      OGRFeature::DestroyFeature(pfeat);
      pfeat = pnextfeat;
      pnextfeat = pvector->GetLayer(GetActiveLayer())->GetNextFeature();
   }

   if (pfeat) {
      pfeat->SetField(SUR_ANOTATION_FIELD, Value.c_str());
      pvector->GetLayer(GetActiveLayer())->SetFeature(pfeat);
      pvector->GetLayer(GetActiveLayer())->SyncToDisk();
   }

   Vector::Close(pvector);
}
}  // namespace suri
