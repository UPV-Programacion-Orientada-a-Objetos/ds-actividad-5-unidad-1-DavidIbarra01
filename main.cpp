#include <iostream>

template <typename T>
class MatrizBase {
protected:
    int _filas;
    int _columnas;

public:
    virtual void cargarValores() = 0;
    virtual MatrizBase<T>* sumar(const MatrizBase<T>& otra) const = 0;
    virtual void imprimir() const = 0;
};

template <typename T>
class MatrizDinamica : public MatrizBase<T> {
    T **_datos;
};

template <typename T, int M, int N>
class MatrizEstatica : public MatrizBase<T> {
    T _datos[M][N];
};


int main() {

    return 0;
}