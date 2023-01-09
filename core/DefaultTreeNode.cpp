/** \file DefaultTreeNode.cpp */
/*!
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

// Includes standard
#include <string>
#include <list>
// Includes Suri
#include "DefaultTreeNode.h"
// Includes Wx
// Includes App
// Defines

namespace suri {

DefaultTreeNode::DefaultTreeNode(bool AllowsChildren) {
   allowsChildren_ = AllowsChildren;
   pParent_ = NULL;
}

DefaultTreeNode::~DefaultTreeNode() {
   std::list<TreeNodeInterface*>::iterator it = childNodes_.begin();
   for (; it != childNodes_.end(); ++it)
      delete *it;
   childNodes_.clear();
}

/**
 * Agrega un nodo en la posicion indicada
 * @param[in] pNewNode nodo que quiero agregar. El nodo
 * recibido pasa a ser responsabilidad de esta clase, no eliminar.
 * @param[in] Index Posicion donde quiero agregar el nodo.
 * @return bool que indica si pudo agregar el nodo.
 */
bool DefaultTreeNode::Insert(TreeNodeInterface* pNewNode, int Index) {
   if (Index < 0 || Index > GetSize() || !AllowsChildren())
      return false;

   std::list<TreeNodeInterface*>::iterator it = childNodes_.begin();
   for (int i = 0; i < Index && it != childNodes_.end() ; it++, i++);

   childNodes_.insert(it, pNewNode);
   pNewNode->SetParent(this);

   return true;
}

/**
 * Mueve el nodo desde el padre actual al nodo indicado
 * @param[in] pNewParent TreeNodeInterface donde quiero mover el nodo.
 * @param[in] Index Posicion donde quiero agregar el nodo.
 * @return bool que indica si pudo mover el nodo.
 */
bool DefaultTreeNode::Move(TreeNodeInterface* pNewParent, int Index) {
   if ( !GetParent() || !pNewParent || !pNewParent->AllowsChildren() )
      return false;

   GetParent()->Remove(this);
   pNewParent->Insert(this, Index);
   SetParent(pNewParent);

   return true;
}

/**
 * Saca un nodo hijo del arbol. El puntero pasa a ser responsabilidad
 * del invocante.
 * @param[in] pNode TreeNodeInterface que quiero remover
 * @return bool que indica si pudo remover el nodo
 */
bool DefaultTreeNode::Remove(TreeNodeInterface* pNode) {
   std::list<TreeNodeInterface*>::iterator it = childNodes_.begin();
   for (; it != childNodes_.end() && *it != pNode; it++);
   if (it == childNodes_.end())
      return false;

   childNodes_.erase(it);
   pNode->SetParent(NULL);

   return true;
}

/**
 * Elimina el nodo del grupo en la posicion indicada
 * @param[in] Index posicion del nodo a eliminar
 * @param[out] pNode TreeNodeInterface removido. El puntero
 * pasa a ser responsabilidad del invocante.
 * @return bool que indica si pudo remover el nodo
 */
bool DefaultTreeNode::Remove(int Index, TreeNodeInterface* &pNode) {
   if (Index < 0 || Index > GetSize())
      return false;

   std::list<TreeNodeInterface*>::iterator it = childNodes_.begin();
   for (int i = 0; i < Index && it != childNodes_.end() ; it++, i++);
   if (it == childNodes_.end())
      return false;

   pNode = *it;
   childNodes_.erase(it);
   pNode->SetParent(NULL);
   return true;
}

/**
 * Retorna el padre del nodo
 * @return TreeNodeInterface* nodo padre. El nodo retornado
 * es parte del arbol, no eliminar.
 * @return NULL si es nodo raiz.
 */
TreeNodeInterface* DefaultTreeNode::GetParent() {
   return pParent_;
}

/**
 * Configura el nodo padre
 * @param[in] pNewParent nuevo padre del nodo.
 */
void DefaultTreeNode::SetParent(TreeNodeInterface* pNewParent) {
   pParent_ = pNewParent;
}

/**
 * Retorna el nodo hijo en la posicion indicada
 * @param[in] Index posicion del nodo buscado
 * @return TreeNodeInterface nodo buscado. El nodo retornado
 * es responsabilidad de esta clase, no eliminar.
 */
TreeNodeInterface* DefaultTreeNode::GetChildAt(int Index) {
   std::list<TreeNodeInterface*>::iterator it = childNodes_.begin();
   for (int i = 0; i < Index && it != childNodes_.end() ; it++, i++);

   return (it != childNodes_.end()) ? *it : NULL;
}


/**
 * Indica si el nodo puede contener nodos hijo.
 * @return bool que indica si puede tener nodos hijo
 */
bool DefaultTreeNode::AllowsChildren() const {
   return allowsChildren_;
}

/**
 * Configura el id de SuriObject asociado al nodo.
 * @param[in] NewContent Nuevo id asociado al nodo
 * @return bool que informa si tuvo exito
 */
bool DefaultTreeNode::SetContent(SuriObject::UuidType NewContent) {
   if (allowsChildren_)
      return false;
   nodeContent_ = NewContent;
   return true;
}

/**
 * Retorna el id de SuriObject asociado al nodo.
 * @return SuriObject::UuidType id de SuriObject asociado al nodo.
 */
SuriObject::UuidType DefaultTreeNode::GetContent() const {
   return nodeContent_;
}

/**
 * Retorna la cantidad de subnodos que contiene el nodo.
 * @return int cantidad de subnodos
 */
int DefaultTreeNode::GetSize() const {
   return childNodes_.size();
}

/**
 * Retorna el nombre del nodo.
 * @return std::string con nombre del nodo
 */
std::string DefaultTreeNode::GetName() const {
   return name_;
}

/**
 * Configura el nombre del nodo.
 * @param[in] Name string con el nuevo nombre del nodo
 */
void DefaultTreeNode::SetName(const std::string &Name) {
   name_ = Name;
}
/** Retorna el id de SuriObject asociado al nodo. */
std::vector<SuriObject::UuidType> DefaultTreeNode::GetAssociatedIds() const {
   return associatedIds_;
}
/** Agrega un id de SuriObject asociado al nodo. */
void DefaultTreeNode::AddAssociatedId(const SuriObject::UuidType& Id) {
   associatedIds_.push_back(Id);
}
/** Devuelve el tipo de contexto */
ViewcontextInterface::ViewcontextType DefaultTreeNode::GetContextType() {
	return ctype_;
}
/** Setea el tipo de contexto */
void DefaultTreeNode::SetContextType( ViewcontextInterface::ViewcontextType ctype ) {
	ctype_ = ctype;
}

/** Devuelve la cantidad de subnodos que contiene el nodo,
 * pero filtrado por contextos */
int DefaultTreeNode::GetContextNodesSize(
		ViewcontextInterface::ViewcontextType ctype,
		std::map<int, std::vector<std::string> > elementsAndContexts, bool boutside) {
	std::list<TreeNodeInterface*>::const_iterator it = childNodes_.begin();
	int nodecount = 0;

	std::map<int, std::vector<std::string> >::iterator it2 = elementsAndContexts.find((int)ctype);

	if ( it2 != elementsAndContexts.end() ) {
		std::vector<std::string>::iterator itId = it2->second.begin();
		while(itId != it2->second.end()) {
			itId++;
			nodecount++;
		}
	}
	for (; it != childNodes_.end(); ++it)
		// Grupos, discriminar por contexto
		if ((*it)->AllowsChildren()) {
			if ((*it)->GetContextType() == ctype)
				if((*it)->GetSize() < 1)
					nodecount++;
		}
	return nodecount;
}
} /* namespace suri */
