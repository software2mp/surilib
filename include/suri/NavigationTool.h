/*! \file NavigationTool.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2013-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef NAVIGATIONTOOL_H_
#define NAVIGATIONTOOL_H_

// Includes Estandar
#include <string>

// Includes Suri
#include "suri/GenericTool.h"

// Includes Wx

// Defines

// forwards

namespace suri {

// forwards
class GoButton;
class ViewportLinkManager;
class CoordinatesNavigationInputInterface;
class Viewer3D;

/*!
 * Ejecuta los comandos de navegacion
 */
class NavigationTool : public GenericTool {
public:
   /** Constructor */
   explicit NavigationTool(DataViewManager* pDataViewManager);
   /** Destructor */
   virtual ~NavigationTool();
   /** Ejecuta un comando */
   virtual void Execute(const Command* pToolCommand);

protected:
   /** Crea un comando */
   virtual Command* CreateCommand(const std::string &Command);
   /** Destruye un comando */
   virtual void DestroyCommand(Command* &pCommand) const;
   /** Crea un widget */
   virtual Widget* CreateWidget(const std::string &Widget);

private:
   /** Metodo que ejecuta el comando de navegacion **/
   void ExecuteNavigation(bool CommandActive);
   void ExecuteNavigation(ViewerWidget* pViewer, bool CommandActive);
   void ExecuteNavigation3D(Viewer3D* pViewer, bool CommandActive);
   /** Ejecuta el comando LayerExtent **/
   void ExecuteLayerExtent();
   /** Metodo que ejecuta el comando Zoom1on1 **/
   void ExecuteZoom1on1();
   /** Metodo que ejecuta comando SaveView **/
   void ExecuteSaveView();
   EventInterface* pZoom_;                /*! Puntero al zoom */
   EventInterface* pDrag_;                /*! Puntero al drag */
   EventInterface* pCrossHair_;                /*! Puntero al evento crosshair */
   GoButton* pGoButton_;                    /*! Puntero al boton */
   ViewportLinkManager* pViewportLink_;   /*! Puntero al vp link */
   /** Widget que posee los sistemas de referencia **/
   CoordinatesNavigationInputInterface* pCoordinateInputWidget_;
};

} /* namespace suri */

#endif /* NAVIGATIONTOOL_H_ */
