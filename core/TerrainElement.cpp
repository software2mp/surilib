/*! \file TerrainElement.cpp */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2013-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

// Includes estandar
#include <sstream>
#include <cstdlib>
#include <string>
#include <vector>
#include <map>

// Includes suri
#include "suri/TerrainElement.h"
#include "suri/messages.h"
#include "suri/RasterElement.h"
#include "suri/Configuration.h"
#include "suri/xmlnames.h"
#include "suri/Part.h"
#include "suri/Image.h"
#include "suri/AuxiliaryFunctions.h"
#include "suri/DatasourceManagerInterface.h"
#include "suri/DatasourceInterface.h"
#include "suri/LayerInterface.h"
#include "suri/DataTypes.h"
#include "TerrainPropertiesPart.h"
#include "resources.h"
#include "suri/CoordinatesTransformation.h"
#include "suri/ParameterCollection.h"
#include "suri/TransformationFactory.h"
#include "suri/TransformationFactoryBuilder.h"

// includes wx
#include "wx/xrc/xmlres.h"
#include "wx/spinctrl.h"

// Defines

// limites
/** minimo paso de camara */
#define MIN_CAMERA_STEP 1
/** maximo paso de camara */
#define MAX_CAMERA_STEP 20
/** factor de exageracion minimo */
#define MIN_EXAGERATION 1
/** factor de exageracion maximo. */
#define MAX_EXAGERATION 20
// multiplicadores de performance
/** multiplicador de performance para textura */
#define PERFORMANCE_MULTIPLIER 100000
/** multiplicador de performance para textura */
#define TEXTURE_PERFORMANCE_MULTIPLIER 5

/** indices de la tabla(es usada en TerrainElement.cpp) */
/** indice de la columna de Terreno en tabla de Calidad */
#define TERRAIN_COLUMN 0
/** indice de la columna de Textura en tabla de Calidad */
#define TEXTURE_COLUMN 1

/** Tabla de conversion de valores de Calidad en Factor de Terreno y Textura */
int qualityTable[20][2] = { { 11, 11 }, { 11, 10 }, { 11, 9 }, { 11, 8 }, { 10, 8 }, {
      9, 8 },
                            { 8, 8 }, { 7, 7 }, { 7, 6 }, { 7, 5 }, { 7, 4 }, { 6, 4 },
                            { 5, 4 }, { 4, 4 }, { 4, 3 }, { 4, 2 }, { 4, 1 }, { 3, 1 },
                            { 2, 1 }, { 1, 1 } };

/** namespace suri */
namespace suri {
/** registra la clase TerrainElement */
AUTO_REGISTER_CLASS(Element, TerrainElement, 0)

/* Constructor
 * @return instancia de la clase TerrainElement
 */
TerrainElement::TerrainElement() : Element(TYPE_PROPERTY_VALUE_TERRAIN), force_(false) {
}

/** Destructor. */
TerrainElement::~TerrainElement() {
}

/* Devuelve un string para formar el icono del elemento.
 * @return string para el icono
 */
wxString TerrainElement::DoGetIcon() const {
   return icon_TERRAIN_ELEMENT;
}

/* retorna los detalles del elemento
 * @return detalles del elemento
 */
wxString TerrainElement::GetDetails() const {
   return wxT("");
}

/**
 * Devuelve el Factor de decimacion del Terreno. Busca y devuelve el valor del
 * nodo renderizacion|factor.
 * \pre	si no existe el nodo renderizacion|factor devuelve el primer valor de
 * la tabla de conversion de calidad que se corresponde con la componente del
 * Factor de Terreno.
 * @return entero con el valor del factor de decimacion del terreno.
 */
int TerrainElement::GetTerrainFactor() {
   wxXmlNode *pfactornode = GetNode(RENDERIZATION_NODE
                                       NODE_SEPARATION_TOKEN FACTOR_NODE);
   int terfactor = qualityTable[0][TERRAIN_COLUMN];
   if (pfactornode) {
      std::stringstream ss;
      ss << pfactornode->GetChildren()->GetContent();
      ss >> terfactor;
   }

   return terfactor;
}

/**
 * Establece el factor de decimacion del terreno siempre que este sea mayor que
 * 0 (cero).
 * \pre si Factor < 0 (cero) no setea el factor y muestra mensaje de debug.
 * @param[in]	Factor factor de decimacion de terreno.
 * \todo (25/06/2009 - Alejandro): analizar si deberia verificarse un valor
 * maximo para el factor.
 */
void TerrainElement::SetTerrainFactor(const int Factor) {
   if (Factor < 0) {
      REPORT_AND_FAIL("D: No se setea Factor de Terreno. Factor menor a 0 (cero).");
   }

   if (Factor == GetTerrainFactor()) {
      REPORT_AND_RETURN("D: Factor de Terreno ya seteado.");
   }

   wxXmlNode *pnode = new wxXmlNode(NULL, wxXML_ELEMENT_NODE, wxT(FACTOR_NODE));

   pnode->AddChild(
         new wxXmlNode(pnode, wxXML_TEXT_NODE, wxEmptyString,
                       wxString::Format(wxT("%d"), Factor).c_str()));
   AddNode(GetNode(wxT(RENDERIZATION_NODE)), pnode, true);
   shouldRender_ = true;
}

/**
 * Devuelve el Factor de decimacion de la Textura. Busca y devuelve el valor del
 * nodo renderizacion|textura|factor.
 * \pre	si no existe el nodo renderizacion|textura|factor devuelve el primer
 * valor de la tabla de conversion de calidad que se corresponde con la
 * componente del Factor de Textura.
 * @return entero con el factor de decimacion de la textura.
 */
int TerrainElement::GetTextureFactor() {
   wxString nodename = RENDERIZATION_NODE;
   nodename += NODE_SEPARATION_TOKEN;
   nodename += TEXTURE_NODE;
   nodename +=  NODE_SEPARATION_TOKEN;
   nodename +=FACTOR_NODE;
   wxXmlNode *pfactornode = GetNode(nodename);
   int texfactor = qualityTable[0][TEXTURE_COLUMN];
   if (pfactornode) {
      std::stringstream ss;
      ss << pfactornode->GetChildren()->GetContent();
      ss >> texfactor;
   }
   return texfactor;
}

/**
 * Establece el factor de decimacion de la Textura siempre que este sea mayor
 * que 0 (cero).
 * \pre si Factor < 0 (cero) no setea el factor y muestra mensaje de debug.
 * @param[in]	Factor factor de decimacion de la Textura.
 * \todo (25/06/2009 - Alejandro): analizar si deberia verificarse un valor
 * maximo para el factor.
 */
void TerrainElement::SetTextureFactor(const int Factor) {
   if (Factor < 0) {
      REPORT_AND_FAIL("D: No se setea Factor de Textura. Factor menor a 0 (cero).");
   }

   if (Factor == GetTextureFactor()) {
      REPORT_AND_RETURN("D: Factor de Textura ya seteado.");
   }

   wxXmlNode * pnode = new wxXmlNode(NULL, wxXML_ELEMENT_NODE, wxT(FACTOR_NODE));
   pnode->AddChild(
         new wxXmlNode(pnode, wxXML_TEXT_NODE, wxEmptyString,
                       wxString::Format(wxT("%d"), Factor).c_str()));
   wxString pathnode = wxString::Format("%s%s%s", RENDERIZATION_NODE,
                                        NODE_SEPARATION_TOKEN, TEXTURE_NODE);
   wxXmlNode* ptexnode = GetNode(wxT(pathnode));
   if (ptexnode)
      AddNode(ptexnode, pnode, true);
   shouldRender_ = true;
}

/**
 * Devuelve el Factor de exageracion del Terreno. Busca y devuelve el valor del
 * nodo renderizacion|exageracion.
 * \pre	si no existe el nodo renderizacion|exageracion devuelve el valor
 * establecido en el archivo de configuracion de la app.
 * \pre si existe el nodo renderizacion|exageracion pero el valor esta fuera del
 * rango [MIN_EXAGERATION, MAX_EXAGERATION]
 * \post devuelve MIN_EXAGERATION
 * @return entero con el factor de exageracion del Terreno.
 * @return MIN_EXAGERATION si el valor del nodo renderizacion|exageracion esta
 * fuera de rango.
 */
int TerrainElement::GetExagerationFactor() {
   long exageration;
   wxString pathnode = wxString::Format("%s%s%s", RENDERIZATION_NODE,
                                        NODE_SEPARATION_TOKEN, EXAGERATION_NODE);
   wxXmlNode *pfactornode = GetNode(pathnode);
   if (!pfactornode) {
      exageration = Configuration::GetParameterEx("v3d_exageracion", MIN_EXAGERATION);
   } else {
      std::stringstream ss;
      ss << pfactornode->GetChildren()->GetContent();
      ss >> exageration;
   }
   if (exageration < MIN_EXAGERATION || exageration > MAX_EXAGERATION) {
      REPORT_DEBUG("D:Error en xml. Error en valores de exageracion");
      exageration = MIN_EXAGERATION;
   }
   return exageration;
}

/**
 * Establece el factor de exageracion siempre y cuando este sea mayor que 0(cero).
 * \pre si Factor es menor que 0 (cero).
 * \pre si Factor no se encuentra dentro del intervalo
 * [MIN_EXAGERATION, MAX_EXAGERATION]
 * \post no setea el factor de exageracion e imprime mensaje de debug.
 * \post setea el Factor como MIN_EXAGERATION
 * @param[in]	Factor Factor de exageracion.
 */
void TerrainElement::SetExagerationFactor(const int Factor) {
// \todo: Mover a messages
   if (Factor < 0) {
      REPORT_AND_FAIL("D: No se setea Factor de Exageracion. Factor menor a 0 (cero).");
   }

   if (Factor == GetExagerationFactor()) {
      REPORT_AND_RETURN("D: Factor de Exageracion ya seteado.");
   }

   wxXmlNode *pnode = new wxXmlNode(NULL, wxXML_ELEMENT_NODE, wxT(EXAGERATION_NODE));
   std::stringstream ss;
   if (Factor < MIN_EXAGERATION || Factor > MAX_EXAGERATION) {
      ss << MIN_EXAGERATION;
   } else {
      ss << Factor;
   }
   pnode->AddChild(new wxXmlNode(pnode, wxXML_TEXT_NODE, wxEmptyString, ss.str()));
   AddNode(GetNode(wxT(RENDERIZATION_NODE)), pnode, true);
   shouldRender_ = true;
}

/**
 * Retorna el color de fondo del terreno guardado en el xml
 * @return string con color de fondo en formato "#FFFFFF"
 * \post si ocurre error retorna "";
 */
std::string TerrainElement::GetBackgroundColor() {
   wxString pathnodecolour = RENDERIZATION_NODE + wxString(NODE_SEPARATION_TOKEN) +
   BACKGROUND_COLOR_NODE;
   wxXmlNode *pnodecolour = GetNode(pathnodecolour);
   wxString colour_node("");
   if (pnodecolour) {
      colour_node = pnodecolour->GetChildren()->GetContent();
   }
   return colour_node.c_str();
}

/**
 * Guarda el color de fondo del terreno guardado en el xml
 * \pre el color debe tener formato "#FFFFFF"
 * @param[in] BackgroundColour nuevo color de fondo
 */
void TerrainElement::SetBackgroundColor(const std::string &BackgroundColour) {
   wxString pathnode = RENDERIZATION_NODE + wxString(NODE_SEPARATION_TOKEN) +
   BACKGROUND_COLOR_NODE;
   wxXmlNode *ptexnode = GetNode(pathnode);
   ptexnode->GetChildren()->SetContent(wxString(BackgroundColour));
}

/**
 * Devuelve el valor de Paso de Camara. Busca y devuelve el valor del
 * nodo renderizacion|paso_de_camara.
 * \pre	si no existe el nodo renderizacion|exageracion devuelve el valor
 * establecido en el archivo de configuracion de la app.
 * \pre si existe el nodo renderizacion|paso_de_camara pero el valor esta fuera
 * del rango [MIN_CAMERA_STEP, MAX_CAMERA_STEP]
 * \post devuelve MIN_CAMERA_STEP
 * @return entero con el valor del paso de camara
 * @return MIN_CAMERA_STEP si el valor del nodo renderizacion|paso_de_camara
 * esta fuera de rango.
 */
int TerrainElement::GetCameraStep() {
   long step;
   wxString pathnode = wxString::Format("%s%s%s", RENDERIZATION_NODE,
                                        NODE_SEPARATION_TOKEN, CAMERA_STEP_NODE);
   wxXmlNode *pfactornode = GetNode(pathnode);
   if (!pfactornode) {
      step = Configuration::GetParameterEx("v3d_velocidad_camara", MIN_CAMERA_STEP);
   } else {
      std::stringstream ss;
      ss << pfactornode->GetChildren()->GetContent();
      ss >> step;
   }
   if (step < MIN_CAMERA_STEP || step > MAX_CAMERA_STEP) {
      step = MIN_CAMERA_STEP;
   }

   return step;
}

/**
 * Establece el paso de camara siempre y cuando este sea mayor que 0(cero).
 * \pre si Step es menor que 0 (cero)
 * \pre si Step no se encuentra dentro del intervalo
 * [MIN_CAMERA_STEP, MAX_CAMERA_STEP]
 * \post no setea el paso de camara e imprime mensaje de debug.
 * \post setea el Factor como MIN_CAMERA_STEP
 * @param[in]	Step paso de camara.
 */
void TerrainElement::SetCameraStep(const int Step) {
// \todo: Mover a messages
   if (Step < 0) {
      REPORT_AND_FAIL("D: No se setea Paso de Camara. Paso menor a 0 (cero).");
   }

   if (Step == GetCameraStep()) {
      REPORT_AND_RETURN("D: Paso de camara ya seteado");
   }

   wxXmlNode *pnode = new wxXmlNode(NULL, wxXML_ELEMENT_NODE, CAMERA_STEP_NODE);
   std::stringstream ss;
   if (Step < MIN_CAMERA_STEP || Step > MAX_CAMERA_STEP) {
      ss << MIN_CAMERA_STEP;
   } else {
      ss << Step;
   }
   pnode->AddChild(new wxXmlNode(pnode, wxXML_TEXT_NODE, wxEmptyString, ss.str()));
   AddNode(GetNode(wxT(RENDERIZATION_NODE)), pnode, true);
}

/**
 * Obtiene el indice de la Tabla de conversion de valores de Calidad que mas se
 * aproxima a los valores de Factor de Terreno y Factor de Textura establecidos
 * para el Terreno.
 * \pre si el par de Factores Terreno-Textura no existe en la Tabla de conversion
 * de valores de Calidad
 * \post devuelve el indice del par cuya componente de Terreno coincida con el
 * Factor de Terreno encontrado y la componente de Textura sea la minima.
 * @return entero que indica el indice de la Tabla de conversion de valores de
 * Calidad.
 */
int TerrainElement::GetQuality() {
   int terrainfactor = GetTerrainFactor();
   int texturefactor = GetTextureFactor();

   // chequeo que sea un par valido
   int quality = 0;
   int qualitycount = GetQualityCount();
   while (quality < qualitycount
         && (qualityTable[quality][TERRAIN_COLUMN] != terrainfactor
               || qualityTable[quality][TEXTURE_COLUMN] != texturefactor))
      quality++;

   // si no es un par valido, busco ajustar la componente de textura.
   if (quality >= qualitycount) {
      texturefactor = GetValidTextureFactor(terrainfactor, texturefactor);
   }

   quality = 0;
   while (quality < qualitycount
         && (qualityTable[quality][TERRAIN_COLUMN] != terrainfactor
               || qualityTable[quality][TEXTURE_COLUMN] != texturefactor))
      quality++;

   return quality;
}

/**
 * Establece los valores de Factor de Terreno y Factor de Textura. Los factores
 * mencionados los obtiene de la Tabla de conversion de valores de Calidad,
 * accediendo a la misma con el valor del parametro Quality como indice.
 * \pre	Quality debe pertenecer al intervalo [0, Cantidad de Elementos de la Tabla)
 * \post	Se establecen los valores de Factor de Terreno y Factor de Textura.
 * \pre	Quality no pertenece al intervalo [0, Cantidad de Elementos de la Tabla)
 * \post	Se establecen los valores de Factor de Terreno y Factor de Textura de
 * que hacen a la calidad minima.
 * @param[in]	Quality indice de la Tabla de conversion de valores de Calidad.
 */
void TerrainElement::SetQuality(const int Quality) {
   int terrainfactor = qualityTable[0][TERRAIN_COLUMN];
   int texturefactor = qualityTable[0][TEXTURE_COLUMN];

   if (Quality >= 0 && Quality < GetQualityCount()) {
      terrainfactor = qualityTable[Quality][TERRAIN_COLUMN];
      texturefactor = qualityTable[Quality][TEXTURE_COLUMN];
   }

   SetTerrainFactor(terrainfactor);
   SetTextureFactor(texturefactor);
}

/**
 * @return entero con la cantidad de elementos que contiene la Tabla de
 * conversion de valores de Calidad.
 */
int TerrainElement::GetQualityCount() {
   return ARRAY_SIZE(qualityTable);
}

/** Funcion estatica para crear un TerrainElement */
/**
 * Crea el terreno, le carga la imagen, setea exageracion, factor de decimacion, paso de la camara
 * y guarda los datos correspondientes en el xml
 * @param[in] FileName URL del terreno
 * @param[in] Metadata mapa de metadatos
 * @return TerrainElemnt terreno inicializado
 */
TerrainElement *TerrainElement::Create(const std::string &FileName,
                                       const Option& Metadata) {
   TerrainElement *pelement = dynamic_cast<TerrainElement*>(Element::Create(
         "TerrainElement", FileName, Metadata));
   if (pelement && !pelement->Initialize(FileName, Metadata)) {
      // si initialize falla, retorna NULL, y nadie informa error hacia arriba
      Element *pelem = pelement;
      suri::Element::Destroy(pelem);
      pelement = NULL;
   }
   return pelement;
}

/**
 * Devuelve ventanas de propiedades de los elementos Terreno.
 * @return vector con punteros a los Part generados.\n
 * NOTA: en caso de tener que agregar Part, RECORDAR que GetParts()devuelve 2
 * Part(1ro Comun a todos los elementos, 2do Para editar xml(solo en DEBUG))
 * ==> los Part a agregar deben agregarse entre medio de dichos Part's.
 */
std::vector<Part *> TerrainElement::DoGetParts(DataViewManager* pDataViewManager,
                                               LayerInterface *pLayer,
                                               DatasourceInterface *pDatasource) {
   std::vector<Part *> vectorparts;
   if (!pDatasource || !pLayer || !pDataViewManager) {
      vectorparts.push_back(new TerrainPropertiesPart(this));
   }
   return vectorparts;
}

/** Devuelve vector con Parts especificos de la visualizacion elemento. */
std::vector<suri::Part*> TerrainElement::DoGetVisualizationParts(
      DataViewManager* pDataViewManager, LayerInterface *pLayer,
      DatasourceInterface *pDatasource) {
   std::vector<Part *> vectorparts;
   TerrainElement* pterrainElement = dynamic_cast<TerrainElement*>(pLayer->GetElement());
   if (pLayer && pterrainElement) {
      // hace falta dynamic cast para pasar el elemento que viene de la capa como terreno para
      // los parts
      vectorparts.push_back(new TerrainPropertiesPart(pterrainElement));
   }
   return vectorparts;
}

/**
 * Crea RasterElement con Filename, obtiene en nodo de la textura, obtiene la
 * imagen, calcula el factor de la textura y guarda los datos correspondientes
 * en el nodo.
 * @param[in] FileName nombre del archivo
 * @return true en caso de poder agregar la textura
 * @return false en caso de no poder agregar la textura
 */
bool TerrainElement::AddTexture(const std::string &FileName) {
   RasterElement* prasterelem = RasterElement::Create(FileName);
   if (!prasterelem)
      return false;
   wxString tnodepath = wxString::Format("%s%s%s", RENDERIZATION_NODE,
                                         NODE_SEPARATION_TOKEN, TEXTURE_NODE);
   wxXmlNode *ptnode = GetNode(tnodepath);
   wxXmlNode *prnode = GetNode(RENDERIZATION_NODE);
   if (!ptnode && prnode) {
      AddNode(prnode, TEXTURE_NODE);
      ptnode = GetNode(tnodepath);
   }
   // calculo del factor de textura
   size_t factor = GetTextureFactor(prasterelem);
   if (factor < 0) {
      delete prasterelem;
      REPORT_AND_FAIL_VALUE("D:No se pudo agregar la textura", false);
   }
   wxXmlNode* pfactornode =
         AddNode(ptnode, FACTOR_NODE, wxString::Format(PERCENT_U, factor).c_str());
   wxXmlNode* ptexturenode = prasterelem->GetProperties();
   wxXmlNode* pelemnode = ptnode->GetChildren();
   wxXmlNode* plasttextnode = NULL;
   while (pelemnode) {
      if (pelemnode->GetName().compare(CONTEXT_ELEMENT_NODE) == 0)
         plasttextnode = pelemnode;
      pelemnode = pelemnode->GetChildren();
   }
   if (plasttextnode)
      ptnode->InsertChildAfter(ptexturenode, plasttextnode);
   else
      ptnode->AddChild(ptexturenode);

   if (!Validate()) {
      // Elimino nodos y retorno false
      ptnode->RemoveChild(ptexturenode);
      ptnode->RemoveChild(pfactornode);
      return false;
   }
   AddDependency(prasterelem->GetUrl().c_str());
   delete prasterelem;
   return true;
}

/** Quita la textura para el Terreno. */
bool TerrainElement::RemoveTexture(const std::string& FileName) {
   if (FileName.empty()) {
      return false;
   }

   wxString texturespath = wxString::Format("%s%s%s", RENDERIZATION_NODE,
                                         NODE_SEPARATION_TOKEN, TEXTURE_NODE);

   // recorro los nodo url hasta encontrar el nodo cuyo contenido sea FileName
   bool found = false;
   wxXmlNode* psibling = NULL;
   wxXmlNode* ptexture = NULL;
   wxXmlNode* ptextures = GetNode(texturespath);
   if (ptextures) {
      ptexture = ptextures->GetChildren();
      while (ptexture) {

         if (ptexture->GetName().Cmp(ELEMENT_NODE) == 0) {
            wxXmlNode* purl = ptexture->GetChildren();
            while (purl) {
               if (purl->GetName().Cmp(URL_NODE) == 0) {
                  if (purl->GetNodeContent().Cmp(FileName.c_str()) == 0) {
                     found = true;
                  }
               }

               if (found) break;

               purl = purl->GetNext();
            }
         }

         if (found) break;

         psibling = ptexture;
         ptexture = ptexture->GetNext();
      }
   }

   // No encuentro el nodo buscado.
   if (!ptexture) {
      return false;
   }

   // enlazo los nodos.
   if (psibling) {
      psibling->SetNext(ptexture->GetNext());
   }
   if (ptexture->GetNext()) {
      ptexture->GetNext()->SetParent(ptexture->GetParent());
   }

   // elimino el nodo.
   wxXmlNode* pparent = ptexture->GetParent();
   pparent->RemoveChild(ptexture);

   delete ptexture;

   return true;
}

/*
 * Valida que la textura y el terreno sean compatibles
 * @return true si el terreno y la textura tienen la misma referencia espacial
 * y extents superpuestos
 * @return false si no se cumple alguna de las condiciones mencionadas.
 */
bool TerrainElement::Validate() {
   return ValidateSpatialReference() && ValidateSubsets();
}

/* Valida que la textura y el terreno tengan el mismo sist de referencia
 * @return true si textura y terreno tienen el mismo sist de referencia
 * @return false si textura y terreno no tienen el mismo sist de referencia
 */
bool TerrainElement::ValidateSpatialReference() {
   // Cargo raster de la textura
   RasterElement* ptexture = GetTexture();
   if (!ptexture) {
      SHOW_ERROR(_(message_TEXTURE_RASTER_ERROR));
      return false;
   }
   // Valido la referencia espacial de la textura contra la del terreno
   bool validsr = true;
   ParameterCollection params;
   params.AddValue<std::string>(TransformationFactory::kParamWktIn,
                                 ptexture->GetSpatialReference().c_str());
   params.AddValue<std::string>(TransformationFactory::kParamWktOut,
                                 GetSpatialReference().c_str());
   TransformationFactory* pfactory = TransformationFactoryBuilder::Build();
   CoordinatesTransformation *pcoordtransform = pfactory->Create(
                                                      TransformationFactory::kExact, params);
   TransformationFactoryBuilder::Release(pfactory);
   // Valido la referencia espacial de la textura contra la del terreno
   if (!pcoordtransform || !pcoordtransform->IsIdentity())
       validsr = false;
   delete pcoordtransform;
   delete ptexture;
   if (!validsr)
      SHOW_ERROR(_(message_TERRAIN_TEXTURE_SPATIAL_REFERENCE_ERROR));
   return validsr;
}

/* Valida que la textura y el terreno tengan extent superpuesto
 * @return true si textura y terreno tienen extent superpuesto
 * @return false si textura y terreno no tienen extent superpuesto
 */
bool TerrainElement::ValidateSubsets() {
   RasterElement* ptexture = GetTexture();
   Element* pterrain = Element::Create("RasterElement", GetUrl().c_str());

   // Valido los subsets
   bool validsubset = true;
   Subset textureextent, terrainextent;
   ptexture->GetElementExtent(textureextent);
   pterrain->GetElementExtent(terrainextent);
   Subset intersection = Intersect(textureextent, terrainextent);
   if ((intersection.lr_.x_ == 0 && intersection.ul_.x_ == 0)
        || (intersection.lr_.y_ == 0 && intersection.ul_.y_ == 0)) {
      validsubset = false;
   }
   Element::Destroy(pterrain);
   delete ptexture;
   if (!validsubset)   
      SHOW_ERROR(_(message_TERRAIN_TEXTURE_SUBSET_ERROR));
   return validsubset;
}

/**
 *	Devuelve Factor de Terreno o de Textura. Para obtener dicho factor verifica
 *	que el tamanio de la imagen sea menor a un factor de Performance. El valor
 *	del factor que se devuelve es obtenido de la Tabla de conversion de valores
 *	de Calidad.
 *	\pre Multiplier debe ser > 0
 *	\pre IsTerrainFactor == true el metodo devuelve el Factor de Terreno.
 *	\pre IsTerrainFactor == false el metodo devuelve el Factor de Textura.
 * @param[in]	Size tamanio del raster.
 * @param[in]	Multiplier multiplicador de performance.
 * @param[in]	IsTerrainFactor indica si se debe devolver factor de Terreno o Textura.
 * @return el valor devuelto es un size_t con el factor (Terreno o Textura) calculado.
 */
int TerrainElement::GetFactor(const int Size, long Multiplier,
                              const bool IsTerrainFactor) {
   if (Size <= 0 || Multiplier <= 0) {
      REPORT_AND_FAIL_VALUE("D: no se puede calcular factor de %s", 0,
                            IsTerrainFactor ? wxT("D:Terreno") : wxT("D:Textura"));
   }

   // Calculo factor, buscando valores en tabla de conversion
   long performance = Configuration::GetParameterEx("v3d_performance", 1);
   performance *= Multiplier;
   // indice maximo de la tabla de calidad
   int qualityidx = GetQualityCount();
   int factor = 1;
   int size = Size;
   while ((size > performance) && (qualityidx > 0)) {
      factor = qualityTable[--qualityidx][
            IsTerrainFactor ? TERRAIN_COLUMN : TEXTURE_COLUMN];
      size = size / factor;
   }

   return factor;
}

/**
 * Devuelve un factor de Textura valido para el factor de Terreno calculado.
 * \pre Deben calcularse previamente los factores de Terreno y Textura.
 * @param[in]	TerrainFactor factor de Terreno calculado.
 * @param[in]	TextureFactor factor de Textura calculado.
 * @return	entero con factor de textura que hace valido el par
 * (Factor de Terreno - Factor de Textura).
 * \todo(23/06/2009 - Alejandro): Verificar si es mas optimo aproximar la textura
 * al terreno o viceversa.
 */
int TerrainElement::GetValidTextureFactor(const int TerrainFactor,
                                          const int TextureFactor) {
   int qualityidx = 0;
   int terrainfactoridx = 0;
   int qualitycount = GetQualityCount();

   /* chequeo que sea un par valido y me guardo el ultimo indice de los pares
    * cuya componente de terreno coincida con el factor de terreno calculado.
    */
   while (qualityidx < qualitycount
         && (qualityTable[qualityidx][TERRAIN_COLUMN] != TerrainFactor
               || qualityTable[qualityidx][TEXTURE_COLUMN] != TextureFactor)) {
      if (TerrainFactor == qualityTable[qualityidx][TERRAIN_COLUMN]) {
         terrainfactoridx = qualityidx;
      }
      qualityidx++;
   }

   /* Si es un par invalido, ajusto la textura. Obtengo un valor de textura que
    * identifique un par valido con respecto al factor de terreno calculado. */
   int texturefactoridx = terrainfactoridx;
   if (qualityidx > qualitycount) {
      qualityidx = terrainfactoridx;
      while ((qualityidx < qualitycount)
            && (qualityTable[qualityidx][TERRAIN_COLUMN] == TerrainFactor
                  && qualityTable[qualityidx][TEXTURE_COLUMN] < TextureFactor))
         texturefactoridx = qualityidx--;
   }

   return qualityTable[texturefactoridx][TEXTURE_COLUMN];
}

/* Devuelve la textura asociadas al DEM (el DEM mismo)
 * En caso de no estar definida, retorna un RasterElement asociado
 * al terreno con renderizacion apta para utilizarlo como textura.
 * @return RasterElement con la textura del DEM
 * @return NULL en caso de error
 */
RasterElement* TerrainElement::GetTexture() const {
   std::vector<RasterElement*> textures;
   RasterElement* ptextureelem = NULL;
   wxXmlNode *ptexturenode = GetNode(RENDERIZATION_NODE
                                 NODE_SEPARATION_TOKEN TEXTURE_NODE
                                 NODE_SEPARATION_TOKEN CONTEXT_ELEMENT_NODE);
   while (ptexturenode && !ptextureelem) {
      if (ptexturenode->GetName().compare(CONTEXT_ELEMENT_NODE) == 0) {
         ptextureelem = dynamic_cast<RasterElement*>(
                              Element::Create("RasterElement", ptexturenode));
      }
      ptexturenode = ptexturenode->GetNext();
   }
   // Si no tiene textura, usa el mismo terreno tomo tal
   if (!ptextureelem) {
      ptextureelem = dynamic_cast<RasterElement*>(
                           Element::Create("RasterElement", GetUrl().c_str()));
      // agrego nodo data_cast y combinacion para que sea RGB
      wxXmlNode *prnode = ptextureelem->AddNode(ptextureelem->GetNode(),
                                RENDERIZATION_NODE, wxT(""),
                                wxT(""), wxT(""), true);
      ptextureelem->AddNode(prnode, BAND_COMBINATION_NODE, wxT("0 0 0"),
                                wxT(""), wxT(""), true);
      ptextureelem->AddNode(prnode, DATA_CAST_NODE, wxT(""), OUTPUT_PROPERTY,
                                DataInfo<unsigned char>::Name, true);
   }
   return ptextureelem;
}

/**
 * Inicializa el terreno con archivo en FileName.
 * Intenta descomponer FileName, utilizando | como separador, en los
 * archivos de terreno (alturas) y textura.
 * @param[in] FileName filename del terreno + textura (separado por "|")
 * @param[in] Metadata mapa de metadatos
 *
 */
TerrainElement* TerrainElement::Initialize(const std::string &FileName,
                                           const Option& Metadata) {
   TerrainElement *pelement = this;
   std::vector<std::string> filetokens = tokenizer(FileName, TEXTURE_SEPARATION_TOKEN);

   if (filetokens.size() == 0)
      return NULL;

   std::string filename = filetokens.at(0);
   pelement->SetUrl(filename);

   Image *pimage = Image::Open(pelement->GetUrl().c_str());
   // si no cumple con la condicion 1 banda, no se toma como terreno.
   if (!pimage || pimage->GetBandCount() != 1) {
      Image::Close(pimage);
      // Falta un delete pelement o Element::Destroy(pelement)?
      return NULL;
   }
   // nodos de georreferencia
   wxXmlNode *pgnode = pelement->AddNode(pelement->GetNode(wxT("")),
                                         GEORREFERENCE_NODE);
   pelement->AddNode(pgnode, SPATIAL_REFERENCE_NODE,
                     pimage->GetOption(Image::Georreference));
   pelement->AddNode(pgnode, RASTER_SPATIAL_MODEL_NODE,
                     pimage->GetOption(Image::RasterMatrix));

   // nodos de renderizacion
   wxXmlNode *prnode = pelement->AddNode(pelement->GetNode(wxT("")),
                                         RENDERIZATION_NODE);

   // bandas a renderizar
   pelement->AddNode(prnode, BAND_COMBINATION_NODE, wxT("0"));

   // exageracion
   long exageration = Configuration::GetParameterEx("v3d_exageracion", MIN_EXAGERATION);
   std::stringstream ss;
   ss << exageration;
   pelement->AddNode(prnode, EXAGERATION_NODE, ss.str());

   // factor de decimacion del terreno
   size_t factor = pelement->GetFactor(pimage->GetSizeX() * pimage->GetSizeY(),
                                       PERFORMANCE_MULTIPLIER, true);

   pelement->AddNode(prnode, FACTOR_NODE,
                     wxString::Format(wxT(PERCENT_U), factor).c_str());
   Image::Close(pimage);

   // color de Fondo
   pelement->AddNode(prnode, BACKGROUND_COLOR_NODE, wxT("#FFFFFF"));

   // Paso de la camara
   long step = Configuration::GetParameterEx("v3d_velocidad_camara", MIN_CAMERA_STEP);
   ss.str("");
   ss << step;
   pelement->AddNode(prnode, CAMERA_STEP_NODE, ss.str());

   // textura, si el archivo se definio en FileName.
   if (filetokens.size()>1) {
      std::string texturefilename;
      texturefilename = filetokens.at(1);
      pelement->AddTexture(texturefilename);
   }

   return pelement;
}

/** retorna el factor de textura a partir de un elemento*/
int TerrainElement::GetTextureFactor(RasterElement* pRaster) {
   // calculo del factor de textura
   Image *pimage = Image::Open(pRaster->GetUrl().c_str());
   if (!pimage) {
      Image::Close(pimage);
      return -1;
   }
   size_t factor = GetFactor(pimage->GetSizeX() * pimage->GetSizeY(),
                             PERFORMANCE_MULTIPLIER * TEXTURE_PERFORMANCE_MULTIPLIER,
                             false);
   return factor;
}

}   // namespace suri
