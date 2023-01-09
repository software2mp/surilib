/** \file IndexFile.h */
/*!
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef INDEXFILE_H_
#define INDEXFILE_H_

// Includes standard
#include <string>
#include <fstream>

namespace suri {

class IndexFile {
public:
   /** Ctor */
   explicit IndexFile(const std::string &FileName);
   /** Dtor */
   virtual ~IndexFile();

   /* Agrega un par clave->data al final del archivo. */
   bool AppendRegister(const std::string &Key, const std::string &Data);
   /* Busca la clave y retorna datos asociados */
   bool GetRegisterData(const std::string &Key, std::string &Data);
   /* Abre el archivo y se posiciona al comienzo */
   bool OpenIndex();
   /* Accede a la proximo registro y retorna los datos. Si es el ultimo registro retorna false. */
   bool GetNextRegister(std::string &Key, std::string &Data);
   /* Cierra el archivo  */
   void CloseIndex();

private:
   /* Nombre de archivo que contiene el indice */
   std::string fileName_;
   /* Puntero a lugar donde esta abierto el archivo */
   std::ifstream* pFileStream_;

   struct IndexRegister {
      IndexRegister(const std::string &Key = "", const std::string &Data = "") :
                                                   key_(Key), data_(Data) {}
      std::string key_;
      std::string data_;
   };

   /*  Item para la bilbioteca de acuerdo a los atributos indicados**/
   IndexRegister CreateRegisterFromLine(const std::string& Line) const;
   /** Metodo auxiliar crea una linea con todos los atributos del item**/
   std::string CreateLineFromRegister(const IndexRegister &Register) const;

   /** Caracter que separa la clave de los datos en archivo */
   static const char REGISTER_SEPARATOR = ';';
};

} /* namespace suri */

#endif /* INDEXFILE_H_ */
