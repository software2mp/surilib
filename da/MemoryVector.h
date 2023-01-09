/*! \file MemoryVector.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

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
