/*! \file View.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef VIEW_H_
#define VIEW_H_

// Includes standard
#include <stddef.h>

// Defines
#if defined(__DEBUG__)
#  include "logmacros.h"
#endif
#include "Model.h"

#define __AVOID_PURE_VIRTUAL_CALL__

// ------------------------------------------------------------------------------
// VIEWER
// ------------------------------------------------------------------------------

/** Clase View para la implementacion del Model-View */
/**
 *  Esta clase se registra con un Model y recibe Update() con el codigo de
 * registracion.
 *  El metodo Update() debe implementarse en las clases derivadas y proveer
 * la funcionalidad de actualizacion del modelo.
 */
class View {
   /** Ctor. de Copia. */
   View(const View &View);

public:
// ------------------------------------------------------------------------------
   /** constructor */
   View() :
         pModel_(NULL) {
   }
// ------------------------------------------------------------------------------
   /** destructor */
   virtual ~View() {
   }
// ------------------------------------------------------------------------------

#if defined(__DEBUG__) || defined(__AVOID_PURE_VIRTUAL_CALL__)
// ------------------------------------------------------------------------------
   /** Metodo de actualizacion del viewer, se ejecuta ante un cambio en el model */
// debe implementarse en las clases derivadas
   virtual void Update(Model *pModel)
#  ifdef REPORT_ERROR
                       {
      REPORT_ERROR("D:Se llamo a virtual Viewer::Update()=0. Esto no debe pasar!!!!");
   }
#  else  // REPORT_ERROR
   {}
#  endif // REPORT_ERROR
// ------------------------------------------------------------------------------
   /** Metodo que recibe al destruirse el modelo */
   virtual void OnModelDelete(Model *pModel)
#  ifdef REPORT_ERROR
                              {
      REPORT_ERROR(
            "D:Se llamo a virtual Viewer::OnModelDelete()=0. Esto no debe pasar!!!");
   }
#  else  // REPORT_ERROR
   {}
#  endif // REPORT_ERROR
#else // __DEBUG__ || __AVOID_PURE_VIRTUAL_CALL__
// ------------------------------------------------------------------------------
   /** Metodo de actualizacion del viewer, se ejecuta ante un cambio en el model */
// debe implementarse en las clases derivadas
   virtual void Update(Model *pModel)=0;
// ------------------------------------------------------------------------------

   /** Metodo que recibe el viewer al destruirse el modelo */
   virtual void OnModelDelete(Model *pModel)=0;
// ------------------------------------------------------------------------------

#endif   // __DEBUG__ || __AVOID_PURE_VIRTUAL_CALL__
   /** Metodo para setear el modelo */
   void SetModel(Model *pModel) {
      pModel_ = pModel;
   }
// ------------------------------------------------------------------------------

   /** Metodo para acceder al modelo */
   template<class ModelClass>
   ModelClass *GetModel() const {
      if (!Model::IsValid(pModel_)) {
         return NULL;
      }
      ModelClass* pmodel = dynamic_cast<ModelClass*>(pModel_);
#ifdef __DEBUG__
      assert(pmodel==pModel_);
#endif
      return pmodel;
   }
// ------------------------------------------------------------------------------

protected:
private:
   Model *pModel_; /*! Guardo el modelo */
};
#else

class View;

#endif /*VIEW_H_*/