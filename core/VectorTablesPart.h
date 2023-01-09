/*! \file VectorTablesPart.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef VECTORTABLESPART_H_
#define VECTORTABLESPART_H_

// Includes standard
#include <vector>
// Includes Suri
#include "suri/Part.h"
// Includes Wx
// Includes App
// Defines
/**
 * No se usa vector en memoria porque para remplazar el que esta en disco no debe
 * estar abierto. El renderizador siempre tiene un Vector abierto.
 */
#undef __USE__MEMORY__VECTOR__

class wxUpdateUIEvent;

class wxChoicebook;

/** namespace suri */
namespace suri {
class VectorEditor;
class VectorDatasource;
class VectorElement;
#ifdef __USE__MEMORY__VECTOR__
class Vector;
#endif

/** declara clase VectorTablesPartEvent */
DECLARE_EVENT_OBJECT(VectorTablesPartEvent);

/** Parte para manejar las propiedades de varias capas de un vector */
/**
 * Carga las tablas de todas las capas de los vectores. Cada operacion que
 * se quiere realizar la aplica sobre cada capa
 * \todo No utilizar bool para determinar el tipo de clase que contiene.
 */
class VectorTablesPart : public Part {
public:
   /** Declara friend la clase VectorTablesPartEvent */
   FRIEND_EVENT_OBJECT(VectorTablesPartEvent);

   /** Ctor */
   VectorTablesPart(VectorDatasource* pVectorDatasource, bool HotlinkPartType = false);
   /** Ctor legacy **/
   VectorTablesPart(VectorElement* pVectorElement, bool HotlinkPartType = false);
   /** Dtor */
   virtual ~VectorTablesPart();

   /** Crea la ventana de la parte */
   virtual bool CreateToolWindow();
   /** Indica si el Part tiene cambios para salvar. */
   virtual bool HasChanged();
   /** Salva los cambios realizados en el Part. */
   virtual bool CommitChanges();
   /** Restaura los valores originales del Part. */
   virtual bool RollbackChanges();
   /** Inicializa los datos mostrados */
   virtual void SetInitialValues();
   /** Devuelve el icono de la parte */
   void GetWindowIcon(wxBitmap& ToolBitmap, int X, int Y) const;
   /** Informa si las tablas tienen datos correctos */
   virtual bool HasValidData();
   /** Evento UIUpdate */
   virtual void OnUiUpdate(wxUpdateUIEvent &Event);
   /** Actualiza el estado de la parte */
   virtual void Update() {
   }

protected:
   /** Carga los datos de disco a la capa indicada */
   bool LoadLayer(int Layer);
   /** Guarda los datos de la capa a disco */
   bool SaveLayer(int Layer);
   VectorDatasource* pVectorDatasource_;
   VectorElement* pVectorElement_; /*! vetor que se esta editando */
   VectorEditor* pVectorEditor_; /*! permite editar vectores */
#ifdef __USE__MEMORY__VECTOR__
   Vector* pMemoryVector_; /*! vector temporal en memoria. Evita que se elimine mientras se use la */
   // herramienta
#endif
   wxChoicebook* pChoicebook_; /*! herramienta que se usa para editar */
   std::vector<Part*> layerTableParts_; /*! vector con tablas de cada capa */
   bool hotlinkPartType_; /*! Variable que se utiliza para determinar el */
   /* tipo de part que se debe mostrar */
   /** Declara la propiedad pEventHandler_ del tipo VectorTablesPartEvent* */
   EVENT_OBJECT_PROPERTY(VectorTablesPartEvent);
};
}  // namespace suri

#endif /* VECTORTABLESPART_H_ */
