/*! \file LayerTablePart.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef LAYERLISTPART_H_
#define LAYERLISTPART_H_

// Includes standard
#include <string>

// Includes Suri
#include "suri/Table.h"
#include "suri/Part.h"
#include "wxVectorLayerGridTable.h"

// Includes Wx
#include "wx/wx.h"

// Includes App

// Defines
/** Flag para indicar que la tabla no es editable */
#define EDIT_NOTHING                   0x00000000
/** Flag para indicar que las celdas son editables */
#define EDIT_CELLS                     0x00000001
/** Flag para indicar que las filas son editables */
#define ADD_ROWS                       0x00000002 // No esta implementado
/** Flag para indicar que las columnas son editables */
#define ADD_COLUMNS                    0x00000004
/** Flag para indicar que se pueden eliminar filas */
#define REMOVE_ROWS                    0x00000008
/** Flag para indicar que se pueden eliminar columnas */
#define REMOVE_COLUMNS                 0x00000010 // No esta implementado
/** Flag para indicar que toda la tabla es editable */
#define EDIT_ALL              (EDIT_CELLS | ADD_ROWS | ADD_COLUMNS | \
                               REMOVE_ROWS | REMOVE_COLUMNS)
// Forwards
class wxGridEvent;
class wxGrid;

/** namespace suri */
namespace suri {
// Forwards
class VectorEditor;

/** Declara el objeto de eventos para los que heredan de CBaseTool */
DECLARE_EVENT_OBJECT(LayerTablePartEvent);

/** Parte para modificar campos en tabla de vector */
/**
 * Muestra en una tabla todos los campos del vector y permite agregar
 * columnas, eliminar features y modificar celdas.
 */
class LayerTablePart : public Part {
public:
   /** Declara el objeto de eventos como clase friend */
   FRIEND_EVENT_OBJECT(LayerTablePartEvent);

   /** Ctor */
   LayerTablePart(int Layer, std::string VectorUrl, long EditFlags = EDIT_NOTHING);
   /** Dtor */
   virtual ~LayerTablePart();

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
   /** Devuelve el icono de la parte */
   void GetWindowIcon(wxBitmap& ToolBitmap, int X, int Y) const;
   /** Informa si las tablas tienen datos correctos */
   virtual bool HasValidData();
   /** Recive set con nombre de campos que no debe mostrar */
   virtual bool SetFilter(const std::set<std::string> &FiltredFieldNames);
   /** Agrega columna a tabla preguntando nombre y tipo. */
   std::string AddColumn(std::string FieldType = "");
   /** Actualiza el estado de la parte */
   virtual void Update() {
   }
   /** Retorna el nombre de la capa */
   virtual std::string GetName();
// Modifica el ancho de las columnas para que entren en ventana.
   void FitColumnsToWindow();
protected:

   /** Retorna string con nombre del vector en memoria */
   std::string GetMemoryUrl();
   /** Elimina columna de tabla. */
   void RemoveRow();
   /** Configura los datos en el Grid para que sean identicos a los de disco. */
   bool ResetGridTable();
// Actualiza la pantalla y analiza si se puede escribir a disco
   bool ConfigureGrid();

// ----------------------EVENTS------------------
   /**
    *  Carga menu agregar/eliminar columna
    */
   void OnLabelRightDown(wxGridEvent &Event);

   /**
    * Se encarga de hacer un resize de la grilla
    * \see FitColumnsToWindow()
    */
   void OnResize(wxGridEvent &Event);

   /**
    *  Se ejecuta cuando se selecciona boton de context menu
    */
   void OnContextMenu(wxMenuEvent &Event);

   wxVectorLayerGridTable* pWxLayerGridTable_; /*! tabla que se carga en wxGrid */
   /* para leer los datos de disco. */
   int layer_; /*! vetor que se esta editando */
   VectorEditor* pMemoryVectorEditor_; /*! herramienta que se usa para editar */
   std::string fileName_; /*! Url del vector editado */
   long editFlags_; /*! determina que operaciones se pueden realizar sobre */
   /* la tabla */
   int selectedRow_; /*! Fila seleccionada para borrar */
   bool readOnly_; /*! Se setea a true al cargar la tabla si no puede escribir. */
   /* Limita todas las opciones de modificacion hasta el */
   /* siguiente LoadLayer. */
   Table* pTable_;
   EVENT_OBJECT_PROPERTY(LayerTablePartEvent); /*! Objeto que conecta los */
   /* eventos */
};
}  // namespace suri

#endif /* LAYERLISTPART_H_ */
