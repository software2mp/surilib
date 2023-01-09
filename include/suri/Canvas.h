/*! \file Canvas.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef CANVAS_H_
#define CANVAS_H_

// Include suri
#include "suri/Image.h"
#include "NoDataValue.h"

// forward
class wxDC;

/** namespace suri */
namespace suri {
// forwards
class Mask;
class RasterBand;

/** Clase abstracta donde se vuelcan los datos de una renderizacion */
/**
 *  Esta clase representa el destino final de una renderizacion. Puede tratarse
 * de memoria, disco, pantalla o, eventualmente, un perfil, estadisticas, etc.
 *
 *  Esta clase se utiliza junto con el RenderizationManager para elegir el
 * destino de la renderizacion. Tambien se utiliza como etapa temporal de los
 * datos y como clase derivada para la mascara raster.
 *
 *  Esta clase debe representar como maximo (en matrices de datos o buffers)
 * SizeX()*SizeY()*DataSize()*GetBandCount() bytes de memoria reservada.
 */
class Canvas : public NoDataValue {
   /** Ctor. de Copia. */
   Canvas(const Canvas &Canvas);

public:
   /** Ctor */
   Canvas();
   /** Dtor */
   virtual ~Canvas();
   /** Inicializa el canvas para que sea compatible con el que se le pasa */
   virtual bool InitializeAs(const Canvas *pCanvas);
   /** Metodo de clonacion */
   virtual Canvas *Clone() const=0;
// ----------------------------- RASTER -----------------------------
   /** Tamanio del raster X e Y */
   virtual void GetSize(int &SizeX, int &SizeY) const;
   /** Tamanio X del raster */
   int GetSizeX() const;
   /** Tamanio Y del raster */
   int GetSizeY() const;
   /** Tamanio */
   virtual void SetSize(const int SizeX, const int SizeY);
   /** Tamanio X */
   void SetSizeX(const int Size);
   /** Tamanio Y */
   void SetSizeY(const int Size);
   /** Lectura del canvas */
   void Read(std::vector<int> &BandIndex, std::vector<void*> &OutputData) const;
   /** Lectura hacia otro canvas (in-place) */
   void Read(Canvas *pTarget, const Mask *pMask = NULL) const;
   /** Escritura al canvas */
   /**
    *  Escribe los datos de Data, en las bandas indicadas por BandIndex en el
    * propio canvas.
    * \attention Los datos son copiados, deben ser eliminados por el codigo
    *            que lo llamo.
    * @param[in] BandIndex indice de bandas donde se escriben los datos
    * @param[in] InputData vector de datos que deben escribirse en las bandas locales
    *                 indicadas por BandIndex.
    * @param[in] pMask Mascara de los datos
    */
   virtual void Write(std::vector<int> &BandIndex, std::vector<void*> &InputData,
                      const Mask *pMask = NULL)=0;
   /** Escritura desde otro canvas (in-place) */
   void Write(const Canvas *pSource, const Mask *pMask = NULL);
// ----------------------------- VECTORIAL -----------------------------
   /** Canvas vectorial */
   virtual wxDC *GetDC();
   /** Escribe los datos */
   virtual void Flush(const Mask *pMask);
   /** Limpia los datos del canvas */
   virtual void Clear()=0;
// -------------------------- MULTI-RASTER --------------------------
   /** Retorna la cantidad de bandas */
   virtual int GetBandCount() const=0;
   /** Permite modificar la cantidad de bandas (en algunos casos) */
   virtual void SetBandCount(int BandCount, bool SaveBands = false)=0;
// --------------------------- DATA-TYPE ----------------------------
   /** Nombre del tipo de dato */
   virtual std::string GetDataType() const=0;
   /** Tamanio del dato en bytes */
   virtual int GetDataSize() const=0;
   /** Nombre del tipo de dato */
   virtual void SetDataType(const std::string& DataType)=0;
// --------------------------- DEBUG ----------------------------
   void SaveCanvas(const std::string Filename)const;
protected:
   /** Retorna buffers internos con los datos */
   /**
    * Llena OutputData con los punteros a los buffers internos correspondientes
    * a los indices indicados en BandIndex del Canvas. En caso de no tener los
    * datos representados en forma de punteros, debera realizar una copia de
    * datos.
    * \note Los buffers solo sirven para lectura, no debe escribirse sobre los
    *       mismos ya que representan la mejor representacion de los datos
    *       internos, pero puedon no ser realmente los datos almacenados sino
    *       una copia de los mismos.
    * \attention : Los datos apuntados por los punteros pueden cambiar, por lo
    *              que deben ser copiados o utilizados lo antes posible.
    * @param[in] BandIndex Indices de los datos solicitados
    * @param[out] OutputData Array con punteros a los datos internos
    */
   virtual void GetInternalData(std::vector<int> &BandIndex,
                                std::vector<void*> &OutputData)=0;

private:
   int sizeX_; /*! Dimension en X, inicializa en -1 */
   int sizeY_; /*! Dimension en Y, inicializa en -1 */
};

/** operador de igualdad para canvas */
/**
 *  Verifica el tamanio, cantidad de bandas y tipo de dato para determinar si
 * dos canvas son iguales (compatibles)
 * @param[in] Lhs canvas a comparar a la izquierda del operador ==
 * @param[in] Rhs canvas a comparar a la derecha del operador ==
 */
bool operator==(const Canvas &Lhs, const Canvas &Rhs);

/** operador desigualdad para canvas */
/**
 *  Verifica el tamanio, cantidad de bandas y tipo de dato para determinar si
 * dos canvas son distintos (incompatibles)
 * @param[in] Lhs canvas a comparar a la izquierda del operador ==
 * @param[in] Rhs canvas a comparar a la derecha del operador ==
 */
bool operator!=(const Canvas &Lhs, const Canvas &Rhs);
}

#endif /*CANVAS_H_*/
