/** \file SingleLayerBandSelectionPart.h
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef SINGLELAYERBANDSELECTIONPART_H_
#define SINGLELAYERBANDSELECTIONPART_H_

// Includes standar
#include <vector>
#include <list>
#include <utility>

// Icludes suri
#include "suri/Part.h"
#include "suri/RasterElement.h"

// Includes wx
#include "wx/wx.h"
#include "wx/listctrl.h"

#include "SelectionPart.h"

namespace suri {

DECLARE_EVENT_OBJECT(SingleLayerBandSelectionEvent);

/**
 * Clase que representa un part para la seleccion de bandas desde
 * una sola capa.
 */
class SingleLayerBandSelectionPart : public SelectionPart {
public:
   /** Declara el objeto de eventos como clase friend */
   FRIEND_EVENT_OBJECT(SingleLayerBandSelectionEvent);

   /** Ctor */
   SingleLayerBandSelectionPart(RasterElement * pElement);

   /** Dtor */
   virtual ~SingleLayerBandSelectionPart();

   /** Crea la ventana de la herramienta */
   virtual bool CreateToolWindow();

   /** Devuelve la lista de bandas seleccionadas en la lista de entrada */
   virtual std::vector<BandInfo> GetInputSelectedBands();

   /** Devuelve el nombre de todas las banda disponibles */
   virtual std::vector<BandInfo> GetAvailableBands() const;

   /**
    * Ajusta el largo de la ultima columna para que ocupe todo el espacio
    * libre en el cotrol lista. Se llama desde el OnResize de la clase
    * que lo contiene.
    */
   virtual void UpdateSourceCtrl();

private:
   EVENT_OBJECT_PROPERTY(SingleLayerBandSelectionEvent); /*! Objeto para conectar eventos */

   RasterElement* pElement_;
   std::vector<BandInfo> bands_;

   /** Configura la lista donde se cargan las bandas de entrada */
   void ConfigureInputBandList();

   /** Carga la lista de bandas de entrada */
   void LoadInputBands();

   /** Metodo que se ejecuta al hacer doble click en la lista de entrada */
   void OnInputSelection(wxListEvent &Event);

};

}  // namespace suri

#endif  // SINGLELAYERBANDSELECTIONPART_H_
