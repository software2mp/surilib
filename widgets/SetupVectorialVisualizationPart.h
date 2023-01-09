/*! \file SetupVectorialVisualizationPart.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2014-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef SETUPVECTORIALVISUALIZATIONPART_H_
#define SETUPVECTORIALVISUALIZATIONPART_H_

#include "suri/Part.h"
#include "SimpleVectorStylePart.h"

namespace suri {

class SetupVectorialVisualizationPart : public Part {
public:
   /** Ctor. */
   SetupVectorialVisualizationPart();
   /** Destructor. */
   virtual ~SetupVectorialVisualizationPart();
   /** Crea el part (el interior de la ventana)*/
   virtual bool CreateToolWindow();
   /** Asigna el icono para el part.  */
   virtual void GetWindowIcon(wxBitmap& ToolBitmap, int X, int Y) const;
   /** Inicializa los controles */
   virtual void SetInitialValues();
   /** Indica si el Part tiene cambios para salvar. */
   virtual bool HasChanged();
   /** Salva los cambios realizados en el Part. */
   virtual bool CommitChanges();
   /** Restaura los valores originales del Part. */
   virtual bool RollbackChanges();
   /** Actualiza el estado de la parte */
   virtual void Update();
private:
   void WriteNewStyles();

   /** Parts */
   std::vector<ui::SimpleVectorStylePart*> pStyles_;

   /** Estilos */
   std::string PolygonStyle_;
   std::string LineStyle_;
   std::string PointStyle_;
};

} /* namespace suri */

#endif /* SETUPVECTORIALVISUALIZATIONPART_H_ */
