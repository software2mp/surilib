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
