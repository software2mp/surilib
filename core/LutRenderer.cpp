/*! \file LutRenderer.cpp */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

// Includes standard
#include <string>
#include <limits>
#include <sstream>

// Includes Suri
#include "LutRenderer.h"
#include "suri/xmlnames.h"
#include "suri/DataTypes.h"
#include "suri/Element.h"
#include "suri/AuxiliaryFunctions.h"
#include "suri/Canvas.h"
#include "suri/XmlFunctions.h"

// Includes Wx
#include "wx/xml/xml.h"

// Includes App

// Defines

/** Valor "si" para la propiedad "activa" del nodo lut */
#define LUT_ENABLED_PROPERTY_VALUE_TRUE ACTIVE_PROPERTY_VALUE
/** Valor "no" para la propiedad "activa" del nodo lut */
#define LUT_ENABLED_PROPERTY_VALUE_FALSE INACTIVE_PROPERTY_VALUE

/** Macro para registrar Renderers en forma automatica */
AUTO_REGISTER_RENDERER(suri::LutRenderer);

/** namespace suri */
namespace suri {

/** Template lut */
/**
 * Este template se utiliza para aplicar mapa a imagen.
 * Aplica mapa a cada pixel en pSrc y guarda resultado pDest.
 * \pre pDest debe tener tamanio imagen con pixeles en formato int
 * \pre pSrc debe tener tamanio Size
 * \post pDest tiene los datos con LUT
 * @param[out] pDest puntero a los datos "mapeados"
 * @param[in] pSrc puntero a los datos a "mapear"
 * @param[in] Size Tamanio en T de pDest
 * @param[in] LutTable mapa que determina valores uchar a partir de doubles
 */
template<typename T>
void lut(void* pDest, void* pSrc, size_t Size, std::map<double, double> LutTable) {
   // le agrego el ultimo repetido al final para que la tabla corte repitiendolo
   std::map<double, double> luttable = LutTable;
   luttable.insert(std::make_pair(std::numeric_limits<double>::max(), luttable.rbegin()->second));
   T* psrc = static_cast<T*>(pSrc);
   T* pdest = static_cast<T*>(pDest);
   for (size_t i = 0; i < Size; i++) {
      std::map<double, double>::iterator it = luttable.lower_bound(static_cast<double>(psrc[i]));
      pdest[i] = static_cast<T>(it->second);
   }
}
/** Inicializa mapa de tipos de datos. */
INITIALIZE_DATATYPE_MAP(LutRenderer::Parameters::LutFunctionType, lut);

/**
 * Constructor
 */
LutRenderer::LutRenderer() {
}

/**
 * Destructor
 */
LutRenderer::~LutRenderer() {
}

/**
 * Crea un renderizador a partir de un pElement
 * @param[in] pElement con el que se quiere crear renderizador
 * @param[in] pPreviousRenderer renderizador anterior en cadena de
 * renderizadores
 * @return renderizador creado. NULL si no se pudo
 * \attention el LutRenderer retornado(y su eliminacion) es responsabilidad
 * del invocante
 */
Renderer *LutRenderer::Create(Element *pElement, Renderer *pPreviousRenderer) const {
   if (!pPreviousRenderer) {
      return NULL;
   }

   // Si no existe nodo LUT con una tabla en elemento retorno false
   wxString nodepath = wxString(wxT(RENDERIZATION_NODE)) + wxT(NODE_SEPARATION_TOKEN)
         + wxString(CreatedNode().c_str()) + wxT(NODE_SEPARATION_TOKEN) + wxString(wxT(TABLE_NODE));

   wxXmlNode *pnode = pElement->GetNode(nodepath);
   if (!pnode) {
      REPORT_ERROR("D:No se pudo encontrar el nodo XML en el elemento \"%s\"",
                   pElement->GetName().c_str());
      return NULL;
   }

   // Creo nuevo renderizador
   LutRenderer *plut = new LutRenderer;
   plut->parameters_ = GetParameters(pElement->GetNode(wxT("")));

   // Verifico si los valores son validos
   if (!LoadFunction(plut->parameters_, pElement, pPreviousRenderer)
         || !ValidateTable(plut->parameters_, pPreviousRenderer)) {
      delete plut;
      return NULL;
   }

   return plut;
}

/**
 * Nombre del renderizador == al nombre del nodo
 * @return nobre del nodo a renderizar
 */
std::string LutRenderer::CreatedNode() const {
   return LUT_NODE;
}

/**
 * Renderiza el elemento dado un World en un Canvas
 * @param[in] pWorldWindow mundo a renderizar
 * @param[in] pCanvas imagen sobre la que se aplica la tabla
 * @param[in] pMask mascara para los valores a ignorar de la imagen
 */
bool LutRenderer::Render(const World *pWorldWindow, Canvas* pCanvas, Mask* pMask) {
   bool prevrenderizationstatus = true;

   // Verifico que datos de entrada sean correctos
   if (pPreviousRenderer_) {
      prevrenderizationstatus = pPreviousRenderer_->Render(pWorldWindow, pCanvas, pMask);
   }
   if (!pCanvas) {
      REPORT_AND_FAIL_VALUE("D:Canvas no puede ser nulo al aplicar LUT.", false);
   }
   if (!parameters_.lut_.active_) {
      REPORT_DEBUG("D:No se aplica LUT porque esta inhabilitada");
      return true && prevrenderizationstatus;
   }

   int sizex, sizey, bandcount = 0;
   std::string dt;
   GetInputParameters(sizex, sizey, bandcount, dt);
   pCanvas->GetSize(sizex, sizey);

   if (pCanvas->GetDataType() != dt) {
      std::string message = "D:No se puede renderizar, inconsistencia entre el ";
      message.append("tipo de datos del canvas y los de salida del renderer");
      REPORT_AND_FAIL_VALUE(message.c_str(), false);
   }
   int size = sizex * sizey;

   // Leo los datos en el canvas
   std::vector<int> bands(bandcount);
   std::vector<void*> data(bandcount);
   for (int b = 0; b < bandcount; b++) {
      bands[b] = b;
      data[b] = new unsigned char[sizex * sizey * pCanvas->GetDataSize()];
   }
   pCanvas->Read(bands, data);

   // Inicializo el canvas con la nueva dimension
   pCanvas->SetBandCount(parameters_.lut_.GetCount());
   pCanvas->SetSize(sizex, sizey);

   // Creo vector donde se guardan datos de salida temporales
   std::vector<void*> outdata(parameters_.lut_.GetCount());
   for (int b = 0; b < parameters_.lut_.GetCount(); b++)
      outdata[b] = new unsigned char[sizex * sizey * pCanvas->GetDataSize()];

   // Aplico LUTs a las bandas
   std::vector<int> newbands(parameters_.lut_.GetCount());
   int inputband = 0;
   LookUpTable::LutType table;
   for (int i = 0; i < parameters_.lut_.GetCount(); i++) {
      parameters_.lut_.GetLookUpTable(i, inputband).GetTable(table);
      if (inputband > bandcount) {
         REPORT_DEBUG("D:Nodo con numero de bandas incorrectas");
         return false;
      }
      parameters_.pFunction_(outdata[i], data[inputband], size, table);
      newbands[i] = i;
   }
   pCanvas->Write(newbands, outdata);

   // Elimino datos de entrada temporales
   for (int b = 0; b < bandcount; b++)
      delete[] static_cast<unsigned char*>(data[b]);

   // Elimino datos de salida temporales
   for (int i = 0; i < parameters_.lut_.GetCount(); i++)
      delete[] static_cast<unsigned char*>(outdata[i]);

   return true && prevrenderizationstatus;
}

/**
 * Obtiene el "bounding box" del elemento renderizado
 * @param[in]	pWorld: puntero a la ventana del mundo.
 * @param[in]	Ulx: uper-left x
 * @param[in]	Uly: uper-left y
 * @param[in]	Lrx: lower-right x
 * @param[in]	Lry: lower-right y
 */
void LutRenderer::GetBoundingBox(const World *pWorld, double &Ulx, double &Uly, double &Lrx,
                                 double &Lry) {
}

/**
 * Obtiene los parametros de entrada del renderer
 * @param[out] SizeX Tamanio en X requerido por el renderizador.
 * @param[out] SizeY Tamanio en Y requerido por el renderizador.
 * @param[out] BandCount Bandas requeridas.
 * @param[out] DataType Tipo de dato.
 */
void LutRenderer::GetInputParameters(int &SizeX, int &SizeY, int &BandCount,
                                     std::string &DataType) const {
   if (pPreviousRenderer_) {
      pPreviousRenderer_->GetOutputParameters(SizeX, SizeY, BandCount, DataType);
   }
}

/**
 * Obtiene los parametros de salida del renderer
 * @param[out] SizeX Tamanio en X requerido por el renderizador.
 * @param[out] SizeY Tamanio en Y requerido por el renderizador.
 * @param[out] BandCount Bandas requeridas.
 * @param[out] DataType Tipo de dato.
 */
void LutRenderer::GetOutputParameters(int &SizeX, int &SizeY, int &BandCount,
                                      std::string &DataType) const {
   if (pPreviousRenderer_) {
      pPreviousRenderer_->GetOutputParameters(SizeX, SizeY, BandCount, DataType);
   }
   if (parameters_.lut_.active_) {
      BandCount = parameters_.lut_.GetCount();
   }
}

// ----------------------- METODOS DE ACTUALIZACION -------------------------
/**
 * Actualizacion de parametros para representar un elemento
 * @param[in]	pElement: puntero al elemento.
 */
void LutRenderer::Update(Element *pElement) {
   parameters_ = GetParameters(pElement->GetNode(wxT("")));
   if (!LoadFunction(parameters_, pElement, pPreviousRenderer_)
         || !ValidateTable(parameters_, pPreviousRenderer_)) {
      REPORT_DEBUG("D:Error en los parametros");
      parameters_.lut_.active_ = false;
   }
}

/**
 * genera un nodo LUT a partir de los parametros
 * @param[in] RenderParameters con los que se crea nodo LUT
 * @return nodo generado
 */
wxXmlNode *LutRenderer::GetXmlNode(const LutRenderer::Parameters &RenderParameters) {
   wxXmlNode *plutnode = NULL;
   LutArray::ToXml(RenderParameters.lut_, plutnode);
   return plutnode;
}

/**
 * genera los parametros a partir de un nodo elemento con nodo LUT dentro
 * de renderizadores.
 * @param[in] pNode nodo elemento con nodo lut interno
 * @return parametros obtenidos
 */
LutRenderer::Parameters LutRenderer::GetParameters(const wxXmlNode *pNode) {
   Parameters params;
   params.lut_.active_ = false;

   // Obtengo nodo de renderizacion
   wxXmlNode *pdnode = pNode->GetChildren();
   wxXmlNode *pcnode = NULL;
   while (pdnode != NULL) {
      if (pdnode->GetName() == wxT(RENDERIZATION_NODE)) {
         pcnode = pdnode->GetChildren();
         pdnode = NULL;
      } else {
         pdnode = pdnode->GetNext();
      }
   }

   // Obtengo nodo lut
   bool found = false;
   while (pcnode != NULL && !found) {
      found = LutArray::FromXml(pcnode, params.lut_);
      pcnode = pcnode->GetNext();
   }
   if (!found) {
      REPORT_DEBUG("D: Se intento obtener parametros de LUT de elemento sin este nodo");
   }
   return params;
}

/**
 * Verifica que exista nodo LUT en nodo de elemento y carga en params funcion
 * del tipo correcto
 * @param[out] Params carga la funcion de mapeo del tipo correcto
 * @param[in] pElement elemento en el que busca el nodo LUT
 * @param[in] pPreviousRenderer renderer anterior
 */
bool LutRenderer::LoadFunction(Parameters &Params, Element* pElement, Renderer *pPreviousRenderer) {
   // Veo si tipo de entrada es correcto
   int x, y, b;
   std::string datatype;
   pPreviousRenderer->GetOutputParameters(x, y, b, datatype);
   Params.pFunction_ = lutTypeMap[datatype];
   if (!Params.pFunction_) {
      REPORT_ERROR("D:Tipo de dato (%s) no manejado", NULL, datatype.c_str());
      return false;
   }
   return true;
}

/**
 * Valida que la tabla no este vacia y que las bandas de entrada no exedan las
 * bandas en pPreviousRenderer. Si la tabla no esta activa retorna true ya que no
 * se aplica ningun cambio al canvas.
 * @param[in] Params nuevos parametros
 * @param[in] pPreviousRenderer renderizador anterior.
 * @return indica si se puede usar la tabla sobre el resultado del renderizador
 * anterior.
 */
bool LutRenderer::ValidateTable(const Parameters &Params, Renderer *pPreviousRenderer) {
   // Veo si tengo renderizador anterior
   if (!pPreviousRenderer) {
      return false;
   }

   // Si no esta activa no es necesario validar
   if (!Params.lut_.active_) {
      return true;
   }

   int sizex, sizey, bandcount = 0;
   std::string dt;
   pPreviousRenderer->GetOutputParameters(sizex, sizey, bandcount, dt);

   // si no hay pares de entrada / salida o tablas => no valida
   bool validated = !(Params.lut_.GetCount() == 0);

   // Verifico que datos en parameters sean correctos
   int inputband = 0;
   for (int i = 0; i < Params.lut_.GetCount() && validated; i++) {
      Params.lut_.GetLookUpTable(i, inputband);
      if (inputband >= bandcount) {
         validated = false;
      }
   }

   return validated;
}

/**
 * Agrega un nuevo nodo LUT, antes del nodo cast, o zoom si este no existiese .
 * @param[in] pElement XMLElement del elemento a asignar el nuevo LUT
 * @param[in] pRenderizationNode Nodo XML de renderizacion.
 * @param[in] pLutNode nodo tipo LUT a asignar al elemento
 * @return resultado de la operacion.
 */
bool SetNewLutNode(Element* pElement, wxXmlNode *pRenderizationNode, wxXmlNode* pLutNode) {
   // Si no hay un nodo nuevo retorna
   if (!pLutNode) {
      return true;
   }

   // Busco nodo data cast
   wxString castnodepath = RENDERIZATION_NODE NODE_SEPARATION_TOKEN DATA_CAST_NODE;
   wxXmlNode *pnextrenderer = pElement->GetNode(castnodepath);

   // Si no lo encuentro busco nodo zoom
   if (pnextrenderer == NULL) {
      wxString zoomnodepath = RENDERIZATION_NODE NODE_SEPARATION_TOKEN INTERPOLATION_NODE;
      pnextrenderer = pElement->GetNode(zoomnodepath);
   }

   // Agrego nodo antes del nodo. Si no encontro ninguno lo agrego al final
   pElement->AddNode(pRenderizationNode, pLutNode, pnextrenderer, false);
   return true;
}

}
