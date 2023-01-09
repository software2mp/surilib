/*! \file Query2DButton.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef QUERY2DBUTTON_H_
#define QUERY2DBUTTON_H_

// Includes suri
#include "QueryButton.h"
#include "suri/DataViewManager.h"

// Includes otros

// Defines
/** Define la cantidad de puntos a  utilizar para la reproyeccion del querybox */
#define _VECTORQUERY_EXTENT_POINTS_ 100
/** Define la tolerancia en pixels para el query de vectores */
#define QUERY_TOLERANCE 4

/** namespace suri */
namespace suri {

// forwards
class Viewer2D;

/** QueryButton que ejecuta el archivo asociado cuando se hace click sobre un link en vista 2D */
class Query2DButton : public QueryButton {
   /** Ctor. de Copia. */
   Query2DButton(const Query2DButton &Query2DButton);

public:
   /** ctor */
   /**
    * @param[in] pToolbar window donde se debe agregar el boton
    * @param[in] pViewer viewer sobre el que se hacen clicks para activar
    * @param[in] pDataViewManager administrador de informacion de la aplicacion
    * hotlink
    */
   Query2DButton(wxWindow *pToolbar, Viewer2D *pViewer, DataViewManager* pDataViewManager = NULL);
   /** dtor */
   virtual ~Query2DButton();

protected:

   /** Analiza si el click puede pertenecer a un elemento valido. */
   /**
    * @param[in] PosX posicion(X) sobre el que se hizo click
    * @param[in] PosY posicion(Y) sobre el que se hizo click
    */
   bool IsValid(int PosX, int PosY);
   /** Retorna el box del elemento activo */
   /**
    * @param[in] PosX lugar donde se hizo click x
    * @param[in] PosY lugar donde se hizo click y
    * @param[in] ElementSize tamanio del elemento
    * @param[out] QueryBox box con dimencion del elemento centrado en click
    * y en coordenadas de la imange.
    */
   bool GetQueryBox(int PosX, int PosY, int ElementSize, Subset &QueryBox);
   /** Activa la herramienta de consulta de pixel **/
   void Start();
   /** Desactiva la herramienta de consulta de pixel **/
   void End();
#ifdef __PIXEL_INFO__
   /** Metodo que se llama cuando se activa/desactiva el togglebutton **/
   virtual bool DoSetValue(bool State);
   /** Cambia cursor a flecha con signo de interrogacion */
   virtual void DoSetCursor(bool State) {}
   /** Busca hotlinks y los despliega */
   virtual void DoOnLeftUp(wxMouseEvent &Event) {}
#endif

private:
   DataViewManager* pDataViewManager_;
};
}
#endif /*QUERY2DBUTTON_H_*/
