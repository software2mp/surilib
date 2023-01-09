/*! \file PrincipalComponentAnalysisPart.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2014-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef PRINCIPALCOMPONENTANALYSISPART_H_
#define PRINCIPALCOMPONENTANALYSISPART_H_

#include "suri/Part.h"
#include "suri/ProcessAtributeProvider.h"
#include "suri/RasterElement.h"

namespace suri {

DECLARE_EVENT_OBJECT(PrincipalComponentAnalysisPartEvent);

class PrincipalComponentAnalysisPart : public Part, public ProcessAtributeProvider {
public:
   FRIEND_EVENT_OBJECT(PrincipalComponentAnalysisPartEvent);

   /**
    * Ctor.
    */
   PrincipalComponentAnalysisPart(RasterElement* pElement, bool Enable = true, bool Modified = false);

   /**
    * Dtor.
    */
   virtual ~PrincipalComponentAnalysisPart();

   /**
    * Indica si el Part tiene cambios para salvar.
    */
   virtual bool HasChanged();

   /**
    * Salva los cambios realizados en el Part.
    */
   virtual bool CommitChanges();

   /**
    * Restaura los valores originales del Part.
    */
   virtual bool RollbackChanges();

   /**
    * Inicializa el part.
    */
   virtual void SetInitialValues();

   /**
    * Devuelve si la parte tiene datos validos.
    */
   virtual bool HasValidData();

   /**
    * Actualiza el estado de la parte.
    */
   virtual void Update();

   /**
    * Implementacion del metodo de creacion de ventana de Widget.
    */
   virtual bool CreateToolWindow();
   /** Retorna el icono de la herramienta */
   virtual void GetWindowIcon(wxBitmap& ToolBitmap, int X = 0, int Y = 0) const;

private:
   EVENT_OBJECT_PROPERTY(PrincipalComponentAnalysisPartEvent);

   RasterElement* pElement_;
   bool changed_;

   /**
    * Handler para la seleccion del radio button correlacion.
    */
   void OnRadCorrelationSelected(wxCommandEvent& Event);

   /**
    * Handler para la seleccion del radio button covarianza.
    */
   void OnRadCovarianceSelected(wxCommandEvent& Event);

   /**
    * Handler para la seleccion del radio button total.
    */
   void OnRadTotalSelected(wxCommandEvent& Event);

   /**
    * Handler para la seleccion del radio button parcial.
    */
   void OnRadPartialSelected(wxCommandEvent& Event);

   /**
    * Habilita o deshabilita el control para la carga
    * de componentes principales.
    */
   void EnablePartialCount(bool Enable = true);
};

}  // namespace suri

#endif  // PRINCIPALCOMPONENTANALYSISPART_H_
