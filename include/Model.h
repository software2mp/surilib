/*! \file Model.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef MODEL_H_
#define MODEL_H_

// Includes standard
#include <list>
#include <set>
#include <vector>

// Includes suri

class View;

/** Clase Model para implementacion del Model-View */
/**
 * La clase Model permite que los viewers se conecten a ella y propaga los
 * cambios a traves del View<ViewerClass>::Update().
 * La clase maneja un conjunto de viewers registrados que la observan y
 * procesan ante un envio de cambio en el modelo.
 *  
 * \todo (danieltesta-#4287): Revisar el uso del elemento asociado (GetAssociatedElement)
 */
class Model {
   /** Tipo definido para la lista de Viewers */
   typedef std::list<View*> ViewerListType;
   /** Tipo definido para el set de Viewers */
   typedef std::set<View*> ViewerSetType;

public:
// ------------------------------------------------------------------------------
   /** Constructor */
   Model();
   /** Constructor de copia */
   Model(const Model &M);

// ------------------------------------------------------------------------------
   /** destructor */
   virtual ~Model();
// ------------------------------------------------------------------------------
   /** Desregistra todos los viewer del modelo y llama a ModelDelete para cada uno de ellos */
   virtual void Clean();
// ------------------------------------------------------------------------------
   /** registra un viewer al modelo y le asigna este ULTIMO */
// ATENCION: Parece que hay viewers que miran mas de un modelo. La asignacion no andaria bien en
// este caso.
   bool RegisterViewer(View *pViewer);
// ------------------------------------------------------------------------------
   /** desregistra un visualizado */
   bool UnregisterViewer(View *pViewer);
// ------------------------------------------------------------------------------
   /** Pone el estado como cambiado */
   void SetChanged();
// ------------------------------------------------------------------------------
   /** Informa si el Modelo fue modificado */
   bool HasChanged() const;

// ------------------------------------------------------------------------------
   /** manda el mensaje de actualizacion a todos los viewers del modelo */
   void SendViewerUpdate();
// ------------------------------------------------------------------------------
   /** Previene que se envie actualizacion en el proximo SendViewerUpdate() */
// DEPRECATED: USAR BlockViewerUpdate() Y UnblockVIewerUpdate() desde ahora
   void PreventNextViewerUpdate(bool Status = true);
// ------------------------------------------------------------------------------
   /** Previene que se envie actualizacion hasta llamada a UnblockViewerUpdate() */
// Tener cuidado que es acumulatorio. No bloquea Update si blockUpdate_==0
   bool IsViewerUpdateBlocked();
   int CountViewerUpdateBlock();
   void BlockViewerUpdate();
   void UnblockViewerUpdate();

// ------------------------------------------------------------------------------
   static bool IsValid(Model *pModel);

   /** Obtiene los viewers registrados */
   ViewerListType GetViewers() {
      return viewers_;
   }

   /** Establece los viewers para el modelo */
   void SetViewers(ViewerListType Viewers) {
      viewers_ = Viewers;
   }

   /** Obtiene los viewers unicos registrados */
   ViewerSetType GetUniqueViewers() {
      return uniqueviewers_;
   }

   /** Obtiene los viewers unicos para el modelo */
   void SetUniqueViewers(ViewerSetType UniqueViewers) {
      uniqueviewers_ = UniqueViewers;
   }

protected:
   ViewerListType viewers_; /*! Lista de los viewers */
   ViewerSetType uniqueviewers_; /*! conjunto de viewers para garantizar */
   /* unicidad */
   bool preventNextViewerUpdate_; /*! flag que previene la actualizacion de */
   /* las vistas */
   int blockViewerUpdate_; /*! contador que previene la actualizacion de las */
   /* vistas desde una llamada al BlockUpdate() hasta */
   /* una llamada al UnblockUpdate() */
   bool hasChanged_; /*! Flag que determina si cambio o no */
// bool isUpdating_;	/*! Flag para evitar reentrar en el update. NO USADO!!! */
   View *pUpdatedElement_; /*! Elemento en estado actualizado \todo: deberia */
   /* ser un std::set para evitar recursion cruzada */
   bool destroying_; /*! Flag para indicar que se esta destruyendo */
private:
   static std::set<Model*> models_; /*! conjunto de modelos que no fueron */
   /* destruidos */
};

#endif /*MODEL_H_*/
