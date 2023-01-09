/*! \file TerrainPropertiesPart.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2013-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef TERRAINPROPERTIESPART_H_
#define TERRAINPROPERTIESPART_H_

// Includes suri
#include "suri/Part.h"
#include "suri/TerrainElement.h"

/** namespace suri */
namespace suri {
/** Declara la clase TerrainPropertiesPartEvent */
DECLARE_EVENT_OBJECT(TerrainPropertiesPartEvent);

/** Part con controles para las propiedades de un terreno */
/**
 * Tiene los controles calidad, exageracion y velocidad camara.
 * \note extiende Part para guardar los cambios en propiedades del terreno
 */
class TerrainPropertiesPart : public Part {
   /** Ctor. de Copia. */
   TerrainPropertiesPart(const TerrainPropertiesPart &TerrainPropertiesPart);

public:
   /** Declara friend la clase TerrainPropertiesPartEvent */
   FRIEND_EVENT_OBJECT(TerrainPropertiesPartEvent);
   /** Ctor. */
   TerrainPropertiesPart(TerrainElement *pElement);
   /** Dtor. */
   virtual ~TerrainPropertiesPart();
   /** Se crea la ventana de herramienta en las clases derivadas */
   virtual bool CreateToolWindow();
   /** Inicializa los controles */
   virtual void SetInitialValues();
   /** Indica si el Part tiene cambios para salvar. */
   virtual bool HasChanged();
   /** Salva los cambios realizados en el Part. */
   virtual bool CommitChanges();
   /** Restaura los valores originales del Part. */
   virtual bool RollbackChanges();
   /** Obtiene el icono del part */
   virtual void GetWindowIcon(wxBitmap& ToolBitmap, int X = 0, int Y = 0) const;
   /** Actualiza el estado de la parte */
   virtual void Update() {
   }
protected:
private:
   /** Evento de movimiento de sliders */
   void OnSliderScroll(wxScrollEvent &Event);

   TerrainElement *pElement_; /*! elemento Terreno. */
   EVENT_OBJECT_PROPERTY(TerrainPropertiesPartEvent); /*! Objeto que conecta */
   /* los eventos */
};
}  // namespace suri

#endif /*TERRAINPROPERTIESPART_H_*/
