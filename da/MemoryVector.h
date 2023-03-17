/* Copyright (c) 2006-2023 SpaceSUR and CONAE

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU Affero General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU Affero General Public License for more details.

You should have received a copy of the GNU Affero General Public License
along with this program.  If not, see <https://www.gnu.org/licenses/>.

This file is part of the SuriLib project and its derived programs. You must retain
this copyright notice and license text in all copies or substantial
portions of the software.

If you develop a program based on this software, you must provide a visible
notice to its users that it contains code from the SuriLib project and provide
a copy of this license. The notice should be displayed in a way that is easily
accessible to users, such as in the program's "About" box or documentation.

For more information about SpaceSUR, visit <https://www.spacesur.com>.
For more information about CONAE, visit <http://www.conae.gov.ar/>. */

#ifndef MEMORYVECTOR_H_
#define MEMORYVECTOR_H_

// Includes suri
#include "suri/Vector.h"

// Defines
/** Encabezado que lleva los urls de archivos en memoria */
#define MEMORY std::string("shpmemory:")
/** Colon */
#define COLON std::string(":")
/** Macro que analiza si es un url de archivo en memoria */
#define IS_MEMORY_VECTOR(Filename) (Filename.substr(0, MEMORY.length()) == MEMORY)
/** Macro que obtiene el nombre en disco a partir de uno en memoria */
#define GET_NAME(Filename) Filename.substr(MEMORY.size(),                   \
                                      Filename.find(COLON, MEMORY.size()) -  \
                                      MEMORY.size())
/** Obtiene referencia espacial a partir de nombre de archivo en memoria. */
#define GET_SPATIAL_REF(Filename) Filename.substr(MEMORY.size() +           \
                                      GET_NAME(Filename).size() +            \
                                      COLON.size() < Filename.size() ?       \
                                            MEMORY.size() +                  \
                                            GET_NAME(Filename).size() +      \
                                            COLON.size() : 0)

/** namespace suri */
namespace suri {
/** Clase que implementa un vector en memoria */
/**
 *  Esta clase implementa un vector OGR de memoria. Se utiliza para edicion en
 * vivo y que pueda ser renderizada.
 *
 */
class MemoryVector : public Vector {
   /** Ctor. de Copia. */
   MemoryVector(const MemoryVector &MemoryVector);

public:
   /** ctor */
   MemoryVector();
   /** dtor */
   virtual ~MemoryVector();
// ------------------------ METODOS ESTATICOS ------------------------
   /** Crea una instancia de la clase Vector */
   static Vector *Create() {
      return new MemoryVector;
   }
   /** Retorna el nombre de la clase */
   static std::string GetClassId() {
      return "MemoryVector";
   }
// ------------------- ESCRITURA A VECTOR EXTERNO -------------------
   /** Salva el vector en un formato dado con las opciones dadas */
   virtual bool Save(const std::string &Filename, const std::string &Format,
                     const OptionsMapType &WriteOptions = OptionsMapType());
protected:
   /** Inspecciona el archivo, para saber si es del formato que maneja */
   virtual bool Inspect(const std::string &Filename, VectorAccessType VectorAccess =
         ReadOnly,
                        int Layers = 0, VectorType Type = Undefined);
   /** Carga el vector */
   virtual Vector* Load(const std::string &Filename, VectorAccessType VectorAccess =
         ReadOnly,
                        int Layers = 0, VectorType Type = Undefined);
   /** Asigna el datasource asociado */
   virtual void SetDataSource(OGRDataSource *pVectorDataSource);
   /** Libera datasource */
   void ReleaseDataSource();
   /** Inicializacion en las clases derivadas, se llama al registrarlas */
   virtual void InitializeClass();
private:
   /** Mapa que relaciona nombre de vectores con sus datasources */
   typedef std::map<std::string, std::pair<int, OGRDataSource*> > DatasourceCollectionType;
   static DatasourceCollectionType loadedDataSources_; /*! lista de vectores en memoria */
   std::string memoryName_; /*! nombre en memoria del vector */
};
}

#endif /* MEMORYVECTOR_H_ */
