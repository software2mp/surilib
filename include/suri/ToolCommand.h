/*! \file ToolCommand.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef TOOLCOMMAND_H_
#define TOOLCOMMAND_H_

// Includes Estandar
#include <string>

// Includes Suri
#include "suri/Command.h"
#include "suri/ToolInterface.h"
#include "suri/SuriObject.h"

// Includes Wx
// Defines
// forwards

namespace suri {

/*!
 *
 */
class ToolCommand : public Command {
public:
   ToolCommand(ToolInterface *pTool, const CommandIdType &Id = SuriObject::NullIntUuid);

   virtual ~ToolCommand();

   virtual bool Execute();

   virtual CommandIdType GetId() const;
   virtual CommandExclusionGroupType GetGroupId() const;
   virtual CommandType GetType() const;
   /** Tooltip / Help */
   virtual const std::string GetDescription() const;
   /** bitmap asociado */
   virtual const std::string GetDecoration() const;
   /** ruta (xpath, ramas, etc) del menu donde aparece, hasta el texto del item. */
   virtual const std::string GetLocation() const;

   virtual void SetId(const CommandIdType &Id);
   virtual void SetGroupId(const CommandExclusionGroupType &GroupId);
   virtual void SetType(const CommandType &Type);
   virtual void SetDescription(const std::string &Description);
   virtual void SetDecoration(const std::string &Decoration);
   virtual void SetLocation(const std::string &Location);

   virtual bool IsActive() const;
   virtual void SetActive(bool Active = true);
   virtual bool IsEnabled() const;
   virtual void SetEnabled(bool Enable = true);
   /** Indica los parametros de entrada al command (ej. opciones para un command de tipo choice) */
   virtual void SetInputParameters(ParameterCollection* pParameters);
   /** Obtiene los parametros de entrada del command*/
   virtual ParameterCollection* GetInputParameters() const;
   /** Indica los parametros de salida al command (ej. seleccion de un choice)*/
   virtual void SetOutputParameters(ParameterCollection* pParameters);
   /** Obtiene los parametros de salida del command */
   virtual ParameterCollection* GetOutputParameters() const;

   /** Obtiene el nombre de la tarea asociada (si existe alguna) */
   virtual std::string GetAssociatedTaskId() const;

   /** Establece el nombre de la tarea asociada (si existe alguna) */
   virtual void SetAssociatedTaskId(std::string TaskId);

protected:
private:
   ToolInterface *pTool_;
   CommandIdType id_;
   CommandExclusionGroupType groupId_;
   CommandType type_;
   std::string decoration_;
   std::string description_;
   std::string location_;
   bool enable_, active_;
   /** Parametros del command */
   ParameterCollection* pInputParams_;
   ParameterCollection* pOutputParams_;
   std::string associatedTaskId_;
};

} /* namespace suri */

#endif /* TOOLCOMMAND_H_ */
