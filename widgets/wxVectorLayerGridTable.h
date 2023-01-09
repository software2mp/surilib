/*! \file wxVectorLayerGridTable.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */
#ifndef wxVectorLayerGridTable_H_
#define wxVectorLayerGridTable_H_

// Includes estandar
#include <set>
#include <vector>

// Includes Suri
#include "suri/VectorEditor.h"

// Includes Wx
#include "wx/grid.h"

// Includes App

// Defines

/** namespace suri */
namespace suri {
/**
 * Se usa para llenar los campos de un wxGrid y para guardar las modificaciones
 * echas sobre la tabla. Los datos los lee/guarda de un VectorEditor que recive
 * por parametro en constructor.
 * Como wxGrid solo le pide los datos que muestra en ese momento es mas
 * eficiente que leer toda la tabla de disco y cargarlo en un wxGrid con tabla
 * default.
 * Tiene un filtro para no mostrar determinados campos.
 * Tiene un flag que indica si se hizo alguna modificacion a la tabla. Este flag
 * no reconoce cuando se hace un cambio y luego se deshace.
 */
class wxVectorLayerGridTable : public wxGridTableBase {
public:
   /** Ctor */
   wxVectorLayerGridTable(VectorEditor* pVectorEditor);
   /** Dtor */
   virtual ~wxVectorLayerGridTable();
   /** Retrona el valor la celda(en formato string) en posicion pedida. */
   virtual wxString GetValue(int Row, int Column);
   /** Actualiza el valor de un campo en vector con el valor que se pasa por parametro. */
   virtual void SetValue(int Row, int Column, const wxString& Value);
   /** Informa el numero de filas que tiene la tabla. */
   virtual int GetNumberRows();
   /** Informa el numero de columnas que tiene la tabla. */
   virtual int GetNumberCols();
   /** Informa si la celda esta vacia */
   virtual bool IsEmptyCell(int Row, int Column);
   /** Retorna el nombre de una columna */
   virtual wxString GetColLabelValue(int Column);
   /** Determina si la tabla del vector esta modificada. */
   void SetModified(bool Modified);
   /** Informa si la tabla del vector esta modificada. */
   bool IsModified();
   /** Determina si la tabla del vector se puede modificar. */
   void SetReadOnly(bool ReadOnly);
   /** Informa si la tabla del vector esta en modo readonly. */
   bool IsReadOnly();
   /** Configura los campos que se deben mostrar */
   bool SetFilter(const std::set<std::string> &FiltredFieldNames);
   /** Usa filteredFieldNames_ y pVectorEditor_ para cargar displayedFields_. */
   bool LoadDisplayedFields();
   /** Obtiene el valor del campo en el vector en VectorEditor. */
   bool GetFieldValue(int Row, int Column, std::string &Value);
   /** Modifica el valor del campo. Puede en el futuro usar un cache. */
   bool SetFieldValue(int Row, int Column, const std::string &Value);
   /** Retorna una copia del vector con campos desplegados */
   std::vector<std::pair<std::string, std::string> > GetDisplayedFields();
   /** Actualiza wxGrid para que coincida con los datos en el vector que se despliega. */
   void RefreshGrid();

private:
   /** Actualiza el tamano de la grilla en pantalla. */
   bool UpdateGridSize();

   bool readOnly_; /*! Determina si la tabla debe estar en modo solo escritura */
   bool modified_; /*! Informa si se modifico el elemento */
   VectorEditor* pVectorEditor_; /*! Se usa para cargar/guardar datos de tabla */
   std::set<std::string> filteredFieldNames_; /*! Filtro seleccionado por usuario */
   std::vector<std::pair<std::string, std::string> > displayedFields_; /*! nombre */
   /* y tipo de los campos desplegados. La posicion de cada par dentro del */
   /* vector es el indice de la columna de la tabla. */
};
}

#endif /* wxVectorLayerGridTable_H_ */
