#include <iostream>

using namespace std;

// ============================================================
// CLASE BASE ABSTRACTA: MatrizBase<T>
// ============================================================
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
    
    // Métodos virtuales puros (obligatorios en clases derivadas)
    virtual void cargarValores() = 0;
    virtual MatrizBase<T>* sumar(const MatrizBase<T>& otra) const = 0;
    virtual void imprimir() const = 0;
    
    // Sobrecarga del operador +
    MatrizBase<T>* operator+(const MatrizBase<T>& otra) const {
        return this->sumar(otra);
    }
    
    // Getters para acceso a dimensiones
    int getFilas() const { return _filas; }
    int getColumnas() const { return _columnas; }
    
    // Método para acceder a un elemento (virtual puro)
    virtual T getElemento(int i, int j) const = 0;
};

// ============================================================
// CLASE CONCRETA: MatrizDinamica<T>
// ============================================================
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
        cout << "Matriz Dinamica creada (" << this->_filas << "x" << this->_columnas << ")" << endl;
    }
    
    // Destructor (REGLA DE LOS CINCO - 1/5)
    ~MatrizDinamica() {
        cout << "Liberando memoria de Matriz Dinamica..." << endl;
        liberarMemoria();
    }
    
    // Constructor de Copia (REGLA DE LOS CINCO - 2/5)
    MatrizDinamica(const MatrizDinamica<T>& otra) : MatrizBase<T>(otra._filas, otra._columnas) {
        asignarMemoria();
        // Copia profunda
        for (int i = 0; i < this->_filas; i++) {
            for (int j = 0; j < this->_columnas; j++) {
                _datos[i][j] = otra._datos[i][j];
            }
        }
    }
    
    // Operador de Asignación (REGLA DE LOS CINCO - 3/5)
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
    
    // Constructor de Movimiento (REGLA DE LOS CINCO - 4/5)
    MatrizDinamica(MatrizDinamica<T>&& otra) : MatrizBase<T>(otra._filas, otra._columnas) {
        _datos = otra._datos;
        otra._datos = nullptr;
        otra._filas = 0;
        otra._columnas = 0;
    }
    
    // Operador de Asignación por Movimiento (REGLA DE LOS CINCO - 5/5)
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
    
    // Implementación de cargarValores
    void cargarValores() {
        cout << "Ingrese los valores para la matriz (" << this->_filas << "x" << this->_columnas << "):" << endl;
        for (int i = 0; i < this->_filas; i++) {
            for (int j = 0; j < this->_columnas; j++) {
                cout << "Elemento [" << i << "][" << j << "]: ";
                cin >> _datos[i][j];
            }
        }
    }
    
    // Método para cargar valores predefinidos (útil para demostración)
    void cargarValoresPredefinidos(T valores[][10], int cols) {
        for (int i = 0; i < this->_filas; i++) {
            for (int j = 0; j < this->_columnas; j++) {
                _datos[i][j] = valores[i][j];
            }
        }
    }
    
    // Implementación de sumar (POLIMORFISMO)
    MatrizBase<T>* sumar(const MatrizBase<T>& otra) const {
        // Verificar dimensiones compatibles
        if (this->_filas != otra.getFilas() || this->_columnas != otra.getColumnas()) {
            cout << "Error: Las matrices deben tener las mismas dimensiones." << endl;
            return nullptr;
        }
        
        // Crear nueva matriz dinámica para el resultado
        MatrizDinamica<T>* resultado = new MatrizDinamica<T>(this->_filas, this->_columnas);
        
        // Realizar la suma elemento por elemento
        for (int i = 0; i < this->_filas; i++) {
            for (int j = 0; j < this->_columnas; j++) {
                resultado->_datos[i][j] = _datos[i][j] + otra.getElemento(i, j);
            }
        }
        
        return resultado;
    }
    
    // Implementación de imprimir
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
    
    // Implementación de getElemento
    T getElemento(int i, int j) const {
        return _datos[i][j];
    }
};

// ============================================================
// CLASE CONCRETA: MatrizEstatica<T, M, N>
// ============================================================
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
        // Verificar dimensiones
        if (M != otra.getFilas() || N != otra.getColumnas()) {
            cout << "Error: Las matrices deben tener las mismas dimensiones." << endl;
            return nullptr;
        }
        
        // Crear nueva matriz dinámica para el resultado
        MatrizDinamica<T>* resultado = new MatrizDinamica<T>(M, N);
        
        // Realizar la suma
        for (int i = 0; i < M; i++) {
            for (int j = 0; j < N; j++) {
                T suma = _datos[i][j] + otra.getElemento(i, j);
                // Accedemos directamente al elemento usando un cast
                MatrizDinamica<T>* temp = dynamic_cast<MatrizDinamica<T>*>(resultado);
                // Como no podemos acceder directamente a _datos privado,
                // usamos un método auxiliar o hacemos friend
                // Por simplicidad, reconstruimos la suma completa
            }
        }
        
        // Segunda pasada para llenar resultado (solución práctica)
        for (int i = 0; i < M; i++) {
            for (int j = 0; j < N; j++) {
                // Usamos getElemento y una asignación manual
                T suma = _datos[i][j] + otra.getElemento(i, j);
                // Acceso mediante casting y reconstrucción
                delete resultado;
                resultado = new MatrizDinamica<T>(M, N);
                break;
            }
            break;
        }
        
        // Solución correcta: recrear con valores correctos
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
    
    // Implementación de imprimir
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

// ============================================================
// FUNCIÓN PRINCIPAL
// ============================================================
int main() {
   
    
    return 0;
}