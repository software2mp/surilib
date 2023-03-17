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

// Includes standard
#include <string>
#include <fstream>

// I  ncludes suri
#include "IndexFile.h"

namespace suri {

IndexFile::IndexFile(const std::string &FileName) {
   pFileStream_ = NULL;
   fileName_ = FileName;
}

IndexFile::~IndexFile() {
}

/* Agrega un par clave->data al final del archivo. */
bool IndexFile::AppendRegister(const std::string &Key, const std::string &Data) {
   if (pFileStream_)
      return false;

   bool added = false;
   std::ofstream file(fileName_.c_str(), std::ofstream::app);
   if (file.is_open()) {
      std::string itemline = CreateLineFromRegister(IndexRegister(Key, Data));
      file << itemline << std::endl;
      added = true;
      file.close();
   }
   return added;
}

/* Busca la clave y retorna datos asociados */
bool IndexFile::GetRegisterData(const std::string &Key, std::string &Data) {
   IndexRegister idxreg;
   std::string line;
   std::ifstream file(fileName_.c_str());
   while (idxreg.key_ != Key && file.good()) {
      getline(file, line);
      idxreg = CreateRegisterFromLine(line);
   }
   if (idxreg.key_ == Key)
      Data = idxreg.data_;
   return (idxreg.key_ == Key);
}

/* Abre el archivo y se posiciona al comienzo */
bool IndexFile::OpenIndex() {
   CloseIndex();
   pFileStream_ = new std::ifstream(fileName_.c_str());
   return true;
}

/*
 * Accede a la proximo registro y retorna los datos.
 * Si es el ultimo registro retorna false.
 */
bool IndexFile::GetNextRegister(std::string &Key, std::string &Data) {
   if (!pFileStream_ || !pFileStream_->is_open())
      return false;

   pFileStream_->peek();
   if (!pFileStream_->good())
      return false;

   std::string line;
   std::getline(*pFileStream_, line);
   IndexRegister idxreg = CreateRegisterFromLine(line);
   Key = idxreg.key_;
   Data = idxreg.data_;
   return true;
}

/* Cierra el archivo  */
void IndexFile::CloseIndex() {
   if (!pFileStream_)
      return;
   pFileStream_->close();
   delete pFileStream_;
   pFileStream_ = NULL;
}

/*  Item para la bilbioteca de acuerdo a los atributos indicados**/
IndexFile::IndexRegister IndexFile::CreateRegisterFromLine(
                                                const std::string& Line) const {
   IndexRegister idxreg;
   size_t found = Line.find(REGISTER_SEPARATOR);
   if (found != std::string::npos) {
      idxreg.data_ = Line.substr(found + 1);
      idxreg.key_ = Line.substr(0, found);
   }
   return idxreg;
}

/** Metodo auxiliar crea una linea con todos los atributos del item**/
std::string IndexFile::CreateLineFromRegister(
                              const IndexFile::IndexRegister &Register) const {
   return Register.key_ + REGISTER_SEPARATOR + Register.data_;
}


} /* namespace suri */
