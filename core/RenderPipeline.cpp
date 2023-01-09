/*! \file RenderPipeline.cpp */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

// Includes standar
#include <algorithm>

// Includes suri
#include "RenderPipeline.h"
#include "suri/Element.h"
#include "suri/DataTypes.h"
#include "suri/Canvas.h"
#include "suri/Renderer.h"
#include "suri/xmlnames.h"
#include "suri/XmlFunctions.h"
#include "Mask.h"
#include "logmacros.h"

/** Busca el renderizador del nombre dado */
template<class IterableType, class NameType>
struct equal_name : public std::binary_function<IterableType, NameType, bool> {
   /** Ctor. */
   equal_name() {
   }
   /** Dtor. */
   ~equal_name() {
   }
   /** Operador () */
   bool operator()(const IterableType &Renderer, const NameType &Name) const {
      return Renderer->CreatedNode() == Name;
   }
private:
};

/** namespace suri */
namespace suri {
// -------------------- PRIVADOS POR USAR CREATE/DESTROY --------------------

/**
 * Constructor
 * @return instancia de la clase RenderPipeline
 */
RenderPipeline::RenderPipeline() {
}

/**
 * Destructor
 */
RenderPipeline::~RenderPipeline() {
   PipelineType::iterator it = renderers_.begin();
   for (; it != renderers_.end(); it++) {
      Renderer::Destroy((*it));
   }
}

/**
 * ctor de copia
 * @return instancia de la clase, copia del objeto recibido por parametro.
 */
RenderPipeline::RenderPipeline(const RenderPipeline&) {
}

// --------------------- ESTATICOS CREACION/DESTRUCCION ---------------------

/**
 * Carga los renderers que figuran en el xml en un vector
 * @param[in] pElement elemento que se quiere renderizar
 * @param[in] pOutputCanvas canvas que se usa al renderizar
 * @return pipeline creado. NULL si fallo por alguna razon
 */
RenderPipeline *RenderPipeline::Create(Element *pElement, Canvas *pOutputCanvas) {
   if (!pElement) {
      return NULL;
   }
   RenderPipeline *ppipeline = new RenderPipeline;
   wxXmlNode *pnode = pElement->GetNode(RENDERIZATION_NODE);
   if (!pnode) {
      REPORT_AND_FAIL_VALUE("D:No se encontro el nodo %s", NULL, RENDERIZATION_NODE);
   }
   // TODO: BORRRAAARRR
   std::string aux(Xml2String(pnode));
   aux.c_str();
   pnode = pnode->GetChildren();
   // recorro el sub-arbol renderizacion del elemento
   Renderer *prenderer = NULL;
   while (pnode) {
      Renderer *ptemp = Renderer::Create(pnode->GetName().c_str(), pElement, prenderer);
      if (ptemp) {
         prenderer = ptemp;
         ppipeline->renderers_.push_back(prenderer);
      } else {
         SHOW_WARNING(message_RENDERER_CREATION_ERROR_ss,
                      pnode->GetName().c_str(), pElement->GetName().c_str());
         // no se traduce
         // porque los macros
         // show_ lo hacen
      }
      pnode = pnode->GetNext();
   }
   if (!ppipeline->renderers_.empty()) {
      // verifico los parametros IO de los Renderers
      PipelineType::reverse_iterator rit1 = ppipeline->renderers_.rbegin(), rit2;
      rit2 = rit1;
      rit2++;
      int x1 = 0, y1 = 0, b1 = 0, x2 = 0, y2 = 0, b2 = 0;
      std::string dt1, dt2;

      while (rit2 != ppipeline->renderers_.rend()) {
         (*rit1)->GetInputParameters(x1, y1, b1, dt1);
         (*rit2)->GetOutputParameters(x2, y2, b2, dt2);

#ifdef __UNUSED_CODE__
         /*Prueba que el renderpipeline se crea una sola vez => no tiene sentido
          * la verificacion. Ver por ejemplo que si a una imagen le cambio la
          * cantidad de bandas con la cual debe renderizar (le seteo 1 banda)
          * deberia fallar porque VectorRenderer utiliza 3 (hardcode).*/
         REPORT_DEBUG("Element: %s", pElement->GetName().c_str() );
         REPORT_DEBUG("Input: %d,%d,%d,%s", x1, y1, b1, dt1.c_str() );
         REPORT_DEBUG("Output: %d,%d,%d,%s", x2, y2, b2, dt2.c_str() );
#endif

         if ((b1 != b2 || dt1 != dt2) && (b1 != -1 && dt1 != DataInfo<void>::Name)) {
            SHOW_ERROR(
                  "No se puede crear un sistema de renderizacion para el elemento %s",
                  pElement->GetName().c_str());
            REPORT_ERROR("Datos de entrada %d, %s", b2, dt2.c_str());
            REPORT_ERROR("Datos de salida %d, %s", b1, dt1.c_str());
            REPORT_ERROR(
                  "Inconsistencia entre %s -> %s", (*rit2)->CreatedNode().c_str(), (*rit1)->CreatedNode().c_str());
            delete ppipeline;
            return NULL;
         }
         rit1++;
         rit2++;
      }
#ifdef __UNUSED_CODE__
      // No se verifica el dato de salida porque limpia el canvas al agregar
      // un elemento nuevo. Luego al renderizar se remplaza paintedBitmap con
      // el canvas en blanco y titila la pantalla si hay update durante
      // renderizado.
      // Para solucionarlo tendria que haber metodo en canvas que permita
      // verificar si soporta el nuevo tamanio/tipo de dato sin modificar nada.
      // Otra opcion es modificar el sistema de renderizacion para que no
      // modifique el canvas de salida.

      // Verifico contra el canvas de salida
      if ( pOutputCanvas )
      {
         ppipeline->renderers_.back()->GetOutputParameters(x2, y2, b2, dt2);
         // se asume que los canvas especiales no aceptan modificaciones
         TRY {
            pOutputCanvas->SetBandCount(b2);
            pOutputCanvas->SetDataType(dt2);
         }CATCH {
            SHOW_ERROR("No se puede crear un sistema de renderizacion para el elemento %s",
                  pElement->GetName().c_str() );
            SHOW_ERROR("Imposible renderizar tipos de dato %d@%s", b2, dt2.c_str() );
            delete ppipeline;
            return NULL;
         }
         b1 = pOutputCanvas->GetBandCount();
         dt1 = pOutputCanvas->GetDataType();
         if ( b1!=b2 || dt1!=dt2 )
         {
            SHOW_ERROR("No se puede crear un sistema de renderizacion para el elemento %s",
                  pElement->GetName().c_str() );
            delete ppipeline;
            return NULL;
         }
      }
#endif
      REPORT_DEBUG("D:Renderizador para el elemento %s creado con exito.",
                   pElement->GetName().c_str());
      return ppipeline;
   }

   REPORT_DEBUG("D:CreatePipeline, No se creo ningun renderizador.");
   delete ppipeline;
   return NULL;
}

/**
 * Elimina el pipeline y deja pPipeline en NULL
 * @param[in] pPipeline pipeline que se quiere eliminar
 */
void RenderPipeline::Destroy(RenderPipeline* &pPipeline) {
   delete pPipeline;
   pPipeline = NULL;
}

// ----------------------------- RENDERIZACION ------------------------------
/**
 *  Llama a el ultimo renderizador y este propaga la renderizacion a los otros.
 * @param[in] pWorld Mundo de donde salen los parametros del subset a renderizar
 * @param[in] pMask Mascara inicial de renderizacion
 * @param[out] pCanvas Canvas sobre el que se renderiza
 * @param[out] pMask Mascara modificada por la renderizacion
 * \post Determina si el pCanvas es compatible con el primer renderizador
 *  y de no serlo genera uno en memoria y luego copia.
 *  @return true si la renderizacion tuvo exito.
 *  @return false si la renderizacion no tuvo exito.
 */
bool RenderPipeline::Render(const World *pWorld, Canvas* pCanvas, Mask* pMask) {
   if (renderers_.back()->Render(pWorld, pCanvas, pMask)) {
      // Para que los vectores (en el DC) pasen a la matriz
      REPORT_DEBUG("D:RenderPipeline::Render()");
      pCanvas->Flush(pMask);
      if (pMask)
         pMask->Flush(NULL);
      return true;
   }
   return false;
}

/**
 * @param[in]	pWorld: puntero a la ventana del mundo.
 * @param[in]	Ulx: uper-left x
 * @param[in]	Uly: uper-left y
 * @param[in]	Lrx: lower-right x
 * @param[in]	Lry: lower-right y
 * \pre el subset debe ser seteado antes de llamar a esta funcion con un
 *      valor invalido.
 * \post se refleja el subset de mundo que renderiza, o no se modifican los datos
 */
void RenderPipeline::GetBoundingBox(const World *pWorld, double &Ulx, double &Uly,
                                    double &Lrx, double &Lry) const {
   if (!renderers_.empty()) {
      (*renderers_.begin())->GetBoundingBox(pWorld, Ulx, Uly, Lrx, Lry);
   }
}

// ----------------------------- ACTUALIZACION ------------------------------
/** Metodo para actualizacion de elemento */
/**
 *  Actualiza el pipeline en funcion del nuevo nodo renderizacion. Crea nuevos
 * Renderizadores si son necesarios y reutiliza los que ya tiene. Destruye los
 * que quedan sin representacion en el XML del elemento.
 *
 * @param[in] pElement Elemento actualizandose
 */
void RenderPipeline::Update(Element *pElement) {
   PipelineType newrenderers;
   wxXmlNode *pnode = pElement->GetNode(RENDERIZATION_NODE);
   if (!pnode) {
      REPORT_AND_FAIL("D:No se encontro el nodo %s", RENDERIZATION_NODE);
   }
   pnode = pnode->GetChildren();
   // recorro el sub-arbol renderizacion del elemento
   Renderer *prenderer = NULL;
   while (pnode) {
      Renderer *ptemp = NULL;
      // si encuentra el renderizador lo reutiliza
      PipelineType::iterator it = find_if(
            renderers_.begin(), renderers_.end(),
            std::bind2nd(equal_name<Renderer*, wxString>(), pnode->GetName()));
      if (it != renderers_.end()) {
         ptemp = *it;
         renderers_.erase(it);
         REPORT_DEBUG("D: Reutilizando Renderer");
      } else  // sino crea uno nuevo
      {
         ptemp = Renderer::Create(pnode->GetName().c_str(), pElement, prenderer);
         REPORT_DEBUG("D: Creando Renderer Nuevo");
      }
      if (ptemp) {
         // actualiza el anterior
         ptemp->SetPrevious(prenderer);
         prenderer = ptemp;
         // lo agrego al nuevo pipeline
         newrenderers.push_back(prenderer);
      } else {
         SHOW_WARNING(message_RENDERER_CREATION_ERROR_ss,
                      pnode->GetName().c_str(), pElement->GetName().c_str());
      }
      pnode = pnode->GetNext();
   }
#ifdef __DEBUG__
   if (!newrenderers.empty()) {
      REPORT_DEBUG("D:Renderizador para el elemento %s creado con exito.",
                   pElement->GetName().c_str());
   }
#endif
   PipelineType::iterator it = renderers_.begin();
   // elimino los renderizadores que hayan podido quedar
   for (; it != renderers_.end(); it++)
      Renderer::Destroy(*it);
   // asigno el nuevo pipeline
   renderers_ = newrenderers;
   // actualizo
   it = renderers_.begin();
   for (; it != renderers_.end(); it++) {
      REPORT_DEBUG("D:%s::Update()", (*it)->CreatedNode().c_str());
      (*it)->Update(pElement);
   }
}
}
