/*! \file QueryButton.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef QUERYBUTTON_H_
#define QUERYBUTTON_H_

// Includes standard
// Includes Suri
#include "suri/ToggleButton.h"
#include "suri/messages.h"
#include "resources.h"
#include "Query.h"

// Includes Wx

// Defines

/** namespace suri */
namespace suri {
class ViewerWidget;
struct HotlinkData;

/** Clase con comportamiento comun a herramientas que abren un hotlink. */
/**
 * Boton que abre archivo de hotlink. Usa metodos virtuales para comportamiento
 * particular de contextos 2D y 3D
 * \note hereda de ToggleButton para que se muestre que la opcion activa es la
 *       de Query
 */
class QueryButton : public ToggleButton {
   /** Ctor. de Copia. */
   QueryButton(const QueryButton &QueryButton);

public:
   /** Ctor */
   QueryButton(wxWindow *pToolbar, ViewerWidget *pViewer);
   /** Dtor */
   virtual ~QueryButton() {
   }

   /** Cambia cursor a flecha con signo de interrogacion */
   virtual void DoSetCursor(bool State);
   /** Busca hotlinks y los despliega */
   virtual void DoOnLeftUp(wxMouseEvent &Event);
   /** Retorna el box del elemento activo */
   virtual bool GetQueryBox(int PosX, int PosY, int VectorSize, Subset &QueryBox)=0;
   /** Analiza si el click puede pertenecer a un elemento valido. */
   virtual bool IsValid(int PosX, int PosY)=0;
   /** Abre el hotlink usando el sistema operativo. */
   virtual void DisplayHotlink(const std::vector<HotlinkData> &Hotlinks);

protected:
   ViewerWidget* pViewer_; /*! Viewer */
   Query* pQuery_;
};
}

#endif /* QUERYBUTTON_H_ */
