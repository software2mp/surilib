/*! \file ViewerPropertiesPart.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2014-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef VIEWERPROPERTIESPART_H_
#define VIEWERPROPERTIESPART_H_

// Includes Estandar
// Includes Suri
#include "suri/Part.h"

// Includes Wx
// Defines
// forwards

namespace suri {

class Viewer2DProperties;
class Viewer3DProperties;
class DynamicViewerWidget;
class LibraryManager;
class ViewerWidget;
class DataViewManager;

class ViewerPropertiesPart : public Part {
public:
   /** constructor **/
   explicit ViewerPropertiesPart(DynamicViewerWidget* pViewer, LibraryManager* pLibManager,
                                 DataViewManager* pDataViewManager);
   /** destructor **/
   virtual ~ViewerPropertiesPart();
   /** Indica si el Part tiene cambios para salvar. */
   virtual bool HasChanged();
   /** Salva los cambios realizados en el Part. */
   virtual bool CommitChanges();
   /** Restaura los valores originales del Part. */
   virtual bool RollbackChanges();
   /** Inicializa el part */
   virtual void SetInitialValues();
   /** Actualiza el estado de la parte */
   virtual void Update();
   /** Implementacion del metodo de creacion de ventana de Widget */
   virtual bool CreateToolWindow();

private:
   Viewer2DProperties* p2dProperties_;
   Viewer3DProperties* p3dProperties_;
   ViewerWidget* pViewer_;
};

} /** namespace suri */

#endif /* VIEWERPROPERTIESPART_H_ */
