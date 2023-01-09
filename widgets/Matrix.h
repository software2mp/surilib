/**
 * Implementacion por Jose Guerrero.
 *
 * URL: http://joseguerreroa.wordpress.com/
 *
 * Nota: se realizaron algunas mejoras y correcciones.
 */

#include <iostream>

using namespace std;

#include <iomanip>
#include <cstdlib>
#include <cassert>
#include <cmath>

template<typename T> class Matrix;

template<typename T> Matrix<T> operator +(Matrix<T> x, Matrix<T> y);
template<typename T> Matrix<T> operator -(Matrix<T> x, Matrix<T> y);
template<typename T> Matrix<T> operator *(Matrix<T> x, Matrix<T> y);
template<typename T> Matrix<T> operator *(T x, Matrix<T> y);

template<class T>
class Matrix {

private:

   T **a;
   int rows, cols;

public:

   Matrix(int m, int n);
   ~Matrix(); // Destructor de la clase

   void initMatrix(T **x);
   void inverMat(T **a);
   void autovecMat(T **a);
   void autovalMat(T **a);
   void transpMat(Matrix<T> x);

   T** getMat();

   T deterMat();

   friend Matrix operator +<>(Matrix<T> x, Matrix<T> y); //sobrecarga al operador + para sumar matrices
   friend Matrix operator -<>(Matrix<T> x, Matrix<T> y); //sobrecarga al operador - para sumar matrices
   friend Matrix operator *<>(Matrix<T> x, Matrix<T> y); //sobrecarga al operador * para multiplicar matrices
   friend Matrix operator *<>(T x, Matrix<T> y); //sobrecarga al operador * para multiplicar escalar*matriz
};

//Constructor por parametros
template<typename T>
Matrix<T>::Matrix(int m, int n) {
   rows = m;
   cols = n;

   a = new T*[rows];

   for (int i = 0; i < rows; ++i) {
      a[i] = new T[cols];
   }
}

template<typename T>
Matrix<T>::~Matrix() {

}

template<typename T>
void Matrix<T>::initMatrix(T **x) {
   for (int i = 0; i < rows; ++i) {
      for (int j = 0; j < cols; ++j) {
         a[i][j] = x[i][j];
      }
   }
}

template<typename T>
Matrix<T> operator +(Matrix<T> x, Matrix<T> y) { //sobrecarga al operador + para sumar matrices

   assert(x.rows == y.rows || x.cols == y.cols);

   Matrix<T> sum_mat(x.rows, x.cols);

   for (int i = 0; i < x.rows; ++i) {
      for (int j = 0; j < x.cols; ++j) {
         sum_mat.a[i][j] = x.a[i][j] + y.a[i][j];
      }
   }

   return sum_mat;

}

template<typename T>
Matrix<T> operator -(Matrix<T> x, Matrix<T> y) { //sobrecarga al operador + para restar matrices

   assert(x.rows == y.rows || x.cols == y.cols);

   Matrix<T> rest_mat(x.rows, x.cols);

   for (int i = 0; i < x.rows; ++i) {
      for (int j = 0; j < x.cols; ++j) {
         rest_mat.a[i][j] = x.a[i][j] + y.a[i][j];
      }
   }

   return rest_mat;

}

template<typename T>
Matrix<T> operator *(Matrix<T> x, Matrix<T> y) { //sobrecarga al operador * para multiplicar matrices

   assert(x.cols == y.rows);

   Matrix<T> mult_mat(x.rows, y.cols);

   for (int i = 0; i < x.rows; ++i) {
      for (int j = 0; j < x.cols; ++j) {
         mult_mat.a[i][j] = 0;

         for (int k = 0; k < x.cols; ++k) {
            mult_mat.a[i][j] += x.a[i][k] * y.a[k][j];
         }
      }
   }

   return mult_mat;
}

template<typename T>
Matrix<T> operator *(T x, Matrix<T> y) { //sobrecarga al operador * para multiplicar escalar*Matrix

   Matrix<T> esc_mat(y.rows, y.cols);

   for (int i = 0; i < y.rows; ++i) {
      for (int j = 0; j < y.cols; ++j) {
         esc_mat.a[i][j] = x * y.a[i][j];
      }
   }

   return esc_mat;
}

template<typename T>
void Matrix<T>::inverMat(T **f) {

   // Algoritmo para la eliminación simple de Gauss
   int i, j, k, n = rows, m = cols;

   assert(rows == cols);

   double factor;

   double **L, **ainv, **z, *D, *X;

   X = new double[n];
   D = new double[n];

   L = new double*[n];
   ainv = new double*[n];
   z = new double*[n];

   for (j = 0; j < n; ++j) {
      L[j] = new double[n];
      ainv[j] = new double[n];
      z[j] = new double[n];
   }

   for (i = 0; i < n; ++i) {
      for (j = 0; j < n; ++j) {
         z[i][j] = f[i][j];
      }
   }

   for (k = 0; k < n - 1; ++k) {
      for (i = k + 1; i < n; ++i) {
         factor = z[i][k] / z[k][k];
         for (j = k + 1; j < n + 1; ++j) {
            z[i][j] = z[i][j] - factor * z[k][j];
         }
      }
   }

   // Cálculo del determinante
   double determ = 1.0;

   for (i = 0; i < n; ++i) {
      determ = determ * z[i][i];
   }

   assert(determ != 0);

   // Rutina para determinar las matrices L (inferior) y U (superior) de la
   // descomposición LU.
   for (i = 0; i < n; ++i) {
      for (j = 0; j < n; ++j) {
         if (i > j) {
            L[i][j] = z[i][j] / z[j][j];
            z[i][j] = 0;
         }
      }
   }

   for (i = 0; i < n; ++i) {
      for (j = 0; j < n; ++j) {
         L[j][j] = 1;
      }
   }

   // Implementación de la rutina para el cálculo de la inversa

   for (k = 0; k < n; ++k) {
      // Esta rutina inicializa los L[i][n] para ser utilizados con la matriz L
      for (i = 0; i < n; ++i) {
         if (i == k)
            L[i][n] = 1;
         else
            L[i][n] = 0;
      }

      // Esta función implementa la sustitución hacia adelante con la matriz L y los L[i][n]
      // que produce la rutina anterior

      double sum = 0.0;

      D[0] = L[0][n];

      for (i = 1; i < n; ++i) {
         sum = 0;

         for (j = 0; j < i; ++j) {
            sum = sum + L[i][j] * D[j];
         }

         D[i] = L[i][n] - sum;
      }

      // Esta rutina asigna los D[i] que produce forward para ser utilizados con la matriz U
      for (i = 0; i < n; ++i) {
         z[i][n] = D[i];
      }

      // Rutina que aplica la sustitución hacia atras
      X[n - 1] = z[n - 1][n] / z[n - 1][n - 1];

      // Determinación de las raíces restantes
      for (i = n - 2; i > -1; --i) {
         sum = 0;

         for (j = i + 1; j < n; ++j) {
            sum = sum + z[i][j] * X[j];
         }

         X[i] = (z[i][n] - sum) / z[i][i];
      }

      // Esta rutina asigna los X[i] que produce Sustituir como los elementos de la matriz inversa

      for (i = 0; i < n; ++i) {
         ainv[i][k] = X[i];
      }

   } // llave de cierre del for para k

   for (i = 0; i < n; ++i) {
      for (j = 0; j < n; ++j) {
         a[i][j] = ainv[i][j];
      }
   }

   delete L, D, X, ainv, z;
}

template<typename T>
T Matrix<T>::deterMat() {

   // Algoritmo para la eliminación simple de Gauss

   int i, j, k, n = rows, m = cols;

   assert(rows == cols);

   double **b = new double*[n];

   for (j = 0; j < n; ++j) {
      b[j] = new double[n];
   }

   for (i = 0; i < n; ++i) {
      for (j = 0; j < n; ++j) {
         b[i][j] = a[i][j];
      }
   }

   double factor = 0.0;

   for (k = 0; k < n - 1; ++k) {
      for (i = k + 1; i < n; ++i) {
         factor = b[i][k] / b[k][k];

         for (j = k + 1; j < n + 1; ++j) {
            b[i][j] = b[i][j] - factor * b[k][j];
         }
      }
   }

   // Cálculo del determinante
   double determ = 1.0;

   for (i = 0; i < n; ++i) {
      determ = determ * b[i][i];
   }

   delete[] b;

   return determ;
}

template<typename T>
void Matrix<T>::autovecMat(T **m) {

   // Define las variables de tipo entero
   int i, j, ip, iq, nrot, n;

   n = rows;

   double **v, **f;

   v = new double *[n];
   f = new double *[n];

   for (i = 0; i < rows; ++i) {
      v[i] = new double[n];
      f[i] = new double[n];
   }

   for (i = 0; i < n; ++i) {
      for (j = 0; j < n; ++j) {
         f[i][j] = m[i][j];
      }
   }

   // Define las variables de tipo doble
   double *b, *z, *d;

   b = new double[n];
   z = new double[n];
   d = new double[n];

   double c, g, h, s, sm, t, tau, theta, tresh;

   // Inicializa a la Matrix identidad
   for (ip = 0; ip < n; ++ip) {
      for (iq = 0; iq < n; ++iq) {
         v[ip][iq] = 0;
      }

      v[ip][ip] = 1;
   }

   // Inicializa b y d a la diagonal de a
   for (ip = 0; ip < n; ++ip) {
      b[ip] = f[ip][ip];
      d[ip] = b[ip];
      z[ip] = 0;
   }

   nrot = 0;

   for (i = 0; i < 50; ++i) {
      sm = 0;

      for (ip = 0; ip < n - 1; ++ip) {
         for (iq = ip + 1; iq < n; ++iq) {
            sm += fabs(f[ip][iq]);
         }
      }

      if (sm == 0) break;

      if (i < 4)
         tresh = 0.2 * sm / (n * n);
      else
         tresh = 0.0;

      for (ip = 0; ip < n - 1; ++ip) {
         for (iq = ip + 1; iq < n; ++iq) {
            g = 100.0 * fabs(f[ip][iq]);

            if (i > 4 && (double) (fabs(d[ip]) + g) == (double) fabs(d[ip])
                  && (double) (fabs(d[iq]) + g) == (double) fabs(d[iq])) {
               f[ip][iq] = 0.0;
            } else if (fabs(f[ip][iq]) > tresh) {
               h = d[iq] - d[ip];

               if ((double) (fabs(h) + g) == (double) fabs(h)) {
                  t = (f[ip][iq]) / h; // t = 1/(2theta)
               } else {
                  theta = 0.5 * h / (f[ip][iq]);

                  t = 1.0 / (fabs(theta) + sqrt(1.0 + theta * theta));

                  if (theta < 0.0) t = -t;
               }

               c = 1.0 / sqrt(1 + t * t);

               s = t * c;

               tau = s / (1.0 + c);

               h = t * f[ip][iq];

               z[ip] -= h;

               z[iq] += h;

               d[ip] -= h;

               d[iq] += h;

               f[ip][iq] = 0.0;

               // Varía desde 0 hasta  ip - 1
               for (j = 0; j < ip; ++j) {
                  g = f[j][ip];
                  h = f[j][iq];
                  f[j][ip] = g - s * (h + g * tau);
                  f[j][iq] = h + s * (g - h * tau);
               }

               // Varía desde ip+1 hasta  iq - 1
               for (j = ip + 1; j < iq; ++j) {
                  g = f[ip][j];
                  h = f[j][iq];
                  f[ip][j] = g - s * (h + g * tau);
                  f[j][iq] = h + s * (g - h * tau);
               }

               for (j = iq + 1; j < n; ++j) {
                  g = f[ip][j];
                  h = f[iq][j];
                  f[ip][j] = g - s * (h + g * tau);
                  f[iq][j] = h + s * (g - h * tau);
               }

               for (j = 0; j < n; ++j) {
                  g = v[j][ip];
                  h = v[j][iq];
                  v[j][ip] = g - s * (h + g * tau);
                  v[j][iq] = h + s * (g - h * tau);
               }

               ++(nrot);
            }
         }
      }

      for (ip = 0; ip < n; ++ip) {
         b[ip] = b[ip] + z[ip];
         d[ip] = b[ip];
         z[ip] = 0.0;
      }

      for (i = 0; i < n; ++i) {
         for (j = 0; j < n; ++j) {
            a[i][j] = v[i][j];
         }
      }
   }

   delete[] v, delete[] f, delete[] b, delete[] z, delete[] d;
}

template<typename T>
void Matrix<T>::autovalMat(T **m) {

   // Define las variables de tipo entero
   int i, j, ip, iq, nrot, n;

   n = rows;

   double **v, **f;

   v = new double *[n];
   f = new double *[n];

   for (i = 0; i < rows; ++i) {
      v[i] = new double[n];
      f[i] = new double[n];
   }

   for (i = 0; i < n; ++i) {
      for (j = 0; j < n; ++j) {
         f[i][j] = m[i][j];
      }
   }

   // Define las variables de tipo doble
   double *b, *z, *d;

   b = new double[n];
   z = new double[n];
   d = new double[n];

   double c, g, h, s, sm, t, tau, theta, tresh;

   // Inicializa a la Matrix identidad
   for (ip = 0; ip < n; ++ip) {
      for (iq = 0; iq < n; ++iq) {
         v[ip][iq] = 0;
      }

      v[ip][ip] = 1;
   }

   // Inicializa b y d a la diagonal de a
   for (ip = 0; ip < n; ++ip) {
      b[ip] = f[ip][ip];
      d[ip] = b[ip];
      z[ip] = 0;
   }

   nrot = 0;

   for (i = 0; i < 50; ++i) {
      sm = 0;

      for (ip = 0; ip < n - 1; ++ip) {
         for (iq = ip + 1; iq < n; ++iq) {
            sm += fabs(f[ip][iq]);
         }
      }

      if (sm == 0) break;

      if (i < 4)
         tresh = 0.2 * sm / (n * n);
      else
         tresh = 0.0;

      for (ip = 0; ip < n - 1; ++ip) {
         for (iq = ip + 1; iq < n; ++iq) {
            g = 100.0 * fabs(f[ip][iq]);

            if (i > 4 && (double) (fabs(d[ip]) + g) == (double) fabs(d[ip])
                  && (double) (fabs(d[iq]) + g) == (double) fabs(d[iq])) {
               f[ip][iq] = 0.0;
            } else if (fabs(f[ip][iq]) > tresh) {
               h = d[iq] - d[ip];

               if ((double) (fabs(h) + g) == (double) fabs(h)) {
                  t = (f[ip][iq]) / h; // t = 1/(2theta)
               } else {
                  theta = 0.5 * h / (f[ip][iq]);
                  t = 1.0 / (fabs(theta) + sqrt(1.0 + theta * theta));
                  if (theta < 0.0) t = -t;
               }

               c = 1.0 / sqrt(1 + t * t);

               s = t * c;

               tau = s / (1.0 + c);

               h = t * f[ip][iq];

               z[ip] -= h;

               z[iq] += h;

               d[ip] -= h;

               d[iq] += h;

               f[ip][iq] = 0.0;

               // Varía desde 0 hasta  ip - 1
               for (j = 0; j < ip; ++j) {
                  g = f[j][ip];
                  h = f[j][iq];
                  f[j][ip] = g - s * (h + g * tau);
                  f[j][iq] = h + s * (g - h * tau);
               }

               // Varía desde ip+1 hasta  iq - 1
               for (j = ip + 1; j < iq; ++j) {
                  g = f[ip][j];
                  h = f[j][iq];
                  f[ip][j] = g - s * (h + g * tau);
                  f[j][iq] = h + s * (g - h * tau);
               }

               for (j = iq + 1; j < n; ++j) {
                  g = f[ip][j];
                  h = f[iq][j];
                  f[ip][j] = g - s * (h + g * tau);
                  f[iq][j] = h + s * (g - h * tau);
               }

               for (j = 0; j < n; ++j) {
                  g = v[j][ip];
                  h = v[j][iq];
                  v[j][ip] = g - s * (h + g * tau);
                  v[j][iq] = h + s * (g - h * tau);
               }

               ++(nrot);
            }
         }
      }

      for (ip = 0; ip < n; ++ip) {
         b[ip] = b[ip] + z[ip];
         d[ip] = b[ip];
         z[ip] = 0.0;
      }

      for (i = 0; i < n; ++i) {

         for (j = 0; j < n; ++j) {
            if (i != j)
               a[i][j] = 0;
            else
               a[i][i] = d[i];
         }
      }
   }

   delete[] v, delete[] f, delete[] b, delete[] z, delete[] d;

}

template<typename T>
void Matrix<T>::transpMat(Matrix<T> x) {
   // Determina la transpuesta de una matriz
   for (int i = 0; i < x.rows; ++i) {
      for (int j = 0; j < x.cols; ++j) {
         a[j][i] = x.a[i][j];
      }
   }
}

template<typename T>
T** Matrix<T>::getMat() {
   T** pmat = new T*[rows];

   for (int ix = 0; ix < rows; ++ix) {
      pmat[ix] = new T[cols];
   }

   for (int ix = 0; ix < rows; ++ix) {
      for (int jx = 0; jx < cols; ++jx) {
         pmat[ix][jx] = a[ix][jx];
      }
   }

   return pmat;
}
