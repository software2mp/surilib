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

// Includes Estandar
#include <string>
#include <vector>
#include <fstream>

// Includes Suri
#include "suri/messages.h"
#include "suri/AuxiliaryFunctions.h"
#include "CsvVectorCreator.h"

// Includes Wx
#include "wx/filename.h"
#include "wx/file.h"

namespace suri {

/** Constructor */
CsvVectorCreator::CsvVectorCreator(std::string Path, int Xcolumn, int Ycolumn, int StartFromRow,
                                   std::string SpatialReference, std::string Token,
                                   bool IsPersonalized, bool HasHeader) :
      path_(Path), xcolumn_(Xcolumn), ycolumn_(Ycolumn), startfromrow_(StartFromRow),
      spatialreference_(SpatialReference), token_(Token), ispersonalized_(IsPersonalized),
      hasheader_(HasHeader) {
}

/** Destructor */
CsvVectorCreator::~CsvVectorCreator() {
}

/**
 * Agrega la fuente de datos utilizando el driver vrt de gdal
 * @param[in] Path: ruta del archivo seleccionado, retorna NULL
 * si hay columna repetida
 */
DatasourceInterface* CsvVectorCreator::CreateVrt(Option Metadata) {
   // Creo el archivo temporal
   wxFile vrtfile;
   wxFileName source(path_);
   wxString vrt = source.GetPath()+ wxFileName::GetPathSeparator() + source.GetName() + wxT(".vrt");
   wxString csv = source.GetPath()+ wxFileName::GetPathSeparator() + source.GetName() + wxT(".csv");
   wxFileName vrtfilename(vrt);
   wxFileName csvfilename(csv);

   // Abro el archivo en modo escritura
   if (!vrtfile.Open(vrtfilename.GetFullPath(), wxFile::write)) {
      SHOW_ERROR(message_OPEN_FILE_ERROR);
      return NULL;
   }

   // Escribo en el archivo el contenido del vrt
   std::string content = "<OGRVRTDataSource><OGRVRTLayer name=\"";
   content += vrtfilename.GetName().c_str();
   content += "\">";
   content += "<SrcDataSource>";
   content += csvfilename.GetFullPath().c_str();
   content += "</SrcDataSource>";
   content += "<GeometryType>wkbPoint</GeometryType>";
   content += "<LayerSRS>" + spatialreference_ + "</LayerSRS>";
   content += "<GeometryField encoding=\"PointFromColumns\"";

   std::string line;
   std::ifstream csvfile;
   csvfile.open(path_.c_str(), std::ifstream::in);

   // Obtengo los headers
   std::vector<std::string> headers;
   getline(csvfile, line);
   headers = tokenizer(line, token_);
   //Verifico q los headers sean distintos
   std::vector<std::string>::const_iterator it=headers.begin();
   for (;it!=headers.end();++it){
      std::vector<std::string>::const_iterator it2=headers.begin();
      for (;it2!=headers.end();++it2){
         if((*it)==(*it2)&&(it!=it2)) return NULL;
      }
   }
   // Obtengo la primer linea con valores
   std::vector<std::string> values;
   getline(csvfile, line);
   values = tokenizer(line, token_);

   // Verifico que el token sea correcto
   if (values.size() < 2) {
      SHOW_ERROR(message_TOKEN_ERROR);
      return NULL;
   }

   /**
    * Si el archivo tiene header debe bindear el nombre de la columna, si no utiliza los campos
    * genericos field_x. Estos no pueden utilizarse en caso de que el archivo posea header.
    */
   if (hasheader_) {
      content += " x=\"" + headers.at(xcolumn_) + "\" y=\"" + headers.at(ycolumn_) + "\"/>";
      for (size_t i = 0; i < values.size(); ++i)
         if (values.at(i).find(".") != std::string::npos
               && values.at(i).find(",") != std::string::npos
               && IsNumber(values.at(i))) {
            SHOW_ERROR(message_COLUMN_VALUE_ERROR, i+1);
            return NULL;
         } else if (values.at(i).find(".") != std::string::npos
               && IsNumber(values.at(i))) {
            content += "<Field name=\"" + headers.at(i)
                  + "\" type=\"Real\" width=\"20\" precision=\"10\" />";
         } else if (values.at(i).find("/") == std::string::npos
               && IsNumber(values.at(i))) {
            content += "<Field name=\"" + headers.at(i) + "\" type=\"Integer\" />";
         } else {
            content += "<Field name=\"" + headers.at(i) + "\" type=\"String\" />";
         }
   } else {
      content += " x=\"field_" + IntToString(xcolumn_ + 1) + "\" y=\"field_"
            + IntToString(ycolumn_ + 1) + "\"/>";
      for (size_t i = 0; i < values.size(); ++i)
         if (values.at(i).find(".") != std::string::npos
               && values.at(i).find(",") != std::string::npos
               && StringToNumber<int>(values.at(i))) {
            SHOW_ERROR(message_COLUMN_VALUE_ERROR, i+1);
            return NULL;
         } else if (values.at(i).find(".") != std::string::npos
               && StringToNumber<double>(values.at(i))) {
            content += "<Field name=\"field_" + IntToString(i + 1)
                  + "\" type=\"Real\" width=\"20\" precision=\"10\" />";
         } else if (values.at(i).find("/") == std::string::npos
               && StringToNumber<int>(values.at(i))) {
            content += "<Field name=\"field_" + IntToString(i + 1) + "\" type=\"Integer\" />";
         } else {
            content += "<Field name=\"field_" + IntToString(i + 1) + "\" type=\"String\" />";
         }
   }

   content += "</OGRVRTLayer></OGRVRTDataSource>";
   vrtfile.Write(content);

   DatasourceInterface* pds = DatasourceInterface::Create(
         "VectorDatasource", vrtfilename.GetFullPath().c_str(), Metadata);
   return pds;
}

/**
 * Metodo auxiliar que obtiene la informacion de metainformacion que configura el usuario
 * @Param[in] Metadata objeto Option donde se debe configurar clave - valor de metadata
 */
void CsvVectorCreator::GetMetadataOptions(Option& Metadata) {
   Metadata.SetOption("Filename", path_);
   Metadata.SetOption("Token", token_);
   Metadata.SetOption("StartFromRow", IntToString(startfromrow_));
   Metadata.SetOption("ColumnX", IntToString(xcolumn_));
   Metadata.SetOption("ColumnY", IntToString(ycolumn_));
   if (hasheader_)
      Metadata.SetOption("Header", "Si");
   else
      Metadata.SetOption("Header", "No");
   if (ispersonalized_)
      Metadata.SetOption("CustomToken", "Si");
   else
      Metadata.SetOption("CustomToken", "No");
}

/**
 * Crea el archivo de soporte correspondiente para importar
 * Dependiendo de los parametros de entrada decide si utiliza el driver de gdal o si se crea un
 * archivo temporal con un formato aceptado por gdal ya que la lectura y el parseo son optimos
 * 1. Gdal acepta archivos sin extension o con extesion csv
 * 2. Gdal acepta tres tipos de tokens: tabulacion, coma y punto y coma
 * 3. Gdal importa la totalidad del archivo
 */
DatasourceInterface* CsvVectorCreator::Create() {
   DatasourceInterface* pdatasource = NULL;
   // Obtengo los metadatos para agregar al proyecto suri
   Option metadata;
   GetMetadataOptions(metadata);
   wxFileName file(path_);
   if ((file.GetExt().compare("csv") == 0 || file.GetExt().compare("") == 0) &&
         !ispersonalized_ && startfromrow_ == 1)
      pdatasource = CreateVrt(metadata);
   else
      pdatasource = CreateFile(metadata);
   return pdatasource;
}

/** Crea un archivo temporal que pueda ser leido por el driver vrt de gdal */
DatasourceInterface* CsvVectorCreator::CreateFile(Option Metadata) {
   // Creo el archivo temporal
   wxFile csvfile;
   wxFileName filename(path_);
   filename.SetExt(wxT("csv"));

   // Abro los archivos
   std::ifstream file;
   if (!csvfile.Open(filename.GetFullPath(), wxFile::write))
      return NULL;
   file.open(path_.c_str(), std::ifstream::in);
   if (!file.is_open()) {
      SHOW_ERROR(message_OPEN_FILE_ERROR);
      return NULL;
   }
   // Itero hasta llegar a la primer linea indicada
   int startfromrow = startfromrow_;
   startfromrow--;
   std::string line;
   for (int i = 0; i < startfromrow; ++i)
      getline(file, line);

   // Mientras no haya ningun flag de error (eofbit, failbit and badbit) itero el archivo
   while (file.good()) {
      getline(file, line);
      // Salteo las lineas en blanco
      if (line.empty())
         continue;
      // Si existen tokens personalizados los reemplazo por un token admitido por el driver vrt
      size_t startpos = 0;
      while (ispersonalized_ && (startpos = line.find(token_, startpos)) != std::string::npos) {
         line.replace(startpos, token_.length(), ";");
      }

      line.append("\r\n");
      csvfile.Write(line);
   }
   file.close();
   return CreateVrt(Metadata);
}

} /** namespace suri */
