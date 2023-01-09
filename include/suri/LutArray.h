/*! \file LutArray.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */
#ifndef LUTARRAY_H_
#define LUTARRAY_H_
// Includes estandar
#include <vector>

// Includes Suri
#include "suri/LookUpTable.h"

// Includes Wx
#include "wx/xml/xml.h"

// Includes App

// Defines

/** namespace suri */
namespace suri {
/** Clase que almacena las tablas de busqueda para una imagen */
/**
 *  Representa un array de LookUpTables.
 *
 *  Las tablas se organizan en funcion del indice de la banda de salida, cada
 * una tiene el indice de la banda de entrada asociada.
 *
 *  Define el estado de activacion de la tabla. \todo Pasar a los renderizadores
 */
class LutArray {
public:
   /** ctor */
   LutArray();
   /** dtor */
   virtual ~LutArray();

// -------------- METODOS DE SERIALIZACION DE UN ARRAY DE TABLAS ---------------
   /** Inicializa un array en funcion de un Nodo xml */
   static bool FromXml(const wxXmlNode *pLutArrayNode, LutArray &Parameters);
   /** Convierte un array a un Nodo xml */
   static bool ToXml(const LutArray &Parameters, wxXmlNode* &pLutArrayNode);
   /** Convierte el array a un Nodo xml */
   bool ToXml(wxXmlNode* &pLutArrayNode) const;
   /** Obtiene la cantidad de tablas */
   int GetCount() const;
   /** Retorna la tabla y banda de entrada para una banda de salida dada */
   LookUpTable GetLookUpTable(int OutputBand, int &InputBand) const;
   /** Agrega una tabla con su banda de entrada */
   void AddLookUpTable(const LookUpTable &Lut, const int InputBand);
   /** Retorna el nombre de la lut */
   std::string GetName() const;
   /** Modifica el nombre de la lut */
   void SetName(const std::string &LutName);

   bool active_; /*! Propiedad que habilita la tabla */
private:
   std::string lutName_; /*! contenido de la propiedad nombre en el nodo lut */
   /** Tipo que almacena la banda de salida y la tabla */
   typedef std::pair<int, LookUpTable> LookUpTablePair; /*! todo:documentar */
   /** Tipo que almacena el array */
   typedef std::vector<LookUpTablePair> LookUpTableArray;
// La posicion del vector es la salida, first es la entrada y LookUpTable la tabla
   LookUpTableArray lutArray_; /*! Array de LookUpTable */
};
}

#endif /* LUTARRAY_H_ */
