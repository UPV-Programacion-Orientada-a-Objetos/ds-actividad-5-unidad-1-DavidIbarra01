#include <iostream>

using namespace std;

//CLASE BASEE
template <typename T>
class MatrizBase {
protected:
    int _filas;
    int _columnas;

public:
    // Constructor
    MatrizBase(int filas, int columnas) : _filas(filas), _columnas(columnas) {}
    
    // Destructor virtual
    virtual ~MatrizBase() {}
    
    // Métodos virtuales
    virtual void cargarValores() = 0;
    virtual MatrizBase<T>* sumar(const MatrizBase<T>& otra) const = 0;
    virtual void imprimir() const = 0;
    
    // Sobrecarga del operador MAS
    MatrizBase<T>* operator+(const MatrizBase<T>& otra) const {
        return this->sumar(otra);
    }
    
    // Mis Getters
    int getFilas() const { return _filas; }
    int getColumnas() const { return _columnas; }
    
    // Método para acceder a un elemento
    virtual T getElemento(int i, int j) const = 0;
};
//--------------------------------------------------------------
template <typename T>
class MatrizDinamica : public MatrizBase<T> {
private:
    T** _datos;
    
    // Método auxiliar para asignar memoria
    void asignarMemoria() {
        _datos = new T*[this->_filas];
        for (int i = 0; i < this->_filas; i++) {
            _datos[i] = new T[this->_columnas];
        }
    }
    
    // Método auxiliar para liberar memoria
    void liberarMemoria() {
        if (_datos != nullptr) {
            for (int i = 0; i < this->_filas; i++) {
                delete[] _datos[i];
            }
            delete[] _datos;
            _datos = nullptr;
        }
    }
//------------------------------------------------------------------
public:
    
    MatrizDinamica(int filas, int columnas) : MatrizBase<T>(filas, columnas) {
        asignarMemoria();
        // Inicializar en ceross
        for (int i = 0; i < this->_filas; i++) {
            for (int j = 0; j < this->_columnas; j++) {
                _datos[i][j] = T(0);
            }
        }
        cout << "Matriz Dinamica creada (" << this->_filas << "x" << this->_columnas << ")" << endl;
    }
    
    // Destructor
    ~MatrizDinamica() {
        cout << "Liberando memoria de Matriz Dinamica..." << endl;
        liberarMemoria();
    }
    
    // Constructor de Copia 
    MatrizDinamica(const MatrizDinamica<T>& otra) : MatrizBase<T>(otra._filas, otra._columnas) {
        asignarMemoria();
        // Copia profunda
        for (int i = 0; i < this->_filas; i++) {
            for (int j = 0; j < this->_columnas; j++) {
                _datos[i][j] = otra._datos[i][j];
            }
        }
    }
    // Operador de Asignación 
    MatrizDinamica<T>& operator=(const MatrizDinamica<T>& otra) {
        if (this != &otra) {
            // Liberar memoria actual
            liberarMemoria();
            
            // Copiar dimensiones
            this->_filas = otra._filas;
            this->_columnas = otra._columnas;
            
            // Asignar nueva memoria y copiar datos
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
    //----------------------------------------------------------
    // Implementación del metodo cargarValores
    void cargarValores() {
        cout << "Ingrese los valores para la matriz (" << this->_filas << "x" << this->_columnas << "):" << endl;
        for (int i = 0; i < this->_filas; i++) {
            for (int j = 0; j < this->_columnas; j++) {
                cout << "Elemento [" << i << "][" << j << "]: ";
                cin >> _datos[i][j];
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
    
    // Implementación de sumar
    MatrizBase<T>* sumar(const MatrizBase<T>& otra) const {
      
        if (this->_filas != otra.getFilas() || this->_columnas != otra.getColumnas()) {
            cout << "Error: Las matrices deben tener las mismas dimensiones." << endl;
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
    
    void imprimir() const {
        for (int i = 0; i < this->_filas; i++) {
            cout << "|";
            for (int j = 0; j < this->_columnas; j++) {
                cout << " " << _datos[i][j] << " ";
                if (j < this->_columnas - 1) cout << "|";
            }
            cout << "|" << endl;
        }
    }
    
    T getElemento(int i, int j) const {
        return _datos[i][j];
    }
};

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
        cout << "Matriz Estatica creada (" << M << "x" << N << ")" << endl;
    }
    
    // Destructor
    ~MatrizEstatica() {
        cout << "Destruyendo Matriz Estatica..." << endl;
    }
    
    // Implementación de cargarValores
    void cargarValores() {
        cout << "Ingrese los valores para la matriz (" << M << "x" << N << "):" << endl;
        for (int i = 0; i < M; i++) {
            for (int j = 0; j < N; j++) {
                cout << "Elemento [" << i << "][" << j << "]: ";
                cin >> _datos[i][j];
            }
        }
    }
    
    // Método para cargar valores predefinidos
    void cargarValoresPredefinidos(T valores[][N]) {
        for (int i = 0; i < M; i++) {
            for (int j = 0; j < N; j++) {
                _datos[i][j] = valores[i][j];
            }
        }
    }
    
    // Implementación de sumar
    MatrizBase<T>* sumar(const MatrizBase<T>& otra) const {
        if (M != otra.getFilas() || N != otra.getColumnas()) {
            cout << "Error: Las matrices deben tener las mismas dimensiones." << endl;
            return nullptr;
        }
        
        MatrizDinamica<T>* resultado = new MatrizDinamica<T>(M, N);
        
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
    
    void imprimir() const {
        for (int i = 0; i < M; i++) {
            cout << "|";
            for (int j = 0; j < N; j++) {
                cout << " " << _datos[i][j] << " ";
                if (j < N - 1) cout << "|";
            }
            cout << "|" << endl;
        }
    }
    
    // Implementación de getElemento
    T getElemento(int i, int j) const {
        return _datos[i][j];
    }
};
//--------------------------------------------------------------------
int main() {
    cout << "--- Sistema Generico de Algebra Lineal ---" << endl << endl;
    //LOS FLOATS =====
    cout << ">> Demostracion de Genericidad (Tipo FLOAT) <<" << endl << endl;
    
    // 1. Creación de Matriz Dinámica A
    cout << "// 1. Creacion de Matriz Dinamica A (3x2)..." << endl;
    MatrizBase<float>* A = new MatrizDinamica<float>(3, 2);
    
    // Cargar valores predefinidos en A
    float valoresA[3][10] = {
        {1.5f, 2.0f},
        {0.0f, 1.0f},
        {4.5f, 3.0f}
    };
    dynamic_cast<MatrizDinamica<float>*>(A)->cargarValoresPredefinidos(valoresA, 2);
    
    cout << "A =" << endl;
    A->imprimir();
    cout << endl;
    
    // 2. Creación de Matriz Estática B
    cout << "// 2. Creacion de Matriz Estatica B (3x2)..." << endl;
    MatrizEstatica<float, 3, 2>* BEstatica = new MatrizEstatica<float, 3, 2>();
    MatrizBase<float>* B = BEstatica;
    
    // Cargar valores predefinidos en B
    float valoresB[3][2] = {
        {0.5f, 1.0f},
        {2.0f, 3.0f},
        {1.0f, 1.0f}
    };
    BEstatica->cargarValoresPredefinidos(valoresB);
    
    cout << "B =" << endl;
    B->imprimir();
    cout << endl;
    
    // 3. Operación Polimórfica - Suma usando operador +
    cout << "// 3. Operacion Polimorfica (Suma)" << endl;
    cout << "SUMANDO: Matriz C = A + B ..." << endl;
    cout << "(La suma es manejada por el metodo virtual de MatrizDinamica)" << endl << endl;
    
    MatrizBase<float>* C = (*A) + (*B);
    
    if (C != nullptr) {
        cout << "Matriz Resultado C (3x2, Tipo FLOAT):" << endl;
        C->imprimir();
        cout << endl;
    }
    
    // LOS INT =====
    cout << endl << ">> Demostracion con Tipo INT <<" << endl << endl;
    
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
    
    cout << "Matriz D (INT):" << endl;
    D->imprimir();
    cout << endl;
    
    cout << "Matriz E (INT):" << endl;
    E->imprimir();
    cout << endl;
    
    cout << "Suma F = D + E:" << endl;
    MatrizBase<int>* F = D->sumar(*E);
    if (F != nullptr) {
        F->imprimir();
        cout << endl;
    }
    
    // LIMPIAR MEMORIAA
    cout << ">> Demostracion de Limpieza de Memoria <<" << endl;
    
    cout << "Llamando al destructor de C..." << endl;
    delete C;
    
    cout << "Llamando al destructor de A..." << endl;
    delete A;
    
    cout << "Llamando al destructor de B..." << endl;
    delete B;
    
    cout << "Llamando al destructor de D..." << endl;
    delete D;
    
    cout << "Llamando al destructor de E..." << endl;
    delete E;
    
    cout << "Llamando al destructor de F..." << endl;
    delete F;
    
    cout << endl << "Sistema cerrado." << endl;
    
    return 0;
}