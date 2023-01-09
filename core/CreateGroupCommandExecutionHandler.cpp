/*! \file CreateGroupCommandExecutionHandler.cpp */

/**                                                                                                 
 *  Biblioteca de procesamiento GeoEspacial SuriLib. 
 *  Copyright 2014-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L. 
 *                                                                                                  
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/) 
 *  
 */

// Includes Estandard
#include <string>
#include <vector>

// Includes Suri
#include "suri/CreateGroupCommandExecutionHandler.h"
#include "suri/Command.h"
#include "suri/GenericTool.h"
#include "VectorDatasource.h"
#include "suri/ToolSupport.h"
#include "suri/DataViewManager.h"
#include "suri/ViewcontextManagerInterface.h"
#include "suri/SuriObject.h"

namespace suri {

namespace core {

/** Constructor **/
CreateGroupCommandExecutionHandler::CreateGroupCommandExecutionHandler(
										DataViewManager* pDataView) : pDataView_(pDataView) {
}

/** Destructor **/
CreateGroupCommandExecutionHandler::~CreateGroupCommandExecutionHandler() {
}
 
/** Funcion auxiliar que obtiene el nombre del grupo a insertar 
 *  @param[out] GroupName nombre del grupo obtenido
 *  @return true en caso de poder obtener el nombre del grupo
 *  @return false en caso contrario
 */
bool GetGroupName(std::string& GroupName) {
  wxDialog dialog(NULL, wxID_ANY, _(caption_NEW_GROUP_INPUT), wxDefaultPosition,
                   wxDefaultSize, wxCAPTION | wxSYSTEM_MENU);
   dialog.SetSizer(new wxBoxSizer(wxVERTICAL));
   wxTextCtrl *ptxtctrl = new wxTextCtrl(&dialog, wxID_ANY,
                                         _(content_DEFAULT_GROUP_NAME),
                                         wxDefaultPosition, wxSize(250, -1));
   dialog.GetSizer()->Add(ptxtctrl, 0, wxEXPAND | wxALL, 5);
   dialog.GetSizer()->Add(dialog.CreateButtonSizer(wxOK | wxCANCEL), 0, wxEXPAND | wxALIGN_BOTTOM);
   dialog.Fit();

   // Valor por default
   wxString newgroupname = _(content_DEFAULT_GROUP_NAME);
   ptxtctrl->SetValidator(wxTextValidator(wxFILTER_NONE, &newgroupname));
   ptxtctrl->SelectAll();
   ptxtctrl->SetFocusFromKbd();

   // muestro el dialogo
   if (dialog.ShowModal() != wxID_OK)
      return false;

   GroupName = newgroupname;

   return true;
}

/**
 * Metodo que en funcion del Command que se pasa por parametro se encarga de administrar 
 * la ejecucion del mismo (en caso de que pueda manejar el tipo de comando).
 * @return true en caso de poder manejar la ejecucion del comando
 * @return false en caso contrario
 */
bool CreateGroupCommandExecutionHandler::Execute(const Command* pCommand, GenericTool* pTool) {
	if (pCommand->GetId() != pTool->GetId("CreateGroup"))
		return false;
   // creo un dialogo para ingresar un txt
   std::string newgroupname;
   if (GetGroupName(newgroupname)) {
		// Agrego grupo al final del arbol
	   WorkGroupInterface* prootworkgroup = pDataView_->GetGroupManager()->GetRootWorkGroup();
	   NodePath rootpath = prootworkgroup->GetRootPath();
	   pDataView_->GetGroupManager()->InsertGroup(newgroupname.c_str(), rootpath,
                                  prootworkgroup->GetRootSize());
   	delete prootworkgroup;
	}
return true;
}

}  /** namespace core **/
}  /** namespace suri **/
