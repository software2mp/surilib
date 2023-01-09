/*! \file HtmlTreeNode.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef HTMLCONFIGURATIONDATA_H_
#define HTMLCONFIGURATIONDATA_H_

// Includes standard
#include <string>

// Includes Suri
#include "suri/Command.h"

// Includes Wx

// Includes App

// Defines

namespace suri {
/**
 * Interfaz que brinda wxSimpleHtmlTreeCtrl para configurar html de un item
 */
class HtmlConfigurationData {
public:
   enum State {
      ON, OFF, BOTH
   };

   /** Ctor */
   HtmlConfigurationData(std::string Name, bool Leaf = true);
   /** Dtor */
   virtual ~HtmlConfigurationData();
   /** Retorna nombre de nodo */
   std::string GetName() const;
   /** Modifica nombre del nodo */
   void SetName(std::string Name);
   /** Retorna el nombre del icono */
   std::string GetIcon() const;
   /** Modifica el nombre del icono */
   void SetIcon(std::string Icon);
   /** Indica si es nodo hoja */
   bool IsLeaf() const;
   /** Modifica indicador de si es hoja */
   void Leaf(bool IsLeaf);
   /** Retorna estado del item. */
   State GetState() const;
   /** Configura estado del item. */
   void SetState(State State);
   /** Retorna command. */
   Command* GetExecuteOnState() const;
   /** Configura command. */
   void SetExecuteOnState(Command* pExecuteOnState);

protected:
   std::string name_; /*! Nombre del nodo */
   std::string icon_; /*! Path del icono asociado al nodo */
   State state_; /*! Indica estado del check. */
   bool isLeaf_; /*! Indica si el nodo es tipo hoja. */
   Command* pExecuteOnState_; /*! Se ejecuta cuando el usuario modifica estado de activacion desde */
   // interfaz.
};
}  // namespace

#endif /* HTMLCONFIGURATIONDATA_H_ */
