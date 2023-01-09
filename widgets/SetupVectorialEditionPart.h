/*! \file SetupVectorialEditionPart.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2014-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef SETUPVECTORIALEDITION_H_
#define SETUPVECTORIALEDITION_H_

#include <vector>

#include "suri/Part.h"
#include "SimpleVectorStylePart.h"
#include "suri/DataViewManager.h"
namespace suri {

/** Clase encargada de la configuracion de la visualizacion de la
 *  edicion vectorial de la aplicacion 
 **/
class SetupVectorialEditionPart : public Part {
public:
   /** Ctor. */
   SetupVectorialEditionPart(DataViewManager* pDataViewManager);
   /** Dtor. */
   virtual ~SetupVectorialEditionPart();
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
   /** Escribe el nuevo estilo en el archivo de configuracion. */
   void WriteNewStyles();
   friend class SimpleVectorStylePart;
private:
   std::vector<std::string> parameters_;
   std::vector<std::string> panels_;
   // Parts
   std::vector<ui::SimpleVectorStylePart*> styles_;
   // Estilos.
   std::string creationStyle_;
   std::string deletingStype_;
   std::string editionStyle_;
   std::string selectionStyle_;
   DataViewManager* pDataViewManager_;

};

} /* namespace suri */

#endif /* SETUPVECTORIALEDITION_H_ */
