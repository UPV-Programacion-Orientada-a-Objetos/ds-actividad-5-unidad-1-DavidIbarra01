#include <iostream>

template <typename T>
class MatrizBase {
protected:
    int _filas;
    int _columnas;

public:
    // Constructor
    MatrizBase(int filas, int columnas) : _filas(filas), _columnas(columnas) {}
    
    // Destructor
    virtual ~MatrizBase() {}
    
    // Metodos virtuales
    virtual void cargarValores() = 0;
    virtual MatrizBase<T>* sumar(const MatrizBase<T>& otra) const = 0;
    virtual void imprimir() const = 0;
    
    // Sobrecarga del operador mass
    MatrizBase<T>* operator+(const MatrizBase<T>& otra) const {
        return this->sumar(otra);
    }
    
    // Mis getters
    int getFilas() const { return _filas; }
    int getColumnas() const { return _columnas; }
    
    virtual T getElemento(int i, int j) const = 0;
};

template <typename T>
class MatrizDinamica : public MatrizBase<T> {
private:
    T** _datos;
    
    void asignarMemoria() {
        _datos = new T*[this->_filas];
        for (int i = 0; i < this->_filas; i++) {
            _datos[i] = new T[this->_columnas];
        }
    }
    
    void liberarMemoria() {
        if (_datos != nullptr) {
            for (int i = 0; i < this->_filas; i++) {
                delete[] _datos[i];
            }
            delete[] _datos;
            _datos = nullptr;
        }
    }
//-------------------------------------------------------
public:
    // Constructor
    MatrizDinamica(int filas, int columnas) : MatrizBase<T>(filas, columnas) {
        asignarMemoria();
        // Inicializar en ceros
        for (int i = 0; i < this->_filas; i++) {
            for (int j = 0; j < this->_columnas; j++) {
                _datos[i][j] = T(0);
            }
        }
        std::cout << "Matriz Dinamica creada (" << this->_filas << "x" << this->_columnas << ")" << std::endl;
    }
    
    ~MatrizDinamica() {
        std::cout << "Liberando memoria de Matriz Dinamica..." << std::endl;
        liberarMemoria();
    }
    
    MatrizDinamica(const MatrizDinamica<T>& otra) : MatrizBase<T>(otra._filas, otra._columnas) {
        asignarMemoria();
        for (int i = 0; i < this->_filas; i++) {
            for (int j = 0; j < this->_columnas; j++) {
                _datos[i][j] = otra._datos[i][j];
            }
        }
    }
    
    MatrizDinamica<T>& operator=(const MatrizDinamica<T>& otra) {
        if (this != &otra) {
            liberarMemoria();
            
            this->_filas = otra._filas;
            this->_columnas = otra._columnas;
            asignarMemoria();
            for (int i = 0; i < this->_filas; i++) {
                for (int j = 0; j < this->_columnas; j++) {
                    _datos[i][j] = otra._datos[i][j];
                }
            }
        }
        return *this;
    }
    
    MatrizDinamica(MatrizDinamica<T>&& otra) : MatrizBase<T>(otra._filas, otra._columnas) {
        _datos = otra._datos;
        otra._datos = nullptr;
        otra._filas = 0;
        otra._columnas = 0;
    }
    
    MatrizDinamica<T>& operator=(MatrizDinamica<T>&& otra) {
        if (this != &otra) {
            liberarMemoria();
            
            this->_filas = otra._filas;
            this->_columnas = otra._columnas;
            _datos = otra._datos;
            
            otra._datos = nullptr;
            otra._filas = 0;
            otra._columnas = 0;
        }
        return *this;
    }
    //------------------------------------
    void cargarValores() {
        std::cout << "Ingrese los valores para la matriz (" << this->_filas << "x" << this->_columnas << "):" << std::endl;
        for (int i = 0; i < this->_filas; i++) {
            for (int j = 0; j < this->_columnas; j++) {
                std::cout << "Elemento [" << i << "][" << j << "]: ";
                std::cin >> _datos[i][j];
            }
        }
    }
    
    void cargarValoresPredefinidos(T valores[][10], int cols) {
        for (int i = 0; i < this->_filas; i++) {
            for (int j = 0; j < this->_columnas; j++) {
                _datos[i][j] = valores[i][j];
            }
        }
    }

    MatrizBase<T>* sumar(const MatrizBase<T>& otra) const {
        // Verificar dimensiones compatibles
        if (this->_filas != otra.getFilas() || this->_columnas != otra.getColumnas()) {
            std::cout << "Error: Las matrices deben tener las mismas dimensiones." << std::endl;
            return nullptr;
        }
        
        MatrizDinamica<T>* resultado = new MatrizDinamica<T>(this->_filas, this->_columnas);
        
        for (int i = 0; i < this->_filas; i++) {
            for (int j = 0; j < this->_columnas; j++) {
                resultado->_datos[i][j] = _datos[i][j] + otra.getElemento(i, j);
            }
        }
        
        return resultado;
    }
    
    // Implementacion de imprimir
    void imprimir() const {
        for (int i = 0; i < this->_filas; i++) {
            std::cout << "|";
            for (int j = 0; j < this->_columnas; j++) {
                std::cout << " " << _datos[i][j] << " ";
                if (j < this->_columnas - 1) std::cout << "|";
            }
            std::cout << "|" << std::endl;
        }
    }
    
    T getElemento(int i, int j) const {
        return _datos[i][j];
    }
};

//------------------------------------------------
template <typename T, int M, int N>
class MatrizEstatica : public MatrizBase<T> {
private:
    T _datos[M][N];

public:
    // Constructor
    MatrizEstatica() : MatrizBase<T>(M, N) {
        // Inicializar en ceros
        for (int i = 0; i < M; i++) {
            for (int j = 0; j < N; j++) {
                _datos[i][j] = T(0);
            }
        }
        std::cout << "Matriz Estatica creada (" << M << "x" << N << ")" << std::endl;
    }
    
    // Destructor
    ~MatrizEstatica() {
        std::cout << "Destruyendo Matriz Estatica..." << std::endl;
    }
    
    // Implementacion de cargarValores
    void cargarValores() {
        std::cout << "Ingrese los valores para la matriz (" << M << "x" << N << "):" << std::endl;
        for (int i = 0; i < M; i++) {
            for (int j = 0; j < N; j++) {
                std::cout << "Elemento [" << i << "][" << j << "]: ";
                std::cin >> _datos[i][j];
            }
        }
    }
    
    // Metodo para cargar valores predefinidos
    void cargarValoresPredefinidos(T valores[][N]) {
        for (int i = 0; i < M; i++) {
            for (int j = 0; j < N; j++) {
                _datos[i][j] = valores[i][j];
            }
        }
    }
    
    // Implementacion de sumar
    MatrizBase<T>* sumar(const MatrizBase<T>& otra) const {
        
        if (M != otra.getFilas() || N != otra.getColumnas()) {
            std::cout << "Error: Las matrices deben tener las mismas dimensiones." << std::endl;
            return nullptr;
        }
        
        // Crear nueva matriz dinamica para el resultado
        MatrizDinamica<T>* resultado = new MatrizDinamica<T>(M, N);
        
        // Realizar la suma
        for (int i = 0; i < M; i++) {
            for (int j = 0; j < N; j++) {
                T suma = _datos[i][j] + otra.getElemento(i, j);
                MatrizDinamica<T>* temp = dynamic_cast<MatrizDinamica<T>*>(resultado);
            }
        }
        
        for (int i = 0; i < M; i++) {
            for (int j = 0; j < N; j++) {
                T suma = _datos[i][j] + otra.getElemento(i, j);
                delete resultado;
                resultado = new MatrizDinamica<T>(M, N);
                break;
            }
            break;
        }
        
        delete resultado;
        resultado = new MatrizDinamica<T>(M, N);
        T valoresTemp[M][10];
        for (int i = 0; i < M; i++) {
            for (int j = 0; j < N; j++) {
                valoresTemp[i][j] = _datos[i][j] + otra.getElemento(i, j);
            }
        }
        resultado->cargarValoresPredefinidos(valoresTemp, N);
        
        return resultado;
    }
    
    // Implementacion de imprimir
    void imprimir() const {
        for (int i = 0; i < M; i++) {
            std::cout << "|";
            for (int j = 0; j < N; j++) {
                std::cout << " " << _datos[i][j] << " ";
                if (j < N - 1) std::cout << "|";
            }
            std::cout << "|" << std::endl;
        }
    }
    
    // Implementacion de getElemento
    T getElemento(int i, int j) const {
        return _datos[i][j];
    }
};
//----------------------------------------
int main() {
    std::cout << "--- Sistema Generico de Algebra Lineal ---" << std::endl << std::endl;
    
    // Matriz flotante ====
    std::cout << ">> Demostracion de Genericidad (Tipo FLOAT) <<" << std::endl << std::endl;
    
    std::cout << "// 1. Creacion de Matriz Dinamica A (3x2)..." << std::endl;
    MatrizBase<float>* A = new MatrizDinamica<float>(3, 2);
    
    float valoresA[3][10] = {
        {1.5f, 2.0f},
        {0.0f, 1.0f},
        {4.5f, 3.0f}
    };
    dynamic_cast<MatrizDinamica<float>*>(A)->cargarValoresPredefinidos(valoresA, 2);
    
    std::cout << "A =" << std::endl;
    A->imprimir();
    std::cout << std::endl;
    
    std::cout << "// 2. Creacion de Matriz Estatica B (3x2)..." << std::endl;
    MatrizEstatica<float, 3, 2>* BEstatica = new MatrizEstatica<float, 3, 2>();
    MatrizBase<float>* B = BEstatica;
    
    float valoresB[3][2] = {
        {0.5f, 1.0f},
        {2.0f, 3.0f},
        {1.0f, 1.0f}
    };
    BEstatica->cargarValoresPredefinidos(valoresB);
    
    std::cout << "B =" << std::endl;
    B->imprimir();
    std::cout << std::endl;
    
    std::cout << "// 3. Operacion Polimorfica (Suma)" << std::endl;
    std::cout << "SUMANDO: Matriz C = A + B ..." << std::endl;
    std::cout << "(La suma es manejada por el metodo virtual de MatrizDinamica)" << std::endl << std::endl;
    
    MatrizBase<float>* C = (*A) + (*B);
    
    if (C != nullptr) {
        std::cout << "Matriz Resultado C (3x2, Tipo FLOAT):" << std::endl;
        C->imprimir();
        std::cout << std::endl;
    }
    
    // LA MATRIZ TIPO INT ===
    std::cout << std::endl << ">> Demostracion con Tipo INT <<" << std::endl << std::endl;
    
    MatrizBase<int>* D = new MatrizDinamica<int>(2, 2);
    int valoresD[2][10] = {
        {5, 10},
        {15, 20}
    };
    dynamic_cast<MatrizDinamica<int>*>(D)->cargarValoresPredefinidos(valoresD, 2);
    
    MatrizEstatica<int, 2, 2>* EEstatica = new MatrizEstatica<int, 2, 2>();
    MatrizBase<int>* E = EEstatica;
    int valoresE[2][2] = {
        {1, 2},
        {3, 4}
    };
    EEstatica->cargarValoresPredefinidos(valoresE);
    
    std::cout << "Matriz D (INT):" << std::endl;
    D->imprimir();
    std::cout << std::endl;
    
    std::cout << "Matriz E (INT):" << std::endl;
    E->imprimir();
    std::cout << std::endl;
    
    std::cout << "Suma F = D + E:" << std::endl;
    MatrizBase<int>* F = D->sumar(*E);
    if (F != nullptr) {
        F->imprimir();
        std::cout << std::endl;
    }
    
    //LIMPIEZA DE MEMORIA ======
    std::cout << ">> Demostracion de Limpieza de Memoria <<" << std::endl;
    
    std::cout << "Llamando al destructor de C..." << std::endl;
    delete C;
    
    std::cout << "Llamando al destructor de A..." << std::endl;
    delete A;
    
    std::cout << "Llamando al destructor de B..." << std::endl;
    delete B;
    
    std::cout << "Llamando al destructor de D..." << std::endl;
    delete D;
    
    std::cout << "Llamando al destructor de E..." << std::endl;
    delete E;
    
    std::cout << "Llamando al destructor de F..." << std::endl;
    delete F;
    
    std::cout << std::endl << "Sistema cerrado." << std::endl;
    
    return 0;
}