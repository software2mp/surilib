/*! \file GuiImpl.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2012-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef GUIIMPL_H_
#define GUIIMPL_H_

// Includes Estandar
#include <string>
#include <map>

// Includes Suri
#include "suri/GuiInterface.h"
#include "suri/Command.h"
#include "suri/ToolGroupManager.h"
// Includes Wx

// Defines

// forwards
class wxMenuItem;

namespace suri {

// forwards
class Widget;
class GuiStateChangeMediator;

/*!
 * Clase encargada de la administracion
 * de herramientas y menues para la aplicacion haciendo uso de los ToolGroupManagers
 * asociados.
 */
class GuiImpl : public GuiInterface {
public:
   explicit GuiImpl(Widget *pGuiWidget);
   virtual ~GuiImpl();
   /** Agrega la herramienta en la ubicacion de los Commands asociados**/
   virtual bool AddTool(ToolInterface* pTool);
   /** Elimina la herramienta de las ubicaciones del os Commandas asociados **/
   virtual bool RemoveTool(ToolInterface* pTool);
   virtual void AddToolbar(Widget *pWidget);
   /** Elimina el Toolbar en la ubicacion indicada por parametro**/
   virtual void RemoveToolbar(const std::string& LocationId);
   virtual void InitializeToolbar(wxWindow *pToolBar, const std::string& LocationId);
   static const std::string CallerLocationToken;
   static const std::string PositionLocationToken;
   virtual void EnableTopMenu(bool Enable, int Position,
                              const std::vector<std::string>& ValidNames);
   /** Configura el GuiChangeMediator. */
   void SetGuiStateChangeMediator(GuiStateChangeMediator* pGuiStateMediator);
   void DisableMenuNoElement();
   void EnableMenu(bool Enable);
   void EnableMenuRemoteLayer(bool Enable);
protected:
   /** Metodo que agrega un menu en la ubicacion descripta por el command**/
   virtual void AddMenu(Command *pCommand, const std::string& MenuLocation);
   /** Elimina un menu **/
   virtual void RemoveMenu(const std::string& MenuLocation);
   /** Agrega widget **/
   virtual void AddWidget(Widget *pWidget);
   /** Elimina widget **/
   virtual void RemoveWidget();
   /** Obtiene el Id del item de menu en Location */
   int GetLocationId(const std::string &Location);
   /** Crea un item de menu a partir del Command */
   wxMenuItem *CreateMenuFromCommand(Command *pCommand, const std::string& MenuLocation);
private:
   /** constantes para la definicion de las posiciones
    * para la descripcion de la ubicacion**/
   static const int LocationPosition;
   static const int LocationDescriptionPosition;

   /** Metodo auxilar que dada una ubicacion agrega el command a todos los
    *  toolgroupmanagerasociados
    */
   void ToolGroupToolAddition(Command *pCommand, const std::string& CommandType,
                              const std::string& Location);
   /** Metodo auxiliar que maneja de una herramienta en funcion al command asociado**/
   void ManageToolAddition(Command *pCommand, ToolGroupManager* pTgm,
                           const std::string& CommandType, const std::string& Location,
                           int Position = suri::ToolGroupManager::Last);
   /** Mapa con relacion ID, toolgroupManager **/
   typedef std::map<std::string, ToolGroupManager*> ToolGroupMapType;
   ToolGroupMapType toolGroupMap_;
   typedef std::multimap<std::string, ToolGroupManager*> CallerToolGroupMapType;
   CallerToolGroupMapType callerToolGroupMultiMap_;
   Widget *pGuiWidget_;
   /** Permite modificar el estado de la gui */
   GuiStateChangeMediator* pGuiStateMediator_;
   std::map<std::string, wxMenu*> menu_;
   bool isMenuEnable_;
   bool isRemoteLayer_;
   std::vector<std::string> validtools_;
};

} /* namespace suri */

#endif /* GUIIMPL_H_ */
