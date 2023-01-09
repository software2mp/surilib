/** \file GuiTreeModel.cpp */
/*!
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

// Includes standard
#include <set>

// Includes suri
#include "suri/GuiTreeModel.h"

namespace suri {

/** Ctor */
GuiTreeModel::GuiTreeModel() {
}

/** Dtor */
GuiTreeModel::~GuiTreeModel() {
}

/**
 * Expande/contrae un grupo en interfaz grafica
 * @param[in] Group path del grupo
 * @param[in] Expand indica si el grupo debe estar expandido(true)
 * o contraido(false).
 */
void GuiTreeModel::ExpandGroup(const NodePath &Group, bool Expand) {
   RefreshModel();
   SuriObject::UuidType groupid = Group.GetLastPathNode()->GetId();
   std::set<SuriObject::UuidType>::iterator it = expandedGroups_.find(groupid);

   if (Expand && it == expandedGroups_.end())
      expandedGroups_.insert(groupid);

   if (!Expand && it != expandedGroups_.end())
      expandedGroups_.erase(it);
}

/**
 * Informa si un grupo esta expandido en la interfaz grafica
 * @param[in] Group path del grupo
 * @return bool que indica si el grupo esta expandido(true) o contraido(false)
 */
bool GuiTreeModel::IsExpanded(const NodePath &Group) {
   RefreshModel();
   SuriObject::UuidType groupid = Group.GetLastPathNode()->GetId();
   return (expandedGroups_.find(groupid) != expandedGroups_.end());
}

/**
 * Indica si un nodo es visible en interfaz. Para estar visible todos los
 * grupos que lo contienen deben estar expandidos
 * @param[in] Node path a un item/grupo
 * @return bool que indica si el nodo esta visible
 */
bool GuiTreeModel::IsVisible(const NodePath &Node) {
   int nodeparentcount = Node.GetPathCount() - 1;
   int i = 1;  // Ignoro al root.
   for (; i < nodeparentcount && IsExpanded(Node.GetPathToParentNode(i)); i++);

   return !(i < nodeparentcount);
}

/**
 * Habilita/Deshabilita un nodo
 * @param[in] Node path del nodo
 * @param[in] Enabled indica si el nodo debe estar habilitado(true)
 * o no(false).
 */
void GuiTreeModel::Enable(const NodePath &Node, bool Enabled) {
   RefreshModel();
   SuriObject::UuidType nodeid = Node.GetLastPathNode()->GetId();
   std::set<SuriObject::UuidType>::iterator it = disabledGroups_.find(nodeid);

   if (!Enabled && it == disabledGroups_.end())
      disabledGroups_.insert(nodeid);

   if (Enabled && it != disabledGroups_.end())
      disabledGroups_.erase(it);
}

/**
 * Indica si un nodo esta habilitado
 * @param[in] Node path a un item/grupo
 * @return bool que indica si el nodo esta habilitado
 */
bool GuiTreeModel::IsEnabled(const NodePath &Node) {
   RefreshModel();
   SuriObject::UuidType groupid = Node.GetLastPathNode()->GetId();
   return (disabledGroups_.find(groupid) == disabledGroups_.end());
}


/** Actualiza las variables internas con los cambios realizados en el modelo */
void GuiTreeModel::RefreshModel() {
   RemoveInvalidItems(expandedGroups_);
   RemoveInvalidItems(disabledGroups_);
}

/**
 * Limpia los id invalidos del conjunto
 * @param[in] WorkingSet conjunto a analizar.
 */
void GuiTreeModel::RemoveInvalidItems(std::set<SuriObject::UuidType> &WorkingSet) {
   WorkGroupInterface* pworkgroup = GetRootWorkGroup();
   std::set<SuriObject::UuidType>::iterator it = WorkingSet.begin();
   while (it != WorkingSet.end()) {
      if (!pworkgroup->SearchNode(*it).IsValid())
         WorkingSet.erase(it++);
      else
         ++it;
   }
   delete pworkgroup;
}

} /* namespace suri */
