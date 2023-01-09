/*! \file HotlinkFieldEditorPart.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef HOTLINKFIELDEDITORPART_H_
#define HOTLINKFIELDEDITORPART_H_

// Includes standard
#include <vector>
#include <string>
// Includes Suri
#include "suri/Part.h"
// Includes Wx
// Includes App
// Defines
// Si se define __MARK__INVALID__HOTLINKS__ los hotlinks invalidos se muestran en rojo
#undef __MARK__INVALID__HOTLINKS__

// Forwards



class wxCommandEvent;
class wxGridEvent;
class wxKeyEvent;

/** namespace suri */
namespace suri {
// Forwards
class VectorEditor;
class VectorDatasource;
class VectorElement;
class LayerTablePart;
/** Declara el objeto de eventos para los que heredan de CBaseTool */
DECLARE_EVENT_OBJECT(HotlinkFieldEditorPartEvent);

/** Clase (Part) para la edicion de los campos hotlink de un vector */
/**
 * Permite seleccionar el campo que contiene los hotlinks y editar su
 * contenido. Para editar la tabla usa un LayerTablePart
 */
class HotlinkFieldEditorPart : public Part {
public:
   /** Declara el objeto de eventos como clase friend */
   FRIEND_EVENT_OBJECT(HotlinkFieldEditorPartEvent);

   /** Ctor */
   HotlinkFieldEditorPart(int Layer, VectorDatasource* pVectorDatasource);
   /** Ctor */
   HotlinkFieldEditorPart(int Layer, VectorElement* pVectorElement);
   /** Dtor */
   virtual ~HotlinkFieldEditorPart();

   /** Crea la ventana de la parte */
   virtual bool CreateToolWindow();
   /** Devuelve el estado del Part. */
   virtual bool IsEnabled();
   /** Indica si el Part tiene cambios para salvar. */
   virtual bool HasChanged();
   /** Salva los cambios realizados en el Part. */
   virtual bool CommitChanges();
   /** Restaura los valores originales del Part. */
   virtual bool RollbackChanges();
   /** Inicializa los datos mostrados */
   virtual void SetInitialValues();
   /** Informa si las tablas tienen datos correctos */
   virtual bool HasValidData();
   /** Actualiza el estado de la parte */
   virtual void Update() {
   }
   /** Retorna el nombre de la capa */
   virtual std::string GetName();
protected:
   /** Carga un filtro en la tabla que solo muestra el campo de nombre indicado */
   bool SetLayerTablePartFilter(const std::string &FieldName, bool Reload = false);
   /** Carga los nombres de campos que son strings en wxChoice y llama a SetLayerTablePartFilter */
   bool LoadFieldList();
   /** Llama a SetLayerTablePartFilter con nombre de obtenido del nodo */
   bool LoadHotlinkFromXml();
   /** Retorna un string con el nombre del campo seleccionado en choice */
   std::string GetSelectedHotlink();

   /** Carga la tabla del campo seleccionado */
   void OnChoiceChanged(wxCommandEvent &Event);
   /** Activa la ventana para seleccionar archivo */
   void OnCellDoubleClick(wxGridEvent &Event);

#ifdef __MARK__INVALID__HOTLINKS__
   /** Controla si el hotlink es valido */
   void OnCellChange(wxGridEvent &Event);
   /** Valida si un URL es correcto o no */
   bool IsUrlValid(const std::string &Url);
   /** Valida si los URLs son correctos o no */
   void ValidateTable();
#endif
   /** Retorna el VectorEditor que se usa para acceder a vector */
   VectorEditor* GetVectorEditor();
   /** Controla si se presiono delete */
   void OnKeyDown(wxKeyEvent &Event);

   int layer_; /*! vetor que se esta editando */
   VectorEditor* pVectorEditor_; /*! herramienta que se usa para editar */
   VectorDatasource* pVectorDatasource_;
   VectorElement* pVectorElement_; /*! Url del vector editado */
   LayerTablePart* pLayerPart_; /*! Part que muestra la tabla asociada a la capa */
   size_t selectedItem_; /*! posicion seleccionada en el choice */
   std::vector<std::string> posibleHotlinkFields_; /*! lista con los nombres de */
   /* los campos que pueden */
   /* ser hotlinks */
   std::string newColumnName_; /*! Nombre de la columna creada al */
   /* seleccionar nuevo campo */
   bool readOnly_; /*! Se setea al cargar la tabla. Determina si se muestras */
   /* opciones de modificacion */
   EVENT_OBJECT_PROPERTY(HotlinkFieldEditorPartEvent); /*! Objeto para conectar */
   /* eventos */
};
}

#endif /* HOTLINKFIELDEDITORPART_H_ */
