/*! \file Renderer.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef RENDERER_H_
#define RENDERER_H_

#include <string>
#include <map>

#include "NoDataValue.h"

/** Macro para registrar Renderers en forma automatica */
#define AUTO_REGISTER_RENDERER(RendererClass) namespace {                     \
   class Dummy {                                                              \
public:                                                                 \
   Dummy() {suri::Renderer::Register(new RendererClass); }};              \
   Dummy dummy; }

class wxXmlNode;

/** namespace suri */
namespace suri {
// forwards
class World;
class Canvas;
class Mask;
class Element;

/** Clase abstracta que renderiza elementos */
/**
 *  Esta clase realiza los pasos de renderizacion de un Element. Cada
 * paso de renderizacion engancha con el siguiente formando un pipe que a la
 * salida ha generado la imagen con todos sus procesamientos.
 *
 *  Las clases derivadas de esta son las encargadas de las diversas etapas de
 * la renderizacion (lectura de bandas, realce, zoom, filtro, etc.)
 *
 *  Cuenta con metodos estilo CloneFactory, Register y Create, para registrar
 * instancia de una clase derivada o crear una clase derivada.
 */
class Renderer : public NoDataValue {
   /** Ctor. de Copia. */
   Renderer(const Renderer &Renderer);

protected:
   /** ctor */
   Renderer();
   /** dtor */
   virtual ~Renderer();
public:
// ------------------- METODOS ESTATICOS DE CONVERSION ----------------------
   /** Recorre los Renderizadores registrados y crea el adecuado para el NodeName */
   static Renderer *Create(const std::string &NodeName, Element *pElement,
                           Renderer *pPreviousRenderer);
   /** Registra una instancia de un renderizador */
   static void Register(Renderer *pRenderer);
   /** Desregistra todos los renderers con la clave indicada */
   static void Unregister(const std::string &RendererKey);
   /** Destructor de clases derivadas */
   static void Destroy(Renderer* &pRenderer);
// -------------------------- METODO DE CREACION ----------------------------
   /** Crea un renderizador a partir de un pElement */
   /**
    *  Metodo virtual abstracto puro utilizado para crear con el metodo
    * estatico Create() instancias de clases derivadas registradas con el
    * metodo RegisterViewer().
    * @param[in] pElement Elemento a renderizar.
    * @param[in] pPreviousRenderer Renderizador anterior.
    */
   virtual Renderer *Create(Element *pElement,
                            Renderer *pPreviousRenderer = NULL) const=0;
   /** Nombre del renderizador == al nombre del nodo */
   /**
    *  Retorna el nombre del nodo que maneja el renderizador.
    * @return Nombre del nodo manejado por la clase derivada.
    */
   virtual std::string CreatedNode() const=0;
// ----------------------- METODOS DE RENDERIZACION -------------------------
   /** Renderiza el elemento dado un World en un Canvas */
   virtual bool Render(const World *pWorldWindow, Canvas* pCanvas, Mask* pMask)=0;
   /** Obtiene el "bounding box" del elemento renderizado */
   /**
    * \pre el subset debe ser seteado antes de llamar a esta funcion con un
    *      valor invalido.
    * \post Se refleja el subset de mundo que renderiza.
    * \post No se modifican los parametros en caso de falla o subset nulo
    * @param[in] pWorld Mundo sobre el cual se debe devolver el BB
    * @param[out] Ulx ULx de mundo renderizado.
    * @param[out] Uly ULy de mundo renderizado.
    * @param[out] Lrx LRx de mundo renderizado.
    * @param[out] Lry LRy de mundo renderizado.
    */
   virtual void GetBoundingBox(const World *pWorld, double &Ulx, double &Uly,
                               double &Lrx, double &Lry)=0;
   /** Obtiene los parametros de entrada del renderer */
   /**
    *  Permite obtener los parametros de entrada que requiere el renderizador,
    * es decir, los datos que espera de pPreviousRenderer.
    *
    *  En caso de que no tenga algun requerimiento, el parametro tomara los
    * siguientes valores.
    * @code
    *  SizeX = -1
    *  SizeY = -1
    *  BandCount = -1
    *  DataType = DataInfo<void>::Name
    * @endcode
    *
    * @param[out] SizeX Tamanio en X requerido por el renderizador.
    * @param[out] SizeY Tamanio en Y requerido por el renderizador.
    * @param[out] BandCount Bandas requeridas.
    * @param[out] DataType Tipo de dato.
    */
   virtual void GetInputParameters(int &SizeX, int &SizeY, int &BandCount,
                                   std::string &DataType) const=0;
   /** Obtiene los parametros de salida del renderer */
   /**
    *  Permite obtener los parametros de salida del renderizador.
    *
    * @param[out] SizeX Tamanio en X requerido por el renderizador.
    * @param[out] SizeY Tamanio en Y requerido por el renderizador.
    * @param[out] BandCount Bandas requeridas.
    * @param[out] DataType Tipo de dato.
    */
   virtual void GetOutputParameters(int &SizeX, int &SizeY, int &BandCount,
                                    std::string &DataType) const=0;
// ----------------------- METODOS DE ACTUALIZACION -------------------------
   /** Actualizacion de parametros para representar un elemento */
   /**
    *  Metodo abstracto de actualizacion del renderizador. Examina el elemento
    * y actualiza sus parametros de manera de reflejar los cambios que hayan
    * sido aplicados.
    * @param[in] pElement Elemento a analizar por cambios.
    */
   virtual void Update(Element *pElement)=0;
   /** actualiza el renderizador anterior */
   void SetPrevious(Renderer *pPrevious) {
      pPreviousRenderer_ = pPrevious;
   }
   /** Indica si el renderizador tiene cambios para aplicar */
   virtual bool IsDirty();
protected:
   /** Obtiene el nodo de renderizacion */
   static wxXmlNode *GetRenderizationNode(const Element *pElement);
   Renderer *pPreviousRenderer_; /*! Renderizador anterior en la tuberia */
private:
   /** tipo de lista para guardar los renderizadores registrados */
   typedef std::multimap<std::string, Renderer*> RendererMapType;
   static RendererMapType *pRenderers_; /*! Mapa con los renderizadores */
};
}

#endif /*RENDERER_H_*/
