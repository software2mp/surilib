/** \file DefaultTreeNode.h */
/*!
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef SURIOBJECTTREENODE_H_
#define SURIOBJECTTREENODE_H_

// Includes standard
#include <string>
#include <list>
// Includes Suri
#include "suri/TreeNodeInterface.h"
#include "suri/ViewcontextInterface.h"
// Includes Wx
// Includes App
// Defines

namespace suri {

/**
 * Implementa TreeNodeInterface.
 */
class DefaultTreeNode : public TreeNodeInterface {
public:

	typedef enum {
		ContextGenericType = 0, Context2DType, Context3DType, ContextPhotosType, ContextHtml,
		ContextMapType, ContextDocumentsType, ContextActivitiesType, ContextErrorType
    } ContextType;

	/** Ctor */
   explicit DefaultTreeNode(bool AllowsChildren);
   /** Dtor */
   virtual ~DefaultTreeNode();

   //  --------  METODOS DE TREENODEINTERFACE --------
   /** Agrega un nodo en la posicion indicada */
   virtual bool Insert(TreeNodeInterface* pNewNode, int Index);
   /** Mueve el nodo desde el padre actual al nodo indicado */
   virtual bool Move(TreeNodeInterface* pNewParent, int Index);
   /** Saca un nodo hijo del arbol. */
   virtual bool Remove(TreeNodeInterface* pNode);
   /** Elimina el nodo del grupo en la posicion indicada */
   virtual bool Remove(int Index, TreeNodeInterface* &pNode);
   /** Retorna el padre del nodo */
   virtual TreeNodeInterface* GetParent();
   /** Configura el nodo padre */
   virtual void SetParent(TreeNodeInterface* pNewParent);
   /** Retorna el nodo hijo en la posicion indicada */
   virtual TreeNodeInterface* GetChildAt(int Index);
   /** Indica si el nodo puede contener nodos hijo. */
   virtual bool AllowsChildren() const;
   /** Configura el id de SuriObject asociado al nodo. */
   virtual bool SetContent(SuriObject::UuidType NewContent);
   /** Retorna el id de SuriObject asociado al nodo. */
   virtual SuriObject::UuidType GetContent() const;
   /** Retorna la cantidad de subnodos que contiene el nodo. */
   virtual int GetSize() const;
   /** Retorna el nombre del nodo. */
   virtual std::string GetName() const;
   /** Configura el nombre del nodo. */
   virtual void SetName(const std::string &Name);
   /** Retorna el id de SuriObject asociado al nodo. */
   virtual std::vector<SuriObject::UuidType> GetAssociatedIds() const;
   /** Retorna el id de SuriObject asociado al nodo. */
   virtual void AddAssociatedId(const SuriObject::UuidType& Id);
   /** Devuelve el tipo de contexto */
   virtual ViewcontextInterface::ViewcontextType GetContextType();

   /** Setea el tipo de contexto */
   virtual void SetContextType(ViewcontextInterface::ViewcontextType ctype);

   /** Devuelve la cantidad de subnodos que contiene el nodo,
    * pero filtrado por contextos */
   virtual int GetContextNodesSize(ViewcontextInterface::ViewcontextType ctype,
			std::map<int, std::vector<std::string> > elementsAndContexts, bool boutside);

private:
   /** Listado con los hijos de este nodo */
   std::list<TreeNodeInterface*> childNodes_;
   /** Nodo padre. Si es NULL es el nodo raiz. */
   TreeNodeInterface* pParent_;
   /** Flag que indica si el nodo permite hijos */
   bool allowsChildren_;

   /**
    * Contenido del nodo. Id del SuriObject al que hace referencia. Solo es
    * valido si es nodo hoja.
    */
   SuriObject::UuidType nodeContent_;

   /**
    * Contenido del nodo. Id del SuriObject al que hace referencia. Solo es
    * valido si es nodo hoja.
    */
   std::vector<SuriObject::UuidType> associatedIds_;

   ViewcontextInterface::ViewcontextType ctype_;

   /** Nombre asignado al nodo. */
   std::string name_;
};

} /* namespace suri */

#endif /* SURIOBJECTTREENODE_H_ */
